//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtUsb.h *#*#*#*#*#*#*#*#*#* (C) 2011-2012 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2012 DekTec Digital Video B.V.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list
//     of conditions and the following disclaimer.
//  2. Redistributions in binary format must reproduce the above copyright notice, this
//     list of conditions and the following disclaimer in the documentation.
//  3. The source code may not be modified for the express purpose of enabling hardware
//     features for which no genuine license has been obtained.
//
// THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL DEKTEC DIGITAL VIDEO BV, ITS AGENTS OR ITS EMPLOYEES BE LIABLE FOR
// ANY DIRECT, INDIRECT, CONSEQUENTIAL, INCIDENTAL, OR OTHER DAMAGES (INCLUDING DAMAGES
// FOR THE LOSS OF USE, INFORMATION, GOODWILL, PROFIT, WORK STOPPAGE, DATA, BUSINESS OR
// REVENUE) UNDER ANY CIRCUMSTANCES, OR UNDER ANY LEGAL THEORY, WHETHER IN CONTRACT, IN
// TORT, IN NEGLIGENCE, OR OTHERWISE, ARISING FROM THE USE OF, OR INABILITY TO USE THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#ifndef __DT_USB_H
#define __DT_USB_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtSal.h"

#ifndef SKIP_USB

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- USB transfer direction -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DT_USB_HOST_TO_DEVICE      0
#define  DT_USB_DEVICE_TO_HOST      1

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public API -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

DtStatus  DtUsbQueuryString(DtDvcObject* pUsbDevice, DtString* pDtString,
                                                                        UInt StringIndex);
DtStatus  DtUsbPipeRead(DtDvcObject* pUsbDevice, void* pRequestObject, 
                                    Int PipeNum, UInt8* pBuf, Int NumToRead, Int Timeout);
DtStatus  DtUsbPipeWrite(DtDvcObject* pUsbDevice, void* pRequestObject, 
                                   Int PipeNum, UInt8* pBuf, Int NumToWrite, Int Timeout);
DtStatus  DtUsbVendorRequest(DtDvcObject* pUsbDevice, void* pRequestObject,
                                      UInt8 Code, UInt16 Value, UInt16 Index, Int Dir,
                                      UInt8* pBuf, Int BufSize, Int* pNumBytesTransferred,
                                      Int Timeout);
Int  DtUsbGetBulkPipeNumber(DtDvcObject* pUsbDevice, Int Dir, Int EndPoint);
void  DtUsbDumpPipeInfo(DtDvcObject* pUsbDevice);
Int  DtUsbGetCtrlMaxPacketSize(DtDvcObject*  pUsbDevice, Int  UsbSpeed);
Bool  DtUsbManufNameEq(DtDvcObject*  pDvc, const char*  pManufName);

#endif // #ifndef SKIP_USB
#endif // #ifndef __DT_USB_H
