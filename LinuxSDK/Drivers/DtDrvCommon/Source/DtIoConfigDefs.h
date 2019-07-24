//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIoConfigDefs.h *#*#*#*#*#*#*#*#*#* (C) 2019 DekTec
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
#define DT_IOCONFIG_DMATESTMODE  9    // DMA-rate test mode
#define DT_IOCONFIG_FAILSAFE     10   // A fail-over relay is available
#define DT_IOCONFIG_FRACMODE     11   // Fractional mode is supported
#define DT_IOCONFIG_GENLOCKED    12   // Locked to a genlock reference
#define DT_IOCONFIG_GENREF       13   // Genlock reference input
#define DT_IOCONFIG_SWS2APSK     14   // DVB-S2 APSK mode
#define DT_IOCONFIG_TRUE         15   // Turn I/O capability on
#define DT_IOCONFIG_FALSE        16   // Turn I/O capability off
#define DT_IOCONFIG_DISABLED     17   // Port is disabled
#define DT_IOCONFIG_INPUT        18   // Uni-directional input
#define DT_IOCONFIG_INTINPUT     19   // Internal input port
#define DT_IOCONFIG_MONITOR      20   // Monitor of input or output
#define DT_IOCONFIG_OUTPUT       21   // Uni-directional output
#define DT_IOCONFIG_SHAREDANT    22   // Get antenna signal from another port
#define DT_IOCONFIG_DBLBUF       23   // Double buffered output
#define DT_IOCONFIG_LOOPS2L3     24   // Loop-through of DVB-S2 in L3-frames
#define DT_IOCONFIG_LOOPS2TS     25   // Loop-through of an DVB-S(2) input
#define DT_IOCONFIG_LOOPTHR      26   // Loop-through of another input
#define DT_IOCONFIG_12GSDI       27   // 12G-SDI
#define DT_IOCONFIG_3GSDI        28   // 3G-SDI
#define DT_IOCONFIG_6GSDI        29   // 6G-SDI
#define DT_IOCONFIG_ASI          30   // DVB-ASI transport stream
#define DT_IOCONFIG_AVENC        31   // Audio/video encoder
#define DT_IOCONFIG_DEKTECST     32   // DekTec Streaming-data Interface
#define DT_IOCONFIG_DEMOD        33   // Demodulation
#define DT_IOCONFIG_GPSTIME      34   // 1PPS and 10MHz GPS-clock input
#define DT_IOCONFIG_HDMI         35   // HDMI
#define DT_IOCONFIG_HDSDI        36   // HD-SDI
#define DT_IOCONFIG_IFADC        37   // IF A/D converter
#define DT_IOCONFIG_IP           38   // Transport stream over IP
#define DT_IOCONFIG_MOD          39   // Modulator output
#define DT_IOCONFIG_PHASENOISE   40   // Phase noise injection
#define DT_IOCONFIG_RS422        41   // RS422 port
#define DT_IOCONFIG_SDIRX        42   // SDI receiver
#define DT_IOCONFIG_SDI          43   // SD-SDI
#define DT_IOCONFIG_SPI          44   // DVB-SPI transport stream
#define DT_IOCONFIG_SPISDI       45   // SD-SDI on a parallel port
#define DT_IOCONFIG_2160P50      46   // 2160p/50 lvl A
#define DT_IOCONFIG_2160P50B     47   // 2160p/50 lvl B
#define DT_IOCONFIG_2160P59_94   48   // 2160p/59.94 lvl A
#define DT_IOCONFIG_2160P59_94B  49   // 2160p/59.94 lvl B
#define DT_IOCONFIG_2160P60      50   // 2160p/60 lvl A
#define DT_IOCONFIG_2160P60B     51   // 2160p/60 lvl B
#define DT_IOCONFIG_1080P50      52   // 1080p/50 lvl A
#define DT_IOCONFIG_1080P50B     53   // 1080p/50 lvl B
#define DT_IOCONFIG_1080P59_94   54   // 1080p/59.94 lvl A
#define DT_IOCONFIG_1080P59_94B  55   // 1080p/59.94 lvl B
#define DT_IOCONFIG_1080P60      56   // 1080p/60 lvl A
#define DT_IOCONFIG_1080P60B     57   // 1080p/60 lvl B
#define DT_IOCONFIG_2160P23_98   58   // 2160p/23.98
#define DT_IOCONFIG_2160P24      59   // 2160p/24
#define DT_IOCONFIG_2160P25      60   // 2160p/25
#define DT_IOCONFIG_2160P29_97   61   // 2160p/29.97
#define DT_IOCONFIG_2160P30      62   // 2160p/30
#define DT_IOCONFIG_1080I50      63   // 1080i/50
#define DT_IOCONFIG_1080I59_94   64   // 1080i/59.94
#define DT_IOCONFIG_1080I60      65   // 1080i/60
#define DT_IOCONFIG_1080P23_98   66   // 1080p/23.98
#define DT_IOCONFIG_1080P24      67   // 1080p/24
#define DT_IOCONFIG_1080P25      68   // 1080p/25
#define DT_IOCONFIG_1080P29_97   69   // 1080p/29.97
#define DT_IOCONFIG_1080P30      70   // 1080p/30
#define DT_IOCONFIG_1080PSF23_98 71   // 1080psf/23.98
#define DT_IOCONFIG_1080PSF24    72   // 1080psf/24
#define DT_IOCONFIG_1080PSF25    73   // 1080psf/25
#define DT_IOCONFIG_1080PSF29_97 74   // 1080psf/29.97
#define DT_IOCONFIG_1080PSF30    75   // 1080psf/30
#define DT_IOCONFIG_720P23_98    76   // 720p/23.98
#define DT_IOCONFIG_720P24       77   // 720p/24
#define DT_IOCONFIG_720P25       78   // 720p/25
#define DT_IOCONFIG_720P29_97    79   // 720p/29.97
#define DT_IOCONFIG_720P30       80   // 720p/30
#define DT_IOCONFIG_720P50       81   // 720p/50
#define DT_IOCONFIG_720P59_94    82   // 720p/59.94
#define DT_IOCONFIG_720P60       83   // 720p/60
#define DT_IOCONFIG_525I59_94    84   // 525i/59.94
#define DT_IOCONFIG_625I50       85   // 625i/50
#define DT_IOCONFIG_SPI525I59_94 86   // SPI 525i/59.94
#define DT_IOCONFIG_SPI625I50    87   // SPI 625i/50
#define DT_IOCONFIG_MODHQ        88   // High-quality modulation
#define DT_IOCONFIG_LOWPWR       89   // Low-power mode
#define DT_IOCONFIG_RFCLKEXT     90   // External RF clock input
#define DT_IOCONFIG_RFCLKINT     91   // Internal RF clock reference
#define DT_IOCONFIG_SPICLKEXT    92   // External clock input
#define DT_IOCONFIG_SPICLKINT    93   // Internal clock reference
#define DT_IOCONFIG_SPIFIXEDCLK  94   // SPI fixed clock with valid signal
#define DT_IOCONFIG_SPIDVBMODE   95   // SPI DVB mode
#define DT_IOCONFIG_SPISER8B     96   // SPI serial 8-bit mode
#define DT_IOCONFIG_SPISER10B    97   // SPI serial 10-bit mode
#define DT_IOCONFIG_SPILVDS1     98   // LVDS1
#define DT_IOCONFIG_SPILVDS2     99   // LVDS2
#define DT_IOCONFIG_SPILVTTL     100  // LVTTL
#define DT_IOCONFIG_EXTTSRATE    101  // External TS rate clock input
#define DT_IOCONFIG_EXTRATIO     102  // External TS rate clock with ratio
#define DT_IOCONFIG_INTTSRATE    103  // Internal TS rate clock reference
#define DT_IOCONFIG_LOCK2INP     104  // Lock TS rate to input port

#define DT_IOCONFIG_COUNT        105  // Number of DT_IOCONFIG codes
#define DT_IOCONFIG_NONE         -1   // DT_IOCONFIG code is not applicable
