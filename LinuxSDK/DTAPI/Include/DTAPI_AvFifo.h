// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DTAPI_AvFifo.h *#*#*#*#*#*#*#*#*#*# (C) 2023 DekTec
//
// DTAPI - AvFifo API - Audio/video FIFO interface for DekTec devices
//
// The AvFifo API is a FIFO-based streaming API designed for streaming SMPTE 2110 and 
// SMPTE 2022-5/6 audio and video streams to DekTec network interface cards (NICs). 
// 
// Detailed documentation for this API can be found in the "DTAPI Reference - AvFifo.pdf"
// manual, which will be referred to as the "AvFifo Manual". This manual offers both
// function reference information and a tutorial, providing comprehensive guidance on
// the usage of the API.
//
// Supported DekTec devices: DTA-2110
// Supported A/V standards : SMPTE 2110-20/21, SMPTE 2022-5/6
//
// Namespaces:
//   Dtapi::AvFifo             General classes defining the A/V FIFO Interface.
//   Dtapi::AvFifo::St2022     Classes specific for SMPTE 2022.
//   Dtapi::AvFifo::St2110     Classes specific for SMPTE 2110.
//

#pragma once

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

#include "DTAPI.h"                  // C++ API for DekTec PCIe-, USB- and IP devices.
#include <array>                    // Standard library: Fixed-size array handling.
#include <stdexcept>                // Standard library: Exception handling.
#include <vector>                   // Standard library: Dynamic-size array handling.

// The AvFifo classes are defined in namespace Dtapi::AvFifo.
namespace Dtapi
{
namespace AvFifo
{

using namespace std;                // Avoid std:: prefix for standard library calls in
                                    // namespace scope Dtapi::AvFifo.

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Enumerations +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Indicates RxFifo/TxFifo status codes.
enum class FifoStatus
{
    Ok,                             // No error, FIFO is operating correctly.
    DstMacResolveFail,              // Failed to resolve the destination MAC address.
                                    // This could be due to the IP address not existing.
    LinkDown,                       // The network link is completely down.
                                    // This can be due to a disabled network port.
    MulticastJoinFail,              // Failed to register the multicast address.
    Unplugged                       // The network cable is disconnected or unplugged.
};

// Indicates whether the user prefers a software or a hardware pipe.
enum class HwOrSwPipe
{
    Auto,                           // DTAPI chooses suitable pipe type.
    ForceHwPipe,                    // Force hardware pipe, exception if none available.
    PreferHwPipe,                   // Prefer hardware pipe, accept software pipe.
    UseSwPipe                       // Use software pipe.
};

// Selects between the IPv4 and IPv6 protocol.
enum class IpProtocolVersion {IPv4, IPv6};

// Selects between RTP-over-UDP and plain UDP without RTP.
enum class IpTransportProtocol {Rtp, Udp};

// Enumerates strategies to handle receive overflow in the RxFifo.
enum class RxFifoOverflowStrategy
{
    DropFrame,                      // When the RxFifo is full: Drop the received frame.
    ThrowException,                 // When the RxFifo is full: Throw exception.
                                    // The exception is thrown in the first subsequent 
                                    // user call to RxFifo::GetFifoLoad().
};

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SMPTE 2022 enumerations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

namespace St2022
{
    // Selects between FEC disabled and FEC enabled (not implemented yet).
    enum class FecMode {Disable};

    // Enumerates SMPTE 2022-7 modes.
    enum class LinkMode {Single, Dual};
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SMPTE 2110 enumerations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

namespace St2110
{
    // Enumerates the supported SMPTE 2110 audio formats.
    enum class AudioFormat
    {
        L16BE,                      // 16-bit PCM, big endian.
        L24BE,                      // 24-bit PCM, big endian.
        Raw
    };

    // Enumerates the SMPTE 2110 video packing modes.
    enum class PackingMode
    {
        General,                    // Only pgroup size restriction.
        Block                       // Multiple of 180 octets per packet.
    };

