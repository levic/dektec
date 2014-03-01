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

#define DTA_LOG_DRIVER_LOADED	0
#define DTA_LOG_SERIAL_FAILED	1
#define DTA_LOG_CONFIG_FAILED	2
#define DTA_LOG_FPGA_TIMEOUT	3
#define DTA_LOG_PROP_STORE_NOT_FOUND	4
#define DTA_LOG_FAKE_DMA_INT	5
#define DTA_LOG_SKIP_ADMIN_STATUS	6
#define DTA_LOG_ADMIN_STATUS_DOWN	7
#define DTA_LOG_ADMIN_STATUS_UP	8
#define DTA_LOG_NWVERSION_MISMATCH	9
#define DTA_MSG_COUNT	10

Bool EventGetMessage(Int MsgId, const char** pLevel, const char** pMsg);


#endif // end of __MESSAGES_H

