/*-
 * Copyright (c) 2002-2005 Sam Leffler, Errno Consulting
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    similar to the "NO WARRANTY" disclaimer below ("Disclaimer") and any
 *    redistribution must be conditioned upon including a substantially
 *    similar Disclaimer requirement for further binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF NONINFRINGEMENT, MERCHANTIBILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES.
 *
 * $Id: //depot/sw/releases/linuxsrc/src/802_11/madwifi/madwifi/net80211/ieee80211_wireless.c#15 $
 */

#ifdef CONFIG_NET_WIRELESS

/*
 * Wireless extensions support for 802.11 common code.
 */
#include <linux/config.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/utsname.h>
#include <linux/if_arp.h>		/* XXX for ARPHRD_ETHER */
#include <net/iw_handler.h>

#include <asm/uaccess.h>

#include "if_media.h"
//**************** Added by Ed Lai
#include "../ath/version.h"
//**************************
#include <net80211/ieee80211_var.h>

#define	IS_UP(_dev) \
	(((_dev)->flags & (IFF_RUNNING|IFF_UP)) == (IFF_RUNNING|IFF_UP))
#define	IS_UP_AUTO(_vap) \
	(IS_UP((_vap)->iv_dev) && \
	 (_vap)->iv_ic->ic_roaming == IEEE80211_ROAMING_AUTO)
#define	RESCAN	1

/*
 * Units are in db above the noise floor. That means the
 * rssi values reported in the tx/rx descriptors in the
 * driver are the SNR expressed in db.
 *
 * If you assume that the noise floor is -95, which is an
 * excellent assumption 99.5 % of the time, then you can
 * derive the absolute signal level (i.e. -95 + rssi). 
 * There are some other slight factors to take into account
 * depending on whether the rssi measurement is from 11b,
 * 11g, or 11a.   These differences are at most 2db and
 * can be documented.
 *
 * NB: various calculations are based on the orinoco/wavelan
 *     drivers for compatibility
 */
static void
set_quality(struct iw_quality *iq, u_int rssi)
{
	iq->qual = rssi;
	/* NB: max is 94 because noise is hardcoded to 161 */
	if (iq->qual > 94)
		iq->qual = 94;

	iq->noise = 161;		/* -95dBm */
	iq->level = iq->noise + iq->qual;
	iq->updated = 7;
}

static struct iw_statistics *
ieee80211_iw_getstats(struct net_device *dev)
{
	struct ieee80211vap *vap = dev->priv;
	struct iw_statistics *is = &vap->iv_iwstats;

	set_quality(&is->qual, ieee80211_getrssi(vap->iv_ic));
	is->status = vap->iv_state;
	is->discard.nwid = vap->iv_stats.is_rx_wrongbss
			 + vap->iv_stats.is_rx_ssidmismatch;
	is->discard.code = vap->iv_stats.is_rx_wepfail
			 + vap->iv_stats.is_rx_decryptcrc;
	is->discard.fragment = 0;
	is->discard.retries = 0;
	is->discard.misc = 0;

	is->miss.beacon = 0;

	return is;
}

static int
ieee80211_ioctl_giwname(struct net_device *dev,
		   struct iw_request_info *info,
		   char *name, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211_channel *c = vap->iv_ic->ic_curchan;

	if (IEEE80211_IS_CHAN_108G(c))
		strncpy(name, "IEEE 802.11Tg", IFNAMSIZ);
	else if (IEEE80211_IS_CHAN_108A(c))
		strncpy(name, "IEEE 802.11Ta", IFNAMSIZ);
	else if (IEEE80211_IS_CHAN_TURBO(c))
		strncpy(name, "IEEE 802.11T", IFNAMSIZ);
	else if (IEEE80211_IS_CHAN_ANYG(c))
		strncpy(name, "IEEE 802.11g", IFNAMSIZ);
	else if (IEEE80211_IS_CHAN_A(c))
		strncpy(name, "IEEE 802.11a", IFNAMSIZ);
	else if (IEEE80211_IS_CHAN_B(c))
		strncpy(name, "IEEE 802.11b", IFNAMSIZ);
	else
		strncpy(name, "IEEE 802.11", IFNAMSIZ);
	/* XXX FHSS */
	return 0;
}

/*
 * Get a key index from a request.  If nothing is
 * specified in the request we use the current xmit
 * key index.  Otherwise we just convert the index
 * to be base zero.
 */
static int
getiwkeyix(struct ieee80211vap *vap, const struct iw_point* erq, int *kix)
{
	int kid;

	kid = erq->flags & IW_ENCODE_INDEX;
	if (kid < 1 || kid > IEEE80211_WEP_NKID) {
		kid = vap->iv_def_txkey;
		if (kid == IEEE80211_KEYIX_NONE)
			kid = 0;
	} else
		--kid;
	if (0 <= kid && kid < IEEE80211_WEP_NKID) {
		*kix = kid;
		return 0;
	} else
		return EINVAL;
}

static int
ieee80211_ioctl_siwencode(struct net_device *dev,
			  struct iw_request_info *info,
			  struct iw_point *erq, char *keybuf)
{
	struct ieee80211vap *vap = dev->priv;
	int kid, error;
	int wepchange = 0;

	if ((erq->flags & IW_ENCODE_DISABLED) == 0) {
		/*
		 * Enable crypto, set key contents, and
		 * set the default transmit key.
		 */
		error = getiwkeyix(vap, erq, &kid);
		if (error)
			return -error;
		if (erq->length > IEEE80211_KEYBUF_SIZE)
			return -EINVAL;
		/* XXX no way to install 0-length key */
		ieee80211_key_update_begin(vap);
		if (erq->length > 0) {
			struct ieee80211_key *k = &vap->iv_nw_keys[kid];

			/*
			 * Set key contents.  This interface only supports WEP.
			 * Indicate intended key index.			 
			 */
			k->wk_keyix = kid;
			if (ieee80211_crypto_newkey(vap, IEEE80211_CIPHER_WEP,
			    IEEE80211_KEY_XMIT | IEEE80211_KEY_RECV, k)) {
				k->wk_keylen = erq->length;
				memcpy(k->wk_key, keybuf, erq->length);
				memset(k->wk_key + erq->length, 0,
					IEEE80211_KEYBUF_SIZE - erq->length);
				if (!ieee80211_crypto_setkey(vap, k, vap->iv_myaddr, NULL))
					error = -EINVAL;		/* XXX */
			} else {
				error = -EINVAL;
			}
		} else {
			/*
			 * When the length is zero the request only changes
			 * the default transmit key.  Verify the new key has
			 * a non-zero length.
			 */
			if (vap->iv_nw_keys[kid].wk_keylen == 0)
				error = -EINVAL;
		}
		if (error == 0) {
			/*
			 * The default transmit key is only changed when:
			 * 1. Privacy is enabled and no key matter is
			 *    specified.
			 * 2. Privacy is currently disabled.
			 * This is deduced from the iwconfig man page.
			 */
			if (erq->length == 0 ||
			    (vap->iv_flags & IEEE80211_F_PRIVACY) == 0)
				vap->iv_def_txkey = kid;
			wepchange = (vap->iv_flags & IEEE80211_F_PRIVACY) == 0;
			vap->iv_flags |= IEEE80211_F_PRIVACY;
		}
		ieee80211_key_update_end(vap);
	} else {
		if ((vap->iv_flags & IEEE80211_F_PRIVACY) == 0)
			return 0;
		vap->iv_flags &= ~IEEE80211_F_PRIVACY;
		wepchange = 1;
		error = 0;
	}
	if (error == 0) {
		/* Set policy for unencrypted frames */	
		if ((erq->flags & IW_ENCODE_OPEN) && 
		    (!(erq->flags & IW_ENCODE_RESTRICTED))) {
			vap->iv_flags &= ~IEEE80211_F_DROPUNENC;
		} else if (!(erq->flags & IW_ENCODE_OPEN) && 
			 (erq->flags & IW_ENCODE_RESTRICTED)) {
			vap->iv_flags |= IEEE80211_F_DROPUNENC;
		} else {
			/* Default policy */
			if (vap->iv_flags & IEEE80211_F_PRIVACY) 
				vap->iv_flags |= IEEE80211_F_DROPUNENC;
			else
				vap->iv_flags &= ~IEEE80211_F_DROPUNENC;
		}	
	}
	if (error == 0 && IS_UP(vap->iv_dev)) {
		/*
		 * Device is up and running; we must kick it to
		 * effect the change.  If we're enabling/disabling
		 * crypto use then we must re-initialize the device
		 * so the 802.11 state machine is reset.  Otherwise
		 * the key state should have been updated above.
		 */
		if (wepchange && IS_UP_AUTO(vap)) {
			ieee80211_new_state(vap, IEEE80211_S_SCAN, 0);
		}
	}
#ifdef ATH_SUPERG_XR
	/* set the same params on the xr vap device if exists */
	if(!error && vap->iv_xrvap && !(vap->iv_flags & IEEE80211_F_XR) )
		ieee80211_ioctl_siwencode(vap->iv_xrvap->iv_dev,info,erq,keybuf);
#endif
	return error;
}

static int
ieee80211_ioctl_giwencode(struct net_device *dev,
			  struct iw_request_info *info,
			  struct iw_point *erq, char *key)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211_key *k;
	int error, kid;

	if (vap->iv_flags & IEEE80211_F_PRIVACY) {
		error = getiwkeyix(vap, erq, &kid);
		if (error != 0)
			return -error;
		k = &vap->iv_nw_keys[kid];
		/* XXX no way to return cipher/key type */

		erq->flags = kid + 1;			/* NB: base 1 */
		if (erq->length > k->wk_keylen)
			erq->length = k->wk_keylen;
		memcpy(key, k->wk_key, erq->length);
		erq->flags |= IW_ENCODE_ENABLED;
	} else {
		erq->length = 0;
		erq->flags = IW_ENCODE_DISABLED;
	}
	if (vap->iv_flags & IEEE80211_F_DROPUNENC)
		erq->flags |= IW_ENCODE_RESTRICTED;
	else
		erq->flags |= IW_ENCODE_OPEN;
	return 0;
}

#ifndef ifr_media
#define	ifr_media	ifr_ifru.ifru_ivalue
#endif

static int
ieee80211_ioctl_siwrate(struct net_device *dev,
			struct iw_request_info *info,
			struct iw_param *rrq, char *extra)
{
	static const u_int mopts[] = {
		IFM_AUTO,
		IFM_IEEE80211_11A,
		IFM_IEEE80211_11B,
		IFM_IEEE80211_11G,
		IFM_IEEE80211_FH,
		IFM_IEEE80211_11A | IFM_IEEE80211_TURBO,
		IFM_IEEE80211_11G | IFM_IEEE80211_TURBO,
	};
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct ifreq ifr;
	struct ifmediareq imr;
	int rate, retv;

	if (vap->iv_media.ifm_cur == NULL)
		return -EINVAL;
	memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_media = vap->iv_media.ifm_cur->ifm_media &~ (IFM_MMASK|IFM_TMASK);
	ifr.ifr_media |= mopts[vap->iv_des_mode];
	if (rrq->fixed) {
		/* XXX fudge checking rates */
		rate = ieee80211_rate2media(ic, 2 * rrq->value / 1000000,
				vap->iv_des_mode);
		if (rate == IFM_AUTO)		/* NB: unknown rate */
			return -EINVAL;
	} else
		rate = IFM_AUTO;
	ifr.ifr_media |= IFM_SUBTYPE(rate);

	/* refresh media capabilities based on channel */
	ifmedia_removeall(&vap->iv_media);
	(void) ieee80211_media_setup(ic, &vap->iv_media,
		vap->iv_caps, vap->iv_media.ifm_change, 
				vap->iv_media.ifm_status);
	ieee80211_media_status(dev, &imr);
	ifmedia_set(&vap->iv_media, imr.ifm_active);

	retv = ifmedia_ioctl(vap->iv_dev, &ifr, &vap->iv_media, SIOCSIFMEDIA);
	if (retv == ENETRESET)
		retv = IS_UP_AUTO(vap) ? ieee80211_open(vap->iv_dev) : 0;
	return -retv;
}

static int
ieee80211_ioctl_giwrate(struct net_device *dev,
			struct iw_request_info *info,
			struct iw_param *rrq, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ifmediareq imr;
	int rate;

	memset(&imr, 0, sizeof(imr));
	vap->iv_media.ifm_status(vap->iv_dev, &imr);

	rrq->fixed = IFM_SUBTYPE(vap->iv_media.ifm_media) != IFM_AUTO;
	/* media status will have the current xmit rate if available */
	rate = ieee80211_media2rate(imr.ifm_active);
	if (rate == -1)		/* IFM_AUTO */
		rate = 0;
	rrq->value = 1000000 * (rate / 2);

	return 0;
}

static int
ieee80211_ioctl_siwsens(struct net_device *dev,
			struct iw_request_info *info,
			struct iw_param *sens, char *extra)
{
	return 0;
}

static int
ieee80211_ioctl_giwsens(struct net_device *dev,
			struct iw_request_info *info,
			struct iw_param *sens, char *extra)
{
	sens->value = 0;
	sens->fixed = 1;

	return 0;
}

static int
ieee80211_ioctl_siwrts(struct net_device *dev,
		       struct iw_request_info *info,
		       struct iw_param *rts, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	u16 val;

	if (rts->disabled)
		val = IEEE80211_RTS_MAX;
	else if (IEEE80211_RTS_MIN <= rts->value &&
	    rts->value <= IEEE80211_RTS_MAX)
		val = rts->value;
	else
		return -EINVAL;
	if (val != vap->iv_rtsthreshold) {
		vap->iv_rtsthreshold = val;
		if (IS_UP(vap->iv_dev))
			return -ic->ic_reset(ic->ic_dev);
	}
	return 0;
}

static int
ieee80211_ioctl_giwrts(struct net_device *dev,
		       struct iw_request_info *info,
		       struct iw_param *rts, char *extra)
{
	struct ieee80211vap *vap = dev->priv;

	rts->value = vap->iv_rtsthreshold;
	rts->disabled = (rts->value == IEEE80211_RTS_MAX);
	rts->fixed = 1;

	return 0;
}

static int
ieee80211_ioctl_siwfrag(struct net_device *dev,
			struct iw_request_info *info,
			struct iw_param *rts, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	u16 val;

	if (rts->disabled)
	    val = 2346;
	else if (rts->value < 256 || rts->value > 2346)
	    return -EINVAL;
	else
	    val = (rts->value & ~0x1);

	if (val != vap->iv_fragthreshold) {
		vap->iv_fragthreshold = val;
		if (IS_UP(ic->ic_dev))
			return -ic->ic_reset(ic->ic_dev);
	}
	return 0;
}

static int
ieee80211_ioctl_giwfrag(struct net_device *dev,
			struct iw_request_info *info,
			struct iw_param *rts, char *extra)
{
	struct ieee80211vap *vap = dev->priv;

	rts->value = vap->iv_fragthreshold;
	rts->disabled = (rts->value == 2346);
	rts->fixed = 1;

	return 0;
}

static int
ieee80211_ioctl_siwap(struct net_device *dev,
		      struct iw_request_info *info,
		      struct sockaddr *ap_addr, char *extra)
{
	static const u_int8_t zero_bssid[IEEE80211_ADDR_LEN];
	struct ieee80211vap *vap = dev->priv;

	/* NB: should only be set when in STA mode */
	if (vap->iv_opmode != IEEE80211_M_STA)
		return -EINVAL;
	IEEE80211_ADDR_COPY(vap->iv_des_bssid, &ap_addr->sa_data);
	/* looks like a zero address disables */
	if (IEEE80211_ADDR_EQ(vap->iv_des_bssid, zero_bssid))
		vap->iv_flags &= ~IEEE80211_F_DESBSSID;
	else
		vap->iv_flags |= IEEE80211_F_DESBSSID;
	if (IS_UP_AUTO(vap))
		ieee80211_new_state(vap, IEEE80211_S_SCAN, 0);
	return 0;
}

static int
ieee80211_ioctl_giwap(struct net_device *dev,
		      struct iw_request_info *info,
		      struct sockaddr *ap_addr, char *extra)
{
	struct ieee80211vap *vap = dev->priv;

	if (vap->iv_flags & IEEE80211_F_DESBSSID)
		IEEE80211_ADDR_COPY(&ap_addr->sa_data, vap->iv_des_bssid);
	else {
		static const u_int8_t zero_bssid[IEEE80211_ADDR_LEN];
		if(vap->iv_state == IEEE80211_S_RUN) 
			IEEE80211_ADDR_COPY(&ap_addr->sa_data, vap->iv_bss->ni_bssid);
		else
			IEEE80211_ADDR_COPY(&ap_addr->sa_data, zero_bssid);
	}
	ap_addr->sa_family = ARPHRD_ETHER;
	return 0;
}

static int
ieee80211_ioctl_siwnickn(struct net_device *dev,
			 struct iw_request_info *info,
			 struct iw_point *data, char *nickname)
{
	struct ieee80211vap *vap = dev->priv;

	if (data->length > IEEE80211_NWID_LEN)
		return -EINVAL;

	memset(vap->iv_nickname, 0, IEEE80211_NWID_LEN);
	memcpy(vap->iv_nickname, nickname, data->length);
	vap->iv_nicknamelen = data->length;

	return 0;
}

static int
ieee80211_ioctl_giwnickn(struct net_device *dev,
			 struct iw_request_info *info,
			 struct iw_point *data, char *nickname)
{
	struct ieee80211vap *vap = dev->priv;

	if (data->length > vap->iv_nicknamelen + 1)
		data->length = vap->iv_nicknamelen + 1;
	if (data->length > 0) {
		memcpy(nickname, vap->iv_nickname, data->length-1);
		nickname[data->length-1] = '\0';
	}
	return 0;
}

static int
find11gchannel(struct ieee80211com *ic, int i, int freq)
{
	for (; i < ic->ic_nchans; i++) {
		const struct ieee80211_channel *c = &ic->ic_channels[i];
		if (c->ic_freq == freq && IEEE80211_IS_CHAN_ANYG(c))
			return 1;
	}
	return 0;
}

static struct ieee80211_channel *
findchannel(struct ieee80211com *ic, int ieee, int mode)
{
	static const u_int chanflags[] = {
		0,			/* IEEE80211_MODE_AUTO */
		IEEE80211_CHAN_A,	/* IEEE80211_MODE_11A */
		IEEE80211_CHAN_B,	/* IEEE80211_MODE_11B */
		IEEE80211_CHAN_PUREG,	/* IEEE80211_MODE_11G */
		IEEE80211_CHAN_FHSS,	/* IEEE80211_MODE_FH */
		IEEE80211_CHAN_108A,	/* IEEE80211_MODE_TURBO_A */
		IEEE80211_CHAN_108G,	/* IEEE80211_MODE_TURBO_G */
		IEEE80211_CHAN_ST,	/* IEEE80211_MODE_TURBO_STATIC_A */
	};
	u_int modeflags;
	int i;

	modeflags = chanflags[mode];
	for (i = 0; i < ic->ic_nchans; i++) {
		struct ieee80211_channel *c = &ic->ic_channels[i];

		if (c->ic_ieee != ieee)
			continue;
		
		/* Skip channels if required channel spacing (half/quater)
		 * is not matched.
		 */
		if (ic->ic_chanbwflag != 0) {
			if ((ic->ic_chanbwflag & c->ic_flags) == 0)
				continue;
		}

		if (mode == IEEE80211_MODE_AUTO) {
			/* ignore turbo channels for autoselect */
			if (IEEE80211_IS_CHAN_TURBO(c))
				continue;
			/*
			 * XXX special-case 11b/g channels so we
			 *     always select the g channel if both
			 *     are present.
			 */
			if (!IEEE80211_IS_CHAN_B(c) ||
			    !find11gchannel(ic, i+1, c->ic_freq))
				return c;
		} else {
			if ((c->ic_flags & modeflags) == modeflags)
				return c;
		}
	}
	return NULL;
}