    // Enumerates the supported pixel formats for incoming SMPTE 2110 video.
    enum class RxFrameFormat
    {
        Raw,                        // Raw underlying format without any conversion. 
        Uyvy422_8b,                 // 8-bit UYVY packed pixel format.
        Uyvy422_10b,                // 10-bit packed UYVY pixel format.
        Uyvy422_10b_to_8b,          // 10-bit packed UYVY converted to 8-bit UYVY. 
        Yuv422p_8b                  // 8-bit planar YUV (3 planes: Y,U,V).
    };

    // Enumerates the packet scheduling method.
    enum class Scheduling {Linear, Gapped};

    // Enumerates the supported pixel formats for transmitting SMPTE 2110 video.
    // AvFifo::TxFifo assumes that the video frames supplied by the application have
    // this format.
    enum class TxFrameFormat
    {
        Uyvy422_8b,                 // 8-bit UYVY.
        Uyvy422_10b                 // 10-bit packed UYVY.
    };

    // Enumerates the supported video scanning modes for received and transmitted video.
    enum class VideoScanning {Progressive, Interlaced, PsF};
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Helper classes and structs +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Stores allocator metadata for a frame BLOB.
struct BlobMetadata
{
    int Alignment{32};              // Address alignment (e.g. 32 = 32-byte alignment).
    int ExtraSize{0};               // Extra bytes allocated at the end of the BLOB.
};

// Represents an exact rational number as a fraction with a numerator and a denominator.
struct ExactRatio 
{
    int Numerator{-1}; int Denominator{-1};
    ExactRatio(int Num=-1, int Den=-1) : Numerator(Num), Denominator(Den) {}
};

// Defines a frame rate as an exact ratio.
// NOTE: For progressive video, FrameRate is the video frame rate.
//       However, for interlaced/PsF video, FrameRate is the video field rate!
using FrameRate = ExactRatio;

// Defines the address filter for source-specific multicast.
struct IpSrcFlt
{
    array<uint8_t, 16> IpAddr;      // 4 (IPv4) or 16 (IPv6) IP address bytes.
    int Port{-1};                   // Port number.
};

// Encapsulates the IP configuration parameters necessary for establishing 
// and managing a network connection in the context of RxFifo or TxFifo operations.
struct IpPars
{
    // IP address parameters.
    array<uint8_t, 16> IpAddr{};    // IP address as 4 (IPv4) or 16 (IPv6) bytes.
    IpProtocolVersion IpVersion{IpProtocolVersion::IPv4};
    int Port{-1};
    vector<IpSrcFlt> SrcFlt;        // Source-specific multicast: optional source filter.

    // Other IP parameters.
    int DiffServ{34 << 2};          // Differentiated service field.
    array<uint8_t, 16> Gateway{};   // Optional gateway parameter.
    int RtpPayloadType{0};          // Payload type in RTP packet.
    int TimeToLive{64};             // Time to live.
    IpTransportProtocol TransportProtocol{IpTransportProtocol::Udp};
    struct {
        int Id{0};                  // VLAN ID.
        int Priority{0};            // VLAN priority.
    } Vlan;                         // VLAN parameters.
};

// Receive statistics as a set of counter values.
struct RxStatistics
{
    int FramesOk{0};                // Number of correctly received frames.
    int FramesIncomplete{0};        // Number of frames with missing IP packets.
    int FramesSizeError{0};         // Number of frames with unexpected size.
    int Gaps{0};                    // Number of discontinuities in RTP sequence numbers.
    int IpPacketErrors{0};          // Number of IP packets with corrupted headers.
    int DroppedFrames{0};           // Number of dropped frames due to FIFO full.
    int SyncErrors{0};              // Number of out-of-sync errors.
};

// Transmit statistics as a set of counter values.
struct TxStatistics
{
    int FramesOk{0};                // Number of frames transmitted.
};

// Defines the video dimensions in pixels.
struct VideoSize {int Width{-1}; int Height{-1};};

// Stores Y/U/V pointers/plane sizes for the 8-bit planar YUV format.
// Frame::Yuv422P_8b_GetPlanes() returns a struct with this type.
struct YuvPlanes
{
    uint8_t* Y{nullptr}; uint8_t* U{nullptr}; uint8_t* V{nullptr};
    size_t SizeY{0}, SizeU{0}, SizeV{0};
};

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SMPTE 2022 structs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

namespace St2022
{
    // Configures the reception of an SMPTE 2022-5/6 stream.
    struct RxConfig
    {
        // Enable FEC decoding.
        // If false, FEC decoding is disabled even if FEC packets are present.
        bool EnableFecDecoding{false};

