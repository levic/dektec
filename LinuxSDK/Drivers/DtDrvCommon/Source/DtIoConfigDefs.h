//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIoConfigDefs.h *#*#*#*#*#*#*#*#*#* (C) 2016 DekTec
//
// CapParser - DT_IOCONFIG definitions
//
// THIS FILE IS GENERATED, DO NOT EDIT!!!


// List of DT_IOCONFIG codes, used for SetIoConfig/GetIoConfig
#define DT_IOCONFIG_IODIR        0   // I/O direction
#define DT_IOCONFIG_IOSTD        1   // I/O standard
#define DT_IOCONFIG_PWRMODE      2   // Power mode
#define DT_IOCONFIG_RFCLKSEL     3   // RF clock source selection
#define DT_IOCONFIG_SPICLKSEL    4   // Parallel port clock source selection
#define DT_IOCONFIG_SPIMODE      5   // Parallel port mode
#define DT_IOCONFIG_SPISTD       6   // Parallel port I/O standard
#define DT_IOCONFIG_TSRATESEL    7   // Transport-stream rate selection
#define DT_IOCONFIG_BW           8   // DEPRECATED, do not use
#define DT_IOCONFIG_FAILSAFE     9   // A fail-over relay is available
#define DT_IOCONFIG_FRACMODE     10  // Fractional mode is supported
#define DT_IOCONFIG_GENLOCKED    11  // Locked to a genlock reference
#define DT_IOCONFIG_GENREF       12  // Genlock reference input
#define DT_IOCONFIG_SWS2APSK     13  // DVB-S2 APSK mode
#define DT_IOCONFIG_TRUE         14  // Turn I/O capability on
#define DT_IOCONFIG_FALSE        15  // Turn I/O capability off
#define DT_IOCONFIG_DISABLED     16  // Port is disabled
#define DT_IOCONFIG_INPUT        17  // Uni-directional input
#define DT_IOCONFIG_INTINPUT     18  // Internal input port
#define DT_IOCONFIG_MONITOR      19  // Monitor of input
#define DT_IOCONFIG_OUTPUT       20  // Uni-directional output
#define DT_IOCONFIG_SHAREDANT    21  // Get antenna signal from another port
#define DT_IOCONFIG_DBLBUF       22  // Double buffered output
#define DT_IOCONFIG_LOOPS2L3     23  // Loop-through of DVB-S2 in L3-frames
#define DT_IOCONFIG_LOOPS2TS     24  // Loop-through of an DVB-S(2) input
#define DT_IOCONFIG_LOOPTHR      25  // Loop-through of another input
#define DT_IOCONFIG_3GSDI        26  // 3G-SDI
#define DT_IOCONFIG_ASI          27  // DVB-ASI transport stream
#define DT_IOCONFIG_AVENC        28  // Audio/video encoder
#define DT_IOCONFIG_DEMOD        29  // Demodulation
#define DT_IOCONFIG_GPSTIME      30  // 1PPS and 10MHz GPS-clock input
#define DT_IOCONFIG_HDMI         31  // HDMI
#define DT_IOCONFIG_HDSDI        32  // HD-SDI
#define DT_IOCONFIG_IFADC        33  // IF A/D converter
#define DT_IOCONFIG_IP           34  // Transport stream over IP
#define DT_IOCONFIG_MOD          35  // Modulator output
#define DT_IOCONFIG_PHASENOISE   36  // Phase noise injection
#define DT_IOCONFIG_RS422        37  // RS422 port
#define DT_IOCONFIG_SDIRX        38  // SDI receiver
#define DT_IOCONFIG_SDI          39  // SD-SDI
#define DT_IOCONFIG_SPI          40  // DVB-SPI transport stream
#define DT_IOCONFIG_SPISDI       41  // SD-SDI on a parallel port
#define DT_IOCONFIG_1080P50      42  // 1080p/50 lvl A
#define DT_IOCONFIG_1080P50B     43  // 1080p/50 lvl B
#define DT_IOCONFIG_1080P59_94   44  // 1080p/59.94 lvl A
#define DT_IOCONFIG_1080P59_94B  45  // 1080p/59.94 lvl B
#define DT_IOCONFIG_1080P60      46  // 1080p/60 lvl A
#define DT_IOCONFIG_1080P60B     47  // 1080p/60 lvl B
#define DT_IOCONFIG_1080I50      48  // 1080i/50
#define DT_IOCONFIG_1080I59_94   49  // 1080i/59.94
#define DT_IOCONFIG_1080I60      50  // 1080i/60
#define DT_IOCONFIG_1080P23_98   51  // 1080p/23.98
#define DT_IOCONFIG_1080P24      52  // 1080p/24
#define DT_IOCONFIG_1080P25      53  // 1080p/25
#define DT_IOCONFIG_1080P29_97   54  // 1080p/29.97
#define DT_IOCONFIG_1080P30      55  // 1080p/30
#define DT_IOCONFIG_1080PSF23_98 56  // 1080psf/23.98
#define DT_IOCONFIG_1080PSF24    57  // 1080psf/24
#define DT_IOCONFIG_1080PSF25    58  // 1080psf/25
#define DT_IOCONFIG_1080PSF29_97 59  // 1080psf/29.97
#define DT_IOCONFIG_1080PSF30    60  // 1080psf/30
#define DT_IOCONFIG_720P23_98    61  // 720p/23.98
#define DT_IOCONFIG_720P24       62  // 720p/24
#define DT_IOCONFIG_720P25       63  // 720p/25
#define DT_IOCONFIG_720P29_97    64  // 720p/29.97
#define DT_IOCONFIG_720P30       65  // 720p/30
#define DT_IOCONFIG_720P50       66  // 720p/50
#define DT_IOCONFIG_720P59_94    67  // 720p/59.94
#define DT_IOCONFIG_720P60       68  // 720p/60
#define DT_IOCONFIG_525I59_94    69  // 525i/59.94
#define DT_IOCONFIG_625I50       70  // 625i/50
#define DT_IOCONFIG_SPI525I59_94 71  // SPI 525i/59.94
#define DT_IOCONFIG_SPI625I50    72  // SPI 625i/50
#define DT_IOCONFIG_MODHQ        73  // High-quality modulation
#define DT_IOCONFIG_LOWPWR       74  // Low-power mode
#define DT_IOCONFIG_RFCLKEXT     75  // External RF clock input
#define DT_IOCONFIG_RFCLKINT     76  // Internal RF clock reference
#define DT_IOCONFIG_SPICLKEXT    77  // External clock input
#define DT_IOCONFIG_SPICLKINT    78  // Internal clock reference
#define DT_IOCONFIG_SPIFIXEDCLK  79  // SPI fixed clock with valid signal
#define DT_IOCONFIG_SPIDVBMODE   80  // SPI DVB mode
#define DT_IOCONFIG_SPISER8B     81  // SPI serial 8-bit mode
#define DT_IOCONFIG_SPISER10B    82  // SPI serial 10-bit mode
#define DT_IOCONFIG_SPILVDS1     83  // LVDS1
#define DT_IOCONFIG_SPILVDS2     84  // LVDS2
#define DT_IOCONFIG_SPILVTTL     85  // LVTTL
#define DT_IOCONFIG_EXTTSRATE    86  // External TS rate clock input
#define DT_IOCONFIG_EXTRATIO     87  // External TS rate clock with ratio
#define DT_IOCONFIG_INTTSRATE    88  // Internal TS rate clock reference
#define DT_IOCONFIG_LOCK2INP     89  // Lock TS rate to input port

#define DT_IOCONFIG_COUNT        90  // Number of DT_IOCONFIG codes
#define DT_IOCONFIG_NONE         -1  // DT_IOCONFIG code is not applicable
