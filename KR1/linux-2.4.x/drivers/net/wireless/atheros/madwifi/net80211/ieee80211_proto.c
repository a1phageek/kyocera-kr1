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
 */
#ifndef EXPORT_SYMTAB
#define	EXPORT_SYMTAB
#endif

__FBSDID("$FreeBSD: src/sys/net80211/ieee80211_proto.c,v 1.6 2003/10/31 18:32:09 brooks Exp $");
__KERNEL_RCSID(0, "$NetBSD: ieee80211_proto.c,v 1.5 2003/10/13 04:23:56 dyoung Exp $");

/*
 * IEEE 802.11 protocol support.
 */
#include <linux/config.h>
#include <linux/version.h>
#include <linux/kmod.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>

#include "if_media.h"

#include <net80211/ieee80211_var.h>
#include <net80211/ieee80211_dot1x.h>

#define	IEEE80211_RATE2MBS(r)	(((r) & IEEE80211_RATE_VAL) / 2)

const char *ieee80211_mgt_subtype_name[] = {
	"assoc_req",	"assoc_resp",	"reassoc_req",	"reassoc_resp",
	"probe_req",	"probe_resp",	"reserved#6",	"reserved#7",
	"beacon",	"atim",		"disassoc",	"auth",
	"deauth",	"reserved#13",	"reserved#14",	"reserved#15"
};
EXPORT_SYMBOL(ieee80211_mgt_subtype_name);
const char *ieee80211_ctl_subtype_name[] = {
	"reserved#0",	"reserved#1",	"reserved#2",	"reserved#3",
	"reserved#3",	"reserved#5",	"reserved#6",	"reserved#7",
	"reserved#8",	"reserved#9",	"ps_poll",	"rts",
	"cts",		"ack",		"cf_end",	"cf_end_ack"
};
EXPORT_SYMBOL(ieee80211_ctl_subtype_name);
const char *ieee80211_state_name[IEEE80211_S_MAX] = {
	"INIT",		/* IEEE80211_S_INIT */
	"SCAN",		/* IEEE80211_S_SCAN */
	"AUTH",		/* IEEE80211_S_AUTH */
	"ASSOC",	/* IEEE80211_S_ASSOC */
	"RUN"		/* IEEE80211_S_RUN */
};
EXPORT_SYMBOL(ieee80211_state_name);

static int ieee80211_newstate(struct ieee80211com *, enum ieee80211_state, int);
void ieee80211_auth_setup(void);

