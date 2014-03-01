//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*# Messages.c #*#*#*#*#*#*#*#*#*#*#*# (C) 2010 DekTec
//
// This file is generated, DO NOT EDIT!!!
//

#include <Messages.h>

const TMessage Messages[] = {
	{ KERN_INFO, "The DtaNw driver V%s has loaded successfully."},
	{ KERN_INFO, "Link status: connected. (%s)"},
	{ KERN_INFO, "Link status: disconnected. (%s)"},
	{ KERN_ERR, "Dta driver version too old. Minimal Dta driver version %s"},
 };

Bool EventGetMessage(Int MsgId, const char** pLevel, const char** pMsg)
{
	if (MsgId < DTANW_MSG_COUNT) {
		*pLevel = Messages[MsgId].pLevel;
		*pMsg = Messages[MsgId].pMsg;
		return TRUE;
	}
	return FALSE;
}

