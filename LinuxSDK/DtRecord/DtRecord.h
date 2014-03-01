//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtRecord.h *#*#*#*#*#*#*#*#*#*#*#* (C) 2000-2007 DekTec
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//	2007.02.10	MG	Created	

#ifndef __DTRECORD_H
#define __DTRECORD_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <stdarg.h>

// Windows/Microsoft uses with _ => Linux uses without
#ifndef WIN32
#define _vsnprintf vsnprintf
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- class Exc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
class Exc
{
public:
	Exc( const char* pFormat, ...  )
	{
		va_list  ArgList;
		va_start(ArgList, pFormat);
		_vsnprintf(m_ErrorMsg, sizeof(m_ErrorMsg)-1, pFormat, ArgList);
		va_end(ArgList);
	};
	virtual ~Exc() {};

	operator const char* () const { return m_ErrorMsg; }

protected:

    // Error message
	char m_ErrorMsg[1024];
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
class CommandLineParams
{
public:
	CommandLineParams();
	virtual ~CommandLineParams();

	void  Init();
	void  ParseCommandLine(int argc, char* argv[]);
	const char* Bandwidth2Str() const;
	const char* RxMode2Str() const;
	const char* ModType2Str() const;
	const char* QamJ83Annex2Str() const;
	const char* IpProtocol2Str() const;

	//---- Application parameters ----

	char  m_FileName[256];		// Name of play file

	int  m_DvcType;				// -t: Type of device 
	int  m_DvcNum;				// -n: Device number
	int  m_Port;				// -i: Port index

	int  m_RxMode;				// -m: Receive mode
	bool  m_RxModeHuffman;		// -c: Enable Huffman compression
	__int64  m_MaxSize;			// -x: Transmission rate

	int  m_ModType;				// -mt: Master modulation type
	double  m_CarrierFreq;		// -mf: carrier frequency
	int  m_QamJ83Annex;			// -ma: J83 annex (QAM-A/B/C)
	int  m_Bandwidth;			// -mb: bandwidth

	DtTsIpPars  m_IpPars;		// IP parameters.
								// -ipa: IP address and IP port
								// -ipp: IP protocol

	int m_Polarity;				// -pc: Polarity Control

	bool  m_SilentMode;			// -s: Enable silent mode (i.e. no printf)
	bool  m_ShowHelp;			// -?: show help

	bool  m_RecDtSdiFile;		// Record to a DTSDI file 

protected:

	void  ParseParam(char* pParam, bool Flag, bool First=false, bool Last=false);
	void  ParseParamFlag(char* pParam, bool First=false, bool Last=false);
	void  ParseParamNotFlag(char* pParam, bool First=false, bool Last=false);

	bool  m_Error;				// Error detected while parsing command line?
	bool  m_LastFlagReqsArg;	// Last flag requires an argument
	char*  m_pLastFlag;			// Last flag
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Recorder -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
class Recorder
{
public:
	Recorder();
	virtual ~Recorder();

	//---- Public operations ----
	int Record(int argc, char* argv[]);

protected:

	//---- Internal operation ----
	void AttachToInput();
	void DisplayPlayInfo();
	void InitInput();
	void Log(const char* pMessage, bool IgnoreSilence=false);
	void LogF(const char* pMessage, ... );
    void RecordFile();
	void ShowHelp();
	

	CommandLineParams  m_CmdLineParams;

	DtDevice  m_DtDvc;				// Our device
	DtInpChannel  m_DtInp;			// Our input channel
	bool  m_Demod;					// Current input is a demodulator
	bool  m_Ip;						// Current input is a IP port
	FILE* m_pFile;					// Our record file

	char* m_pBuf;					// Our data buffer
	int  m_Allign;					// Alignment for record file (i.e. file should be
									// multiple of allignment)
};

#endif // #ifndef __DTRECORD_H
