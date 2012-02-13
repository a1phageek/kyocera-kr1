/*
 * Copyright (c) 2002-2005 Atheros Communications, Inc.
 * All rights reserved.
 *
 */

#include "opt_ah.h"

#ifdef AH_SUPPORT_AR5212

#include "ah.h"
#include "ah_internal.h"

#include "ar5212/ar5212.h"
#include "ar5212/ar5212reg.h"
#include "ar5212/ar5212desc.h"
#include "ar5212/ar5212phy.h"
#include "ar5212/ar5212radar.h"

/*
 * Finds the radar state entry that matches the current channel
 */

static u_int32_t
_round(int32_t val)
{
	u_int32_t ival,rem;

	if (val < 0)
		return 0;
	ival = val/100;
	rem = val-(ival*100);
	if (rem <50)
		return ival;
	else
		return(ival+1);
}

static void
printDelayLine(struct ar5212RadarDelayLine *dl)
{
	int i=0,index;
	struct ar5212RadarDelayElem *de;

	index = dl->dl_lastElem;
 	for (i=0; i<dl->dl_numElems; i++) {
		de = &dl->dl_elems[index];
		ath_hal_printf(AH_NULL, "Elem %d: ts = %u (0x%x) dur=%u\n",i,
			       de->de_time, de->de_time, de->de_dur);
		index = (index - 1)& HAL_MAX_DL_MASK;
	}
}

static void
printRadarFilters(struct ar5212RadarFilter *rf)
{
	int i=0;
	struct ar5212RadarDelayElem *de;

 	for (i=0; i<rf->rf_numPulses; i++) {
		de = &rf->rf_pulses[i];
		ath_hal_printf(AH_NULL, "Elem %d: ts = %u (0x%x) dur=%u\n",i,
			       de->de_time, de->de_time, de->de_dur);
	}
}

static void
printRadarTempPulse(struct ar5212RadarDelayElem *de, u_int32_t numPulses)
{
	int i;

	for (i=0;numPulses; i++) {
		ath_hal_printf(AH_NULL, "Elem %d: ts = %u (0x%x) dur=%u\n",i,
			       de[i].de_time, de[i].de_time, de[i].de_dur);
	}
}



struct ar5212RadarState *
ar5212GetRadarChanState(struct ath_hal *ah, u_int8_t *index)
{
	struct ath_hal_5212 *ahp=AH5212(ah);
	struct ar5212RadarState *rs=AH_NULL;
	int i;
	
	for (i = 0; i < HAL_NUM_RADAR_STATES; i++) {
		if (ahp->ah_radar[i].rs_chan == AH_PRIVATE(ah)->ah_curchan) {
			if (index != AH_NULL)
				*index = (u_int8_t) i;
			return &(ahp->ah_radar[i]);
		}
	}
	/* No existing channel found, look for first free channel state entry */
	for (i=0; i < HAL_NUM_RADAR_STATES; i++) {
		if (ahp->ah_radar[i].rs_chan == AH_NULL) {
			rs = &(ahp->ah_radar[i]);
			/* Found one, set channel info and default thresholds */
			rs->rs_chan = AH_PRIVATE(ah)->ah_curchan;
			rs->rs_firpwr = HAL_RADAR_FIRPWR;
			rs->rs_radarRssi = HAL_RADAR_RRSSI;
			rs->rs_height = HAL_RADAR_HEIGHT;
			rs->rs_pulseRssi = HAL_RADAR_PRSSI;
			rs->rs_inband = HAL_RADAR_INBAND;
			if (index != AH_NULL)
				*index = (u_int8_t) i;
			return (rs);
		}
	}
	HALDEBUG(ah, "%s: No more radar states left.\n", __func__);
	return(AH_NULL);
}

void
ar5212ResetAR(struct ath_hal *ah)
{
	struct ath_hal_5212 *ahp=AH5212(ah);

	OS_MEMZERO(&ahp->ah_ar, sizeof(ahp->ah_ar));
	ahp->ah_ar.ar_packetThreshold = HAL_AR_PKT_COUNT_THRESH;
	ahp->ah_ar.ar_parThreshold = HAL_AR_ACK_DETECT_PAR_THRESH;
}

static void
ar5212ResetArQ(struct ath_hal *ah)
{
	struct ath_hal_5212 *ahp = AH5212(ah);

	OS_MEMZERO(ahp->ah_arq, sizeof(struct ar5212RadarQElem)*HAL_ARQ_SIZE);
	OS_MEMZERO(&ahp->ah_arqInfo, sizeof(ahp->ah_arqInfo));
	ahp->ah_arqInfo.ri_qsize = HAL_ARQ_SIZE;
	ahp->ah_arqInfo.ri_seqSize = HAL_ARQ_SEQSIZE;
}

/*
 * Clear all delay lines for all filter types
 */

static void
ar5212ResetAllDelayLines(struct ath_hal *ah)
{
	struct ath_hal_5212 *ahp = AH5212(ah);
	struct ar5212RadarDelayLine *dl;
	int i;

	for (i=0; i<HAL_MAX_RADAR_TYPES; i++) {
		dl = &(ahp->ah_radarf[i].ft_dl);
		OS_MEMZERO(dl, sizeof(struct ar5212RadarDelayLine));
		dl->dl_lastElem = (0xFFFFFFFF)&HAL_MAX_DL_MASK;
	}
}

/*
 * Clear only a single delay line
 */

static void
ar5212ResetDelayLine(struct ar5212RadarDelayLine *dl)
{
	OS_MEMZERO(&(dl->dl_elems[0]), sizeof(dl->dl_elems));
	dl->dl_lastElem = (0xFFFFFFFF)&HAL_MAX_DL_MASK;
}
	

/*
 * Clear/Reset radar information for all channel states
 * Also clears all radar delay lines for all filter types
 */

void
ar5212ResetRadar(struct ath_hal *ah)
{
	struct ath_hal_5212 *ahp = AH5212(ah);

	/* Free radar state delay lines */
	ar5212ResetAllDelayLines(ah);
}

/*
 * Clear/Reset radar information for a single radar state/channel
 * Also clears all radar delay lines for all filter types
 */

static void
ar5212ResetRadarChannel(struct ath_hal *ah, struct ar5212RadarState *rs)
{
	ar5212ResetAllDelayLines(ah);
	rs->rs_numRadarEvents=0;
}

static void
ar5212ResetRadarQ(struct ath_hal *ah)
{
	struct ath_hal_5212 *ahp = AH5212(ah);

	OS_MEMZERO(ahp->ah_radarq, sizeof(struct ar5212RadarQElem)*HAL_RADARQ_SIZE);
	OS_MEMZERO(&ahp->ah_radarqInfo, sizeof(ahp->ah_radarqInfo));
	ahp->ah_radarqInfo.ri_qsize = HAL_RADARQ_SIZE;
	ahp->ah_radarqInfo.ri_seqSize = HAL_RADARQ_SEQSIZE;
}

/*
 * Reverse the pulses for each filter so they represent h(-t)
 */

static void
ar5212ReverseFilter(struct ar5212RadarDelayElem *origPulses, struct ar5212RadarFilter *rf)
{
	u_int32_t n,numPulses;
	struct ar5212RadarDelayElem *orig_de;

	numPulses = rf->rf_numPulses;
	for (n=0; n<numPulses;n++) {
		orig_de = &(origPulses[numPulses-n-1]);
		rf->rf_pulses[n].de_time = rf->rf_filterLen - orig_de->de_time - orig_de->de_dur;
		rf->rf_pulses[n].de_dur = orig_de->de_dur;
	}
}

/*
 * 1,2 us gives dur 0
 * 5 us gives dur 9->11 = 7->9us
 * 10us gives 15->18  = 12->15us
 * 11us               = 14-16us
 * 12us		      = 15-16us
 * 13us               = 17-18us
 * 14us               = 17-19us
 * 15us gives 23-24 = 18-21us
 * 16us		      = 19-22us
 * 17us               = 21-22us
 * 18us               = 22-23us
 * 19us               = 22-24us
 * 20us gives 29-30 = 23-24
 * 30us gives 41-42 = 33-34
 */

#if 0
/* The following are ETSI radar pulse */
struct ar5212RadarPulse ar5212_radars[] = {
	{15, 1, 755, 48, 50, 0, 2, 27, 0},

