//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIoConfigDefs.h *#*#*#*#*#*#*#*#*#* (C) 2021 DekTec
//
// CapParser - DT_IOCONFIG definitions
//
// THIS FILE IS GENERATED, DO NOT EDIT!!!


// List of DT_IOCONFIG codes, used for SetIoConfig/GetIoConfig
#define DT_IOCONFIG_IODIR         0    // I/O direction
#define DT_IOCONFIG_IOSTD         1    // I/O standard
#define DT_IOCONFIG_IODOWNSCALE   2    // I/O down-scaling
#define DT_IOCONFIG_PWRMODE       3    // Power mode
#define DT_IOCONFIG_RFCLKSEL      4    // RF clock source selection
#define DT_IOCONFIG_SPICLKSEL     5    // Parallel port clock source selection
#define DT_IOCONFIG_SPIMODE       6    // Parallel port mode
#define DT_IOCONFIG_SPISTD        7    // Parallel port I/O standard
#define DT_IOCONFIG_TSRATESEL     8    // Transport-stream rate selection
#define DT_IOCONFIG_AUTOBFGEN     9    // Automatic black-frame generation
#define DT_IOCONFIG_BW            10   // DEPRECATED, do not use
#define DT_IOCONFIG_DMATESTMODE   11   // DMA-rate test mode
#define DT_IOCONFIG_FAILSAFE      12   // A fail-over relay is available
#define DT_IOCONFIG_FRACMODE      13   // Fractional mode is supported
#define DT_IOCONFIG_GENLOCKED     14   // Locked to a genlock reference
#define DT_IOCONFIG_GENREF        15   // Genlock reference input
#define DT_IOCONFIG_SWS2APSK      16   // DVB-S2 APSK mode
#define DT_IOCONFIG_TRUE          17   // Turn I/O capability on
#define DT_IOCONFIG_FALSE         18   // Turn I/O capability off
#define DT_IOCONFIG_DISABLED      19   // Port is disabled
#define DT_IOCONFIG_INPUT         20   // Uni-directional input
#define DT_IOCONFIG_INTINPUT      21   // Internal input port
#define DT_IOCONFIG_INTOUTPUT     22   // Internal output port
#define DT_IOCONFIG_MONITOR       23   // Monitor of input or output
#define DT_IOCONFIG_OUTPUT        24   // Uni-directional output
#define DT_IOCONFIG_SHAREDANT     25   // Get antenna signal from another port
#define DT_IOCONFIG_DBLBUF        26   // Double buffered output
#define DT_IOCONFIG_LOOPS2L3      27   // Loop-through of DVB-S2 in L3-frames
#define DT_IOCONFIG_LOOPS2TS      28   // Loop-through of an DVB-S(2) input
#define DT_IOCONFIG_LOOPTHR       29   // Loop-through of another input
#define DT_IOCONFIG_12GSDI        30   // 12G-SDI
#define DT_IOCONFIG_3GSDI         31   // 3G-SDI
#define DT_IOCONFIG_6GSDI         32   // 6G-SDI
#define DT_IOCONFIG_ASI           33   // DVB-ASI transport stream
#define DT_IOCONFIG_AVENC         34   // Audio/video encoder
#define DT_IOCONFIG_DEKTECST      35   // DekTec Streaming-data Interface
#define DT_IOCONFIG_DEMOD         36   // Demodulation
#define DT_IOCONFIG_GPSTIME       37   // 1PPS and 10MHz GPS-clock input
#define DT_IOCONFIG_HDMI          38   // HDMI
#define DT_IOCONFIG_HDSDI         39   // HD-SDI
#define DT_IOCONFIG_IFADC         40   // IF A/D converter
#define DT_IOCONFIG_IP            41   // Transport stream over IP
#define DT_IOCONFIG_MOD           42   // Modulator output
#define DT_IOCONFIG_PHASENOISE    43   // Phase noise injection
#define DT_IOCONFIG_RS422         44   // RS422 port
#define DT_IOCONFIG_SDIRX         45   // SDI receiver
#define DT_IOCONFIG_SDI           46   // SD-SDI
#define DT_IOCONFIG_SPI           47   // DVB-SPI transport stream
#define DT_IOCONFIG_SPISDI        48   // SD-SDI on a parallel port
#define DT_IOCONFIG_2160P50       49   // 2160p/50 lvl A
#define DT_IOCONFIG_2160P50B      50   // 2160p/50 lvl B
#define DT_IOCONFIG_2160P59_94    51   // 2160p/59.94 lvl A
#define DT_IOCONFIG_2160P59_94B   52   // 2160p/59.94 lvl B
#define DT_IOCONFIG_2160P60       53   // 2160p/60 lvl A
#define DT_IOCONFIG_2160P60B      54   // 2160p/60 lvl B
#define DT_IOCONFIG_1080P50       55   // 1080p/50 lvl A
#define DT_IOCONFIG_1080P50B      56   // 1080p/50 lvl B
#define DT_IOCONFIG_1080P59_94    57   // 1080p/59.94 lvl A
#define DT_IOCONFIG_1080P59_94B   58   // 1080p/59.94 lvl B
#define DT_IOCONFIG_1080P60       59   // 1080p/60 lvl A
#define DT_IOCONFIG_1080P60B      60   // 1080p/60 lvl B
#define DT_IOCONFIG_2160P23_98    61   // 2160p/23.98
#define DT_IOCONFIG_2160P24       62   // 2160p/24
#define DT_IOCONFIG_2160P25       63   // 2160p/25
#define DT_IOCONFIG_2160P29_97    64   // 2160p/29.97
#define DT_IOCONFIG_2160P30       65   // 2160p/30
#define DT_IOCONFIG_1080I50       66   // 1080i/50
#define DT_IOCONFIG_1080I59_94    67   // 1080i/59.94
#define DT_IOCONFIG_1080I60       68   // 1080i/60
#define DT_IOCONFIG_1080P23_98    69   // 1080p/23.98
#define DT_IOCONFIG_1080P24       70   // 1080p/24
#define DT_IOCONFIG_1080P25       71   // 1080p/25
#define DT_IOCONFIG_1080P29_97    72   // 1080p/29.97
#define DT_IOCONFIG_1080P30       73   // 1080p/30
#define DT_IOCONFIG_1080PSF23_98  74   // 1080psf/23.98
#define DT_IOCONFIG_1080PSF24     75   // 1080psf/24
#define DT_IOCONFIG_1080PSF25     76   // 1080psf/25
#define DT_IOCONFIG_1080PSF29_97  77   // 1080psf/29.97
#define DT_IOCONFIG_1080PSF30     78   // 1080psf/30
#define DT_IOCONFIG_720P23_98     79   // 720p/23.98
#define DT_IOCONFIG_720P24        80   // 720p/24
#define DT_IOCONFIG_720P25        81   // 720p/25
#define DT_IOCONFIG_720P29_97     82   // 720p/29.97
#define DT_IOCONFIG_720P30        83   // 720p/30
#define DT_IOCONFIG_720P50        84   // 720p/50
#define DT_IOCONFIG_720P59_94     85   // 720p/59.94
#define DT_IOCONFIG_720P60        86   // 720p/60
#define DT_IOCONFIG_525I59_94     87   // 525i/59.94
#define DT_IOCONFIG_625I50        88   // 625i/50
#define DT_IOCONFIG_SPI525I59_94  89   // SPI 525i/59.94
#define DT_IOCONFIG_SPI625I50     90   // SPI 625i/50
#define DT_IOCONFIG_SCALE_12GTO3G 91   // Downscale 12G-SDI to 3G-SDI
#define DT_IOCONFIG_SCALE_BYPASS  92   // Bypass the downscaler
#define DT_IOCONFIG_MODHQ         93   // High-quality modulation
#define DT_IOCONFIG_LOWPWR        94   // Low-power mode
#define DT_IOCONFIG_RFCLKEXT      95   // External RF clock input
#define DT_IOCONFIG_RFCLKINT      96   // Internal RF clock reference
#define DT_IOCONFIG_SPICLKEXT     97   // External clock input
#define DT_IOCONFIG_SPICLKINT     98   // Internal clock reference
#define DT_IOCONFIG_SPIFIXEDCLK   99   // SPI fixed clock with valid signal
#define DT_IOCONFIG_SPIDVBMODE    100  // SPI DVB mode
#define DT_IOCONFIG_SPISER8B      101  // SPI serial 8-bit mode
#define DT_IOCONFIG_SPISER10B     102  // SPI serial 10-bit mode
#define DT_IOCONFIG_SPILVDS1      103  // LVDS1
#define DT_IOCONFIG_SPILVDS2      104  // LVDS2
#define DT_IOCONFIG_SPILVTTL      105  // LVTTL
#define DT_IOCONFIG_EXTTSRATE     106  // External TS rate clock input
#define DT_IOCONFIG_EXTRATIO      107  // External TS rate clock with ratio
#define DT_IOCONFIG_INTTSRATE     108  // Internal TS rate clock reference
#define DT_IOCONFIG_LOCK2INP      109  // Lock TS rate to input port

#define DT_IOCONFIG_COUNT         110  // Number of DT_IOCONFIG codes
#define DT_IOCONFIG_NONE          -1   // DT_IOCONFIG code is not applicable
