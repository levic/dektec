// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* CDTAPI.h *#*#*#*#*#*#*#*#*# (C) 2022-2023 DekTec
//
// CDTAPI - C API for DekTec SDI
//
#ifndef CDTAPI_H
#define CDTAPI_H

#include "CDTAPI_Version.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__cplusplus) || defined(DEFINE_MACROS)
// clang-format off

#define DTAPI_OK                    0
#define DTAPI_OK_FAILSAFE           1
#define DTAPI_OK_PENDING            2
#define DTAPI_OK_PARTIAL_DEV        3
#define DTAPI_OK_TAINTED_FW         4
#define DTAPI_OK_OBSOLETE_FW        5
#define DTAPI_OK_UNLIC_FWVARIANT    6
#define DTAPI_E                     0x1000
#define DTAPI_E_ATTACHED            (DTAPI_E + 0)
#define DTAPI_E_BUF_TOO_SMALL       (DTAPI_E + 1)
#define DTAPI_E_DEV_DRIVER          (DTAPI_E + 2)
#define DTAPI_E_EEPROM_FULL         (DTAPI_E + 3)
#define DTAPI_E_EEPROM_READ         (DTAPI_E + 4)
#define DTAPI_E_EEPROM_WRITE        (DTAPI_E + 5)
#define DTAPI_E_EEPROM_FORMAT       (DTAPI_E + 6)
#define DTAPI_E_FIFO_FULL           (DTAPI_E + 7)
#define DTAPI_E_IN_USE              (DTAPI_E + 8)
#define DTAPI_E_INVALID_BUF         (DTAPI_E + 9)
#define DTAPI_E_INVALID_FLAGS       (DTAPI_E + 11)
#define DTAPI_E_INVALID_MODE        (DTAPI_E + 12)
#define DTAPI_E_INVALID_RATE        (DTAPI_E + 13)
#define DTAPI_E_INVALID_SIZE        (DTAPI_E + 14)
#define DTAPI_E_KEYWORD             (DTAPI_E + 15)
#define DTAPI_E_NO_DEVICE           (DTAPI_E + 16)
#define DTAPI_E_NO_LOOPBACK         (DTAPI_E + 17)
#define DTAPI_E_NO_SUCH_DEVICE      (DTAPI_E + 18)
#define DTAPI_E_NO_SUCH_OUTPUT      (DTAPI_E + 19)
#define DTAPI_E_NO_DT_OUTPUT        (DTAPI_E + 20)
#define DTAPI_E_NO_TS_OUTPUT        (DTAPI_E + 20)
#define DTAPI_E_NOT_ATTACHED        (DTAPI_E + 21)
#define DTAPI_E_NOT_FOUND           (DTAPI_E + 22)
#define DTAPI_E_NOT_SUPPORTED       (DTAPI_E + 23)
#define DTAPI_E_DEVICE              (DTAPI_E + 24)
#define DTAPI_E_TOO_LONG            (DTAPI_E + 25)
#define DTAPI_E_UNDERFLOW           (DTAPI_E + 26)
#define DTAPI_E_NO_SUCH_INPUT       (DTAPI_E + 27)
#define DTAPI_E_NO_DT_INPUT         (DTAPI_E + 28)
#define DTAPI_E_NO_TS_INPUT         (DTAPI_E + 28)
#define DTAPI_E_DRIVER_INCOMP       (DTAPI_E + 29)
#define DTAPI_E_INTERNAL            (DTAPI_E + 30)
#define DTAPI_E_OUT_OF_MEM          (DTAPI_E + 31)
#define DTAPI_E_INVALID_J83ANNEX    (DTAPI_E + 32)
#define DTAPI_E_IDLE                (DTAPI_E + 33)
#define DTAPI_E_INSUF_LOAD          (DTAPI_E + 34)
#define DTAPI_E_INVALID_BANDWIDTH   (DTAPI_E + 35)
#define DTAPI_E_INVALID_CONSTEL     (DTAPI_E + 36)
#define DTAPI_E_INVALID_GUARD       (DTAPI_E + 37)
#define DTAPI_E_INVALID_INTERLVNG   (DTAPI_E + 38)
#define DTAPI_E_INVALID_TRANSMODE   (DTAPI_E + 39)
#define DTAPI_E_INVALID_TSTYPE      (DTAPI_E + 40)
#define DTAPI_E_NO_IPPARS           (DTAPI_E + 41)
#define DTAPI_E_NO_TSRATE           (DTAPI_E + 42)
#define DTAPI_E_NOT_IDLE            (DTAPI_E + 43)
#define DTAPI_E_INVALID_ARG         (DTAPI_E + 44)
#define DTAPI_E_NW_DRIVER           (DTAPI_E + 45)
#define DTAPI_E_DST_MAC_ADDR        (DTAPI_E + 46)
#define DTAPI_E_NO_SUCH_PORT        (DTAPI_E + 47)
#define DTAPI_E_WINSOCK             (DTAPI_E + 48)
#define DTAPI_E_MULTICASTJOIN       (DTAPI_E + 49)
#define DTAPI_E_EMBEDDED            (DTAPI_E + 50)
#define DTAPI_E_LOCKED              (DTAPI_E + 51)
#define DTAPI_E_NO_VALID_CALDATA    (DTAPI_E + 52)
#define DTAPI_E_NO_LINK             (DTAPI_E + 53)
#define DTAPI_E_INVALID_HEADER      (DTAPI_E + 54)
#define DTAPI_E_INVALID_PARS        (DTAPI_E + 55)
#define DTAPI_E_NOT_SDI_MODE        (DTAPI_E + 56)
#define DTAPI_E_INCOMP_FRAME        (DTAPI_E + 57)
#define DTAPI_E_UNSUP_CONV          (DTAPI_E + 58)
#define DTAPI_E_OUTBUF_TOO_SMALL    (DTAPI_E + 59)
#define DTAPI_E_CONFIG              (DTAPI_E + 60)
#define DTAPI_E_TIMEOUT             (DTAPI_E + 61)
#define DTAPI_E_INVALID_TIMEOUT     (DTAPI_E + 62)
#define DTAPI_E_INVALID_FHMODE      (DTAPI_E + 63)
#define DTAPI_E_INVALID_PILOTS      (DTAPI_E + 64)
#define DTAPI_E_INVALID_USEFRAMENO  (DTAPI_E + 65)
#define DTAPI_E_SYMRATE_REQD        (DTAPI_E + 66)
#define DTAPI_E_NO_SYMRATE          (DTAPI_E + 67)
#define DTAPI_E_INVALID_NUMSEGM     (DTAPI_E + 68)
#define DTAPI_E_INVALID_NUMTAPS     (DTAPI_E + 69)
#define DTAPI_E_COMMUNICATION       (DTAPI_E + 70)
#define DTAPI_E_BIND                (DTAPI_E + 71)
#define DTAPI_E_FRAME_INTERVAL      (DTAPI_E + 72)
#define DTAPI_E_INVALID_BWT_EXT     (DTAPI_E + 73)
#define DTAPI_E_INVALID_FFTMODE     (DTAPI_E + 74)
#define DTAPI_E_INVALID_NUMDTSYM    (DTAPI_E + 75)
#define DTAPI_E_INVALID_NUMT2FRM    (DTAPI_E + 76)
#define DTAPI_E_INVALID_SUBCH       (DTAPI_E + 77)
#define DTAPI_E_INVALID_TIME_IL     (DTAPI_E + 78)
#define DTAPI_E_NUM_PLP             (DTAPI_E + 79)
#define DTAPI_E_PLP_NUMBLOCKS       (DTAPI_E + 80)
#define DTAPI_E_NUMPLPS_MUSTBE_1    (DTAPI_E + 81)
#define DTAPI_E_INBAND              (DTAPI_E + 82)
#define DTAPI_E_ISSY                (DTAPI_E + 83)
#define DTAPI_E_OTHER_PLP_IN_BAND   (DTAPI_E + 84)
#define DTAPI_E_CM_NUMPATHS         (DTAPI_E + 85)
#define DTAPI_E_PILOT_PATTERN       (DTAPI_E + 86)
#define DTAPI_E_SUBSLICES           (DTAPI_E + 87)
#define DTAPI_E_NO_GENREF           (DTAPI_E + 88)
#define DTAPI_E_TI_MEM_OVF          (DTAPI_E + 89)
#define DTAPI_E_FEF                 (DTAPI_E + 90)
#define DTAPI_E_UNSUP_FORMAT        (DTAPI_E + 91)
#define DTAPI_E_OUT_OF_SYNC         (DTAPI_E + 92)
#define DTAPI_E_NO_FRAME            (DTAPI_E + 93)
#define DTAPI_E_NO_SUCH_DATA        (DTAPI_E + 94)
#define DTAPI_E_INVALID_TYPE        (DTAPI_E + 95)
#define DTAPI_E_INVALID_MODPARS     (DTAPI_E + 96)
#define DTAPI_E_BIAS_BAL_CELLS      (DTAPI_E + 97)
#define DTAPI_E_COMMON_PLP_COUNT    (DTAPI_E + 98)
#define DTAPI_E_PLP_ID              (DTAPI_E + 99)
#define DTAPI_E_BUFS                (DTAPI_E + 100)
#define DTAPI_E_FIXED_CELL_PARS     (DTAPI_E + 101)
#define DTAPI_E_CM_CHANNEL          (DTAPI_E + 102)
#define DTAPI_E_INVALID_FIFO_IDX    (DTAPI_E + 103)
#define DTAPI_E_INVALID_INP_TYPE    (DTAPI_E + 104)
#define DTAPI_E_INVALID_OUTP_TYPE   (DTAPI_E + 105)
#define DTAPI_E_INVALID_START_FREQ  (DTAPI_E + 106)
#define DTAPI_E_DSLICE_TUNE_POS     (DTAPI_E + 107)
#define DTAPI_E_DSLICE_OFFSETS      (DTAPI_E + 108)
#define DTAPI_E_DSLICE_OVERLAP      (DTAPI_E + 109)
#define DTAPI_E_NOTCH_OFFSETS       (DTAPI_E + 110)
#define DTAPI_E_PLP_BUNDLED         (DTAPI_E + 111)
#define DTAPI_E_BROADBAND_NOTCH     (DTAPI_E + 112)
#define DTAPI_E_L1_PART2_TOO_LONG   (DTAPI_E + 113)
#define DTAPI_E_DSLICE_T1_NDP       (DTAPI_E + 114)
#define DTAPI_E_DSLICE_T1_TSRATE    (DTAPI_E + 115)
#define DTAPI_E_CONNECT_TO_SERVICE  (DTAPI_E + 116)
#define DTAPI_E_INVALID_SYMRATE     (DTAPI_E + 117)
#define DTAPI_E_MODPARS_NOT_SET     (DTAPI_E + 118)
#define DTAPI_E_SERVICE_INCOMP      (DTAPI_E + 119)
#define DTAPI_E_INVALID_LEVEL       (DTAPI_E + 120)
#define DTAPI_E_MODTYPE_UNSUP       (DTAPI_E + 121)
#define DTAPI_E_I2C_LOCK_TIMEOUT    (DTAPI_E + 122)
#define DTAPI_E_INVALID_FREQ        (DTAPI_E + 123)
#define DTAPI_E_INVALID_TSRATESEL   (DTAPI_E + 124)
#define DTAPI_E_INVALID_SPICLKSEL   (DTAPI_E + 125)
#define DTAPI_E_INVALID_SPIMODE     (DTAPI_E + 126)
#define DTAPI_E_NOT_INITIALIZED     (DTAPI_E + 127)
#define DTAPI_E_NOT_LOCKED          (DTAPI_E + 128)
#define DTAPI_E_NO_PERMISSION       (DTAPI_E + 129)
#define DTAPI_E_CANCELLED           (DTAPI_E + 130)
#define DTAPI_E_OUT_OF_RESOURCES    (DTAPI_E + 131)
#define DTAPI_E_LISTEN              (DTAPI_E + 132)
#define DTAPI_E_INVALID_STREAMFMT   (DTAPI_E + 133)
#define DTAPI_E_EVENT_POWER         (DTAPI_E + 134)
#define DTAPI_E_EVENT_REMOVAL       (DTAPI_E + 135)
#define DTAPI_E_UNSUP_ROLLOFF       (DTAPI_E + 136)
#define DTAPI_E_T2_LITE             (DTAPI_E + 137)
#define DTAPI_E_COMP_OVERLAP        (DTAPI_E + 138)
#define DTAPI_E_MULTI_COMPS         (DTAPI_E + 139)
#define DTAPI_E_INVALID_ISI         (DTAPI_E + 140)
#define DTAPI_E_FIRMW_INCOMP        (DTAPI_E + 141)
#define DTAPI_E_INVALID_MODTYPE     (DTAPI_E + 142)
#define DTAPI_E_NO_VIDSTD           (DTAPI_E + 143)
#define DTAPI_E_INVALID_VIDSTD      (DTAPI_E + 144)
#define DTAPI_E_INVALID_AUDSTD      (DTAPI_E + 145)
#define DTAPI_E_INVALID_SCALING     (DTAPI_E + 146)
#define DTAPI_E_INVALID_ROW         (DTAPI_E + 147)
#define DTAPI_E_NOT_STARTED         (DTAPI_E + 148)
#define DTAPI_E_STARTED             (DTAPI_E + 149)
#define DTAPI_E_INVALID_LINE        (DTAPI_E + 150)
#define DTAPI_E_INVALID_STREAM      (DTAPI_E + 151)
#define DTAPI_E_INVALID_ANC         (DTAPI_E + 152)
#define DTAPI_E_INVALID_FRAME       (DTAPI_E + 153)
#define DTAPI_E_NOT_IMPLEMENTED     (DTAPI_E + 154)
#define DTAPI_E_INVALID_CHANNEL     (DTAPI_E + 155)
#define DTAPI_E_INVALID_GROUP       (DTAPI_E + 156)
#define DTAPI_E_INVALID_FORMAT      (DTAPI_E + 157)
#define DTAPI_E_INVALID_FIELD       (DTAPI_E + 158)
#define DTAPI_E_BUF_TOO_LARGE       (DTAPI_E + 159)
#define DTAPI_E_INVALID_DELAY       (DTAPI_E + 160)
#define DTAPI_E_EXCL_MANDATORY      (DTAPI_E + 161)
#define DTAPI_E_INVALID_ROLLOFF     (DTAPI_E + 162)
#define DTAPI_E_CM_UNSUP            (DTAPI_E + 163)
#define DTAPI_E_I2C                 (DTAPI_E + 164)
#define DTAPI_E_STATE               (DTAPI_E + 165)
#define DTAPI_E_NO_LOCK             (DTAPI_E + 166)
#define DTAPI_E_RANGE               (DTAPI_E + 167)
#define DTAPI_E_INVALID_T2PROFILE   (DTAPI_E + 168)
#define DTAPI_E_DSLICE_ID           (DTAPI_E + 169)
#define DTAPI_E_EXCL_ACCESS_REQD    (DTAPI_E + 170)
#define DTAPI_E_CHAN_ALREADY_ADDED  (DTAPI_E + 171)
#define DTAPI_E_LAYER_ID            (DTAPI_E + 172)
#define DTAPI_E_INVALID_FECMODE     (DTAPI_E + 173)
#define DTAPI_E_INVALID_PORT        (DTAPI_E + 174)
#define DTAPI_E_INVALID_PROTOCOL    (DTAPI_E + 175)
#define DTAPI_E_INVALID_FEC_MATRIX  (DTAPI_E + 176)
#define DTAPI_E_INVALID_IP_ADDR     (DTAPI_E + 177)
#define DTAPI_E_INVALID_SRCIP_ADDR  (DTAPI_E + 178)
#define DTAPI_E_IPV6_NOT_SUPPORTED  (DTAPI_E + 179)
#define DTAPI_E_INVALID_DIFFSERV    (DTAPI_E + 180)
#define DTAPI_E_INVALID_FOR_ACM     (DTAPI_E + 181)
#define DTAPI_E_NWAP_DRIVER         (DTAPI_E + 182)
#define DTAPI_E_INIT_ERROR          (DTAPI_E + 183)
#define DTAPI_E_NOT_USB3            (DTAPI_E + 184)
#define DTAPI_E_INSUF_BW            (DTAPI_E + 185)
#define DTAPI_E_NO_ANC_DATA         (DTAPI_E + 186)
#define DTAPI_E_MATRIX_HALTED       (DTAPI_E + 187)
#define DTAPI_E_VLAN_NOT_FOUND      (DTAPI_E + 188)
#define DTAPI_E_NO_ADAPTER_IP_ADDR  (DTAPI_E + 189)
#define DTAPI_E_INVALID_BTYPE       (DTAPI_E + 190)
#define DTAPI_E_INVALID_PARTIAL     (DTAPI_E + 191)
#define DTAPI_E_INVALID_NUMTS       (DTAPI_E + 192)
#define DTAPI_E_INVALID             (DTAPI_E + 193)
#define DTAPI_E_NO_RS422            (DTAPI_E + 194)
#define DTAPI_E_FECFRAMESIZE        (DTAPI_E + 195)
#define DTAPI_E_SFN_NOT_SUPPORTED   (DTAPI_E + 196)
#define DTAPI_E_SFN_INVALID_MODE    (DTAPI_E + 197)
#define DTAPI_E_SFN_INVALID_OFFSET  (DTAPI_E + 198)
#define DTAPI_E_SFN_DISABLED        (DTAPI_E + 199)
#define DTAPI_E_SFN_INVALID_TIMEDIFF (DTAPI_E + 200)
#define DTAPI_E_NO_GPSCLKREF        (DTAPI_E + 201)
#define DTAPI_E_NO_GPSSYNC          (DTAPI_E + 202)
#define DTAPI_E_INVALID_PROFILE     (DTAPI_E + 203)
#define DTAPI_E_INVALID_LINKSTD     (DTAPI_E + 204)
#define DTAPI_E_FRAMERATE_MISMATCH  (DTAPI_E + 205)
#define DTAPI_E_CID_INVALID_ID      (DTAPI_E + 206)
#define DTAPI_E_CID_INVALID_INFO    (DTAPI_E + 207)
#define DTAPI_E_CID_INVALID_FORMAT  (DTAPI_E + 208)
#define DTAPI_E_CID_NOT_SUPPORTED   (DTAPI_E + 209)
#define DTAPI_E_INVALID_SAMPRATE    (DTAPI_E + 210)
#define DTAPI_E_MULTIMOD_UNSUP      (DTAPI_E + 211)
#define DTAPI_E_NUM_CHAN            (DTAPI_E + 212)
#define DTAPI_E_INVALID_TIME        (DTAPI_E + 213)
#define DTAPI_E_INVALID_LINK        (DTAPI_E + 214)
#define DTAPI_E_TUNING              (DTAPI_E + 215)
#define DTAPI_E_BUSY                (DTAPI_E + 216)
#define DTAPI_E_ENC_TYPE_NOTSET     (DTAPI_E + 217)
#define DTAPI_E_INITIALIZING        (DTAPI_E + 218)
#define DTAPI_E_INVALID_ENC_TYPE    (DTAPI_E + 219)
#define DTAPI_E_LICENSE             (DTAPI_E + 220)
#define DTAPI_E_NO_ENCODER          (DTAPI_E + 221)
#define DTAPI_E_NO_POWER            (DTAPI_E + 222)
#define DTAPI_E_PASSTHROUGH_INV     (DTAPI_E + 223)
#define DTAPI_E_PASSTHROUGH_ONLY    (DTAPI_E + 224)
#define DTAPI_E_RX_RATE_OVF         (DTAPI_E + 225)
#define DTAPI_E_IN_ERROR_STATE      (DTAPI_E + 226)
#define DTAPI_E_XML_SYNTAX          (DTAPI_E + 227)
#define DTAPI_E_XML_ELEM            (DTAPI_E + 228)
#define DTAPI_E_FAN_FAIL            (DTAPI_E + 229)
#define DTAPI_E_RESTART_REQD        (DTAPI_E + 230)
#define DTAPI_E_TOO_MANY_SEGM       (DTAPI_E + 231)
#define DTAPI_E_FILE_OPEN           (DTAPI_E + 232)
#define DTAPI_E_INVALID_EAS         (DTAPI_E + 233)
#define DTAPI_E_INVALID_CRED        (DTAPI_E + 234)
#define DTAPI_E_INVALID_PARITY      (DTAPI_E + 235)
#define DTAPI_E_INVALID_PAPR        (DTAPI_E + 236)
#define DTAPI_E_INVALID_FRAMEMODE   (DTAPI_E + 237)
#define DTAPI_E_INVALID_FRAMELENGTH (DTAPI_E + 238)
#define DTAPI_E_NUM_SUBFRAMES       (DTAPI_E + 239)
#define DTAPI_E_PILOT_BOOST         (DTAPI_E + 240)
#define DTAPI_E_NUM_SYMBOLS         (DTAPI_E + 241)
#define DTAPI_E_INVALID_LAYER       (DTAPI_E + 242)
#define DTAPI_E_INVALID_CODERATE    (DTAPI_E + 243)
#define DTAPI_E_INVALID_FECTYPE     (DTAPI_E + 244)
#define DTAPI_E_INVALID_NUM_INPUTS  (DTAPI_E + 245)
#define DTAPI_E_INVALID_VERSION     (DTAPI_E + 246)
#define DTAPI_E_INVALID_LDM_LEVEL   (DTAPI_E + 247)
#define DTAPI_E_INVALID_MISO        (DTAPI_E + 248)
#define DTAPI_E_INVALID_PLP_TYPE    (DTAPI_E + 249)
#define DTAPI_E_NUM_SUBSLICES       (DTAPI_E + 250)
#define DTAPI_E_SUBSLICE_INTERVAL   (DTAPI_E + 251)
#define DTAPI_E_INVALID_HTI_PARS    (DTAPI_E + 252)
#define DTAPI_E_PREAMBLE_PAR_COMBI  (DTAPI_E + 253)
#define DTAPI_E_INVALID_PLP_SIZE    (DTAPI_E + 254)
#define DTAPI_E_INVALID_PLP_START   (DTAPI_E + 255)
#define DTAPI_E_INVALID_PLP_REF     (DTAPI_E + 256)
#define DTAPI_E_INVALID_TXID_INJ    (DTAPI_E + 257)
#define DTAPI_E_INVALID_TXID        (DTAPI_E + 258)
#define DTAPI_E_INVALID_BSID        (DTAPI_E + 259)
#define DTAPI_E_INVALID_BONDING     (DTAPI_E + 260)
#define DTAPI_E_UNSUP_CPP_VERS      (DTAPI_E + 261)
#define DTAPI_E_STAT_NOTAVAIL       (DTAPI_E + 262)
#define DTAPI_E_TAINTED_FW          (DTAPI_E + 263)
#define DTAPI_E_OBSOLETE_FW         (DTAPI_E + 264)
#define DTAPI_E_PLP_COLLISION       (DTAPI_E + 265)
#define DTAPI_E_PLP_NOT_FOUND       (DTAPI_E + 266)
#define DTAPI_E_CONFIG_AUDIO        (DTAPI_E + 267)
#define DTAPI_E_CONFIG_AUX          (DTAPI_E + 268)
#define DTAPI_E_CONFIG_AUX_ANC      (DTAPI_E + 269)
#define DTAPI_E_CONFIG_AUX_LINE21   (DTAPI_E + 270)
#define DTAPI_E_CONFIG_AUX_RAWVBI   (DTAPI_E + 271)
#define DTAPI_E_CONFIG_AUX_TELETEXT (DTAPI_E + 272)
#define DTAPI_E_CONFIG_AUX_VIDINDEX (DTAPI_E + 273)
#define DTAPI_E_CONFIG_AUX_VITC     (DTAPI_E + 274)
#define DTAPI_E_CONFIG_AUX_VPID     (DTAPI_E + 275)
#define DTAPI_E_CONFIG_AUX_WSS      (DTAPI_E + 276)
#define DTAPI_E_CONFIG_RAW          (DTAPI_E + 277)
#define DTAPI_E_CONFIG_RAW_SDI      (DTAPI_E + 278)
#define DTAPI_E_CONFIG_VIDEO        (DTAPI_E + 279)
#define DTAPI_E_CONFIG_VIDEO_WEAVE  (DTAPI_E + 280)

