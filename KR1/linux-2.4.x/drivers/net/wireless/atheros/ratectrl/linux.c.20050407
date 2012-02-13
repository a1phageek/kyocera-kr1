/*-
 * Copyright (c) 2004 Video54 Technologies, Inc.
 * Copyright (c) 2004 Atheros Communications, Inc.
 * All rights reserved.
 *
 * $Id: //depot/sw/releases/SuperG_Linux_AP/src/802_11/madwifi/ratectrl/linux.c#4 $
 */
#ifndef EXPORT_SYMTAB
#define	EXPORT_SYMTAB
#endif

/*
 * Atheros rate control algorithm (Linux-specific code)
 */
#include <linux/config.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/cache.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>

#include <asm/uaccess.h>

#include <net80211/if_media.h>
#include <net80211/ieee80211_var.h>

#include "if_athvar.h"
#include "ah_desc.h"

#include "ratectrl.h"

/*
 * Attach to a device instance.  Setup the public definition
 * of how much per-node space we need and setup the private
 * phy tables that have rate control parameters.  These tables
 * are normally part of the Atheros hal but are not included
 * in our hal as the rate control data was not being used and
 * was considered proprietary (at the time).
 */
struct ath_ratectrl *
ath_rate_attach(struct ath_softc *sc)
{
	struct atheros_softc *asc;

	asc = kmalloc(sizeof(struct atheros_softc), GFP_ATOMIC);
	if (asc == NULL)
		return NULL;
	memset(asc, 0, sizeof(struct atheros_softc));
	asc->arc.arc_space = sizeof(struct atheros_node);
	/*
	 * Use the magic number to figure out the chip type.
	 * There's probably a better way to do this but for
	 * now this suffices.
	 *
	 * NB: We don't have a separate set of tables for the
	 *     5210; treat it like a 5211 since it has the same
	 *     tx descriptor format and (hopefully) sufficiently
	 *     similar operating characteristics to work ok.
	 */
	switch (sc->sc_ah->ah_magic) {
	case 0x19570405:
	default:		/* XXX 5210 */
		ar5211AttachRateTables(asc);
		break;
	case 0x19541014:	/* 5212 */
		ar5212AttachRateTables(asc);
		break;
	}
	return &asc->arc;
}
EXPORT_SYMBOL(ath_rate_attach);

void
ath_rate_detach(struct ath_ratectrl *rc)
{
	kfree(rc);
}
EXPORT_SYMBOL(ath_rate_detach);

/*
 * Setup rate codes for management/control frames.  We force
 * all such frames to the lowest rate.
 */
static void
ath_rate_setmgtrates(struct ath_softc *sc, struct ath_node *an)
{
	const HAL_RATE_TABLE *rt = sc->sc_currates;

	/* setup rates for management frames */
	/* XXX management/control frames always go at lowest speed */
	an->an_tx_mgtrate = rt->info[0].rateCode;
	an->an_tx_mgtratesp = an->an_tx_mgtrate
			    | rt->info[0].shortPreamble;
}

/*
 * Initialize per-node rate control state.
 */
void
ath_rate_node_init(struct ath_softc *sc, struct ath_node *an)
{
	rcSibInit(sc, an);
	ath_rate_setmgtrates(sc, an);
}
EXPORT_SYMBOL(ath_rate_node_init);

/*
 * Cleanup per-node rate control state.
 */
void
ath_rate_node_cleanup(struct ath_softc *sc, struct ath_node *an)
{
	/* NB: nothing to do */
}
EXPORT_SYMBOL(ath_rate_node_cleanup);

/*
 * Copy per-node rate control state.
 */
void
ath_rate_node_copy(struct ath_softc *sc,
	struct ath_node *dst, const struct ath_node *src)
{
	struct atheros_node *odst = ATH_NODE_ATHEROS(dst);
	const struct atheros_node *osrc = ATH_NODE_ATHEROS(src);

	memcpy(odst, osrc, sizeof(struct atheros_node));
}
EXPORT_SYMBOL(ath_rate_node_copy);

/*
 * Return the next series 0 transmit rate and setup for a callback
 * to install the multi-rate transmit data if appropriate.  We cannot
 * install the multi-rate transmit data here because the caller is
 * going to initialize the tx descriptor and so would clobber whatever
 * we write. Note that we choose an arbitrary series 0 try count to
 * insure we get called back; this permits us to defer calculating
 * the actual number of tries until the callback at which time we
 * can just copy the pre-calculated series data.
 */
