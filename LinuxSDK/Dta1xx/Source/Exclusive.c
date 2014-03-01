//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Exclusive.c *#*#*#*#*#*#*#*#*#*#*# (C) 2007 DekTec
//
//  Routines to implement exclusive access to an Ts output or input channel.

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// For a revision history of the Dta1xx driver, please refer to the Dta1xx readme

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "../Include/Dta1xx.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#define EXCL_ACC_FLAG_TX	1		// Transmit exclusive-access flag
#define EXCL_ACC_FLAG_RX	256		// Transmit exclusive-access flag

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxRequestExclusiveAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Called by DeviceIoControl to request exclusive access.
// If exclusive access is granted, the thread pointer is stored, so that the exclusive-
// access lock can be released upon a CLOSE of the thread.
// The exclusive-access variables are protected by a lock.
//
Int  Dta1xxRequestExclusiveAccess(
	DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	Int PortIndex,			// Port index
	struct file* pFileObject,	// File object requesting/dropping excl. access
	Int  Request,			// 0 = Request exclusive access
							// 1 = Release exclusive access
	Int*  pGranted)			// Granted Yes / No
{
	Channel* pCh=NULL;

	// Check port index
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumNonIpChannels )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxRequestExclusiveAccess: PortIndex=%d INVALID",
					PortIndex );
#endif
		return -EFAULT;
	}
	pCh = &pFdo->m_Channel[PortIndex];

#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxRequestExclusiveAccess: Request=%d",
			    PortIndex, Request );
#endif

	if (Request!=0 && Request!=1)
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxRequestExclusiveAccess: Request=%d ILLEGAL",
				    PortIndex, Request );
#endif
		return -EFAULT;
	}

	if ( 0!=down_interruptible(&pCh->m_ExclAccLock) )
		return -ERESTARTSYS;

	if (Request == 0)
	{
		// Request exclusive access
		if (pCh->m_ExclAccess == 0)
		{
			pCh->m_ExclAccess = 1;
			pCh->m_pExclAccFileObject = pFileObject;
			*pGranted = 1;
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxRequestExclusiveAccess: Exclusive access "
					    "GRANTED", PortIndex );
#endif
		} else {
			*pGranted = 0;
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxRequestExclusiveAccess: Exclusive access "
					    "DENIED", PortIndex );
#endif
		}
	}
	else
	{
		// Release exclusive access.
		// Based on cooperative model: The exclusive-access lock is ALWAYS cleared,
		// without file-object check.
		// So, DeviceIoControl can also be used to unconditionally clear the lock.
		pCh->m_ExclAccess = 0;
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxRequestExclusiveAccess: Exclusive access "
				    "RELEASED", PortIndex );
#endif
	}
	up(&pCh->m_ExclAccLock);

	return 0;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxCheckExclusiveAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Called by Close to check whether the current file object held exclusive access to the