// I/O configuration groups
#define DTAPI_IOCONFIG_IODIR             0       // I/O direction
#define DTAPI_IOCONFIG_IOSTD             1       // I/O standard
#define DTAPI_IOCONFIG_IODOWNSCALE       2       // I/O down-scaling
#define DTAPI_IOCONFIG_PWRMODE           3       // Power mode
#define DTAPI_IOCONFIG_RFCLKSEL          4       // RF clock source selection
#define DTAPI_IOCONFIG_SPICLKSEL         5       // Parallel port clock source selection
#define DTAPI_IOCONFIG_SPIMODE           6       // Parallel port mode
#define DTAPI_IOCONFIG_SPISTD            7       // Parallel port I/O standard
#define DTAPI_IOCONFIG_TSRATESEL         8       // Transport-stream rate selection
#define DTAPI_IOCONFIG_TODREFSEL         9       // TimeOfDay reference selection

// I/O configuration groups - Boolean I/O
#define DTAPI_IOCONFIG_AUTOBFGEN         10      // Automatic black-frame generation
#define DTAPI_IOCONFIG_BW                11      // DEPRECATED, do not use
#define DTAPI_IOCONFIG_DMATESTMODE       12      // DMA-rate test mode
#define DTAPI_IOCONFIG_FAILSAFE          13      // A fail-over relay is available
#define DTAPI_IOCONFIG_FRACMODE          14      // Fractional mode is supported
#define DTAPI_IOCONFIG_GENLOCKED         15      // Locked to a genlock reference
#define DTAPI_IOCONFIG_GENREF            16      // Genlock reference input
#define DTAPI_IOCONFIG_SWS2APSK          17      // DVB-S2 APSK mode