	{10, 1, 200, 24, 80, 0, 2, 27, 50},
	{10, 1, 300, 24, 80, 0, 2, 27, 50},
	{10, 1, 500, 24, 80, 0, 2, 27, 12},
	{10, 1, 800, 24, 80, 0, 2, 27, 12},
	{10, 1, 1000, 24, 80, 0, 2, 27, 12},

	{10, 5, 202, 35, 100, 6, 9, 0, 0},
	{10, 5, 303, 35, 90, 6, 9, 0, 0},
	{10, 5, 505, 24, 100, 6, 9,0, 0},
	{10, 5, 800, 24, 90, 6, 9, 0, 12},
	{10, 5, 1009, 24, 90, 6, 9,0, 12},

	{15, 10, 202, 24, 90, 12, 15, 0, 8},
	{15, 10, 302, 24, 90, 12, 15, 0, 8},
	{15, 10, 502, 24, 90, 12, 15, 0, 12},
	{15, 10, 803, 24, 90, 12, 15, 0, 12},
	{15, 10, 1009, 24, 90, 12, 15,0, 12},

	{15, 15, 202, 24, 90, 17, 19, 0, 10},
	{15, 15, 303, 24, 90, 17, 19, 0, 0},
	{15, 15, 504, 24, 90, 17, 19, 0, 12},
	{15, 15, 806, 24, 90, 17, 19, 0, 12},
	{15, 15, 1013, 24, 90, 17, 19,0, 12},

	{15, 1, 1200, 24, 80, 0, 2, 27, 12},
	{15, 1, 1489, 24, 80, 0, 2, 27, 12},
	{15, 1, 1583, 24, 80, 0, 2, 27, 12},
	{15, 5, 1200, 24, 90, 6, 9, 0, 12},
	{15, 5, 1489, 24, 90, 6, 9, 0, 12},
	{15, 5, 1585, 24, 90, 6, 9, 0, 12},

	{15, 10, 1210, 24, 90, 12, 15, 0, 12},
	{15, 10, 1510, 24, 90, 12, 15, 0, 12},
	{15, 10, 1612, 24, 90, 12, 15, 0, 12},
	{15, 15, 1215, 24, 90, 17, 19, 0, 12},
	{15, 15, 1515, 24, 90, 17, 19, 0, 12},
	{15, 15, 1620, 24, 90, 17, 19, 0, 12},

	{25, 1, 2255, 24, 80, 0, 2, 27, 12},
	{25, 1, 2910, 24, 80, 0, 2, 27, 12},
	{25, 1, 3390, 24, 80, 0, 2, 27, 12},
	{25, 1, 3850, 24, 80, 0, 2, 27, 12},
	{25, 5, 2290, 24, 90, 6, 9, 0, 12},
	{25, 5, 2950, 24, 90, 6, 9, 0, 12},
	{25, 5, 3420, 24, 90, 6, 9, 0, 12},
	{25, 5, 3900, 24, 90, 6, 9, 0, 12},
	{25, 10, 2290, 24, 90, 12, 15, 0, 12},
	{25, 10, 2970, 24, 90, 12, 15, 0, 12},
	{25, 10, 3450, 24, 90, 12, 15, 0, 12},
	{25, 10, 3930, 24, 90, 12, 15, 0, 12},
	{25, 15, 2315, 24, 90, 17, 19, 0, 12},
	{25, 15, 3030, 24, 90, 17, 19, 0, 12},
	{25, 15, 3560, 24, 90, 17, 19, 0, 12},
	{25, 15, 4090, 24, 90, 17, 19, 0, 12},

	{20, 20, 2030, 24, 90, 22, 24, 0, 12},
	{20, 20, 3040, 24, 90, 22, 24, 0, 12},
	{20, 20, 4070, 24, 90, 22, 24, 0, 12},

	{20, 30, 2090, 24, 90, 32, 34, 0, 12},
	{20, 30, 3170, 24, 90, 32, 34, 0, 12},
	{20, 30, 4000, 24, 90, 32, 34, 0, 12},
	};
#endif

/* The following are for FCC Bin 1-4 pulses */
struct ar5212RadarPulse ar5212_radars[] = {
	
	{9,  1, 3020, 40, 70, 0,  2,  27, 0},
	
	{10, 1,  704, 40, 70, 0,  2,  27, 0},
	
	{10, 2, 6666, 90, 70, 0, 3, 27, 0},
	{10, 2, 5900, 90, 70, 0, 3, 27, 0},
	{10, 2, 5200, 90, 70, 0, 3, 27, 0},
	{10, 2, 4800, 90, 70, 0, 3, 27, 0},
	{10, 2, 4400, 60, 70, 0, 3, 27, 0},
	{10, 5, 6666, 70, 70, 3, 9, 27, 0},
	{10, 5, 5900, 70, 70, 3, 9, 27, 0},
	{10, 5, 5200, 70, 70, 3, 9, 27, 0},
	{10, 5, 4800, 70, 70, 3, 9, 27, 0},
	{10, 5, 4400, 50, 70, 3, 9, 27, 0},

	{8, 10, 5000, 100, 60, 9, 12, 27, 0},
	{8, 10, 3000, 100, 60, 9, 12, 27, 0},
	{8, 10, 2000, 40, 60, 9, 12, 27, 0},
	{8, 14, 5000, 100, 60, 13, 16, 27, 0},
	{8, 14, 3000, 100, 60, 13, 16, 27, 0},
	{8, 14, 2000, 40, 60, 13, 16, 27, 0},

	{6, 16, 5000, 80, 70, 14, 19, 27, 0},
	{6, 16, 3000, 80, 70, 14, 19, 27, 0},
	{6, 16, 2000, 40, 70, 14, 19, 27, 0},
	{6, 21, 5000, 80, 70, 19, 24, 27, 0},
	{6, 21, 3000, 80, 70, 19, 24, 27, 0},
	{6, 21, 2000, 40, 70, 19, 24, 27, 0},
};

