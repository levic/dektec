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

#define DTPCIE_LOG_DRIVER_LOADED	0
#define DTPCIE_LOG_SERIAL_FAILED	1
#define DTPCIE_LOG_CONFIG_FAILED	2
#define DTPCIE_LOG_FPGA_TIMEOUT	3
#define DTPCIE_LOG_PROP_STORE_NOT_FOUND	4
#define DTPCIE_LOG_FAKE_DMA_INT	5
#define DTPCIE_LOG_UNSUPP_FWVARIANT	6
#define DTPCIE_LOG_ERROR_GENERIC	7
#define DTPCIE_LOG_INFO_GENERIC	8
#define DTPCIE_MSG_COUNT	9

Bool EventGetMessage(Int MsgId, const char** pLevel, const char** pMsg);


#endif // end of __MESSAGES_H

