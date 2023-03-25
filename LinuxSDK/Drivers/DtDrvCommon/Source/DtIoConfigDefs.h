//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIoConfigDefs.h *#*#*#*#*#*#*#*#*#* (C) 2023 DekTec
//
// CapParser - DT_IOCONFIG definitions
//
// THIS FILE IS GENERATED, DO NOT EDIT!!!


// List of DT_IOCONFIG codes, used for SetIoConfig/GetIoConfig
#define DT_IOCONFIG_IODIR              0    // I/O direction
#define DT_IOCONFIG_IOSTD              1    // I/O standard
#define DT_IOCONFIG_IODOWNSCALE        2    // I/O down-scaling
#define DT_IOCONFIG_PWRMODE            3    // Power mode
#define DT_IOCONFIG_RFCLKSEL           4    // RF clock source selection
#define DT_IOCONFIG_SPICLKSEL          5    // Parallel port clock source selection
#define DT_IOCONFIG_SPIMODE            6    // Parallel port mode
#define DT_IOCONFIG_SPISTD             7    // Parallel port I/O standard
#define DT_IOCONFIG_TSRATESEL          8    // Transport-stream rate selection
#define DT_IOCONFIG_TODREFSEL          9    // TimeOfDay reference selection
#define DT_IOCONFIG_AUTOBFGEN          10   // Automatic black-frame generation
#define DT_IOCONFIG_BW                 11   // DEPRECATED, do not use
#define DT_IOCONFIG_DMATESTMODE        12   // DMA-rate test mode
#define DT_IOCONFIG_FAILSAFE           13   // A fail-over relay is available
#define DT_IOCONFIG_FRACMODE           14   // Fractional mode is supported
#define DT_IOCONFIG_GENLOCKED          15   // Locked to a genlock reference
#define DT_IOCONFIG_GENREF             16   // Genlock reference input
#define DT_IOCONFIG_SWS2APSK           17   // DVB-S2 APSK mode
#define DT_IOCONFIG_TRUE               18   // Turn I/O capability on
#define DT_IOCONFIG_FALSE              19   // Turn I/O capability off
#define DT_IOCONFIG_DISABLED           20   // Port is disabled
#define DT_IOCONFIG_INPUT              21   // Uni-directional input
#define DT_IOCONFIG_INTINPUT           22   // Internal input port
#define DT_IOCONFIG_INTOUTPUT          23   // Internal output port
#define DT_IOCONFIG_MONITOR            24   // Monitor of input or output
#define DT_IOCONFIG_OUTPUT             25   // Uni-directional output
#define DT_IOCONFIG_SHAREDANT          26   // Get antenna signal from another port
#define DT_IOCONFIG_DBLBUF             27   // Double buffered output
#define DT_IOCONFIG_LOOPS2L3           28   // Loop-through of DVB-S2 in L3-frames
#define DT_IOCONFIG_LOOPS2TS           29   // Loop-through of an DVB-S(2) input
#define DT_IOCONFIG_LOOPTHR            30   // Loop-through of another input
#define DT_IOCONFIG_12GSDI             31   // 12G-SDI
#define DT_IOCONFIG_3GSDI              32   // 3G-SDI
#define DT_IOCONFIG_6GSDI              33   // 6G-SDI
#define DT_IOCONFIG_ASI                34   // DVB-ASI transport stream
#define DT_IOCONFIG_AVENC              35   // Audio/video encoder
#define DT_IOCONFIG_DEKTECST           36   // DekTec Streaming-data Interface
#define DT_IOCONFIG_DEMOD              37   // Demodulation
#define DT_IOCONFIG_GPSTIME            38   // 1PPS and 10MHz GPS-clock input
#define DT_IOCONFIG_HDMI               39   // HDMI
#define DT_IOCONFIG_HDSDI              40   // HD-SDI
#define DT_IOCONFIG_IFADC              41   // IF A/D converter
#define DT_IOCONFIG_IP                 42   // Transport stream over IP
#define DT_IOCONFIG_MOD                43   // Modulator output
#define DT_IOCONFIG_PHASENOISE         44   // Phase noise injection
#define DT_IOCONFIG_RS422              45   // RS422 port
#define DT_IOCONFIG_SDIRX              46   // SDI receiver
#define DT_IOCONFIG_SDI                47   // SD-SDI
#define DT_IOCONFIG_SPI                48   // DVB-SPI transport stream
#define DT_IOCONFIG_SPISDI             49   // SD-SDI on a parallel port
#define DT_IOCONFIG_2160P50            50   // 2160p/50 lvl A
#define DT_IOCONFIG_2160P50B           51   // 2160p/50 lvl B
#define DT_IOCONFIG_2160P59_94         52   // 2160p/59.94 lvl A
#define DT_IOCONFIG_2160P59_94B        53   // 2160p/59.94 lvl B
#define DT_IOCONFIG_2160P60            54   // 2160p/60 lvl A
#define DT_IOCONFIG_2160P60B           55   // 2160p/60 lvl B
#define DT_IOCONFIG_1080P50            56   // 1080p/50 lvl A
#define DT_IOCONFIG_1080P50B           57   // 1080p/50 lvl B
#define DT_IOCONFIG_1080P59_94         58   // 1080p/59.94 lvl A
#define DT_IOCONFIG_1080P59_94B        59   // 1080p/59.94 lvl B
#define DT_IOCONFIG_1080P60            60   // 1080p/60 lvl A
#define DT_IOCONFIG_1080P60B           61   // 1080p/60 lvl B
#define DT_IOCONFIG_2160P23_98         62   // 2160p/23.98
#define DT_IOCONFIG_2160P24            63   // 2160p/24
#define DT_IOCONFIG_2160P25            64   // 2160p/25
#define DT_IOCONFIG_2160P29_97         65   // 2160p/29.97
#define DT_IOCONFIG_2160P30            66   // 2160p/30
#define DT_IOCONFIG_1080I50            67   // 1080i/50
#define DT_IOCONFIG_1080I59_94         68   // 1080i/59.94
#define DT_IOCONFIG_1080I60            69   // 1080i/60
#define DT_IOCONFIG_1080P23_98         70   // 1080p/23.98
#define DT_IOCONFIG_1080P24            71   // 1080p/24
#define DT_IOCONFIG_1080P25            72   // 1080p/25
#define DT_IOCONFIG_1080P29_97         73   // 1080p/29.97
#define DT_IOCONFIG_1080P30            74   // 1080p/30
#define DT_IOCONFIG_1080PSF23_98       75   // 1080psf/23.98
#define DT_IOCONFIG_1080PSF24          76   // 1080psf/24
#define DT_IOCONFIG_1080PSF25          77   // 1080psf/25
#define DT_IOCONFIG_1080PSF29_97       78   // 1080psf/29.97
#define DT_IOCONFIG_1080PSF30          79   // 1080psf/30
#define DT_IOCONFIG_720P23_98          80   // 720p/23.98
#define DT_IOCONFIG_720P24             81   // 720p/24
#define DT_IOCONFIG_720P25             82   // 720p/25
#define DT_IOCONFIG_720P29_97          83   // 720p/29.97
#define DT_IOCONFIG_720P30             84   // 720p/30
#define DT_IOCONFIG_720P50             85   // 720p/50
#define DT_IOCONFIG_720P59_94          86   // 720p/59.94
#define DT_IOCONFIG_720P60             87   // 720p/60
#define DT_IOCONFIG_525I59_94          88   // 525i/59.94
#define DT_IOCONFIG_625I50             89   // 625i/50
#define DT_IOCONFIG_SPI525I59_94       90   // SPI 525i/59.94
#define DT_IOCONFIG_SPI625I50          91   // SPI 625i/50
#define DT_IOCONFIG_SCALE_12GTO3G      92   // Downscale 12G-SDI to 3G-SDI
#define DT_IOCONFIG_SCALE_BYPASS       93   // Bypass the downscaler
#define DT_IOCONFIG_MODHQ              94   // High-quality modulation
#define DT_IOCONFIG_LOWPWR             95   // Low-power mode
#define DT_IOCONFIG_RFCLKEXT           96   // External RF clock input
#define DT_IOCONFIG_RFCLKINT           97   // Internal RF clock reference
#define DT_IOCONFIG_SPICLKEXT          98   // External clock input
#define DT_IOCONFIG_SPICLKINT          99   // Internal clock reference
#define DT_IOCONFIG_SPIFIXEDCLK        100  // SPI fixed clock with valid signal
#define DT_IOCONFIG_SPIDVBMODE         101  // SPI DVB mode
#define DT_IOCONFIG_SPISER8B           102  // SPI serial 8-bit mode
#define DT_IOCONFIG_SPISER10B          103  // SPI serial 10-bit mode
#define DT_IOCONFIG_SPILVDS1           104  // LVDS1
#define DT_IOCONFIG_SPILVDS2           105  // LVDS2
#define DT_IOCONFIG_SPILVTTL           106  // LVTTL
#define DT_IOCONFIG_EXTTSRATE          107  // External TS rate clock input
#define DT_IOCONFIG_EXTRATIO           108  // External TS rate clock with ratio
#define DT_IOCONFIG_INTTSRATE          109  // Internal TS rate clock reference
#define DT_IOCONFIG_LOCK2INP           110  // Lock TS rate to input port
#define DT_IOCONFIG_TODREF_INTERNAL    111  // Internal clock reference
#define DT_IOCONFIG_TODREF_STEADYCLOCK 112  // Steady clock reference

#define DT_IOCONFIG_COUNT              113  // Number of DT_IOCONFIG codes
#define DT_IOCONFIG_NONE               -1   // DT_IOCONFIG code is not applicable