HAL_STATUS
ar5212RadarAttach(struct ath_hal *ah)
{
#define	N(a)	(sizeof(a)/sizeof(a[0]))
	struct ath_hal_5212 *ahp=AH5212(ah);
	int numPulses,p,n,numRadars;
	u_int32_t tsfStart,tsfEnd,zeroSum;
	u_int32_t T,t,i,temp;
	struct ar5212RadarFilterType *ft;
	struct ar5212RadarFilter *rf;
	struct ar5212RadarDelayElem *temp_pulse;

	if (ar5212GetCapability(ah, HAL_CAP_PHYDIAG, HAL_CAP_RADAR, AH_NULL) == HAL_OK) {
		ahp->ah_procPhyErr |= HAL_RADAR_EN;
		ahp->ah_radarq = ath_hal_malloc(sizeof(struct ar5212RadarQElem)*HAL_RADARQ_SIZE);
		if (ahp->ah_radarq == AH_NULL) {
			HALDEBUG(ah, "%s: cannot allocate memory for radar q\n",
				       __func__);
			return HAL_ENOMEM;
		}
		numRadars = N(ar5212_radars);
		ahp->ah_rinfo.rn_useNol = AH_TRUE;
		ahp->ah_rinfo.rn_numRadars = 0;
		temp_pulse = ath_hal_malloc(sizeof(struct ar5212RadarDelayElem)*HAL_MAX_DL_SIZE);
		if (temp_pulse == AH_NULL) {
			HALDEBUG(ah, "%s: cannot allocate memory for temp delay line\n",
				       __func__);
			goto bad;
		}
		OS_MEMZERO(temp_pulse,HAL_MAX_DL_SIZE*sizeof(struct ar5212RadarDelayElem));

		/* Now, initialize the radar filters */
		for (p=0; p<numRadars; p++) {
			ft = AH_NULL;
			for (n=0; n<ahp->ah_rinfo.rn_numRadars; n++) {
				if (ar5212_radars[p].rp_pulseDur == 
				    ahp->ah_radarf[n].ft_filterDur) {
					ft = &(ahp->ah_radarf[n]);
					break;
				}
			}
			if (ft == AH_NULL) {
				/* No filter of the appropriate dur was found */
				ft = &(ahp->ah_radarf[ahp->ah_rinfo.rn_numRadars++]);
				ar5212ResetDelayLine(&ft->ft_dl);
				ft->ft_numFilters = 0;
				ft->ft_maxFilterLen = 0;
				ft->ft_minDur = ar5212_radars[p].rp_minDur;
				ft->ft_maxDur = ar5212_radars[p].rp_maxDur;
				ft->ft_filterDur = ar5212_radars[p].rp_pulseDur;
				ft->ft_rssiThresh = ar5212_radars[p].rp_rssiThresh;
			}
			rf = &(ft->ft_filters[ft->ft_numFilters++]);

			numPulses = ar5212_radars[p].rp_numPulses;
			T = (100000000/ar5212_radars[p].rp_pulseFreq) -
				100*(ar5212_radars[p].rp_pulseVar) - 100*(ar5212_radars[p].rp_meanOffset);
			rf->rf_numPulses = numPulses;
			rf->rf_pulseDur = ft->ft_maxDur;
			rf->rf_pulses = ath_hal_malloc(numPulses*sizeof(struct ar5212RadarDelayElem));
			if (rf->rf_pulses == AH_NULL) {
				HALDEBUG(ah,"Cannot allocate memory for rf pulses\n");
				goto bad1;
			}
			OS_MEMZERO(rf->rf_pulses, numPulses*sizeof(struct ar5212RadarDelayElem));

			for (n=0,t=0,tsfEnd=0; n<numPulses; n++) {
				tsfStart = _round((((int32_t) t) - 
						   (int32_t)(100*ar5212_radars[p].rp_pulseVar)));
				t += 100*(ar5212_radars[p].rp_pulseDur+ar5212_radars[p].rp_pulseVar);
				tsfEnd =  _round((int32_t)t);
				HALDEBUG(ah,"tsfEnd = %u\n",tsfEnd);
				temp_pulse[n].de_time = tsfStart;
				temp_pulse[n].de_dur = tsfEnd - tsfStart + 1;
				t += T;
			}
			rf->rf_filterLen = tsfEnd+1;
			if (rf->rf_filterLen > ft->ft_maxFilterLen)
				ft->ft_maxFilterLen = rf->rf_filterLen;
			ar5212ReverseFilter(temp_pulse,rf);
			zeroSum = rf->rf_filterLen - 2*ar5212_radars[p].rp_numPulses *
				ar5212_radars[p].rp_maxDur;
#if (AH_RADAR_CALIBRATE == 3)
			printRadarFilters(rf);
			ath_hal_printf(ah,"Zero Sum = %d\n",zeroSum);
#endif

			temp = (ar5212_radars[p].rp_minDur + ar5212_radars[p].rp_maxDur+2)/2;
			rf->rf_threshold = (2*temp*ar5212_radars[p].rp_numPulses)*ar5212_radars[p].rp_threshold/100+zeroSum;

		}
		ath_hal_free(temp_pulse);
		temp_pulse = AH_NULL;
		ar5212ResetAllDelayLines(ah);
		ar5212ResetRadarQ(ah);
	}
	if (ar5212GetCapability(ah, HAL_CAP_PHYDIAG, HAL_CAP_AR, AH_NULL) == HAL_OK) {
		ahp->ah_procPhyErr |= HAL_AR_EN;
		ahp->ah_arq = ath_hal_malloc(sizeof(struct ar5212RadarQElem)*HAL_ARQ_SIZE);
		if (ahp->ah_arq == AH_NULL) {
			HALDEBUG(ah, "%s: cannot allocate memory for AR q\n",
				 __func__);
			if (ar5212GetCapability(ah, HAL_CAP_PHYDIAG, HAL_CAP_RADAR, AH_NULL) == HAL_OK)
				goto bad1;
			return HAL_ENOMEM;
		}
		ar5212ResetAR(ah);
		ar5212ResetArQ(ah);
	}
	ahp->ah_curchanRadIndex = -1;
	return HAL_OK;

bad1:
	ath_hal_free(temp_pulse);
	temp_pulse = AH_NULL;
	for (i=0; i<ft->ft_numFilters; i++) {
		rf = &(ft->ft_filters[i]);
		ath_hal_free(rf->rf_pulses);
		rf->rf_pulses = AH_NULL;
	}
bad:
	ath_hal_free(ahp->ah_radarq);
	ahp->ah_radarq = AH_NULL;
	return HAL_ENOMEM;
	
#undef N
};

void
ar5212RadarDetach(struct ath_hal *ah)
{
	struct ath_hal_5212 *ahp = AH5212(ah);
	int p,s,n;
	if (ahp->ah_radarq != AH_NULL) {
		ar5212ResetAllDelayLines(ah);
		if (ahp->ah_radarq != AH_NULL) {
			ath_hal_free(ahp->ah_radarq);
			ahp->ah_radarq=AH_NULL;
		}
		for (p=0;p<ahp->ah_rinfo.rn_numRadars; p++) {
			for (n=0; n<ahp->ah_radarf[p].ft_numFilters; n++)
				if (ahp->ah_radarf[p].ft_filters[n].rf_pulses != AH_NULL) {
					ath_hal_free(ahp->ah_radarf[p].ft_filters[n].rf_pulses);
					ahp->ah_radarf[p].ft_filters[n].rf_pulses = AH_NULL;
				}
		}
		if (ahp->ah_radarNol != AH_NULL) {
			struct ar5212RadarNolElem *nol, *next;
			nol = ahp->ah_radarNol;
			while (nol != AH_NULL) {
				next = nol->nol_next;
				ath_hal_free(nol);
				nol = next;
			}
			ahp->ah_radarNol = AH_NULL;
		}
	}
	if (ahp->ah_arq != AH_NULL) {
		ar5212ResetAR(ah);
		if (ahp->ah_arq) {
			ath_hal_free(ahp->ah_arq);
			ahp->ah_arq=AH_NULL;
		}
	}
}

static void
ar5212QueueRadarEvent(struct ath_hal *ah, struct ath_desc *ds,
		      struct ar5212RadarQElem *q, struct ar5212RadarQInfo *qInfo,
		      u_int64_t fullTsf)
{
	u_int32_t index,seqNum;
	volatile register u_int32_t *data;
	volatile u_int32_t busy,val;
	u_int16_t qsize, seqsize, wrSeqNum, rdSeqNum, thisTimeStamp;
	u_int32_t ts,lowerts;
	u_int8_t rssi;
	u_int32_t dur;

	rssi = (u_int8_t) ds->ds_rxstat.rs_rssi;
	dur = (ds->ds_rxstat.rs_datalen ?		/* Only lower 8 bits are valid */
	       (u_int32_t)(*((u_int8_t *) ds->ds_vdata)) : 0) & 0xff;
	if ((rssi == 0) && (dur == 0))
		return;

	qsize = qInfo->ri_qsize;
	seqsize = qInfo->ri_seqSize;

	index = qInfo->ri_writer.wr_index;
	seqNum = qInfo->ri_writer.wr_seq;
	data = &q[index].rq_busy;
	qInfo->ri_reader.rd_start = 1;
	
	busy = *data;
	if (busy) {
		/* 
		 * If the entry is busy, we can still write to it if
		 * the entry was already processed.  If the reader has read the
		 * sequence numbers before we get here, then it will just wait till
		 * the next time. Otherwise, it will not know the difference.
		 */
		data = &q[index].rq_seqNum;
		val = *data;
		wrSeqNum  = val & HAL_RADAR_SMASK;
		rdSeqNum = (val >> HAL_RADAR_SSHIFT) & HAL_RADAR_SMASK;
		if (wrSeqNum != rdSeqNum) {
			index = (index + 1)%qsize;
			seqNum = (seqNum + 1)%seqsize;
			qInfo->ri_reader.rd_resetVal =
				((seqNum & HAL_RADAR_SMASK) << HAL_RADAR_ISHIFT) |
				(index & HAL_RADAR_IMASK);
		}
	} else {
		data = &q[index].rq_seqNum;
		val = *data;
		wrSeqNum  = val & HAL_RADAR_SMASK;
		rdSeqNum = (val >> HAL_RADAR_SSHIFT) & HAL_RADAR_SMASK;
		if (wrSeqNum != rdSeqNum) {
			val = (((index + (qsize >> 1)) % qsize) & HAL_RADAR_IMASK) |
				((((seqNum - (qsize >> 1)) % seqsize) & HAL_RADAR_SMASK) << HAL_RADAR_ISHIFT);
			qInfo->ri_reader.rd_resetVal = val;
		}
	}
	q[index].rq_event.re_rssi = (u_int8_t) ds->ds_rxstat.rs_rssi;
	q[index].rq_event.re_dur = (ds->ds_rxstat.rs_datalen ? 
			   (u_int32_t)(*((u_int8_t *) ds->ds_vdata)) : 0) & 0xff;	/* Only lower 8 bits are valid */
	q[index].rq_event.re_chanIndex = (u_int8_t) AH5212(ah)->ah_curchanRadIndex;

	q[index].rq_event.re_ts = (ds->ds_rxstat.rs_tstamp) & HAL_RADAR_TSMASK;
	q[index].rq_event.re_fullts = fullTsf;

	q[index].rq_seqNum = (HAL_RADAR_SMASK << HAL_RADAR_SSHIFT) | 
		(seqNum & HAL_RADAR_SMASK);
	qInfo->ri_writer.wr_index = (index + 1)%qsize;
	qInfo->ri_writer.wr_seq = (seqNum + 1)%seqsize;
	qInfo->ri_numWrite++;
}

