//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtSdiFileFmt.h *#*#*#*#*#*#*#* (C) 2007-2012 DekTec
//
// Defines the file format of DekTec SDI files
//
// The suggested file-extension for files using this format is .dtsdi
//
// File format:
//      Header              See struct DtSdiFileHdr
//      SDI frames          See DekTec DTAPI documentation
//

#ifndef __DTSDIFILEFMT_H
#define __DTSDIFILEFMT_H


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Typedefs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// The file header is defined by a C/C++ struct, using the following types.
// These integer types will be stored in the .dtsdi file with Least-Significant-Byte
// first. This corresponds to LITTLE-ENDIAN storage in memory.
typedef unsigned int   uint_litend_32;
typedef unsigned short uint_litend_16;
typedef unsigned char  uint_litend_8;


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtSdiFileHdr +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Identifies the file as a ".dtsdi file" and defines the format of the SDI frames.
//
struct DtSdiFileHdr
{
    uint_litend_32 m_MagicCode1;    // Magic code 1, identifies file type
    uint_litend_32 m_MagicCode2;    // Magic code 2,     "       "    "
    uint_litend_32 m_MagicCode3;    // Magic code 3,     "       "    "
    uint_litend_8  m_FmtVersion;    // Version number of .dtsdi file format
    uint_litend_8  m_DataType;      // Type of data
    uint_litend_16 m_Flags;         // Additional data-format flags
};

// V2 file header
struct DtSdiFileHdrV2
{
    DtSdiFileHdr  m_BaseHdr;         // Base file heder
    // Extra parameters
    uint_litend_32 m_FrameSize;      // Size of frames (incl. padding)
    uint_litend_32 m_NumFrames;      // Number of frames stored in file
};


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Magic codes - Identify a file as an ".dtsdi file"
#define DTSDI_MAGIC_CODE1 0x546B6544  // Magic code part 1
#define DTSDI_MAGIC_CODE2 0x642E6365  // Magic code part 2
#define DTSDI_MAGIC_CODE3 0x69647374  // Magic code part 3

// (Current) format version
#define DTSDI_FMT_VERSION  0
#define DTSDI_FMT_VERSION2 1

// Data types
#define DTSDI_TYPE_SDI_625          0x01    // 625 lines per SDI frame
#define DTSDI_TYPE_SDI_525          0x02    // 525 lines per SDI frame
#define DTSDI_TYPE_SDI_720_P23_98   0x03
#define DTSDI_TYPE_SDI_720_P24      0x04
#define DTSDI_TYPE_SDI_720_P25      0x05
#define DTSDI_TYPE_SDI_720_P29_97   0x06
#define DTSDI_TYPE_SDI_720_P30      0x07
#define DTSDI_TYPE_SDI_720_P50      0x08
#define DTSDI_TYPE_SDI_720_P59_94   0x09
#define DTSDI_TYPE_SDI_720_P60      0x0A
#define DTSDI_TYPE_SDI_1080_P23_98  0x0B
#define DTSDI_TYPE_SDI_1080_P24     0x0C
#define DTSDI_TYPE_SDI_1080_P25     0x0D
#define DTSDI_TYPE_SDI_1080_P30     0x0E
#define DTSDI_TYPE_SDI_1080_P29_97  0x0F
#define DTSDI_TYPE_SDI_1080_I50     0x10
#define DTSDI_TYPE_SDI_1080_I59_94  0x11
#define DTSDI_TYPE_SDI_1080_I60     0x12
#define DTSDI_TYPE_SDI_1080_P50     0x13
#define DTSDI_TYPE_SDI_1080_P59_94  0x14
#define DTSDI_TYPE_SDI_1080_P60     0x15

// Data-format flags
#define DTSDI_SDI_FULL          0x0001      // Full SDI frame
#define DTSDI_SDI_ACTVID        0x0002      // Active video only
#define DTSDI_SDI_HANC          0x0004      // HANC section only
#define DTSDI_SDI_VANC          0x0008      // VANC section only
// The following flags can be OR-ed with: DTSDI_SDI_FULL or DTSDI_SDI_ACTVID
#define DTSDI_SDI_10B           0x0080      // Set if the file contains 10-bit samples,
                                            // otherwise file contains 8-bit samples
#define DTSDI_SDI_16B           0x0100      // Set if the file contains 16-bit samples,
                                            // mutual exclucive with DTSDI_SDI_10B
