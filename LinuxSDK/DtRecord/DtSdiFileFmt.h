//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtSdiFileFmt.h *#*#*#*#*#*#*#* (C) 2007-2008 DekTec
//
// Defines the file format of DekTec SDI files
//
// The suggested file-extension for files using this format is .dtsdi
//
// File format:
//		Header				See struct DtSdiFileHdr
//		SDI frames			See DekTec DTAPI documentation
//

#ifndef __DTSDIFILEFMT_H
#define __DTSDIFILEFMT_H


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Typedefs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// The file header is defined by a C/C++ struct, using the following types.
// These integer types will be stored in the .dtsdi file with Least-Significant-Byte
// first. This corresponds to LITTLE-ENDIAN storage in memory.
typedef unsigned long  uint_litend_32;
typedef unsigned short uint_litend_16;
typedef unsigned char  uint_litend_8;


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtSdiFileHdr +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Identifies the file as a ".dtsdi file" and defines the format of the SDI frames.
//
struct DtSdiFileHdr
{
	uint_litend_32 m_MagicCode1;			// Magic code 1, identifies file type
	uint_litend_32 m_MagicCode2;			// Magic code 2,     "       "    "
	uint_litend_32 m_MagicCode3;			// Magic code 3,     "       "    "
	uint_litend_8  m_FmtVersion;			// Version number of .dtsdi file format
	uint_litend_8  m_DataType;				// Type of data
	uint_litend_16 m_Flags;					// Additional data-format flags
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Magic Code -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Identifies file as ".dtsdi file"
//
#define DTSDI_MAGIC_CODE1		0x546B6544	// Magic code part 1
#define DTSDI_MAGIC_CODE2		0x642E6365	// Magic code part 2
#define DTSDI_MAGIC_CODE3		0x69647374	// Magic code part 3

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- (Current) Format Version -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTSDI_FMT_VERSION		0

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Data Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define DTSDI_TYPE_SDI_625		0x01		// 625 lines per SDI frame
#define DTSDI_TYPE_SDI_525		0x02		// 525 lines per SDI samples

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Data-Format Flags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTSDI_SDI_FULL			0x0001		// Full SDI frame
#define DTSDI_SDI_ACTVID		0x0002		// Active video only
// Following flags can be OR-ed with: DTSDI_SDI_FULL or DTSDI_SDI_ACTVID
#define DTSDI_SDI_10B			0x0080		// Set if the file contains 10-bit samples,
											// otherwise file contains 8-bit samples
#define DTSDI_SDI_HUFFMAN		0x0200		// Set if the file contains compressed SDI
											// samples, otherwise file contains 
											// uncompressed samples

#endif // #ifndef __DTSDIFILEFMT_H