/*
 * Returns the next radar event in the structure provided by *radarEvent.
 * Also returns AH_TRUE if the returned value is valid.
 * Otherwise, AH_FALSE is returned indicating no more data is available.
 */

HAL_BOOL
ar5212DeQueueRadarEvent(struct ar5212RadarQElem *q,
			struct ar5212RadarQInfo *qInfo,
			struct ar5212RadarEvent *radarEvent,
			HAL_BOOL *flush)
{
	volatile register u_int32_t *data;
	volatile u_int32_t rval;
	u_int16_t qsize, seqsize, index, expSeqNum;
	u_int16_t ws,rs;

	*flush = AH_FALSE;
	if (qInfo->ri_reader.rd_start) {
		qsize = qInfo->ri_qsize;
		seqsize = qInfo->ri_seqSize;

		index = qInfo->ri_reader.rd_index;
		expSeqNum = qInfo->ri_reader.rd_expSeq;

		while (1) {
			data = &q[index].rq_busy;
			*data = 1;
			data = &q[index].rq_seqNum;
			ws = ((*data) & HAL_RADAR_SMASK);
			rs = ((*data) >> HAL_RADAR_SSHIFT);
			if (ws == rs) {
				return AH_FALSE;
			} else {
				if (expSeqNum == ws) {
					*radarEvent = q[index].rq_event;
					qInfo->ri_reader.rd_index = (index + 1)%qsize;
					qInfo->ri_reader.rd_expSeq = (expSeqNum + 1)%seqsize;
					qInfo->ri_numRead++;
					q[index].rq_seqNum = (ws << HAL_RADAR_SSHIFT)| ws;
					*data = 0;
					return AH_TRUE;
				} else {
					u_int16_t oldIndex, oldExpSeqNum;
					*flush = AH_TRUE;
					data = &qInfo->ri_reader.rd_resetVal;
					rval = *data;
					oldIndex = index;
					oldExpSeqNum = expSeqNum;
					index = (rval & HAL_RADAR_IMASK);
					expSeqNum = (rval >> HAL_RADAR_ISHIFT) & HAL_RADAR_SMASK;
					if ((index == oldIndex) && (expSeqNum == oldExpSeqNum))
					    return AH_FALSE;
				}
			}
		}
	} else
		return AH_FALSE;
}

HAL_BOOL
ar5212HaveRadarEvent(struct ath_hal *ah)
{
	struct ath_hal_5212 *ahp=AH5212(ah);
	
	if (ahp->ah_radarqInfo.ri_numRead == ahp->ah_radarqInfo.ri_numWrite)
		return AH_FALSE;
	else
		return AH_TRUE;
}


void
ar5212ArEnable(struct ath_hal *ah)
{
	struct ath_hal_5212 *ahp=AH5212(ah);
	u_int32_t rfilt,val;
	struct ar5212ArState *ar;
	HAL_CHANNEL_INTERNAL *chan=AH_PRIVATE(ah)->ah_curchan;

	ar = (struct ar5212ArState *) &ahp->ah_ar;
	if (ahp->ah_procPhyErr & (HAL_RADAR_EN | HAL_AR_EN)) {
		if ((chan->channelFlags & CHANNEL_108G) == CHANNEL_108G) {
			/* We are in turbo G, so enable AR*/
			HALDEBUG(ah, "Enabling ar\n");
			ar5212ResetAR(ah);
			val = OS_REG_READ(ah, AR_PHY_RADAR_0);
			ar->ar_radarRssi = HAL_AR_RADAR_RSSI_THR;
			val |= SM(ar->ar_radarRssi, AR_PHY_RADAR_0_RRSSI);
			rfilt = ar5212GetRxFilter(ah);
			rfilt |= HAL_RX_FILTER_PHYRADAR;
			ar5212SetRxFilter(ah,rfilt);
			OS_REG_WRITE(ah, AR_PHY_RADAR_0, val|AR_PHY_RADAR_0_ENA);
		}
	} else {
		HALDEBUG(ah,"Disabling ar\n");
		rfilt = ar5212GetRxFilter(ah);
		rfilt &= ~HAL_RX_FILTER_PHYRADAR;
		ar5212SetRxFilter(ah,rfilt);
	}
}

/*
 * Disable all Radar detection (AR or Radar) 
 */

void
ar5212RadarDisable(struct ath_hal *ah)
{
	u_int32_t rfilt;

	HALDEBUG(ah, "Disabling radar\n");
	rfilt = ar5212GetRxFilter(ah);
	rfilt &= ~HAL_RX_FILTER_PHYERR;
	ar5212SetRxFilter(ah,rfilt);
	OS_REG_WRITE(ah, AR_PHY_RADAR_0,
		     OS_REG_READ(ah, AR_PHY_RADAR_0) & ~AR_PHY_RADAR_0_ENA);
}

void ar5212RadarEnable(struct ath_hal *ah)
{
	struct ath_hal_5212 *ahp=AH5212(ah);
	u_int32_t rfilt,val;
	struct ar5212RadarState *rs;
	u_int8_t index;
	HAL_CHANNEL_INTERNAL *chan=AH_PRIVATE(ah)->ah_curchan;

	rfilt = ar5212GetRxFilter(ah);
	if ((ahp->ah_procPhyErr & (HAL_RADAR_EN | HAL_AR_EN)) &&
	    (AH_PRIVATE(ah)->ah_opmode == HAL_M_HOSTAP)) {
		if (chan->privFlags & CHANNEL_DFS) {
			/*
			 * Disable radar detection in case we need to setup
			 * a new channel state and radars are somehow being
			 * reported. Avoids locking problem.
			 */

			ar5212RadarDisable(ah);
			ar5212ResetAllDelayLines(ah);
			
			rs = ar5212GetRadarChanState(ah, &index);
			if (rs != AH_NULL) {
				if (index != ahp->ah_curchanRadIndex)
					ar5212ResetAllDelayLines(ah);
				ahp->ah_curchanRadIndex = (int16_t) index;
				val = 0;
				val |= SM(rs->rs_firpwr, AR_PHY_RADAR_0_FIRPWR);
				val |= SM(rs->rs_radarRssi, AR_PHY_RADAR_0_RRSSI);
				val |= SM(rs->rs_height, AR_PHY_RADAR_0_HEIGHT);
				val |= SM(rs->rs_pulseRssi, AR_PHY_RADAR_0_PRSSI);
				val |= SM(rs->rs_inband, AR_PHY_RADAR_0_INBAND);

				rfilt |= HAL_RX_FILTER_PHYRADAR;
				ar5212SetRxFilter(ah,rfilt);
				OS_REG_WRITE(ah, AR_PHY_RADAR_0, val|AR_PHY_RADAR_0_ENA);
			} else
				HALDEBUG(ah, "%s: No more radar states left\n", __func__);
		} else {
			if (!(chan->channelFlags & CHANNEL_2GHZ)) {
				/* Disable Radar if not 2GHZ channel and not DFS */
				rfilt &= ~HAL_RX_FILTER_PHYRADAR;
				ar5212SetRxFilter(ah,rfilt);
			}
		}
	} else {
		/* Disable Radar if RADAR or AR not enabled */
		rfilt &= ~HAL_RX_FILTER_PHYRADAR;
		ar5212SetRxFilter(ah,rfilt);
	}
}

/*
 * Disable Radar if AR channel
 */

