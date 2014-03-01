//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIoConfigDefs.h *#*#*#*#*#*#*#*#*#* (C) 2012 DekTec
//
// CapParser - DT_IOCONFIG definitions
//
// THIS FILE IS GENERATED, DO NOT EDIT!!!


// List of DT_IOCONFIG codes, used for SetIoConfig/GetIoConfig
#define DT_IOCONFIG_IODIR        0   // I/O direction
#define DT_IOCONFIG_IOSTD        1   // I/O standard
#define DT_IOCONFIG_RFCLKSEL     2   // RF clock source selection
#define DT_IOCONFIG_SPICLKSEL    3   // Parallel port clock source selection
#define DT_IOCONFIG_SPIMODE      4   // Parallel port mode
#define DT_IOCONFIG_SPISTD       5   // Parallel port I/O standard
#define DT_IOCONFIG_TSRATESEL    6   // Transport-stream rate selection
#define DT_IOCONFIG_FAILSAFE     7   // A fail-over relay is available
#define DT_IOCONFIG_FRACMODE     8   // Fractional mode is supported
#define DT_IOCONFIG_GENLOCKED    9   // Locked to a genlock reference
#define DT_IOCONFIG_GENREF       10  // Genlock reference input
#define DT_IOCONFIG_SWS2APSK     11  // DVB-S2 APSK mode
#define DT_IOCONFIG_TRUE         12  // Turn I/O capability on
#define DT_IOCONFIG_FALSE        13  // Turn I/O capability off
#define DT_IOCONFIG_DISABLED     14  // Port is disabled
#define DT_IOCONFIG_INPUT        15  // Uni-directional input
#define DT_IOCONFIG_OUTPUT       16  // Uni-directional output
#define DT_IOCONFIG_SHAREDANT    17  // Get antenna signal from another port
#define DT_IOCONFIG_DBLBUF       18  // Double buffered output
#define DT_IOCONFIG_LOOPS2L3     19  // Loop-through of DVB-S2 in L3-frames
#define DT_IOCONFIG_LOOPS2TS     20  // Loop-through of an DVB-S(2) input
#define DT_IOCONFIG_LOOPTHR      21  // Loop-through of another input
#define DT_IOCONFIG_3GSDI        22  // 3G-SDI
#define DT_IOCONFIG_ASI          23  // DVB-ASI transport stream
#define DT_IOCONFIG_DEMOD        24  // Demodulation
#define DT_IOCONFIG_HDSDI        25  // HD-SDI
#define DT_IOCONFIG_IFADC        26  // IF A/D converter
#define DT_IOCONFIG_IP           27  // Transport stream over IP
#define DT_IOCONFIG_MOD          28  // Modulator output
#define DT_IOCONFIG_SDI          29  // SD-SDI
#define DT_IOCONFIG_SPI          30  // DVB-SPI transport stream
#define DT_IOCONFIG_SPISDI       31  // SD-SDI on a parallel port
#define DT_IOCONFIG_1080P50      32  // 1080p/50
#define DT_IOCONFIG_1080P59_94   33  // 1080p/59.94
#define DT_IOCONFIG_1080P60      34  // 1080p/60
#define DT_IOCONFIG_1080I50      35  // 1080i/50
#define DT_IOCONFIG_1080I59_94   36  // 1080i/59.94
#define DT_IOCONFIG_1080I60      37  // 1080i/60
#define DT_IOCONFIG_1080P23_98   38  // 1080p/23.98
#define DT_IOCONFIG_1080P24      39  // 1080p/24
#define DT_IOCONFIG_1080P25      40  // 1080p/25
#define DT_IOCONFIG_1080P29_97   41  // 1080p/29.97
#define DT_IOCONFIG_1080P30      42  // 1080p/30
#define DT_IOCONFIG_720P23_98    43  // 720p/23.98
#define DT_IOCONFIG_720P24       44  // 720p/24
#define DT_IOCONFIG_720P25       45  // 720p/25
#define DT_IOCONFIG_720P29_97    46  // 720p/29.97
#define DT_IOCONFIG_720P30       47  // 720p/30
#define DT_IOCONFIG_720P50       48  // 720p/50
#define DT_IOCONFIG_720P59_94    49  // 720p/59.94
#define DT_IOCONFIG_720P60       50  // 720p/60
#define DT_IOCONFIG_525I59_94    51  // 525i/59.94
#define DT_IOCONFIG_625I50       52  // 625i/50
#define DT_IOCONFIG_SPI525I59_94 53  // SPI 525i/59.94
#define DT_IOCONFIG_SPI625I50    54  // SPI 625i/50
#define DT_IOCONFIG_RFCLKEXT     55  // External RF clock input
#define DT_IOCONFIG_RFCLKINT     56  // Internal RF clock reference
#define DT_IOCONFIG_SPICLKEXT    57  // External clock input
#define DT_IOCONFIG_SPICLKINT    58  // Internal clock reference
#define DT_IOCONFIG_SPIFIXEDCLK  59  // SPI fixed clock with valid signal
#define DT_IOCONFIG_SPIDVBMODE   60  // SPI DVB mode
#define DT_IOCONFIG_SPISER8B     61  // SPI serial 8-bit mode
#define DT_IOCONFIG_SPISER10B    62  // SPI serial 10-bit mode
#define DT_IOCONFIG_SPILVDS1     63  // LVDS1
#define DT_IOCONFIG_SPILVDS2     64  // LVDS2
#define DT_IOCONFIG_SPILVTTL     65  // LVTTL
#define DT_IOCONFIG_EXTTSRATE    66  // External TS rate clock input
#define DT_IOCONFIG_EXTRATIO     67  // External TS rate clock with ratio
#define DT_IOCONFIG_INTTSRATE    68  // Internal TS rate clock reference
#define DT_IOCONFIG_LOCK2INP     69  // Lock TS rate to input port

#define DT_IOCONFIG_COUNT        70  // Number of DT_IOCONFIG codes
#define DT_IOCONFIG_NONE         -1  // DT_IOCONFIG code is not applicable
