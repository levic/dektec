//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPlay.h *#*#*#*#*#*#*#*#*#*#*#* (C) 2006 DekTec
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//	2006.10.08	MG	Created	

#ifndef __DTPLAY_H
#define __DTPLAY_H

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
	const char* TxMode2Str() const;
	const char* ModType2Str() const;
	const char* CodeRate2Str() const;
	const char* QamJ83Annex2Str() const;
	const char* Constellation2Str() const;
	const char* OfdmBandwidth2Str() const;
	const char* OfdmGaurdItv2Str() const;
	const char* OfdmTxMode2Str() const;
	const char* DvbS2Pilots2Str() const;
	const char* DvbS2FecFrameLength2Str() const;
	const char* DtmbFrameHdrMode2Str() const;
	const char* IqInterpFilter2Str() const;
	const char* IpProtocol2Str() const;

	//---- Application parameters ----

	char  m_FileName[256];		// Name of play file
	int  m_LoopCnt;				// Number of times to loop file


	int  m_DvcType;				// -t: Type of device 
	int  m_DvcNum;				// -n: Device number
	int  m_Port;				// -i: Port number of channel to use
	bool m_DblBuff;				// -db: use doubly buffered output

	int  m_TxMode;				// -m: Transmssion mode
	int  m_Stuffing;			// -mS: ASI/SDI stuffing
	int  m_TxRate;				// -r: Transmission rate
	int  m_DataPid;				// -dP: PID of the data stream (T2MI/CCMB)

	int m_ModType;				// -mt: Master modulation type
	double m_CarrierFreq;		// -mf: carrier frequency
	double m_OutpLevel;			// -ml: Modulator output level
	int m_CodeRate;				// -mc: code rate
	int m_QamJ83Annex;			// -ma: J83 annex (QAM-A/B/C)

	int m_IqInterpFilter;		// -if: Interpolation filter to use in IQ mode

	double m_Snr;				// -snr: signal-to-noise ration (-1.0 is no noise)

	int  m_Constellation;		// -mC: constellation
	int  m_Bandwidth;			// -mB: bandwidth
	int  m_OfdmTxMode;			// -mT: transmision mode
	int  m_OfdmGuardItv;		// -mG: guard interval

	int  m_DvbS2GoldSeqInit;	// -mI: DVB-S2 gold sequence initialisation value
	int  m_DvbS2Pilots;			// -mP: DVB-S2 pilots
	int  m_DvbS2FecFrameLength;	// -mF: DVB-S2 FEC frame length

	int  m_DtmbFrameHdrMode;	// -mH: DTMB frame header mode

	DtTsIpPars m_IpPars;		// IP parameters.
								// -ipa: IP address and IP port
								// -ipp: IP protocol
								// -ipn: Number of TPs per IP packet

	bool  m_SilentMode;			// -s: Enable silent mode (i.e. no printf)
	bool  m_ShowHelp;			// -?: show help

	bool m_PlayDtSdiFile;		// Play .dtsdi file

protected:

	void  ParseParam(char* pParam, bool Flag, bool First=false, bool Last=false);
	void  ParseParamFlag(char* pParam, bool First=false, bool Last=false);
	void  ParseParamNotFlag(char* pParam, bool First=false, bool Last=false);

	bool  m_Error;				// Error detected while parsing command line?
	bool  m_LastFlagReqsArg;	// Last flag requires an argument
	char*  m_pLastFlag;			// Last flag
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Player -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
class Player
{
public:
	Player();
	virtual ~Player();

	//---- Public operations ----
	int Play(int argc, char* argv[]);

protected:

	//---- Internal operation ----
	void AttachToOutput();
	void AutoDetectSdiFormat();
	void DisplayPlayInfo();
	void InitIsdbtPars(DtIsdbtPars& IsdbtPars);
	void InitOutput();
	void Log(const char* pMessage, bool IgnoreSilence=false);
	void LogF(const char* pMessage, ... );
    void LoopFile();
	void ShowHelp();
	

	CommandLineParams  m_CmdLineParams;

	DtDevice  m_DtDvc;				// Our device
	DtOutpChannel  m_DtOutp;		// Our output channel
	int m_IoConfig;					// Our Ioconfig
	int	m_ParXtra;					// Our ParXtra
	bool  m_Modulator;				// Current output is a modulator
	bool  m_Ip;						// Current output is a IP port
	FILE* m_pFile;					// Our play file

	char* m_pBuf;					// Our data buffer
	int  m_ExitLoad;				// Load at which we should exit our loop
};

#endif // #ifndef __DTPLAY_H
