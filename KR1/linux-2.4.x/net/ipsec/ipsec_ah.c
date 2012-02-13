/*
 * processing code for AH
 * Copyright (C) 2003-2004   Michael Richardson <mcr@xelerance.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

char ipsec_ah_c_version[] = "RCSID $Id: ipsec_ah.c,v 1.1 2005/02/22 01:59:11 rupert Exp $";
#include <linux/config.h>
#include <linux/version.h>

#define __NO_VERSION__
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */

#include "openswan/ipsec_param.h"

#ifdef MALLOC_SLAB
# include <linux/slab.h> /* kmalloc() */
#else /* MALLOC_SLAB */
# include <linux/malloc.h> /* kmalloc() */
#endif /* MALLOC_SLAB */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/interrupt.h> /* mark_bh */

#include <linux/netdevice.h>	/* struct device, and other headers */
#include <linux/etherdevice.h>	/* eth_type_trans */
#include <linux/ip.h>		/* struct iphdr */
#include <linux/skbuff.h>
#include <openswan.h>
#ifdef SPINLOCK
# ifdef SPINLOCK_23
#  include <linux/spinlock.h> /* *lock* */
# else /* SPINLOCK_23 */
#  include <asm/spinlock.h> /* *lock* */
# endif /* SPINLOCK_23 */
#endif /* SPINLOCK */
#ifdef NET_21
# include <asm/uaccess.h>
# include <linux/in6.h>
# define proto_priv cb
#endif /* NET21 */
#include <asm/checksum.h>
#include <net/ip.h>

#include "openswan/radij.h"
#include "openswan/ipsec_encap.h"
#include "openswan/ipsec_sa.h"

#include "openswan/ipsec_radij.h"
#include "openswan/ipsec_xform.h"
#include "openswan/ipsec_tunnel.h" 
#include "openswan/ipsec_rcv.h"
#include "openswan/ipsec_xmit.h"

#include "openswan/ipsec_auth.h"

#ifdef CONFIG_IPSEC_AH
#include "openswan/ipsec_ah.h"
#endif /* CONFIG_IPSEC_AH */

#include "openswan/ipsec_proto.h"

#ifdef CONFIG_IPSEC_DEBUG
int debug_ah = 0;
#endif /* CONFIG_IPSEC_DEBUG */

__u32 zeroes[AH_AMAX];

#ifdef CONFIG_IPSEC_AH
enum ipsec_rcv_value
ipsec_rcv_ah_checks(struct ipsec_rcv_state *irs,
		    struct sk_buff *skb)
{
	int ahminlen;

	ahminlen = irs->hard_header_len + sizeof(struct iphdr);

	/* take care not to deref this pointer until we check the minlen though */
	irs->protostuff.ahstuff.ahp = (struct ahhdr *) (skb->data + irs->iphlen);

	if((skb->len < ahminlen+sizeof(struct ahhdr)) ||
	   (skb->len < ahminlen+(irs->protostuff.ahstuff.ahp->ah_hl << 2))) {
		KLIPS_PRINT(debug_rcv & DB_RX_INAU,
			    "klips_debug:ipsec_rcv: "
			    "runt ah packet of skb->len=%d received from %s, dropped.\n",
			    skb->len,
			    irs->ipsaddr_txt);
		if(irs->stats) {
			irs->stats->rx_errors++;
		}
		return IPSEC_RCV_BADLEN;
	}

	irs->said.spi = irs->protostuff.ahstuff.ahp->ah_spi;

	/* XXX we only support the one 12-byte authenticator for now */
	if(irs->protostuff.ahstuff.ahp->ah_hl != ((AHHMAC_HASHLEN+AHHMAC_RPLLEN) >> 2)) {
		KLIPS_PRINT(debug_rcv & DB_RX_INAU,
			    "klips_debug:ipsec_rcv: "
			    "bad authenticator length %ld, expected %lu from %s.\n",
			    (long)(irs->protostuff.ahstuff.ahp->ah_hl << 2),
			    (unsigned long) sizeof(struct ahhdr),
			    irs->ipsaddr_txt);
		if(irs->stats) {
			irs->stats->rx_errors++;
		}
		return IPSEC_RCV_BADLEN;
	}

	return IPSEC_RCV_OK;
}


enum ipsec_rcv_value
ipsec_rcv_ah_setup_auth(struct ipsec_rcv_state *irs,
			struct sk_buff *skb,
			__u32          *replay,
			unsigned char **authenticator)
{
	struct ahhdr *ahp = irs->protostuff.ahstuff.ahp;

	*replay = ntohl(ahp->ah_rpl);
	*authenticator = ahp->ah_data;

	return IPSEC_RCV_OK;
}

enum ipsec_rcv_value
ipsec_rcv_ah_authcalc(struct ipsec_rcv_state *irs,
		      struct sk_buff *skb)
{
	struct auth_alg *aa;
	struct ahhdr *ahp = irs->protostuff.ahstuff.ahp;
	union {
		MD5_CTX		md5;
		SHA1_CTX	sha1;
	} tctx;
	struct iphdr ipo;
	int ahhlen;