void
ath_rate_findrate(struct ath_softc *sc, struct ath_node *an,
	HAL_BOOL shortPreamble, size_t frameLen,
	u_int8_t *rix, int *try0, u_int8_t *txrate)
{
	struct atheros_node *oan = ATH_NODE_ATHEROS(an);
	struct atheros_softc *asc = (struct atheros_softc *) sc->sc_rc;
	const RATE_TABLE *pRateTable = asc->hwRateTable[sc->sc_curmode];

	if (asc->fixedrix == -1) {
		*rix = rcRateFind(sc, oan, frameLen);
		if (sc->sc_mrretry)
			*try0 = ATH_TXMAXTRY-1;	/* anything != ATH_TXMAXTRY */
		else
			*try0 = ATH_TXMAXTRY;
	} else {
		*rix = asc->fixedrix;
		*try0 = ATH_TXMAXTRY;
	}
	*txrate = pRateTable->info[*rix].rateCode
		| (shortPreamble ? pRateTable->info[*rix].shortPreamble : 0);
	/* update ni_txrate for status reports */
	an->an_node.ni_txrate = oan->rixMap[*rix];
}
EXPORT_SYMBOL(ath_rate_findrate);

/*
 * Install the extended tx descriptor state.  This should only
 * ever be called for devices that support multi-rate retry.
 * Note that we cheat here and block-copy the pre-calculated
 * series data for all series.
 */
void
ath_rate_setupxtxdesc(struct ath_softc *sc, struct ath_node *an,
	struct ath_desc *ds, HAL_BOOL shortPreamble, u_int8_t rix)
{
	struct atheros_node *oan = ATH_NODE_ATHEROS(an);
	struct atheros_softc *asc = (struct atheros_softc *) sc->sc_rc;
	const RATE_TABLE *pRateTable = asc->hwRateTable[sc->sc_curmode];
	const void *retrySched;

	if (!sc->sc_mrretry) {
		DPRINTF(sc, "%s: no multi-rate retry!\n", __func__);
		/* XXX something is wrong */
		return;
	}
	/* NB: only called for data frames */
	if (oan->txRateCtrl.probeRate) {
		retrySched = shortPreamble ?
			(const void *) &pRateTable->info[rix].probeShortSched :
			(const void *) &pRateTable->info[rix].probeSched;
	} else {
		retrySched = shortPreamble ?
			(const void *) &pRateTable->info[rix].shortSched :
			(const void *) &pRateTable->info[rix].normalSched;
	}
	memcpy(ds->ds_hw+2, retrySched, 2*sizeof(A_UINT32));
}
EXPORT_SYMBOL(ath_rate_setupxtxdesc);

#include <ar5212/ar5212desc.h>

#define	MS(_v, _f)	(((_v) & _f) >> _f##_S)

/*
 * Process a tx descriptor for a completed transmit (success or failure).
 */
void
ath_rate_tx_complete(struct ath_softc *sc,
	struct ath_node *an, const struct ath_desc *ds)
{
	struct atheros_softc *asc = (struct atheros_softc *) sc->sc_rc;
	const RATE_TABLE *pRateTable = asc->hwRateTable[sc->sc_curmode];
	u_int8_t txRate = ds->ds_txstat.ts_rate &~ HAL_TXSTAT_ALTRATE;

	if (pRateTable->rateCodeToIndex[txRate] == (u_int8_t) -1) {
		/*
		 * This can happen, for example, when switching bands
		 * and pending tx's are processed before the queue
		 * is flushed (should fix mode switch to ensure this
		 * does not happen).
		 */
		DPRINTF(sc, "%s: no mapping for rate code 0x%x",
			__func__, txRate);
		return;
	}
	if (ds->ds_txstat.ts_rate & HAL_TXSTAT_ALTRATE) {
		struct ar5212_desc *ads = AR5212DESC(ds);
		int finalTSIdx = MS(ads->ds_txstatus1, AR_FinalTSIndex);
		int series;

		/*
		 * Process intermediate rates that failed.
		 */
		for (series = 0; series < finalTSIdx; series++) {
			int rate, tries;

			/* NB: we know series <= 2 */
			switch (series) {
			case 0:
				rate = MS(ads->ds_ctl3, AR_XmitRate0);
				tries = MS(ads->ds_ctl2, AR_XmitDataTries0);
				break;
			case 1:
				rate = MS(ads->ds_ctl3, AR_XmitRate1);
				tries = MS(ads->ds_ctl2, AR_XmitDataTries1);
				break;
			default:
				rate = MS(ads->ds_ctl3, AR_XmitRate2);
				tries = MS(ads->ds_ctl2, AR_XmitDataTries2);
				break;
			}
			rcUpdate(sc, an
				, 2
				, pRateTable->rateCodeToIndex[rate]
				, tries
				, ds->ds_txstat.ts_rssi
				, ds->ds_txstat.ts_antenna
			);
		}
	}
	rcUpdate(sc, an
		, (ds->ds_txstat.ts_status & HAL_TXERR_XRETRY) != 0
		, pRateTable->rateCodeToIndex[txRate]
		, ds->ds_txstat.ts_shortretry + ds->ds_txstat.ts_longretry
		, ds->ds_txstat.ts_rssi
		, ds->ds_txstat.ts_antenna
	);
}
EXPORT_SYMBOL(ath_rate_tx_complete);