#define	IEEE80211_MODE_TURBO_STATIC_A	IEEE80211_MODE_MAX
static int
ieee80211_check_mode_consistency(struct ieee80211com *ic,int mode,struct ieee80211_channel *c)
{
	if (c == IEEE80211_CHAN_ANYC) return 0;
	switch (mode) {
		case IEEE80211_MODE_11B:
			if(IEEE80211_IS_CHAN_B(c)) 
				return 0;
			else
				return 1;
			break;

		case IEEE80211_MODE_11G:
			if(IEEE80211_IS_CHAN_ANYG(c)) 
				return 0;
			else
				return 1;
			break;

		case IEEE80211_MODE_11A:
			if(IEEE80211_IS_CHAN_A(c)) 
				return 0;
			else
				return 1;
			break;

		case IEEE80211_MODE_TURBO_STATIC_A:
			if(IEEE80211_IS_CHAN_A(c) && IEEE80211_IS_CHAN_STURBO(c) ) 
				return 0;
			else
				return 1;
			break;

		case IEEE80211_MODE_AUTO:
			return 0;
			break;

	}
	return 1;

}
#undef	IEEE80211_MODE_TURBO_STATIC_A

static int
ieee80211_ioctl_siwfreq(struct net_device *dev,
			struct iw_request_info *info,
			struct iw_freq *freq, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct ieee80211_channel *c, *c2;
	int i;
	
	if (freq->e > 1)
		return -EINVAL;
	if (freq->e == 1)
		i = (ic->ic_mhz2ieee)(ic, freq->m / 100000, 0);
	else
		i = freq->m;
	if (i != 0) {
		if (i > IEEE80211_CHAN_MAX)
			return -EINVAL;
		c = findchannel(ic, i, vap->iv_des_mode);
		if (c == NULL) {
			c = findchannel(ic, i, IEEE80211_MODE_AUTO);
			if (c == NULL)			/* no channel */
				return -EINVAL;
		}
		/*
		 * Fine tune channel selection based on desired mode:
		 *   if 11b is requested, find the 11b version of any
		 *      11g channel returned,
		 *   if static turbo, find the turbo version of any
		 *	11a channel return,
		 *   otherwise we should be ok with what we've got.
		 */
		switch (vap->iv_des_mode) {
		case IEEE80211_MODE_11B:
			if (IEEE80211_IS_CHAN_ANYG(c)) {
				c2 = findchannel(ic, i, IEEE80211_MODE_11B);
				/* NB: should not happen, =>'s 11g w/o 11b */
				if (c2 != NULL)
					c = c2;
			}
			break;
		case IEEE80211_MODE_TURBO_A:
			if (IEEE80211_IS_CHAN_A(c)) {
				c2 = findchannel(ic, i, IEEE80211_MODE_TURBO_A);
				if (c2 != NULL)
					c = c2;
			}
			break;
		default:		/* NB: no static turboG */
			break;
		}
		if(ieee80211_check_mode_consistency(ic,vap->iv_des_mode,c)) {
			if (vap->iv_opmode == IEEE80211_M_HOSTAP)
				return -EINVAL;
		}
		if (vap->iv_state == IEEE80211_S_RUN && c == ic->ic_bsschan)
			return 0;			/* no change, return */

		/* Don't allow to change to channel with radar found */
		if (c->ic_flags & IEEE80211_CHAN_RADAR)
			return -EINVAL;

		/*
		 * Mark desired channel and if running force a
		 * radio change.
		 */
		vap->iv_des_chan = c;
	} else {
		/*
		 * Intepret channel 0 to mean "no desired channel";
		 * otherwise there's no way to undo fixing the desired
		 * channel.
		 */
		if (vap->iv_des_chan == IEEE80211_CHAN_ANYC)
			return 0;
		vap->iv_des_chan = IEEE80211_CHAN_ANYC;
	}
#if 0
	if (vap->iv_des_chan != IEEE80211_CHAN_ANYC) {
		int mode = ieee80211_chan2mode(vap->iv_des_chan);
		if (mode != ic->ic_curmode)
			ieee80211_setmode(ic, mode);
	}
#endif
	if ((vap->iv_opmode == IEEE80211_M_MONITOR ||
	    vap->iv_opmode == IEEE80211_M_WDS) &&
	    vap->iv_des_chan != IEEE80211_CHAN_ANYC) {
		/*
		 * Monitor and wds modes can switch directly.
		 */
		if (vap->iv_state == IEEE80211_S_RUN) {
			ic->ic_curchan = vap->iv_des_chan;
			ic->ic_set_channel(ic);
		}
	} else {
		/*
		 * Need to go through the state machine in case we need
		 * to reassociate or the like.  The state machine will
		 * pickup the desired channel and avoid scanning.
		 */
		if (IS_UP_AUTO(vap))
			ieee80211_new_state(vap, IEEE80211_S_SCAN, 0);
	}
	return 0;
}

static int
ieee80211_ioctl_giwfreq(struct net_device *dev,
			struct iw_request_info *info,
			struct iw_freq *freq, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;

	if (vap->iv_state == IEEE80211_S_RUN &&
	    vap->iv_opmode != IEEE80211_M_MONITOR) {
		/*
		 * NB: use curchan for monitor mode so you can see
		 *     manual scanning by apps like kismet.
		 */
		KASSERT(ic->ic_bsschan != IEEE80211_CHAN_ANYC,
			("bss channel not set"));
		freq->m = ic->ic_bsschan->ic_freq;
	} else if (vap->iv_state != IEEE80211_S_INIT)	/* e.g. when scanning */
		freq->m = ic->ic_curchan->ic_freq;
	else if (vap->iv_des_chan != IEEE80211_CHAN_ANYC)
		freq->m = vap->iv_des_chan->ic_freq;
	else
		freq->m = 0;
	freq->m *= 100000;
	freq->e = 1;

	return 0;
}

#ifdef ATH_SUPERG_XR 
/*
 * Copy desired ssid state from one vap to another.
 */
static void
copy_des_ssid(struct ieee80211vap *dst, const struct ieee80211vap *src)
{
	dst->iv_des_nssid = src->iv_des_nssid;
	memcpy(dst->iv_des_ssid, src->iv_des_ssid,
	    src->iv_des_nssid * sizeof(src->iv_des_ssid[0]));
}
#endif /* ATH_SUPERG_XR */

static int
ieee80211_ioctl_siwessid(struct net_device *dev,
			 struct iw_request_info *info,
			 struct iw_point *data, char *ssid)
{
	struct ieee80211vap *vap = dev->priv;

	if (vap->iv_opmode == IEEE80211_M_WDS)
		return -EOPNOTSUPP;

	if (data->flags == 0) {		/* ANY */
		vap->iv_des_nssid = 0;
	} else {
		if (data->length > IEEE80211_NWID_LEN)
			data->length = IEEE80211_NWID_LEN;
		/* NB: always use entry 0 */
		memcpy(vap->iv_des_ssid[0].ssid, ssid, data->length);
		vap->iv_des_ssid[0].len = data->length;
		vap->iv_des_nssid = 1;
		/*
		 * Deduct a trailing \0 since iwconfig passes a string
		 * length that includes this.  Unfortunately this means
		 * that specifying a string with multiple trailing \0's
		 * won't be handled correctly.  Not sure there's a good
		 * solution; the API is botched (the length should be
		 * exactly those bytes that are meaningful and not include
		 * extraneous stuff).
		 */
		if (data->length > 0 &&
		    vap->iv_des_ssid[0].ssid[data->length-1] == '\0')
			vap->iv_des_ssid[0].len--;
	}
#ifdef ATH_SUPERG_XR 
	if (vap->iv_xrvap != NULL && !(vap->iv_flags & IEEE80211_F_XR)) {
		if (data->flags == 0)
			vap->iv_des_nssid = 0;
		else
			copy_des_ssid(vap->iv_xrvap, vap);
	}
#endif
	return IS_UP_AUTO(vap) ? -ieee80211_init(vap->iv_dev, RESCAN) : 0;
}

static int
ieee80211_ioctl_giwessid(struct net_device *dev,
			 struct iw_request_info *info,
			 struct iw_point *data, char *essid)
{
	struct ieee80211vap *vap = dev->priv;

	if (vap->iv_opmode == IEEE80211_M_WDS)
		return -EOPNOTSUPP;

	data->flags = 1;		/* active */
	if (vap->iv_opmode == IEEE80211_M_HOSTAP) {
		if (vap->iv_des_nssid > 0) {
			if (data->length > vap->iv_des_ssid[0].len)
				data->length = vap->iv_des_ssid[0].len;
			memcpy(essid, vap->iv_des_ssid[0].ssid, data->length);
		} else {
			data->length = 0;
		}
	} else {
		if (vap->iv_des_nssid == 0) {
			if (data->length > vap->iv_bss->ni_esslen)
				data->length = vap->iv_bss->ni_esslen;
			memcpy(essid, vap->iv_bss->ni_essid, data->length);
		} else {
			if (data->length > vap->iv_des_ssid[0].len)
				data->length = vap->iv_des_ssid[0].len;
			memcpy(essid, vap->iv_des_ssid[0].ssid, data->length);
		}
	}
	return 0;
}

static int
ieee80211_ioctl_giwrange(struct net_device *dev,
			 struct iw_request_info *info,
			 struct iw_point *data, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct ieee80211_node *ni = vap->iv_bss;
	struct iw_range *range = (struct iw_range *) extra;
	struct ieee80211_rateset *rs;
	u_int8_t reported[IEEE80211_CHAN_BYTES];	/* XXX stack usage? */
	int i, r;

	data->length = sizeof(struct iw_range);
	memset(range, 0, sizeof(struct iw_range));

	/* TODO: could fill num_txpower and txpower array with
	 * something; however, there are 128 different values.. */

	range->txpower_capa = IW_TXPOW_DBM;

	if (vap->iv_opmode == IEEE80211_M_STA ||
	    vap->iv_opmode == IEEE80211_M_IBSS) {
		range->min_pmp = 1 * 1024;
		range->max_pmp = 65535 * 1024;
		range->min_pmt = 1 * 1024;
		range->max_pmt = 1000 * 1024;
		range->pmp_flags = IW_POWER_PERIOD;
		range->pmt_flags = IW_POWER_TIMEOUT;
		range->pm_capa = IW_POWER_PERIOD | IW_POWER_TIMEOUT |
			IW_POWER_UNICAST_R | IW_POWER_ALL_R;
	}

	range->we_version_compiled = WIRELESS_EXT;
	range->we_version_source = 13;

	range->retry_capa = IW_RETRY_LIMIT;
	range->retry_flags = IW_RETRY_LIMIT;
	range->min_retry = 0;
	range->max_retry = 255;

	range->num_channels = ic->ic_nchans;

	range->num_frequency = 0;
	memset(reported, 0, sizeof(reported));
	for (i = 0; i < ic->ic_nchans; i++) {
		const struct ieee80211_channel *c = &ic->ic_channels[i];

		/* discard if previously reported (e.g. b/g) */
		if (isclr(reported, c->ic_ieee)) {
			setbit(reported, c->ic_ieee);
			range->freq[range->num_frequency].i = c->ic_ieee;
			range->freq[range->num_frequency].m =
				ic->ic_channels[i].ic_freq * 100000;
			range->freq[range->num_frequency].e = 1;
			if (++range->num_frequency == IW_MAX_FREQUENCIES)
				break;
		}
	}

	/* Max quality is max field value minus noise floor */
	range->max_qual.qual  = 0xff - 161;

	/*
	 * In order to use dBm measurements, 'level' must be lower
	 * than any possible measurement (see iw_print_stats() in
	 * wireless tools).  It's unclear how this is meant to be
	 * done, but setting zero in these values forces dBm and
	 * the actual numbers are not used.
	 */
	range->max_qual.level = 0;
	range->max_qual.noise = 0;

	range->sensitivity = 3;

	range->max_encoding_tokens = IEEE80211_WEP_NKID;
	/* XXX query driver to find out supported key sizes */
	range->num_encoding_sizes = 3;
	range->encoding_size[0] = 5;		/* 40-bit */
	range->encoding_size[1] = 13;		/* 104-bit */
	range->encoding_size[2] = 16;		/* 128-bit */

	/* XXX this only works for station mode */
	rs = &ni->ni_rates;
	range->num_bitrates = rs->rs_nrates;
	if (range->num_bitrates > IW_MAX_BITRATES)
		range->num_bitrates = IW_MAX_BITRATES;
	for (i = 0; i < range->num_bitrates; i++) {
		r = rs->rs_rates[i] & IEEE80211_RATE_VAL;
		range->bitrate[i] = (r * 1000000) / 2;
	}

	/* estimated maximum TCP throughput values (bps) */
	range->throughput = 5500000;

	range->min_rts = 0;
	range->max_rts = 2347;
	range->min_frag = 256;
	range->max_frag = 2346;

	return 0;
}

static int
ieee80211_ioctl_siwmode(struct net_device *dev,
			struct iw_request_info *info,
			__u32 *mode, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ifmediareq imr;
	int valid = 0;
	
	memset(&imr, 0, sizeof(imr));
	vap->iv_media.ifm_status(vap->iv_dev, &imr);
	
	if (imr.ifm_active & IFM_IEEE80211_HOSTAP)
		valid = (*mode == IW_MODE_MASTER);
#if WIRELESS_EXT >= 15
	else if (imr.ifm_active & IFM_IEEE80211_MONITOR)
		valid = (*mode == IW_MODE_MONITOR);
#endif
	else if (imr.ifm_active & IFM_IEEE80211_ADHOC)
		valid = (*mode == IW_MODE_ADHOC);
	else if (imr.ifm_active & IFM_IEEE80211_WDS)
		valid = (*mode == IW_MODE_REPEAT);
	else
		valid = (*mode == IW_MODE_INFRA);
		
	return valid ? 0 : -EINVAL;	
}

static int
ieee80211_ioctl_giwmode(struct net_device *dev,
			struct iw_request_info *info,
			__u32 *mode, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ifmediareq imr;

	memset(&imr, 0, sizeof(imr));
	vap->iv_media.ifm_status(vap->iv_dev, &imr);

	if (imr.ifm_active & IFM_IEEE80211_HOSTAP)
		*mode = IW_MODE_MASTER;
#if WIRELESS_EXT >= 15
	else if (imr.ifm_active & IFM_IEEE80211_MONITOR)
		*mode = IW_MODE_MONITOR;
#endif
	else if (imr.ifm_active & IFM_IEEE80211_ADHOC)
		*mode = IW_MODE_ADHOC;
	else if (imr.ifm_active & IFM_IEEE80211_WDS)
		*mode = IW_MODE_REPEAT;
	else
		*mode = IW_MODE_INFRA;
	return 0;
}

static int
ieee80211_ioctl_siwpower(struct net_device *dev,
			 struct iw_request_info *info,
			 struct iw_param *wrq, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;

	if (wrq->disabled) {
		if (ic->ic_flags & IEEE80211_F_PMGTON) {
			ic->ic_flags &= ~IEEE80211_F_PMGTON;
			goto done;
		}
		return 0;
	}

	if ((ic->ic_caps & IEEE80211_C_PMGT) == 0)
		return -EOPNOTSUPP;
	switch (wrq->flags & IW_POWER_MODE) {
	case IW_POWER_UNICAST_R:
	case IW_POWER_ALL_R:
	case IW_POWER_ON:
		ic->ic_flags |= IEEE80211_F_PMGTON;
		break;
	default:
		return -EINVAL;
	}
	if (wrq->flags & IW_POWER_TIMEOUT) {
		ic->ic_holdover = IEEE80211_MS_TO_TU(wrq->value);
		ic->ic_flags |= IEEE80211_F_PMGTON;
	}
	if (wrq->flags & IW_POWER_PERIOD) {
		ic->ic_lintval = IEEE80211_MS_TO_TU(wrq->value);
		ic->ic_flags |= IEEE80211_F_PMGTON;
	}
done:
	return IS_UP(ic->ic_dev) ? -ic->ic_reset(ic->ic_dev) : 0;
}

static int
ieee80211_ioctl_giwpower(struct net_device *dev,
			struct iw_request_info *info,
			struct iw_param *rrq, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;

	rrq->disabled = (ic->ic_flags & IEEE80211_F_PMGTON) == 0;
	if (!rrq->disabled) {
		switch (rrq->flags & IW_POWER_TYPE) {
		case IW_POWER_TIMEOUT:
			rrq->flags = IW_POWER_TIMEOUT;
			rrq->value = IEEE80211_TU_TO_MS(ic->ic_holdover);
			break;
		case IW_POWER_PERIOD:
			rrq->flags = IW_POWER_PERIOD;
			rrq->value = IEEE80211_TU_TO_MS(ic->ic_lintval);
			break;
		}
		rrq->flags |= IW_POWER_ALL_R;
	}
	return 0;
}

static int
ieee80211_ioctl_siwretry(struct net_device *dev,
			 struct iw_request_info *info,
			 struct iw_param *rrq, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;

	if (rrq->disabled) {
		if (vap->iv_flags & IEEE80211_F_SWRETRY) {
			vap->iv_flags &= ~IEEE80211_F_SWRETRY;
			goto done;
		}
		return 0;
	}

	if ((vap->iv_caps & IEEE80211_C_SWRETRY) == 0)
		return -EOPNOTSUPP;
	if (rrq->flags == IW_RETRY_LIMIT) {
		if (rrq->value >= 0) {
			vap->iv_txmin = rrq->value;
			vap->iv_txmax = rrq->value;	/* XXX */
			vap->iv_txlifetime = 0;		/* XXX */
			vap->iv_flags |= IEEE80211_F_SWRETRY;
		} else {
			vap->iv_flags &= ~IEEE80211_F_SWRETRY;
		}
		return 0;
	}
done:
	return IS_UP(vap->iv_dev) ? -ic->ic_reset(vap->iv_dev) : 0;
}

static int
ieee80211_ioctl_giwretry(struct net_device *dev,
			 struct iw_request_info *info,
			 struct iw_param *rrq, char *extra)
{
	struct ieee80211vap *vap = dev->priv;

	rrq->disabled = (vap->iv_flags & IEEE80211_F_SWRETRY) == 0;
	if (!rrq->disabled) {
		switch (rrq->flags & IW_RETRY_TYPE) {
		case IW_RETRY_LIFETIME:
			rrq->flags = IW_RETRY_LIFETIME;
			rrq->value = IEEE80211_TU_TO_MS(vap->iv_txlifetime);
			break;
		case IW_RETRY_LIMIT:
			rrq->flags = IW_RETRY_LIMIT;
			switch (rrq->flags & IW_RETRY_MODIFIER) {
			case IW_RETRY_MIN:
				rrq->flags |= IW_RETRY_MAX;
				rrq->value = vap->iv_txmin;
				break;
			case IW_RETRY_MAX:
				rrq->flags |= IW_RETRY_MAX;
				rrq->value = vap->iv_txmax;
				break;
			}
			break;
		}
	}
	return 0;
}

static int
ieee80211_ioctl_siwtxpow(struct net_device *dev,
			 struct iw_request_info *info,
			 struct iw_param *rrq, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	int fixed, disabled;

	fixed = (ic->ic_flags & IEEE80211_F_TXPOW_FIXED);
	disabled = (fixed && vap->iv_bss->ni_txpower == 0);
	if (rrq->disabled) {
		if (!disabled) {
			if ((ic->ic_caps & IEEE80211_C_TXPMGT) == 0)
				return -EOPNOTSUPP;
			ic->ic_flags |= IEEE80211_F_TXPOW_FIXED;
			vap->iv_bss->ni_txpower = 0;
			goto done;
		}
		return 0;
	}

	if (rrq->fixed) {
		if ((ic->ic_caps & IEEE80211_C_TXPMGT) == 0)
			return -EOPNOTSUPP;
		if (rrq->flags != IW_TXPOW_DBM)
			return -EOPNOTSUPP;
		vap->iv_bss->ni_txpower = 2*rrq->value;
		ic->ic_flags |= IEEE80211_F_TXPOW_FIXED;
	} else {
		if (!fixed)		/* no change */
			return 0;
		ic->ic_flags &= ~IEEE80211_F_TXPOW_FIXED;
	}
done:
	return IS_UP(ic->ic_dev) ? -ic->ic_reset(ic->ic_dev) : 0;
}

