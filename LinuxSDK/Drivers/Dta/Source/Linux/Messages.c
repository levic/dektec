//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*# Messages.c #*#*#*#*#*#*#*#*#*#*#*# (C) 2010 DekTec
//
// This file is generated, DO NOT EDIT!!!
//

#include <Messages.h>

const TMessage Messages[] = {
	{ KERN_INFO, "The Dta driver V%s has loaded successfully."},
	{ KERN_ERR, "Failed to read serial number from VPD."},
	{ KERN_ERR, "Failed to get one or more property values from the property store (%s)."},
	{ KERN_ERR, "FPGA did not respond within time."},
	{ KERN_ERR, "Failed to find a property table for device %s, this device is not supported by the Dta driver."},
	{ KERN_ERR, "DMA interrupt occured but transfer was not started."},
	{ KERN_INFO, "Ignore network admin status!"},
	{ KERN_INFO, "Network admin status set to DOWN."},
	{ KERN_INFO, "Network admin status set to UP."},
	{ KERN_ERR, "DtaNw driver version too old. Minimal DtaNw driver version %s"},
 };

Bool EventGetMessage(Int MsgId, const char** pLevel, const char** pMsg)
{
	if (MsgId < DTA_MSG_COUNT) {
		*pLevel = Messages[MsgId].pLevel;
		*pMsg = Messages[MsgId].pMsg;
		return TRUE;
	}
	return FALSE;
}