void ieee80211_proto_initWmeParams(struct ieee80211com *ic)
{
	typedef struct phyParamType { 
		u_int8_t aifsn; 
		u_int8_t logcwmin;
		u_int8_t logcwmax; 
		u_int16_t txopLimit;
		u_int8_t acm;
	} paramType;

	paramType *pPhyParam, *pBssPhyParam;

	static struct phyParamType phyParamForAC_BE[IEEE80211_MODE_MAX] = {
	/* IEEE80211_MODE_AUTO  */ {          3,                4,                6,                  0,              0 },    
	/* IEEE80211_MODE_11A   */ {          3,                4,                6,                  0,              0 },
	/* IEEE80211_MODE_11B   */ {          3,                5,                7,                  0,              0 },
	/* IEEE80211_MODE_11G   */ {          3,                4,                6,                  0,              0 },
	/* IEEE80211_MODE_FH    */ {          3,                5,                7,                  0,              0 },
	/* IEEE80211_MODE_TURBO */ {          2,                3,                5,                  0,              0 },
	/* IEEE80211_MODE_TURBO */ {          2,                3,                5,                  0,              0 }};
	static struct phyParamType phyParamForAC_BK[IEEE80211_MODE_MAX] = {
        /* IEEE80211_MODE_AUTO  */ {          7,                4,               10,                 0,              0 },
        /* IEEE80211_MODE_11A   */ {          7,                4,               10,                 0,              0 },
        /* IEEE80211_MODE_11B   */ {          7,                5,               10,                 0,              0 },
        /* IEEE80211_MODE_11G   */ {          7,                4,               10,                 0,              0 },
	/* IEEE80211_MODE_FH    */ {          7,                5,               10,                 0,              0 },
	/* IEEE80211_MODE_TURBO */ {          7,                3,               10,                 0,              0 },
	/* IEEE80211_MODE_TURBO */ {          7,                3,               10,                 0,              0 }};
	static struct phyParamType phyParamForAC_VI[IEEE80211_MODE_MAX] = {
	/* IEEE80211_MODE_AUTO  */ {          1,                3,               4,                  94,              0 },
	/* IEEE80211_MODE_11A   */ {          1,                3,               4,                  94,              0 },
	/* IEEE80211_MODE_11B   */ {          1,                4,               5,                 188,              0 },
	/* IEEE80211_MODE_11G   */ {          1,                3,               4,                  94,              0 },
	/* IEEE80211_MODE_FH    */ {          1,                4,               5,                 188,              0 },
	/* IEEE80211_MODE_TURBO */ {          1,                2,               3,                  94,              0 },
	/* IEEE80211_MODE_TURBO */ {          1,                2,               3,                  94,              0 }};
	static struct phyParamType phyParamForAC_VO[IEEE80211_MODE_MAX] = {
        /* IEEE80211_MODE_AUTO  */ {          1,                2,               3,                  47,              0 },
	/* IEEE80211_MODE_11A   */ {          1,                2,               3,                  47,              0 },
        /* IEEE80211_MODE_11B   */ {          1,                3,               4,                 102,              0 },
        /* IEEE80211_MODE_11G   */ {          1,                2,               3,                  47,              0 },
	/* IEEE80211_MODE_FH    */ {          1,                3,               4,                 102,              0 },
        /* IEEE80211_MODE_TURBO */ {          1,                2,               2,                  47,              0 },
        /* IEEE80211_MODE_TURBO */ {          1,                2,               2,                  47,              0 }};

	static struct phyParamType bssPhyParamForAC_BE[IEEE80211_MODE_MAX] = {
        /* IEEE80211_MODE_AUTO  */ {          3,                4,              10,                  0,              0 },
        /* IEEE80211_MODE_11A   */ {          3,                4,              10,                  0,              0 },
        /* IEEE80211_MODE_11B   */ {          3,                5,              10,                  0,              0 },
        /* IEEE80211_MODE_11G   */ {          3,                4,              10,                  0,              0 },
        /* IEEE80211_MODE_FH    */ {          3,                5,              10,                  0,              0 },
        /* IEEE80211_MODE_TURBO */ {          2,                3,              10,                  0,              0 },
        /* IEEE80211_MODE_TURBO */ {          2,                3,              10,                  0,              0 }};
	static struct phyParamType bssPhyParamForAC_VI[IEEE80211_MODE_MAX] = {
        /* IEEE80211_MODE_AUTO  */ {          2,                3,               4,                  94,              0 },
        /* IEEE80211_MODE_11A   */ {          2,                3,               4,                  94,              0 },
        /* IEEE80211_MODE_11B   */ {          2,                4,               5,                 188,              0 },
        /* IEEE80211_MODE_11G   */ {          2,                3,               4,                  94,              0 },
        /* IEEE80211_MODE_FH    */ {          2,                4,               5,                 188,              0 },
        /* IEEE80211_MODE_TURBO */ {          2,                2,               3,                  94,              0 },
        /* IEEE80211_MODE_TURBO */ {          2,                2,               3,                  94,              0 }};
	static struct phyParamType bssPhyParamForAC_VO[IEEE80211_MODE_MAX] = {
        /* IEEE80211_MODE_AUTO  */ {          2,                2,               3,                  47,              0 },    
        /* IEEE80211_MODE_11A   */ {          2,                2,               3,                  47,              0 },
        /* IEEE80211_MODE_11B   */ {          2,                3,               4,                 102,              0 },
        /* IEEE80211_MODE_11G   */ {          2,                2,               3,                  47,              0 },
        /* IEEE80211_MODE_FH    */ {          2,                3,               4,                 102,              0 },
	/* IEEE80211_MODE_TURBO */ {          1,                2,               2,                  47,              0 },
	/* IEEE80211_MODE_TURBO */ {          1,                2,               2,                  47,              0 }};

	int i;
	for (i = 0; i < WME_NUM_AC; i++) {
		switch (i) {
		case WME_AC_BK:
			pPhyParam = &phyParamForAC_BK[ic->ic_curmode];
			pBssPhyParam = &phyParamForAC_BK[ic->ic_curmode];
			break;
		case WME_AC_VI:
			pPhyParam = &phyParamForAC_VI[ic->ic_curmode];				
			pBssPhyParam = &bssPhyParamForAC_VI[ic->ic_curmode];			
			break;
		case WME_AC_VO:
			pPhyParam = &phyParamForAC_VO[ic->ic_curmode];
			pBssPhyParam = &bssPhyParamForAC_VO[ic->ic_curmode];			
			break;
		case WME_AC_BE:
		default:
			pPhyParam = &phyParamForAC_BE[ic->ic_curmode];		
			pBssPhyParam = &bssPhyParamForAC_BE[ic->ic_curmode];
			break;
		}
	
		if (ic->ic_opmode == IEEE80211_M_HOSTAP) {
			ic->ic_wmeChanParams.cap_wmeParams[i].wmep_acm = pPhyParam->acm;
			ic->ic_wmeChanParams.cap_wmeParams[i].wmep_aifsn = pPhyParam->aifsn;	
			ic->ic_wmeChanParams.cap_wmeParams[i].wmep_logcwmin = pPhyParam->logcwmin;	
			ic->ic_wmeChanParams.cap_wmeParams[i].wmep_logcwmax = pPhyParam->logcwmax;		
			ic->ic_wmeChanParams.cap_wmeParams[i].wmep_txopLimit = pPhyParam->txopLimit;
		} else {
			ic->ic_wmeChanParams.cap_wmeParams[i].wmep_acm = pBssPhyParam->acm;
			ic->ic_wmeChanParams.cap_wmeParams[i].wmep_aifsn = pBssPhyParam->aifsn;	
			ic->ic_wmeChanParams.cap_wmeParams[i].wmep_logcwmin = pBssPhyParam->logcwmin;	
			ic->ic_wmeChanParams.cap_wmeParams[i].wmep_logcwmax = pBssPhyParam->logcwmax;		
			ic->ic_wmeChanParams.cap_wmeParams[i].wmep_txopLimit = pBssPhyParam->txopLimit;

		}	
		ic->ic_wmeBssChanParams.cap_wmeParams[i].wmep_acm = pBssPhyParam->acm;
		ic->ic_wmeBssChanParams.cap_wmeParams[i].wmep_aifsn = pBssPhyParam->aifsn;	
		ic->ic_wmeBssChanParams.cap_wmeParams[i].wmep_logcwmin = pBssPhyParam->logcwmin;	
		ic->ic_wmeBssChanParams.cap_wmeParams[i].wmep_logcwmax = pBssPhyParam->logcwmax;		
		ic->ic_wmeBssChanParams.cap_wmeParams[i].wmep_txopLimit = pBssPhyParam->txopLimit;
	}
}
EXPORT_SYMBOL(ieee80211_proto_initWmeParams);