static int
ieee80211_ioctl_giwtxpow(struct net_device *dev,
			 struct iw_request_info *info,
			 struct iw_param *rrq, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;

	rrq->value = vap->iv_bss->ni_txpower/2;
	rrq->fixed = (ic->ic_flags & IEEE80211_F_TXPOW_FIXED) != 0;
	rrq->disabled = (rrq->fixed && rrq->value == 0);
	rrq->flags = IW_TXPOW_DBM;
	return 0;
}

struct waplistreq {
	struct ieee80211vap *vap;
	struct sockaddr addr[IW_MAX_AP];
	struct iw_quality qual[IW_MAX_AP];
	int	i;
};

static void
waplist_cb(void *arg, const struct ieee80211_scan_entry *se)
{
	struct waplistreq *req = arg;
	int i = req->i;

	if (i >= IW_MAX_AP)
		return;
	req->addr[i].sa_family = ARPHRD_ETHER;
	if (req->vap->iv_opmode == IEEE80211_M_HOSTAP)
		IEEE80211_ADDR_COPY(req->addr[i].sa_data, se->se_macaddr);
	else
		IEEE80211_ADDR_COPY(req->addr[i].sa_data, se->se_bssid);
	set_quality(&req->qual[i], se->se_rssi);
	req->i = i+1;
}

static int
ieee80211_ioctl_iwaplist(struct net_device *dev,
			struct iw_request_info *info,
			struct iw_point *data, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct waplistreq req;		/* XXX off stack */

	req.vap = vap;
	req.i = 0;
	ieee80211_scan_iterate(ic, waplist_cb, &req);

	data->length = req.i;
	memcpy(extra, &req.addr, req.i*sizeof(req.addr[0]));
	data->flags = 1;		/* signal quality present (sort of) */
	memcpy(extra + req.i*sizeof(req.addr[0]), &req.qual,
		req.i*sizeof(req.qual[0]));

	return 0;

}

#ifdef SIOCGIWSCAN
static int
ieee80211_ioctl_siwscan(struct net_device *dev,
			struct iw_request_info *info,
			struct iw_point *data, char *extra)
{
	struct ieee80211vap *vap = dev->priv;

	/*
	 * XXX don't permit a scan to be started unless we
	 * know the device is ready.  For the moment this means
	 * the device is marked up as this is the required to
	 * initialize the hardware.  It would be better to permit
	 * scanning prior to being up but that'll require some
	 * changes to the infrastructure.
	 */
	if (!IS_UP(vap->iv_dev))
		return -EINVAL;		/* XXX */
	/* XXX always manual... */
	IEEE80211_DPRINTF(vap, IEEE80211_MSG_SCAN,
	    "%s: active scan request\n", __func__);
	(void) ieee80211_start_scan(vap,
			IEEE80211_SCAN_ACTIVE |
			IEEE80211_SCAN_NOPICK |
			IEEE80211_SCAN_ONCE, IEEE80211_SCAN_FOREVER,
			/* XXX use ioctl params */
			vap->iv_des_nssid, vap->iv_des_ssid);
	return 0;
}

#if WIRELESS_EXT > 14
/*
 * Encode a WPA or RSN information element as a custom
 * element using the hostap format.
 */
static u_int
encode_ie(void *buf, size_t bufsize,
	const u_int8_t *ie, size_t ielen,
	const char *leader, size_t leader_len)
{
	u_int8_t *p;
	int i;

	if (bufsize < leader_len)
		return 0;
	p = buf;
	memcpy(p, leader, leader_len);
	bufsize -= leader_len;
	p += leader_len;
	for (i = 0; i < ielen && bufsize > 2; i++)
		p += sprintf(p, "%02x", ie[i]);
	return (i == ielen ? p - (u_int8_t *)buf : 0);
}
#endif /* WIRELESS_EXT > 14 */

struct iwscanreq {
	struct ieee80211vap	*vap;
	char			*current_ev;
	char			*end_buf;
	int			mode;
};

static void
giwscan_cb(void *arg, const struct ieee80211_scan_entry *se)
{
	struct iwscanreq *req = arg;
	struct ieee80211vap *vap = req->vap;
	char *current_ev = req->current_ev;
	char *end_buf = req->end_buf;
#if WIRELESS_EXT > 14
	char buf[64*2 + 30];
#endif
	struct iw_event iwe;
	char *current_val;
	int j;

	if (current_ev >= end_buf)
		return;
	/* WPA/!WPA sort criteria */
	if ((req->mode != 0) ^ (se->se_wpa_ie != NULL))
		return;

	memset(&iwe, 0, sizeof(iwe));
	iwe.cmd = SIOCGIWAP;
	iwe.u.ap_addr.sa_family = ARPHRD_ETHER;
	if (vap->iv_opmode == IEEE80211_M_HOSTAP)
		IEEE80211_ADDR_COPY(iwe.u.ap_addr.sa_data, se->se_macaddr);
	else
		IEEE80211_ADDR_COPY(iwe.u.ap_addr.sa_data, se->se_bssid);
	current_ev = iwe_stream_add_event(current_ev,
		end_buf, &iwe, IW_EV_ADDR_LEN);

	memset(&iwe, 0, sizeof(iwe));
	iwe.cmd = SIOCGIWESSID;
	iwe.u.data.flags = 1;
	if (vap->iv_opmode == IEEE80211_M_HOSTAP) {
		iwe.u.data.length = vap->iv_des_nssid > 0 ?
			vap->iv_des_ssid[0].len : 0;
		current_ev = iwe_stream_add_point(current_ev,
				end_buf, &iwe, vap->iv_des_ssid[0].ssid);
	} else {
		iwe.u.data.length = se->se_ssid[1];
		current_ev = iwe_stream_add_point(current_ev,
				end_buf, &iwe, (char *) se->se_ssid+2);
	}

	if (se->se_capinfo & (IEEE80211_CAPINFO_ESS|IEEE80211_CAPINFO_IBSS)) {
		memset(&iwe, 0, sizeof(iwe));
		iwe.cmd = SIOCGIWMODE;
		iwe.u.mode = se->se_capinfo & IEEE80211_CAPINFO_ESS ?
			IW_MODE_MASTER : IW_MODE_ADHOC;
		current_ev = iwe_stream_add_event(current_ev,
				end_buf, &iwe, IW_EV_UINT_LEN);
	}

	memset(&iwe, 0, sizeof(iwe));
	iwe.cmd = SIOCGIWFREQ;
	iwe.u.freq.m = se->se_chan->ic_freq * 100000;
	iwe.u.freq.e = 1;
	current_ev = iwe_stream_add_event(current_ev,
			end_buf, &iwe, IW_EV_FREQ_LEN);

	memset(&iwe, 0, sizeof(iwe));
	iwe.cmd = IWEVQUAL;
	set_quality(&iwe.u.qual, se->se_rssi);
	current_ev = iwe_stream_add_event(current_ev,
		end_buf, &iwe, IW_EV_QUAL_LEN);

	memset(&iwe, 0, sizeof(iwe));
	iwe.cmd = SIOCGIWENCODE;
	if (se->se_capinfo & IEEE80211_CAPINFO_PRIVACY)
		iwe.u.data.flags = IW_ENCODE_ENABLED | IW_ENCODE_NOKEY;
	else
		iwe.u.data.flags = IW_ENCODE_DISABLED;
	iwe.u.data.length = 0;
	current_ev = iwe_stream_add_point(current_ev, end_buf, &iwe, "");

	memset(&iwe, 0, sizeof(iwe));
	iwe.cmd = SIOCGIWRATE;
	current_val = current_ev + IW_EV_LCP_LEN;
	/* NB: not sorted, does it matter? */
	for (j = 0; j < se->se_rates[1]; j++) {
		int r = se->se_rates[2+j] & IEEE80211_RATE_VAL;
		if (r != 0) {
			iwe.u.bitrate.value = r * (1000000 / 2);
			current_val = iwe_stream_add_value(current_ev,
				current_val, end_buf, &iwe,
				IW_EV_PARAM_LEN);
		}
	}
	for (j = 0; j < se->se_xrates[1]; j++) {
		int r = se->se_xrates[2+j] & IEEE80211_RATE_VAL;
		if (r != 0) {
			iwe.u.bitrate.value = r * (1000000 / 2);
			current_val = iwe_stream_add_value(current_ev,
				current_val, end_buf, &iwe,
				IW_EV_PARAM_LEN);
		}
	}
	/* remove fixed header if no rates were added */
	if ((current_val - current_ev) > IW_EV_LCP_LEN)
		current_ev = current_val;

#if WIRELESS_EXT > 14
	memset(&iwe, 0, sizeof(iwe));
	iwe.cmd = IWEVCUSTOM;
	snprintf(buf, sizeof(buf), "bcn_int=%d", se->se_intval);
	iwe.u.data.length = strlen(buf);
	current_ev = iwe_stream_add_point(current_ev, end_buf, &iwe, buf);

	if (se->se_wpa_ie != NULL) {
		static const char rsn_leader[] = "rsn_ie=";
		static const char wpa_leader[] = "wpa_ie=";

		memset(&iwe, 0, sizeof(iwe));
		iwe.cmd = IWEVCUSTOM;
		if (se->se_wpa_ie[0] == IEEE80211_ELEMID_RSN)
			iwe.u.data.length = encode_ie(buf, sizeof(buf),
				se->se_wpa_ie, se->se_wpa_ie[1]+2,
				rsn_leader, sizeof(rsn_leader)-1);
		else
			iwe.u.data.length = encode_ie(buf, sizeof(buf),
				se->se_wpa_ie, se->se_wpa_ie[1]+2,
				wpa_leader, sizeof(wpa_leader)-1);
		if (iwe.u.data.length != 0)
			current_ev = iwe_stream_add_point(current_ev, end_buf,
				&iwe, buf);
	}
	if (se->se_wme_ie != NULL) {
		static const char wme_leader[] = "wme_ie=";

		memset(&iwe, 0, sizeof(iwe));
		iwe.cmd = IWEVCUSTOM;
		iwe.u.data.length = encode_ie(buf, sizeof(buf),
			se->se_wme_ie, se->se_wme_ie[1]+2,
			wme_leader, sizeof(wme_leader)-1);
		if (iwe.u.data.length != 0)
			current_ev = iwe_stream_add_point(current_ev, end_buf,
				&iwe, buf);
	}
	if (se->se_ath_ie != NULL) {
		static const char ath_leader[] = "ath_ie=";

		memset(&iwe, 0, sizeof(iwe));
		iwe.cmd = IWEVCUSTOM;
		iwe.u.data.length = encode_ie(buf, sizeof(buf),
			se->se_ath_ie, se->se_ath_ie[1]+2,
			ath_leader, sizeof(ath_leader)-1);
		if (iwe.u.data.length != 0)
			current_ev = iwe_stream_add_point(current_ev, end_buf,
				&iwe, buf);
	}
#endif /* WIRELESS_EXT > 14 */
	req->current_ev = current_ev;
}

static int
ieee80211_ioctl_giwscan(struct net_device *dev,
			struct iw_request_info *info,
			struct iw_point *data, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct iwscanreq req;

	req.vap = vap;
	req.current_ev = extra;
	req.end_buf = extra + IW_SCAN_MAX_DATA;

	/*
	 * Do two passes to insure WPA/non-WPA scan candidates
	 * are sorted to the front.  This is a hack to deal with
	 * the wireless extensions capping scan results at
	 * IW_SCAN_MAX_DATA bytes.  In densely populated environments
	 * it's easy to overflow this buffer (especially with WPA/RSN
	 * information elements).  Note this sorting hack does not
	 * guarantee we won't overflow anyway.
	 */
	req.mode = vap->iv_flags & IEEE80211_F_WPA;
	ieee80211_scan_iterate(ic, giwscan_cb, &req);
	req.mode = req.mode ? 0 : IEEE80211_F_WPA;
	ieee80211_scan_iterate(ic, giwscan_cb, &req);

	data->length = req.current_ev - extra;
	return 0;
}
#endif /* SIOCGIWSCAN */

static int
cipher2cap(int cipher)
{
	switch (cipher) {
	case IEEE80211_CIPHER_WEP:	return IEEE80211_C_WEP;
	case IEEE80211_CIPHER_AES_OCB:	return IEEE80211_C_AES;
	case IEEE80211_CIPHER_AES_CCM:	return IEEE80211_C_AES_CCM;
	case IEEE80211_CIPHER_CKIP:	return IEEE80211_C_CKIP;
	case IEEE80211_CIPHER_TKIP:	return IEEE80211_C_TKIP;
	}
	return 0;
}

#define	IEEE80211_MODE_TURBO_STATIC_A	IEEE80211_MODE_MAX

static int
ieee80211_convert_mode(const char *mode)
{
#define TOUPPER(c) ((((c) > 0x60) && ((c) < 0x7b)) ? ((c) - 0x20) : (c))
	static const struct {
		char *name;
		int mode;
	} mappings[] = {
		/* NB: need to order longest strings first for overlaps */ 
		{ "11AST" , IEEE80211_MODE_TURBO_STATIC_A },
		{ "AUTO"  , IEEE80211_MODE_AUTO },
		{ "11A"   , IEEE80211_MODE_11A },
		{ "11B"   , IEEE80211_MODE_11B },
		{ "11G"   , IEEE80211_MODE_11G },
		{ "FH"    , IEEE80211_MODE_FH },
		{ "0"     , IEEE80211_MODE_AUTO },
		{ "1"     , IEEE80211_MODE_11A },
		{ "2"     , IEEE80211_MODE_11B },
		{ "3"     , IEEE80211_MODE_11G },
		{ "4"     , IEEE80211_MODE_FH },
		{ "5"     , IEEE80211_MODE_TURBO_STATIC_A },
		{ NULL }
	};
	int i, j;
	const char *cp;

	for (i = 0; mappings[i].name != NULL; i++) {
		cp = mappings[i].name;
		for (j = 0; j < strlen(mode) + 1; j++) {
			/* convert user-specified string to upper case */
			if (TOUPPER(mode[j]) != cp[j])
				break;
			if (cp[j] == '\0')
				return mappings[i].mode;
		}
	}
	return -1;
#undef TOUPPER
}

static int
ieee80211_ioctl_setmode(struct net_device *dev, struct iw_request_info *info,
		   	 void *w, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct iw_point *wri;
	struct ifreq ifr;
	char s[6];		/* big enough for ``11adt'' */
	int retv,mode,ifr_mode;

	if (ic->ic_media.ifm_cur == NULL)
		return -EINVAL;
	wri = (struct iw_point *) extra;
	if (wri->length > sizeof(s))		/* silently truncate */
		wri->length = sizeof(s);
	if (copy_from_user(s, wri->pointer, wri->length))
		return -EINVAL;
	s[sizeof(s)-1] = '\0';			/* insure null termination */
	mode = ieee80211_convert_mode(s);
	if (mode < 0)
		return -EINVAL;

	if(ieee80211_check_mode_consistency(ic,mode,vap->iv_des_chan)) { 
		/*
		 * error in AP mode.
		 * overwrite channel selection in other modes.
		 */
		if (vap->iv_opmode == IEEE80211_M_HOSTAP)
			return -EINVAL;
		else
			vap->iv_des_chan=IEEE80211_CHAN_ANYC;
	}

	ifr_mode = mode;
	memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_media = ic->ic_media.ifm_cur->ifm_media &~ IFM_MMASK;
	if (mode ==  IEEE80211_MODE_TURBO_STATIC_A) {
			ifr_mode =  IEEE80211_MODE_11A;
	} 
	ifr.ifr_media |= IFM_MAKEMODE(ifr_mode);
	retv = ifmedia_ioctl(ic->ic_dev, &ifr, &ic->ic_media, SIOCSIFMEDIA);
	if ((!retv || retv == ENETRESET) &&  mode != vap->iv_des_mode) {
		ieee80211_scan_flush(ic);	/* NB: could optimize */
		vap->iv_des_mode = mode;
		if (IS_UP_AUTO(vap))
			ieee80211_new_state(vap, IEEE80211_S_SCAN, 0);
		retv = 0;
	}
#ifdef ATH_SUPERG_XR
	/* set the same params on the xr vap device if exists */
	if(vap->iv_xrvap && !(vap->iv_flags & IEEE80211_F_XR) ) {
		vap->iv_xrvap->iv_des_mode = mode;
	}
#endif
	return -retv;
}
#undef IEEE80211_MODE_TURBO_STATIC_A

#ifdef ATH_SUPERG_XR
static void
ieee80211_setupxr(struct ieee80211vap *vap)
{
	struct net_device *dev = vap->iv_dev;
	struct ieee80211com *ic = vap->iv_ic;
	if((vap->iv_opmode == IEEE80211_M_HOSTAP)  &&
	   !(vap->iv_flags & IEEE80211_F_XR) ) {
		if((vap->iv_ath_cap  & IEEE80211_ATHC_XR) && !vap->iv_xrvap ) { 
			char name[IFNAMSIZ];
			strcpy(name,dev->name);
			strcat(name,"-xr");
			/*
			 * create a new XR vap. if the normal VAP is already up , bring
			 * up the XR vap aswell.
			 */
			ieee80211_scan_flush(ic);	/* NB: could optimize */
			 
			vap->iv_xrvap = ic->ic_vap_create(ic, name, vap->iv_unit,
											  IEEE80211_M_HOSTAP,IEEE80211_VAP_XR | IEEE80211_CLONE_BSSID);
			vap->iv_xrvap->iv_fragthreshold = IEEE80211_XR_FRAG_THRESHOLD;
			copy_des_ssid(vap->iv_xrvap, vap);
			vap->iv_xrvap->iv_des_mode = vap->iv_des_mode;
		} else if(!(vap->iv_ath_cap  & IEEE80211_ATHC_XR) && vap->iv_xrvap ) { 
			/*
			 * destroy the XR vap. if the XR VAP is up , bring
			 * it down before destroying.
			 */
			if(vap->iv_xrvap) {
				ieee80211_stop(vap->iv_xrvap->iv_dev);
				ic->ic_vap_delete(vap->iv_xrvap);
			}
			vap->iv_xrvap = NULL;
		}
	}
}
#endif

static int
ieee80211_setathcap(struct ieee80211vap *vap, int cap, int setting)
{
	struct ieee80211com *ic = vap->iv_ic;
	int ocap;

	if ((ic->ic_ath_cap & cap) == 0)
		return -EINVAL;
	ocap = vap->iv_ath_cap;
	if (setting)
		vap->iv_ath_cap |= cap;
	else
		vap->iv_ath_cap &= ~cap;
	ic->ic_athcapsetup(vap->iv_ic, vap->iv_ath_cap);
	return (vap->iv_ath_cap != ocap ? ENETRESET : 0);
}

static int
ieee80211_set_turbo(struct net_device *dev, int flag)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct ifreq ifr;
	struct ieee80211vap *tmpvap = dev->priv;
	int nvap=0;

	TAILQ_FOREACH(tmpvap, &ic->ic_vaps, iv_next)
			nvap++;
	if(nvap > 1 && flag ) return -EINVAL;
	ifr.ifr_media = ic->ic_media.ifm_cur->ifm_media &~ IFM_MMASK;
	if(flag)
		ifr.ifr_media |= IFM_IEEE80211_TURBO;
	else
		ifr.ifr_media &= ~IFM_IEEE80211_TURBO;
	(void) ifmedia_ioctl(ic->ic_dev, &ifr, &ic->ic_media, SIOCSIFMEDIA);

	return 0;
    
}