// Values for boolean I/O configuration options
#define DTAPI_IOCONFIG_TRUE              18      // Turn I/O capability on
#define DTAPI_IOCONFIG_FALSE             19      // Turn I/O capability off

// Values for group IO_CONFIG_IODIR (I/O direction)
#define DTAPI_IOCONFIG_DISABLED          20      // Port is disabled
#define DTAPI_IOCONFIG_INPUT             21      // Uni-directional input
#define DTAPI_IOCONFIG_INTINPUT          22      // Internal input port
#define DTAPI_IOCONFIG_INTOUTPUT         23      // Internal output port
#define DTAPI_IOCONFIG_MONITOR           24      // Monitor of input or output
#define DTAPI_IOCONFIG_OUTPUT            25      // Uni-directional output

// SubValues for group DTAPI_IOCONFIG_IODIR, value DTAPI_IOCONFIG_INPUT
#define DTAPI_IOCONFIG_SHAREDANT         26      // Get antenna signal from another port

// SubValues for group DTAPI_IOCONFIG_IODIR, value DTAPI_IOCONFIG_INTOUTPUT
#define DTAPI_IOCONFIG_DBLBUF            27      // Double buffered output
#define DTAPI_IOCONFIG_LOOPS2L3          28      // Loop-through of DVB-S2 in L3-frames
#define DTAPI_IOCONFIG_LOOPS2TS          29      // Loop-through of an DVB-S(2) input
#define DTAPI_IOCONFIG_LOOPTHR           30      // Loop-through of another input

