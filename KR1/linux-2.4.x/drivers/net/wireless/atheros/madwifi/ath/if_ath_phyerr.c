/*
 * Copyright (c) 2002-2004 Atheros Communications, Inc.
 * All rights reserved.
 *
 */

#include <linux/config.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/random.h>
#include <linux/delay.h>
#include <linux/cache.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/if_arp.h>

#include <asm/uaccess.h>

#include "if_ethersubr.h"		/* for ETHER_IS_MULTICAST */
#include "if_media.h"
#include "if_llc.h"
#include "net80211/if_athproto.h"
#include <net80211/ieee80211_var.h>

#define	AR_DEBUG
#include "if_athvar.h"
#include "ah_desc.h"
#include "ah_devid.h"			/* XXX to identify IBM cards */

#ifdef ATH_PCI		/* PCI BUS */
#include "if_ath_pci.h"
#endif			/* PCI BUS */
#ifdef ATH_AHB		/* AHB BUS */
#include "if_ath_ahb.h"
#endif			/* AHB BUS */
#include "ah.h"

#define	HAL_CAP_RADAR		0		/* NB: Must match the ah_internal.h def'n */
#define	HAL_CAP_RADAR_FIRPWR	1		/* Radar parameter firpwr threshold */
#define	HAL_CAP_RADAR_HEIGHT	2		/* Radar parameter height threshold */
#define	HAL_CAP_RADAR_RRSSI	3		/* Radar parameter rssi thershold */
#define	HAL_CAP_RADAR_PRSSI	4		/* Radar parameter pulse rssi threshold */
#define	HAL_CAP_RADAR_INBAND	5		/* Radar parameter inband threshold */

#define	ATH_RADAR_AR_EN		0x00000001	/* Enable radar detect for AR */
#define	ATH_RADAR_EN		0x00000002	/* Enable radar detect in 11a */

#define ATH_RADAR_FIRPWR -45
#define ATH_RADAR_RRSSI 14
#define ATH_RADAR_HEIGHT 20
#define ATH_RADAR_PRSSI 24
#define ATH_RADAR_INBAND 6

#define	ATH_AR_RADAR_RSSI_THR		5	/* in dB */
#define	ATH_AR_RADAR_RESET_INT		1	/* in secs */
#define	ATH_AR_RADAR_MAX_HISTORY	500
#define	ATH_AR_REGION_WIDTH		128
#define	ATH_AR_RSSI_THRESH_STRONG_PKTS	17	/* in dB */
#define	ATH_AR_RSSI_DOUBLE_THRESHOLD	15	/* in dB */
#define	ATH_AR_MAX_NUM_ACK_REGIONS	9
#define	ATH_AR_ACK_DETECT_PAR_THRESH	20
#define	ATH_AR_PKT_COUNT_THRESH		20

#define	ATH_MAX_ACK_RADAR_DUR		511
#define ATH_MAX_NUM_PEAKS		3

struct ath_AR_state {
	u_int16_t	ar_prevTimeStamp;
	u_int32_t	ar_prevWidth;
	u_int32_t	ar_phyErrCount[ATH_MAX_ACK_RADAR_DUR];
	u_int32_t	ar_ackSum;
	u_int16_t	ar_peakList[ATH_MAX_NUM_PEAKS];
	u_int32_t	ar_packetThreshold;	/* Thresh to determine traffic load */
	u_int32_t	ar_parThreshold;	/* Thresh to determine peak */
};

struct ath_radar_state {
	struct ieee80211_channel c;		/* Channel info */
	u_int32_t	rad_numRadarEvents;	/* Number of radar events */
	int32_t		rad_firpwr;		/* Thresh to check radar sig is gone */ 
	u_int32_t	rad_radarRssi;		/* Thresh to start radar det (dB) */
	u_int32_t	rad_height;		/* Thresh for pulse height (dB)*/
	u_int32_t	rad_pulseRssi;		/* Thresh to check if pulse is gone (dB) */
	u_int32_t	rad_inband;		/* Thresh to check if pusle is inband (0.5 dB) */
};

#define NUM_RADAR_STATES	64