void ieee80211_proto_updateChanParams(struct ieee80211com *ic)
{
	static const struct           { u_int8_t aifsn; u_int8_t logcwmin; u_int8_t logcwmax; u_int16_t txopLimit;}
	phyParam[IEEE80211_MODE_MAX] = {
          /* IEEE80211_MODE_AUTO  */ {          2,                4,               10, 			64 },	
          /* IEEE80211_MODE_11A   */ {          2,                4,               10, 			64 },
          /* IEEE80211_MODE_11B   */ {          2,                5,               10, 			64 },
          /* IEEE80211_MODE_11G   */ {          2,                4,               10,			64 },
          /* IEEE80211_MODE_FH    */ {          2,                5,               10,			64 },
          /* IEEE80211_MODE_TURBO */ {          1,                3,               10,			64 }};
	int i;
	
       	/* set up the channel access parameters for the physical device */

	for (i = 0; i < WME_NUM_AC; i++) {
		ic->ic_chanParams.cap_wmeParams[i].wmep_aifsn 
				= ic->ic_wmeChanParams.cap_wmeParams[i].wmep_aifsn;
		ic->ic_chanParams.cap_wmeParams[i].wmep_logcwmin 
				= ic->ic_wmeChanParams.cap_wmeParams[i].wmep_logcwmin;
		ic->ic_chanParams.cap_wmeParams[i].wmep_logcwmax 
				= ic->ic_wmeChanParams.cap_wmeParams[i].wmep_logcwmax;
		ic->ic_chanParams.cap_wmeParams[i].wmep_txopLimit 
				= ic->ic_wmeChanParams.cap_wmeParams[i].wmep_txopLimit;
		ic->ic_bssChanParams.cap_wmeParams[i].wmep_aifsn 
				= ic->ic_wmeBssChanParams.cap_wmeParams[i].wmep_aifsn;
		ic->ic_bssChanParams.cap_wmeParams[i].wmep_logcwmin 
				= ic->ic_wmeBssChanParams.cap_wmeParams[i].wmep_logcwmin;
		ic->ic_bssChanParams.cap_wmeParams[i].wmep_logcwmax 
				= ic->ic_wmeBssChanParams.cap_wmeParams[i].wmep_logcwmax;
		ic->ic_bssChanParams.cap_wmeParams[i].wmep_txopLimit 
				= ic->ic_wmeBssChanParams.cap_wmeParams[i].wmep_txopLimit;
	}

        if (((ic->ic_opmode == IEEE80211_M_HOSTAP) && (!ic->ic_nonAggressiveMode)) 
	 || ((ic->ic_opmode != IEEE80211_M_HOSTAP) && (!(ic->ic_bss->ni_flags & IEEE80211_NODE_QOS)))
	 || (!(ic->ic_flags & IEEE80211_F_WME))) {
		ic->ic_chanParams.cap_wmeParams[WME_AC_BE].wmep_aifsn = phyParam[ic->ic_curmode].aifsn;
		ic->ic_chanParams.cap_wmeParams[WME_AC_BE].wmep_logcwmin = phyParam[ic->ic_curmode].logcwmin;
		ic->ic_chanParams.cap_wmeParams[WME_AC_BE].wmep_logcwmax = phyParam[ic->ic_curmode].logcwmax;		
		ic->ic_chanParams.cap_wmeParams[WME_AC_BE].wmep_txopLimit 
				= (ic->ic_ath_cap & IEEE80211_ATHC_BURST) ? phyParam[ic->ic_curmode].txopLimit : 0;
		ic->ic_bssChanParams.cap_wmeParams[WME_AC_BE].wmep_aifsn = phyParam[ic->ic_curmode].aifsn;
		ic->ic_bssChanParams.cap_wmeParams[WME_AC_BE].wmep_logcwmin = phyParam[ic->ic_curmode].logcwmin;
		ic->ic_bssChanParams.cap_wmeParams[WME_AC_BE].wmep_logcwmax = phyParam[ic->ic_curmode].logcwmax;
		ic->ic_bssChanParams.cap_wmeParams[WME_AC_BE].wmep_txopLimit 
				= (ic->ic_ath_cap & IEEE80211_ATHC_BURST) ? phyParam[ic->ic_curmode].txopLimit : 0;		
	}
	
	if ((ic->ic_opmode == IEEE80211_M_HOSTAP) && (!ic->ic_nonAggressiveMode) &&
	    (ic->ic_ath_cap & IEEE80211_ATHC_WME)) {
        	static const u_int8_t logCwMin[IEEE80211_MODE_MAX] = {
              		/* IEEE80211_MODE_AUTO  */   3,		
              		/* IEEE80211_MODE_11A   */   3,
              		/* IEEE80211_MODE_11B   */   4,
              		/* IEEE80211_MODE_11G   */   3,
              		/* IEEE80211_MODE_FH    */   4,	
              		/* IEEE80211_MODE_TURBO */   3 };
    
/* MW */       	if (ic->ic_numClientsAssoc < 2) {
			ic->ic_chanParams.cap_wmeParams[WME_AC_BE].wmep_logcwmin 
				= ic->ic_bssChanParams.cap_wmeParams[WME_AC_BE].wmep_logcwmin 
				= logCwMin[ic->ic_curmode];
			
        	}
    	}	
}
EXPORT_SYMBOL(ieee80211_proto_updateChanParams);

