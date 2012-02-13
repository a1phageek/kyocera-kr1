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
 * $File: //depot/sw/releases/SuperG_Linux_AP/src/802_11/madwifi/madwifi/net80211/ieee80211_beacon.c $
 * $Author: sam $
 * $DateTime: 2005/01/24 10:12:07 $
 *
 */
#ifndef EXPORT_SYMTAB
#define	EXPORT_SYMTAB
#endif

__FBSDID("$FreeBSD:$");
__KERNEL_RCSID(0, "$NetBSD:$");


/*
 * IEEE 802.11 beacon handling routines
 */
#include <linux/config.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/if_vlan.h>

#include "if_media.h"
#include <net80211/ieee80211_var.h>


#ifdef IEEE80211_DEBUG
void dump_tim(struct ieee80211_tim_element *tim, u_int8_t *data)
{
	int    i;

	printk("%s: id          : %d\n", __func__, tim->id);
	printk("%s: length      : %d\n", __func__, tim->len);
	printk("%s: dtim_count  : %d\n", __func__, tim->dtim_count);
	printk("%s: dtim_interval : %d\n", __func__, tim->dtim_interval);
	printk("%s: bitmap_ctl  : %d\n", __func__, tim->bitmap_control);

	for (i=0; i < tim->len; i++) {
		if (data[i]) {
			printk("%s: index %d, value %d\n", __func__, i, data[i]);
		}
	}
}
#endif



/*
 * ieee80211_build_tim
 *
 */
static u_int8_t 
ieee80211_build_tim(u_int8_t *frm, struct ieee80211com *ic, u_int8_t cab)
{
	u_int8_t index, n1, n2;
	struct ieee80211_tim_element *tim;
	struct ieee80211_beacon_mgt  *bm;
	u_int8_t                     *bitmap;

	bm     = &ic->ic_bm;	
	tim    = bm->tim = (struct ieee80211_tim_element *)frm;
	bitmap = bm->tim_bitmap;

	tim->id= IEEE80211_ELEMID_TIM;
	
	n1 = n2 = 0;
	if (bm->tim_bits_set) {
		/*
		 * Find N1, which is equal to the largest even number such that
		 * bits numbered 1 through (N1 * 8) - 1 in the bitmap are all 0.
		 */
		for (index = 0; index < TIM_BITMAP_LEN; index++) {
			if (bitmap[index] != 0) {
				n1 = index & 0xfe;
				break;
			}
		}
		
		/*
		 * Find N2, which is equal to the smallest number such that
		 * bits numbered (N2 + 1) * 8 through 2007 in the bitmap are all 0.
		 */
		for (index = TIM_BITMAP_LEN - 1; index >= n1; index--) {
			if (bitmap[index] != 0) {
				n2 = index;
				break;
			}
		}
	}
    
	/* Copy our partial virtual bitmap into the beacon's TIM element. */
	memcpy(tim->bitmap, bitmap + n1, n2 - n1 + 1);
	
	tim->bitmap_control = n1 & 0xFE;  /* clear the bottom bit for now */
	
	/* Update the length field. */
	tim->len = (n2 - n1) + 4;
    
	/* Update the Bitmap Control field. */
	tim->bitmap_control = n1 | (cab ? 1 : 0); 

	if (bm->tim_init) {
		tim->dtim_count = 0;
		bm->tim_init    = 0;
	}
	/* Decrement the DTIM Count modulo the DTIM Period. */
	if (tim->dtim_count == 0) {
		tim->dtim_count    = bm->dtim_interval - 1;
		tim->dtim_interval = bm->dtim_interval;
	} else {
		tim->dtim_count--;
	}
	return 2 + tim->len;
}



/*
 *
 * ieee80211_update_tim
 *
 */
void
ieee80211_update_tim(struct ieee80211com *ic, u_int16_t aid, u_int8_t set)
{
	struct ieee80211_beacon_mgt  *bm = &ic->ic_bm;
	u_int8_t                     byte_index, bit_index;
	u_int8_t                     *bitmap;

	if (ic->ic_opmode == IEEE80211_M_STA)
		return;

	bitmap = bm->tim_bitmap;

	aid &= 0x3fff;

	/* calculate the index for byte and bit */
	byte_index = aid / 8;
	bit_index  = 1 << (aid & 0x7);

	/* Bit is already in the right state. */
	if ((bitmap[byte_index] & bit_index) == (set ? bit_index : 0)) {
		return;
	}
	if (set) {
		bitmap[byte_index] |= bit_index;
		bm->tim_bits_set++;
	} else {
		bitmap[byte_index] &= ~bit_index;
		bm->tim_bits_set--;
	}
}
EXPORT_SYMBOL(ieee80211_update_tim);