struct ath_phyerr_state {
	struct ath_AR_state pe_arState;		/* AR detection state */
	struct ath_radar_state pe_radarState[NUM_RADAR_STATES];	/* per channel radar state */
	struct ath_radar_state *pe_curRadar;	/* cached current radar state */
};

static void ath_radar_event(struct net_device *dev, const struct ath_desc *ds,
			    struct sk_buff *skb);
static void ath_AR_event(struct net_device *dev, const struct ath_desc *ds,
			 struct sk_buff *skb);

/*
 * Finds the index in the radar state array that matches the current channel
 */

static int
get_radar_chan_index(struct ath_softc *sc)
{
#define N(a)     (sizeof(a) / sizeof(a[0]))
	struct ieee80211com *ic = &sc->sc_ic;
	struct ieee80211_channel *chan=ic->ic_ibss_chan;
	struct ath_phyerr_state *pe;
	int i;

	pe = (struct ath_phyerr_state *) sc->sc_phyerr_state;
	for (i = 0; i < N(pe->pe_radarState); i++) {
		if (pe->pe_radarState[i].c.ic_freq == chan->ic_freq)
			return i;
		if (pe->pe_radarState[i].c.ic_freq == 0) {
			pe->pe_radarState[i].c.ic_freq = chan->ic_freq;
			pe->pe_radarState[i].c.ic_freq = chan->ic_flags;
			return i;
		}
	}
	printk("No more channel states left. Using channel 0.\n");
	return 0;		/* XXX gotta return something valid */
#undef N
}

static void
ath_reset_ar(struct ath_softc *sc)
{
	struct ath_phyerr_state *pe=NULL;

	KASSERT(sc->sc_phyerr_state != NULL,
		("NULL phyerr_state"));
	pe = (struct ath_phyerr_state *) sc->sc_phyerr_state;
	
	memset(&pe->pe_arState, 0, sizeof(pe->pe_arState));
	pe->pe_arState.ar_packetThreshold = ATH_AR_PKT_COUNT_THRESH;
	pe->pe_arState.ar_parThreshold = ATH_AR_ACK_DETECT_PAR_THRESH;
}

static void
ath_reset_radar(struct ath_softc *sc)
{
	struct ath_phyerr_state *pe = NULL;

	KASSERT(sc->sc_phyerr_state != NULL,
		("NULL phyerr_state"));
	pe = (struct ath_phyerr_state *) sc->sc_phyerr_state;
	pe->pe_curRadar = NULL;
	memset(pe->pe_radarState, 0, sizeof(pe->pe_radarState));
}	

void
ath_phyerr_reset(struct ath_softc *sc)
{
	struct ieee80211com *ic = &sc->sc_ic;
	u_int32_t mode;

	if (ic->ic_opmode == IEEE80211_M_HOSTAP) {
		mode = ieee80211_chan2mode(ic, ic->ic_ibss_chan);
		if (mode == IEEE80211_MODE_TURBO_G) 
			ath_reset_ar(sc);
		if ((mode == IEEE80211_MODE_11A) ||
		    (mode == IEEE80211_MODE_TURBO_A))
			ath_reset_radar(sc);
	}

}

void
ath_phyerr_attach(struct ath_softc *sc)
{
	struct ieee80211com *ic = &sc->sc_ic;
	sc->sc_phyerr_cap = 0;

	if (ath_hal_getcapability(sc->sc_ah, HAL_CAP_PHYDIAG, HAL_CAP_RADAR, NULL) == HAL_OK) {
		sc->sc_phyerr_cap |= ATH_RADAR_EN;
		if ((ic->ic_ath_cap & (IEEE80211_ATHC_TURBOP)) &&
		    (ic->ic_ath_cap & (IEEE80211_ATHC_AR)))
			sc->sc_phyerr_cap |= ATH_RADAR_AR_EN;
	}
	sc->sc_phyerr_state = (void *) kmalloc(sizeof(struct ath_phyerr_state), GFP_KERNEL);
	ath_reset_radar(sc);
	ath_reset_ar(sc);
};