        // SMPTE 2022-7 parameters.
        struct {
            // IP parameters for redundant link if SMPTE 2022-7 mode is 'Dual'.
            // Parameters of the main link should be specified with RxFifo::SetIpPars.
            IpPars IpParsLink2;

            // Single link (no network redundancy), or dual link (network redundancy).
            LinkMode Mode{LinkMode::Single};

            // Configuration: Maximum bitrate and maximum skew between path 1 and path 2.
            DtIpProfile Profile;
        } St2022_7;
    };

    // Configures the transmission of an SMPTE 2022-5/6 stream.
    struct TxConfig
    {
        // Set the video standard to transmit.
        int VideoStandard{-1};      // DTAPI_VIDSTD_ defines.
        int TrOffset{-1};           // The PTP transmit time offset with respect to the 
                                    // most recent integer multiple of the time period 
                                    // between consecutive frames of video at the 
                                    // prevailing frame rate. Starts at the EPOCH time. 
                                    // If the value is -1, the TroDefault is used.
        int PayloadSize{-1};
        St2022::FecMode FecMode{St2022::FecMode::Disable};

        // SMPTE 2022-7 parameters.
        struct {
            // IP parameters for redundant link if SMPTE 2022-7 mode is 'Dual'.
            // Parameters of the main link should be specified with TxFifo::SetIpPars.
            IpPars IpParsPath2;

            // Single link (no network redundancy), or dual link (network redundancy).
            LinkMode Mode{LinkMode::Single};
        } St2022_7;
    };
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SMPTE 2110 structs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

namespace St2110
{
    // .-.-.-.-.-.-.-.-.-.- SMPTE 2110 configuration - Helper structs -.-.-.-.-.-.-.-.-.-.

    // Defines the video packing parameters.
    struct VideoPacking
    {
        bool OneLinePerPacket{false};
        St2110::PackingMode PackingMode{St2110::PackingMode::General};
        int PayloadSize{-1};
    };

    // Configures the timing aspects of SMPTE 2110 video transmission.
    struct VideoTiming
    {
        FrameRate Rate;             // Progressive video: frame rate.
                                    // Interlaced/PsF video: field rate.
        St2110::Scheduling Scheduling{St2110::Scheduling::Gapped};
        St2110::VideoScanning VideoScanning{St2110::VideoScanning::Progressive};
    };

    // .-.-.-.-.-.-.-.-.-.-.-.- SMPTE 2110 configuration structs -.-.-.-.-.-.-.-.-.-.-.-.-

    // Configures the reception parameters of an SMPTE 2110 audio stream.
    struct RxConfigAudio
    {
        AudioFormat Format{AudioFormat::Raw};
        int SampleRate{48000};      // Defines the audio sample rate in Hz.
    };

    // Configures the 'raw' reception of SMPTE 2110 streams.
    // This allows users direct access to raw SMPTE 2110 data.
    struct RxConfigRaw
    {
        // If true, the RTP header will be included in the received data, allowing 
        // parsing of RTP header fields.
        bool IncludeRtpHeader{false};
        int MaxRate{-1};            // Maximum expected rate (bytes per second).
    };

    // Configures the reception of SMPTE 2110 video.
    struct RxConfigVideo
    {
        RxFrameFormat Format{RxFrameFormat::Raw};
    };
        
    // Configures the transmission of SMPTE 2110 audio.
    struct TxConfigAudio
    {
        AudioFormat Format{AudioFormat::L16BE};
        int NumChannels{2};         // Stereo is 2 channels.
        int NumSamplesPerIpPacket{-1};
        int SampleRate{48000};      // Sample rate in Hz.
    };

    // Configures the 'raw' transmission of SMPTE 2110 streams.
    // This allows users to fully customize the contents of the SMPTE 2110 payload.
    struct TxConfigRaw
    {
        bool CustomRtpHeader{false};// User supplies a custom RTP header?
        int MaxRate{-1};            // Maximum transmission rate in bytes per second.
    };