static int
ieee80211_ioctl_setparam(struct net_device *dev, struct iw_request_info *info,
		   	 void *w, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct ieee80211_rsnparms *rsn = &vap->iv_bss->ni_rsn;
	int *i = (int *) extra;
	int param = i[0];		/* parameter id is 1st */
	int value = i[1];		/* NB: most values are TYPE_INT */
	int retv = 0;
	int j, caps;
	const struct ieee80211_authenticator *auth;
	const struct ieee80211_aclator *acl;

	switch (param) {
	case IEEE80211_PARAM_AUTHMODE:
		switch (value) {
		case IEEE80211_AUTH_WPA:	/* WPA */
		case IEEE80211_AUTH_8021X:	/* 802.1x */
		case IEEE80211_AUTH_OPEN:	/* open */
		case IEEE80211_AUTH_SHARED:	/* shared-key */
		case IEEE80211_AUTH_AUTO:	/* auto */
			auth = ieee80211_authenticator_get(value);
			if (auth == NULL)
				return -EINVAL;
			break;
		default:
			return -EINVAL;
		}
		switch (value) {
		case IEEE80211_AUTH_WPA:	/* WPA w/ 802.1x */
			vap->iv_flags |= IEEE80211_F_PRIVACY;
			value = IEEE80211_AUTH_8021X;
			break;
		case IEEE80211_AUTH_OPEN:	/* open */
			vap->iv_flags &= ~(IEEE80211_F_WPA|IEEE80211_F_PRIVACY);
			break;
		case IEEE80211_AUTH_SHARED:	/* shared-key */
		case IEEE80211_AUTH_AUTO:	/* auto */
		case IEEE80211_AUTH_8021X:	/* 802.1x */
			vap->iv_flags &= ~IEEE80211_F_WPA;
			/* both require a key so mark the PRIVACY capability */
			vap->iv_flags |= IEEE80211_F_PRIVACY;
			break;
		}
		/* NB: authenticator attach/detach happens on state change */
		vap->iv_bss->ni_authmode = value;
		/* XXX mixed/mode/usage? */
		vap->iv_auth = auth;
		retv = ENETRESET;
		break;
	case IEEE80211_PARAM_PROTMODE:
		if (value > IEEE80211_PROT_RTSCTS)
			return -EINVAL;
		ic->ic_protmode = value;
		/* NB: if not operating in 11g this can wait */
		if (ic->ic_bsschan != IEEE80211_CHAN_ANYC &&
		    IEEE80211_IS_CHAN_ANYG(ic->ic_bsschan))
			retv = ENETRESET;
		break;
	case IEEE80211_PARAM_MCASTCIPHER:
		if ((vap->iv_caps & cipher2cap(value)) == 0 &&
		    !ieee80211_crypto_available(value))
			return -EINVAL;
		rsn->rsn_mcastcipher = value;
		if (vap->iv_flags & IEEE80211_F_WPA)
			retv = ENETRESET;
		break;
	case IEEE80211_PARAM_MCASTKEYLEN:
		if (!(0 < value && value < IEEE80211_KEYBUF_SIZE))
			return -EINVAL;
		/* XXX no way to verify driver capability */
		rsn->rsn_mcastkeylen = value;
		if (vap->iv_flags & IEEE80211_F_WPA)
			retv = ENETRESET;
		break;
	case IEEE80211_PARAM_UCASTCIPHERS:
		/*
		 * Convert cipher set to equivalent capabilities.
		 * NB: this logic intentionally ignores unknown and
		 * unsupported ciphers so folks can specify 0xff or
		 * similar and get all available ciphers.
		 */
		caps = 0;
		for (j = 1; j < 32; j++)	/* NB: skip WEP */
			if ((value & (1<<j)) &&
			    ((vap->iv_caps & cipher2cap(j)) ||
			     ieee80211_crypto_available(j)))
				caps |= 1<<j;
		if (caps == 0)			/* nothing available */
			return -EINVAL;
		/* XXX verify ciphers ok for unicast use? */
		/* XXX disallow if running as it'll have no effect */
		rsn->rsn_ucastcipherset = caps;
		if (vap->iv_flags & IEEE80211_F_WPA)
			retv = ENETRESET;
		break;
	case IEEE80211_PARAM_UCASTCIPHER:
		if ((rsn->rsn_ucastcipherset & cipher2cap(value)) == 0)
			return -EINVAL;
		rsn->rsn_ucastcipher = value;
		break;
	case IEEE80211_PARAM_UCASTKEYLEN:
		if (!(0 < value && value < IEEE80211_KEYBUF_SIZE))
			return -EINVAL;
		/* XXX no way to verify driver capability */
		rsn->rsn_ucastkeylen = value;
		break;
	case IEEE80211_PARAM_KEYMGTALGS:
		/* XXX check */
		rsn->rsn_keymgmtset = value;
		if (vap->iv_flags & IEEE80211_F_WPA)
			retv = ENETRESET;
		break;
	case IEEE80211_PARAM_RSNCAPS:
		/* XXX check */
		rsn->rsn_caps = value;
		if (vap->iv_flags & IEEE80211_F_WPA)
			retv = ENETRESET;
		break;
	case IEEE80211_PARAM_WPA:
		if (value > 3)
			return -EINVAL;
		/* XXX verify ciphers available */
		vap->iv_flags &= ~IEEE80211_F_WPA;
		switch (value) {
		case 1:
			vap->iv_flags |= IEEE80211_F_WPA1;
			break;
		case 2:
			vap->iv_flags |= IEEE80211_F_WPA2;
			break;
		case 3:
			vap->iv_flags |= IEEE80211_F_WPA1 | IEEE80211_F_WPA2;
			break;
		}
		retv = ENETRESET;		/* XXX? */
		break;
	case IEEE80211_PARAM_ROAMING:
		if (!(IEEE80211_ROAMING_DEVICE <= value &&
		    value <= IEEE80211_ROAMING_MANUAL))
			return -EINVAL;
		ic->ic_roaming = value;
		break;
	case IEEE80211_PARAM_PRIVACY:
		if (value) {
			/* XXX check for key state? */
			vap->iv_flags |= IEEE80211_F_PRIVACY;
		} else
			vap->iv_flags &= ~IEEE80211_F_PRIVACY;
		break;
	case IEEE80211_PARAM_DROPUNENCRYPTED:
		if (value)
			vap->iv_flags |= IEEE80211_F_DROPUNENC;
		else
			vap->iv_flags &= ~IEEE80211_F_DROPUNENC;
		break;
	case IEEE80211_PARAM_COUNTERMEASURES:
		if (value) {
			if ((vap->iv_flags & IEEE80211_F_WPA) == 0)
				return -EINVAL;
			vap->iv_flags |= IEEE80211_F_COUNTERM;
		} else
			vap->iv_flags &= ~IEEE80211_F_COUNTERM;
		break;
	case IEEE80211_PARAM_DRIVER_CAPS:
		vap->iv_caps = value;		/* NB: for testing */
		break;
	case IEEE80211_PARAM_MACCMD:
		acl = vap->iv_acl;
		switch (value) {
		case IEEE80211_MACCMD_POLICY_OPEN:
		case IEEE80211_MACCMD_POLICY_ALLOW:
		case IEEE80211_MACCMD_POLICY_DENY:
			if (acl == NULL) {
				acl = ieee80211_aclator_get("mac");
				if (acl == NULL || !acl->iac_attach(vap))
					return -EINVAL;
				vap->iv_acl = acl;
			}
			acl->iac_setpolicy(vap, value);
			break;
		case IEEE80211_MACCMD_FLUSH:
			if (acl != NULL)
				acl->iac_flush(vap);
			/* NB: silently ignore when not in use */
			break;
		case IEEE80211_MACCMD_DETACH:
			if (acl != NULL) {
				vap->iv_acl = NULL;
				acl->iac_detach(vap);
			}
			break;
		}
		break;
	case IEEE80211_PARAM_WMM:
		if (ic->ic_caps & IEEE80211_C_WME){
			if (value) {
				vap->iv_flags |= IEEE80211_F_WME;
				vap->iv_ic->ic_flags |= IEEE80211_F_WME; /* XXX needed by ic_reset */
			}
			else {
				vap->iv_flags &= ~IEEE80211_F_WME;
				vap->iv_ic->ic_flags &= ~IEEE80211_F_WME; /* XXX needed by ic_reset */
			}
			retv = ENETRESET;	/* Renegotiate for capabilities */
		}
		break;
	case IEEE80211_PARAM_HIDESSID:
		if (value)
			vap->iv_flags |= IEEE80211_F_HIDESSID;
		else
			vap->iv_flags &= ~IEEE80211_F_HIDESSID;
		retv = ENETRESET;
		break;
	case IEEE80211_PARAM_APBRIDGE:
		if (value == 0)
			vap->iv_flags |= IEEE80211_F_NOBRIDGE;
		else
			vap->iv_flags &= ~IEEE80211_F_NOBRIDGE;
		break;
	case IEEE80211_PARAM_INACT:
		vap->iv_inact_run = value / IEEE80211_INACT_WAIT;
		break;
	case IEEE80211_PARAM_INACT_AUTH:
		vap->iv_inact_auth = value / IEEE80211_INACT_WAIT;
		break;
	case IEEE80211_PARAM_INACT_INIT:
		vap->iv_inact_init = value / IEEE80211_INACT_WAIT;
		break;
	case IEEE80211_PARAM_ABOLT:
		caps = 0;
		/*
		 * Map abolt settings to capability bits;
		 * this also strips unknown/unwanted bits.
		 */
		if (value & IEEE80211_ABOLT_TURBO_PRIME)
			caps |= IEEE80211_ATHC_TURBOP;
		if (value & IEEE80211_ABOLT_COMPRESSION)
			caps |= IEEE80211_ATHC_COMP;
		if (value & IEEE80211_ABOLT_FAST_FRAME)
			caps |= IEEE80211_ATHC_FF;
		if (value & IEEE80211_ABOLT_XR)
			caps |= IEEE80211_ATHC_XR;
		if (value & IEEE80211_ABOLT_AR)
			caps |= IEEE80211_ATHC_AR;
		if (value & IEEE80211_ABOLT_BURST)
			caps |= IEEE80211_ATHC_BURST;
        if (value & IEEE80211_ABOLT_WME_ELE)
            caps |= IEEE80211_ATHC_WME;
		/* verify requested capabilities are supported */
		if ((caps & ic->ic_ath_cap) != caps)
			return -EINVAL;
		if (vap->iv_ath_cap != caps) {
			if ((vap->iv_ath_cap ^ caps) & IEEE80211_ATHC_TURBOP) {
				if (ieee80211_set_turbo(dev,  caps & IEEE80211_ATHC_TURBOP))
					return -EINVAL;
				ieee80211_scan_flush(ic);
			}
			vap->iv_ath_cap = caps;
#ifdef ATH_SUPERG_XR
			ieee80211_setupxr(vap);
#endif
			ic->ic_athcapsetup(vap->iv_ic, vap->iv_ath_cap);
			retv = ENETRESET;
		}
		break;
	case IEEE80211_PARAM_DTIM_PERIOD:
		if (vap->iv_opmode != IEEE80211_M_HOSTAP &&
		    vap->iv_opmode != IEEE80211_M_IBSS)
			return -EINVAL;
		if (IEEE80211_DTIM_MIN <= value &&
		    value <= IEEE80211_DTIM_MAX) {
			vap->iv_dtim_period = value;
			retv = ENETRESET;		/* requires restart */
		} else
			retv = EINVAL;
		break;
	case IEEE80211_PARAM_BEACON_INTERVAL:
		if (vap->iv_opmode != IEEE80211_M_HOSTAP &&
		    vap->iv_opmode != IEEE80211_M_IBSS)
			return -EINVAL;
		if (IEEE80211_BINTVAL_MIN <= value &&
		    value <= IEEE80211_BINTVAL_MAX) {
			ic->ic_lintval = value;		/* XXX multi-bss */
			retv = ENETRESET;		/* requires restart */
		} else
			retv = EINVAL;
		break;
	case IEEE80211_PARAM_DOTH:
		if (value) {
			ic->ic_flags |= IEEE80211_F_DOTH;
		}
		else
			ic->ic_flags &= ~IEEE80211_F_DOTH;
		retv = ENETRESET;	/* XXX: need something this drastic? */
		break;
	case IEEE80211_PARAM_PWRTARGET:
		ic->ic_curchanmaxpwr = value;
		break;
	case IEEE80211_PARAM_GENREASSOC:
		IEEE80211_SEND_MGMT(vap->iv_bss, IEEE80211_FC0_SUBTYPE_REASSOC_REQ, 0);
		break;
	case IEEE80211_PARAM_COMPRESSION:
		retv = ieee80211_setathcap(vap, IEEE80211_ATHC_COMP, value);
		break;
    case IEEE80211_PARAM_WMM_AGGRMODE:
        retv = ieee80211_setathcap(vap, IEEE80211_ATHC_WME, value);
        break;
	case IEEE80211_PARAM_FF:
		retv = ieee80211_setathcap(vap, IEEE80211_ATHC_FF, value);
		break;
	case IEEE80211_PARAM_TURBO:
		retv = ieee80211_setathcap(vap, IEEE80211_ATHC_TURBOP, value);
		if (retv == ENETRESET) {
			if(ieee80211_set_turbo(dev,value))
					return -EINVAL;
			ieee80211_scan_flush(ic);
		}
		break;
	case IEEE80211_PARAM_XR:
		retv = ieee80211_setathcap(vap, IEEE80211_ATHC_XR, value);
#ifdef ATH_SUPERG_XR
		ieee80211_setupxr(vap);
#endif
		break;
	case IEEE80211_PARAM_BURST:
		retv = ieee80211_setathcap(vap, IEEE80211_ATHC_BURST, value);
		break;
	case IEEE80211_PARAM_AR:
		retv = ieee80211_setathcap(vap, IEEE80211_ATHC_AR, value);
		break;
	case IEEE80211_PARAM_PUREG:
		if (value)
			vap->iv_flags |= IEEE80211_F_PUREG;
		else
			vap->iv_flags &= ~IEEE80211_F_PUREG;
		/* NB: reset only if we're operating on an 11g channel */
		if (ic->ic_bsschan != IEEE80211_CHAN_ANYC &&
		    IEEE80211_IS_CHAN_ANYG(ic->ic_bsschan))
			retv = ENETRESET;
		break;
	case IEEE80211_PARAM_WDS:
		if (value)
			vap->iv_flags_ext |= IEEE80211_FEXT_WDS;
		else
			vap->iv_flags_ext &= ~IEEE80211_FEXT_WDS;
		break;
	case IEEE80211_PARAM_BGSCAN:
		if (value) {
			if ((vap->iv_caps & IEEE80211_C_BGSCAN) == 0)
				return -EINVAL;
			vap->iv_flags |= IEEE80211_F_BGSCAN;
		} else {
			/* XXX racey? */
			vap->iv_flags &= ~IEEE80211_F_BGSCAN;
			ieee80211_cancel_scan(vap);	/* anything current */
		}
		break;
	case IEEE80211_PARAM_BGSCAN_IDLE:
		if (value >= IEEE80211_BGSCAN_IDLE_MIN)
			vap->iv_bgscanidle = value*HZ/1000;
		else
			retv = EINVAL;
		break;
	case IEEE80211_PARAM_BGSCAN_INTERVAL:
		if (value >= IEEE80211_BGSCAN_INTVAL_MIN)
			vap->iv_bgscanintvl = value*HZ;
		else
			retv = EINVAL;
		break;
	case IEEE80211_PARAM_MCAST_RATE:
		/* units are in KILObits per second */
		if (value >= 256 && value <= 54000)
			vap->iv_mcast_rate = value;
		else
			retv = EINVAL;
		break;
	case IEEE80211_PARAM_COVERAGE_CLASS:
		if (value >= 0 && value <= IEEE80211_COVERAGE_CLASS_MAX) {
			ic->ic_coverageclass = value;
			if (IS_UP_AUTO(vap))
				ieee80211_new_state(vap, IEEE80211_S_SCAN, 0);
			retv = 0;
		}
		else
			retv = EINVAL;
		break;
	case IEEE80211_PARAM_COUNTRY_IE:
		if (value)
			ic->ic_flags_ext |= IEEE80211_FEXT_COUNTRYIE;
		else
			ic->ic_flags_ext &= ~IEEE80211_FEXT_COUNTRYIE;
		retv = ENETRESET;
		break;
	case IEEE80211_PARAM_REGCLASS:
		if (value)
			ic->ic_flags_ext |= IEEE80211_FEXT_REGCLASS;
		else
			ic->ic_flags_ext &= ~IEEE80211_FEXT_REGCLASS;
		retv = ENETRESET;
		break;
	case IEEE80211_PARAM_SCANVALID:
		vap->iv_scanvalid = value*HZ;
		break;
	case IEEE80211_PARAM_ROAM_RSSI_11A:
		vap->iv_roam.rssi11a = value;
		break;
	case IEEE80211_PARAM_ROAM_RSSI_11B:
		vap->iv_roam.rssi11bOnly = value;
		break;
	case IEEE80211_PARAM_ROAM_RSSI_11G:
		vap->iv_roam.rssi11b = value;
		break;
	case IEEE80211_PARAM_ROAM_RATE_11A:
		vap->iv_roam.rate11a = value;
		break;
	case IEEE80211_PARAM_ROAM_RATE_11B:
		vap->iv_roam.rate11bOnly = value;
		break;
	case IEEE80211_PARAM_ROAM_RATE_11G:
		vap->iv_roam.rate11b = value;
		break;
	case IEEE80211_PARAM_UAPSDINFO:
		if (vap->iv_opmode == IEEE80211_M_HOSTAP) {
			if (ic->ic_caps & IEEE80211_C_UAPSD) {
				if (value)
					IEEE80211_VAP_UAPSD_ENABLE(vap);
				else
					IEEE80211_VAP_UAPSD_DISABLE(vap);
				retv = ENETRESET;
			}
		}
		else if (vap->iv_opmode == IEEE80211_M_STA) {
			vap->iv_uapsdinfo = value;
			IEEE80211_VAP_UAPSD_ENABLE(vap);
			retv = ENETRESET;
		}
		break;
	case IEEE80211_PARAM_SLEEP:
		/* XXX: Forced sleep for testing. Does not actually place the
		 *      HW in sleep mode yet. this only makes sense for STAs.
		 */
		if (value) {
			/* goto sleep */
			IEEE80211_VAP_GOTOSLEEP(vap);
		}
		else {
			/* wakeup */
			IEEE80211_VAP_WAKEUP(vap);
		}
		ieee80211_send_nulldata(ieee80211_ref_node(vap->iv_bss));
		break;
	case IEEE80211_PARAM_QOSNULL:
		/* Force a QoS Null for testing. */
		ieee80211_send_qosnulldata(vap->iv_bss, value);
		break;
	case IEEE80211_PARAM_PSPOLL:
		/* Force a PS-POLL for testing. */
		ieee80211_send_pspoll(vap->iv_bss);
		break;
	case IEEE80211_PARAM_EOSPDROP:
		if (vap->iv_opmode == IEEE80211_M_HOSTAP) {
			if (value) IEEE80211_VAP_EOSPDROP_ENABLE(vap);
			else IEEE80211_VAP_EOSPDROP_DISABLE(vap);
		}
		break;
	case IEEE80211_PARAM_MARKDFS:
		if (value)
			ic->ic_flags_ext |= IEEE80211_FEXT_MARKDFS;
		else
			ic->ic_flags_ext &= ~IEEE80211_FEXT_MARKDFS;
		break;
	case IEEE80211_PARAM_CHANBW:
		switch (value) {
			case 0:
				ic->ic_chanbwflag = 0;
				break;
			case 1:
				ic->ic_chanbwflag = IEEE80211_CHAN_HALF;
				break;
			case 2:
				ic->ic_chanbwflag = IEEE80211_CHAN_QUARTER;
				break;
			default:
				retv = EINVAL;
				break;
		}
		break;
	default:
		retv = EOPNOTSUPP;
		break;
	}
#ifdef ATH_SUPERG_XR
	/* set the same params on the xr vap device if exists */
	if(vap->iv_xrvap && !(vap->iv_flags & IEEE80211_F_XR) ) {
		ieee80211_ioctl_setparam(vap->iv_xrvap->iv_dev,info,w,extra);
		vap->iv_xrvap->iv_ath_cap &= IEEE80211_ATHC_XR; /* XR vap does not support  any superG features */
	} 
	/*
	 * do not reset the xr vap , which is automatically 
	 * reset by the state machine now.
	 */
	if(!vap->iv_xrvap || (vap->iv_xrvap && !(vap->iv_flags & IEEE80211_F_XR))) {
		if (retv == ENETRESET)
			retv = IS_UP_AUTO(vap) ? ieee80211_open(vap->iv_dev) : 0;
	}
#else
	/* XXX should any of these cause a rescan? */
	if (retv == ENETRESET)
		retv = IS_UP_AUTO(vap) ? ieee80211_open(vap->iv_dev) : 0;
#endif
	return -retv;
}