void
ieee80211_proto_attach(struct ieee80211com *ic)
{

#ifdef notdef
	ic->ic_rtsthreshold = IEEE80211_RTS_DEFAULT;
#else
	ic->ic_rtsthreshold = IEEE80211_RTS_MAX;
#endif
	ic->ic_fragthreshold = 2346;		/* XXX not used yet */
	ic->ic_fixed_rate = -1;			/* no fixed rate */
	ic->ic_protmode = IEEE80211_PROT_CTSONLY;
	ic->ic_roaming = IEEE80211_ROAMING_AUTO;

	/* protocol state change handler */
	ic->ic_newstate = ieee80211_newstate;

	/* initialize management frame handlers */
	ic->ic_recv_mgmt = ieee80211_recv_mgmt;
	ic->ic_send_mgmt = ieee80211_send_mgmt;
	ieee80211_auth_setup();
}

void
ieee80211_proto_detach(struct ieee80211com *ic)
{
	/*
	 * This should not be needed as we detach when reseting
	 * the state but be conservative here since the
	 * authenticator may do things like spawn kernel threads.
	 */
	if (ic->ic_auth->ia_detach)
		ic->ic_auth->ia_detach(ic);

	/*
	 * Detach any ACL'ator.
	 */
	if (ic->ic_acl != NULL)
		ic->ic_acl->iac_detach(ic);
}

/*
 * Simple-minded authenticator module support.
 */

#define	IEEE80211_AUTH_MAX	(IEEE80211_AUTH_WPA+1)
/* XXX well-known names */
static const char *auth_modnames[IEEE80211_AUTH_MAX] = {
	"wlan_internal",	/* IEEE80211_AUTH_NONE */
	"wlan_internal",	/* IEEE80211_AUTH_OPEN */
	"wlan_internal",	/* IEEE80211_AUTH_SHARED */
	"wlan_xauth",		/* IEEE80211_AUTH_8021X	 */
	"wlan_internal",	/* IEEE80211_AUTH_AUTO */
	"wlan_xauth",		/* IEEE80211_AUTH_WPA */
};
static const struct ieee80211_authenticator *authenticators[IEEE80211_AUTH_MAX];

static const struct ieee80211_authenticator auth_internal = {
	.ia_name		= "wlan_internal",
	.ia_attach		= NULL,
	.ia_detach		= NULL,
	.ia_node_join		= NULL,
	.ia_node_leave		= NULL,
};

/*
 * Setup internal authenticators once; they are never unregistered.
 */
void
ieee80211_auth_setup(void)
{
	ieee80211_authenticator_register(IEEE80211_AUTH_OPEN, &auth_internal);
	ieee80211_authenticator_register(IEEE80211_AUTH_SHARED, &auth_internal);
	ieee80211_authenticator_register(IEEE80211_AUTH_AUTO, &auth_internal);
}

const struct ieee80211_authenticator *
ieee80211_authenticator_get(int auth)
{
	if (auth >= IEEE80211_AUTH_MAX)
		return NULL;
	if (authenticators[auth] == NULL)
		request_module(auth_modnames[auth]);
	return authenticators[auth];
}

void
ieee80211_authenticator_register(int type,
	const struct ieee80211_authenticator *auth)
{
	if (type >= IEEE80211_AUTH_MAX)
		return;
	authenticators[type] = auth;
}
EXPORT_SYMBOL(ieee80211_authenticator_register);

void
ieee80211_authenticator_unregister(int type)
{
	if (type >= IEEE80211_AUTH_MAX)
		return;
	authenticators[type] = NULL;
}
EXPORT_SYMBOL(ieee80211_authenticator_unregister);

/*
 * Very simple-minded authenticator backend module support.
 */
/* XXX just one for now */
static	const struct ieee80211_authenticator_backend *backend = NULL;

void
ieee80211_authenticator_backend_register(
	const struct ieee80211_authenticator_backend *be)
{
	printk(KERN_INFO "wlan: %s backend registered\n", be->iab_name);
	backend = be;
}
EXPORT_SYMBOL(ieee80211_authenticator_backend_register);