void
ath_phyerr_enable(struct ath_softc *sc, struct ieee80211_channel *chan)
{
	u_int32_t rfilt;
	struct ieee80211com *ic = &sc->sc_ic;
	u_int32_t index;
	struct ath_phyerr_state *pe;
	enum ieee80211_phymode mode;

	pe = (struct ath_phyerr_state *) sc->sc_phyerr_state;
	mode = ieee80211_chan2mode(ic, chan);
	if ((sc->sc_phyerr_cap & (ATH_RADAR_AR_EN | ATH_RADAR_EN)) &&
	    (ic->ic_opmode == IEEE80211_M_HOSTAP) &&
	    ((mode == IEEE80211_MODE_11A) ||
	     (mode == IEEE80211_MODE_TURBO_A) ||
	     (mode == IEEE80211_MODE_TURBO_G))) {
		if (mode == IEEE80211_MODE_TURBO_G) {
			/* We are in turbo G, so enable AR*/
			index = get_radar_chan_index(sc);
			pe->pe_curRadar = &pe->pe_radarState[index];
			ath_reset_ar(sc);
			pe->pe_curRadar->rad_radarRssi = ATH_AR_RADAR_RSSI_THR;
			ath_hal_setcapability(sc->sc_ah, HAL_CAP_PHYDIAG,
					      HAL_CAP_RADAR_RRSSI,
					      pe->pe_curRadar->rad_radarRssi, NULL);
		} else {
			ath_reset_radar(sc);
			index = get_radar_chan_index(sc);
			pe->pe_curRadar = &pe->pe_radarState[index];

			pe->pe_curRadar->rad_firpwr = ATH_RADAR_FIRPWR;
			ath_hal_setcapability(sc->sc_ah, HAL_CAP_PHYDIAG,
					      HAL_CAP_RADAR_FIRPWR,
					      (u_int32_t) pe->pe_curRadar->rad_firpwr, NULL);

			/* XXX - need to Set firpwr */
			pe->pe_curRadar->rad_radarRssi = ATH_RADAR_RRSSI;
			ath_hal_setcapability(sc->sc_ah, HAL_CAP_PHYDIAG,
					      HAL_CAP_RADAR_RRSSI,
					      pe->pe_curRadar->rad_radarRssi, NULL);
			pe->pe_curRadar->rad_height = ATH_RADAR_HEIGHT;
			ath_hal_setcapability(sc->sc_ah, HAL_CAP_PHYDIAG,
					      HAL_CAP_RADAR_HEIGHT,
					      pe->pe_curRadar->rad_height, NULL);
			pe->pe_curRadar->rad_pulseRssi = ATH_RADAR_PRSSI;
			ath_hal_setcapability(sc->sc_ah, HAL_CAP_PHYDIAG,
					      HAL_CAP_RADAR_PRSSI,
					      pe->pe_curRadar->rad_pulseRssi, NULL);
			pe->pe_curRadar->rad_inband = ATH_RADAR_INBAND;
			ath_hal_setcapability(sc->sc_ah, HAL_CAP_PHYDIAG,
					      HAL_CAP_RADAR_INBAND,
					      pe->pe_curRadar->rad_inband, NULL);
		}
		rfilt = ath_hal_getrxfilter(sc->sc_ah);
		rfilt |= HAL_RX_FILTER_PHYRADAR;
		ath_hal_setrxfilter(sc->sc_ah,rfilt);
		ath_hal_enablePhyDiag(sc->sc_ah);
	} else {
		rfilt = ath_hal_getrxfilter(sc->sc_ah);
		rfilt &= ~HAL_RX_FILTER_PHYRADAR;
		ath_hal_setrxfilter(sc->sc_ah,rfilt);
	}
}

void
ath_phyerr_disable(struct ath_softc *sc)
{
	u_int32_t rfilt;

	rfilt = ath_hal_getrxfilter(sc->sc_ah);
	rfilt &= ~HAL_RX_FILTER_PHYERR;
	ath_hal_setrxfilter(sc->sc_ah,rfilt);
	ath_hal_disablePhyDiag(sc->sc_ah);
}	

void
ath_phyerr_detach(struct ath_softc *sc)
{
	kfree(sc->sc_phyerr_state);
}

