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