// Values for group IO_CONFIG_IOSTD (I/O standard)
#define DTAPI_IOCONFIG_12GSDI            31      // 12G-SDI
#define DTAPI_IOCONFIG_3GSDI             32      // 3G-SDI
#define DTAPI_IOCONFIG_6GSDI             33      // 6G-SDI
#define DTAPI_IOCONFIG_ASI               34      // DVB-ASI transport stream
#define DTAPI_IOCONFIG_AVENC             35      // Audio/video encoder
#define DTAPI_IOCONFIG_DEKTECST          36      // DekTec Streaming-data Interface
#define DTAPI_IOCONFIG_DEMOD             37      // Demodulation
#define DTAPI_IOCONFIG_GPSTIME           38      // 1PPS and 10MHz GPS-clock input
#define DTAPI_IOCONFIG_HDMI              39      // HDMI
#define DTAPI_IOCONFIG_HDSDI             40      // HD-SDI
#define DTAPI_IOCONFIG_IFADC             41      // IF A/D converter
#define DTAPI_IOCONFIG_IP                42      // Transport stream over IP
#define DTAPI_IOCONFIG_MOD               43      // Modulator output
#define DTAPI_IOCONFIG_PHASENOISE        44      // Phase noise injection
#define DTAPI_IOCONFIG_RS422             45      // RS422 port
#define DTAPI_IOCONFIG_SDIRX             46      // SDI receiver
#define DTAPI_IOCONFIG_SDI               47      // SD-SDI
#define DTAPI_IOCONFIG_SPI               48      // DVB-SPI transport stream
#define DTAPI_IOCONFIG_SPISDI            49      // SD-SDI on a parallel port

