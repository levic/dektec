// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* CDTAPI_AvFifo.h *#*#*#*#*#*#*# (C) 2022-2023 DekTec
//
#ifndef CDTAPI_AVFIFO_H
#define CDTAPI_AVFIFO_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "CDTAPI.h"

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ HELPERS +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Generic struct to define an exact ratio.
typedef struct Ratio
{
    int Numerator;
    int Denominator;
} Ratio, FrameRate;

// Defines the video dimensions in pixels.
typedef struct VideoSize
{
    int Width;
    int Height;
} VideoSize;

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ St2110 Rx +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Defines the frame (pixel) format of incoming ST2110 video.
// AvFifo::RxFifo assumes that the video has this format.
typedef enum St2110_RxFrameFormatC
{
    St2110_RxFrameFormat_Raw, // Raw underlying format without any conversion.
    St2110_RxFrameFormat_Uyvy422_8b,        // 8-bit UYVY.
    St2110_RxFrameFormat_Uyvy422_10b,       // 10-bit packed UYVY.
    St2110_RxFrameFormat_Uyvy422_10b_to_8b, // Line format is 10-bit packed UYVY,
                                            // converted to 8-bit UYVY in RxFifo.
    St2110_RxFrameFormat_Yuv422p_8b // 8-bit planar YUV (3 planes: Y,U,V).
} St2110_RxFrameFormat;

// Defines the audio format.
typedef enum St2110_AudioFormatC
{
    St2110_AudioFormat_L16BE,                      // 16-bit PCM, big endian.
    St2110_AudioFormat_L24BE,                      // 24-bit PCM, big endian.
    St2110_AudioFormat_Raw
    // St2110_AudioFormat_L16LE,
    // St2110_AudioFormat_L24LE
} St2110_AudioFormat;

// Configures the reception of ST2110 audio.
typedef struct St2110_RxConfigAudioC
{
    St2110_AudioFormat Format;
    int SampleRate;      // Sample rate in Hz.
} St2110_RxConfigAudio;

// Configures the reception of ST2110 video.
typedef struct St2110_RxConfigVideoC
{
    St2110_RxFrameFormat Format;
} St2110_RxConfigVideo;

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ St2110 Tx +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Defines the video scanning mode.
typedef enum St2110_VideoScanning
{
    St2110_VideoScanning_Progressive,
    St2110_VideoScanning_Interlaced,
    St2110_VideoScanning_PsF,
} St2110_VideoScanning;

// Defines the pixel format of ST2110 video frames to be transmitted.
// AvFifo::TxFifo assumes that the video frames supplied by the application have
// this format.
typedef enum St2110_TxFrameFormat
{
    St2110_TxFrameFormat_Uyvy422_8b, // 8-bit UYVY.
    St2110_TxFrameFormat_Uyvy422_10b // 10-bit packed UYVY.
} St2110_TxFrameFormat;

// Defines the video packing mode.
typedef enum St2110_PackingMode
{
    St2110_PackingMode_General, // Only pgroup size restriction.
    St2110_PackingMode_Block    // Multiple of 180 octets per packet.
} St2110_PackingMode;

// Defines the packet scheduling method.
typedef enum St2110_Scheduling
{
    St2110_Scheduling_Linear,
    St2110_Scheduling_Gapped
} St2110_Scheduling;

// Defines the video packing parameters.
typedef struct St2110_VideoPacking
{
    int OneLinePerPacket;
    St2110_PackingMode PackingMode;
    int PayloadSize;
} St2110_VideoPacking;

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Tx Configuration -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Configures the transmission of ST2110 ancillary data.
typedef struct St2110_TxConfigAnc
{
    St2110_VideoScanning VideoScanning;
} St2110_TxConfigAnc;

// Configures the transmission of ST2110 audio.
typedef struct St2110_TxConfigAudio
{
    St2110_AudioFormat Format;
    int NumChannels;
    int NumSamplesPerIpPacket;
    int SampleRate; // Sample rate in Hz.
} St2110_TxConfigAudio;