void
ar5212ArDisable(struct ath_hal *ah)
{
	u_int32_t rfilt;
	HAL_CHANNEL_INTERNAL *chan=AH_PRIVATE(ah)->ah_curchan;

	if ((chan->channelFlags & CHANNEL_108G) == CHANNEL_108G) {
		rfilt = ar5212GetRxFilter(ah);
		rfilt &= ~HAL_RX_FILTER_PHYERR;
		ar5212SetRxFilter(ah,rfilt);
		OS_REG_WRITE(ah, AR_PHY_RADAR_0,
			     OS_REG_READ(ah, AR_PHY_RADAR_0) & ~AR_PHY_RADAR_0_ENA);
		ar5212ResetAR(ah);
		ar5212ResetArQ(ah);
	}
}

void
ar5212ProcessRadar(struct ath_hal *ah, struct ath_desc *ds, u_int64_t fullTsf)
{
	struct ath_hal_5212 *ahp=AH5212(ah);

	HAL_CHANNEL_INTERNAL *chan=AH_PRIVATE(ah)->ah_curchan;

	if (((chan->channelFlags & CHANNEL_108G) == CHANNEL_108G) &&
	    (AH_PRIVATE(ah)->ah_opmode == HAL_M_HOSTAP)) {
		ar5212QueueRadarEvent(ah, ds, ahp->ah_arq, &ahp->ah_arqInfo, fullTsf);
	}
	if (chan->privFlags & CHANNEL_DFS) {
		ar5212QueueRadarEvent(ah, ds, ahp->ah_radarq, &ahp->ah_radarqInfo, fullTsf);
	}
}

/*
 * Add the radar event to the appropriate linked list which represents
 * the pulses which are currently in the tapped delay line with time
 * markers representing their current position in the filter.
 * pulseIndex is the index of the pulse which we are trying to match,
 * so it is the index of the pulse whose filter we are convolving.
 */

static void
ar5212AddPulseToDelayLine(struct ath_hal *ah, struct ar5212RadarDelayLine *dl, 
			  struct ar5212RadarEvent *re, u_int32_t filterLen,
			  u_int32_t shift)
{
	u_int32_t index,n,numElems;

	/* Circular buffer of size 2^n */
	index = (dl->dl_lastElem + 1) & HAL_MAX_DL_MASK;
	if ((dl->dl_numElems+1) == HAL_MAX_DL_SIZE)
		dl->dl_firstElem = (dl->dl_firstElem + 1) & HAL_MAX_DL_MASK;
	else
		dl->dl_numElems++;
	dl->dl_lastElem = index;
	dl->dl_elems[index].de_time = 0;
	dl->dl_elems[index].de_dur = re->re_dur;

	index = (dl->dl_lastElem-1) & HAL_MAX_DL_MASK;
	for (n=0;n<dl->dl_numElems-1; n++) {
		dl->dl_elems[index].de_time += shift;
		if (dl->dl_elems[index].de_time > filterLen) {
			dl->dl_firstElem = (index+1)&HAL_MAX_DL_MASK;
			dl->dl_numElems = n+1;
		}
		index = (index-1)&HAL_MAX_DL_MASK;
	}
	HALDEBUG(ah,"dl firstElem = %d  lastElem = %d\n",dl->dl_firstElem, dl->dl_lastElem);
}

/*
 * Shift and form the inner product at each shift between the filter
 * and delay line which represents the input square waves.  The
 * hamming weight of the inner product (xor of each bit position) is
 * compared with the threshold for the filter to determine if we find
 * a radar.  The value "bit" is shifted into the tapped delay line at
 * each shift.  The implementation walks thru each linked list of
 * pulses considers the resulting intersection of times and the state
 * of each of the constituent pulses to determine the hamming weight
 * contribution for that segment of time.  Since we expect the number
 * of pulses in the delay line and filter to be significantly smaller
 * than the number of usecs needed to cover an entire radar burst,
 * this should be faster.
 */

#define _min(a, b) (((a) < (b)) ? (a) : (b))

static HAL_BOOL
ar5212RadarInnerProduct(struct ar5212RadarFilter *rf,
			struct ar5212RadarDelayLine *dl, u_int32_t shift,
			u_int32_t bit)
{
	u_int32_t fstart, fend, pstart, pend,s,t,fevent,pevent;
	struct ar5212RadarDelayElem *fde,*pde,de;
	HAL_BOOL found=AH_FALSE;
	u_int32_t fstate=0,pstate=0;
	int32_t maxSum,minSum,sum;
	int delayIndex,filterIndex;
	u_int32_t overlap, maxOverlap, totOverlap;

	minSum = 0x7fffffff;
	maxSum = -minSum;
	if (dl->dl_elems == AH_NULL)
		return AH_FALSE;
	sum = 0;
	for (t=0,s=1; (s<=shift)&&(!found); s++) {
		sum=0;
		filterIndex = 0;
		fde = &rf->rf_pulses[filterIndex];
		if (bit) {
			de.de_time = -s;
			de.de_dur = s;
			pde = &de;
			delayIndex = -1;
		} else {
			delayIndex = dl->dl_lastElem;
			pde = &dl->dl_elems[delayIndex];
		}
		fstate = pstate = 0;
		fstart = fde->de_time;
		fend = fstart+fde->de_dur;
		pstart = pde->de_time+s;
		pend = pstart+pde->de_dur;
		pevent = pstart;
		fevent = fstart;
		maxOverlap = 0;
		t = 0;
		while (dl->dl_numElems > 0) {
			if (fevent < pevent) {
				/* 
				 * A filter event is the triggering event
				 * If pulse and filter are both "1", handle the
				 * overlap case, otw, do the original algorithm
				 */
				if ((pstate == 1) && (fstate == 1)) {
					/* Both fstate and pstate are 1 */
					totOverlap = (fevent-t);
					overlap = _min(totOverlap,rf->rf_pulseDur);
					/*
					 * Count the nonoverlap part as if the filter
					 * was zero over that interval
					 */
					sum += totOverlap-overlap;
					if (overlap > maxOverlap) {
						/* Remove previous max overlap */
						sum -= 2*maxOverlap;
						sum += overlap;
						maxOverlap = overlap;
					} else {
						/* The pulse was not an overlap, so subtract it */
						sum -= overlap;
					}
				} else {
					if (fstate == 1) {
						/*
						 * pstate must be zero to hit this case
						 * We must be @ end of filter pulse, so see
						 * if we've detected ANY pulses. If not, then
						 * subtract out the duration of a filter
						 */
						sum += ((int) (fevent-t));
						if (maxOverlap == 0)
							sum -= 2*rf->rf_pulseDur;
					} else {
						/* 
						 * pstate == 1, fstate == 0 or
						 * pstate == 0, fstate == 0 
						 */ 
						sum += ((int) (fevent-t))*(((int)(2*(!(fstate ^ pstate))))-1);
					}
				}
				fstate = (fstate+1)&0x01;
				t = fevent;
				if (fevent == fend) {
					/* Reset max overlap b/c we move on to the next filter pulse */
					maxOverlap = 0;
					filterIndex++;
					if (filterIndex < rf->rf_numPulses) {
						fde = &rf->rf_pulses[filterIndex];
						fstart = fde->de_time;
						fend = fstart+fde->de_dur;
						fevent = fstart;
					} else
						break;
				} else
					fevent = fend;
			} else if (pevent < fevent) {
				/* A pulse input event is the triggering event */
				if ((fstate == 1) && (pstate == 1)) {
					/* 
					 * The triggering event is an input pulse falling edge 
					 * and the filter was also a "1" over the period
					 */
					totOverlap = (pevent-t);
					overlap = _min(totOverlap, rf->rf_pulseDur);
					/* Treat the filter as zeros except for a match pulse duration */
					sum += totOverlap - overlap;
					if (overlap > maxOverlap) {
						/* Remove previous max overlap */
						sum -= 2*maxOverlap;
						sum += overlap;
						maxOverlap = overlap;
					} else {
						/* Pulse was not an overalp, so subtract it */
						sum -= overlap;
					}
				} else {
					if (fstate == 1) {
						/*
						 * pstate must be zero to hit this case
						 * Triggering event was a input rising edge
						 */
						sum += ((int) (pevent-t));
					} else {
						/* 
						 * pstate == 1, fstate == 0 or
						 * pstate == 0, fstate == 0 
						 */ 
						sum += ((int) (fevent-t))*(((int)(2*(!(fstate ^ pstate))))-1);
					}
				}
				pstate = (pstate+1)&0x01;
				t = pevent;
				if (pevent == pend) {
					if (delayIndex == dl->dl_firstElem)
						break;
					if (delayIndex == -1)
						delayIndex = dl->dl_lastElem;
					else
						delayIndex = (int32_t) ((((u_int32_t) delayIndex) - 1) & HAL_MAX_DL_MASK);
					pde = &dl->dl_elems[delayIndex];
					pstart = pde->de_time+s;
					pend = pstart+pde->de_dur;
					pevent = pstart;
				} else
					pevent = pend;
			} else {
				/* Both events occur at the same time */
				if ((fstate == 1) && (pstate == 1)) {
					totOverlap = (pevent-t);
					overlap = _min(totOverlap, rf->rf_pulseDur);
					sum += totOverlap - overlap;
					if (overlap > maxOverlap) {
						/* Remove previous max overlap */
						sum -= 2*maxOverlap;
						sum += overlap;
						maxOverlap = overlap;
					} else 
						sum -= overlap;
				} else {
					if (fstate == 1) {
						/*
						 * filter event is a falling edge, pulse event is a rising
						 * edge.
						 */
						sum += ((int) (fevent-t));
						if (maxOverlap == 0)
							sum -= 2*rf->rf_pulseDur;
					} else 
						sum += ((int) (pevent-t))*(((int)(2*(!(fstate ^ pstate))))-1);
				}
				pstate = (pstate+1)&0x01;
				fstate = (fstate+1)&0x01;
				t = pevent;
				if (fevent == fend) {
					filterIndex++;
					if (filterIndex < rf->rf_numPulses) {
						/* Reset max overlap */
						maxOverlap = 0;
						fde = &rf->rf_pulses[filterIndex];
						fstart = fde->de_time;
						fend = fstart+fde->de_dur;
						fevent = fstart;
					} else
						break;
				} else
					fevent = fend;
				if (pevent == pend) {
					if (delayIndex == dl->dl_firstElem)
						break;
					if (delayIndex == -1)
						delayIndex = dl->dl_lastElem;
					else 
						delayIndex = (int32_t) ((((u_int32_t) delayIndex) - 1) & HAL_MAX_DL_MASK);
					pde = &dl->dl_elems[delayIndex];
					pstart = pde->de_time+s;
					pend = pstart+pde->de_dur;
					pevent = pstart;
				} else
					pevent = pend;
			}				
		}
		if ((pevent == pend) && (delayIndex == dl->dl_firstElem)) {
			pstate = 0;
			if (fevent == fend) {
				sum += (fevent-t);
				if (maxOverlap == 0)
					sum -= 2*rf->rf_pulseDur;
				t = fend;
				filterIndex++;
			}
			while (filterIndex < rf->rf_numPulses) {
				fde = &rf->rf_pulses[filterIndex];
				fstart = fde->de_time;
				fend = fstart + fde->de_dur;
				sum += (fstart-t)+(fend-fstart+1)-2*rf->rf_pulseDur;
				t = fend;
				filterIndex++;
			}
		}
		if (sum > maxSum)
			maxSum = sum;
		if (sum < minSum)
			minSum = sum;
		if (sum > ((int32_t) rf->rf_threshold))
			found = AH_TRUE;
	}
#if (AH_RADAR_CALIBRATE == 3)
	ath_hal_printf(AH_NULL,"maxSum = %d minSum = %d Threshold = %u\n",maxSum, minSum,
		       rf->rf_threshold);
#endif
	return (found);
}

