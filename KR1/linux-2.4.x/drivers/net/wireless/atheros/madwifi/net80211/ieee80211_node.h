/*	$NetBSD: ieee80211_node.h,v 1.7 2003/10/29 21:50:57 dyoung Exp $	*/
/*-
 * Copyright (c) 2001 Atsushi Onoe
 * Copyright (c) 2002-2004 Sam Leffler, Errno Consulting
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $FreeBSD: src/sys/net80211/ieee80211_node.h,v 1.7 2003/10/17 21:41:52 sam Exp $
 */
#ifndef _NET80211_IEEE80211_NODE_H_
#define _NET80211_IEEE80211_NODE_H_

#include <net80211/ieee80211_ioctl.h>		/* for ieee80211_nodestats */

/*
 * Each ieee80211com instance has a single timer that fires once a
 * second.  This is used to initiate various work depending on the
 * state of the instance: scanning (passive or active), ``transition''
 * (waiting for a response to a management frame when operating
 * as a station), and node inactivity processing (when operating
 * as an AP).  For inactivity processing each node has a timeout
 * set in it's ni_intval field that is decremented on each timeout
 * and the node is reclaimed when the counter goes to zero.  We
 * use different inactivity timeout values depending on whether
 * the node is associated and authorized (either by 802.1x or
 * open/shared key authentication) or associated but yet to be
 * authorized.  The latter timeout is shorter to more aggressively
 * reclaim nodes that leave part way through the 802.1x exchange.
 */
#define	IEEE80211_PSCAN_WAIT 	5		/* passive scan intvl (secs) */
#define	IEEE80211_TRANS_WAIT 	5		/* transition interval (secs) */
#define	IEEE80211_INACT_WAIT	5		/* inactivity interval (secs) */
#define	IEEE80211_INACT_INIT	(30/IEEE80211_INACT_WAIT)	/* initial */
#define	IEEE80211_INACT_AUTH	(180/IEEE80211_INACT_WAIT)	/* associated but not authorized */
#define	IEEE80211_INACT_RUN	(300/IEEE80211_INACT_WAIT)	/* authorized */

#define	IEEE80211_NODE_HASHSIZE	32
/* simple hash is enough for variation of macaddr */
#define	IEEE80211_NODE_HASH(addr)	\
	(((u_int8_t *)(addr))[IEEE80211_ADDR_LEN - 1] % IEEE80211_NODE_HASHSIZE)

#define	IEEE80211_RATE_SIZE	8		/* 802.11 standard */
#define	IEEE80211_RATE_MAXSIZE	15		/* max rates we'll handle */

struct ieee80211_rateset {
	u_int8_t		rs_nrates;
	u_int8_t		rs_rates[IEEE80211_RATE_MAXSIZE];
};

struct ieee80211_rsnparms {
	u_int8_t	rsn_mcastcipher;	/* mcast/group cipher */
	u_int8_t	rsn_mcastkeylen;	/* mcast key length */
	u_int8_t	rsn_ucastcipherset;	/* unicast cipher set */
	u_int8_t	rsn_ucastcipher;	/* selected unicast cipher */
	u_int8_t	rsn_ucastkeylen;	/* unicast key length */
	u_int8_t	rsn_keymgmtset;		/* key mangement algorithms */
	u_int8_t	rsn_keymgmt;		/* selected key mgmt algo */
	u_int16_t	rsn_caps;		/* capabilities */
};

/*
 * Node specific information.  Note that drivers are expected
 * to derive from this structure to add device-specific per-node
 * state.  This is done by overriding the ic_node_* methods in
 * the ieee80211com structure.
 */