void
ieee80211_authenticator_backend_unregister(
	const struct ieee80211_authenticator_backend * be)
{
	if (backend == be)
		backend = NULL;
	printk(KERN_INFO "wlan: %s backend unregistered\n",
		be->iab_name);
}
EXPORT_SYMBOL(ieee80211_authenticator_backend_unregister);

const struct ieee80211_authenticator_backend *
ieee80211_authenticator_backend_get(const char *name)
{
	if (backend == NULL)
		request_module("wlan_radius");
	return backend && strcmp(backend->iab_name, name) == 0 ? backend : NULL;
}
EXPORT_SYMBOL(ieee80211_authenticator_backend_get);

/*
 * Very simple-minded ACL module support.
 */
/* XXX just one for now */
static	const struct ieee80211_aclator *acl = NULL;

void
ieee80211_aclator_register(const struct ieee80211_aclator *iac)
{
	printk(KERN_INFO "wlan: %s acl policy registered\n", iac->iac_name);
	acl = iac;
}
EXPORT_SYMBOL(ieee80211_aclator_register);

void
ieee80211_aclator_unregister(const struct ieee80211_aclator *iac)
{
	if (acl == iac)
		acl = NULL;
	printk(KERN_INFO "wlan: %s acl policy unregistered\n", iac->iac_name);
}
EXPORT_SYMBOL(ieee80211_aclator_unregister);

const struct ieee80211_aclator *
ieee80211_aclator_get(const char *name)
{
	if (acl == NULL)
		request_module("wlan_acl");
	return acl && strcmp(acl->iac_name, name) == 0 ? acl : NULL;
}
EXPORT_SYMBOL(ieee80211_aclator_get);

void
ieee80211_print_essid(const u_int8_t *essid, int len)
{
	int i;
	const u_int8_t *p; 

	if (len > IEEE80211_NWID_LEN)
		len = IEEE80211_NWID_LEN;
	/* determine printable or not */
	for (i = 0, p = essid; i < len; i++, p++) {
		if (*p < ' ' || *p > 0x7e)
			break;
	}
	if (i == len) {
		printf("\"");
		for (i = 0, p = essid; i < len; i++, p++)
			printf("%c", *p);
		printf("\"");
	} else {
		printf("0x");
		for (i = 0, p = essid; i < len; i++, p++)
			printf("%02x", *p);
	}
}

void
ieee80211_dump_pkt(struct ieee80211com *ic,
	const u_int8_t *buf, int len, int rate, int rssi)
{
	const struct ieee80211_frame *wh;
	int i;

	wh = (const struct ieee80211_frame *)buf;
	switch (wh->i_fc[1] & IEEE80211_FC1_DIR_MASK) {
	case IEEE80211_FC1_DIR_NODS:
		printf("NODS %s", ether_sprintf(wh->i_addr2));
		printf("->%s", ether_sprintf(wh->i_addr1));
		printf("(%s)", ether_sprintf(wh->i_addr3));
		break;
	case IEEE80211_FC1_DIR_TODS:
		printf("TODS %s", ether_sprintf(wh->i_addr2));
		printf("->%s", ether_sprintf(wh->i_addr3));
		printf("(%s)", ether_sprintf(wh->i_addr1));
		break;
	case IEEE80211_FC1_DIR_FROMDS:
		printf("FRDS %s", ether_sprintf(wh->i_addr3));
		printf("->%s", ether_sprintf(wh->i_addr1));
		printf("(%s)", ether_sprintf(wh->i_addr2));
		break;
	case IEEE80211_FC1_DIR_DSTODS:
		printf("DSDS %s", ether_sprintf((u_int8_t *)&wh[1]));
		printf("->%s", ether_sprintf(wh->i_addr3));
		printf("(%s", ether_sprintf(wh->i_addr2));
		printf("->%s)", ether_sprintf(wh->i_addr1));
		break;
	}
	switch (wh->i_fc[0] & IEEE80211_FC0_TYPE_MASK) {
	case IEEE80211_FC0_TYPE_DATA:
		printf(" data");
		break;
	case IEEE80211_FC0_TYPE_MGT:
		printf(" %s", ieee80211_mgt_subtype_name[
		    (wh->i_fc[0] & IEEE80211_FC0_SUBTYPE_MASK)
		    >> IEEE80211_FC0_SUBTYPE_SHIFT]);
		break;
	default:
		printf(" type#%d", wh->i_fc[0] & IEEE80211_FC0_TYPE_MASK);
		break;
	}
	if (IEEE80211_QOS_HAS_SEQ(wh)) {
		const struct ieee80211_qosframe *qwh = 
			(const struct ieee80211_qosframe *)buf;
		printf(" QoS [TID %u%s]", qwh->i_qos[0] & IEEE80211_QOS_TID,
			qwh->i_qos[0] & IEEE80211_QOS_ACKPOLICY ? " ACM" : "");
	}
	if (wh->i_fc[1] & IEEE80211_FC1_WEP) {
		int off;

		off = ieee80211_anyhdrspace(ic, wh);
		printf(" WEP [IV %.02x %.02x %.02x",
			buf[off+0], buf[off+1], buf[off+2]);
		if (buf[off+IEEE80211_WEP_IVLEN] & IEEE80211_WEP_EXTIV)
			printf(" %.02x %.02x %.02x",
				buf[off+4], buf[off+5], buf[off+6]);
		printf(" KID %u]", buf[off+IEEE80211_WEP_IVLEN] >> 6);
	}
	if (rate >= 0)
		printf(" %dM", rate / 2);
	if (rssi >= 0)
		printf(" +%d", rssi);
	printf("\n");
	if (len > 0) {
		for (i = 0; i < len; i++) {
			if ((i & 1) == 0)
				printf(" ");
			printf("%02x", buf[i]);
		}
		printf("\n");
	}
}
EXPORT_SYMBOL(ieee80211_dump_pkt);