// Configures the timing aspects of ST2110 video transmission.
typedef struct St2110_VideoTiming
{
    FrameRate Rate; // Progressive video: frame rate.
                    // Interlaced/PsF video: field rate.
    St2110_Scheduling Scheduling;
    St2110_VideoScanning VideoScanning;
} St2110_VideoTiming;

// Configures the transmission of RAW ST2110 video.
typedef struct St2110_TxConfigRawVideo
{
    Ratio ActiveVideo;    // Active video time relative to total line time.
    int Is420;            // 4:2:0 chroma subsampling?
    int NumRows;          // Total #rows in a frame.
    St2110_VideoPacking Packing; // Packet packing parameters.
    struct
    {
        int NumBytes;  // #bytes in a ST2110-20 pgroup (pixel group).
        int NumPixels; // #pixels in a ST2110-20 pgroup.
    } PGroup;
    int RowSize;        // Total #bytes in a row.
    St2110_VideoTiming Timing; // Video timing parameters.
    int TrOffset;       // The PTP transmit time offset with respect to the
                        // most recent integer multiple of the time period
                        // between consecutive frames of video at the
                        // prevailing frame rate. Starts at the EPOCH time.
} St2110_TxConfigRawVideo;

// Configures the transmission of ST2110 video.
typedef struct St2110_TxConfigVideo
{
    St2110_TxFrameFormat Format;
    St2110_VideoPacking Packing; // Packet packing parameters.
    VideoSize Resolution; // Resolution of the active video.
    St2110_VideoTiming Timing;   // Video timing parameters.
} St2110_TxConfigVideo;

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ AvFifo_IpPars +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Selects between the IPv4 and IPv6 protocol.
typedef enum IpProtocolVersion
{
    IpProtocolVersion_IPv4,
    IpProtocolVersion_IPv6
} IpProtocolVersion;

// Selects between RTP-over-UDP and plain UDP without RTP.
typedef enum IpTransportProtocol
{
    IpTransportProtocol_Rtp,
    IpTransportProtocol_Udp
} IpTransportProtocol;

// Defines the address filter for source-specific multicast.
typedef struct IpSrcFlt
{
    uint8_t IpAddr[16]; // 4 (IPv4) or 16 (IPv6) IP address bytes.
    int Port;           // Port number.
} IpSrcFlt;

// Defines an IP endpoint.
typedef struct AvFifo_IpPars
{
    // IP address parameters.
    uint8_t IpAddr[16]; // IP address as 4 (IPv4) or 16 (IPv6) bytes.
    IpProtocolVersion IpVersion;
    int Port;
    IpSrcFlt* SrcFlt; // Source-specific multicast: optional source filter.
    int NSrcFlt;      // Number of source filters

    // Other IP parameters.
    int DiffServ;        // Differentiated service field.
    uint8_t Gateway[16]; // Optional gateway parameter.
    int RtpPayloadType;  // Payload type in RTP packet.
    int TimeToLive;      // Time to live.
    IpTransportProtocol TransportProtocol;
    struct
    {
        int Id;       // VLAN ID.
        int Priority; // VLAN priority.
    } Vlan;           // VLAN parameters.
} AvFifo_IpPars;

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ AvFifo_Frame +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

typedef struct AvFifo_FrameC {
    // Timestamps.
    uint32_t RtpTime;            // RTP timestamp assigned to RTP packets of the frame.
    DtTimeOfDay ToD;              // Time-of-day of the first sample of the frame.

    // Attributes.
    uint8_t* Data;          // Gets raw data pointer to the frame BLOB.
    int Field;                   // Video only: field indication for Interlaced/PsF.
    int NumValidBytes;           // Number of valid bytes in the frame-data BLOB.
    size_t Size;            // Gets frame BLOB's size in bytes (excl. oversizing).

    // Helper functions meaningful only when SMPTE 2110 video frames are received.
    int Is420;             // Is 4:2:0 chroma subsampling used?
    int NumRows;            // Total number of rows in a video frame.
} AvFifo_Frame;

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ AvFifo_RxFifo +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