struct ieee80211_node {
	TAILQ_ENTRY(ieee80211_node)	ni_list;
	LIST_ENTRY(ieee80211_node)	ni_hash;
	atomic_t		ni_refcnt;
	u_int			ni_scangen;	/* gen# for timeout scan */
	u_int8_t		ni_authmode;	/* authentication algorithm */
	u_int16_t		ni_flags;	/* special-purpose state */
#define	IEEE80211_NODE_AUTH	0x0001		/* authorized for data */
#define	IEEE80211_NODE_QOS	0x0002		/* QoS enabled */
#define	IEEE80211_NODE_ERP	0x0004		/* ERP enabled */
/* NB: this must have the same value as IEEE80211_FC1_PWR_MGT */
#define	IEEE80211_NODE_PWR_MGT	0x0010		/* power save mode enabled */
	u_int8_t		ni_ath_flags;	/* Atheros feature flags */
	/* NB: These must have the same values as IEEE80211_ATHC_* */
#define IEEE80211_NODE_TURBOP	0x0001		/* Turbo prime enable */
#define IEEE80211_NODE_COMP	0x0002		/* Compresssion enable */
#define IEEE80211_NODE_FF	0x0004          /* Fast Frame capable */
#define IEEE80211_NODE_XR	0x0008		/* Atheros WME enable */
#define IEEE80211_NODE_AR	0x0010		/* AR capable */
#define IEEE80211_NODE_BOOST	0x0080 
	u_int16_t		ni_associd;	/* assoc response */
	u_int16_t		ni_txpower;	/* current transmit power */
	u_int16_t		ni_vlan;	/* vlan tag */
	u_int32_t		*ni_challenge;	/* shared-key challenge */
	u_int8_t		*ni_wpa_ie;	/* captured WPA/RSN ie */
	u_int8_t		*ni_wme_ie;	/* captured WME ie */
	u_int8_t		*ni_ath_ie;     /* captured Atheros ie */
	u_int16_t		ni_txseqs[17];	/* seq to be xmitted per tid */
	u_int16_t		ni_rxseqs[17];	/* seq previous per tid*/
	u_int32_t		ni_rxfragstamp;	/* time stamp of last rx frag */
	struct sk_buff		*ni_rxfrag[3];	/* rx frag reassembly */
	struct ieee80211_rsnparms ni_rsn;	/* RSN/WPA parameters */
	struct ieee80211_key	ni_ucastkey;	/* unicast key */

	// Dino Chang 2005/03/04
	u_int32_t		ni_assoctime;
	// Dino

	/* hardware */
	u_int32_t		ni_rstamp;	/* recv timestamp */
	u_int8_t		ni_rssi;	/* recv ssi */

	/* header */
	u_int8_t		ni_macaddr[IEEE80211_ADDR_LEN];
	u_int8_t		ni_bssid[IEEE80211_ADDR_LEN];

	/* beacon, probe response */
	union {
		u_int8_t	data[8];
		u_int64_t	tsf;
	} ni_tstamp;				/* from last rcv'd beacon */
	u_int16_t		ni_intval;	/* beacon interval */
	u_int16_t		ni_capinfo;	/* capabilities */
	u_int8_t		ni_esslen;
	u_int8_t		ni_essid[IEEE80211_NWID_LEN];
	struct ieee80211_rateset ni_rates;	/* negotiated rate set */
	struct ieee80211_channel *ni_chan;
	u_int16_t		ni_fhdwell;	/* FH only */
	u_int8_t		ni_fhindex;	/* FH only */
	u_int8_t		ni_erp;		/* ERP from beacon/probe resp */
	u_int16_t		ni_timoff;	/* byte offset to TIM ie */

	/* others */
	struct sk_buff_head	ni_savedq;	/* packets queued for pspoll */
	u_int32_t		ni_pwrsavedrops;/* pspoll packets dropped */
	int			ni_fails;	/* failure count to associate */
	int			ni_inact;	/* current inactivity count */
	int			ni_txrate;	/* index to ni_rates[] */
	struct ieee80211_nodestats ni_stats;	/* per-node statistics */
};
MALLOC_DECLARE(M_80211_NODE);

#define	IEEE80211_NODE_AID(ni)	IEEE80211_AID(ni->ni_associd)

static __inline struct ieee80211_node *
ieee80211_ref_node(struct ieee80211_node *ni)
{
	ieee80211_node_incref(ni);
	return ni;
}

static __inline void
ieee80211_unref_node(struct ieee80211_node **ni)
{
	ieee80211_node_decref(*ni);
	*ni = NULL;			/* guard against use */
}

struct ieee80211com;

extern	void ieee80211_node_attach(struct ieee80211com *);
extern	void ieee80211_node_lateattach(struct ieee80211com *);
extern	void ieee80211_node_detach(struct ieee80211com *);