static int
cap2cipher(int flag)
{
	switch (flag) {
	case IEEE80211_C_WEP:		return IEEE80211_CIPHER_WEP;
	case IEEE80211_C_AES:		return IEEE80211_CIPHER_AES_OCB;
	case IEEE80211_C_AES_CCM:	return IEEE80211_CIPHER_AES_CCM;
	case IEEE80211_C_CKIP:		return IEEE80211_CIPHER_CKIP;
	case IEEE80211_C_TKIP:		return IEEE80211_CIPHER_TKIP;
	}
	return -1;
}

int
ieee80211_ioctl_getmode(struct net_device *dev, struct iw_request_info *info,
			void *w, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct iw_point *wri = (struct iw_point *)extra;
	struct ifmediareq imr;
	char mode[8];

	ic->ic_media.ifm_status(ic->ic_dev, &imr);
	switch (IFM_MODE(imr.ifm_active)) {
	case IFM_IEEE80211_11A:
		strcpy(mode,"11a");
		break;
	case IFM_IEEE80211_11B:
		strcpy(mode,"11b");
		break;
	case IFM_IEEE80211_11G:
		strcpy(mode,"11g");
		break;
	case IFM_IEEE80211_FH:
		strcpy(mode,"FH");
		break;
	case IFM_AUTO:
		strcpy(mode,"auto");
		break;
	default:
		return -EINVAL;
	}
	if (ic->ic_media.ifm_media & IFM_IEEE80211_TURBO) {
		if (vap->iv_ath_cap & IEEE80211_ATHC_TURBOP)
			strcat(mode,"T");
		else
			strcat(mode,"ST");
	}
	return (copy_to_user(wri->pointer, mode, 6) ? -EFAULT : 0);
}

int
ieee80211_ioctl_getparam(struct net_device *dev, struct iw_request_info *info,
			void *w, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct ieee80211_rsnparms *rsn = &vap->iv_bss->ni_rsn;
	int *param = (int *) extra;
	u_int m;
	switch (param[0]) {
	case IEEE80211_PARAM_AUTHMODE:
		if (vap->iv_flags & IEEE80211_F_WPA)
			param[0] = IEEE80211_AUTH_WPA;
		else
			param[0] = vap->iv_bss->ni_authmode;
		break;
	case IEEE80211_PARAM_PROTMODE:
		param[0] = ic->ic_protmode;
		break;
	case IEEE80211_PARAM_MCASTCIPHER:
		param[0] = rsn->rsn_mcastcipher;
		break;
	case IEEE80211_PARAM_MCASTKEYLEN:
		param[0] = rsn->rsn_mcastkeylen;
		break;
	case IEEE80211_PARAM_UCASTCIPHERS:
		param[0] = 0;
		for (m = 0x1; m != 0; m <<= 1)
			if (rsn->rsn_ucastcipherset & m)
				param[0] |= 1<<cap2cipher(m);
		break;
	case IEEE80211_PARAM_UCASTCIPHER:
		param[0] = rsn->rsn_ucastcipher;
		break;
	case IEEE80211_PARAM_UCASTKEYLEN:
		param[0] = rsn->rsn_ucastkeylen;
		break;
	case IEEE80211_PARAM_KEYMGTALGS:
		param[0] = rsn->rsn_keymgmtset;
		break;
	case IEEE80211_PARAM_RSNCAPS:
		param[0] = rsn->rsn_caps;
		break;
	case IEEE80211_PARAM_WPA:
		switch (vap->iv_flags & IEEE80211_F_WPA) {
		case IEEE80211_F_WPA1:
			param[0] = 1;
			break;
		case IEEE80211_F_WPA2:
			param[0] = 2;
			break;
		case IEEE80211_F_WPA1 | IEEE80211_F_WPA2:
			param[0] = 3;
			break;
		default:
			param[0] = 0;
			break;
		}
		break;
	case IEEE80211_PARAM_ROAMING:
		param[0] = ic->ic_roaming;
		break;
	case IEEE80211_PARAM_PRIVACY:
		param[0] = (vap->iv_flags & IEEE80211_F_PRIVACY) != 0;
		break;
	case IEEE80211_PARAM_DROPUNENCRYPTED:
		param[0] = (vap->iv_flags & IEEE80211_F_DROPUNENC) != 0;
		break;
	case IEEE80211_PARAM_COUNTERMEASURES:
		param[0] = (vap->iv_flags & IEEE80211_F_COUNTERM) != 0;
		break;
	case IEEE80211_PARAM_DRIVER_CAPS:
		param[0] = vap->iv_caps;
		break;
	case IEEE80211_PARAM_WMM:
		param[0] = (vap->iv_flags & IEEE80211_F_WME) != 0;
		break;
	case IEEE80211_PARAM_HIDESSID:
		param[0] = (vap->iv_flags & IEEE80211_F_HIDESSID) != 0;
		break;
	case IEEE80211_PARAM_APBRIDGE:
		param[0] = (vap->iv_flags & IEEE80211_F_NOBRIDGE) == 0;
		break;
	case IEEE80211_PARAM_INACT:
		param[0] = vap->iv_inact_run * IEEE80211_INACT_WAIT;
		break;
	case IEEE80211_PARAM_INACT_AUTH:
		param[0] = vap->iv_inact_auth * IEEE80211_INACT_WAIT;
		break;
	case IEEE80211_PARAM_INACT_INIT:
		param[0] = vap->iv_inact_init * IEEE80211_INACT_WAIT;
		break;
	case IEEE80211_PARAM_ABOLT:
		/*
		 * Map capability bits to abolt settings.
		 */
		param[0] = 0;
		if (vap->iv_ath_cap & IEEE80211_ATHC_COMP)
			param[0] |= IEEE80211_ABOLT_COMPRESSION;
		if (vap->iv_ath_cap & IEEE80211_ATHC_FF)
			param[0] |= IEEE80211_ABOLT_FAST_FRAME;
		if (vap->iv_ath_cap & IEEE80211_ATHC_XR)
			param[0] |= IEEE80211_ABOLT_XR;
		if (vap->iv_ath_cap & IEEE80211_ATHC_BURST)
			param[0] |= IEEE80211_ABOLT_BURST;
		if (vap->iv_ath_cap & IEEE80211_ATHC_TURBOP)
			param[0] |= IEEE80211_ABOLT_TURBO_PRIME;
		if (vap->iv_ath_cap & IEEE80211_ATHC_AR)
			param[0] |= IEEE80211_ABOLT_AR;
        if (vap->iv_ath_cap & IEEE80211_ATHC_WME)
            param[0] |= IEEE80211_ABOLT_WME_ELE;
		break;
	case IEEE80211_PARAM_COMPRESSION:
		param[0] = (vap->iv_ath_cap & IEEE80211_ATHC_COMP) != 0;
		break;
	case IEEE80211_PARAM_FF:
		param[0] = (vap->iv_ath_cap & IEEE80211_ATHC_FF) != 0;
		break;
    case IEEE80211_PARAM_WMM_AGGRMODE:
        param[0] = (vap->iv_ath_cap & IEEE80211_ATHC_WME) != 0;
        break;
	case IEEE80211_PARAM_XR:
		param[0] = (vap->iv_ath_cap & IEEE80211_ATHC_XR) != 0;
		break;
	case IEEE80211_PARAM_BURST:
		param[0] = (vap->iv_ath_cap & IEEE80211_ATHC_BURST) != 0;
		break;
	case IEEE80211_PARAM_AR:
		param[0] = (vap->iv_ath_cap & IEEE80211_ATHC_AR) != 0;
		break;
	case IEEE80211_PARAM_TURBO:
		param[0] = (vap->iv_ath_cap & IEEE80211_ATHC_TURBOP) != 0;
		break;
	case IEEE80211_PARAM_DTIM_PERIOD:
		param[0] = vap->iv_dtim_period;
		break;
	case IEEE80211_PARAM_BEACON_INTERVAL:
		/* NB: get from ic_bss for station mode */
		param[0] = vap->iv_bss->ni_intval;
		break;
	case IEEE80211_PARAM_DOTH:
		param[0] = (ic->ic_flags & IEEE80211_F_DOTH) != 0;
		break;
	case IEEE80211_PARAM_PWRTARGET:
		param[0] = ic->ic_curchanmaxpwr;
		break;
	case IEEE80211_PARAM_PUREG:
		param[0] = (vap->iv_flags & IEEE80211_F_PUREG) != 0;
		break;
	case IEEE80211_PARAM_WDS:
		param[0] = ((vap->iv_flags_ext & IEEE80211_FEXT_WDS) == IEEE80211_FEXT_WDS);
		break;
	case IEEE80211_PARAM_BGSCAN:
		param[0] = (vap->iv_flags & IEEE80211_F_BGSCAN) != 0;
		break;
	case IEEE80211_PARAM_BGSCAN_IDLE:
		param[0] = vap->iv_bgscanidle*HZ/1000;	/* ms */
		break;
	case IEEE80211_PARAM_BGSCAN_INTERVAL:
		param[0] = vap->iv_bgscanintvl/HZ;	/* seconds */
		break;
	case IEEE80211_PARAM_MCAST_RATE:
		param[0] = vap->iv_mcast_rate;	/* seconds */
		break;
	case IEEE80211_PARAM_COVERAGE_CLASS:
		param[0] = ic->ic_coverageclass;
		break;
	case IEEE80211_PARAM_COUNTRY_IE:
		param[0] = (ic->ic_flags_ext & IEEE80211_FEXT_COUNTRYIE) != 0;
		break;
	case IEEE80211_PARAM_REGCLASS:
		param[0] = (ic->ic_flags_ext & IEEE80211_FEXT_REGCLASS) != 0;
		break;
	case IEEE80211_PARAM_SCANVALID:
		param[0] = vap->iv_scanvalid/HZ;	/* seconds */
		break;
	case IEEE80211_PARAM_ROAM_RSSI_11A:
		param[0] = vap->iv_roam.rssi11a;
		break;
	case IEEE80211_PARAM_ROAM_RSSI_11B:
		param[0] = vap->iv_roam.rssi11bOnly;
		break;
	case IEEE80211_PARAM_ROAM_RSSI_11G:
		param[0] = vap->iv_roam.rssi11b;
		break;
	case IEEE80211_PARAM_ROAM_RATE_11A:
		param[0] = vap->iv_roam.rate11a;
		break;
	case IEEE80211_PARAM_ROAM_RATE_11B:
		param[0] = vap->iv_roam.rate11bOnly;
		break;
	case IEEE80211_PARAM_ROAM_RATE_11G:
		param[0] = vap->iv_roam.rate11b;
		break;
	case IEEE80211_PARAM_UAPSDINFO:
		if (vap->iv_opmode == IEEE80211_M_HOSTAP) {
			if (IEEE80211_VAP_UAPSD_ENABLED(vap))
				param[0] = 1;
			else
				param[0] = 0;
		}
		else if (vap->iv_opmode == IEEE80211_M_STA) {
			param[0] = vap->iv_uapsdinfo;
		}
		break;
	case IEEE80211_PARAM_SLEEP:
		param[0] = vap->iv_bss->ni_flags & IEEE80211_NODE_PWR_MGT;
		break;
	case IEEE80211_PARAM_EOSPDROP:
		param[0] = IEEE80211_VAP_EOSPDROP_ENABLED(vap);
		break;
	case IEEE80211_PARAM_MARKDFS:
		if (ic->ic_flags_ext & IEEE80211_FEXT_MARKDFS)
			param[0] = 1;
		else
			param[0] = 0;
		break;
	case IEEE80211_PARAM_CHANBW:
		switch(ic->ic_chanbwflag) {
			case IEEE80211_CHAN_HALF:
				param[0] = 1;
				break;
			case IEEE80211_CHAN_QUARTER:
				param[0] = 2;
				break;
			default:
				param[0] = 0;
				break;
		}
		break;
	default:
		return -EOPNOTSUPP;
	}
	return 0;
}

static int
ieee80211_ioctl_setoptie(struct net_device *dev, struct iw_request_info *info,
		   	 void *w, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	union iwreq_data *u = w;
	void *ie;

	/*
	 * NB: Doing this for ap operation could be useful (e.g. for
	 *     WPA and/or WME) except that it typically is worthless
	 *     without being able to intervene when processing
	 *     association response frames--so disallow it for now.
	 */
	if (vap->iv_opmode != IEEE80211_M_STA)
		return -EINVAL;
	/* NB: data.length is validated by the wireless extensions code */
	MALLOC(ie, void *, u->data.length, M_DEVBUF, M_WAITOK);
	if (ie == NULL)
		return -ENOMEM;
	memcpy(ie, extra, u->data.length);
	/* XXX sanity check data? */
	if (vap->iv_opt_ie != NULL)
		FREE(vap->iv_opt_ie, M_DEVBUF);
	vap->iv_opt_ie = ie;
	vap->iv_opt_ie_len = u->data.length;
#ifdef ATH_SUPERG_XR
	/* set the same params on the xr vap device if exists */
	if(vap->iv_xrvap && !(vap->iv_flags & IEEE80211_F_XR) )
		ieee80211_ioctl_setoptie(vap->iv_xrvap->iv_dev,info,w,extra);
#endif
	return 0;
}

static int
ieee80211_ioctl_getoptie(struct net_device *dev, struct iw_request_info *info,
		   	 void *w, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	union iwreq_data *u = w;

	if (vap->iv_opt_ie == NULL)
		return -EINVAL;
	if (u->data.length < vap->iv_opt_ie_len)
		return -EINVAL;
	u->data.length = vap->iv_opt_ie_len;
	memcpy(extra, vap->iv_opt_ie, u->data.length);
	return 0;
}

static int
ieee80211_ioctl_setkey(struct net_device *dev, struct iw_request_info *info,
		   	 void *w, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct ieee80211req_key *ik = (struct ieee80211req_key *)extra;
	struct ieee80211_node *ni;
	struct ieee80211_key *wk;
	u_int16_t kid;
	int error, flags,i;

	/* NB: cipher support is verified by ieee80211_crypt_newkey */
	/* NB: this also checks ik->ik_keylen > sizeof(wk->wk_key) */
	if (ik->ik_keylen > sizeof(ik->ik_keydata))
		return -E2BIG;
	kid = ik->ik_keyix;
	if (kid == IEEE80211_KEYIX_NONE) {
		/* XXX unicast keys currently must be tx/rx */
		if (ik->ik_flags != (IEEE80211_KEY_XMIT | IEEE80211_KEY_RECV))
			return -EINVAL;
		if (vap->iv_opmode == IEEE80211_M_STA) {
			ni = ieee80211_ref_node(vap->iv_bss);
			if (!IEEE80211_ADDR_EQ(ik->ik_macaddr, ni->ni_bssid))
				return -EADDRNOTAVAIL;
		} else {
			ni = ieee80211_find_node(&ic->ic_sta, ik->ik_macaddr);
		}
		if (ni == NULL)
			return -ENOENT;
		wk = &ni->ni_ucastkey;
	} else {
		if (kid >= IEEE80211_WEP_NKID)
			return -EINVAL;
		wk = &vap->iv_nw_keys[kid];
		ni = NULL;
		/* XXX auto-add group key flag until applications are updated */
		if ((ik->ik_flags & IEEE80211_KEY_XMIT) == 0)	/* XXX */
			ik->ik_flags |= IEEE80211_KEY_GROUP;	/* XXX */
	}
	error = 0;
	flags = ik->ik_flags & IEEE80211_KEY_COMMON;
	ieee80211_key_update_begin(vap);
	if (ieee80211_crypto_newkey(vap, ik->ik_type, flags, wk)) {
		wk->wk_keylen = ik->ik_keylen;
		/* NB: MIC presence is implied by cipher type */
		if (wk->wk_keylen > IEEE80211_KEYBUF_SIZE)
			wk->wk_keylen = IEEE80211_KEYBUF_SIZE;
		for(i=0;i<IEEE80211_TID_SIZE;++i)
				wk->wk_keyrsc[i] = ik->ik_keyrsc;
		wk->wk_keytsc = 0;			/* new key, reset */
		memset(wk->wk_key, 0, sizeof(wk->wk_key));
		memcpy(wk->wk_key, ik->ik_keydata, ik->ik_keylen);
		if (!ieee80211_crypto_setkey(vap, wk,
		    ni != NULL ? ni->ni_macaddr : ik->ik_macaddr, ni))
			error = -EIO;
		else if ((ik->ik_flags & IEEE80211_KEY_DEFAULT))
			vap->iv_def_txkey = kid;
	} else
		error = -ENXIO;
	ieee80211_key_update_end(vap);
	if (ni != NULL)
		ieee80211_free_node(ni);
#ifdef ATH_SUPERG_XR
	/* set the same params on the xr vap device if exists */
	if(vap->iv_xrvap && !(vap->iv_flags & IEEE80211_F_XR) )
		ieee80211_ioctl_setkey(vap->iv_xrvap->iv_dev,info,w,extra);
#endif
	return error;
}

static int
ieee80211_ioctl_getkey(struct net_device *dev, struct iwreq *iwr)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct ieee80211_node *ni;
	struct ieee80211req_key ik;
	struct ieee80211_key *wk;
	const struct ieee80211_cipher *cip;
	u_int kid;

	if (iwr->u.data.length != sizeof(ik))
		return -EINVAL;
	if (copy_from_user(&ik, iwr->u.data.pointer, sizeof(ik)))
		return -EFAULT;
	kid = ik.ik_keyix;
	if (kid == IEEE80211_KEYIX_NONE) {
		ni = ieee80211_find_node(&ic->ic_sta, ik.ik_macaddr);
		if (ni == NULL)
			return -EINVAL;		/* XXX */
		wk = &ni->ni_ucastkey;
	} else {
		if (kid >= IEEE80211_WEP_NKID)
			return -EINVAL;
		wk = &vap->iv_nw_keys[kid];
		IEEE80211_ADDR_COPY(&ik.ik_macaddr, vap->iv_bss->ni_macaddr);
		ni = NULL;
	}
	cip = wk->wk_cipher;
	ik.ik_type = cip->ic_cipher;
	ik.ik_keylen = wk->wk_keylen;
	ik.ik_flags = wk->wk_flags & (IEEE80211_KEY_XMIT | IEEE80211_KEY_RECV);
	if (wk->wk_keyix == vap->iv_def_txkey)
		ik.ik_flags |= IEEE80211_KEY_DEFAULT;
	if (capable(CAP_NET_ADMIN)) {
		/* NB: only root can read key data */
		ik.ik_keyrsc = wk->wk_keyrsc[0];
		ik.ik_keytsc = wk->wk_keytsc;
		memcpy(ik.ik_keydata, wk->wk_key, wk->wk_keylen);
		if (cip->ic_cipher == IEEE80211_CIPHER_TKIP) {
			memcpy(ik.ik_keydata+wk->wk_keylen,
				wk->wk_key + IEEE80211_KEYBUF_SIZE,
				IEEE80211_MICBUF_SIZE);
			ik.ik_keylen += IEEE80211_MICBUF_SIZE;
		}
	} else {
		ik.ik_keyrsc = 0;
		ik.ik_keytsc = 0;
		memset(ik.ik_keydata, 0, sizeof(ik.ik_keydata));
	}
	if (ni != NULL)
		ieee80211_free_node(ni);
	return (copy_to_user(iwr->u.data.pointer, &ik, sizeof(ik)) ?
			-EFAULT : 0);
}