typedef struct RxStatistics
{
    int FramesOk;         // Number of correctly received frames.
    int FramesIncomplete; // Number of frames with missing IP packets.
    int FramesSizeError;  // Number of frames with unexpected size.
    int Gaps;             // Number of discontinuities in RTP sequence numbers.
    int IpPacketErrors;   // Number of IP packets with corrupted headers.
    int DroppedFrames;    // Number of dropped frames due to FIFO full.
    int SyncErrors;       // Number of out-of-sync errors.
} RxStatistics;

typedef struct AvFifo_RxFifoC AvFifo_RxFifo;

AvFifo_RxFifo* AvFifo_RxFifo_Alloc(void);
void AvFifo_RxFifo_Free(AvFifo_RxFifo* Fifo);
void AvFifo_RxFifo_Freep(AvFifo_RxFifo** Fifo);

unsigned int AvFifo_RxFifo_Attach(AvFifo_RxFifo* Fifo, const DtDevice* Device, int Port);
unsigned int AvFifo_RxFifo_Detach(AvFifo_RxFifo* Fifo);
unsigned int AvFifo_RxFifo_Clear(AvFifo_RxFifo* Fifo);
unsigned int AvFifo_RxFifo_Start(AvFifo_RxFifo* Fifo);
unsigned int AvFifo_RxFifo_Stop(AvFifo_RxFifo* Fifo);

unsigned int AvFifo_RxFifo_ConfigureAudio(AvFifo_RxFifo* Fifo, const St2110_RxConfigAudio* Config);
unsigned int AvFifo_RxFifo_ConfigureVideo(AvFifo_RxFifo* Fifo, const St2110_RxConfigVideo* Config);
unsigned int AvFifo_RxFifo_SetIpPars(AvFifo_RxFifo* Fifo, const AvFifo_IpPars* IpPars);

int AvFifo_RxFifo_GetFifoLoad(const AvFifo_RxFifo* Fifo);
AvFifo_Frame* AvFifo_RxFifo_Read(AvFifo_RxFifo* Fifo);

unsigned int AvFifo_RxFifo_ReturnToMemPool(AvFifo_RxFifo* Fifo, AvFifo_Frame* Frame);

int AvFifo_RxFifo_GetMaxSize(const AvFifo_RxFifo* Fifo);
void AvFifo_RxFifo_SetMaxSize(AvFifo_RxFifo* Fifo, int Size);

RxStatistics AvFifo_RxFifo_GetStatistics(const AvFifo_RxFifo* Fifo);

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ AvFifo_TxFifo +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

typedef struct TxStatistics
{
    int FramesOk; // Number of frames transmitted.
} TxStatistics;

typedef struct AvFifo_TxFifoC AvFifo_TxFifo;

AvFifo_TxFifo* AvFifo_TxFifo_Alloc(void);
void AvFifo_TxFifo_Free(AvFifo_TxFifo* Fifo);
void AvFifo_TxFifo_Freep(AvFifo_TxFifo** Fifo);

unsigned int AvFifo_TxFifo_Attach(AvFifo_TxFifo* Fifo, const DtDevice* Device, int Port);
unsigned int AvFifo_TxFifo_Detach(AvFifo_TxFifo* Fifo);
unsigned int AvFifo_TxFifo_Clear(AvFifo_TxFifo* Fifo);
unsigned int AvFifo_TxFifo_Start(AvFifo_TxFifo* Fifo);
unsigned int AvFifo_TxFifo_Stop(AvFifo_TxFifo* Fifo);

unsigned int AvFifo_TxFifo_ConfigureAudio(AvFifo_TxFifo* Fifo, const St2110_TxConfigAudio* Config);
unsigned int AvFifo_TxFifo_ConfigureVideo(AvFifo_TxFifo* Fifo, const St2110_TxConfigVideo* Config);
unsigned int AvFifo_TxFifo_SetIpPars(AvFifo_TxFifo* Fifo, const AvFifo_IpPars* IpPars);

