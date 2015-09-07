//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIoConfigDefs.h *#*#*#*#*#*#*#*#*#* (C) 2015 DekTec
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
#define DT_IOCONFIG_OUTPUT       18  // Uni-directional output
#define DT_IOCONFIG_SHAREDANT    19  // Get antenna signal from another port
#define DT_IOCONFIG_DBLBUF       20  // Double buffered output
#define DT_IOCONFIG_LOOPS2L3     21  // Loop-through of DVB-S2 in L3-frames
#define DT_IOCONFIG_LOOPS2TS     22  // Loop-through of an DVB-S(2) input
#define DT_IOCONFIG_LOOPTHR      23  // Loop-through of another input
#define DT_IOCONFIG_3GSDI        24  // 3G-SDI
#define DT_IOCONFIG_ASI          25  // DVB-ASI transport stream
#define DT_IOCONFIG_DEMOD        26  // Demodulation
#define DT_IOCONFIG_GPSTIME      27  // Supports GPS-time
#define DT_IOCONFIG_HDSDI        28  // HD-SDI
#define DT_IOCONFIG_IFADC        29  // IF A/D converter
#define DT_IOCONFIG_IP           30  // Transport stream over IP
#define DT_IOCONFIG_MOD          31  // Modulator output
#define DT_IOCONFIG_PHASENOISE   32  // Phase noise injector
#define DT_IOCONFIG_RS422        33  // RS422 port
#define DT_IOCONFIG_SDI          34  // SD-SDI
#define DT_IOCONFIG_SPI          35  // DVB-SPI transport stream
#define DT_IOCONFIG_SPISDI       36  // SD-SDI on a parallel port
#define DT_IOCONFIG_1080P50      37  // 1080p/50 lvl A
#define DT_IOCONFIG_1080P50B     38  // 1080p/50 lvl B
#define DT_IOCONFIG_1080P59_94   39  // 1080p/59.94 lvl A
#define DT_IOCONFIG_1080P59_94B  40  // 1080p/59.94 lvl B
#define DT_IOCONFIG_1080P60      41  // 1080p/60 lvl A
#define DT_IOCONFIG_1080P60B     42  // 1080p/60 lvl B
#define DT_IOCONFIG_1080I50      43  // 1080i/50
#define DT_IOCONFIG_1080I59_94   44  // 1080i/59.94
#define DT_IOCONFIG_1080I60      45  // 1080i/60
#define DT_IOCONFIG_1080P23_98   46  // 1080p/23.98
#define DT_IOCONFIG_1080P24      47  // 1080p/24
#define DT_IOCONFIG_1080P25      48  // 1080p/25
#define DT_IOCONFIG_1080P29_97   49  // 1080p/29.97
#define DT_IOCONFIG_1080P30      50  // 1080p/30
#define DT_IOCONFIG_1080PSF23_98 51  // 1080psf/23.98
#define DT_IOCONFIG_1080PSF24    52  // 1080psf/24
#define DT_IOCONFIG_1080PSF25    53  // 1080psf/25
#define DT_IOCONFIG_1080PSF29_97 54  // 1080psf/29.97
#define DT_IOCONFIG_1080PSF30    55  // 1080psf/30
#define DT_IOCONFIG_720P23_98    56  // 720p/23.98
#define DT_IOCONFIG_720P24       57  // 720p/24
#define DT_IOCONFIG_720P25       58  // 720p/25
#define DT_IOCONFIG_720P29_97    59  // 720p/29.97
#define DT_IOCONFIG_720P30       60  // 720p/30
#define DT_IOCONFIG_720P50       61  // 720p/50
#define DT_IOCONFIG_720P59_94    62  // 720p/59.94
#define DT_IOCONFIG_720P60       63  // 720p/60
#define DT_IOCONFIG_525I59_94    64  // 525i/59.94
#define DT_IOCONFIG_625I50       65  // 625i/50
#define DT_IOCONFIG_SPI525I59_94 66  // SPI 525i/59.94
#define DT_IOCONFIG_SPI625I50    67  // SPI 625i/50
#define DT_IOCONFIG_MODHQ        68  // High quality power mode
#define DT_IOCONFIG_LOWPWR       69  // Low power operation mode
#define DT_IOCONFIG_RFCLKEXT     70  // External RF clock input
#define DT_IOCONFIG_RFCLKINT     71  // Internal RF clock reference
#define DT_IOCONFIG_SPICLKEXT    72  // External clock input
#define DT_IOCONFIG_SPICLKINT    73  // Internal clock reference
#define DT_IOCONFIG_SPIFIXEDCLK  74  // SPI fixed clock with valid signal
#define DT_IOCONFIG_SPIDVBMODE   75  // SPI DVB mode
#define DT_IOCONFIG_SPISER8B     76  // SPI serial 8-bit mode
#define DT_IOCONFIG_SPISER10B    77  // SPI serial 10-bit mode
#define DT_IOCONFIG_SPILVDS1     78  // LVDS1
#define DT_IOCONFIG_SPILVDS2     79  // LVDS2
#define DT_IOCONFIG_SPILVTTL     80  // LVTTL
#define DT_IOCONFIG_EXTTSRATE    81  // External TS rate clock input
#define DT_IOCONFIG_EXTRATIO     82  // External TS rate clock with ratio
#define DT_IOCONFIG_INTTSRATE    83  // Internal TS rate clock reference
#define DT_IOCONFIG_LOCK2INP     84  // Lock TS rate to input port

#define DT_IOCONFIG_COUNT        85  // Number of DT_IOCONFIG codes
#define DT_IOCONFIG_NONE         -1  // DT_IOCONFIG code is not applicable
