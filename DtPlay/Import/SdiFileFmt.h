// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* SdiFileFmt.h *#*#*#*#*#*#*#*# (C) 2024-2025 DekTec
//
#pragma once

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#include <cstdint>

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

static constexpr int SDI_MAGIC = 0x2e736469;

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct Fraction -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
struct Fraction
{
    int Num;
    int Den;
};

// -.-.-.-.-.-.-.-.-.-.-.-.-.- enum class SdiSamplingStructure -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
enum class SdiSamplingStructure : uint8_t
{
    YCbCr422 = 0
};

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- enum class SdiLineRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
enum class SdiLineRate : uint8_t
{
    SDI_SD = 0,
    SDI_ED = 1,
    SDI_HD = 2,
    SDI_3G = 3,
    SDI_6G = 4,
    SDI_12G = 5,
    SDI_24G = 6,

    COUNT
};

// -.-.-.-.-.-.-.-.-.-.-.-.-.- enum class SdiInterleavingType -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
enum class SdiInterleavingType : uint8_t
{
    NONE = 0,
    SDI_2SI = 1,
    QUADRANT = 2
};

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- enum class SdiLevel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
enum class SdiLevel : uint8_t
{
    NOT_APPLICABLE = 0,
    A = 1,
    B_DL = 2,
    B_DS = 3
};

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- enum class SdiCompressionMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
enum class SdiCompressionMode : uint8_t
{
    NONE = 0
};

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct SdiFormat -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
struct SdiFormat
{
    SdiLineRate LineRate;
    SdiInterleavingType InterleavingType;
    SdiLevel SdiLevel;
};

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- struct LogicalFrameProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
struct LogicalFrameProperties
{
    Fraction PictureRate;
    Fraction AspectRatio;
    int IsInterlaced;
    SdiSamplingStructure SamplingStructure;
    int IsStereoscopic;
    int BitDepth;
    int PictureWidth;
    int PictureHeight;
};

// -.-.-.-.-.-.-.-.-.-.-.-.-.- struct PhysicalFieldProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
struct PhysicalFieldProperties
{
    int NumLinesField;
    int FirstVideoLine;
    int NumLinesVideo;
};

// -.-.-.-.-.-.-.-.-.-.-.-.-.- struct PhysicalFrameProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
struct PhysicalFrameProperties
{
    int NumFields;
    int CrcOmitted;
    int NumLinesFrame;
    int NumSymsHanc;
    int NumSymsVancVideo;
    PhysicalFieldProperties FieldProperties[2];
};

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct SdiFileHeader -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
struct SdiFileHeader
{
    int MagicCode;                     // shall have the value 0x2e736469 (=".sdi")
    int Version;                       // shall have the value 0
    int HeaderSize;                    // total size of SdiFileHeader
    int NumPhysicalLinks;
    int FrameSize;
    int NumFrames;
    SdiCompressionMode CompressionMode;

    SdiFormat Format;
    LogicalFrameProperties LogicalFrameProperties;
    PhysicalFrameProperties PhysicalFrameProperties;
};

void ReadSdiFileHeader(uint8_t* Buf, int BufSize, SdiFileHeader& Header);
int WriteSdiFileHeader(uint8_t* Buf, int BufSize, const SdiFileHeader& Header);

int GetVidStd(const SdiFileHeader& Header);