static void
ieee80211_beacon_build(struct ieee80211com *ic, 
		       struct ieee80211_node *ni,
		       struct sk_buff *skb)
{
	struct ieee80211_beacon_mgt *b = &ic->ic_bm;
	struct ieee80211_rateset    *rs;
	u_int8_t                    *frm;
	int                         c_len, n_len, dist;

	frm = b->bo_start;
	rs  = &ni->ni_rates;

	if (ic->ic_opmode == IEEE80211_M_IBSS) {
		*frm++ = IEEE80211_ELEMID_IBSSPARMS;
		*frm++ = 2;  /* TODO: ATIM window */
		*frm++ = 0; 
		*frm++ = 0;		
	} else {
		frm += ieee80211_build_tim(frm, ic, 0 /* XXX */);
	}
	
	if (ic->ic_flags & IEEE80211_F_WPA)
		frm = ieee80211_add_wpa(frm, ic);
	if (ic->ic_curmode == IEEE80211_MODE_11G)
		frm = ieee80211_add_erp(frm, ic);

	frm = ieee80211_add_xrates(frm, rs);

	if (ic->ic_flags & IEEE80211_F_WME) {
		if (!ic->ic_nonAggressiveMode) {
			if (ic->ic_stats.is_highpri_traffic >
			    ((HIGH_PRI_SWITCH_THRESH * ni->ni_intval) / 100)) {
				ic->ic_nonAggressiveMode = 1;
				ic->ic_updateTxParams(ic->ic_dev);
				ic->ic_stats.is_highpri_traffic = AGGRESSIVE_MODE_SWITCH_HYSTERESIS;
			} else {
				ic->ic_stats.is_highpri_traffic = 0;
			}
		} else {
			if (ic->ic_stats.is_highpri_traffic <=
			    ((HIGH_PRI_SWITCH_THRESH * ni->ni_intval) / 100)) {
				ic->ic_nonAggressiveMode = 0;
				ic->ic_updateTxParams(ic->ic_dev);
				ic->ic_stats.is_highpri_traffic = 0;
			} else {
				ic->ic_stats.is_highpri_traffic = AGGRESSIVE_MODE_SWITCH_HYSTERESIS;
			}
		}
	
		frm = ieee80211_add_wme_param(frm, ic);
	}

	if (ic->ic_ath_cap)
		frm = ieee80211_add_athAdvCap(frm, ic->ic_ath_cap, 0x07fff);

	/* Reset the length of the skb */
	c_len = skb->tail - skb->data;
	n_len = frm - skb->data;
	dist  = n_len - c_len;

	if (dist > 0) {
		skb_put(skb, dist);
	} else if (dist < 0) {
		skb_trim(skb, n_len);
	}
}




/*
 * Allocate a beacon frame and fillin the appropriate bits.
 */
struct sk_buff *
ieee80211_beacon_alloc(struct ieee80211com *ic, 
		       struct ieee80211_node *ni,
		       struct ieee80211_beacon_mgt *b)
{
	struct net_device        *dev = ic->ic_dev;
	struct ieee80211_frame   *wh;
	struct sk_buff           *skb;
	struct ieee80211_rateset *rs;
	u_int8_t                 *frm;
	u_int16_t                capinfo;
	
	/*
	 * beacon frame format
	 *	[8]   time stamp
	 *	[2]   beacon interval
	 *	[2]   cabability information
	 *	[tlv] ssid
	 *	[tlv] supported rates
	 *	[3]   parameter set (DS)
	 *	[tlv] parameter set (IBSS/TIM)
	 *	[tlv] extended rate phy (ERP)
	 *	[tlv] extended supported rates
	 *	[tlv] WPA/RSN parameters
	 *      [tlv] WME
	 *  	[tlv] Atheros Advanced Capabilities
	 *   XXX  Vendor-specific OIDs (e.g. Atheros)
	 */

	rs  = &ni->ni_rates;

	skb = ieee80211_getmgtframe(&frm, IEEE80211_MTU_MAX);
	if (skb == NULL) {
		IEEE80211_DPRINTF(ic, IEEE80211_MSG_ANY,
				  ("%s: cannot get buf; size %u\n", __func__, 
				   IEEE80211_MTU_MAX));
		ic->ic_stats.is_tx_nobuf++;
		return NULL;
	}

