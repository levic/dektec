//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Exclusive.c *#*#*#*#*#*#*#*#* (C) 2000-2008 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// For a revision history of the Dtu2xx driver, please refer to the Dta1xx readme

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "Dtu2xx.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#define EXCL_ACC_FLAG_TX	0x0001		// Transmit exclusive-access flag
#define EXCL_ACC_FLAG_RX	0x0100		// Receive exclusive-access flag

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRequestExclusiveAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Called by DeviceIoControl to request exclusive access to an Rx channel.
// If exclusive access is granted, the thread pointer is stored, so that the exclusive-
// access lock can be released upon a CLOSE of the thread.
// The exclusive-access variables are protected by a spin lock.
//
Int  Dtu2xxRequestExclusiveAccess(
	IN PDTU2XX_FDO  pFdo,			// Functional device object
	IN struct file* pFileObject,	// File object requesting/dropping excl. access
	IN Int  PortIndex,				// Channel index
	IN Int  Request,				// 0 = Request exclusive access
									// 1 = Release exclusive access
	OUT Int*  pGranted)				// Granted Yes / No
{
	Channel* pCh=NULL;

	// Check port index
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels )
	{
		DTU2XX_LOG( KERN_INFO, "Dtu2xxRequestExclusiveAccess: PortIndex=%d INVALID",
					PortIndex );
		return -EFAULT;
	}
	pCh = &pFdo->m_Channel[PortIndex];

#if LOG_LEVEL > 0
	DTU2XX_LOG( KERN_INFO, "[%d] Dtu2xxRequestExclusiveAccess: Request=%d",
			    PortIndex, Request );
