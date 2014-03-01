//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIoConfigDefs.h *#*#*#*#*#*#*#*#*#* (C) 2014 DekTec
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
#define DT_IOCONFIG_BW           7   // Isochronous bandwidth allocated
#define DT_IOCONFIG_FAILSAFE     8   // A fail-over relay is available
#define DT_IOCONFIG_FRACMODE     9   // Fractional mode is supported
#define DT_IOCONFIG_GENLOCKED    10  // Locked to a genlock reference
#define DT_IOCONFIG_GENREF       11  // Genlock reference input
#define DT_IOCONFIG_SWS2APSK     12  // DVB-S2 APSK mode
#define DT_IOCONFIG_TRUE         13  // Turn I/O capability on
#define DT_IOCONFIG_FALSE        14  // Turn I/O capability off
#define DT_IOCONFIG_DISABLED     15  // Port is disabled
#define DT_IOCONFIG_INPUT        16  // Uni-directional input
#define DT_IOCONFIG_OUTPUT       17  // Uni-directional output
#define DT_IOCONFIG_SHAREDANT    18  // Get antenna signal from another port
#define DT_IOCONFIG_DBLBUF       19  // Double buffered output
#define DT_IOCONFIG_LOOPS2L3     20  // Loop-through of DVB-S2 in L3-frames
#define DT_IOCONFIG_LOOPS2TS     21  // Loop-through of an DVB-S(2) input
#define DT_IOCONFIG_LOOPTHR      22  // Loop-through of another input
#define DT_IOCONFIG_3GSDI        23  // 3G-SDI
#define DT_IOCONFIG_ASI          24  // DVB-ASI transport stream
#define DT_IOCONFIG_DEMOD        25  // Demodulation
#define DT_IOCONFIG_HDSDI        26  // HD-SDI
#define DT_IOCONFIG_IFADC        27  // IF A/D converter
#define DT_IOCONFIG_IP           28  // Transport stream over IP
#define DT_IOCONFIG_MOD          29  // Modulator output
#define DT_IOCONFIG_RS422        30  // RS422 port
#define DT_IOCONFIG_SDI          31  // SD-SDI
#define DT_IOCONFIG_SPI          32  // DVB-SPI transport stream
#define DT_IOCONFIG_SPISDI       33  // SD-SDI on a parallel port
#define DT_IOCONFIG_1080P50      34  // 1080p/50
#define DT_IOCONFIG_1080P59_94   35  // 1080p/59.94
#define DT_IOCONFIG_1080P60      36  // 1080p/60
#define DT_IOCONFIG_1080I50      37  // 1080i/50
#define DT_IOCONFIG_1080I59_94   38  // 1080i/59.94
#define DT_IOCONFIG_1080I60      39  // 1080i/60
#define DT_IOCONFIG_1080P23_98   40  // 1080p/23.98
#define DT_IOCONFIG_1080P24      41  // 1080p/24
#define DT_IOCONFIG_1080P25      42  // 1080p/25
#define DT_IOCONFIG_1080P29_97   43  // 1080p/29.97
#define DT_IOCONFIG_1080P30      44  // 1080p/30
#define DT_IOCONFIG_720P23_98    45  // 720p/23.98
#define DT_IOCONFIG_720P24       46  // 720p/24
#define DT_IOCONFIG_720P25       47  // 720p/25
#define DT_IOCONFIG_720P29_97    48  // 720p/29.97
#define DT_IOCONFIG_720P30       49  // 720p/30
#define DT_IOCONFIG_720P50       50  // 720p/50
#define DT_IOCONFIG_720P59_94    51  // 720p/59.94
#define DT_IOCONFIG_720P60       52  // 720p/60
#define DT_IOCONFIG_525I59_94    53  // 525i/59.94
#define DT_IOCONFIG_625I50       54  // 625i/50
#define DT_IOCONFIG_SPI525I59_94 55  // SPI 525i/59.94
#define DT_IOCONFIG_SPI625I50    56  // SPI 625i/50
#define DT_IOCONFIG_RFCLKEXT     57  // External RF clock input
#define DT_IOCONFIG_RFCLKINT     58  // Internal RF clock reference
#define DT_IOCONFIG_SPICLKEXT    59  // External clock input
#define DT_IOCONFIG_SPICLKINT    60  // Internal clock reference
#define DT_IOCONFIG_SPIFIXEDCLK  61  // SPI fixed clock with valid signal
#define DT_IOCONFIG_SPIDVBMODE   62  // SPI DVB mode
#define DT_IOCONFIG_SPISER8B     63  // SPI serial 8-bit mode
#define DT_IOCONFIG_SPISER10B    64  // SPI serial 10-bit mode
#define DT_IOCONFIG_SPILVDS1     65  // LVDS1
#define DT_IOCONFIG_SPILVDS2     66  // LVDS2
#define DT_IOCONFIG_SPILVTTL     67  // LVTTL
#define DT_IOCONFIG_EXTTSRATE    68  // External TS rate clock input
#define DT_IOCONFIG_EXTRATIO     69  // External TS rate clock with ratio
#define DT_IOCONFIG_INTTSRATE    70  // Internal TS rate clock reference
#define DT_IOCONFIG_LOCK2INP     71  // Lock TS rate to input port

#define DT_IOCONFIG_COUNT        72  // Number of DT_IOCONFIG codes
#define DT_IOCONFIG_NONE         -1  // DT_IOCONFIG code is not applicable