#define DTSDI_SDI_HUFFMAN       0x0200      // Set if the file contains compressed SDI
                                            // samples, otherwise file contains 
                                            // uncompressed samples

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Inline functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- VidStd2DtSdiDataType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static int VidStd2DtSdiDataType(int SdiSubValue)
{
    switch (SdiSubValue)
    {
    case DTAPI_VIDSTD_1080P50:          return DTSDI_TYPE_SDI_1080_P50; break;
    case DTAPI_VIDSTD_1080P59_94:       return DTSDI_TYPE_SDI_1080_P59_94; break;
    case DTAPI_VIDSTD_1080P60:          return DTSDI_TYPE_SDI_1080_P60; break;
    case DTAPI_VIDSTD_1080I50:          return DTSDI_TYPE_SDI_1080_I50; break;
    case DTAPI_VIDSTD_1080I59_94:       return DTSDI_TYPE_SDI_1080_I59_94; break;
    case DTAPI_VIDSTD_1080I60:          return DTSDI_TYPE_SDI_1080_I60; break;
    case DTAPI_VIDSTD_1080P23_98:       return DTSDI_TYPE_SDI_1080_P23_98; break;
    case DTAPI_VIDSTD_1080P24:          return DTSDI_TYPE_SDI_1080_P24; break;
    case DTAPI_VIDSTD_1080P25:          return DTSDI_TYPE_SDI_1080_P25; break;
    case DTAPI_VIDSTD_1080P29_97:       return DTSDI_TYPE_SDI_1080_P29_97; break;
    case DTAPI_VIDSTD_1080P30:          return DTSDI_TYPE_SDI_1080_P30; break;
    case DTAPI_VIDSTD_720P23_98:        return DTSDI_TYPE_SDI_720_P23_98; break;
    case DTAPI_VIDSTD_720P24:           return DTSDI_TYPE_SDI_720_P24; break;
    case DTAPI_VIDSTD_720P25:           return DTSDI_TYPE_SDI_720_P25; break;
    case DTAPI_VIDSTD_720P29_97:        return DTSDI_TYPE_SDI_720_P29_97; break;
    case DTAPI_VIDSTD_720P30:           return DTSDI_TYPE_SDI_720_P30; break;
    case DTAPI_VIDSTD_720P50:           return DTSDI_TYPE_SDI_720_P50; break;
    case DTAPI_VIDSTD_720P59_94:        return DTSDI_TYPE_SDI_720_P59_94; break;
    case DTAPI_VIDSTD_720P60:           return DTSDI_TYPE_SDI_720_P60; break;
    case DTAPI_VIDSTD_525I59_94:        return DTSDI_TYPE_SDI_525; break;
    case DTAPI_VIDSTD_625I50:           return DTSDI_TYPE_SDI_625; break;
    }
    return -1;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtSdiDataType2VidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static int DtSdiDataType2VidStd(int DataType)
{
    switch (DataType)
    {
    case DTSDI_TYPE_SDI_1080_P50:       return DTAPI_VIDSTD_1080P50; break;
    case DTSDI_TYPE_SDI_1080_P59_94:    return DTAPI_VIDSTD_1080P59_94; break;
    case DTSDI_TYPE_SDI_1080_P60:       return DTAPI_VIDSTD_1080P60; break;
    case DTSDI_TYPE_SDI_1080_I50:       return DTAPI_VIDSTD_1080I50; break;
    case DTSDI_TYPE_SDI_1080_I59_94:    return DTAPI_VIDSTD_1080I59_94; break;
    case DTSDI_TYPE_SDI_1080_I60:       return DTAPI_VIDSTD_1080I60; break;
    case DTSDI_TYPE_SDI_1080_P23_98:    return DTAPI_VIDSTD_1080P23_98; break;
    case DTSDI_TYPE_SDI_1080_P24:       return DTAPI_VIDSTD_1080P24; break;
    case DTSDI_TYPE_SDI_1080_P25:       return DTAPI_VIDSTD_1080P25; break;
    case DTSDI_TYPE_SDI_1080_P29_97:    return DTAPI_VIDSTD_1080P29_97; break;
    case DTSDI_TYPE_SDI_1080_P30:       return DTAPI_VIDSTD_1080P30; break;
    case DTSDI_TYPE_SDI_720_P23_98:     return DTAPI_VIDSTD_720P23_98; break;
    case DTSDI_TYPE_SDI_720_P24:        return DTAPI_VIDSTD_720P24; break;
    case DTSDI_TYPE_SDI_720_P25:        return DTAPI_VIDSTD_720P25; break;
    case DTSDI_TYPE_SDI_720_P29_97:     return DTAPI_VIDSTD_720P29_97; break;
    case DTSDI_TYPE_SDI_720_P30:        return DTAPI_VIDSTD_720P30; break;
    case DTSDI_TYPE_SDI_720_P50:        return DTAPI_VIDSTD_720P50; break;
    case DTSDI_TYPE_SDI_720_P59_94:     return DTAPI_VIDSTD_720P59_94; break;
    case DTSDI_TYPE_SDI_720_P60:        return DTAPI_VIDSTD_720P60; break;
    case DTSDI_TYPE_SDI_525:            return DTAPI_VIDSTD_525I59_94; break;
    case DTSDI_TYPE_SDI_625:            return DTAPI_VIDSTD_625I50; break;
    }
    return -1;
}

#endif // #ifndef __DTSDIFILEFMT_H
