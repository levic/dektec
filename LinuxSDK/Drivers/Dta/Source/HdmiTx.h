//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* HdmiTx.h *#*#*#*#*#*#*#*#*# (C) 2015-2016 DekTec
//


// Flags for video definition
#define DT_HDMI_FLAGS_HSPOL_NEG         0x0001
#define DT_HDMI_FLAGS_VSPOL_NEG         0x0002
#define DT_HDMI_FLAGS_FRACMODE          0x0004  // divide by 1.001
#define DT_HDMI_FLAGS_INTERLACED        0x0008
#define DT_HDMI_FLAGS_CLKRATIO_4K       0x0010
#define DT_HDMI_FLAGS_PIXELREP          0x0020
#define DT_HDMI_FLAGS_HDMI_VIC          0x0040
#define DT_HDMI_FLAGS_MULTIPLMODE       0x0080  // multiply by 1.001
#define DT_HDMI_FLAGS_ASPECT_RATIO_4_3  0x0100


// HDMI I2C address defines
#define DDC_I2C_ADDRESS     0xA0      // Display Data Channel I2C address
#define EDDC_I2C_ADDRESS    0x60      // Enhanced Display Data Channel (Segment register)
#define SCDC_I2C_ADDRRESS   0xA8      // Status and Control Data Channel

//
#define EDID_BLOCK_SIZE     128

#define EDID_EXTENSION_BLOCK_TAG_CEA       0x02 // CEA 861 Series Extension
#define EDID_EXTENSION_BLOCK_TAG_VTB       0x10 // Video Timing Block Extension
#define EDID_EXTENSION_BLOCK_TAG_DI        0x40 // Display Information Extension
#define EDID_EXTENSION_BLOCK_TAG_LS        0x50 // Localized String Extension
#define EDID_EXTENSION_BLOCK_TAG_DPVL      0x60 // Digital Packet Video Link Extension
#define EDID_EXTENSION_BLOCK_TAG_BLOCKMAP  0xF0 // Extension Block Map
#define EDID_EXTENSION_BLOCK_TAG_MANUF     0xFF // Extension defines by display manuf.

typedef struct _DtaHdmiVidStd
{
    Int  m_VidStd;
    Int  m_AspectRatio;
} DtaHdmiVidStd;

// Init errors
#define DT_HDMITX_E_NOT_INITIALISED       0x00000000  // No error state
#define DT_HDMITX_E_STATE_RESET           0x00000001  // No error state
#define DT_HDMITX_E_SCDC_INIT             0x00000002
#define DT_HDMITX_E_READ_EDID             0x00000004
#define DT_HDMITX_E_UPD_VIDSTD            0x00000008
#define DT_HDMITX_E_DISCONNECTED          0x00000010
#define DT_HDMITX_E_HPD_FORCE_DETECTED    0x00000020

// Error switching vid std
#define DT_HDMITX_E_PLL_LOCKED_TIMEOUT    0x00000100
#define DT_HDMITX_E_SCDC_SET_TMDS_CONFIG  0x00000200
#define DT_HDMITX_E_SN65DP159_INIT        0x00000400

// Force states
#define DT_HDMITX_E_FORCE_MON_DETECTED    0x00010000
#define DT_HDMITX_E_FORCE_TEST_PICTURE    0x00020000
#define DT_HDMITX_E_VIDSTD_FORCED         0x00040000
#define DT_HDMITX_E_YCBCR_422_FORCED      0x00080000
#define DT_HDMITX_E_YCBCR_444_FORCED      0x00100000
#define DT_HDMITX_E_HDR_FORCED            0x00200000

// Warnings
#define DT_HDMITX_E_VIDSTD_NOT_FOUND      0x01000000
#define DT_HDMITX_E_MON_NOT_DETECTED      0x02000000
#define DT_HDMITX_E_USE_SD_SDI_4_3        0x04000000
#define DT_HDMITX_E_VIDSTD_NOT_SUPP       0x08000000
#define DT_HDMITX_E_USE_3G                0x10000000
#define DT_HDMITX_E_VIDSTD_UNKNOWN        0x20000000
#define DT_HDMITX_E_HDR_ENABLED           0x40000000
#define DT_HDMITX_E_O_DISABLED            0x80000000

#define DT_HDMITX_E_MASK_HOTPLUG          0x000000FF    // Init Errors
#define DT_HDMITX_E_MASK_UPDVIDSTD        0x0000FF00    // VidStd Errors
#define DT_HDMITX_E_MASK_FORCE_NSUPPORT   0x00FF0000    // Forced States
#define DT_HDMITX_E_MASK_WARNING          0xFF000000    // Warning