    // Configures the transmission of raw SMPTE 2110 video.
    struct TxConfigRawVideo
    {
        ExactRatio ActiveVideo{};   // Active video time relative to total line time.
        bool Is420{false};          // 4:2:0 chroma subsampling?
        int NumRows{-1};            // Total #rows in a frame.
        VideoPacking Packing{};       // Packet packing parameters.
        struct {
            int NumBytes{-1};       // #bytes in a SMPTE 2110-20 pgroup (pixel group).
            int NumPixels{-1};      // #pixels in a SMPTE 2110-20 pgroup.
        } PGroup;
        int RowSize{-1};            // Total #bytes in a row.
        VideoTiming Timing{};       // Video timing parameters.
        int TrOffset{-1};           // The PTP transmit time offset with respect to the 
                                    // most recent integer multiple of the time period 
                                    // between consecutive frames of video at the 
                                    // prevailing frame rate. Starts at the EPOCH time. 
    };

    // Configures the transmission of SMPTE 2110 video.
    struct TxConfigVideo
    {
        TxFrameFormat Format{TxFrameFormat::Uyvy422_10b};
        VideoPacking Packing{};     // Video packing parameters.
        VideoSize Resolution{};     // Resolution of the active video.
        VideoTiming Timing{};       // Video timing parameters.
    };
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ Exceptions in the AvFifo API +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// The AvFifo API function calls can potentially throw exceptions when encountering an
// error. These exceptions are derived from the std::exception class, allowing the use of
// the what() method to obtain a human-readable description of the exception.
// When an existing std exception class adequately represents a specific error condition,
// no dedicated AvFifo exception class is defined.

// Signals any errors that occur during driver interactions by the AvFifo implementation.
class DriverError : public runtime_error
{
public:
    explicit DriverError(const string& Message, DTAPI_RESULT Result) 
               : runtime_error(Message + " [" + string(DtapiResult2Str(Result) + "]"s)) {}
};

// Signals an error when a frame with an incorrect format is supplied.
class InvalidFormatError : public runtime_error
{
public:
    using runtime_error::runtime_error;
};

// Signals that a hardware pipe is unavailable, potentially because all pipes are
// currently in use by other processes.
class HardwarePipeUnavailable : public runtime_error
{
public:
    using runtime_error::runtime_error;
};

// Signals a receive FIFO overflow error.
class OverflowError : public runtime_error
{
public:
    using runtime_error::runtime_error;
};

// Signals a transmit scheduling error due to an invalid timestamp, either too far
// in the future or the past.
class SchedulingError : public runtime_error
{
public:
    using runtime_error::runtime_error;
};

// Signals an error in AvFifo call usage or sequencing. Indicates programmer misuse
// rather than internal errors or unexpected conditions.
class UsageError : public logic_error
{
public:
    using logic_error::logic_error;
};

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ AvFifo::Frame +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct Frame -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Represents a single timestamped unit from a video, audio or ANC data stream.
// As far as A/V FIFO is concerned, a Frame is a timestamped bag of bytes (a BLOB).
// 
// NOTE: Terminology may be somewhat confusing: In a video stream, a 'Frame' may 
//       refer to either a video frame or a video field.
// 
// VIDEO:
//   For progressive video, an AvFifo::Frame object contains a video frame.
//   For interlaced/PsF video, an AvFifo::Frame object contains a video FIELD.
//
// RECEIVE:
//   Refer to the comments on AvFifo::RxFifo for configuring Frame reception.
//   For the full interpretation of received frames, you need to obtain metadata about 
//   the frame structure via a 'side channel', e.g. an SDP-based signaling method.
//   Alternatively, many aspects (but not all) of a stream can be auto-detected.
// 
// TRANSMIT:
//   Refer to the comments on AvFifo::TxFifo for configuring Frame transmission.
// 
// MEMORY MANAGEMENT:
//   Upon reading a Frame from AvFifo::RxFifo, the user acquires a pointer to the actual
//   frame data, which resides in a memory pool managed by the A/V FIFO implementation.
//   After the application processes the Frame, it must release it by invoking the
//   RxFifo::ReturnToMemPool(.) method.
//
//   Before transmitting a Frame, first request empty frame memory by calling 
//   TxFifo::GetFrameFromMemPool(.) and write the frame data in the so-obtained Frame. 
//   When you write the frame to AvFifo::TxFifo, ownership is passed to that TxFifo.
//
struct Frame
{
    // Timestamps.
    uint32_t RtpTime{0};            // RTP timestamp assigned to RTP packets of the frame.
    DtTimeOfDay ToD{};              // Time-of-day of the first sample of the frame.