// SubValues for group DTAPI_IOCONFIG_IOSTD, value DTAPI_IOCONFIG_12GSDI
#define DTAPI_IOCONFIG_2160P50           50      // 2160p/50 lvl A
#define DTAPI_IOCONFIG_2160P50B          51      // 2160p/50 lvl B
#define DTAPI_IOCONFIG_2160P59_94        52      // 2160p/59.94 lvl A
#define DTAPI_IOCONFIG_2160P59_94B       53      // 2160p/59.94 lvl B
#define DTAPI_IOCONFIG_2160P60           54      // 2160p/60 lvl A
#define DTAPI_IOCONFIG_2160P60B          55      // 2160p/60 lvl B

// SubValues for group DTAPI_IOCONFIG_IOSTD, value DTAPI_IOCONFIG_3GSDI
#define DTAPI_IOCONFIG_1080P50           56      // 1080p/50 lvl A
#define DTAPI_IOCONFIG_1080P50B          57      // 1080p/50 lvl B
#define DTAPI_IOCONFIG_1080P59_94        58      // 1080p/59.94 lvl A
#define DTAPI_IOCONFIG_1080P59_94B       59      // 1080p/59.94 lvl B
#define DTAPI_IOCONFIG_1080P60           60      // 1080p/60 lvl A
#define DTAPI_IOCONFIG_1080P60B          61      // 1080p/60 lvl B

