//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Messages.h *#*#*#*#*#*#*#*#*#*#*# (C) 2010 DekTec
//
// This file is generated, DO NOT EDIT!!!
//

#ifndef __MESSAGES_H
#define __MESSAGES_H

#include <DtSal.h>

typedef struct _TMessage
{
	const char* pLevel;
	const char* pMsg;
} TMessage;

#define DTANW_LOG_DRIVER_LOADED	0
#define DTANW_LOG_LINK_CONNECTED	1
#define DTANW_LOG_LINK_DISCONNECTED	2
#define DTANW_LOG_VERSION_MISMATCH	3
#define DTANW_LOG_FATAL_ERROR	4
#define DTANW_MSG_COUNT	5

Bool EventGetMessage(Int MsgId, const char** pLevel, const char** pMsg);


#endif // end of __MESSAGES_H