    // Attributes.
    uint8_t* Data() const;          // Gets raw data pointer to the frame BLOB.
    int Field{0};                   // Video only: field indication for Interlaced/PsF.
    int NumValidBytes{0};           // Number of valid bytes in the frame-data BLOB.
    size_t Size() const;            // Gets frame BLOB's size in bytes (excl. oversizing).

    // Helper functions meaningful only when SMPTE 2110 video frames are received.
    bool Is420() const;             // Is 4:2:0 chroma subsampling used?
    int NumRows() const;            // Total number of rows in a video frame.

    // .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Helper functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

    // Computes pointers and sizes of the Y-, U- and V-plane, given the precondition
    // that the video format is RxFrameFormat::Yuv422p_8b.
    YuvPlanes Yuv422P_8b_GetPlanes();
};

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- class AvFifo::RxFifo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Represents a FIFO buffer for Frame objects, originating from a single SMPTE 2022 or
// SMPTE 2110 stream.
//
class RxFifo
{
public:
    // Attaches an RxFifo object to a specified DekTec device and port.
    void Attach(const DtDevice&, int Port, HwOrSwPipe Pref=HwOrSwPipe::Auto, 
                    RxFifoOverflowStrategy OvfStrategy=RxFifoOverflowStrategy::DropFrame);

    // Empties the RxFifo.
    void Clear();

    // -.-.-.-.-.-.-.-.-.-.-.-.-.- Configuration of the RxFifo -.-.-.-.-.-.-.-.-.-.-.-.-.-

    // Overload set of Configure functions. 
    // The function invoked determines the standard of the stream to be received based 
    // on the input argument's type. Each overload configures the RxFifo for stream 
    // reception as per different SMPTE standards. 
    // An optional 'Md' parameter, common to all overloads, allows customization of the
    // alignment and oversizing of received frame BLOBs, using defaults if not specified.

    // Configures for SMPTE 2022-5/6 stream reception.
    void Configure(const St2022::RxConfig&, BlobMetadata Md={});

    // Configures for SMPTE 2110-20 audio stream reception.
    // An optional 'Md' parameter allows customization of the alignment and oversizing
    // of received frame BLOBs, using defaults if not specified.    
    void Configure(const St2110::RxConfigAudio&, BlobMetadata Md={});

    // Configures for receiving streams of any SMPTE 2110 substandard.
    // The raw payload is directly returned to the application without interpretation.
    void Configure(const St2110::RxConfigRaw&, BlobMetadata Md={});

    // Configures for SMPTE 2110-10 video stream reception.
    void Configure(const St2110::RxConfigVideo&, BlobMetadata Md={});

    // -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

    // Detaches the RxFifo from the hardware.
    void Detach();

    // Gets the number of frames currently stored in the RxFifo.
    int GetFifoLoad() const;
    
    // Retrieves the maximum number of frames that the RxFifo can store.
    int GetMaxSize() const;

    // Gets the current size of the shared buffer (in bytes) used to pass data from the
    // NIC to the RxFifo.
    int GetSharedBufferSize() const;

    // Gets receive statistics. Statistics will be cleared when Start() is called.
    RxStatistics GetStatistics() const;

    // Gets the status of the FIFO's IP connection.
    FifoStatus GetStatus() const;

    // Reads one frame from the RxFifo. 
    // The RxFifo must not be empty, or an exception will be thrown.
    Frame* Read();                  

    // Returns a frame to the memory pool.
    // After processing a frame read from the RxFifo, the frame must be returned to the
    // memory pool.
    void ReturnToMemPool(Frame*);

    // Configures the RxFifo's networking parameters, including the IP address, 
    // port number, and other related IP settings.
    void SetIpPars(const IpPars&);

    // Sets the maximum number of frames that the RxFifo can store (default: 4).
    void SetMaxSize(int Size);