// SubValues for group DTAPI_IOCONFIG_IOSTD, value DTAPI_IOCONFIG_6GSDI
#define DTAPI_IOCONFIG_2160P23_98        62      // 2160p/23.98
#define DTAPI_IOCONFIG_2160P24           63      // 2160p/24
#define DTAPI_IOCONFIG_2160P25           64      // 2160p/25
#define DTAPI_IOCONFIG_2160P29_97        65      // 2160p/29.97
#define DTAPI_IOCONFIG_2160P30           66      // 2160p/30

// SubValues for group DTAPI_IOCONFIG_IOSTD, value DTAPI_IOCONFIG_HDSDI
#define DTAPI_IOCONFIG_1080I50           67      // 1080i/50
#define DTAPI_IOCONFIG_1080I59_94        68      // 1080i/59.94
#define DTAPI_IOCONFIG_1080I60           69      // 1080i/60
#define DTAPI_IOCONFIG_1080P23_98        70      // 1080p/23.98
#define DTAPI_IOCONFIG_1080P24           71      // 1080p/24
#define DTAPI_IOCONFIG_1080P25           72      // 1080p/25
#define DTAPI_IOCONFIG_1080P29_97        73      // 1080p/29.97
#define DTAPI_IOCONFIG_1080P30           74      // 1080p/30
#define DTAPI_IOCONFIG_1080PSF23_98      75      // 1080psf/23.98
#define DTAPI_IOCONFIG_1080PSF24         76      // 1080psf/24
#define DTAPI_IOCONFIG_1080PSF25         77      // 1080psf/25
#define DTAPI_IOCONFIG_1080PSF29_97      78      // 1080psf/29.97
#define DTAPI_IOCONFIG_1080PSF30         79      // 1080psf/30
#define DTAPI_IOCONFIG_720P23_98         80      // 720p/23.98
#define DTAPI_IOCONFIG_720P24            81      // 720p/24
#define DTAPI_IOCONFIG_720P25            82      // 720p/25
#define DTAPI_IOCONFIG_720P29_97         83      // 720p/29.97
#define DTAPI_IOCONFIG_720P30            84      // 720p/30
#define DTAPI_IOCONFIG_720P50            85      // 720p/50
#define DTAPI_IOCONFIG_720P59_94         86      // 720p/59.94
#define DTAPI_IOCONFIG_720P60            87      // 720p/60

// SubValues for group DTAPI_IOCONFIG_IOSTD, value DTAPI_IOCONFIG_SDI
#define DTAPI_IOCONFIG_525I59_94         88      // 525i/59.94
#define DTAPI_IOCONFIG_625I50            89      // 625i/50

// SubValues for group DTAPI_IOCONFIG_IOSTD, value DTAPI_IOCONFIG_SPISDI
#define DTAPI_IOCONFIG_SPI525I59_94      90      // SPI 525i/59.94
#define DTAPI_IOCONFIG_SPI625I50         91      // SPI 625i/50

// Values for group IO_CONFIG_IODOWNSCALE (I/O down-scaling)
#define DTAPI_IOCONFIG_SCALE_12GTO3G     92      // Downscale 12G-SDI to 3G-SDI
#define DTAPI_IOCONFIG_SCALE_BYPASS      93      // Bypass the downscaler

// Values for group IO_CONFIG_PWRMODE (Power mode)
#define DTAPI_IOCONFIG_MODHQ             94      // High-quality modulation
#define DTAPI_IOCONFIG_LOWPWR            95      // Low-power mode

// Values for group IO_CONFIG_RFCLKSEL (RF clock source selection)
#define DTAPI_IOCONFIG_RFCLKEXT          96      // External RF clock input
#define DTAPI_IOCONFIG_RFCLKINT          97      // Internal RF clock reference

// Values for group IO_CONFIG_SPICLKSEL (Parallel port clock source selection)
#define DTAPI_IOCONFIG_SPICLKEXT         98      // External clock input
#define DTAPI_IOCONFIG_SPICLKINT         99      // Internal clock reference

// Values for group IO_CONFIG_SPIMODE (Parallel port mode)
#define DTAPI_IOCONFIG_SPIFIXEDCLK       100     // SPI fixed clock with valid signal
#define DTAPI_IOCONFIG_SPIDVBMODE        101     // SPI DVB mode
#define DTAPI_IOCONFIG_SPISER8B          102     // SPI serial 8-bit mode
#define DTAPI_IOCONFIG_SPISER10B         103     // SPI serial 10-bit mode

// Values for group IO_CONFIG_SPISTD (Parallel port I/O standard)
#define DTAPI_IOCONFIG_SPILVDS1          104     // LVDS1
#define DTAPI_IOCONFIG_SPILVDS2          105     // LVDS2
#define DTAPI_IOCONFIG_SPILVTTL          106     // LVTTL

// Values for group IO_CONFIG_TSRATESEL (Transport-stream rate selection)
#define DTAPI_IOCONFIG_EXTTSRATE         107     // External TS rate clock input
#define DTAPI_IOCONFIG_EXTRATIO          108     // External TS rate clock with ratio
#define DTAPI_IOCONFIG_INTTSRATE         109     // Internal TS rate clock reference
#define DTAPI_IOCONFIG_LOCK2INP          110     // Lock TS rate to input port

// Values for group IO_CONFIG_TODREFSEL (TimeOfDay reference selection)
#define DTAPI_IOCONFIG_TODREF_INTERNAL   111     // Internal clock reference
#define DTAPI_IOCONFIG_TODREF_STEADYCLOCK 112    // Steady clock reference

// Transmit control
#define DTAPI_TXCTRL_IDLE           1
#define DTAPI_TXCTRL_HOLD           2
#define DTAPI_TXCTRL_SEND           3

// Receive Control
#define DTAPI_RXCTRL_IDLE           0
#define DTAPI_RXCTRL_RCV            1