int
ieee80211_fix_rate(struct ieee80211com *ic, struct ieee80211_node *ni, int flags)
{
#define	RV(v)	((v) & IEEE80211_RATE_VAL)
	int i, j, ignore, error;
	int okrate, badrate;
	struct ieee80211_rateset *srs, *nrs;
	u_int8_t r;

	error = 0;
	okrate = badrate = 0;
	srs = &ic->ic_sup_rates[ieee80211_chan2mode(ic, ni->ni_chan)];
	nrs = &ni->ni_rates;
	for (i = 0; i < nrs->rs_nrates; ) {
		ignore = 0;
		if (flags & IEEE80211_F_DOSORT) {
			/*
			 * Sort rates.
			 */
			for (j = i + 1; j < nrs->rs_nrates; j++) {
				if (RV(nrs->rs_rates[i]) > RV(nrs->rs_rates[j])) {
					r = nrs->rs_rates[i];
					nrs->rs_rates[i] = nrs->rs_rates[j];
					nrs->rs_rates[j] = r;
				}
			}
		}
		r = nrs->rs_rates[i] & IEEE80211_RATE_VAL;
		badrate = r;
		if (flags & IEEE80211_F_DOFRATE) {
			/*
			 * Apply fixed rate constraint.  Note that we do
			 * not apply the constraint to basic rates as
			 * otherwise we may not be able to associate if
			 * the rate set we submit to the AP is invalid
			 * (e.g. fix rate at 36Mb/s which is not a basic
			 * rate for 11a operation).
			 */
			if ((nrs->rs_rates[i] & IEEE80211_RATE_BASIC) == 0 &&
			    ic->ic_fixed_rate >= 0 &&
			    r != RV(srs->rs_rates[ic->ic_fixed_rate]))
				ignore++;
		}
		if (flags & IEEE80211_F_DONEGO) {
			/*
			 * Check against supported rates.
			 */
			for (j = 0; j < srs->rs_nrates; j++) {
				if (r == RV(srs->rs_rates[j])) {
					/*
					 * Overwrite with the supported rate
					 * value so any basic rate bit is set.
					 * This insures that response we send
					 * to stations have the necessary basic
					 * rate bit set.
					 */
					nrs->rs_rates[i] = srs->rs_rates[j];
					break;
				}
			}
			if (j == srs->rs_nrates) {
				/*
				 * A rate in the node's rate set is not
				 * supported.  If this is a basic rate and we
				 * are operating as an AP then this is an error.
				 * Otherwise we just discard/ignore the rate.
				 * Note that this is important for 11b stations
				 * when they want to associate with an 11g AP.
				 */
				if (ic->ic_opmode == IEEE80211_M_HOSTAP &&
				    (nrs->rs_rates[i] & IEEE80211_RATE_BASIC))
					error++;
				ignore++;
			}
		}
		if (flags & IEEE80211_F_DODEL) {
			/*
			 * Delete unacceptable rates.
			 */
			if (ignore) {
				nrs->rs_nrates--;
				for (j = i; j < nrs->rs_nrates; j++)
					nrs->rs_rates[j] = nrs->rs_rates[j + 1];
				nrs->rs_rates[j] = 0;
				continue;
			}
		}
		if (!ignore)
			okrate = nrs->rs_rates[i];
		i++;
	}
	if (okrate == 0 || error != 0)
		return badrate | IEEE80211_RATE_BASIC;
	else
		return RV(okrate);
#undef RV
}

/*
 * Check if the specified rate set supports ERP.
 * NB: the rate set is assumed to be sorted.
 */
int
ieee80211_iserp_rateset(struct ieee80211com *ic, struct ieee80211_rateset *rs)
{
#define N(a)	(sizeof(a) / sizeof(a[0]))
	static const int rates[] = { 2, 4, 11, 22, 12, 24, 48 };
	int i, j;

	if (rs->rs_nrates < N(rates))
		return 0;
	for (i = 0; i < N(rates); i++) {
		for (j = 0; j < rs->rs_nrates; j++) {
			int r = rs->rs_rates[j] & IEEE80211_RATE_VAL;
			if (rates[i] == r)
				goto next;
			if (r > rates[i])
				return 0;
		}
		return 0;
	next:
		;
	}
	return 1;
#undef N
}