void
ath_phyerr_diag(struct net_device *dev, u_int phyerr,
		struct ath_desc *ds, struct sk_buff *skb)
{
	struct ath_softc *sc = dev->priv;
	struct ieee80211com *ic = &sc->sc_ic;
	struct ieee80211_channel *chan;
	u_int32_t mode;

	chan = ic->ic_bss->ni_chan;
	if ((phyerr == HAL_PHYERR_RADAR) && 
	    (ic->ic_opmode == IEEE80211_M_HOSTAP)) {
		mode = ieee80211_chan2mode(ic, chan);
		if (mode == IEEE80211_MODE_TURBO_G)
			ath_AR_event(dev, ds, skb);
		else
			ath_radar_event(dev, ds, skb);
	}
}
		
static void
ath_radar_event(struct net_device *dev, const struct ath_desc *ds,
		struct sk_buff *skb)
{
	struct ath_softc *sc = dev->priv;
	struct ath_phyerr_state *pe = NULL;
	struct ath_radar_state *radarState;

	KASSERT(sc->sc_phyerr_state != NULL,
		("NULL phyerr_state"));
	pe = (struct ath_phyerr_state *) sc->sc_phyerr_state;
	KASSERT(pe->pe_curRadar != NULL,
		("NULL curRadar"));

	radarState = pe->pe_curRadar;
	radarState->rad_numRadarEvents++;
#if 0
	if ((radarState->rad_numRadarEvents % 10) == 0)
		printk("Radar event count = %d\n",
		       radarState->rad_numRadarEvents);
#endif
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
 * This routine builds the hsitogram based on radar duration and does pattern matching
 * on incoming radars to determine if neighboring traffic is present.
 */

static void
ath_AR_event(struct net_device *dev, const struct ath_desc *ds,
	     struct sk_buff *skb)
{
	struct ath_softc *sc = dev->priv;
	struct ath_phyerr_state *pe;
	u_int32_t sumPeak=0,numPeaks,rssi,width,origRegionSum=0, i;
	u_int16_t thisTimeStamp;

	KASSERT(sc->sc_phyerr_state != NULL,
		("NULL phyerr_state"));
	pe = (struct ath_phyerr_state *) sc->sc_phyerr_state;
	thisTimeStamp =(u_int16_t) ds->ds_rxstat.rs_tstamp;
	rssi = ds->ds_rxstat.rs_rssi;
	width = ds->ds_rxstat.rs_datalen ?
		(u_int32_t)(*((u_int8_t *) skb->data)) : 0;
	width &= 0xff;	/* Only lower 8 bits are valid */

	/* determine if current radar is an extension of previous radar */
	if (pe->pe_arState.ar_prevWidth == 255) {
		/* tag on previous width for consideraion of low data rate ACKs */
		pe->pe_arState.ar_prevWidth += width;
		width = (width == 255) ? 255 : pe->pe_arState.ar_prevWidth;
	} else if ((width == 255) &&
		   (pe->pe_arState.ar_prevWidth == 510 ||
		    pe->pe_arState.ar_prevWidth == 765 ||
		    pe->pe_arState.ar_prevWidth == 1020)) {
		/* Aggregate up to 5 consecuate max radar widths
		 * to consider 11Mbps long preamble 1500-byte pkts
		 */
		pe->pe_arState.ar_prevWidth += width;
	} else if (pe->pe_arState.ar_prevWidth == 1275 && width != 255) {
		/* Found 5th consecute maxed out radar, reset history */
		width += pe->pe_arState.ar_prevWidth;
		pe->pe_arState.ar_prevWidth = 0;
	} else if (pe->pe_arState.ar_prevWidth > 255) {
		/* Ignore if there are less than 5 consecutive maxed out radars */
		pe->pe_arState.ar_prevWidth = width;
		width = 255;
	} else {
		pe->pe_arState.ar_prevWidth = width;
	}
	/* For ignoring noises with radar duration in ranges of 3-30: AP4x */
	if ((width >= 257 && width <= 278) ||	/* Region 7 - 5.5Mbps (long pre) ACK = 270 = 216 us */
	    (width >= 295 && width <= 325) ||	/* Region 8 - 2Mbps (long pre) ACKC = 320 = 256us */
	    (width >= 1280 && width <= 1300)) {
		u_int16_t wrapAroundAdj=0;
		u_int16_t base = (width >= 1280) ? 1275 : 255;
		if (thisTimeStamp < pe->pe_arState.ar_prevTimeStamp) {
			wrapAroundAdj = 32768;
		}
		if ((thisTimeStamp + wrapAroundAdj - pe->pe_arState.ar_prevTimeStamp) !=
		    (width - base)) {
			width = 1;
		}
	}
	if (width <= 10)
		return;

