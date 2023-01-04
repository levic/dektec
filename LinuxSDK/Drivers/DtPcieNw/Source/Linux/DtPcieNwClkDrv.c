// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtNwClkDrv.c *#*#*#*#*#*#*#*#*#*#* (C) 2020 DekTec
//

#include <DtNwIncludes.h>

// Clock driver

Int  DtNwClkDrvAdjFine(struct ptp_clock_info* ptp, long scaled_ppm)
{
	return -EOPNOTSUPP;
}

Int  DtNwClkDrvAdjFreq(struct ptp_clock_info* ptp, s32 delta)
{
	return -EOPNOTSUPP;
}

Int  DtNwClkDrvAdjPhase(struct ptp_clock_info* ptp, s32 phase)
{
	return -EOPNOTSUPP;
}

Int  DtNwClkDrvAdjTime(struct ptp_clock_info* ptp, s64 delta)
{
	return -EOPNOTSUPP;
}

Int  DtNwClkDrvGetTime64(struct ptp_clock_info* ptp, struct timespec64* ts)
{
	return -EOPNOTSUPP;
}

Int  DtNwClkDrvGetTimeX64(struct ptp_clock_info* ptp, struct timespec64* ts,
														 struct ptp_system_timestamp* sts)
{
	return -EOPNOTSUPP;
}

Int  DtNwClkDrvGetCrossTStamp(*getcrosststamp)(struct ptp_clock_info* ptp,
													struct system_device_crosststamp* cts)
{
	return -EOPNOTSUPP;
}

Int  DtNwClkDrvSetTime64(struct ptp_clock_info* p, const struct timespec64* ts)
{
	return -EOPNOTSUPP;
}

Int  DtNwClkDrvEnable(struct ptp_clock_info* ptp, struct ptp_clock_request* request,
																				   int on)
{
	return -EOPNOTSUPP;
}

Int  DtNwClkDrvVerify(struct ptp_clock_info* ptp, unsigned int pin,
											enum ptp_pin_function func, unsigned int chan)
{
	return -EOPNOTSUPP;
}

long  DtNwClkDrvDoAuxWork(struct ptp_clock_info* ptp)
{
	return -EOPNOTSUPP;
}

Int  DtNwClkDrvInit(DtNwIalData* pIalData, struct device* pParent)
{
	ptp_clock_info*  pPtpClockInfo = &pIalData->m_PtpClockInfo;

	// Fill the ptp_clock_info structure
	DtMemZero(pPtpClockInfo, sizeof(ptp_clock_info));
	strcpy(pPtpClockInfo->name, "DtNwPtp");
	pPtpClockInfo->owner = THIS_MODULE;
	pPtpClockInfo->adjfine = DtNwClkDrvAdjFine;
	pPtpClockInfo->adjfreq = DtNwClkDrvAdjFreq;
	pPtpClockInfo->adjphase = DtNwClkDrvAdjPhase;
	pPtpClockInfo->adjtime = DtNwClkDrvAdjTime;
	pPtpClockInfo->gettime64 = DtNwClkDrvGetTime64;
	pPtpClockInfo->gettimex64 = DtNwClkDrvGetTimeX64;
	pPtpClockInfo->getcrosststamp = DtNwClkDrvGetCrossTStamp;
	pPtpClockInfo->settime64 = DtNwClkDrvSetTime64;
	pPtpClockInfo->enable = DtNwClkDrvEnable;
	pPtpClockInfo->verify = DtNwClkDrvVerify;
	pPtpClockInfo->do_aux_work = DtNwClkDrvDoAuxWork;

	pPtpClockInfo->max_adj = 0;	// The maximum possible frequency adjustment, in parts per billon.
	pPtpClockInfo->n_alarm = 0; // The number of programmable alarms.
	pPtpClockInfo->n_ext_ts = 1; // The number of external time stamp channels.
	pPtpClockInfo->n_per_out = 0; // The number of programmable periodic signals.
	pPtpClockInfo->n_pins = 0; // The number of programmable pins.
	pPtpClockInfo->pps = 0; // Indicates whether the clock supports a PPS callback.
	pPtpClockInfo->pin_config = nullptr; //  Array of length 'n_pins'
	pIalData->m_PtpClock = ptp_clock_register(pPtpClockInfo, pParent);
}

Int  DtNwClkDrvClose(DtNwIalData* pIalData)
{
	int err = 0;
	if (pIalData->m_PtpClock != nullptr)
		err = ptp_clock_unregister(pIalData->m_PtpClock);
	pIalData->m_PtpClock = nullptr;
	return err;
}