static int
ieee80211_newstate(struct ieee80211com *ic, enum ieee80211_state nstate, int arg)
{
	struct net_device *dev = ic->ic_dev;
	struct ieee80211_node *ni;
	enum ieee80211_state ostate;

	ostate = ic->ic_state;
	IEEE80211_DPRINTF(ic, IEEE80211_MSG_STATE, ("%s: %s -> %s\n", __func__,
		ieee80211_state_name[ostate], ieee80211_state_name[nstate]));
	ic->ic_state = nstate;			/* state transition */
	ni = ic->ic_bss;			/* NB: no reference held */
	switch (nstate) {
	case IEEE80211_S_INIT:
		switch (ostate) {
		case IEEE80211_S_INIT:
			break;
		case IEEE80211_S_RUN:
			switch (ic->ic_opmode) {
			case IEEE80211_M_STA:
				IEEE80211_SEND_MGMT(ic, ni,
				    IEEE80211_FC0_SUBTYPE_DISASSOC,
				    IEEE80211_REASON_ASSOC_LEAVE);
				ieee80211_sta_leave(ic, ni);
				break;
			case IEEE80211_M_HOSTAP:
				IEEE80211_NODE_LOCK(ic);
				TAILQ_FOREACH(ni, &ic->ic_node, ni_list) {
					if (ni->ni_associd == 0)
						continue;
					IEEE80211_SEND_MGMT(ic, ni,
					    IEEE80211_FC0_SUBTYPE_DISASSOC,
					    IEEE80211_REASON_ASSOC_LEAVE);
				}
				IEEE80211_NODE_UNLOCK(ic);
				break;
			default:
				break;
			}
			goto reset;
		case IEEE80211_S_ASSOC:
			switch (ic->ic_opmode) {
			case IEEE80211_M_STA:
				IEEE80211_SEND_MGMT(ic, ni,
				    IEEE80211_FC0_SUBTYPE_DEAUTH,
				    IEEE80211_REASON_AUTH_LEAVE);
				break;
			case IEEE80211_M_HOSTAP:
				IEEE80211_NODE_LOCK(ic);
				TAILQ_FOREACH(ni, &ic->ic_node, ni_list) {
					IEEE80211_SEND_MGMT(ic, ni,
					    IEEE80211_FC0_SUBTYPE_DEAUTH,
					    IEEE80211_REASON_AUTH_LEAVE);
				}
				IEEE80211_NODE_UNLOCK(ic);
				break;
			default:
				break;
			}
			goto reset;
		case IEEE80211_S_AUTH:
		case IEEE80211_S_SCAN:
		reset:
			ic->ic_mgt_timer = 0;
			ic->ic_inact_timer = 0;
			ieee80211_free_allnodes(ic);
			break;
		}
		if (ic->ic_auth->ia_detach != NULL)
			ic->ic_auth->ia_detach(ic);
		break;
	case IEEE80211_S_SCAN:
		ic->ic_flags &= ~IEEE80211_F_SIBSS;
		/* initialize bss for probe request */
		IEEE80211_ADDR_COPY(ni->ni_macaddr, dev->broadcast);
		IEEE80211_ADDR_COPY(ni->ni_bssid, dev->broadcast);
		ni->ni_rates = ic->ic_sup_rates[
			ieee80211_chan2mode(ic, ni->ni_chan)];
		ni->ni_associd = 0;
		ni->ni_rstamp = 0;
		switch (ostate) {
		case IEEE80211_S_INIT:
			if ((ic->ic_opmode == IEEE80211_M_HOSTAP ||
			     ic->ic_opmode == IEEE80211_M_IBSS ||
			     ic->ic_opmode == IEEE80211_M_AHDEMO) &&
			    ic->ic_des_chan != IEEE80211_CHAN_ANYC) {
				/*
				 * AP operation and we already have a channel;
				 * bypass the scan and startup immediately.
				 */
				ieee80211_create_ibss(ic, ic->ic_des_chan);
			} else {
				ieee80211_begin_scan(ic, arg);
			}
			break;
		case IEEE80211_S_SCAN:
			/*
			 * Scan next. If doing an active scan and the
			 * channel is not marked passive-only then send
			 * a probe request.  Otherwise just listen for
			 * beacons on the channel.
			 */
			if ((ic->ic_flags & IEEE80211_F_ASCAN) &&
			    (ni->ni_chan->ic_flags & IEEE80211_CHAN_PASSIVE) == 0) {
				IEEE80211_SEND_MGMT(ic, ni,
				    IEEE80211_FC0_SUBTYPE_PROBE_REQ, 0);
			}
			break;
		case IEEE80211_S_RUN:
			/* beacon miss */
			IEEE80211_DPRINTF(ic, IEEE80211_MSG_STATE,
				("no recent beacons from %s; rescanning\n",
				ether_sprintf(ic->ic_bss->ni_bssid)));
			ieee80211_sta_leave(ic, ni);
			/* XXX this clears the scan set */
			ieee80211_free_allnodes(ic);
			/* FALLTHRU */
		case IEEE80211_S_AUTH:
		case IEEE80211_S_ASSOC:

			/* XXX clear the bss node if set*/
			if (ic->ic_bss != NULL)
				node_cleanup(ic, ic->ic_bss);	/* for station mode */
			/* XXX doesn't work, ni_macaddr rewritten above */
			/* timeout restart scan */
			ni = ieee80211_find_node(ic, ic->ic_bss->ni_macaddr);
			if (ni != NULL) {
				ni->ni_fails++;
				ieee80211_unref_node(&ni);
			}
			ieee80211_begin_scan(ic, arg);
			break;
		}
		break;
	case IEEE80211_S_AUTH:
		switch (ostate) {
		case IEEE80211_S_INIT:
		case IEEE80211_S_SCAN:
			IEEE80211_SEND_MGMT(ic, ni,
			    IEEE80211_FC0_SUBTYPE_AUTH, 1);
			break;
		case IEEE80211_S_AUTH:
		case IEEE80211_S_ASSOC:
			switch (arg) {
			case IEEE80211_FC0_SUBTYPE_AUTH:
				/* ??? */
				IEEE80211_SEND_MGMT(ic, ni,
				    IEEE80211_FC0_SUBTYPE_AUTH, 2);
				break;
			case IEEE80211_FC0_SUBTYPE_DEAUTH:
				/* ignore and retry scan on timeout */
				break;
			}
			break;
		case IEEE80211_S_RUN:
			switch (arg) {
			case IEEE80211_FC0_SUBTYPE_AUTH:
				IEEE80211_SEND_MGMT(ic, ni,
				    IEEE80211_FC0_SUBTYPE_AUTH, 2);
				ic->ic_state = ostate;	/* stay RUN */
				break;
			case IEEE80211_FC0_SUBTYPE_DEAUTH:
				/* try to reauth */
				IEEE80211_SEND_MGMT(ic, ni,
				    IEEE80211_FC0_SUBTYPE_AUTH, 1);
				ieee80211_sta_leave(ic, ni);
				break;
			}
			break;
		}
		break;
	case IEEE80211_S_ASSOC:
		switch (ostate) {
		case IEEE80211_S_INIT:
		case IEEE80211_S_SCAN:
		case IEEE80211_S_ASSOC:
			IEEE80211_DPRINTF(ic, IEEE80211_MSG_ANY,
				("%s: invalid transition\n", __func__));
			break;
		case IEEE80211_S_AUTH:
			IEEE80211_SEND_MGMT(ic, ni,
			    IEEE80211_FC0_SUBTYPE_ASSOC_REQ, 0);
			break;
		case IEEE80211_S_RUN:
			IEEE80211_SEND_MGMT(ic, ni,
			    IEEE80211_FC0_SUBTYPE_ASSOC_REQ, 1);
			ieee80211_sta_leave(ic, ni);
			break;
		}
		break;
	case IEEE80211_S_RUN:
		if (ic->ic_flags & IEEE80211_F_WPA) {
			/* XXX validate prerequisites */
		}
		switch (ostate) {
		case IEEE80211_S_INIT:
			if (ic->ic_opmode == IEEE80211_M_MONITOR)
				break;
			/* fall thru... */
		case IEEE80211_S_AUTH:
			IEEE80211_DPRINTF(ic, IEEE80211_MSG_ANY,
				("%s: invalid transition\n", __func__));
			break;
		case IEEE80211_S_RUN:
			/* 
			 * This maybe a valid transition for non-standard
			 * (i.e. Atheros SuperG) methods which may 
			 * dynamically change channel settings/flags.
			 */
			break;
		case IEEE80211_S_SCAN:		/* adhoc/hostap mode */
		case IEEE80211_S_ASSOC:		/* infra mode */
			KASSERT(ni->ni_txrate < ni->ni_rates.rs_nrates,
				("%s: bogus xmit rate %u setup\n", __func__,
					ni->ni_txrate));
// Dino Chang 2005/03/31
#ifdef IEEE80211_DEBUG
// Dino
			if (ieee80211_msg_debug(ic)) {
				if_printf(ic->ic_dev, " ");
				if (ic->ic_opmode == IEEE80211_M_STA)
					printf("associated ");
				else
					printf("synchronized ");
				printf("with %s ssid ",
				    ether_sprintf(ni->ni_bssid));
				ieee80211_print_essid(ic->ic_bss->ni_essid,
				    ni->ni_esslen);
				printf(" channel %d start %uMb\n",
					ieee80211_chan2ieee(ic, ni->ni_chan),
					IEEE80211_RATE2MBS(ni->ni_rates.rs_rates[ni->ni_txrate]));
			}
// Dino Chang 2005/03/31
#endif
// Dino
			ic->ic_mgt_timer = 0;
			if (ic->ic_opmode == IEEE80211_M_STA)
				ieee80211_notify_node_join(ic, ni, 
					arg == IEEE80211_FC0_SUBTYPE_ASSOC_RESP);
			break;
		}
		/*
		 * Start/stop the authenticator when operating as an
		 * AP.  We delay until here to allow configuration to
		 * happen out of order.
		 */
		if (ic->ic_opmode == IEEE80211_M_HOSTAP && /* XXX IBSS/AHDEMO */
		    ic->ic_auth->ia_attach != NULL) {
			/* XXX check failure */
			ic->ic_auth->ia_attach(ic);
		} else if (ic->ic_auth->ia_detach != NULL) {
			ic->ic_auth->ia_detach(ic);
		}
		/*
		 * When 802.1x is not in use mark the port authorized
		 * at this point so traffic can flow.
		 */
		if (ni->ni_authmode != IEEE80211_AUTH_8021X)
			ieee80211_node_authorize(ic, ni);
		break;
	}
	return 0;
}