	/*
	 * Overloading the width=2 in: Store a count of radars w/max duration
	 * and high RSSI (not noise)
	 */
	if ((width == 255) && (rssi > ATH_AR_RSSI_THRESH_STRONG_PKTS))
		width = 2;
	
	/*
	 * Overloading the width=3 bin:
	 *   Double and store a count of rdars of durtaion that matches 11Mbps (long preamble)
	 *   TCP ACKs or 1500-byte data packets
	 */
	if ((width >= 1280 && width <= 1300) ||
	    (width >= 318 && width <= 325)) {
		width = 3;
		pe->pe_arState.ar_phyErrCount[3] += 2;
		pe->pe_arState.ar_ackSum += 2;
	}

	/* build histogram of radar duration */
	if (width > 0 && width <= 510)
		pe->pe_arState.ar_phyErrCount[width]++;
	else
		/* invalid radar width, throw it away */
		return;
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
		pe->pe_arState.ar_ackSum++;

		/* double the count for strong radars that match one of the ACK signatures */
		if (rssi > ATH_AR_RSSI_DOUBLE_THRESHOLD) {
			pe->pe_arState.ar_phyErrCount[width]++;
			pe->pe_arState.ar_ackSum++;
		}
		UPDATE_TOP_THREE_PEAKS(pe->pe_arState.ar_phyErrCount,
				       pe->pe_arState.ar_peakList, width);
		/* sum the counts of these peaks */
		numPeaks = ATH_MAX_NUM_PEAKS;
		origRegionSum = pe->pe_arState.ar_ackSum;
		for (i=0; i<= ATH_MAX_NUM_PEAKS; i++) {
			if (pe->pe_arState.ar_peakList[i] > 0) {
				if ((i==0) &&
				    (pe->pe_arState.ar_peakList[i] == 3) &&
				    (pe->pe_arState.ar_phyErrCount[3] <
				     pe->pe_arState.ar_phyErrCount[2]) &&
				    (pe->pe_arState.ar_phyErrCount[3] > 6)) {
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
					sumPeak += (pe->pe_arState.ar_phyErrCount[2]
						    + pe->pe_arState.ar_phyErrCount[3]);
					pe->pe_arState.ar_ackSum += (pe->pe_arState.ar_phyErrCount[2]
								   + pe->pe_arState.ar_phyErrCount[3]);
				} else {
					sumPeak += pe->pe_arState.ar_phyErrCount[pe->pe_arState.ar_peakList[i]];
				}
			} else 
				numPeaks--;
		}
		
		/*
		 * If sum of patterns matches exceeds packet threshold,
		 * perform comparison between peak-to-avg ratio against parThreshold
		 */
		if ((pe->pe_arState.ar_ackSum > pe->pe_arState.ar_packetThreshold) &&
		    ((sumPeak * ATH_AR_REGION_WIDTH) > (pe->pe_arState.ar_parThreshold * numPeaks *
							pe->pe_arState.ar_ackSum))) {
			/* neighboring traffic detected, get out of Turbo */
			sc->sc_curchan.channelFlags |= CHANNEL_AR;
			
			memset(pe->pe_arState.ar_peakList, 0,
			       sizeof(pe->pe_arState.ar_peakList));
			pe->pe_arState.ar_ackSum = 0;
			memset(pe->pe_arState.ar_phyErrCount, 0, 
			       sizeof(pe->pe_arState.ar_phyErrCount));
		} else {
			/*
			 * reset sum of matches to discount the count of
			 * strong radars with max duration
			 */
			pe->pe_arState.ar_ackSum = origRegionSum;
		}
	}
        pe->pe_arState.ar_prevTimeStamp = thisTimeStamp;
}
