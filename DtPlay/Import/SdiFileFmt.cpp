// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* SdiFileFmt.cpp *#*#*#*#*#*#*#* (C) 2024-2025 DekTec
//

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#include "SdiFileFmt.h"

#include "DTAPI.h"

#include <algorithm>
#include <cstring>
#include <fstream>

using std::min;

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

class BitReader;
class BitWriter;

static void ReadSdiFileHeader(BitReader& Reader, SdiFileHeader& Header);
static void ReadSdiFormat(BitReader& Reader, SdiFormat& Format);
static void ReadLogicalFrameProperties(BitReader& Reader, LogicalFrameProperties& Props);
static void ReadPhysicalFrameProperties(BitReader& Reader, PhysicalFrameProperties& Props);
static void ReadPhysicalFieldProperties(BitReader& Reader, PhysicalFieldProperties& Props);

static int WriteSdiFileHeader(BitWriter& Writer, const SdiFileHeader& Header);
static int WriteSdiFormat(BitWriter& Writer, const SdiFormat& Format);
static int WriteLogicalFrameProperties(BitWriter& Writer, const LogicalFrameProperties& Props);
static int WritePhysicalFrameProperties(BitWriter& Writer, const PhysicalFrameProperties& Props);
static int WritePhysicalFieldProperties(BitWriter& Writer, const PhysicalFieldProperties& Props);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+ BitReader / BitWriter Implementation +=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Read64 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static inline uint64_t Read64(uint8_t* Ptr)
{
#ifdef _WIN32
    return _byteswap_uint64(*((uint64_t*)Ptr));
#else
    return (uint64_t)Ptr[0] << 56 |
           (uint64_t)Ptr[1] << 48 |
           (uint64_t)Ptr[2] << 40 |
           (uint64_t)Ptr[3] << 32 |
           (uint64_t)Ptr[4] << 24 |
           (uint64_t)Ptr[5] << 16 |
           (uint64_t)Ptr[6] <<  8 |
           (uint64_t)Ptr[7];
#endif
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Write64 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static inline void Write64(uint8_t* Ptr, uint64_t Value)
{
#ifdef _WIN32
    *((uint64_t*)Ptr) = _byteswap_uint64(Value);
#else
    Ptr[0] = Value >> 56;
    Ptr[1] = Value >> 48;
    Ptr[2] = Value >> 40;
    Ptr[3] = Value >> 32;
    Ptr[4] = Value >> 24;
    Ptr[5] = Value >> 16;
    Ptr[6] = Value >>  8;
    Ptr[7] = Value;
#endif
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- class BitReader -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
class BitReader
{
public:
    void Init(uint8_t* Data, int Count)
    {
        Bytes = Data;
        NumBytes = Count;
        BytePos = 0;
        BitBuffer = 0;
        NumBitsRemaining = 0;

        Fill();
    }

    uint64_t ReadBits(int NumBits)
    {
        uint64_t Result = 0;

        if (NumBits < NumBitsRemaining) {
            Result = (BitBuffer >> (NumBitsRemaining - NumBits));
            NumBitsRemaining -= NumBits;
        } else {
            int RemainingBits = NumBits - NumBitsRemaining;
            Result = BitBuffer << RemainingBits;
            NumBitsRemaining = 0;
            Fill();
            if (RemainingBits > 0 && RemainingBits <= NumBitsRemaining)
            {
                Result |= (BitBuffer >> (NumBitsRemaining - RemainingBits));
                NumBitsRemaining -= RemainingBits;
            }
            else if (RemainingBits > 0 && NumBitsRemaining == 0)
                throw std::runtime_error{"BitReader out of bounds"};
        }
        Result &= ((1ull << NumBits) - 1);

        return Result;
    }

    void Fill()
    {
        int RemainingBytes = NumBytes - BytePos;
        if (RemainingBytes >= 8)
        {
            BitBuffer = Read64(&Bytes[BytePos]);
            BytePos += 8;
            NumBitsRemaining = 64;
        }
        else if (RemainingBytes > 0) {
            for (int i = 0; i < RemainingBytes; i++)
            {
                BitBuffer <<= 8;
                BitBuffer |= Bytes[BytePos + i];
            }
            BytePos += RemainingBytes;
            NumBitsRemaining = RemainingBytes * 8;
        }
    }

private:
    uint8_t* Bytes = nullptr;
    int NumBytes = 0;
    int BytePos = 0;

    uint64_t BitBuffer = 0;
    int NumBitsRemaining = 0;
};

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- class BitWriter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
class BitWriter
{
public:
    void Init(uint8_t* Data, int Count)
    {
        Bytes = Data;
        NumBytes = Count;
        BytePos = 0;
        BitBuffer = 0;
        NumBitsFree = min(64, Count*8);
    }

    void WriteBits(uint64_t Value, int NumBits)
    {
        int Mask = ((1ull << NumBits) - 1);
        Value &= Mask;

        if (NumBits < NumBitsFree)
        {
            BitBuffer = (BitBuffer << NumBits) | Value;
            NumBitsFree -= NumBits;
        } else {
            int RemainingBits = NumBits - NumBitsFree;
            BitBuffer <<= NumBitsFree;
            BitBuffer |= Value >> RemainingBits;
            NumBitsFree = 0;
            Flush();
            if (RemainingBits > NumBitsFree)
                throw std::runtime_error{"BitWriter out of bounds"};
            BitBuffer = Value;
            NumBitsFree -= RemainingBits;
        }
    }

    void Flush()
    {
        int BytesFree = NumBytes - BytePos;
        int NumBytesToFlush = std::min((64 - NumBitsFree) / 8, BytesFree);
        if (NumBytesToFlush == 8)
        {
            Write64(&Bytes[BytePos], BitBuffer);
            BytePos += 8;
            BitBuffer = 0;
            NumBitsFree = 64;
        }
        else if (NumBytesToFlush > 0)
        {
            int NumBitsToFlush = NumBytesToFlush * 8;
            BitBuffer <<= (8-NumBytesToFlush)*8;
            for (int i = 0; i < NumBytesToFlush; i++)
            {
                int Shift = 56 - (i * 8);
                Bytes[BytePos + i] = (uint8_t)(BitBuffer >> Shift);
            }
            BytePos += NumBytesToFlush;
            BitBuffer >>= NumBitsToFlush;
            NumBitsFree += NumBitsToFlush;
        }
        BytesFree = NumBytes - BytePos;
        NumBitsFree = min(64, BytesFree*8);
    }

private:
    uint8_t* Bytes = nullptr;
    int NumBytes = 0;
    int BytePos = 0;

    uint64_t BitBuffer = 0;
    int NumBitsFree = 0;
};

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ SDI File Reader Implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ReadSdiFileHeader -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void ReadSdiFileHeader(uint8_t* Buf, int BufSize, SdiFileHeader& Header)
{
    BitReader Reader;
    Reader.Init(Buf, BufSize);
    ReadSdiFileHeader(Reader, Header);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ReadSdiFileHeader -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void ReadSdiFileHeader(BitReader& Reader, SdiFileHeader& Header)
{
    Header.MagicCode = (int)Reader.ReadBits(32);
    Header.Version = (int)Reader.ReadBits(8);
    Header.HeaderSize = (int)Reader.ReadBits(16);
    Header.NumPhysicalLinks = (int)Reader.ReadBits(8);
    Header.FrameSize = (int)Reader.ReadBits(32);
    Header.NumFrames = (int)Reader.ReadBits(32);
    Header.CompressionMode = (SdiCompressionMode)Reader.ReadBits(8);
    ReadSdiFormat(Reader, Header.Format);
    ReadLogicalFrameProperties(Reader, Header.LogicalFrameProperties);
    ReadPhysicalFrameProperties(Reader, Header.PhysicalFrameProperties);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ReadSdiFormat -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void ReadSdiFormat(BitReader& Reader, SdiFormat& Format)
{
    Format.LineRate = (SdiLineRate)(Reader.ReadBits(5));
    Format.InterleavingType = (SdiInterleavingType)(Reader.ReadBits(2));
    int SdiLevel = (int)Reader.ReadBits(2);
    int NumStreams = (int)Reader.ReadBits(2);
    if (SdiLevel == 0)
        Format.SdiLevel = SdiLevel::NOT_APPLICABLE;
    else if (SdiLevel == 1)
        Format.SdiLevel = SdiLevel::A;
    else if (SdiLevel == 2) {
        if (NumStreams == 1)
            Format.SdiLevel = SdiLevel::B_DL;
        else if (NumStreams == 2)
            Format.SdiLevel = SdiLevel::B_DS;
        else // Assume B-DL
            Format.SdiLevel = SdiLevel::B_DL;
    } else // Assume Level A
        Format.SdiLevel = SdiLevel::A;
    Reader.ReadBits(5); // Reserved
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- ReadLogicalFrameProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void ReadLogicalFrameProperties(BitReader& Reader, LogicalFrameProperties& Props)
{
    Props.PictureRate.Num = (int)Reader.ReadBits(32);
    Props.PictureRate.Den = (int)Reader.ReadBits(32);
    Props.AspectRatio.Num = (int)Reader.ReadBits(8);
    Props.AspectRatio.Den = (int)Reader.ReadBits(8);
    Props.IsInterlaced = (int)Reader.ReadBits(1);
    Props.SamplingStructure = (SdiSamplingStructure)Reader.ReadBits(4);
    Props.IsStereoscopic = (int)Reader.ReadBits(1);
    Reader.ReadBits(2); // Reserved
    Props.BitDepth = (int)Reader.ReadBits(8);
    Props.PictureWidth = (int)Reader.ReadBits(16);
    Props.PictureHeight = (int)Reader.ReadBits(16);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- ReadPhysicalFrameProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void ReadPhysicalFrameProperties(BitReader& Reader, PhysicalFrameProperties& Props)
{
    Props.NumFields = (int)Reader.ReadBits(2);
    Props.CrcOmitted = (int)Reader.ReadBits(1);
    Reader.ReadBits(5); // Reserved
    Props.NumLinesFrame = (int)Reader.ReadBits(16);
    Props.NumSymsHanc = (int)Reader.ReadBits(16);
    Props.NumSymsVancVideo = (int)Reader.ReadBits(16);
    memset(Props.FieldProperties, 0, sizeof(PhysicalFieldProperties) * 2);
    for (int i = 0; i < std::min(Props.NumFields, 2); i++)
        ReadPhysicalFieldProperties(Reader, Props.FieldProperties[i]);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- ReadPhysicalFieldProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void ReadPhysicalFieldProperties(BitReader& Reader, PhysicalFieldProperties& Props)
{
    Props.NumLinesField = (int)Reader.ReadBits(16);
    Props.FirstVideoLine = (int)Reader.ReadBits(16);
    Props.NumLinesVideo = (int)Reader.ReadBits(16);
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ SDI File Writer Implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- WriteSdiFileHeader -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
int WriteSdiFileHeader(uint8_t* Buf, int BufSize, const SdiFileHeader& Header)
{
    int NumBits = 0;

    BitWriter Writer;
    Writer.Init(Buf, BufSize);
    NumBits = WriteSdiFileHeader(Writer, Header);
    Writer.Flush();

    return NumBits;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- WriteSdiFileHeader -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
int WriteSdiFileHeader(BitWriter& Writer, const SdiFileHeader& Header)
{
    int NumBits = 136;

    Writer.WriteBits(Header.MagicCode, 32);
    Writer.WriteBits(Header.Version, 8);
    Writer.WriteBits(Header.HeaderSize, 16);
    Writer.WriteBits(Header.NumPhysicalLinks, 8);
    Writer.WriteBits(Header.FrameSize, 32);
    Writer.WriteBits(Header.NumFrames, 32);
    Writer.WriteBits((uint64_t)Header.CompressionMode, 8);
    NumBits += WriteSdiFormat(Writer, Header.Format);
    NumBits += WriteLogicalFrameProperties(Writer, Header.LogicalFrameProperties);
    NumBits += WritePhysicalFrameProperties(Writer, Header.PhysicalFrameProperties);

    return NumBits;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- WriteSdiFormat -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
int WriteSdiFormat(BitWriter& Writer, const SdiFormat& Format)
{
    int NumBits = 16;

    Writer.WriteBits((uint64_t)Format.LineRate, 5);
    Writer.WriteBits((uint64_t)Format.InterleavingType, 2);
    switch(Format.SdiLevel)
    {
    case SdiLevel::A:
        Writer.WriteBits(1, 2);
        Writer.WriteBits(0, 2);
        break;
    case SdiLevel::B_DL:
        Writer.WriteBits(2, 2);
        Writer.WriteBits(1, 2);
        break;
    case SdiLevel::B_DS:
        Writer.WriteBits(2, 2);
        Writer.WriteBits(2, 2);
        break;
    default:
        Writer.WriteBits(0, 2);
        Writer.WriteBits(0, 2);
    };
    Writer.WriteBits(0, 5); // Reserved

    return NumBits;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- WriteLogicalFrameProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
int WriteLogicalFrameProperties(BitWriter& Writer, const LogicalFrameProperties& Props)
{
    int NumBits = 128;

    Writer.WriteBits(Props.PictureRate.Num, 32);
    Writer.WriteBits(Props.PictureRate.Den, 32);
    Writer.WriteBits(Props.AspectRatio.Num, 8);
    Writer.WriteBits(Props.AspectRatio.Den, 8);
    Writer.WriteBits(Props.IsInterlaced, 1);
    Writer.WriteBits((uint64_t)Props.SamplingStructure, 4);
    Writer.WriteBits(Props.IsStereoscopic, 1);
    Writer.WriteBits(0, 2); // Reserved
    Writer.WriteBits(Props.BitDepth, 8);
    Writer.WriteBits(Props.PictureWidth, 16);
    Writer.WriteBits(Props.PictureHeight, 16);

    return NumBits;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- WritePhysicalFrameProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
int WritePhysicalFrameProperties(BitWriter& Writer, const PhysicalFrameProperties& Props)
{
    int NumBits = 56;

    Writer.WriteBits(Props.NumFields, 2);
    Writer.WriteBits(Props.CrcOmitted, 1);
    Writer.WriteBits(0, 5);
    Writer.WriteBits(Props.NumLinesFrame, 16);
    Writer.WriteBits(Props.NumSymsHanc, 16);
    Writer.WriteBits(Props.NumSymsVancVideo, 16);
    for (int i = 0; i < std::min(Props.NumFields, 2); i++)
        NumBits += WritePhysicalFieldProperties(Writer, Props.FieldProperties[i]);

    return NumBits;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- WritePhysicalFieldProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
int WritePhysicalFieldProperties(BitWriter& Writer, const PhysicalFieldProperties& Props)
{
    int NumBits = 48;

    Writer.WriteBits(Props.NumLinesField, 16);
    Writer.WriteBits(Props.FirstVideoLine, 16);
    Writer.WriteBits(Props.NumLinesVideo, 16);

    return NumBits;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Utility Functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

struct SdiStandard
{
    int VidStd;
    Fraction AspectRatio;
    Fraction PictureRate;
    bool IsInterlaced;
    int NumFields;
    int PictureWidth;
    int PictureHeight;
};

static const SdiStandard Standards[] = {
    { DTAPI_VIDSTD_525I59_94,    Fraction{4,3},  Fraction{30000,1001},  true,  2, 720,  480 },
    { DTAPI_VIDSTD_525I59_94,    Fraction{4,3},  Fraction{30000,1001},  true,  2, 720,  487 },
    { DTAPI_VIDSTD_625I50,       Fraction{4,3},  Fraction{25,1},        true,  2, 720,  576 },
    
    { DTAPI_VIDSTD_720P23_98,    Fraction{16,9}, Fraction{24000, 1001}, false, 1, 1280, 720 },
    { DTAPI_VIDSTD_720P24,       Fraction{16,9}, Fraction{24,1},        false, 1, 1280, 720 },
    { DTAPI_VIDSTD_720P25,       Fraction{16,9}, Fraction{25,1},        false, 1, 1280, 720 },
    { DTAPI_VIDSTD_720P29_97,    Fraction{16,9}, Fraction{30000,1001},  false, 1, 1280, 720 },
    { DTAPI_VIDSTD_720P30,       Fraction{16,9}, Fraction{30,1},        false, 1, 1280, 720 },
    { DTAPI_VIDSTD_720P50,       Fraction{16,9}, Fraction{50,1},        false, 1, 1280, 720 },
    { DTAPI_VIDSTD_720P59_94,    Fraction{16,9}, Fraction{60000,1001},  false, 1, 1280, 720 },
    { DTAPI_VIDSTD_720P60,       Fraction{16,9}, Fraction{60,1},        false, 1, 1280, 720 },
    
    { DTAPI_VIDSTD_1080P23_98,   Fraction{16,9}, Fraction{24000, 1001}, false, 1, 1920, 1080 },
    { DTAPI_VIDSTD_1080P24,      Fraction{16,9}, Fraction{24,1},        false, 1, 1920, 1080 },
    { DTAPI_VIDSTD_1080P25,      Fraction{16,9}, Fraction{25,1},        false, 1, 1920, 1080 },
    { DTAPI_VIDSTD_1080P29_97,   Fraction{16,9}, Fraction{30000,1001},  false, 1, 1920, 1080 },
    { DTAPI_VIDSTD_1080P30,      Fraction{16,9}, Fraction{30,1},        false, 1, 1920, 1080 },
    { DTAPI_VIDSTD_1080PSF23_98, Fraction{16,9}, Fraction{24000, 1001}, false, 2, 1920, 1080 },
    { DTAPI_VIDSTD_1080PSF24,    Fraction{16,9}, Fraction{24,1},        false, 2, 1920, 1080 },
    { DTAPI_VIDSTD_1080PSF25,    Fraction{16,9}, Fraction{25,1},        false, 2, 1920, 1080 },
    { DTAPI_VIDSTD_1080PSF29_97, Fraction{16,9}, Fraction{30000,1001},  false, 2, 1920, 1080 },
    { DTAPI_VIDSTD_1080PSF30,    Fraction{16,9}, Fraction{30,1},        false, 2, 1920, 1080 },
    { DTAPI_VIDSTD_1080I50,      Fraction{16,9}, Fraction{25,1},        true,  2, 1920, 1080 },
    { DTAPI_VIDSTD_1080I59_94,   Fraction{16,9}, Fraction{30000,1001},  true,  2, 1920, 1080 },
    { DTAPI_VIDSTD_1080I60,      Fraction{16,9}, Fraction{30,1},        true,  2, 1920, 1080 },
    
    { DTAPI_VIDSTD_1080P50,      Fraction{16,9}, Fraction{50,1},        false, 1, 1920, 1080 },
    { DTAPI_VIDSTD_1080P50B,     Fraction{16,9}, Fraction{50,1},        false, 1, 1920, 1080 },
    { DTAPI_VIDSTD_1080P59_94,   Fraction{16,9}, Fraction{60000,1001},  false, 1, 1920, 1080 },
    { DTAPI_VIDSTD_1080P59_94B,  Fraction{16,9}, Fraction{60000,1001},  false, 1, 1920, 1080 },
    { DTAPI_VIDSTD_1080P60,      Fraction{16,9}, Fraction{60,1},        false, 1, 1920, 1080 },
    { DTAPI_VIDSTD_1080P60B,     Fraction{16,9}, Fraction{60,1},        false, 1, 1920, 1080 },
    
    { DTAPI_VIDSTD_2160P50,      Fraction{16,9}, Fraction{50,1},        false, 1, 3840, 2160 },
    { DTAPI_VIDSTD_2160P50B,     Fraction{16,9}, Fraction{50,1},        false, 1, 3840, 2160 },
    { DTAPI_VIDSTD_2160P59_94,   Fraction{16,9}, Fraction{60000,1001},  false, 1, 3840, 2160 },
    { DTAPI_VIDSTD_2160P59_94B,  Fraction{16,9}, Fraction{60000,1001},  false, 1, 3840, 2160 },
    { DTAPI_VIDSTD_2160P60,      Fraction{16,9}, Fraction{60,1},        false, 1, 3840, 2160 },
    { DTAPI_VIDSTD_2160P60B,     Fraction{16,9}, Fraction{60,1},        false, 1, 3840, 2160 },
    { DTAPI_VIDSTD_2160P23_98,   Fraction{16,9}, Fraction{24000, 1001}, false, 1, 3840, 2160 },
    { DTAPI_VIDSTD_2160P24,      Fraction{16,9}, Fraction{24,1},        false, 1, 3840, 2160 },
    { DTAPI_VIDSTD_2160P25,      Fraction{16,9}, Fraction{25,1},        false, 1, 3840, 2160 },
    { DTAPI_VIDSTD_2160P29_97,   Fraction{16,9}, Fraction{30000,1001},  false, 1, 3840, 2160 },
    { DTAPI_VIDSTD_2160P30,      Fraction{16,9}, Fraction{30,1},        false, 1, 3840, 2160 },
};

int GetVidStd(const SdiFileHeader& Header)
{
    Fraction PictureRate = Header.LogicalFrameProperties.PictureRate;
    Fraction AspectRatio = Header.LogicalFrameProperties.AspectRatio;
    bool IsInterlaced = Header.LogicalFrameProperties.IsInterlaced;
    int NumFields = Header.PhysicalFrameProperties.NumFields;
    int PictureWidth = Header.LogicalFrameProperties.PictureWidth;
    int PictureHeight = Header.LogicalFrameProperties.PictureHeight;

    for (const SdiStandard& Standard : Standards)
    {
        if (Standard.AspectRatio.Num != AspectRatio.Num ||
                                                Standard.AspectRatio.Den != AspectRatio.Den)
            continue;
        if (Standard.PictureRate.Num != PictureRate.Num ||
                                                Standard.PictureRate.Den != PictureRate.Den)
            continue;
        if (Standard.IsInterlaced != IsInterlaced)
            continue;
        if (Standard.NumFields != NumFields)
            continue;
        if (Standard.PictureWidth != PictureWidth)
            continue;
        if (Standard.PictureHeight != PictureHeight)
            continue;

        return Standard.VidStd;
    }

    return DTAPI_VIDSTD_UNKNOWN;
}