	aa = irs->authfuncs;

	/* copy the initialized keying material */
	memcpy(&tctx, irs->ictx, irs->ictx_len);

	ipo = *irs->ipp;
	ipo.tos = 0;	/* mutable RFC 2402 3.3.3.1.1.1 */
	ipo.frag_off = 0;
	ipo.ttl = 0;
	ipo.check = 0;


	/* do the sanitized header */
	(*aa->update)((void*)&tctx, (caddr_t)&ipo, sizeof(struct iphdr));

	/* XXX we didn't do the options here! */

	/* now do the AH header itself */
	ahhlen = AH_BASIC_LEN + (ahp->ah_hl << 2);
	(*aa->update)((void*)&tctx, (caddr_t)ahp,  ahhlen - AHHMAC_HASHLEN);

	/* now, do some zeroes */
	(*aa->update)((void*)&tctx, (caddr_t)zeroes,  AHHMAC_HASHLEN);

	/* finally, do the packet contents themselves */
	(*aa->update)((void*)&tctx,
		      (caddr_t)skb->data + irs->iphlen + ahhlen,
		      skb->len - irs->iphlen - ahhlen);

	(*aa->final)(irs->hash, (void *)&tctx);

	memcpy(&tctx, irs->octx, irs->octx_len);

	(*aa->update)((void *)&tctx, irs->hash, aa->hashlen);
	(*aa->final)(irs->hash, (void *)&tctx);

	return IPSEC_RCV_OK;
}

enum ipsec_rcv_value
ipsec_rcv_ah_decap(struct ipsec_rcv_state *irs)
{
	struct ahhdr *ahp = irs->protostuff.ahstuff.ahp;
	struct sk_buff *skb;
	int ahhlen;

	skb=irs->skb;

	ahhlen = AH_BASIC_LEN + (ahp->ah_hl << 2);

	irs->ipp->tot_len = htons(ntohs(irs->ipp->tot_len) - ahhlen);
	irs->next_header  = ahp->ah_nh;

	/*
	 * move the IP header forward by the size of the AH header, which
	 * will remove the the AH header from the packet.
	 */
	memmove((void *)(skb->data + ahhlen),
		(void *)(skb->data), irs->iphlen);

	ipsec_rcv_dmp("ah postmove", skb->data, skb->len);

	/* skb_pull below, will move up by ahhlen */

	/* XXX not clear how this can happen, as the message indicates */
	if(skb->len < ahhlen) {
		printk(KERN_WARNING
		       "klips_error:ipsec_rcv: "
		       "tried to skb_pull ahhlen=%d, %d available.  This should never happen, please report.\n",
		       ahhlen,
		       (int)(skb->len));
		return IPSEC_RCV_DECAPFAIL;
	}
	skb_pull(skb, ahhlen);

	irs->ipp = (struct iphdr *)skb->data;

	ipsec_rcv_dmp("ah postpull", skb->data, skb->len);

	return IPSEC_RCV_OK;
}