static int
ieee80211_ioctl_delkey(struct net_device *dev, struct iw_request_info *info,
		   	 void *w, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct ieee80211req_del_key *dk = (struct ieee80211req_del_key *)extra;
	int kid;

	kid = dk->idk_keyix;
	/* XXX u_int8_t -> u_int16_t */
	if (dk->idk_keyix == (u_int8_t) IEEE80211_KEYIX_NONE) {
		struct ieee80211_node *ni;

		ni = ieee80211_find_node(&ic->ic_sta, dk->idk_macaddr);
		if (ni == NULL)
			return -EINVAL;		/* XXX */
		/* XXX error return */
		ieee80211_crypto_delkey(vap, &ni->ni_ucastkey, ni);
		ieee80211_free_node(ni);
	} else {
		if (kid >= IEEE80211_WEP_NKID)
			return -EINVAL;
		/* XXX error return */
		ieee80211_crypto_delkey(vap, &vap->iv_nw_keys[kid], NULL);
	}
	return 0;
}

static void
domlme(void *arg, struct ieee80211_node *ni)
{
	struct ieee80211req_mlme *mlme = arg;

	if (ni->ni_associd != 0) {
		IEEE80211_SEND_MGMT(ni,
			mlme->im_op == IEEE80211_MLME_DEAUTH ?
				IEEE80211_FC0_SUBTYPE_DEAUTH :
				IEEE80211_FC0_SUBTYPE_DISASSOC,
			mlme->im_reason);
	}
	ieee80211_node_leave(ni);
}

struct scanlookup {
	const u_int8_t *mac;
	int esslen;
	const u_int8_t *essid;
	const struct ieee80211_scan_entry *se;
};

/*
 * Match mac address and any ssid.
 */
static void
mlmelookup(void *arg, const struct ieee80211_scan_entry *se)
{
	struct scanlookup *look = arg;

	if (!IEEE80211_ADDR_EQ(look->mac, se->se_macaddr))
		return;
	if (look->esslen != 0) {
		if (se->se_ssid[1] != look->esslen)
			return;
		if (memcmp(look->essid, se->se_ssid+2, look->esslen))
			return;
	}
	look->se = se;
}

static int
ieee80211_ioctl_setmlme(struct net_device *dev, struct iw_request_info *info,
		   	 void *w, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct ieee80211req_mlme *mlme = (struct ieee80211req_mlme *)extra;
	struct ieee80211_node *ni;

	switch (mlme->im_op) {
	case IEEE80211_MLME_ASSOC:
		if (vap->iv_opmode == IEEE80211_M_STA) {
			struct scanlookup lookup;

			lookup.se = NULL;
			lookup.mac = mlme->im_macaddr;
			/* XXX use revised api w/ explicit ssid */
			lookup.esslen = vap->iv_des_ssid[0].len;
			lookup.essid = vap->iv_des_ssid[0].ssid;
			ieee80211_scan_iterate(ic, mlmelookup, &lookup);
			if (lookup.se != NULL &&
			    ieee80211_sta_join(vap, lookup.se))
				return 0;
		}
		return -EINVAL;
	case IEEE80211_MLME_DISASSOC:
	case IEEE80211_MLME_DEAUTH:
		switch (vap->iv_opmode) {
		case IEEE80211_M_STA:
			/* XXX not quite right */
			ieee80211_new_state(vap, IEEE80211_S_INIT,
				mlme->im_reason);
			break;
		case IEEE80211_M_HOSTAP:
			/* NB: the broadcast address means do 'em all */
			if (!IEEE80211_ADDR_EQ(mlme->im_macaddr, vap->iv_dev->broadcast)) {
				ni = ieee80211_find_node(&ic->ic_sta,
					mlme->im_macaddr);
				if (ni == NULL)
					return -EINVAL;
				domlme(mlme, ni);
				ieee80211_free_node(ni);
			} else {
				ieee80211_iterate_nodes(&ic->ic_sta,
						domlme, mlme);
			}
			break;
		default:
			return -EINVAL;
		}
		break;
	case IEEE80211_MLME_AUTHORIZE:
	case IEEE80211_MLME_UNAUTHORIZE:
		if (vap->iv_opmode != IEEE80211_M_HOSTAP)
			return -EINVAL;
		ni = ieee80211_find_node(&ic->ic_sta, mlme->im_macaddr);
		if (ni == NULL)
			return -EINVAL;
		if (mlme->im_op == IEEE80211_MLME_AUTHORIZE)
			ieee80211_node_authorize(ni);
		else
			ieee80211_node_unauthorize(ni);
		ieee80211_free_node(ni);
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static int
ieee80211_ioctl_addmac(struct net_device *dev, struct iw_request_info *info,
		   	 void *w, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct sockaddr *sa = (struct sockaddr *)extra;
	const struct ieee80211_aclator *acl = vap->iv_acl;

	if (acl == NULL) {
		acl = ieee80211_aclator_get("mac");
		if (acl == NULL || !acl->iac_attach(vap))
			return -EINVAL;
		vap->iv_acl = acl;
	}
	acl->iac_add(vap, sa->sa_data);
	return 0;
}

static int
ieee80211_ioctl_delmac(struct net_device *dev, struct iw_request_info *info,
		   	 void *w, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct sockaddr *sa = (struct sockaddr *)extra;
	const struct ieee80211_aclator *acl = vap->iv_acl;

	if (acl == NULL) {
		acl = ieee80211_aclator_get("mac");
		if (acl == NULL || !acl->iac_attach(vap))
			return -EINVAL;
		vap->iv_acl = acl;
	}
	acl->iac_remove(vap, sa->sa_data);
	return 0;
}

static int
ieee80211_ioctl_setchanlist(struct net_device *dev,
	struct iw_request_info *info, void *w, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct ieee80211req_chanlist *list =
		(struct ieee80211req_chanlist *)extra;
	u_char chanlist[IEEE80211_CHAN_BYTES];
	int i, j, nchan;

	memset(chanlist, 0, sizeof(chanlist));
	/*
	 * Since channel 0 is not available for DS, channel 1
	 * is assigned to LSB on WaveLAN.
	 */
	if (ic->ic_phytype == IEEE80211_T_DS)
		i = 1;
	else
		i = 0;
	nchan = 0;
	for (j = 0; i <= IEEE80211_CHAN_MAX; i++, j++) {
		/*
		 * NB: silently discard unavailable channels so users
		 *     can specify 1-255 to get all available channels.
		 */
		if (isset(list->ic_channels, j) && isset(ic->ic_chan_avail, i)) {
			setbit(chanlist, i);
			nchan++;
		}
	}
	if (nchan == 0)			/* no valid channels, disallow */
		return -EINVAL;
	if (ic->ic_bsschan != IEEE80211_CHAN_ANYC &&	/* XXX */
	    isclr(chanlist, ic->ic_bsschan->ic_ieee))
		ic->ic_bsschan = IEEE80211_CHAN_ANYC;	/* invalidate */
	memcpy(ic->ic_chan_active, chanlist, sizeof(ic->ic_chan_active));
	if (IS_UP_AUTO(vap))
		ieee80211_new_state(vap, IEEE80211_S_SCAN, 0);
	return 0;
}

static int
ieee80211_ioctl_getchanlist(struct net_device *dev,
	struct iw_request_info *info, void *w, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;

	memcpy(extra, ic->ic_chan_active, sizeof(ic->ic_chan_active));
	return 0;
}

static int
ieee80211_ioctl_getchaninfo(struct net_device *dev,
	struct iw_request_info *info, void *w, char *extra)
{
#define IS_NEW_CHANNEL(c)		\
	((IEEE80211_IS_CHAN_5GHZ((c)) && isclr(reported_a, (c)->ic_ieee)) || \
	 (IEEE80211_IS_CHAN_2GHZ((c)) && isclr(reported_bg, (c)->ic_ieee)))

	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct ieee80211req_chaninfo *chans =
		(struct ieee80211req_chaninfo *) extra;
	u_int8_t reported_a[IEEE80211_CHAN_BYTES];	/* XXX stack usage? */
	u_int8_t reported_bg[IEEE80211_CHAN_BYTES];	/* XXX stack usage? */
	int i;

	memset(chans, 0, sizeof(*chans));
	memset(reported_a, 0, sizeof(reported_a));
	memset(reported_bg, 0, sizeof(reported_bg));
	for (i = 0; i < ic->ic_nchans; i++) {
		const struct ieee80211_channel *c = &ic->ic_channels[i];
		const struct ieee80211_channel *c1 = c;

		if (IS_NEW_CHANNEL(c) || IEEE80211_IS_CHAN_HALF(c) ||
		    		IEEE80211_IS_CHAN_QUARTER(c)) {

			if (IEEE80211_IS_CHAN_5GHZ(c))
				setbit(reported_a, c->ic_ieee);
			else
				setbit(reported_bg, c->ic_ieee);

			/* pick turbo channel over non-turbo channel, and
			 * 11g channel over 11b channel */
			if (IEEE80211_IS_CHAN_A(c))
				c1 = findchannel(ic, c->ic_ieee, IEEE80211_MODE_TURBO_A);
			if (IEEE80211_IS_CHAN_ANYG(c))
				c1 = findchannel(ic, c->ic_ieee, IEEE80211_MODE_TURBO_G);
			else if (IEEE80211_IS_CHAN_B(c)) {
				c1 = findchannel(ic, c->ic_ieee, IEEE80211_MODE_TURBO_G);
				if (!c1)
					c1 = findchannel(ic, c->ic_ieee, IEEE80211_MODE_11G);
			}

			if (c1)
				c = c1;
			chans->ic_chans[chans->ic_nchans].ic_ieee = c->ic_ieee;
			chans->ic_chans[chans->ic_nchans].ic_freq = c->ic_freq;
			chans->ic_chans[chans->ic_nchans].ic_flags = c->ic_flags;
			if (++chans->ic_nchans >= IEEE80211_CHAN_MAX)
				break;
		}
	}
	return 0;

#undef IS_NEW_CHANNEL
}

static int
ieee80211_ioctl_setwmmparams(struct net_device *dev,
	struct iw_request_info *info, void *w, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	int *param = (int *) extra;
	int ac = (param[1] < WME_NUM_AC) ? param[1] : WME_AC_BE;
	int bss = param[2]; 
	struct ieee80211_wme_state *wme = &vap->iv_ic->ic_wme;

	switch (param[0]) {
        case IEEE80211_WMMPARAMS_CWMIN:
		if (param[3] < 0 || param[3] > 15) 
			return EINVAL;
        	if (bss) {
			wme->wme_wmeBssChanParams.cap_wmeParams[ac].wmep_logcwmin = param[3];
			if ((wme->wme_flags & WME_F_AGGRMODE) == 0) {
				wme->wme_bssChanParams.cap_wmeParams[ac].wmep_logcwmin = param[3];
			}
		} else {
			wme->wme_wmeChanParams.cap_wmeParams[ac].wmep_logcwmin = param[3];
			wme->wme_chanParams.cap_wmeParams[ac].wmep_logcwmin = param[3];
		}
		ieee80211_wme_updateparams(vap);	
		break;
	case IEEE80211_WMMPARAMS_CWMAX:
		if (param[3] < 0 || param[3] > 15) 
			return EINVAL;
        	if (bss) {
			wme->wme_wmeBssChanParams.cap_wmeParams[ac].wmep_logcwmax = param[3];
			if ((wme->wme_flags & WME_F_AGGRMODE) == 0) {
				wme->wme_bssChanParams.cap_wmeParams[ac].wmep_logcwmax = param[3];
			}
		} else {
			wme->wme_wmeChanParams.cap_wmeParams[ac].wmep_logcwmax = param[3];
			wme->wme_chanParams.cap_wmeParams[ac].wmep_logcwmax = param[3];
		}
		ieee80211_wme_updateparams(vap);	
		break;
        case IEEE80211_WMMPARAMS_AIFS:
		if (param[3] < 0 || param[3] > 15) 
			return EINVAL;	
        	if (bss) {
			wme->wme_wmeBssChanParams.cap_wmeParams[ac].wmep_aifsn = param[3];
			if ((wme->wme_flags & WME_F_AGGRMODE) == 0) {
				wme->wme_bssChanParams.cap_wmeParams[ac].wmep_aifsn = param[3];
			}
		} else {
			wme->wme_wmeChanParams.cap_wmeParams[ac].wmep_aifsn = param[3];
			wme->wme_chanParams.cap_wmeParams[ac].wmep_aifsn = param[3];
		}
		ieee80211_wme_updateparams(vap);	
		break;
        case IEEE80211_WMMPARAMS_TXOPLIMIT:
		if (param[3] < 0 || param[3] > 8192) 
			return EINVAL;
        	if (bss) {
			wme->wme_wmeBssChanParams.cap_wmeParams[ac].wmep_txopLimit 
				= IEEE80211_US_TO_TXOP(param[3]);
			if ((wme->wme_flags & WME_F_AGGRMODE) == 0) {
				wme->wme_bssChanParams.cap_wmeParams[ac].wmep_txopLimit 
					= IEEE80211_US_TO_TXOP(param[3]);
			}
		} else {
			wme->wme_wmeChanParams.cap_wmeParams[ac].wmep_txopLimit 
				= IEEE80211_US_TO_TXOP(param[3]);
			wme->wme_chanParams.cap_wmeParams[ac].wmep_txopLimit 
				= IEEE80211_US_TO_TXOP(param[3]);
		}
		ieee80211_wme_updateparams(vap);	
		break;
        case IEEE80211_WMMPARAMS_ACM:
		if (!bss || param[3] < 0 || param[3] > 1) 
			return EINVAL;
        	/* ACM bit applies to BSS case only */
		wme->wme_wmeBssChanParams.cap_wmeParams[ac].wmep_acm = param[3];
		if ((wme->wme_flags & WME_F_AGGRMODE) == 0)
			wme->wme_bssChanParams.cap_wmeParams[ac].wmep_acm = param[3];
		break;
        case IEEE80211_WMMPARAMS_NOACKPOLICY:
		if (bss || param[3] < 0 || param[3] > 1) 
			return EINVAL;	
        	/* ack policy applies to non-BSS case only */
		wme->wme_wmeChanParams.cap_wmeParams[ac].wmep_noackPolicy = param[3];
		wme->wme_chanParams.cap_wmeParams[ac].wmep_noackPolicy = param[3];
		break;
	default:
		break;
	}
	return 0;
}

static int
ieee80211_ioctl_getwmmparams(struct net_device *dev,
	struct iw_request_info *info, void *w, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	int *param = (int *) extra;
	int ac = (param[1] < WME_NUM_AC) ? param[1] : WME_AC_BE;
	struct ieee80211_wme_state *wme = &vap->iv_ic->ic_wme;
	struct chanAccParams *chanParams = (param[2] == 0) ? 
		                             &(wme->wme_chanParams) 
					   : &(wme->wme_bssChanParams);

	switch (param[0]) {
        case IEEE80211_WMMPARAMS_CWMIN: 
		param[0] = chanParams->cap_wmeParams[ac].wmep_logcwmin;
		break;
        case IEEE80211_WMMPARAMS_CWMAX: 
		param[0] = chanParams->cap_wmeParams[ac].wmep_logcwmax;
		break;
        case IEEE80211_WMMPARAMS_AIFS: 
		param[0] = chanParams->cap_wmeParams[ac].wmep_aifsn;
		break;
        case IEEE80211_WMMPARAMS_TXOPLIMIT: 
		param[0] = IEEE80211_TXOP_TO_US(chanParams->cap_wmeParams[ac].wmep_txopLimit);
		break;
        case IEEE80211_WMMPARAMS_ACM: 
		param[0] =wme->wme_wmeBssChanParams.cap_wmeParams[ac].wmep_acm;
		break;
        case IEEE80211_WMMPARAMS_NOACKPOLICY: 
		param[0] =wme->wme_wmeChanParams.cap_wmeParams[ac].wmep_noackPolicy;
		break;
	default:
		break;
	}
	return 0;
}

static int
ieee80211_ioctl_getwpaie(struct net_device *dev, struct iwreq *iwr)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct ieee80211_node *ni;
	struct ieee80211req_wpaie wpaie;

	if (iwr->u.data.length != sizeof(wpaie))
		return -EINVAL;
	if (copy_from_user(&wpaie, iwr->u.data.pointer, IEEE80211_ADDR_LEN))
		return -EFAULT;
	ni = ieee80211_find_node(&ic->ic_sta, wpaie.wpa_macaddr);
	if (ni == NULL)
		return -EINVAL;		/* XXX */
	memset(wpaie.wpa_ie, 0, sizeof(wpaie.wpa_ie));
	if (ni->ni_wpa_ie != NULL) {
		int ielen = ni->ni_wpa_ie[1] + 2;
		if (ielen > sizeof(wpaie.wpa_ie))
			ielen = sizeof(wpaie.wpa_ie);
		memcpy(wpaie.wpa_ie, ni->ni_wpa_ie, ielen);
	}
	ieee80211_free_node(ni);
	return (copy_to_user(iwr->u.data.pointer, &wpaie, sizeof(wpaie)) ?
			-EFAULT : 0);
}

static int
ieee80211_ioctl_getstastats(struct net_device *dev, struct iwreq *iwr)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct ieee80211_node *ni;
	u_int8_t macaddr[IEEE80211_ADDR_LEN];
	const int off = __offsetof(struct ieee80211req_sta_stats, is_stats);
	int error;

	if (iwr->u.data.length < off)
		return -EINVAL;
	if (copy_from_user(macaddr, iwr->u.data.pointer, IEEE80211_ADDR_LEN))
		return -EFAULT;
	ni = ieee80211_find_node(&ic->ic_sta, macaddr);
	if (ni == NULL)
		return -EINVAL;		/* XXX */
	if (iwr->u.data.length > sizeof(struct ieee80211req_sta_stats))
		iwr->u.data.length = sizeof(struct ieee80211req_sta_stats);
	/* NB: copy out only the statistics */
	error = copy_to_user(iwr->u.data.pointer + off, &ni->ni_stats,
			iwr->u.data.length - off);
	ieee80211_free_node(ni);
	return (error ? -EFAULT : 0);
}

struct scanreq {
	struct ieee80211req_scan_result *sr;
	size_t space;
};

static size_t
scan_space(const struct ieee80211_scan_entry *se, int *ielen)
{

	*ielen = 0;
	if (se->se_wpa_ie != NULL)
		*ielen += 2+se->se_wpa_ie[1];
	if (se->se_wme_ie != NULL)
		*ielen += 2+se->se_wme_ie[1];
	if (se->se_ath_ie != NULL)
		*ielen += 2+se->se_ath_ie[1];
	return roundup(sizeof(struct ieee80211req_scan_result) +
			se->se_ssid[1] + *ielen, sizeof(u_int32_t));
}

static void
get_scan_space(void *arg, const struct ieee80211_scan_entry *se)
{
	struct scanreq *req = arg;
	int ielen;

	req->space += scan_space(se, &ielen);
}