    // Sets the size of the shared buffer (in bytes) used to pass data from the NIC to
    // the RxFifo.
    // The size of the shared buffer can only be changed if the RxFifo is not started.
    void SetSharedBufferSize(int Size);

    // Starts receiving frames in the RxFifo.
    // Returns true if starting is successful, false otherwise.
    // If false, use RxFifo::GetStatus() to get the reason. 
    bool Start();

    // Stops receiving frames in the RxFifo.
    void Stop();

    // Indicates whether a hardware pipe (true) or software pipe (false) is used.
    bool UsesHwPipe() const;

    // Constructor/destructor.
    RxFifo();
    ~RxFifo();

private:
    // Use the PIMPL pattern to move implementation details to RxFifo::Impl.
    class Impl;
    unique_ptr<Impl> Pimpl;
};

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+ SMPTE 2022 / SMPTE 2110 Transmission +=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ class AvFifo::TxFifo +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Represents a FIFO buffer for Frame objects, for transmission of a SMPTE 2022 or 
// SMPTE 2110 stream.
//
class TxFifo
{
public:
    // Attaches a TxFifo object to a specified DekTec device and port.
    void Attach(const DtDevice&, int Port, HwOrSwPipe Pref=HwOrSwPipe::Auto);

    // Empties the TxFifo and the frame memory pool.
    void Clear();

    // -.-.-.-.-.-.-.-.-.-.-.-.-.- Configuration of the TxFifo -.-.-.-.-.-.-.-.-.-.-.-.-.-

    // Overload set of Configure functions. 
    // The function invoked determines the standard of the stream to be transmitted based 
    // on the input argument's type. Each overload configures the TxFifo for stream 
    // transmission for a different SMPTE standard. 
    // An optional 'Md' parameter, common to all overloads, allows customization of the
    // alignment and oversizing of frame BLOBs, using defaults if not specified.

    // Configures for SMPTE 2022-5/6 stream transmission.
    void Configure(const St2022::TxConfig&, BlobMetadata Md={});

    // Configures for SMPTE 2110-20 audio stream transmission.
    // An optional 'Md' parameter allows customization of the alignment and oversizing
    // of received frame BLOBs, using defaults if not specified.    
    void Configure(const St2110::TxConfigAudio&, BlobMetadata Md={});

    // Configures for receiving streams of any SMPTE 2110 substandard.
    // The raw payload is directly returned to the application without interpretation.
    void Configure(const St2110::TxConfigRaw&, BlobMetadata Md={});

    // Configures for 'raw' SMPTE 2110-10 video stream transmission.
    // This overload allows for specifying all video parameters at a low level.
    void Configure(const St2110::TxConfigRawVideo&, BlobMetadata Md={});

    // Configures for SMPTE 2110-10 video stream transmission.
    void Configure(const St2110::TxConfigVideo&, BlobMetadata Md={});

    // -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

    // Detaches the TxFifo from the hardware.
    void Detach();

    // Gets the number of frames currently stored in the TxFifo.
    int GetFifoLoad() const;

    // Gets a Frame object from the memory pool with an embedded buffer of the given size.
    // If there are no available frames in the memory pool, a new Frame object will be
    // allocated, using the alignment and oversizing specified in the BlobMetadata
    // argument in the Configure routine.
    // The application writes frames to the TxFifo, DTAPI will return the frames to the 
    // memory pool for recycling when it has been transmitted.
    // NOTE: The application should not attempt to free Frame objects itself.
    Frame* GetFrameFromMemPool(size_t Size);

    // Retrieves the maximum number of frames that the TxFifo can store.
    int GetMaxSize() const;

    // Gets the current size of the shared buffer (in bytes) used to pass data from the
    // TxFifo to the NIC.
    int GetSharedBufferSize() const;

    // Gets transmit statistics. Statistics will be cleared when Start() is called.
    TxStatistics GetStatistics() const;

    // Gets the status of the FIFO's IP connection.
    FifoStatus GetStatus() const;

    // Indicates whether the TxFifo has been started.
    bool IsStarted() const;

    // Configures the TxFifo's networking parameters, including the IP address, 
    // port number, and other related IP settings.
    void SetIpPars(const IpPars&);