enum ipsec_xmit_value
ipsec_xmit_ah_setup(struct ipsec_xmit_state *ixs)
{
  struct iphdr ipo;
  struct ahhdr *ahp;
  __u8 hash[AH_AMAX];
  union {
#ifdef CONFIG_IPSEC_AUTH_HMAC_MD5
    MD5_CTX md5;
#endif /* CONFIG_IPSEC_AUTH_HMAC_MD5 */
#ifdef CONFIG_IPSEC_AUTH_HMAC_SHA1
    SHA1_CTX sha1;
#endif /* CONFIG_IPSEC_AUTH_HMAC_SHA1 */
  } tctx;
  unsigned char *dat = (unsigned char *)ixs->iph;

  ahp = (struct ahhdr *)(dat + ixs->iphlen);
  ahp->ah_spi = ixs->ipsp->ips_said.spi;
  ahp->ah_rpl = htonl(++(ixs->ipsp->ips_replaywin_lastseq));
  ahp->ah_rv = 0;
  ahp->ah_nh = ixs->iph->protocol;
  ahp->ah_hl = (sizeof(struct ahhdr) >> 2) - sizeof(__u64)/sizeof(__u32);
  ixs->iph->protocol = IPPROTO_AH;
  ipsec_xmit_dmp("ahp", (char*)ahp, sizeof(*ahp));
  
  ipo = *ixs->iph;
  ipo.tos = 0;
  ipo.frag_off = 0;
  ipo.ttl = 0;
  ipo.check = 0;
  ipsec_xmit_dmp("ipo", (char*)&ipo, sizeof(ipo));
  
  switch(ixs->ipsp->ips_authalg) {
#ifdef CONFIG_IPSEC_AUTH_HMAC_MD5
  case AH_MD5:
    tctx.md5 = ((struct md5_ctx*)(ixs->ipsp->ips_key_a))->ictx;
    ipsec_xmit_dmp("ictx", (char*)&tctx.md5, sizeof(tctx.md5));
    MD5Update(&tctx.md5, (unsigned char *)&ipo, sizeof (struct iphdr));
    ipsec_xmit_dmp("ictx+ipo", (char*)&tctx.md5, sizeof(tctx.md5));
    MD5Update(&tctx.md5, (unsigned char *)ahp,
	      sizeof(struct ahhdr) - sizeof(ahp->ah_data));
    ipsec_xmit_dmp("ictx+ahp", (char*)&tctx.md5, sizeof(tctx.md5));
    MD5Update(&tctx.md5, (unsigned char *)zeroes, AHHMAC_HASHLEN);
    ipsec_xmit_dmp("ictx+zeroes", (char*)&tctx.md5, sizeof(tctx.md5));
    MD5Update(&tctx.md5,  dat + ixs->iphlen + sizeof(struct ahhdr),
	      ixs->skb->len - ixs->iphlen - sizeof(struct ahhdr));
    ipsec_xmit_dmp("ictx+dat", (char*)&tctx.md5, sizeof(tctx.md5));
    MD5Final(hash, &tctx.md5);
    ipsec_xmit_dmp("ictx hash", (char*)&hash, sizeof(hash));
    tctx.md5 = ((struct md5_ctx*)(ixs->ipsp->ips_key_a))->octx;
    ipsec_xmit_dmp("octx", (char*)&tctx.md5, sizeof(tctx.md5));
    MD5Update(&tctx.md5, hash, AHMD596_ALEN);
    ipsec_xmit_dmp("octx+hash", (char*)&tctx.md5, sizeof(tctx.md5));
    MD5Final(hash, &tctx.md5);
    ipsec_xmit_dmp("octx hash", (char*)&hash, sizeof(hash));
    
    memcpy(ahp->ah_data, hash, AHHMAC_HASHLEN);
    
    /* paranoid */
    memset((caddr_t)&tctx.md5, 0, sizeof(tctx.md5));
    memset((caddr_t)hash, 0, sizeof(*hash));
    break;
#endif /* CONFIG_IPSEC_AUTH_HMAC_MD5 */
#ifdef CONFIG_IPSEC_AUTH_HMAC_SHA1
  case AH_SHA:
    tctx.sha1 = ((struct sha1_ctx*)(ixs->ipsp->ips_key_a))->ictx;
    SHA1Update(&tctx.sha1, (unsigned char *)&ipo, sizeof (struct iphdr));
    SHA1Update(&tctx.sha1, (unsigned char *)ahp, sizeof(struct ahhdr) - sizeof(ahp->ah_data));
    SHA1Update(&tctx.sha1, (unsigned char *)zeroes, AHHMAC_HASHLEN);
    SHA1Update(&tctx.sha1, dat + ixs->iphlen + sizeof(struct ahhdr),
	       ixs->skb->len - ixs->iphlen - sizeof(struct ahhdr));
    SHA1Final(hash, &tctx.sha1);
    tctx.sha1 = ((struct sha1_ctx*)(ixs->ipsp->ips_key_a))->octx;
    SHA1Update(&tctx.sha1, hash, AHSHA196_ALEN);
    SHA1Final(hash, &tctx.sha1);
    
    memcpy(ahp->ah_data, hash, AHHMAC_HASHLEN);
    
    /* paranoid */
    memset((caddr_t)&tctx.sha1, 0, sizeof(tctx.sha1));
    memset((caddr_t)hash, 0, sizeof(*hash));
    break;
#endif /* CONFIG_IPSEC_AUTH_HMAC_SHA1 */
  default:
    ixs->stats->tx_errors++;
    return IPSEC_XMIT_AH_BADALG;
  }
#ifdef NET_21
  ixs->skb->h.raw = (unsigned char*)ahp;
#endif /* NET_21 */

  return IPSEC_XMIT_OK;
}

struct xform_functions ah_xform_funcs[]={
	{	rcv_checks:         ipsec_rcv_ah_checks,
		rcv_setup_auth:     ipsec_rcv_ah_setup_auth,
		rcv_calc_auth:      ipsec_rcv_ah_authcalc,
		rcv_decrypt:        ipsec_rcv_ah_decap,

		xmit_setup:         ipsec_xmit_ah_setup,
		xmit_headroom:      sizeof(struct ahhdr),
		xmit_needtailroom:  0,
	},
};

struct inet_protocol ah_protocol =
{
	ipsec_rcv,				/* AH handler */
	NULL,				/* TUNNEL error control */
#ifdef NETDEV_25
	1,				/* no policy */
#else
	0,				/* next */
	IPPROTO_AH,			/* protocol ID */
	0,				/* copy */
	NULL,				/* data */
	"AH"				/* name */
#endif
};

#endif /* CONFIG_IPSEC_AH */

/*
 * $Log: ipsec_ah.c,v $
 * Revision 1.1  2005/02/22 01:59:11  rupert
 * +: merge openswan-2.2.0
 *
 * Revision 1.2  2004/04/06 02:49:25  mcr
 * 	pullup of algo code from alg-branch.
 *
 *
 *
 */