#endif

	if (Request!=0 && Request!=1)
	{
		DTU2XX_LOG( KERN_INFO, "[%d] Dtu2xxRequestExclusiveAccess: Request=%d ILLEGAL",
				    PortIndex, Request );
		return -EFAULT;
	}

	if ( 0!=down_interruptible(&pCh->m_ExclAccLock) )
		return -EFAULT;

	if (Request == 0)
	{
		// Request exclusive access
		if (pCh->m_ExclAccess == 0)
		{
			pCh->m_ExclAccess = 1;
			pCh->m_pExclAccFileObject = pFileObject;
			*pGranted = 1;
#if LOG_LEVEL > 0
			DTU2XX_LOG( KERN_INFO, "[%d] Dtu2xxRequestExclusiveAccess: Exclusive access "
					    "GRANTED", PortIndex );
#endif
		} else {
			*pGranted = 0;
#if LOG_LEVEL > 0
			DTU2XX_LOG( KERN_INFO, "[%d] Dtu2xxRequestExclusiveAccess: Exclusive access "
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
		DTU2XX_LOG( KERN_INFO, "[%d] Dtu2xxRequestExclusiveAccess: Exclusive access "
				    "RELEASED", PortIndex );
#endif
	}
	up(&pCh->m_ExclAccLock);
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxI2cReqExclAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  Dtu2xxI2cReqExclAccess(
	IN PDTU2XX_FDO  pFdo,			    // Functional device object
	IN struct file* pFileObj,	        // File object requesting/dropping excl. access
	IN Int  Request,					// 0 = Request exclusive access
										// 1 = Release exclusive access
	OUT Int*  pGranted)					// Granted Yes / No
{
    int Access;
    unsigned long OldIrqL;
    int Recursive;

    Access = 1;
    Recursive = 0;

	if (Request!=0 && Request!=1) {
		DTU2XX_LOG(KERN_INFO, "Dtu2xxI2cReqExclAccess: ILLEGAL "
				   "exclusive-access Request code %d", Request);
		return -EFAULT;
	}

    spin_lock_irqsave(&pFdo->m_I2cExclAccSpinLock, OldIrqL);
	if (Request == 0)
    {

		// Request exclusive access
		if (pFdo->m_pI2cExclAccFileObj == NULL) 
        {
			pFdo->m_pI2cExclAccFileObj = pFileObj;
			*pGranted = 1;
			pFdo->m_I2cExclAccRecursiveCount = 0;
		}
        else 
        {
            Access = (pFdo->m_pI2cExclAccFileObj == pFileObj);
			if (Access) 
            {
				*pGranted = 1;
				pFdo->m_I2cExclAccRecursiveCount++;
				Recursive = pFdo->m_I2cExclAccRecursiveCount;
			}
            else 
            {
				*pGranted = 0;
			}
		}
	}
    else
    {


		// Only release exclusive access if we owned it just once
        Access = (pFdo->m_pI2cExclAccFileObj == pFileObj);
		if (Access) 
        {
            Recursive = pFdo->m_I2cExclAccRecursiveCount;
			if (pFdo->m_I2cExclAccRecursiveCount == 0) 
            {
				pFdo->m_pI2cExclAccFileObj = NULL;
			}
            else
            {
				pFdo->m_I2cExclAccRecursiveCount--;
			}
		}
	}
    spin_unlock_irqrestore(&pFdo->m_I2cExclAccSpinLock, OldIrqL);

#if LOG_LEVEL > 0
    if (Access) 
    {
        if (Request == 0) 
        {
            if (Recursive == 0)
            { 
    			DTU2XX_LOG(KERN_INFO, "Dtu2xxI2cReqExclAccess: REQUEST "
    					   "exclusive access GRANTED");
            }
            else 
            {
    			DTU2XX_LOG(KERN_INFO, "Dtu2xxI2cReqExclAccess: REQUEST "
    					   "exclusive access GRANTED (recursive %d)", Recursive);
            }
        } 
        else 
        {
            if (Recursive == 0) 
            { 
        		DTU2XX_LOG(KERN_INFO, "Dtu2xxI2cReqExclAccess: RELEASED exclusive "
        				   "access");
            }
            else 
            {
    			DTU2XX_LOG(KERN_INFO, "Dtu2xxI2cReqExclAccess: Exclusive "
    			   "access not release yet (%d remaining)",
    			   Recursive);
            }
        }
    }
    else 
    {
		DTU2XX_LOG(KERN_INFO, "Dtu2xxI2cReqExclAccess: REQUEST "
				   "exclusive access DENIED");
    }
#endif
	
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxCheckExclusiveAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Called by Close to check whether the current file object held exclusive access to the
// device. If so, the exclusive-access lock is released.
//
Int  Dtu2xxCheckExclusiveAccess(
	IN PDTU2XX_FDO  pFdo,			// Functional device object
	IN struct file* pFileObject)	// File object for which we should check excludive access
{
	Int  i;
	unsigned long  OldIrqL;
	Channel*  pCh;				// Channel pointer
    int Release;

	// I2C port
	spin_lock_irqsave(&pFdo->m_I2cExclAccSpinLock, OldIrqL);
    Release = (pFdo->m_pI2cExclAccFileObj == pFileObject) && (pFdo->m_pI2cExclAccFileObj != NULL);
	if (Release)
	    pFdo->m_pI2cExclAccFileObj = NULL;
	pFdo->m_I2cExclAccRecursiveCount = 0;
	spin_unlock_irqrestore(&pFdo->m_I2cExclAccSpinLock, OldIrqL);
#if LOG_LEVEL > 0
    if (Release) {
        DTU2XX_LOG(KERN_INFO, "Dtu2xxCheckExclusiveAccess: I2C port: RELEASED exclusive access");
    }
#endif

	// Check all channels
	for (i=0; i<pFdo->m_NumChannels; i++)
	{
		pCh = &pFdo->m_Channel[i];
		if ( 0!=down_interruptible(&pCh->m_ExclAccLock) )
			return -EFAULT;

		if ( pCh->m_pExclAccFileObject==pFileObject && pCh->m_ExclAccess!=0 )
		{
			pCh->m_ExclAccess = 0;
#if LOG_LEVEL > 0
			DTU2XX_LOG( KERN_INFO, "[%d] Dtu2xxCheckExclusiveAccess: Exclusive-access lock "
						"CLEARED", i );
#endif
		}
		up(&pCh->m_ExclAccLock);
	}
	return 0;
}
