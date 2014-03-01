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

#define DTU_LOG_DRIVER_LOADED	0
#define DTU_LOG_SERIAL_FAILED	1
#define DTU_LOG_CONFIG_FAILED	2
#define DTU_LOG_PROP_STORE_NOT_FOUND	3
#define DTU_MSG_COUNT	4

Bool EventGetMessage(Int MsgId, const char** pLevel, const char** pMsg);


#endif // end of __MESSAGES_H