// device. If so, the exclusive-access lock is released.
//
Int  Dta1xxCheckExclusiveAccess(
	DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	struct file* pFileObject)	// Pointer to file object holding exclusive access
{
	Int  i;
	Channel*  pCh;				// Channel pointer

    // Release potential hold on I2C-lock
    if ( 0!=down_interruptible(&pFdo->m_I2cExclAccLock) )
        return -ERESTARTSYS;
    // Who has the lock?
    if (pFdo->m_pI2cExclAccFileObj==pFileObject)
    {
        // Current ownwer is closing handle to driver => reset lock count
        pFdo->m_I2cExclAccRecursiveCount = 0;
        pFdo->m_pI2cExclAccFileObj = NULL;   // Clear owner as well
#if LOG_LEVEL > 0
        DTA1XX_LOG(KERN_INFO, 
                         "Dta1xxCheckExclusiveAccess: I2C-exclusive-access lock CLEARED");
#endif
    }
    up(&pFdo->m_I2cExclAccLock);

	// Check all channels
	for (i=0; i<pFdo->m_NumChannels; i++)
	{
		pCh = &pFdo->m_Channel[i];
		if ( 0!=down_interruptible(&pCh->m_ExclAccLock) )
			return -ERESTARTSYS;

		if ( pCh->m_pExclAccFileObject==pFileObject && pCh->m_ExclAccess!=0 )
		{
			pCh->m_ExclAccess = 0;
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCheckExclusiveAccess: Exclusive-access lock "
						"CLEARED", i );
#endif
//			if (pCh->m_ChannelType == DTA1XX_NW_TX_CHANNEL) {
//				// Disable Nw receive
//				for (j = 0; j < pFdo->m_NumIpPorts; j++) {
//					if (pCh == pFdo->m_NwTx[j]) {
//						pFdo->m_NwStore[j].m_EnableNDisRx = FALSE;
//						DTA1XX_LOG( KERN_INFO, " **********CLOSE. NDIS Rx DISABLED**********" );
//					}
//				}
//			}
		}
		up(&pCh->m_ExclAccLock);
	}

	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxI2cReqExclAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  Dta1xxI2cReqExclAccess(
	DTA1XX_FDO*  pFdo,	       // Device Extension
	struct file*	 pFileObj, // FileObject that requests excl access
	Int  Request,			   // 0 = Request exclusive access
							   // 1 = Release exclusive access
	Int*  pGranted)		       // Granted Yes / No
{
	if (Request!=0 && Request!=1) 
    {
        DTA1XX_LOG(KERN_INFO, "Dta1xxI2cReqExclAccess: ILLEGAL "
				   "exclusive-access Request code %d", Request);
		return STATUS_INVALID_PARAMETER;
	}

    if ( 0!=down_interruptible(&pFdo->m_I2cExclAccLock) )
        return -ERESTARTSYS;

	if (Request == 0) 
    {
		// Request exclusive access
		if (pFdo->m_pI2cExclAccFileObj == NULL) 
        {
			pFdo->m_pI2cExclAccFileObj = pFileObj;
			*pGranted = 1;
			pFdo->m_I2cExclAccRecursiveCount = 0;
#if LOG_LEVEL > 0
			DTA1XX_LOG(KERN_INFO, "Dta1xxI2cReqExclAccess: REQUEST "
					                                          "exclusive access GRANTED");
#endif
		}
        else 
        {
			if (pFdo->m_pI2cExclAccFileObj == pFileObj) 
            {
				*pGranted = 1;
				pFdo->m_I2cExclAccRecursiveCount++;
#if LOG_LEVEL > 0
			DTA1XX_LOG(KERN_INFO, "Dta1xxI2cReqExclAccess: REQUEST "
					   "exclusive access GRANTED (recursive %d)", 
                                                        pFdo->m_I2cExclAccRecursiveCount);
#endif
			}
            else
            {
				*pGranted = 0;
#if LOG_LEVEL > 0
			DTA1XX_LOG(KERN_INFO, "Dta1xxI2cReqExclAccess: REQUEST "
					                                           "exclusive access DENIED");
#endif
			}
		}
	} 
    else 
    {
		// Only release exclusive access if we owned it just once
		if (pFdo->m_pI2cExclAccFileObj == pFileObj) 
        {
			if (pFdo->m_I2cExclAccRecursiveCount == 0) 
            {
				pFdo->m_pI2cExclAccFileObj = NULL;
#if LOG_LEVEL > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxI2cReqExclAccess: RELEASED exclusive "
				   "access");
#endif
			}
            else
            {
				pFdo->m_I2cExclAccRecursiveCount--;
#if LOG_LEVEL > 0
				DTA1XX_LOG(KERN_INFO, "Dta1xxI2cReqExclAccess: Exclusive "
				   "access not release yet (%d remaining)",
				   pFdo->m_I2cExclAccRecursiveCount);
#endif
			}
		}
	}
	up(&pFdo->m_I2cExclAccLock);
	
	return STATUS_SUCCESS;
}
