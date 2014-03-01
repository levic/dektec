//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*# Messages.c #*#*#*#*#*#*#*#*#*#*#*# (C) 2010 DekTec
//
// This file is generated, DO NOT EDIT!!!
//

#include <Messages.h>

const TMessage Messages[] = {
	{ KERN_INFO, "The Dtu driver V%s has loaded successfully."},
	{ KERN_ERR, "Failed to read serial number from VPD."},
	{ KERN_ERR, "Failed to get one or more property values from the property store (%s)."},
	{ KERN_ERR, "Failed to find a property table for device %s, this device is not supported by the Dtu driver."},
 };

Bool EventGetMessage(Int MsgId, const char** pLevel, const char** pMsg)
{
	if (MsgId < DTU_MSG_COUNT) {
		*pLevel = Messages[MsgId].pLevel;
		*pMsg = Messages[MsgId].pMsg;
		return TRUE;
	}
	return FALSE;
}

