//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIoConfigDefs.h *#*#*#*#*#*#*#*#*#* (C) 2015 DekTec
//
// CapParser - DT_IOCONFIG definitions
//
// THIS FILE IS GENERATED, DO NOT EDIT!!!


// List of DT_IOCONFIG codes, used for SetIoConfig/GetIoConfig
#define DT_IOCONFIG_3GLVL        0   // 3G-SDI level config
#define DT_IOCONFIG_IODIR        1   // I/O direction
#define DT_IOCONFIG_IOSTD        2   // I/O standard
#define DT_IOCONFIG_RFCLKSEL     3   // RF clock source selection
#define DT_IOCONFIG_SPICLKSEL    4   // Parallel port clock source selection
#define DT_IOCONFIG_SPIMODE      5   // Parallel port mode
#define DT_IOCONFIG_SPISTD       6   // Parallel port I/O standard
#define DT_IOCONFIG_TSRATESEL    7   // Transport-stream rate selection
#define DT_IOCONFIG_BW           8   // Isochronous bandwidth allocated
#define DT_IOCONFIG_FAILSAFE     9   // A fail-over relay is available
#define DT_IOCONFIG_FRACMODE     10  // Fractional mode is supported
#define DT_IOCONFIG_GENLOCKED    11  // Locked to a genlock reference
#define DT_IOCONFIG_GENREF       12  // Genlock reference input
#define DT_IOCONFIG_SWS2APSK     13  // DVB-S2 APSK mode
#define DT_IOCONFIG_3GLVLA       14  // 3G-SDI level A
#define DT_IOCONFIG_3GLVLB       15  // 3G-SDI level B
#define DT_IOCONFIG_TRUE         16  // Turn I/O capability on
#define DT_IOCONFIG_FALSE        17  // Turn I/O capability off
#define DT_IOCONFIG_DISABLED     18  // Port is disabled
#define DT_IOCONFIG_INPUT        19  // Uni-directional input
#define DT_IOCONFIG_OUTPUT       20  // Uni-directional output
#define DT_IOCONFIG_SHAREDANT    21  // Get antenna signal from another port
#define DT_IOCONFIG_DBLBUF       22  // Double buffered output
#define DT_IOCONFIG_LOOPS2L3     23  // Loop-through of DVB-S2 in L3-frames
#define DT_IOCONFIG_LOOPS2TS     24  // Loop-through of an DVB-S(2) input
#define DT_IOCONFIG_LOOPTHR      25  // Loop-through of another input
#define DT_IOCONFIG_3GSDI        26  // 3G-SDI
#define DT_IOCONFIG_ASI          27  // DVB-ASI transport stream
#define DT_IOCONFIG_DEMOD        28  // Demodulation
#define DT_IOCONFIG_GPSTIME      29  // Supports GPS-time
#define DT_IOCONFIG_HDSDI        30  // HD-SDI
#define DT_IOCONFIG_IFADC        31  // IF A/D converter
#define DT_IOCONFIG_IP           32  // Transport stream over IP
#define DT_IOCONFIG_MOD          33  // Modulator output
#define DT_IOCONFIG_PHASENOISE   34  // Phase noise injector
#define DT_IOCONFIG_RS422        35  // RS422 port
#define DT_IOCONFIG_SDI          36  // SD-SDI
#define DT_IOCONFIG_SPI          37  // DVB-SPI transport stream
#define DT_IOCONFIG_SPISDI       38  // SD-SDI on a parallel port
#define DT_IOCONFIG_1080P50      39  // 1080p/50
#define DT_IOCONFIG_1080P59_94   40  // 1080p/59.94
#define DT_IOCONFIG_1080P60      41  // 1080p/60
#define DT_IOCONFIG_1080I50      42  // 1080i/50
#define DT_IOCONFIG_1080I59_94   43  // 1080i/59.94
#define DT_IOCONFIG_1080I60      44  // 1080i/60
#define DT_IOCONFIG_1080P23_98   45  // 1080p/23.98
#define DT_IOCONFIG_1080P24      46  // 1080p/24
#define DT_IOCONFIG_1080P25      47  // 1080p/25
#define DT_IOCONFIG_1080P29_97   48  // 1080p/29.97
#define DT_IOCONFIG_1080P30      49  // 1080p/30
#define DT_IOCONFIG_1080PSF23_98 50  // 1080psf/23.98
#define DT_IOCONFIG_1080PSF24    51  // 1080psf/24
#define DT_IOCONFIG_1080PSF25    52  // 1080psf/25
#define DT_IOCONFIG_1080PSF29_97 53  // 1080psf/29.97
#define DT_IOCONFIG_1080PSF30    54  // 1080psf/30
#define DT_IOCONFIG_720P23_98    55  // 720p/23.98
#define DT_IOCONFIG_720P24       56  // 720p/24
#define DT_IOCONFIG_720P25       57  // 720p/25
#define DT_IOCONFIG_720P29_97    58  // 720p/29.97
#define DT_IOCONFIG_720P30       59  // 720p/30
#define DT_IOCONFIG_720P50       60  // 720p/50
#define DT_IOCONFIG_720P59_94    61  // 720p/59.94
#define DT_IOCONFIG_720P60       62  // 720p/60
#define DT_IOCONFIG_525I59_94    63  // 525i/59.94
#define DT_IOCONFIG_625I50       64  // 625i/50
#define DT_IOCONFIG_SPI525I59_94 65  // SPI 525i/59.94
#define DT_IOCONFIG_SPI625I50    66  // SPI 625i/50
#define DT_IOCONFIG_RFCLKEXT     67  // External RF clock input
#define DT_IOCONFIG_RFCLKINT     68  // Internal RF clock reference
#define DT_IOCONFIG_SPICLKEXT    69  // External clock input
#define DT_IOCONFIG_SPICLKINT    70  // Internal clock reference
#define DT_IOCONFIG_SPIFIXEDCLK  71  // SPI fixed clock with valid signal
#define DT_IOCONFIG_SPIDVBMODE   72  // SPI DVB mode
#define DT_IOCONFIG_SPISER8B     73  // SPI serial 8-bit mode
#define DT_IOCONFIG_SPISER10B    74  // SPI serial 10-bit mode
#define DT_IOCONFIG_SPILVDS1     75  // LVDS1
#define DT_IOCONFIG_SPILVDS2     76  // LVDS2
#define DT_IOCONFIG_SPILVTTL     77  // LVTTL
#define DT_IOCONFIG_EXTTSRATE    78  // External TS rate clock input
#define DT_IOCONFIG_EXTRATIO     79  // External TS rate clock with ratio
#define DT_IOCONFIG_INTTSRATE    80  // Internal TS rate clock reference
#define DT_IOCONFIG_LOCK2INP     81  // Lock TS rate to input port

#define DT_IOCONFIG_COUNT        82  // Number of DT_IOCONFIG codes
#define DT_IOCONFIG_NONE         -1  // DT_IOCONFIG code is not applicable