static void
get_scan_result(void *arg, const struct ieee80211_scan_entry *se)
{
	struct scanreq *req = arg;
	struct ieee80211req_scan_result *sr;
	int ielen, len, nr, nxr;
	u_int8_t *cp;

	len = scan_space(se, &ielen);
	if (len > req->space)
		return;

	sr = req->sr;
	memset(sr, 0, sizeof(*sr));
	sr->isr_ssid_len = se->se_ssid[1];
	/* XXX watch for overflow */
	sr->isr_ie_len = ielen;
	sr->isr_len = len;
	sr->isr_freq = se->se_chan->ic_freq;
	sr->isr_flags = se->se_chan->ic_flags;
	sr->isr_rssi = se->se_rssi;
	sr->isr_intval = se->se_intval;
	sr->isr_capinfo = se->se_capinfo;
	sr->isr_erp = se->se_erp;
	IEEE80211_ADDR_COPY(sr->isr_bssid, se->se_bssid);
	/* XXX bounds check */
	nr = se->se_rates[1];
	memcpy(sr->isr_rates, se->se_rates+2, nr);
	nxr = se->se_xrates[1];
	memcpy(sr->isr_rates+nr, se->se_xrates+2, nxr);
	sr->isr_nrates = nr + nxr;

	cp = (u_int8_t *)(sr+1);
	memcpy(cp, se->se_ssid+2, sr->isr_ssid_len);
	cp += sr->isr_ssid_len;
	if (se->se_wpa_ie != NULL) {
		memcpy(cp, se->se_wpa_ie, 2+se->se_wpa_ie[1]);
		cp += 2+se->se_wpa_ie[1];
	}
	if (se->se_wme_ie != NULL) {
		memcpy(cp, se->se_wme_ie, 2+se->se_wme_ie[1]);
		cp += 2+se->se_wme_ie[1];
	}
	if (se->se_ath_ie != NULL) {
		memcpy(cp, se->se_ath_ie, 2+se->se_ath_ie[1]);
		cp += 2+se->se_ath_ie[1];
	}

	req->space -= len;
	req->sr = (struct ieee80211req_scan_result *)(((u_int8_t *)sr) + len);
}

static int
ieee80211_ioctl_getscanresults(struct net_device *dev, struct iwreq *iwr)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct scanreq req;
	int error;

	if (iwr->u.data.length < sizeof(struct scanreq))
		return -EFAULT;

	error = 0;
	req.space = 0;
	ieee80211_scan_iterate(ic, get_scan_space, &req);
	if (req.space > iwr->u.data.length)
		req.space = iwr->u.data.length;
	if (req.space > 0) {
		size_t space;
		void *p;

		space = req.space;
		MALLOC(p, void *, space, M_TEMP, M_WAITOK);
		if (p == NULL)
			return -ENOMEM;
		req.sr = p;
		ieee80211_scan_iterate(ic, get_scan_result, &req);
		iwr->u.data.length = space - req.space;
		error = copy_to_user(iwr->u.data.pointer, p, iwr->u.data.length);
		FREE(p, M_TEMP);
	} else
		iwr->u.data.length = 0;

	return (error ? -EFAULT : 0);
}

#if 0
//********** Ed Lai 05/11/01 /*  Don't Use This One ...*/
static int
ieee80211_ioctl_getstationlist(struct net_device *dev, struct iwreq *iwr)
{ // ? -=-=-=-=-=-=-=-=-= Begin of IEEE80211_IOCTL_WLACONN block =-=-=-= ?
		struct ieee80211vap *vap = dev->priv;
		struct ieee80211com *ic = vap->iv_ic;
		struct ieee80211_node *ni;
		struct ieee80211_node_table *nt = &ic->ic_sta;
		int listnum = 0;
		int error;

		WLA_CONN *list;
		int list_size = WLA_CONN_SIZE * sizeof(WLA_CONN);
		list = kmalloc(list_size, GFP_KERNEL);
		if(list == NULL) {
			error = -EFAULT;
		} else {
			memset(list, 0, list_size);
			IEEE80211_NODE_LOCK(nt);
			TAILQ_FOREACH(ni, &nt->nt_node, ni_list) {
				if(ni->ni_associd == 0)
					continue;
				IEEE80211_ADDR_COPY(list[listnum].mac, ni->ni_macaddr);
				list[listnum].assoc_time = ni->ni_assoctime;
				if(++listnum == WLA_CONN_SIZE)
					break;
			}
			IEEE80211_NODE_UNLOCK(ni->ni_table);
			
			if (copy_to_user(iwr->iwreq, list, list_size))
				error = -EFAULT;
			else
				error = 0;
		
			kfree(list);
		}
		return error;	
} // ? -=-=-=-=-=-=-=-=-= End of IEEE80211_IOCTL_WLACONN block =-=-=-= ?
#endif
//******************

struct stainforeq {
	struct ieee80211vap *vap;
	struct ieee80211req_sta_info *si;
	size_t	space;
};

static size_t
sta_space(const struct ieee80211_node *ni, size_t *ielen)
{
	*ielen = 0;
	if (ni->ni_wpa_ie != NULL)
		*ielen += 2+ni->ni_wpa_ie[1];
	if (ni->ni_wme_ie != NULL)
		*ielen += 2+ni->ni_wme_ie[1];
	if (ni->ni_ath_ie != NULL)
		*ielen += 2+ni->ni_ath_ie[1];
	return roundup(sizeof(struct ieee80211req_sta_info) + *ielen,
		      sizeof(u_int32_t));
}

static void
get_sta_space(void *arg, struct ieee80211_node *ni)
{
	struct stainforeq *req = arg;
	struct ieee80211vap *vap = ni->ni_vap;
	size_t ielen;

	if (vap != req->vap && vap != req->vap->iv_xrvap)		/* only entries for this vap */
		return;
	if (vap->iv_opmode == IEEE80211_M_HOSTAP &&
	    ni->ni_associd == 0)	/* only associated stations */
		return;
	req->space += sta_space(ni, &ielen);
}

static void
get_sta_info(void *arg, struct ieee80211_node *ni)
{
	struct stainforeq *req = arg;
	struct ieee80211vap *vap = ni->ni_vap;
	struct ieee80211com *ic = vap->iv_ic;
	struct ieee80211req_sta_info *si;
	size_t ielen, len;
	u_int8_t *cp;

	if (vap != req->vap && vap != req->vap->iv_xrvap)		/* only entries for this vap (or) xrvap */
		return;
	if (vap->iv_opmode == IEEE80211_M_HOSTAP &&
	    ni->ni_associd == 0)	/* only associated stations */
		return;
	if (ni->ni_chan == IEEE80211_CHAN_ANYC)	/* XXX bogus entry */
		return;
	len = sta_space(ni, &ielen);
	if (len > req->space)
		return;
	si = req->si;
	si->isi_len = len;
	si->isi_ie_len = ielen;
	si->isi_freq = ni->ni_chan->ic_freq;
	si->isi_flags = ni->ni_chan->ic_flags;
	si->isi_state = ni->ni_flags;
	si->isi_authmode = ni->ni_authmode;
	si->isi_rssi = ic->ic_node_getrssi(ni);
	si->isi_capinfo = ni->ni_capinfo;
	si->isi_athflags = ni->ni_ath_flags;
	si->isi_erp = ni->ni_erp;
	IEEE80211_ADDR_COPY(si->isi_macaddr, ni->ni_macaddr);
	si->isi_nrates = ni->ni_rates.rs_nrates;
	if (si->isi_nrates > 15)
		si->isi_nrates = 15;
	memcpy(si->isi_rates, ni->ni_rates.rs_rates, si->isi_nrates);
	si->isi_txrate = ni->ni_txrate;
	si->isi_ie_len = ielen;
	si->isi_associd = ni->ni_associd;
	si->isi_txpower = ni->ni_txpower;
	si->isi_vlan = ni->ni_vlan;
	if (ni->ni_flags & IEEE80211_NODE_QOS) {
		memcpy(si->isi_txseqs, ni->ni_txseqs, sizeof(ni->ni_txseqs));
		memcpy(si->isi_rxseqs, ni->ni_rxseqs, sizeof(ni->ni_rxseqs));
	} else {
		si->isi_txseqs[0] = ni->ni_txseqs[0];
		si->isi_rxseqs[0] = ni->ni_rxseqs[0];
	}
	si->isi_uapsd = ni->ni_uapsd;
	if ( vap == req->vap->iv_xrvap)		
		si->isi_opmode = IEEE80211_STA_OPMODE_XR;
	else
		si->isi_opmode = IEEE80211_STA_OPMODE_NORMAL;
	/* NB: leave all cases in case we relax ni_associd == 0 check */
	if (ieee80211_node_is_authorized(ni))
		si->isi_inact = vap->iv_inact_run;
	else if (ni->ni_associd != 0)
		si->isi_inact = vap->iv_inact_auth;
	else
		si->isi_inact = vap->iv_inact_init;
	si->isi_inact = (si->isi_inact - ni->ni_inact) * IEEE80211_INACT_WAIT;

	cp = (u_int8_t *)(si+1);
	if (ni->ni_wpa_ie != NULL) {
		memcpy(cp, ni->ni_wpa_ie, 2+ni->ni_wpa_ie[1]);
		cp += 2+ni->ni_wpa_ie[1];
	}
	if (ni->ni_wme_ie != NULL) {
		memcpy(cp, ni->ni_wme_ie, 2+ni->ni_wme_ie[1]);
		cp += 2+ni->ni_wme_ie[1];
	}
	if (ni->ni_ath_ie != NULL) {
		memcpy(cp, ni->ni_ath_ie, 2+ni->ni_ath_ie[1]);
		cp += 2+ni->ni_ath_ie[1];
	}

	req->si = (struct ieee80211req_sta_info *)(((u_int8_t *)si) + len);
	req->space -= len;
}

static int
ieee80211_ioctl_getstainfo(struct net_device *dev, struct iwreq *iwr)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct stainforeq req;
	int error;

	if (iwr->u.data.length < sizeof(struct stainforeq))
		return -EFAULT;

	/* estimate space required for station info */
	error = 0;
	req.space = sizeof(struct stainforeq);
	req.vap = vap;
	ieee80211_iterate_nodes(&ic->ic_sta, get_sta_space, &req);
	if (req.space > iwr->u.data.length)
		req.space = iwr->u.data.length;
	if (req.space > 0) {
		size_t space;
		void *p;

		space = req.space;
		MALLOC(p, void *, space, M_TEMP, M_WAITOK);
		req.si = (struct ieee80211req_sta_info *)p;
		ieee80211_iterate_nodes(&ic->ic_sta, get_sta_info, &req);
		iwr->u.data.length = space - req.space;
		error = copy_to_user(iwr->u.data.pointer, p, iwr->u.data.length);
		FREE(p, M_TEMP);
	} else
		iwr->u.data.length = 0;

	return (error ? -EFAULT : 0);
}

int
ieee80211_ioctl_chanswitch(struct net_device *dev, struct iw_request_info *info,
			void *w, char *extra)
{
	struct ieee80211vap *vap = dev->priv;
	struct ieee80211com *ic = vap->iv_ic;
	int *param = (int *) extra;

	if (!(ic->ic_flags & IEEE80211_F_DOTH))
		return 0;

	/* now flag the beacon update to include the channel switch IE */
	ic->ic_flags |= IEEE80211_F_CHANSWITCH;
	ic->ic_chanchange_chan = param[0];
	ic->ic_chanchange_tbtt = param[1];
	
	return 0;
}

#define	IW_PRIV_TYPE_OPTIE	IW_PRIV_TYPE_BYTE | IEEE80211_MAX_OPT_IE
#define	IW_PRIV_TYPE_KEY \
	IW_PRIV_TYPE_BYTE | sizeof(struct ieee80211req_key)
#define	IW_PRIV_TYPE_DELKEY \
	IW_PRIV_TYPE_BYTE | sizeof(struct ieee80211req_del_key)
#define	IW_PRIV_TYPE_MLME \
	IW_PRIV_TYPE_BYTE | sizeof(struct ieee80211req_mlme)
#define	IW_PRIV_TYPE_CHANLIST \
	IW_PRIV_TYPE_BYTE | sizeof(struct ieee80211req_chanlist)
#define	IW_PRIV_TYPE_CHANINFO \
	IW_PRIV_TYPE_BYTE | sizeof(struct ieee80211req_chaninfo)

