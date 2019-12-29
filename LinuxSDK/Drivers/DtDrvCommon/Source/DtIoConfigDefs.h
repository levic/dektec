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
#define DT_IOCONFIG_AUTOBFGEN    8    // Automatic black-frame generation
#define DT_IOCONFIG_BW           9    // DEPRECATED, do not use
#define DT_IOCONFIG_DMATESTMODE  10   // DMA-rate test mode
#define DT_IOCONFIG_FAILSAFE     11   // A fail-over relay is available
#define DT_IOCONFIG_FRACMODE     12   // Fractional mode is supported
#define DT_IOCONFIG_GENLOCKED    13   // Locked to a genlock reference
#define DT_IOCONFIG_GENREF       14   // Genlock reference input
#define DT_IOCONFIG_SWS2APSK     15   // DVB-S2 APSK mode
#define DT_IOCONFIG_TRUE         16   // Turn I/O capability on
#define DT_IOCONFIG_FALSE        17   // Turn I/O capability off
#define DT_IOCONFIG_DISABLED     18   // Port is disabled
#define DT_IOCONFIG_INPUT        19   // Uni-directional input
#define DT_IOCONFIG_INTINPUT     20   // Internal input port
#define DT_IOCONFIG_MONITOR      21   // Monitor of input or output
#define DT_IOCONFIG_OUTPUT       22   // Uni-directional output
#define DT_IOCONFIG_SHAREDANT    23   // Get antenna signal from another port
#define DT_IOCONFIG_DBLBUF       24   // Double buffered output
#define DT_IOCONFIG_LOOPS2L3     25   // Loop-through of DVB-S2 in L3-frames
#define DT_IOCONFIG_LOOPS2TS     26   // Loop-through of an DVB-S(2) input
#define DT_IOCONFIG_LOOPTHR      27   // Loop-through of another input
#define DT_IOCONFIG_12GSDI       28   // 12G-SDI
#define DT_IOCONFIG_3GSDI        29   // 3G-SDI
#define DT_IOCONFIG_6GSDI        30   // 6G-SDI
#define DT_IOCONFIG_ASI          31   // DVB-ASI transport stream
#define DT_IOCONFIG_AVENC        32   // Audio/video encoder
#define DT_IOCONFIG_DEKTECST     33   // DekTec Streaming-data Interface
#define DT_IOCONFIG_DEMOD        34   // Demodulation
#define DT_IOCONFIG_GPSTIME      35   // 1PPS and 10MHz GPS-clock input
#define DT_IOCONFIG_HDMI         36   // HDMI
#define DT_IOCONFIG_HDSDI        37   // HD-SDI
#define DT_IOCONFIG_IFADC        38   // IF A/D converter
#define DT_IOCONFIG_IP           39   // Transport stream over IP
#define DT_IOCONFIG_MOD          40   // Modulator output
#define DT_IOCONFIG_PHASENOISE   41   // Phase noise injection
#define DT_IOCONFIG_RS422        42   // RS422 port
#define DT_IOCONFIG_SDIRX        43   // SDI receiver
#define DT_IOCONFIG_SDI          44   // SD-SDI
#define DT_IOCONFIG_SPI          45   // DVB-SPI transport stream
#define DT_IOCONFIG_SPISDI       46   // SD-SDI on a parallel port
#define DT_IOCONFIG_2160P50      47   // 2160p/50 lvl A
#define DT_IOCONFIG_2160P50B     48   // 2160p/50 lvl B
#define DT_IOCONFIG_2160P59_94   49   // 2160p/59.94 lvl A
#define DT_IOCONFIG_2160P59_94B  50   // 2160p/59.94 lvl B
#define DT_IOCONFIG_2160P60      51   // 2160p/60 lvl A
#define DT_IOCONFIG_2160P60B     52   // 2160p/60 lvl B
#define DT_IOCONFIG_1080P50      53   // 1080p/50 lvl A
#define DT_IOCONFIG_1080P50B     54   // 1080p/50 lvl B
#define DT_IOCONFIG_1080P59_94   55   // 1080p/59.94 lvl A
#define DT_IOCONFIG_1080P59_94B  56   // 1080p/59.94 lvl B
#define DT_IOCONFIG_1080P60      57   // 1080p/60 lvl A
#define DT_IOCONFIG_1080P60B     58   // 1080p/60 lvl B
#define DT_IOCONFIG_2160P23_98   59   // 2160p/23.98
#define DT_IOCONFIG_2160P24      60   // 2160p/24
#define DT_IOCONFIG_2160P25      61   // 2160p/25
#define DT_IOCONFIG_2160P29_97   62   // 2160p/29.97
#define DT_IOCONFIG_2160P30      63   // 2160p/30
#define DT_IOCONFIG_1080I50      64   // 1080i/50
#define DT_IOCONFIG_1080I59_94   65   // 1080i/59.94
#define DT_IOCONFIG_1080I60      66   // 1080i/60
#define DT_IOCONFIG_1080P23_98   67   // 1080p/23.98
#define DT_IOCONFIG_1080P24      68   // 1080p/24
#define DT_IOCONFIG_1080P25      69   // 1080p/25
#define DT_IOCONFIG_1080P29_97   70   // 1080p/29.97
#define DT_IOCONFIG_1080P30      71   // 1080p/30
#define DT_IOCONFIG_1080PSF23_98 72   // 1080psf/23.98
#define DT_IOCONFIG_1080PSF24    73   // 1080psf/24
#define DT_IOCONFIG_1080PSF25    74   // 1080psf/25
#define DT_IOCONFIG_1080PSF29_97 75   // 1080psf/29.97
#define DT_IOCONFIG_1080PSF30    76   // 1080psf/30
#define DT_IOCONFIG_720P23_98    77   // 720p/23.98
#define DT_IOCONFIG_720P24       78   // 720p/24
#define DT_IOCONFIG_720P25       79   // 720p/25
#define DT_IOCONFIG_720P29_97    80   // 720p/29.97
#define DT_IOCONFIG_720P30       81   // 720p/30
#define DT_IOCONFIG_720P50       82   // 720p/50
#define DT_IOCONFIG_720P59_94    83   // 720p/59.94
#define DT_IOCONFIG_720P60       84   // 720p/60
#define DT_IOCONFIG_525I59_94    85   // 525i/59.94
#define DT_IOCONFIG_625I50       86   // 625i/50
#define DT_IOCONFIG_SPI525I59_94 87   // SPI 525i/59.94
#define DT_IOCONFIG_SPI625I50    88   // SPI 625i/50
#define DT_IOCONFIG_MODHQ        89   // High-quality modulation
#define DT_IOCONFIG_LOWPWR       90   // Low-power mode
#define DT_IOCONFIG_RFCLKEXT     91   // External RF clock input
#define DT_IOCONFIG_RFCLKINT     92   // Internal RF clock reference
#define DT_IOCONFIG_SPICLKEXT    93   // External clock input
#define DT_IOCONFIG_SPICLKINT    94   // Internal clock reference
#define DT_IOCONFIG_SPIFIXEDCLK  95   // SPI fixed clock with valid signal
#define DT_IOCONFIG_SPIDVBMODE   96   // SPI DVB mode
#define DT_IOCONFIG_SPISER8B     97   // SPI serial 8-bit mode
#define DT_IOCONFIG_SPISER10B    98   // SPI serial 10-bit mode
#define DT_IOCONFIG_SPILVDS1     99   // LVDS1
#define DT_IOCONFIG_SPILVDS2     100  // LVDS2
#define DT_IOCONFIG_SPILVTTL     101  // LVTTL
#define DT_IOCONFIG_EXTTSRATE    102  // External TS rate clock input
#define DT_IOCONFIG_EXTRATIO     103  // External TS rate clock with ratio
#define DT_IOCONFIG_INTTSRATE    104  // Internal TS rate clock reference
#define DT_IOCONFIG_LOCK2INP     105  // Lock TS rate to input port

#define DT_IOCONFIG_COUNT        106  // Number of DT_IOCONFIG codes
#define DT_IOCONFIG_NONE         -1   // DT_IOCONFIG code is not applicable