typedef struct _DtaHdmiTx
{
    DtaDeviceData*  m_pDvcData;
    Int  m_PortIndex;
    DtMutex  m_StateLock;           // Locks HDMI status and register access
    DtDpc  m_HotplugDpc;
    DtWorkItem  m_HotplugWorkItem;
       
    UInt16  m_HdmiRegsOffset;       // Firmware register mapping
    volatile UInt8*  m_pHdmiRegs;

    Int  m_SelAudStd;               // DT_AUDSTD_...
    DtaHdmiVidStd  m_SelVidStd;     // DT_VIDSTD_... Video standard as selected by user 
                                    // (can be different to the actual HDMI output format 
                                    //  in case of 12G->3G conversion of test screen)
    DtaHdmiVidStd  m_UsedVidStd;    // DT_VIDSTD_... Video standard used at HDMI output
    Int  m_SelVidMode;              // DT_HDMI_VIDMOD_... color mode as selected by user
    Int  m_UsedVidMode;             // DT_HDMI_VIDMOD_... color mode used at HDMI output
    Bool  m_EdidError;              // TRUE: Error in edid
    Bool  m_MonDetected;            // TRUE if a monitor is detected
    Bool  m_DisableHdmiOutput;      // TRUE if HDMI output must be disabled
    Bool  m_DisableEdidCheck;       // TRUE: If user wants to force format also when not
                                    // supported by monitor
    Bool  m_ForceTestPicture;       // TRUE: if user wants to force the test picture
    Bool  m_UsingTestPicture;       // TRUE: if currently using test picture
    Bool  m_ForceMonitorDetected;   // TRUE: if user wants to force monitor detected state
    Int  m_Colorimetry;             // DEFAULT: 0, can be overruled by user
    Int  m_ExtendedColorimetry;     // DEFAULT: 0, can be overruled by user
    Int  m_AudioIndexCh1;           // DEFAULT: 1
    Int  m_AudioIndexCh2;           // DEFAULT: 2
    Int  m_AudioType;               // NOT USED, can be used for multichannel or MASK

    UInt32  m_HdmiErrorState;       // HDMI INIT ERROR STATE


    // Supported features
    UInt64  m_MonSupportedFormats;  // From DTD + SVD
    UInt64  m_MonSupportedAudio;    // From ...??...
    Bool  m_SupportHdmi;            // HDMI VSDB (IEEE 0xc03) available
    Bool  m_SupportHdr;             // From extented data block 0x6: HDR static metadata
    Bool  m_SupportYCbCr444;        // From CEA Extension byte3
    Bool  m_SupportYCbCr422;        // From CEA Extension byte3
    Bool  m_SupportBasicAudio;      // From CEA Extension byte3
    Bool  m_SupportScDc;            // From HF VSDB
    Bool  m_SupportMonitorRangeLimits; // Monitor Descriptor Block
    UInt  m_SupportedHdrFormats;    // From extented data block 0x6: HDR static metadata
    
    // Monitor range limits: If forcing formats, always check this before enabling
    UInt  m_MaxPixelClk;            // From Monitor Descriptor Block (Monitor Range Limits)
    UInt  m_MinVRate;               // For interlaced = Field rate
    UInt  m_MaxVRate;               // For interlaced = Field rate
    UInt  m_MinHRate;
    UInt  m_MaxHRate;
} DtaHdmiTx;

DtStatus  DtHdmiTxInit(DtaNonIpPort* pNonIpPort);
DtStatus  DtHdmiTxInitPowerUp(DtaNonIpPort* pNonIpPort);
DtStatus  DtHdmiTxInitPowerUpPost(DtaNonIpPort* pNonIpPort);
DtStatus  DtHdmiTxUpdateVidStd(DtaNonIpPort* pNonIpPort, DtaHdmiVidStd VidStd, Bool Force);
DtStatus  DtHdmiTxInterruptDisable(DtaNonIpPort* pNonIpPort);
DtStatus  DtHdmiTxInterruptEnable(DtaNonIpPort* pNonIpPort);
DtStatus  DtHdmiTxPowerDown(DtaNonIpPort* pNonIpPort);
Bool  DtHdmiTxInterrupt(DtaNonIpPort*  pNonIpPort);
DtStatus  DtHdmiTxIoctl(DtaDeviceData* pDvcData, DtFileObject* pFile, DtIoctlObject* pIoctl);
void  DtHdmiTxReleaseForcedStates(DtaNonIpPort* pNonIpPort);