static void
ar5212AddChanToNol(struct ath_hal *ah, HAL_CHANNEL_INTERNAL *ichan)
{
	struct ath_hal_5212 *ahp=AH5212(ah);
	struct ar5212RadarNolElem *nol, *elem, **next;
	u_int32_t found=AH_FALSE;
	u_int64_t tsf;

	nol = ahp->ah_radarNol;
	next = &(ahp->ah_radarNol);
	tsf = ar5212GetTsf64(ah);
	tsf += HAL_RADAR_NOL_TIME;
	while (nol != AH_NULL) {
		if (nol->nol_chan == ichan) {
			nol->nol_tsfFree = tsf;
			return;
		}
		next = &(nol->nol_next);
		nol = nol->nol_next;
	}
	elem = ath_hal_malloc(sizeof(struct ar5212RadarNolElem));
	if (elem == AH_NULL) {
		HALDEBUG(ah,"%s: failed to allocate memory for nol entry\n",
			 __func__);
		return;
	}
	OS_MEMZERO(elem, sizeof(struct ar5212RadarNolElem));
	elem->nol_chan = ichan;
	elem->nol_tsfFree = tsf;
	elem->nol_next = AH_NULL;
	*next = elem;
}

u_int32_t
ar5212CheckRadarNol(struct ath_hal *ah, HAL_CHANNEL *chans, u_int32_t nchans)
{
	struct ath_hal_5212 *ahp=AH5212(ah);
	struct ar5212RadarNolElem *nol,**prevNext;
	u_int64_t tsf;
	u_int32_t freeIndex=0;
	
	tsf = ar5212GetTsf64(ah);
	prevNext = &(ahp->ah_radarNol);
	nol = ahp->ah_radarNol;
	while (nol != AH_NULL) {
		if (tsf >= nol->nol_tsfFree) {
			*prevNext = nol->nol_next;
			nol->nol_chan->privFlags &= ~CHANNEL_INTERFERENCE;
			if (freeIndex < nchans) {
				chans[freeIndex].channel = nol->nol_chan->channel;
				chans[freeIndex].channelFlags = nol->nol_chan->channelFlags;
				chans[freeIndex].privFlags = nol->nol_chan->privFlags;
				freeIndex++;
			}
			ath_hal_free(nol);
			nol = *prevNext;
		} else {
			prevNext = &(nol->nol_next);
			nol = nol->nol_next;
		}
	}
	return freeIndex;
}