/*
 * Update rate-control state on station associate/reassociate.
 */
void
ath_rate_newassoc(struct ath_softc *sc, struct ath_node *an, int isnew)
{
	if (isnew) {
		struct atheros_softc *asc = (struct atheros_softc *) sc->sc_rc;
		struct atheros_node *oan = ATH_NODE_ATHEROS(an);

		rcSibUpdate(sc, an, 0);
		ath_rate_setmgtrates(sc, an);
		/* 
		 * Set an initial tx rate for the net80211 layer.
		 * Even though noone uses it, it wants to validate
		 * the setting before entering RUN state so if there
		 * was a pervious setting from a different node it
		 * may be invalid.
		 */
		if (asc->fixedrix != -1) 
			an->an_node.ni_txrate = oan->rixMap[asc->fixedrix];
		else
			an->an_node.ni_txrate = 0;
	}
}
EXPORT_SYMBOL(ath_rate_newassoc);

/*
 * Update rate-control state on a device state change.  When
 * operating as a station this includes associate/reassociate
 * with an AP.  Otherwise this gets called, for example, when
 * the we transition to run state when operating as an AP.
 */
void
ath_rate_newstate(struct ath_softc *sc, enum ieee80211_state state)
{
	struct atheros_softc *asc = (struct atheros_softc *) sc->sc_rc;
	struct ieee80211com *ic = &sc->sc_ic;

	/*
	 * Calculate index of any fixed rate configured.  It is safe
	 * to do this only here as changing/setting the fixed rate
	 * causes the 802.11 state machine to transition (which causes
	 * us to be notified).
	 */
	if (ic->ic_fixed_rate != -1) {
		const struct ieee80211_rateset *rs =
			&ic->ic_sup_rates[ic->ic_curmode];
		int r = rs->rs_rates[ic->ic_fixed_rate] & IEEE80211_RATE_VAL;
		asc->fixedrix = sc->sc_rixmap[r];
	} else
		asc->fixedrix = -1;
	if (state == IEEE80211_S_RUN)
		ath_rate_newassoc(sc, ATH_NODE(ic->ic_bss), 1);
}
EXPORT_SYMBOL(ath_rate_newstate);

void
atheros_setuptable(RATE_TABLE *rt)
{
	int i;

	for (i = 0; i < RATE_TABLE_SIZE; i++)
		rt->rateCodeToIndex[i] = (u_int8_t) -1;
	for (i = 0; i < rt->rateCount; i++) {
		u_int8_t code = rt->info[i].rateCode;
		rt->rateCodeToIndex[code] = i;
		rt->rateCodeToIndex[ code | rt->info[i].shortPreamble] = i;
	}
}

/*
 * Module glue.
 */
static	char *dev_info = "ath_rate_atheros";

MODULE_AUTHOR("Video54 Technologies, Inc.");
MODULE_DESCRIPTION("Rate control support for Atheros devices");
#ifdef MODULE_LICENSE
MODULE_LICENSE("Proprietary");
#endif

static int __init
init_ath_rate_atheros(void)
{
	/* XXX version is a guess; where should it come from? */
	printk(KERN_INFO "%s: Version 2.0.1\n"
		"Copyright (c) 2001-2004 Atheros Communications, Inc, "
		"All Rights Reserved\n", dev_info);
	ar5211SetupRateTables();
	ar5212SetupRateTables();
	return 0;
}
module_init(init_ath_rate_atheros);

static void __exit
exit_ath_rate_atheros(void)
{
	printk(KERN_INFO "%s: driver unloaded\n", dev_info);
}
module_exit(exit_ath_rate_atheros);
