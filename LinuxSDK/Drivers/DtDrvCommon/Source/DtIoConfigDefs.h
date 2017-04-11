//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIoConfigDefs.h *#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
//
// CapParser - DT_IOCONFIG definitions
//
// THIS FILE IS GENERATED, DO NOT EDIT!!!


// List of DT_IOCONFIG codes, used for SetIoConfig/GetIoConfig
#define DT_IOCONFIG_IODIR        0    // I/O direction
#define DT_IOCONFIG_IOSTD        1    // I/O standard
#define DT_IOCONFIG_PWRMODE      2    // Power mode
#define DT_IOCONFIG_RFCLKSEL     3    // RF clock source selection
#define DT_IOCONFIG_SPICLKSEL    4    // Parallel port clock source selection
#define DT_IOCONFIG_SPIMODE      5    // Parallel port mode
#define DT_IOCONFIG_SPISTD       6    // Parallel port I/O standard
#define DT_IOCONFIG_TSRATESEL    7    // Transport-stream rate selection
#define DT_IOCONFIG_BW           8    // DEPRECATED, do not use
#define DT_IOCONFIG_FAILSAFE     9    // A fail-over relay is available
#define DT_IOCONFIG_FRACMODE     10   // Fractional mode is supported
#define DT_IOCONFIG_GENLOCKED    11   // Locked to a genlock reference
#define DT_IOCONFIG_GENREF       12   // Genlock reference input
#define DT_IOCONFIG_SWS2APSK     13   // DVB-S2 APSK mode
#define DT_IOCONFIG_TRUE         14   // Turn I/O capability on
#define DT_IOCONFIG_FALSE        15   // Turn I/O capability off
#define DT_IOCONFIG_DISABLED     16   // Port is disabled
#define DT_IOCONFIG_INPUT        17   // Uni-directional input
#define DT_IOCONFIG_INTINPUT     18   // Internal input port
#define DT_IOCONFIG_MONITOR      19   // Monitor of input or output
#define DT_IOCONFIG_OUTPUT       20   // Uni-directional output
#define DT_IOCONFIG_SHAREDANT    21   // Get antenna signal from another port
#define DT_IOCONFIG_DBLBUF       22   // Double buffered output
#define DT_IOCONFIG_LOOPS2L3     23   // Loop-through of DVB-S2 in L3-frames
#define DT_IOCONFIG_LOOPS2TS     24   // Loop-through of an DVB-S(2) input
#define DT_IOCONFIG_LOOPTHR      25   // Loop-through of another input
#define DT_IOCONFIG_12GSDI       26   // 12G-SDI
#define DT_IOCONFIG_3GSDI        27   // 3G-SDI
#define DT_IOCONFIG_6GSDI        28   // 6G-SDI
#define DT_IOCONFIG_ASI          29   // DVB-ASI transport stream
#define DT_IOCONFIG_AVENC        30   // Audio/video encoder
#define DT_IOCONFIG_DEMOD        31   // Demodulation
#define DT_IOCONFIG_GPSTIME      32   // 1PPS and 10MHz GPS-clock input
#define DT_IOCONFIG_HDMI         33   // HDMI
#define DT_IOCONFIG_HDSDI        34   // HD-SDI
#define DT_IOCONFIG_IFADC        35   // IF A/D converter
#define DT_IOCONFIG_IP           36   // Transport stream over IP
#define DT_IOCONFIG_MOD          37   // Modulator output
#define DT_IOCONFIG_PHASENOISE   38   // Phase noise injection
#define DT_IOCONFIG_RS422        39   // RS422 port
#define DT_IOCONFIG_SDIRX        40   // SDI receiver
#define DT_IOCONFIG_SDI          41   // SD-SDI
#define DT_IOCONFIG_SPI          42   // DVB-SPI transport stream
#define DT_IOCONFIG_SPISDI       43   // SD-SDI on a parallel port
#define DT_IOCONFIG_2160P50      44   // 2160p/50 lvl A
#define DT_IOCONFIG_2160P50B     45   // 2160p/50 lvl B
#define DT_IOCONFIG_2160P59_94   46   // 2160p/59.94 lvl A
#define DT_IOCONFIG_2160P59_94B  47   // 2160p/59.94 lvl B
#define DT_IOCONFIG_2160P60      48   // 2160p/60 lvl A
#define DT_IOCONFIG_2160P60B     49   // 2160p/60 lvl B
#define DT_IOCONFIG_1080P50      50   // 1080p/50 lvl A
#define DT_IOCONFIG_1080P50B     51   // 1080p/50 lvl B
#define DT_IOCONFIG_1080P59_94   52   // 1080p/59.94 lvl A
#define DT_IOCONFIG_1080P59_94B  53   // 1080p/59.94 lvl B
#define DT_IOCONFIG_1080P60      54   // 1080p/60 lvl A
#define DT_IOCONFIG_1080P60B     55   // 1080p/60 lvl B
#define DT_IOCONFIG_2160P23_98   56   // 2160p/23.98
#define DT_IOCONFIG_2160P24      57   // 2160p/24
#define DT_IOCONFIG_2160P25      58   // 2160p/25
#define DT_IOCONFIG_2160P29_97   59   // 2160p/29.97
#define DT_IOCONFIG_2160P30      60   // 2160p/30
#define DT_IOCONFIG_1080I50      61   // 1080i/50
#define DT_IOCONFIG_1080I59_94   62   // 1080i/59.94
#define DT_IOCONFIG_1080I60      63   // 1080i/60
#define DT_IOCONFIG_1080P23_98   64   // 1080p/23.98
#define DT_IOCONFIG_1080P24      65   // 1080p/24
#define DT_IOCONFIG_1080P25      66   // 1080p/25
#define DT_IOCONFIG_1080P29_97   67   // 1080p/29.97
#define DT_IOCONFIG_1080P30      68   // 1080p/30
#define DT_IOCONFIG_1080PSF23_98 69   // 1080psf/23.98
#define DT_IOCONFIG_1080PSF24    70   // 1080psf/24
#define DT_IOCONFIG_1080PSF25    71   // 1080psf/25
#define DT_IOCONFIG_1080PSF29_97 72   // 1080psf/29.97
#define DT_IOCONFIG_1080PSF30    73   // 1080psf/30
#define DT_IOCONFIG_720P23_98    74   // 720p/23.98
#define DT_IOCONFIG_720P24       75   // 720p/24
#define DT_IOCONFIG_720P25       76   // 720p/25
#define DT_IOCONFIG_720P29_97    77   // 720p/29.97
#define DT_IOCONFIG_720P30       78   // 720p/30
#define DT_IOCONFIG_720P50       79   // 720p/50
#define DT_IOCONFIG_720P59_94    80   // 720p/59.94
#define DT_IOCONFIG_720P60       81   // 720p/60
#define DT_IOCONFIG_525I59_94    82   // 525i/59.94
#define DT_IOCONFIG_625I50       83   // 625i/50
#define DT_IOCONFIG_SPI525I59_94 84   // SPI 525i/59.94
#define DT_IOCONFIG_SPI625I50    85   // SPI 625i/50
#define DT_IOCONFIG_MODHQ        86   // High-quality modulation
#define DT_IOCONFIG_LOWPWR       87   // Low-power mode
#define DT_IOCONFIG_RFCLKEXT     88   // External RF clock input
#define DT_IOCONFIG_RFCLKINT     89   // Internal RF clock reference
#define DT_IOCONFIG_SPICLKEXT    90   // External clock input
#define DT_IOCONFIG_SPICLKINT    91   // Internal clock reference
#define DT_IOCONFIG_SPIFIXEDCLK  92   // SPI fixed clock with valid signal
#define DT_IOCONFIG_SPIDVBMODE   93   // SPI DVB mode
#define DT_IOCONFIG_SPISER8B     94   // SPI serial 8-bit mode
#define DT_IOCONFIG_SPISER10B    95   // SPI serial 10-bit mode
#define DT_IOCONFIG_SPILVDS1     96   // LVDS1
#define DT_IOCONFIG_SPILVDS2     97   // LVDS2
#define DT_IOCONFIG_SPILVTTL     98   // LVTTL
#define DT_IOCONFIG_EXTTSRATE    99   // External TS rate clock input
#define DT_IOCONFIG_EXTRATIO     100  // External TS rate clock with ratio
#define DT_IOCONFIG_INTTSRATE    101  // Internal TS rate clock reference
#define DT_IOCONFIG_LOCK2INP     102  // Lock TS rate to input port

#define DT_IOCONFIG_COUNT        103  // Number of DT_IOCONFIG codes
#define DT_IOCONFIG_NONE         -1   // DT_IOCONFIG code is not applicable