	/* 
	 * Push the 80211 frame header 
	 */
	wh = (struct ieee80211_frame *)
		skb_push(skb, sizeof(struct ieee80211_frame));
	wh->i_fc[0] = IEEE80211_FC0_VERSION_0 | IEEE80211_FC0_TYPE_MGT |
		IEEE80211_FC0_SUBTYPE_BEACON;
	wh->i_fc[1] = IEEE80211_FC1_DIR_NODS;
	*(u_int16_t *)wh->i_dur = 0;
	IEEE80211_ADDR_COPY(wh->i_addr1, dev->broadcast);
	IEEE80211_ADDR_COPY(wh->i_addr2, ic->ic_myaddr);
	IEEE80211_ADDR_COPY(wh->i_addr3, ni->ni_bssid);
	*(u_int16_t *)wh->i_seq = 0;

	/*
	 * Set up the management data structure
	 */
	b->tim_init      = 1;
	b->dtim_interval = ic->ic_dtim_period;

	/* XXX timestamp is set by hardware */
	memset(frm, 0, 8);	

	frm += 8;
	*(u_int16_t *)frm = cpu_to_le16(ni->ni_intval);
	frm += 2;
	if (ic->ic_opmode == IEEE80211_M_IBSS)
		capinfo = IEEE80211_CAPINFO_IBSS;
	else
		capinfo = IEEE80211_CAPINFO_ESS;
	if (ic->ic_flags & IEEE80211_F_PRIVACY)
		capinfo |= IEEE80211_CAPINFO_PRIVACY;
	if ((ic->ic_flags & IEEE80211_F_SHPREAMBLE) &&
	    IEEE80211_IS_CHAN_2GHZ(ni->ni_chan))
		capinfo |= IEEE80211_CAPINFO_SHORT_PREAMBLE;
	if (ic->ic_flags & IEEE80211_F_SHSLOT)
		capinfo |= IEEE80211_CAPINFO_SHORT_SLOTTIME;

	b->bo_caps = (u_int16_t *)frm;

	*(u_int16_t *)frm = cpu_to_le16(capinfo);
	frm += 2;
	*frm++ = IEEE80211_ELEMID_SSID;
	if ((ic->ic_flags & IEEE80211_F_HIDESSID) == 0) {
		*frm++ = ni->ni_esslen;
		memcpy(frm, ni->ni_essid, ni->ni_esslen);
		frm += ni->ni_esslen;
	} else
		*frm++ = 0;

	frm = ieee80211_add_rates(frm, rs);
	if (ic->ic_curmode != IEEE80211_MODE_FH) {
		*frm++ = IEEE80211_ELEMID_DSPARMS;
		*frm++ = 1;
		*frm++ = ieee80211_chan2ieee(ic, ni->ni_chan);
	}

	/*
	 * End preset buffer
	 */
	b->bo_start = frm;

	ieee80211_beacon_build(ic, ni, skb);

	return skb;
}
EXPORT_SYMBOL(ieee80211_beacon_alloc);



/*
 * Update the dynamic parts of a beacon frame based on the current state.
 */
int
ieee80211_beacon_update(struct ieee80211com *ic, 
			struct ieee80211_node *ni,
			struct ieee80211_beacon_mgt *b, 
			struct sk_buff **skb0)
{
	u_int16_t capinfo;
 
	if (ic->ic_opmode == IEEE80211_M_IBSS)
		capinfo = IEEE80211_CAPINFO_IBSS;
	else
		capinfo = IEEE80211_CAPINFO_ESS;
	
	if (ic->ic_flags & IEEE80211_F_PRIVACY)
		capinfo |= IEEE80211_CAPINFO_PRIVACY;
	
	if ((ic->ic_flags & IEEE80211_F_SHPREAMBLE) &&
	    IEEE80211_IS_CHAN_2GHZ(ni->ni_chan))
		capinfo |= IEEE80211_CAPINFO_SHORT_PREAMBLE;
	
	if (ic->ic_flags & IEEE80211_F_SHSLOT)
		capinfo |= IEEE80211_CAPINFO_SHORT_SLOTTIME;
	
	*b->bo_caps = htole16(capinfo);
	
	ieee80211_beacon_build(ic, ni, *skb0);

	return 1;
}
EXPORT_SYMBOL(ieee80211_beacon_update);