int AvFifo_TxFifo_GetFifoLoad(const AvFifo_TxFifo* Fifo);
unsigned int AvFifo_TxFifo_Write(AvFifo_TxFifo* Fifo, AvFifo_Frame* Frame);

AvFifo_Frame* AvFifo_TxFifo_GetFromMemPool(AvFifo_TxFifo* Fifo, int Size);

int AvFifo_TxFifo_GetMaxSize(const AvFifo_TxFifo* Fifo);
void AvFifo_TxFifo_SetMaxSize(AvFifo_TxFifo* Fifo, int Size);

TxStatistics AvFifo_TxFifo_GetStatistics(const AvFifo_TxFifo* Fifo);

typedef enum ChromaSubsampling
{
    ChromaSubsampling_Key,
    ChromaSubsampling_420,
    ChromaSubsampling_422,
    ChromaSubsampling_444,
} ChromaSubsampling;

typedef struct FrameProperties
{
    int Is420;
    int NLines;
    int BytesPerLine;
    int BytesPerFrame;

    int Width;
    int Height;
    int IsInterlaced;
    ChromaSubsampling Subsampling;
    int BitDepth;
} FrameProperties;
int GetFrameProperties(const AvFifo_Frame* Frame, FrameProperties* Properties);
const char* GetLastException(void);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ TIMING HELPER FUNCTIONS +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Three types of timing helper functions are defined:
//  1. Alignment of time-of-day to the media clock grid.
//  2. (namespace St2110) Conversion from RTP timestamp to time-of-day.
//  3. (namespace St2110) Conversion from time-of-day to RTP timestamp.
// 
// Functions in category 2 and 3 use implicit alignment: if the input time is on the 
// media clock grid (with a small margin), the output time will be aligned to grid.
// For more information about the use of these functions, refer to the A/V FIFO Tutorial.

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- 1. Time-of-day grid alignment -.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Aligns a given time-of-day to the grid for AUDIO, given the audio sample rate.
DtTimeOfDay Tod2Grid_Audio(const DtTimeOfDay*, int SampleRate);

// Aligns a given time-of-day to the grid for VIDEO, given the video frame or field rate.
DtTimeOfDay Tod2Grid_Video(const DtTimeOfDay*, const FrameRate*);

// -.-.-.-.-.-.-.-.-.-.-.-.-.- 2. RTP Timestamp => Time-of-Day -.-.-.-.-.-.-.-.-.-.-.-.-.-

// The time-of-day computed by Rtp2Tod_Audio and Rtp2Tod_Video will be on the media clock
// grid if the given RTP time-of-day is on grid (+/- 1 RTP clock tick).
// The approximate time-of-day must be passed so that the function can compute the time 
// window in which the RTP timestamp is located. Typically the current time can be used.

// Converts an RTP timestamp to time-of-day for AUDIO, given the audio sample rate and 
// the approximate time-of-day.
DtTimeOfDay Rtp2Tod_Audio(uint32_t RtpTime, const DtTimeOfDay*, int SampleRate);

// Converts an RTP timestamp to time-of-day for VIDEO, given the frame or field rate
// and the approximate time-of-day.
DtTimeOfDay Rtp2Tod_Video(uint32_t RtpTime, const DtTimeOfDay*);

// -.-.-.-.-.-.-.-.-.-.-.-.-.- 3. Time-of-day => RTP timestamp -.-.-.-.-.-.-.-.-.-.-.-.-.-

// The computed RTP timestamp will be on grid if the given time-of-day is on grid.
// If time-of-day is off grid, the RTP timestamp will NOT be aligned to the grid.

// Converts time-of-day to an RTP timestamp for AUDIO, given the audio sample rate in Hz.
uint32_t Tod2Rtp_Audio(const DtTimeOfDay*, int SampleRate);

// Converts time-of-day to an RTP timestamp for VIDEO, given the frame or field rate.
uint32_t Tod2Rtp_Video(const DtTimeOfDay*);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // #ifndef CDTAPI_AVFIFO_H
