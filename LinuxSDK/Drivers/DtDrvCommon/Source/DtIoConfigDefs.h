//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIoConfigDefs.h *#*#*#*#*#*#*#*#*#* (C) 2014 DekTec
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
#define DT_IOCONFIG_HDSDI        29  // HD-SDI
#define DT_IOCONFIG_IFADC        30  // IF A/D converter
#define DT_IOCONFIG_IP           31  // Transport stream over IP
#define DT_IOCONFIG_MOD          32  // Modulator output
#define DT_IOCONFIG_RS422        33  // RS422 port
#define DT_IOCONFIG_SDI          34  // SD-SDI
#define DT_IOCONFIG_SPI          35  // DVB-SPI transport stream
#define DT_IOCONFIG_SPISDI       36  // SD-SDI on a parallel port
#define DT_IOCONFIG_1080P50      37  // 1080p/50
#define DT_IOCONFIG_1080P59_94   38  // 1080p/59.94
#define DT_IOCONFIG_1080P60      39  // 1080p/60
#define DT_IOCONFIG_1080I50      40  // 1080i/50
#define DT_IOCONFIG_1080I59_94   41  // 1080i/59.94
#define DT_IOCONFIG_1080I60      42  // 1080i/60
#define DT_IOCONFIG_1080P23_98   43  // 1080p/23.98
#define DT_IOCONFIG_1080P24      44  // 1080p/24
#define DT_IOCONFIG_1080P25      45  // 1080p/25
#define DT_IOCONFIG_1080P29_97   46  // 1080p/29.97
#define DT_IOCONFIG_1080P30      47  // 1080p/30
#define DT_IOCONFIG_720P23_98    48  // 720p/23.98
#define DT_IOCONFIG_720P24       49  // 720p/24
#define DT_IOCONFIG_720P25       50  // 720p/25
#define DT_IOCONFIG_720P29_97    51  // 720p/29.97
#define DT_IOCONFIG_720P30       52  // 720p/30
#define DT_IOCONFIG_720P50       53  // 720p/50
#define DT_IOCONFIG_720P59_94    54  // 720p/59.94
#define DT_IOCONFIG_720P60       55  // 720p/60
#define DT_IOCONFIG_525I59_94    56  // 525i/59.94
#define DT_IOCONFIG_625I50       57  // 625i/50
#define DT_IOCONFIG_SPI525I59_94 58  // SPI 525i/59.94
#define DT_IOCONFIG_SPI625I50    59  // SPI 625i/50
#define DT_IOCONFIG_RFCLKEXT     60  // External RF clock input
#define DT_IOCONFIG_RFCLKINT     61  // Internal RF clock reference
#define DT_IOCONFIG_SPICLKEXT    62  // External clock input
#define DT_IOCONFIG_SPICLKINT    63  // Internal clock reference
#define DT_IOCONFIG_SPIFIXEDCLK  64  // SPI fixed clock with valid signal
#define DT_IOCONFIG_SPIDVBMODE   65  // SPI DVB mode
#define DT_IOCONFIG_SPISER8B     66  // SPI serial 8-bit mode
#define DT_IOCONFIG_SPISER10B    67  // SPI serial 10-bit mode
#define DT_IOCONFIG_SPILVDS1     68  // LVDS1
#define DT_IOCONFIG_SPILVDS2     69  // LVDS2
#define DT_IOCONFIG_SPILVTTL     70  // LVTTL
#define DT_IOCONFIG_EXTTSRATE    71  // External TS rate clock input
#define DT_IOCONFIG_EXTRATIO     72  // External TS rate clock with ratio
#define DT_IOCONFIG_INTTSRATE    73  // Internal TS rate clock reference
#define DT_IOCONFIG_LOCK2INP     74  // Lock TS rate to input port

#define DT_IOCONFIG_COUNT        75  // Number of DT_IOCONFIG codes
#define DT_IOCONFIG_NONE         -1  // DT_IOCONFIG code is not applicable
