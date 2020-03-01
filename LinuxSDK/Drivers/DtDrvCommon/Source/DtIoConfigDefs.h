//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIoConfigDefs.h *#*#*#*#*#*#*#*#*#* (C) 2020 DekTec
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
#define DT_IOCONFIG_INTOUTPUT    21   // Internal output port
#define DT_IOCONFIG_MONITOR      22   // Monitor of input or output
#define DT_IOCONFIG_OUTPUT       23   // Uni-directional output
#define DT_IOCONFIG_SHAREDANT    24   // Get antenna signal from another port
#define DT_IOCONFIG_DBLBUF       25   // Double buffered output
#define DT_IOCONFIG_LOOPS2L3     26   // Loop-through of DVB-S2 in L3-frames
#define DT_IOCONFIG_LOOPS2TS     27   // Loop-through of an DVB-S(2) input
#define DT_IOCONFIG_LOOPTHR      28   // Loop-through of another input
#define DT_IOCONFIG_12GSDI       29   // 12G-SDI
#define DT_IOCONFIG_3GSDI        30   // 3G-SDI
#define DT_IOCONFIG_6GSDI        31   // 6G-SDI
#define DT_IOCONFIG_ASI          32   // DVB-ASI transport stream
#define DT_IOCONFIG_AVENC        33   // Audio/video encoder
#define DT_IOCONFIG_DEKTECST     34   // DekTec Streaming-data Interface
#define DT_IOCONFIG_DEMOD        35   // Demodulation
#define DT_IOCONFIG_GPSTIME      36   // 1PPS and 10MHz GPS-clock input
#define DT_IOCONFIG_HDMI         37   // HDMI
#define DT_IOCONFIG_HDSDI        38   // HD-SDI
#define DT_IOCONFIG_IFADC        39   // IF A/D converter
#define DT_IOCONFIG_IP           40   // Transport stream over IP
#define DT_IOCONFIG_MOD          41   // Modulator output
#define DT_IOCONFIG_PHASENOISE   42   // Phase noise injection
#define DT_IOCONFIG_RS422        43   // RS422 port
#define DT_IOCONFIG_SDIRX        44   // SDI receiver
#define DT_IOCONFIG_SDI          45   // SD-SDI
#define DT_IOCONFIG_SPI          46   // DVB-SPI transport stream
#define DT_IOCONFIG_SPISDI       47   // SD-SDI on a parallel port
#define DT_IOCONFIG_2160P50      48   // 2160p/50 lvl A
#define DT_IOCONFIG_2160P50B     49   // 2160p/50 lvl B
#define DT_IOCONFIG_2160P59_94   50   // 2160p/59.94 lvl A
#define DT_IOCONFIG_2160P59_94B  51   // 2160p/59.94 lvl B
#define DT_IOCONFIG_2160P60      52   // 2160p/60 lvl A
#define DT_IOCONFIG_2160P60B     53   // 2160p/60 lvl B
#define DT_IOCONFIG_1080P50      54   // 1080p/50 lvl A
#define DT_IOCONFIG_1080P50B     55   // 1080p/50 lvl B
#define DT_IOCONFIG_1080P59_94   56   // 1080p/59.94 lvl A
#define DT_IOCONFIG_1080P59_94B  57   // 1080p/59.94 lvl B
#define DT_IOCONFIG_1080P60      58   // 1080p/60 lvl A
#define DT_IOCONFIG_1080P60B     59   // 1080p/60 lvl B
#define DT_IOCONFIG_2160P23_98   60   // 2160p/23.98
#define DT_IOCONFIG_2160P24      61   // 2160p/24
#define DT_IOCONFIG_2160P25      62   // 2160p/25
#define DT_IOCONFIG_2160P29_97   63   // 2160p/29.97
#define DT_IOCONFIG_2160P30      64   // 2160p/30
#define DT_IOCONFIG_1080I50      65   // 1080i/50
#define DT_IOCONFIG_1080I59_94   66   // 1080i/59.94
#define DT_IOCONFIG_1080I60      67   // 1080i/60
#define DT_IOCONFIG_1080P23_98   68   // 1080p/23.98
#define DT_IOCONFIG_1080P24      69   // 1080p/24
#define DT_IOCONFIG_1080P25      70   // 1080p/25
#define DT_IOCONFIG_1080P29_97   71   // 1080p/29.97
#define DT_IOCONFIG_1080P30      72   // 1080p/30
#define DT_IOCONFIG_1080PSF23_98 73   // 1080psf/23.98
#define DT_IOCONFIG_1080PSF24    74   // 1080psf/24
#define DT_IOCONFIG_1080PSF25    75   // 1080psf/25
#define DT_IOCONFIG_1080PSF29_97 76   // 1080psf/29.97
#define DT_IOCONFIG_1080PSF30    77   // 1080psf/30
#define DT_IOCONFIG_720P23_98    78   // 720p/23.98
#define DT_IOCONFIG_720P24       79   // 720p/24
#define DT_IOCONFIG_720P25       80   // 720p/25
#define DT_IOCONFIG_720P29_97    81   // 720p/29.97
#define DT_IOCONFIG_720P30       82   // 720p/30
#define DT_IOCONFIG_720P50       83   // 720p/50
#define DT_IOCONFIG_720P59_94    84   // 720p/59.94
#define DT_IOCONFIG_720P60       85   // 720p/60
#define DT_IOCONFIG_525I59_94    86   // 525i/59.94
#define DT_IOCONFIG_625I50       87   // 625i/50
#define DT_IOCONFIG_SPI525I59_94 88   // SPI 525i/59.94
#define DT_IOCONFIG_SPI625I50    89   // SPI 625i/50
#define DT_IOCONFIG_MODHQ        90   // High-quality modulation
#define DT_IOCONFIG_LOWPWR       91   // Low-power mode
#define DT_IOCONFIG_RFCLKEXT     92   // External RF clock input
#define DT_IOCONFIG_RFCLKINT     93   // Internal RF clock reference
#define DT_IOCONFIG_SPICLKEXT    94   // External clock input
#define DT_IOCONFIG_SPICLKINT    95   // Internal clock reference
#define DT_IOCONFIG_SPIFIXEDCLK  96   // SPI fixed clock with valid signal
#define DT_IOCONFIG_SPIDVBMODE   97   // SPI DVB mode
#define DT_IOCONFIG_SPISER8B     98   // SPI serial 8-bit mode
#define DT_IOCONFIG_SPISER10B    99   // SPI serial 10-bit mode
#define DT_IOCONFIG_SPILVDS1     100  // LVDS1
#define DT_IOCONFIG_SPILVDS2     101  // LVDS2
#define DT_IOCONFIG_SPILVTTL     102  // LVTTL
#define DT_IOCONFIG_EXTTSRATE    103  // External TS rate clock input
#define DT_IOCONFIG_EXTRATIO     104  // External TS rate clock with ratio
#define DT_IOCONFIG_INTTSRATE    105  // Internal TS rate clock reference
#define DT_IOCONFIG_LOCK2INP     106  // Lock TS rate to input port

#define DT_IOCONFIG_COUNT        107  // Number of DT_IOCONFIG codes
#define DT_IOCONFIG_NONE         -1   // DT_IOCONFIG code is not applicable