HAL_BOOL
ar5212ProcessRadarEvent(struct ath_hal *ah, HAL_CHANNEL *chan)
{
	struct ath_hal_5212 *ahp=AH5212(ah);
	struct ar5212RadarEvent re;
	struct ar5212RadarState *rs;
	struct ar5212RadarFilterType *ft;
	struct ar5212RadarFilter *rf;
	HAL_BOOL isFlush=AH_FALSE;
	HAL_BOOL found, retVal=AH_FALSE;
	HAL_CHANNEL_INTERNAL *ichan;
	int64_t	deltaT;
	u_int32_t totShift;
	u_int64_t deltaFullTs,thisTs;
	int p;

	while ((ar5212DeQueueRadarEvent(ahp->ah_radarq, &ahp->ah_radarqInfo, &re, &isFlush))&&
	       (!retVal)) {
		found = AH_FALSE;
		if (re.re_chanIndex < HAL_NUM_RADAR_STATES)
			rs = &ahp->ah_radar[re.re_chanIndex];
		else
			continue;
		if (rs->rs_chan->privFlags & CHANNEL_INTERFERENCE)
			continue;
		if (isFlush)
			ar5212ResetRadarChannel(ah, rs);
		if (ahp->ah_rinfo.rn_lastFullTs == 0) {
			/*
			 * Either not started, or 64-bit rollover exactly to zero
			 * Just prepend zeros to the 15-bit ts
			 */
			ahp->ah_rinfo.rn_tsPrefix = 0;
			thisTs = (u_int64_t) re.re_ts;
		} else {
			if (re.re_ts <= ahp->ah_rinfo.rn_lastTs) {
				ahp->ah_rinfo.rn_tsPrefix += 
					(((u_int64_t) 1) << HAL_RADAR_TSSHIFT);
				/* Now, see if it's been more than 1 wrap */
				deltaFullTs = re.re_fullts - ahp->ah_rinfo.rn_lastFullTs;
				if (deltaFullTs > 
				    ((u_int64_t)((HAL_RADAR_TSMASK - ahp->ah_rinfo.rn_lastTs) + 1 + re.re_ts)))
					deltaFullTs -= (HAL_RADAR_TSMASK - ahp->ah_rinfo.rn_lastTs) + 1 + re.re_ts;
				deltaFullTs = deltaFullTs >> HAL_RADAR_TSSHIFT;
				if (deltaFullTs > 1) {
					ahp->ah_rinfo.rn_tsPrefix += 
						((deltaFullTs - 1) << HAL_RADAR_TSSHIFT);
				}
			} else {
				deltaFullTs = re.re_fullts - ahp->ah_rinfo.rn_lastFullTs;
				if (deltaFullTs > (u_int64_t) HAL_RADAR_TSMASK) {
					deltaFullTs = deltaFullTs >> HAL_RADAR_TSSHIFT;
					ahp->ah_rinfo.rn_tsPrefix += 
						((deltaFullTs - 1) << HAL_RADAR_TSSHIFT);
				}
			}
			thisTs = ahp->ah_rinfo.rn_tsPrefix | ((u_int64_t) re.re_ts);
		}
		ahp->ah_rinfo.rn_lastFullTs = re.re_fullts;
		ahp->ah_rinfo.rn_lastTs = re.re_ts;
		if (re.re_dur == 0)
			re.re_dur = 1;
		else {
			/* Convert 0.8us durations to TSF ticks (usecs) */
			re.re_dur = _round((int32_t)(80*re.re_dur));
		}

		thisTs -= re.re_dur;
		
		ft=AH_NULL;
		for (p=0;(p < ahp->ah_rinfo.rn_numRadars); p++) {
			if ((re.re_dur >= ahp->ah_radarf[p].ft_minDur) &&
			    (re.re_dur <= ahp->ah_radarf[p].ft_maxDur)) {
				ft = &(ahp->ah_radarf[p]);
				break;
			}
		}
		if (ft == AH_NULL)
			continue;
		if (re.re_rssi < ft->ft_rssiThresh) {
			HALDEBUG(ah,"Rejecting on rssi\n");
			continue;
		}
#if (AH_RADAR_CALIBRATE >= 2)		
		ath_hal_printf(ah,"  ** RD (%d): ts %x dur %u rssi %u\n",
			       rs->rs_chan->channel,
			       re.re_ts, re.re_dur, re.re_rssi);
#endif
		deltaT = thisTs - ((u_int64_t) re.re_dur) - ft->ft_dl.dl_lastTs;
#if (AH_RADAR_CALIBRATE == 3)
		ath_hal_printf(ah,"deltaT = %lld (ts: 0x%llx)  (lastTs: 0x%llx)\n",deltaT,thisTs,
			       ft->ft_dl.dl_lastTs);
#endif
		if (deltaT < 0)
			deltaT = (int64_t) ((HAL_RADAR_TSF_WRAP - ft->ft_dl.dl_lastTs) + thisTs +1);

		for (p=0, found = AH_FALSE; (p<ft->ft_numFilters) && (!found); p++) {
			rf = &(ft->ft_filters[p]);
			if (deltaT >= ((u_int64_t) rf->rf_filterLen)) {
				found = ar5212RadarInnerProduct(rf,&ft->ft_dl,
								rf->rf_filterLen, 0);
			} else {
				found = ar5212RadarInnerProduct(rf,&ft->ft_dl,
								(u_int32_t) deltaT, 0);
			}

			/* 
			 * If deltaT-filterLen > 0, the remainder of the zero shifts don't do
			 * anything and the delay line will still be full of zeros, so we can
			 * just skip that part.
			 * If a radar was detected in the zero shift part above, we don't even
			 * need to shift in any of the pulse.  Just add the pulse to the list
			 * of pulses "in" a delay line and adjust the tsf for the delay line.
			 */
			if (!found) {
				found = ar5212RadarInnerProduct(rf,&ft->ft_dl,
								re.re_dur, 1);
			}
		}
		if (deltaT >= ((u_int64_t) rf->rf_filterLen))
			totShift = rf->rf_filterLen+1;
		else
			totShift = ((u_int32_t) deltaT) + re.re_dur;
		ar5212AddPulseToDelayLine(ah, &ft->ft_dl, &re, ft->ft_maxFilterLen,
					  totShift);
#if (AH_RADAR_CALIBRATE == 3)
		printDelayLine(&ft->ft_dl);
#endif
		ft->ft_dl.dl_lastTs = thisTs;
		if (found) {
			ichan = rs->rs_chan;
			if (ahp->ah_rinfo.rn_useNol)
				ichan->privFlags |= CHANNEL_INTERFERENCE;
			chan->channel = ichan->channel;
			chan->channelFlags = ichan->channelFlags;
			chan->privFlags  = ichan->privFlags;
			chan->maxRegTxPower = ichan->maxRegTxPower;
			if (ahp->ah_rinfo.rn_useNol)
				ar5212AddChanToNol(ah, ichan);
		}
		retVal |= found;
#if (AH_RADAR_CALIBRATE >= 2)
		/* XXX - DEBUG ONLY */
		if (found) {
			ath_hal_printf(ah,"Found on channel minDur = %d\n",ft->ft_minDur);
		}
#endif
	}
	if (retVal == AH_TRUE) {
		ar5212RadarDisable(ah);
		ar5212ResetRadarQ(ah);
		ar5212ResetAllDelayLines(ah);
		/* XXX Should we really enable again? Maybe not... */
		ar5212RadarEnable(ah);
	}
	return retVal;
}

#define UPDATE_TOP_THREE_PEAKS(_histo, _peakPtrList, _currWidth) { \
	if ((_histo)[(_peakPtrList)[0]] < (_histo)[(_currWidth)]) {	\
		(_peakPtrList)[2] = (_currWidth != (_peakPtrList)[1]) ?	\
					(_peakPtrList)[1] : (_peakPtrList)[2];  \
		(_peakPtrList)[1] = (_peakPtrList)[0]; \
		(_peakPtrList)[0] = (_currWidth); \
	} else if ((_currWidth != (_peakPtrList)[0])	\
			&& ((_histo)[(_peakPtrList)[1]] < (_histo)[(_currWidth)])) { \
		(_peakPtrList)[2] = (_peakPtrList)[1]; \
		(_peakPtrList)[1] = (_currWidth);      \
	} else if ((_currWidth != (_peakPtrList)[1])   \
			&& (_currWidth != (_peakPtrList)[0])  \
			&& ((_histo)[(_peakPtrList)[2]] < (_histo)[(_currWidth)])) { \
		(_peakPtrList)[2] = (_currWidth);  \
	} \
}                                                                           

/*
 * This routine builds the histogram based on radar duration and does pattern matching
 * on incoming radars to determine if neighboring traffic is present.
 */