    // Sets the maximum number of frames that the TxFifo can store (default: 4).
    void SetMaxSize(int Size);

    // Sets the size of the shared buffer (in bytes) used to pass data from the TxFifo to
    // the NIC.
    // The size of the shared buffer can only be changed if the TxFifo is not started.
    void SetSharedBufferSize(int Size);

    // Starts transmitting frames from the TxFifo.
    // Returns true if starting is successful, false otherwise.
    // If false, use TxFifo::GetStatus() to get the reason. 
    bool Start();

    // Stops transmitting frames.
    void Stop();

    // Indicates whether a hardware pipe (true) or software pipe (false) is used.
    bool UsesHwPipe() const;

    // Writes a frame to the TxFifo.
    void Write(Frame*);

    // Constructor/destructor.
    TxFifo();
    ~TxFifo();

private:
    // Use the PIMPL pattern to move implementation details to TxFifo::Impl.
    class Impl;
    unique_ptr<Impl> Pimpl;
};

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Helper functions - Timing +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Three categories of timing helper functions are defined:
//  1. Alignment of time-of-day to the media grid.
//  2. (namespace St2110) Conversion from RTP timestamp to time-of-day.
//  3. (namespace St2110) Conversion from time-of-day to RTP timestamp.
// 
// For details on how to use these functions, refer to the AvFifo Manual.

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- 1. Time-of-day grid alignment -.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Aligns a given time-of-day to the AUDIO media grid, given the audio sample rate.
DtTimeOfDay Tod2Grid_Audio(DtTimeOfDay, int SampleRate=48000);

// Aligns a given time-of-day to the VIDEO media grid, given the video frame/field rate.
DtTimeOfDay Tod2Grid_Video(DtTimeOfDay, FrameRate);

namespace St2110
{
// -.-.-.-.-.-.-.-.-.-.-.-.-.- 2. RTP timestamp => Time-of-day -.-.-.-.-.-.-.-.-.-.-.-.-.-

// The time-of-day computed by Rtp2Tod_Audio and Rtp2Tod_Video will be aligned to the
// media grid, provided that the given RTP timestamp is also grid-aligned.
// To assess if the RTP timestamp is on grid, an approximate time-of-day is required.
// This allows the functions to determine the number of RTP clock counter wraparounds
// that have occurred since the epoch began.

// Converts an RTP timestamp to time-of-day for AUDIO, given the audio sample rate and 
// the approximate time-of-day.
DtTimeOfDay Rtp2Tod_Audio(uint32_t RtpTime, DtTimeOfDay ApproxToD, int SampleRate=48000);

// Converts an RTP timestamp to time-of-day for VIDEO, given the frame or field rate
// and the approximate time-of-day.
DtTimeOfDay Rtp2Tod_Video(uint32_t RtpTime, DtTimeOfDay ApproxToD);

// -.-.-.-.-.-.-.-.-.-.-.-.-.- 3. Time-of-day => RTP timestamp -.-.-.-.-.-.-.-.-.-.-.-.-.-

// The RTP timestamp calculated by Tod2Rtp_Audio and Tod2Rtp_Video is guaranteed to be
// aligned to the media grid if the input time-of-day is grid-aligned.
// If the input time-of-day is not grid-aligned, the functions will make a reasonable
// rounding attempt to determine the closest RTP timestamp, taking into account
// complexities arising from fractional video frame rates. For more details on the 
// rounding process and its implications, please refer to the AvFifo manual.

// Converts time-of-day to an RTP timestamp for AUDIO, given the audio sample rate in Hz.
uint32_t Tod2Rtp_Audio(DtTimeOfDay, int SampleRate=48000);

// Converts time-of-day to an RTP timestamp for VIDEO.
uint32_t Tod2Rtp_Video(DtTimeOfDay);

} // namespace St2110

namespace St2022
{

// Converts time-of-day to an RTP timestamp for an SMPTE 2022-5/6 stream.
uint32_t Tod2Rtp(DtTimeOfDay);

} // namespace St2022

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+ Helper functions - Miscellaneous +=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Converts a FifoStatus enumeration value to a message.
string FifoStatusToMessage(FifoStatus);

} // namespace AvFifo
} // namespace DTAPI