// Transmit status flags
#define DTAPI_TX_FIFO_UFL           0x0002
#define DTAPI_TX_SYNC_ERR           0x0004
#define DTAPI_TX_READBACK_ERR       0x0008
#define DTAPI_TX_TARGET_ERR         0x0010
#define DTAPI_TX_MUX_OVF            0x0020
#define DTAPI_TX_FIFO_OVF           0x0020
#define DTAPI_TX_LINK_ERR           0x0040
#define DTAPI_TX_DATA_ERR           0x0080
#define DTAPI_TX_CPU_UFL            0x0100
#define DTAPI_TX_DMA_UFL            0x0200

// Video standards
#define DTAPI_VIDSTD_UNKNOWN        -1
#define DTAPI_VIDSTD_525I59_94      88
#define DTAPI_VIDSTD_625I50         89
#define DTAPI_VIDSTD_720P23_98      80
#define DTAPI_VIDSTD_720P24         81
#define DTAPI_VIDSTD_720P25         82
#define DTAPI_VIDSTD_720P29_97      83
#define DTAPI_VIDSTD_720P30         84
#define DTAPI_VIDSTD_720P50         85
#define DTAPI_VIDSTD_720P59_94      86
#define DTAPI_VIDSTD_720P60         87
#define DTAPI_VIDSTD_1080P23_98     70
#define DTAPI_VIDSTD_1080P24        71
#define DTAPI_VIDSTD_1080P25        72
#define DTAPI_VIDSTD_1080P29_97     73
#define DTAPI_VIDSTD_1080P30        74
#define DTAPI_VIDSTD_1080PSF23_98   75
#define DTAPI_VIDSTD_1080PSF24      76
#define DTAPI_VIDSTD_1080PSF25      77
#define DTAPI_VIDSTD_1080PSF29_97   78
#define DTAPI_VIDSTD_1080PSF30      79
#define DTAPI_VIDSTD_1080I50        67
#define DTAPI_VIDSTD_1080I59_94     68
#define DTAPI_VIDSTD_1080I60        69
#define DTAPI_VIDSTD_1080P50        56
#define DTAPI_VIDSTD_1080P50B       57
#define DTAPI_VIDSTD_1080P59_94     58
#define DTAPI_VIDSTD_1080P59_94B    59
#define DTAPI_VIDSTD_1080P60        60
#define DTAPI_VIDSTD_1080P60B       61
#define DTAPI_VIDSTD_2160P50        50
#define DTAPI_VIDSTD_2160P50B       51
#define DTAPI_VIDSTD_2160P59_94     52
#define DTAPI_VIDSTD_2160P59_94B    53
#define DTAPI_VIDSTD_2160P60        54
#define DTAPI_VIDSTD_2160P60B       55
#define DTAPI_VIDSTD_2160P23_98     62
#define DTAPI_VIDSTD_2160P24        63
#define DTAPI_VIDSTD_2160P25        64
#define DTAPI_VIDSTD_2160P29_97     65
#define DTAPI_VIDSTD_2160P30        66

// Transmit mode for SDI - Modes
#define DTAPI_TXMODE_SDI            0x1000
#define DTAPI_TXMODE_SDI_MODE_BITS  0x0F00
#define DTAPI_TXMODE_SDI_FULL       (DTAPI_TXMODE_SDI | 0x100)
#define DTAPI_TXMODE_SDI_ACTVID     (DTAPI_TXMODE_SDI | 0x200)
#define DTAPI_TXMODE_SDI_MASK       (DTAPI_TXMODE_SDI | DTAPI_TXMODE_SDI_MODE_BITS)
// Transmit mode for SDI - Flags
#define DTAPI_TXMODE_SDI_HUFFMAN    0x00002000
#define DTAPI_TXMODE_SDI_10B        0x00004000
#define DTAPI_TXMODE_SDI_16B        0x00008000
#define DTAPI_TXMODE_SDI_10B_NBO    0x00010000

// Receive mode for SDI - Modes
#define DTAPI_RXMODE_SDI            0x1000
#define DTAPI_RXMODE_SDI_MODE_BITS  0x0F00
#define DTAPI_RXMODE_SDI_FULL       (DTAPI_RXMODE_SDI | 0x100)
#define DTAPI_RXMODE_SDI_ACTVID     (DTAPI_RXMODE_SDI | 0x200)
#define DTAPI_RXMODE_SDI_RAWDMA     (DTAPI_RXMODE_SDI | 0x400)
#define DTAPI_RXMODE_SDI_MASK       (DTAPI_RXMODE_SDI | DTAPI_RXMODE_SDI_MODE_BITS)
// Receive mode for SDI - Flags
#define DTAPI_RXMODE_SDI_HUFFMAN    0x00002000
#define DTAPI_RXMODE_SDI_10B        0x00004000
#define DTAPI_RXMODE_SDI_16B        0x00008000
#define DTAPI_RXMODE_SDI_10B_NBO    0x00010000
#define DTAPI_RXMODE_SDI_STAT       0x00020000

// Receiver status flags
#define DTAPI_RX_FIFO_OVF           0x0002
#define DTAPI_RX_SYNC_ERR           0x0004
#define DTAPI_RX_RATE_OVF           0x0008
#define DTAPI_RX_TARGET_ERR         0x0010
#define DTAPI_RX_LINK_ERR           0x0040
#define DTAPI_RX_DATA_ERR           0x0080
#define DTAPI_RX_DRV_BUF_OVF        0x0100
#define DTAPI_RX_SYNTAX_ERR         0x0200

// Transmit status flags
#define DTAPI_TX_FIFO_UFL           0x0002
#define DTAPI_TX_SYNC_ERR           0x0004
#define DTAPI_TX_READBACK_ERR       0x0008
#define DTAPI_TX_TARGET_ERR         0x0010
#define DTAPI_TX_MUX_OVF            0x0020
#define DTAPI_TX_FIFO_OVF           0x0020
#define DTAPI_TX_LINK_ERR           0x0040
#define DTAPI_TX_DATA_ERR           0x0080
#define DTAPI_TX_CPU_UFL            0x0100
#define DTAPI_TX_DMA_UFL            0x0200

// clang-format on
#endif

#define DTAPI_E_EXCEPTION (DTAPI_E + 300) // For AvFifo exceptions see GetLastException

typedef struct DtTimeOfDay
{
    unsigned int Seconds;     // Integer number of seconds part of the TOD time
    unsigned int Nanoseconds; // Number of nanoseconds part of the TOD time
} DtTimeOfDay;

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtHwFuncDesc +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