static const struct iw_priv_args ieee80211_priv_args[] = {
	/* NB: setoptie & getoptie are !IW_PRIV_SIZE_FIXED */
	{ IEEE80211_IOCTL_SETOPTIE,
	  IW_PRIV_TYPE_OPTIE, 0,			"setoptie" },
	{ IEEE80211_IOCTL_GETOPTIE,
	  0, IW_PRIV_TYPE_OPTIE,			"getoptie" },
	{ IEEE80211_IOCTL_SETKEY,
	  IW_PRIV_TYPE_KEY | IW_PRIV_SIZE_FIXED, 0,	"setkey" },
	{ IEEE80211_IOCTL_DELKEY,
	  IW_PRIV_TYPE_DELKEY | IW_PRIV_SIZE_FIXED, 0,	"delkey" },
	{ IEEE80211_IOCTL_SETMLME,
	  IW_PRIV_TYPE_MLME | IW_PRIV_SIZE_FIXED, 0,	"setmlme" },
	{ IEEE80211_IOCTL_ADDMAC,
	  IW_PRIV_TYPE_ADDR | IW_PRIV_SIZE_FIXED | 1, 0,"addmac" },
	{ IEEE80211_IOCTL_DELMAC,
	  IW_PRIV_TYPE_ADDR | IW_PRIV_SIZE_FIXED | 1, 0,"delmac" },
	{ IEEE80211_IOCTL_SETCHANLIST,
	  IW_PRIV_TYPE_CHANLIST | IW_PRIV_SIZE_FIXED, 0,"setchanlist" },
	{ IEEE80211_IOCTL_GETCHANLIST,
	  0, IW_PRIV_TYPE_CHANLIST | IW_PRIV_SIZE_FIXED,"getchanlist" },
	{ IEEE80211_IOCTL_GETCHANINFO,
	  0, IW_PRIV_TYPE_CHANINFO | IW_PRIV_SIZE_FIXED,"getchaninfo" },
	{ IEEE80211_IOCTL_SETMODE,
	  IW_PRIV_TYPE_CHAR |  6, 0, "mode" },
	{ IEEE80211_IOCTL_GETMODE,
	  0, IW_PRIV_TYPE_CHAR | 6, "get_mode" },
#if WIRELESS_EXT >= 12	  
	{ IEEE80211_IOCTL_SETWMMPARAMS,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 4, 0,"setwmmparams" },
	{ IEEE80211_IOCTL_GETWMMPARAMS,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 3, 
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1,   "getwmmparams" },
	/*
	 * These depends on sub-ioctl support which added in version 12.
	 */
	{ IEEE80211_IOCTL_SETWMMPARAMS,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 3, 0,"" },
	{ IEEE80211_IOCTL_GETWMMPARAMS,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 2, 
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1,   "" },
	/* sub-ioctl handlers */
	{ IEEE80211_WMMPARAMS_CWMIN,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 3, 0,"cwmin" },
	{ IEEE80211_WMMPARAMS_CWMIN,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 2, 
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1,   "get_cwmin" },
	{ IEEE80211_WMMPARAMS_CWMAX,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 3, 0,"cwmax" },
	{ IEEE80211_WMMPARAMS_CWMAX,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 2, 
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1,   "get_cwmax" },
	{ IEEE80211_WMMPARAMS_AIFS,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 3, 0,"aifs" },
	{ IEEE80211_WMMPARAMS_AIFS,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 2, 
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1,   "get_aifs" },
	{ IEEE80211_WMMPARAMS_TXOPLIMIT,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 3, 0,"txoplimit" },
	{ IEEE80211_WMMPARAMS_TXOPLIMIT,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 2, 
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1,   "get_txoplimit" },
	{ IEEE80211_WMMPARAMS_ACM,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 3, 0,"acm" },
	{ IEEE80211_WMMPARAMS_ACM,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 2, 
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1,   "get_acm" },
	{ IEEE80211_WMMPARAMS_NOACKPOLICY,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 3, 0,"noackpolicy" },
	{ IEEE80211_WMMPARAMS_NOACKPOLICY,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 2, 	
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1,   "get_noackpolicy" },
	
	{ IEEE80211_IOCTL_SETPARAM,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 2, 0, "setparam" },
	/*
	 * These depends on sub-ioctl support which added in version 12.
	 */
	{ IEEE80211_IOCTL_GETPARAM,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1,	"getparam" },

	/* sub-ioctl handlers */
	{ IEEE80211_IOCTL_SETPARAM,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "" },
	{ IEEE80211_IOCTL_GETPARAM,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "" },

	/* sub-ioctl definitions */
	{ IEEE80211_PARAM_AUTHMODE,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "authmode" },
	{ IEEE80211_PARAM_AUTHMODE,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_authmode" },
	{ IEEE80211_PARAM_PROTMODE,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "protmode" },
	{ IEEE80211_PARAM_PROTMODE,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_protmode" },
	{ IEEE80211_PARAM_MCASTCIPHER,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "mcastcipher" },
	{ IEEE80211_PARAM_MCASTCIPHER,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_mcastcipher" },
	{ IEEE80211_PARAM_MCASTKEYLEN,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "mcastkeylen" },
	{ IEEE80211_PARAM_MCASTKEYLEN,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_mcastkeylen" },
	{ IEEE80211_PARAM_UCASTCIPHERS,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "ucastciphers" },
	{ IEEE80211_PARAM_UCASTCIPHERS,
	/*
	 * NB: can't use "get_ucastciphers" 'cuz iwpriv command names
	 *     must be <IFNAMESIZ which is 16.
	 */
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_uciphers" },
	{ IEEE80211_PARAM_UCASTCIPHER,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "ucastcipher" },
	{ IEEE80211_PARAM_UCASTCIPHER,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_ucastcipher" },
	{ IEEE80211_PARAM_UCASTKEYLEN,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "ucastkeylen" },
	{ IEEE80211_PARAM_UCASTKEYLEN,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_ucastkeylen" },
	{ IEEE80211_PARAM_KEYMGTALGS,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "keymgtalgs" },
	{ IEEE80211_PARAM_KEYMGTALGS,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_keymgtalgs" },
	{ IEEE80211_PARAM_RSNCAPS,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "rsncaps" },
	{ IEEE80211_PARAM_RSNCAPS,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_rsncaps" },
	{ IEEE80211_PARAM_ROAMING,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "roaming" },
	{ IEEE80211_PARAM_ROAMING,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_roaming" },
	{ IEEE80211_PARAM_PRIVACY,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "privacy" },
	{ IEEE80211_PARAM_PRIVACY,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_privacy" },
	{ IEEE80211_PARAM_COUNTERMEASURES,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "countermeasures" },
	{ IEEE80211_PARAM_COUNTERMEASURES,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_countermeas" },
	{ IEEE80211_PARAM_DROPUNENCRYPTED,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "dropunencrypted" },
	{ IEEE80211_PARAM_DROPUNENCRYPTED,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_dropunencry" },
	{ IEEE80211_PARAM_WPA,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "wpa" },
	{ IEEE80211_PARAM_WPA,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_wpa" },
	{ IEEE80211_PARAM_DRIVER_CAPS,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "driver_caps" },
	{ IEEE80211_PARAM_DRIVER_CAPS,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_driver_caps" },
	{ IEEE80211_PARAM_MACCMD,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "maccmd" },
	{ IEEE80211_PARAM_WMM,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "wmm" },
	{ IEEE80211_PARAM_WMM,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_wmm" },
	{ IEEE80211_PARAM_HIDESSID,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "hide_ssid" },
	{ IEEE80211_PARAM_HIDESSID,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_hide_ssid" },
	{ IEEE80211_PARAM_APBRIDGE,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "ap_bridge" },
	{ IEEE80211_PARAM_APBRIDGE,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_ap_bridge" },
	{ IEEE80211_PARAM_INACT,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "inact" },
	{ IEEE80211_PARAM_INACT,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_inact" },
	{ IEEE80211_PARAM_INACT_AUTH,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "inact_auth" },
	{ IEEE80211_PARAM_INACT_AUTH,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_inact_auth" },
	{ IEEE80211_PARAM_INACT_INIT,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "inact_init" },
	{ IEEE80211_PARAM_INACT_INIT,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_inact_init" },
	{ IEEE80211_PARAM_ABOLT,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "abolt" },
	{ IEEE80211_PARAM_ABOLT,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_abolt" },
	{ IEEE80211_PARAM_DTIM_PERIOD,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "dtim_period" },
	{ IEEE80211_PARAM_DTIM_PERIOD,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_dtim_period" },
	/* XXX bintval chosen to avoid 16-char limit */
	{ IEEE80211_PARAM_BEACON_INTERVAL,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "bintval" },
	{ IEEE80211_PARAM_BEACON_INTERVAL,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_bintval" },
	{ IEEE80211_PARAM_DOTH,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "doth" },
	{ IEEE80211_PARAM_DOTH,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_doth" },
	{ IEEE80211_PARAM_PWRTARGET,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "doth_pwrtgt" },
	{ IEEE80211_PARAM_PWRTARGET,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_doth_pwrtgt" },
	{ IEEE80211_PARAM_GENREASSOC,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "doth_reassoc" },
	{ IEEE80211_PARAM_COMPRESSION,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "compression" },
	{ IEEE80211_PARAM_COMPRESSION,0,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_compression" },
	{ IEEE80211_PARAM_FF,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "ff" },
	{ IEEE80211_PARAM_FF,0,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_ff" },
	{ IEEE80211_PARAM_TURBO,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "turbo" },
	{ IEEE80211_PARAM_TURBO,0,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_turbo" },
	{ IEEE80211_PARAM_XR,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "xr" },
	{ IEEE80211_PARAM_XR,0,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_xr" },
	{ IEEE80211_PARAM_BURST,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "burst" },
	{ IEEE80211_PARAM_BURST,0,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_burst" },
	{ IEEE80211_IOCTL_CHANSWITCH,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 2, 0,	"doth_chanswitch" },
	{ IEEE80211_PARAM_PUREG,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "pureg" },
	{ IEEE80211_PARAM_PUREG,0,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_pureg" },
	{ IEEE80211_PARAM_AR,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "ar" },
	{ IEEE80211_PARAM_AR,0,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_ar" },
	{ IEEE80211_PARAM_WDS,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "wds" },
	{ IEEE80211_PARAM_WDS,0,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_wds" },
	{ IEEE80211_PARAM_BGSCAN,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "bgscan" },
	{ IEEE80211_PARAM_BGSCAN,0,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_bgscan" },
	{ IEEE80211_PARAM_BGSCAN_IDLE,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "bgscanidle" },
	{ IEEE80211_PARAM_BGSCAN_IDLE,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_bgscanidle" },
	{ IEEE80211_PARAM_BGSCAN_INTERVAL,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "bgscanintvl" },
	{ IEEE80211_PARAM_BGSCAN_INTERVAL,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_bgscanintvl" },
	{ IEEE80211_PARAM_MCAST_RATE,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "mcast_rate" },
	{ IEEE80211_PARAM_MCAST_RATE,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_mcast_rate" },
	{ IEEE80211_PARAM_COVERAGE_CLASS,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "coverageclass" },
	{ IEEE80211_PARAM_COVERAGE_CLASS,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_coveragecls" },
	{ IEEE80211_PARAM_COUNTRY_IE,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "countryie" },
	{ IEEE80211_PARAM_COUNTRY_IE,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_countryie" },
	{ IEEE80211_PARAM_SCANVALID,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "scanvalid" },
	{ IEEE80211_PARAM_SCANVALID,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_scanvalid" },
	{ IEEE80211_PARAM_REGCLASS,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "regclass" },
	{ IEEE80211_PARAM_REGCLASS,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_regclass" },
	/*
	 * NB: these should be roamrssi* etc, but iwpriv usurps all
	 *     strings that start with roam!
	 */
	{ IEEE80211_PARAM_ROAM_RSSI_11A,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "rssi11a" },
	{ IEEE80211_PARAM_ROAM_RSSI_11A,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_rssi11a" },
	{ IEEE80211_PARAM_ROAM_RSSI_11B,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "rssi11b" },
	{ IEEE80211_PARAM_ROAM_RSSI_11B,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_rssi11b" },
	{ IEEE80211_PARAM_ROAM_RSSI_11G,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "rssi11g" },
	{ IEEE80211_PARAM_ROAM_RSSI_11G,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_rssi11g" },
	{ IEEE80211_PARAM_ROAM_RATE_11A,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "rate11a" },
	{ IEEE80211_PARAM_ROAM_RATE_11A,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_rate11a" },
	{ IEEE80211_PARAM_ROAM_RATE_11B,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "rate11b" },
	{ IEEE80211_PARAM_ROAM_RATE_11B,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_rate11b" },
	{ IEEE80211_PARAM_ROAM_RATE_11G,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "rate11g" },
	{ IEEE80211_PARAM_ROAM_RATE_11G,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_rate11g" },
	{ IEEE80211_PARAM_UAPSDINFO,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "uapsd" },
	{ IEEE80211_PARAM_UAPSDINFO,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_uapsd" },
	{ IEEE80211_PARAM_SLEEP,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "sleep" },
	{ IEEE80211_PARAM_SLEEP,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_sleep" },
	{ IEEE80211_PARAM_QOSNULL,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "qosnull" },
	{ IEEE80211_PARAM_PSPOLL,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "pspoll" },
	{ IEEE80211_PARAM_EOSPDROP,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "eospdrop" },
	{ IEEE80211_PARAM_EOSPDROP,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_eospdrop" },
	{ IEEE80211_PARAM_MARKDFS,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "markdfs" },
	{ IEEE80211_PARAM_MARKDFS,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_markdfs" },
	{ IEEE80211_PARAM_CHANBW,
	  IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "chanbw" },
	{ IEEE80211_PARAM_CHANBW,
	  0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "get_chanbw" },

#endif /* WIRELESS_EXT >= 12 */
};

static const iw_handler ieee80211_handlers[] = {
	(iw_handler) NULL,				/* SIOCSIWCOMMIT */
	(iw_handler) ieee80211_ioctl_giwname,		/* SIOCGIWNAME */
	(iw_handler) NULL,				/* SIOCSIWNWID */
	(iw_handler) NULL,				/* SIOCGIWNWID */
	(iw_handler) ieee80211_ioctl_siwfreq,		/* SIOCSIWFREQ */
	(iw_handler) ieee80211_ioctl_giwfreq,		/* SIOCGIWFREQ */
	(iw_handler) ieee80211_ioctl_siwmode,		/* SIOCSIWMODE */
	(iw_handler) ieee80211_ioctl_giwmode,		/* SIOCGIWMODE */
	(iw_handler) ieee80211_ioctl_siwsens,		/* SIOCSIWSENS */
	(iw_handler) ieee80211_ioctl_giwsens,		/* SIOCGIWSENS */
	(iw_handler) NULL /* not used */,		/* SIOCSIWRANGE */
	(iw_handler) ieee80211_ioctl_giwrange,		/* SIOCGIWRANGE */
	(iw_handler) NULL /* not used */,		/* SIOCSIWPRIV */
	(iw_handler) NULL /* kernel code */,		/* SIOCGIWPRIV */
	(iw_handler) NULL /* not used */,		/* SIOCSIWSTATS */
	(iw_handler) NULL /* kernel code */,		/* SIOCGIWSTATS */
	(iw_handler) NULL,				/* SIOCSIWSPY */
	(iw_handler) NULL,				/* SIOCGIWSPY */
	(iw_handler) NULL,				/* -- hole -- */
	(iw_handler) NULL,				/* -- hole -- */
	(iw_handler) ieee80211_ioctl_siwap,		/* SIOCSIWAP */
	(iw_handler) ieee80211_ioctl_giwap,		/* SIOCGIWAP */
	(iw_handler) NULL,				/* -- hole -- */
	(iw_handler) ieee80211_ioctl_iwaplist,		/* SIOCGIWAPLIST */
#ifdef SIOCGIWSCAN
	(iw_handler) ieee80211_ioctl_siwscan,		/* SIOCSIWSCAN */
	(iw_handler) ieee80211_ioctl_giwscan,		/* SIOCGIWSCAN */
#else
	(iw_handler) NULL,				/* SIOCSIWSCAN */
	(iw_handler) NULL,				/* SIOCGIWSCAN */
#endif /* SIOCGIWSCAN */
	(iw_handler) ieee80211_ioctl_siwessid,		/* SIOCSIWESSID */
	(iw_handler) ieee80211_ioctl_giwessid,		/* SIOCGIWESSID */
	(iw_handler) ieee80211_ioctl_siwnickn,		/* SIOCSIWNICKN */
	(iw_handler) ieee80211_ioctl_giwnickn,		/* SIOCGIWNICKN */
	(iw_handler) NULL,				/* -- hole -- */
	(iw_handler) NULL,				/* -- hole -- */
	(iw_handler) ieee80211_ioctl_siwrate,		/* SIOCSIWRATE */
	(iw_handler) ieee80211_ioctl_giwrate,		/* SIOCGIWRATE */
	(iw_handler) ieee80211_ioctl_siwrts,		/* SIOCSIWRTS */
	(iw_handler) ieee80211_ioctl_giwrts,		/* SIOCGIWRTS */
	(iw_handler) ieee80211_ioctl_siwfrag,		/* SIOCSIWFRAG */
	(iw_handler) ieee80211_ioctl_giwfrag,		/* SIOCGIWFRAG */
	(iw_handler) ieee80211_ioctl_siwtxpow,		/* SIOCSIWTXPOW */
	(iw_handler) ieee80211_ioctl_giwtxpow,		/* SIOCGIWTXPOW */
	(iw_handler) ieee80211_ioctl_siwretry,		/* SIOCSIWRETRY */
	(iw_handler) ieee80211_ioctl_giwretry,		/* SIOCGIWRETRY */
	(iw_handler) ieee80211_ioctl_siwencode,		/* SIOCSIWENCODE */
	(iw_handler) ieee80211_ioctl_giwencode,		/* SIOCGIWENCODE */
	(iw_handler) ieee80211_ioctl_siwpower,		/* SIOCSIWPOWER */
	(iw_handler) ieee80211_ioctl_giwpower,		/* SIOCGIWPOWER */
};
static const iw_handler ieee80211_priv_handlers[] = {
	(iw_handler) ieee80211_ioctl_setparam,		/* SIOCWFIRSTPRIV+0 */
	(iw_handler) ieee80211_ioctl_getparam,		/* SIOCWFIRSTPRIV+1 */
	(iw_handler) ieee80211_ioctl_setkey,		/* SIOCWFIRSTPRIV+2 */
	(iw_handler) ieee80211_ioctl_setwmmparams,	/* SIOCWFIRSTPRIV+3 */
	(iw_handler) ieee80211_ioctl_delkey,		/* SIOCWFIRSTPRIV+4 */
	(iw_handler) ieee80211_ioctl_getwmmparams,	/* SIOCWFIRSTPRIV+5 */
	(iw_handler) ieee80211_ioctl_setmlme,		/* SIOCWFIRSTPRIV+6 */
	(iw_handler) ieee80211_ioctl_getchaninfo,	/* SIOCWFIRSTPRIV+7 */
	(iw_handler) ieee80211_ioctl_setoptie,		/* SIOCWFIRSTPRIV+8 */
	(iw_handler) ieee80211_ioctl_getoptie,		/* SIOCWFIRSTPRIV+9 */
	(iw_handler) ieee80211_ioctl_addmac,		/* SIOCWFIRSTPRIV+10 */
	(iw_handler) ieee80211_ioctl_getscanresults,	/* SIOCWFIRSTPRIV+11 */
	(iw_handler) ieee80211_ioctl_delmac,		/* SIOCWFIRSTPRIV+12 */
	(iw_handler) ieee80211_ioctl_getchanlist,	/* SIOCWFIRSTPRIV+13 */
	(iw_handler) ieee80211_ioctl_setchanlist,	/* SIOCWFIRSTPRIV+14 */
	(iw_handler) NULL,				/* SIOCWFIRSTPRIV+15 */
	(iw_handler) ieee80211_ioctl_chanswitch,	/* SIOCWFIRSTPRIV+16 */
	(iw_handler) ieee80211_ioctl_setmode,		/* SIOCWFIRSTPRIV+17 */
	(iw_handler) ieee80211_ioctl_getmode,		/* SIOCWFIRSTPRIV+18 */
#if 0
//********** Ed Lai 05/11/01 /*  Don't Use This One ...*/
	(iw_handler) ieee80211_ioctl_getstationlist,/* SIOCWFIRSTPRIV+19 */
//**************
#endif
};
static struct iw_handler_def ieee80211_iw_handler_def = {
#define	N(a)	(sizeof (a) / sizeof (a[0]))
	.standard		= (iw_handler *) ieee80211_handlers,
	.num_standard		= N(ieee80211_handlers),
	.private		= (iw_handler *) ieee80211_priv_handlers,
	.num_private		= N(ieee80211_priv_handlers),
	.private_args		= (struct iw_priv_args *) ieee80211_priv_args,
	.num_private_args	= N(ieee80211_priv_args),
#undef N
};

static	void ieee80211_delete_wlanunit(u_int);

/*
 * Handle private ioctl requests.
 */
static int
ieee80211_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	struct ieee80211vap *vap = dev->priv;
	u_int unit;
	//printk(KERN_WARNING "%s:%d:cmd (0x%04X) OoOoOoOoOoOoOoO \n",__FUNCTION__,__LINE__,cmd);//ED LAI
	switch (cmd) {
	case SIOCG80211STATS:
		return copy_to_user(ifr->ifr_data, &vap->iv_stats,
				sizeof (vap->iv_stats)) ? -EFAULT : 0;
	case SIOC80211IFDESTROY:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;
		ieee80211_stop(vap->iv_dev);	/* force state before cleanup */
		unit = vap->iv_unit;
		vap->iv_ic->ic_vap_delete(vap);
		ieee80211_delete_wlanunit(unit);
		return 0;
	case IEEE80211_IOCTL_GETKEY:
		return ieee80211_ioctl_getkey(dev, (struct iwreq *) ifr);
	case IEEE80211_IOCTL_GETWPAIE:
		return ieee80211_ioctl_getwpaie(dev, (struct iwreq *) ifr);
	case IEEE80211_IOCTL_STA_STATS:
		return ieee80211_ioctl_getstastats(dev, (struct iwreq *) ifr);
	case IEEE80211_IOCTL_STA_INFO:
		return ieee80211_ioctl_getstainfo(dev, (struct iwreq *) ifr);
	case IEEE80211_IOCTL_SCAN_RESULTS:
		return ieee80211_ioctl_getscanresults(dev, (struct iwreq *)ifr);
#if 1
// ************** Ed Lai 05/10/31 reponse use-space for station list
	case IEEE80211_IOCTL_WLACONN:
		{ 
		struct ieee80211vap *vap = dev->priv;
		struct ieee80211com *ic = vap->iv_ic;
		struct ieee80211_node *ni;
		struct ieee80211_node_table *nt = &ic->ic_sta;
		int listnum = 0;
		int error;

		WLA_CONN *list;
		int list_size = WLA_CONN_SIZE * sizeof(WLA_CONN);
		list = kmalloc(list_size, GFP_KERNEL);
		if(list == NULL) {
			error = -EFAULT;
		} else {
			memset(list, 0, list_size);
			IEEE80211_NODE_LOCK(nt);
			TAILQ_FOREACH(ni, &nt->nt_node, ni_list) {
				if(ni->ni_associd == 0)
					continue;
				IEEE80211_ADDR_COPY(list[listnum].mac, ni->ni_macaddr);
				list[listnum].assoc_time = ni->ni_assoctime;
				if(++listnum == WLA_CONN_SIZE)
					break;
			}
			IEEE80211_NODE_UNLOCK(ni->ni_table);
			
			if (copy_to_user(ifr->ifr_data, list, list_size))
				error = -EFAULT;
			else
				error = 0;
		
			kfree(list);
		}
		return error;	
		} 
		break;
		
		/*[Don't Use This One]return ieee80211_ioctl_getstationlist(dev, (struct iwreq *)ifr);*/
		
                //Added By Ed Lai 05/11/07
                case IEEE80211_IOCTL_GATHDRVVER:
		{
			int error;	
			if (copy_to_user(ifr->ifr_data, ATH_PCI_VERSION, 32))
				error = -EFAULT;
			else
				error = 0;
			return error;	
		}
                break;

#endif
// ********************************
	}
	return -EOPNOTSUPP;
}

static u_int8_t wlan_units[32];		/* enough for 256 */

/*
 * Allocate a new unit number.  If the map is full return -1;
 * otherwise the allocate unit number is returned.
 */
static int
ieee80211_new_wlanunit(void)
{
#define	N(a)	(sizeof(a)/sizeof(a[0]))
	u_int unit;
	u_int8_t b;
	int i;

	/* NB: covered by rtnl_lock */
	unit = 0;
	for (i = 0; i < N(wlan_units) && wlan_units[i] == 0xff; i++)
		unit += NBBY;
	if (i == N(wlan_units))
		return -1;
	for (b = wlan_units[i]; b & 1; b >>= 1)
		unit++;
	setbit(wlan_units, unit);

	return unit;
#undef N
}

/*
 * Check if the specified unit number is available and, if
 * so, mark it in use.  Return 1 on success, 0 on failure.
 */
static int
ieee80211_alloc_wlanunit(u_int unit)
{
	/* NB: covered by rtnl_lock */
	if (unit < sizeof(wlan_units)*NBBY && isclr(wlan_units, unit)) {
		setbit(wlan_units, unit);
		return 1;
	} else
		return 0;
}

/*
 * Reclaim the specified unit number.
 */
static void
ieee80211_delete_wlanunit(u_int unit)
{
	/* NB: covered by rtnl_lock */
	KASSERT(unit < sizeof(wlan_units)*NBBY, ("invalid wlan unit %u", unit));
	KASSERT(isset(wlan_units, unit), ("wlan unit %u not allocated", unit));
	clrbit(wlan_units, unit);
}

/*
 * Extract a unit number from an interface name.  If no unit
 * number is specified then -1 is returned for the unit number.
 * Return 0 on success or an error code.
 */
static int
ifc_name2unit(const char *name, int *unit)
{
	const char *cp;

	for (cp = name; *cp != '\0' && !('0' <= *cp && *cp <= '9'); cp++)
		;
	if (*cp != '\0') {
		*unit = 0;
		for (; *cp != '\0'; cp++) {
			if (!('0' <= *cp && *cp <= '9'))
				return -EINVAL;
			*unit = (*unit * 10) + (*cp - '0');
		}
	} else
		*unit = -1;
	return 0;
}

/*
 * Create a virtual ap.  This is public as it must be implemented
 * outside our control (e.g. in the driver).
 */
int
ieee80211_ioctl_create_vap(struct ieee80211com *ic, struct ifreq *ifr)
{
	struct ieee80211_clone_params cp;
	struct ieee80211vap *vap;
	char name[IFNAMSIZ];
	int error, unit;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;
	if (copy_from_user(&cp, ifr->ifr_data, sizeof(cp)))
		return -EFAULT;
	error = ifc_name2unit(cp.icp_name, &unit);
	if (error)
		return error;
	if (unit == -1) {
		unit = ieee80211_new_wlanunit();
		if (unit == -1)
			return -EIO;		/* XXX */
		/* XXX verify name fits */
		snprintf(name, sizeof(name), "%s%d", cp.icp_name, unit);
	} else {
		if (!ieee80211_alloc_wlanunit(unit))
			return -EINVAL;
		strncpy(name, cp.icp_name, sizeof(name));
	}
	vap = ic->ic_vap_create(ic, name, unit, cp.icp_opmode, cp.icp_flags);
	if (vap == NULL) {
		ieee80211_delete_wlanunit(unit);
		return -EIO;
	}
	/* return final device name */
	strncpy(ifr->ifr_name, vap->iv_dev->name, IFNAMSIZ);
	return 0;
}
EXPORT_SYMBOL(ieee80211_ioctl_create_vap);

void
ieee80211_ioctl_vattach(struct ieee80211vap *vap)
{
	struct net_device *dev = vap->iv_dev;

	dev->do_ioctl = ieee80211_ioctl;
	dev->get_wireless_stats = ieee80211_iw_getstats;
	dev->wireless_handlers = &ieee80211_iw_handler_def;
}

void
ieee80211_ioctl_vdetach(struct ieee80211vap *vap)
{
}
#endif /* CONFIG_NET_WIRELESS */