extern	void ieee80211_node_authorize(struct ieee80211com *,
		struct ieee80211_node *);
extern	void ieee80211_node_unauthorize(struct ieee80211com *,
		struct ieee80211_node *);

extern	void ieee80211_begin_scan(struct ieee80211com *, int);
extern	int ieee80211_next_scan(struct ieee80211com *);
extern	void ieee80211_create_ibss(struct ieee80211com*,
		struct ieee80211_channel *);
extern	void ieee80211_end_scan(struct ieee80211com *);
extern	int ieee80211_sta_join(struct ieee80211com *,
		struct ieee80211_node *);
extern	void ieee80211_sta_leave(struct ieee80211com *,
		struct ieee80211_node *);

extern	struct ieee80211_node *ieee80211_alloc_node(struct ieee80211com *,
		const u_int8_t *);
extern	struct ieee80211_node *ieee80211_dup_bss(struct ieee80211com *,
		const u_int8_t *);
extern	void ieee80211_free_allnodes(struct ieee80211com *);

#ifdef IEEE80211_DEBUG_REFCNT
extern	void ieee80211_free_node_debug(struct ieee80211com *,
		struct ieee80211_node *, const char *func, int line);
extern	struct ieee80211_node *ieee80211_find_node_debug(
		struct ieee80211com *, const u_int8_t *,
		const char *func, int line);
extern	struct ieee80211_node *ieee80211_find_txnode_debug(
		struct ieee80211com *, const u_int8_t *,
		const char *func, int line);
extern	struct ieee80211_node *ieee80211_find_node_with_channel_debug(
		struct ieee80211com *, const u_int8_t *macaddr,
		struct ieee80211_channel *, const char *func, int line);
extern	struct ieee80211_node *ieee80211_find_node_with_ssid_debug(
		struct ieee80211com *, const u_int8_t *macaddr,
		u_int ssidlen, const u_int8_t *ssid,
		const char *func, int line);
#define	ieee80211_free_node(nt, ni) \
	ieee80211_free_node_debug(nt, ni, __func__, __LINE__)
#define	ieee80211_find_node(nt, mac) \
	ieee80211_find_node_debug(nt, mac, __func__, __LINE__)
#define	ieee80211_find_txnode(nt, mac) \
	ieee80211_find_txnode_debug(nt, mac, __func__, __LINE__)
#define	ieee80211_find_node_with_channel(nt, mac, c) \
	ieee80211_find_node_with_channel_debug(nt, mac, c, __func__, __LINE__)
#define	ieee80211_find_node_with_ssid(nt, mac, sl, ss) \
	ieee80211_find_node_with_ssid_debug(nt, mac, sl, ss, __func__, __LINE__)
#else
extern	void ieee80211_free_node(struct ieee80211com *,
		struct ieee80211_node *);
extern	struct ieee80211_node *ieee80211_find_node(struct ieee80211com *,
		const u_int8_t *);
extern	struct ieee80211_node *ieee80211_find_txnode(struct ieee80211com *,
		const u_int8_t *);
extern	struct ieee80211_node *ieee80211_find_node_with_channel(
		struct ieee80211com *, const u_int8_t *macaddr,
		struct ieee80211_channel *);
extern	struct ieee80211_node *ieee80211_find_node_with_ssid(
		struct ieee80211com *, const u_int8_t *macaddr, u_int ssidlen,
		const u_int8_t *ssid);
#endif

typedef void ieee80211_iter_func(void *, struct ieee80211_node *);
extern	void ieee80211_iterate_nodes(struct ieee80211com *,
		ieee80211_iter_func *, void *);
extern	void node_cleanup(struct ieee80211com *, struct ieee80211_node *);

extern	void ieee80211_dump_node(struct ieee80211_node *);
extern	void ieee80211_dump_nodes(struct ieee80211com *);
extern	void ieee80211_timeout_nodes(struct ieee80211com *);

extern	void ieee80211_node_join(struct ieee80211com *,
		struct ieee80211_node *, int);
extern	void ieee80211_node_leave(struct ieee80211com *,
		struct ieee80211_node *);
extern	void ieee80211_set_shortslottime(struct ieee80211com *, int onoff);
#endif /* _NET80211_IEEE80211_NODE_H_ */