#define MAX_DEVICE_NAME_SIZE 256
#define MAX_DEVICE_DESC_SIZE 256
typedef struct DtHwFuncDesc
{
    char DeviceName[MAX_DEVICE_NAME_SIZE];
    char Description[MAX_DEVICE_DESC_SIZE];
    int64_t SerialNumber;
    int Port;
    int IsSdi;
    int IsAvFifo;
    int IsInput;
    int IsOutput;
} DtHwFuncDesc;

// unsigned int DtHwFuncDescToString(const DtHwFuncDesc* Desc, char* Buffer, int Size);

unsigned int DtapiHwFuncScan(int NumEntries, int* NumEntriesResult, DtHwFuncDesc* HwFuncs);

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDevice +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

typedef enum DtAspectRatio
{
    DT_AR_UNKNOWN, // Unknown aspect ratio
    DT_AR_4_3,     // 4x3
    DT_AR_16_9,    // 16x9
    DT_AR_14_9     // 14x9
} DtAspectRatio;

typedef struct DtDetVidStd
{
    int VidStd;                // Detected video standard
    int LinkStd;               // Detected link standard
    int LinkNr;                // Link number, -1 for single-link standards
    unsigned int Vpid;         // Raw VPID extracted from stream, 0 if not available
    unsigned int Vpid2;        // Raw VPID from link 2, only for 3G level B signals
    DtAspectRatio AspectRatio; // Picture Aspect Ratio

    // Original properties (i.e. before hardware preprocessing, such as scaling)
    int OriginalVidStd;  // Video standard before preprocessing
    int OriginalLinkStd; // Link standard before preprocessing
} DtDetVidStd;

typedef struct DtDeviceC DtDevice;

DtDevice* DtDevice_Alloc(void);
void DtDevice_Free(DtDevice* Device);
void DtDevice_Freep(DtDevice** Device);

unsigned int DtDevice_AttachToSerial(DtDevice* Device, int64_t SerialNumber);
unsigned int DtDevice_Detach(DtDevice* Device);
unsigned int DtDevice_SetIoConfig(DtDevice* Device, int Port, int Group, int Value, int SubValue);
unsigned int DtDevice_SetToOutput(DtDevice* Device, int Port);
unsigned int DtDevice_SetToInput(DtDevice* Device, int Port);

DtDetVidStd DtDevice_WaitForSignal(DtDevice* Device, int Port);
unsigned int DtDevice_DetectVidStd(DtDevice* Device, int Port, int* VidStd);

unsigned int DtDevice_GetTimeOfDay(const DtDevice* Device, DtTimeOfDay* TimeOfDay);

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtInpChannel +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

typedef struct DtInpChannelC DtInpChannel;

DtInpChannel* DtInpChannel_Alloc(void);
void DtInpChannel_Free(DtInpChannel* InpChannel);
void DtInpChannel_Freep(DtInpChannel** InpChannel);

unsigned int DtInpChannel_AttachToPort(DtInpChannel* InpChannel, DtDevice* Device, int Port);
unsigned int DtInpChannel_ClearFifo(DtInpChannel* InpChannel);
unsigned int DtInpChannel_ClearFlags(DtInpChannel* InpChannel, int Latched);
unsigned int DtInpChannel_Detach(DtInpChannel* InpChannel, int DetachMode);
unsigned int DtInpChannel_DetectIoStd(DtInpChannel* InpChannel, int* Value, int* SubValue);
unsigned int DtInpChannel_GetFifoLoad(DtInpChannel* InpChannel, int* FifoLoad);
unsigned int DtInpChannel_GetMaxFifoSize(DtInpChannel* InpChannel, int* MaxFifoSize);
unsigned int DtInpChannel_GetFlags(DtInpChannel* InpChannel, int* Flags, int* Latched);
unsigned int DtInpChannel_SetIoConfig(DtInpChannel* InpChannel, int Group, int Value, int SubValue);
unsigned int DtInpChannel_SetRxControl(DtInpChannel* InpChannel, int RxControl);
unsigned int DtInpChannel_SetRxMode(DtInpChannel* InpChannel, int RxMode);
unsigned int DtInpChannel_ReadFrame(DtInpChannel* InpChannel, char* FrameBuffer, int* FrameSize, int TimeOut);


// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtOutpChannel +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

typedef struct DtOutpChannelC DtOutpChannel;

DtOutpChannel* DtOutpChannel_Alloc(void);
void DtOutpChannel_Free(DtOutpChannel* OutpChannel);
void DtOutpChannel_Freep(DtOutpChannel** OutpChannel);

unsigned int DtOutpChannel_AttachToPort(DtOutpChannel* OutpChannel, DtDevice* Device, int Port);
unsigned int DtOutpChannel_ClearFifo(DtOutpChannel* OutpChannel);
unsigned int DtOutpChannel_Detach(DtOutpChannel* OutpChannel, int DetachMode);
unsigned int DtOutpChannel_GetFifoLoad(DtOutpChannel* OutpChannel, int* FifoLoad);
unsigned int DtOutpChannel_GetFifoSize(DtOutpChannel* OutpChannel, int* FifoSize);
unsigned int DtOutpChannel_GetMaxFifoSize(DtOutpChannel* OutpChannel, int* MaxFifoSize);
unsigned int DtOutpChannel_GetFlags(DtOutpChannel* OutpChannel, int* Status, int* Latched);
unsigned int DtOutpChannel_SetIoConfig(DtOutpChannel* OutpChannel, int Group, int Value, int SubValue);
unsigned int DtOutpChannel_SetTxControl(DtOutpChannel* OutpChannel, int TxControl);
unsigned int DtOutpChannel_SetTxMode(DtOutpChannel* OutpChannel, int TxMode, int StuffMode);
unsigned int DtOutpChannel_Write(DtOutpChannel* OutpChannel, char* Buffer, int NumBytesToWrite);

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Global functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

unsigned int DtapiVidStd2IoStd(int VideoStandard, int LinkStandard, int* Value, int* SubValue);
const char* DtapiResult2Str(unsigned int Result);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // #ifndef CDTAPI_H