void
ar5212ProcessArEvent(struct ath_hal *ah, HAL_CHANNEL *chan)
{
	struct ath_hal_5212 *ahp=AH5212(ah);
	struct ar5212ArState *ar;
	u_int32_t sumPeak=0,numPeaks,rssi,width,origRegionSum=0, i;
	u_int16_t thisTimeStamp;
	struct ar5212RadarEvent re;
	HAL_BOOL isFlush=AH_FALSE;


	ar = (struct ar5212ArState *) &(ahp->ah_ar);
	while (ar5212DeQueueRadarEvent(ahp->ah_arq, &ahp->ah_arqInfo, &re, &isFlush)) {
		if (isFlush) {
			ar5212ResetAR(ah);
		}
		thisTimeStamp = re.re_ts;
		rssi = re.re_rssi;
		width = re.re_dur;

		/* determine if current radar is an extension of previous radar */
		if (ar->ar_prevWidth == 255) {
			/* tag on previous width for consideraion of low data rate ACKs */
			ar->ar_prevWidth += width;
			width = (width == 255) ? 255 : ar->ar_prevWidth;
		} else if ((width == 255) &&
			   (ar->ar_prevWidth == 510 ||
			    ar->ar_prevWidth == 765 ||
			    ar->ar_prevWidth == 1020)) {
			/* Aggregate up to 5 consecuate max radar widths
			 * to consider 11Mbps long preamble 1500-byte pkts
			 */
			ar->ar_prevWidth += width;
		} else if (ar->ar_prevWidth == 1275 && width != 255) {
			/* Found 5th consecute maxed out radar, reset history */
			width += ar->ar_prevWidth;
			ar->ar_prevWidth = 0;
		} else if (ar->ar_prevWidth > 255) {
			/* Ignore if there are less than 5 consecutive maxed out radars */
			ar->ar_prevWidth = width;
			width = 255;
		} else {
			ar->ar_prevWidth = width;
		}
		/* For ignoring noises with radar duration in ranges of 3-30: AP4x */
		if ((width >= 257 && width <= 278) ||	/* Region 7 - 5.5Mbps (long pre) ACK = 270 = 216 us */
		    (width >= 295 && width <= 325) ||	/* Region 8 - 2Mbps (long pre) ACKC = 320 = 256us */
		    (width >= 1280 && width <= 1300)) {
			u_int16_t wrapAroundAdj=0;
			u_int16_t base = (width >= 1280) ? 1275 : 255;
			if (thisTimeStamp < ar->ar_prevTimeStamp) {
				wrapAroundAdj = 32768;
			}
			if ((thisTimeStamp + wrapAroundAdj - ar->ar_prevTimeStamp) !=
			    (width - base)) {
				width = 1;
			}
		}
		if (width <= 10)
			continue;
		
		/*
		 * Overloading the width=2 in: Store a count of radars w/max duration
		 * and high RSSI (not noise)
		 */
		if ((width == 255) && (rssi > HAL_AR_RSSI_THRESH_STRONG_PKTS))
			width = 2;
		
		/*
		 * Overloading the width=3 bin:
		 *   Double and store a count of rdars of durtaion that matches 11Mbps (long preamble)
		 *   TCP ACKs or 1500-byte data packets
		 */
		if ((width >= 1280 && width <= 1300) ||
		    (width >= 318 && width <= 325)) {
			width = 3;
			ar->ar_phyErrCount[3] += 2;
			ar->ar_ackSum += 2;
		}
		
		/* build histogram of radar duration */
		if (width > 0 && width <= 510)
			ar->ar_phyErrCount[width]++;
		else
			/* invalid radar width, throw it away */
			continue;
		/* Received radar of interest (i.e., signature match), proceed to check if
		 * there is enough neighboring traffic to drop out of Turbo 
		 */
		if ((width >= 33 && width <= 38) ||          /* Region 0: 24Mbps ACK = 35 = 28us */
		    (width >= 39 && width <= 44) ||          /* Region 1: 12Mbps ACK = 40 = 32us */
		    (width >= 53 && width <= 58) ||          /* Region 2:  6Mbps ACK = 55 = 44us */
		    (width >= 126 && width <= 140) ||        /* Region 3: 11Mbps ACK = 135 = 108us */
		    (width >= 141 && width <= 160) ||        /* Region 4: 5.5Mbps ACK = 150 = 120us */
		    (width >= 189 && width <= 210) ||        /* Region 5:  2Mbps ACK = 200 = 160us */
		    (width >= 360 && width <= 380) ||        /* Region 6   1Mbps ACK = 400 = 320us */
		    (width >= 257 && width <= 270) ||        /* Region 7   5.5Mbps (Long Pre) ACK = 270 = 216us */
		    (width >= 295 && width <= 302) ||        /* Region 8   2Mbps (Long Pre) ACK = 320 = 256us */
		    /* Ignoring Region 9 due to overlap with 255 which is same as board noise */
		    /* Region 9  11Mbps (Long Pre) ACK = 255 = 204us */            
		    (width == 3)) {
			ar->ar_ackSum++;
			
			/* double the count for strong radars that match one of the ACK signatures */
			if (rssi > HAL_AR_RSSI_DOUBLE_THRESHOLD) {
				ar->ar_phyErrCount[width]++;
				ar->ar_ackSum++;
			}
			UPDATE_TOP_THREE_PEAKS(ar->ar_phyErrCount,
					       ar->ar_peakList, width);
			/* sum the counts of these peaks */
			numPeaks = HAL_MAX_NUM_PEAKS;
			origRegionSum = ar->ar_ackSum;
			for (i=0; i<= HAL_MAX_NUM_PEAKS; i++) {
				if (ar->ar_peakList[i] > 0) {
					if ((i==0) &&
					    (ar->ar_peakList[i] == 3) &&
					    (ar->ar_phyErrCount[3] <
					     ar->ar_phyErrCount[2]) &&
					    (ar->ar_phyErrCount[3] > 6)) {
						/*
						 * If the top peak is one that
						 * maches the 11Mbps long
						 * preamble TCP Ack/1500-byte
						 * data, include the count for
						 * radars that hav emax
						 * duration and high rssi
						 * (width = 2) to boost the
						 * sum for the PAR test that
						 * follows */
						sumPeak += (ar->ar_phyErrCount[2]
							    + ar->ar_phyErrCount[3]);
						ar->ar_ackSum += (ar->ar_phyErrCount[2]
								  + ar->ar_phyErrCount[3]);
					} else {
						sumPeak += ar->ar_phyErrCount[ar->ar_peakList[i]];
					}
				} else 
					numPeaks--;
			}
			
			/*
			 * If sum of patterns matches exceeds packet threshold,
			 * perform comparison between peak-to-avg ratio against parThreshold
			 */
			if ((ar->ar_ackSum > ar->ar_packetThreshold) &&
			    ((sumPeak * HAL_AR_REGION_WIDTH) > (ar->ar_parThreshold * numPeaks *
								ar->ar_ackSum))) {
				/* neighboring traffic detected, get out of Turbo */
				chan->privFlags |= CHANNEL_INTERFERENCE;
				
				OS_MEMZERO(ar->ar_peakList, sizeof(ar->ar_peakList));
				ar->ar_ackSum = 0;
				OS_MEMZERO(ar->ar_phyErrCount, sizeof(ar->ar_phyErrCount));
			} else {
				/*
				 * reset sum of matches to discount the count of
				 * strong radars with max duration
				 */
				ar->ar_ackSum = origRegionSum;
			}
		}
		ar->ar_prevTimeStamp = thisTimeStamp;
	}
}

#ifdef AH_PRIVATE_DIAG
HAL_BOOL
ar5212SetRadarThresholds(struct ath_hal *ah, const u_int32_t threshType,
			 const u_int32_t value)
{
	int16_t chanIndex;
	struct ar5212RadarState *rs;
	u_int32_t val;
	struct ath_hal_5212 *ahp=AH5212(ah);


	chanIndex = ahp->ah_curchanRadIndex;
	if ((chanIndex <0) || (chanIndex >= HAL_NUM_RADAR_STATES))
		return AH_FALSE;
	rs = &(ahp->ah_radar[chanIndex]);
	switch (threshType) {
	case HAL_RADAR_PARAM_FIRPWR:
		rs->rs_firpwr = (int32_t) value;
		break;
	case HAL_RADAR_PARAM_RRSSI:
		rs->rs_radarRssi = value;
		break;
	case HAL_RADAR_PARAM_HEIGHT:
		rs->rs_height = value;
		break;
	case HAL_RADAR_PARAM_PRSSI:
		rs->rs_pulseRssi = value;
		break;
	case HAL_RADAR_PARAM_INBAND:
		rs->rs_inband = value;
		break;
	}
	val = 0;
	val |= SM(rs->rs_firpwr, AR_PHY_RADAR_0_FIRPWR);
	val |= SM(rs->rs_radarRssi, AR_PHY_RADAR_0_RRSSI);
	val |= SM(rs->rs_height, AR_PHY_RADAR_0_HEIGHT);
	val |= SM(rs->rs_pulseRssi, AR_PHY_RADAR_0_PRSSI);
	val |= SM(rs->rs_inband, AR_PHY_RADAR_0_INBAND);

	OS_REG_WRITE(ah, AR_PHY_RADAR_0, val|AR_PHY_RADAR_0_ENA);
	return AH_TRUE;
}

HAL_BOOL
ar5212GetRadarThresholds(struct ath_hal *ah, struct ar5212RadarState *thresh)
{
	u_int32_t val,temp;

	val = OS_REG_READ(ah, AR_PHY_RADAR_0);

	temp = MS(val,AR_PHY_RADAR_0_FIRPWR);
	temp |= 0xFFFFFF80;
	thresh->rs_firpwr = temp;
	thresh->rs_radarRssi = MS(val, AR_PHY_RADAR_0_RRSSI);
	thresh->rs_height =  MS(val, AR_PHY_RADAR_0_HEIGHT);
	thresh->rs_pulseRssi = MS(val, AR_PHY_RADAR_0_PRSSI);
	thresh->rs_inband = MS(val, AR_PHY_RADAR_0_INBAND);
	return AH_TRUE;
}
#endif /* AH_PRIVATE_DIAG */

#endif /* AH_SUPPORT_AR5212 */
