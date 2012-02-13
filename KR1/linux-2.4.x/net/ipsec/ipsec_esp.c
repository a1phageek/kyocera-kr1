/*
 * processing code for ESP
 * Copyright (C) 2003 Michael Richardson <mcr@sandelman.ottawa.on.ca>
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

char ipsec_esp_c_version[] = "RCSID $Id: ipsec_esp.c,v 1.2 2005/02/22 01:58:10 rupert Exp $";
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

#ifdef CONFIG_IPSEC_ESP
#include "openswan/ipsec_esp.h"
#endif /* CONFIG_IPSEC_ESP */

#include "openswan/ipsec_proto.h"
#include "openswan/ipsec_alg.h"

#ifdef CONFIG_IPSEC_DEBUG
int debug_esp = 0;
#endif /* CONFIG_IPSEC_DEBUG */


#ifdef CONFIG_IPSEC_ESP
enum ipsec_rcv_value
ipsec_rcv_esp_checks(struct ipsec_rcv_state *irs,
		     struct sk_buff *skb)
{
	__u8 proto;
	int len;	/* packet length */

	len = skb->len;
	proto = irs->ipp->protocol;

	/* XXX this will need to be 8 for IPv6 */
	if ((proto == IPPROTO_ESP) && ((len - irs->iphlen) % 4)) {
		printk("klips_error:ipsec_rcv: "
		       "got packet with content length = %d from %s -- should be on 4 octet boundary, packet dropped\n",
		       len - irs->iphlen,
		       irs->ipsaddr_txt);
		if(irs->stats) {
			irs->stats->rx_errors++;
		}
		return IPSEC_RCV_BADLEN;
	}

	if(skb->len < (irs->hard_header_len + sizeof(struct iphdr) + sizeof(struct esphdr))) {
		KLIPS_PRINT(debug_rcv & DB_RX_INAU,
			    "klips_debug:ipsec_rcv: "
			    "runt esp packet of skb->len=%d received from %s, dropped.\n",
			    skb->len,
			    irs->ipsaddr_txt);
		if(irs->stats) {
			irs->stats->rx_errors++;
		}
		return IPSEC_RCV_BADLEN;
	}

	irs->protostuff.espstuff.espp = (struct esphdr *)(skb->data + irs->iphlen);
	irs->said.spi = irs->protostuff.espstuff.espp->esp_spi;

	return IPSEC_RCV_OK;
}

enum ipsec_rcv_value
ipsec_rcv_esp_decrypt_setup(struct ipsec_rcv_state *irs,
			    struct sk_buff *skb,
			    __u32          *replay,
			    unsigned char **authenticator)
{
	struct esphdr *espp = irs->protostuff.espstuff.espp;

	KLIPS_PRINT(debug_rcv,
		    "klips_debug:ipsec_rcv: "
		    "packet from %s received with seq=%d (iv)=0x%08x%08x iplen=%d esplen=%d sa=%s\n",
		    irs->ipsaddr_txt,
		    (__u32)ntohl(espp->esp_rpl),
		    (__u32)ntohl(*((__u32 *)(espp->esp_iv)    )),
		    (__u32)ntohl(*((__u32 *)(espp->esp_iv) + 1)),
		    irs->len,
		    irs->ilen,
		    irs->sa_len ? irs->sa : " (error)");

	*replay = ntohl(espp->esp_rpl);
	*authenticator = &(skb->data[irs->len - irs->authlen]);

	return IPSEC_RCV_OK;
}

enum ipsec_rcv_value
ipsec_rcv_esp_authcalc(struct ipsec_rcv_state *irs,
		       struct sk_buff *skb)
{
	struct auth_alg *aa;
	struct esphdr *espp = irs->protostuff.espstuff.espp;
	union {
		MD5_CTX		md5;
		SHA1_CTX	sha1;
	} tctx;

#ifdef CONFIG_IPSEC_ALG
	if (irs->ipsp->ips_alg_auth) {
		KLIPS_PRINT(debug_rcv,
				"klips_debug:ipsec_rcv: "
				"ipsec_alg hashing proto=%d... ",
				irs->said.proto);
		if(irs->said.proto == IPPROTO_ESP) {
			ipsec_alg_sa_esp_hash(irs->ipsp,
					(caddr_t)espp, irs->ilen,
					irs->hash, AHHMAC_HASHLEN);
			return IPSEC_RCV_OK;
		}
		return IPSEC_RCV_BADPROTO;
	}
#endif
	aa = irs->authfuncs;

	/* copy the initialized keying material */
	memcpy(&tctx, irs->ictx, irs->ictx_len);

	(*aa->update)((void *)&tctx, (caddr_t)espp, irs->ilen);

	(*aa->final)(irs->hash, (void *)&tctx);

	memcpy(&tctx, irs->octx, irs->octx_len);

	(*aa->update)((void *)&tctx, irs->hash, aa->hashlen);
	(*aa->final)(irs->hash, (void *)&tctx);

	return IPSEC_RCV_OK;
}


enum ipsec_rcv_value
ipsec_rcv_esp_decrypt(struct ipsec_rcv_state *irs)
{
	struct ipsec_sa *ipsp = irs->ipsp;
	struct esphdr *espp = irs->protostuff.espstuff.espp;
	int esphlen = 0;
	__u8 *idat;	/* pointer to content to be decrypted/authenticated */
	__u32 iv[2];
	int pad = 0, padlen;
	int badpad = 0;
	int i;
	struct sk_buff *skb;
#ifdef CONFIG_IPSEC_ALG
	struct ipsec_alg_enc *ixt_e=NULL;
#endif /* CONFIG_IPSEC_ALG */

	skb=irs->skb;

	idat = skb->data + irs->iphlen;

#ifdef CONFIG_IPSEC_ALG
	if ((ixt_e=ipsp->ips_alg_enc)) {
		esphlen = ESP_HEADER_LEN + ixt_e->ixt_ivlen/8;
		KLIPS_PRINT(debug_rcv,
				"klips_debug:ipsec_rcv: "
				"encalg=%d esphlen=%d\n",
				ipsp->ips_encalg, esphlen);
	} else
#endif /* CONFIG_IPSEC_ALG */
	switch(ipsp->ips_encalg) {
	case ESP_3DES:
		iv[0] = *((__u32 *)(espp->esp_iv)    );
		iv[1] = *((__u32 *)(espp->esp_iv) + 1);
		esphlen = sizeof(struct esphdr);
		break;
	default:
		ipsp->ips_errs.ips_alg_errs += 1;
		if(irs->stats) {
			irs->stats->rx_errors++;
		}
		return IPSEC_RCV_ESP_BADALG;
	}

	idat += esphlen;
	irs->ilen -= esphlen;

#ifdef CONFIG_IPSEC_ALG
	if (ixt_e)
	{
		if (ipsec_alg_esp_encrypt(ipsp, 
					idat, irs->ilen, espp->esp_iv, 
					IPSEC_ALG_DECRYPT) <= 0)
		{
			printk("klips_error:ipsec_rcv: "
					"got packet with esplen = %d "
					"from %s -- should be on "
					"ENC(%d) octet boundary, "
					"packet dropped\n",
					irs->ilen,
					irs->ipsaddr_txt,
					ipsp->ips_encalg);
			if(irs->stats) {
				irs->stats->rx_errors++;
			}
			return IPSEC_RCV_BAD_DECRYPT;
		}
	} else
#endif /* CONFIG_IPSEC_ALG */
	switch(ipsp->ips_encalg) {
	case ESP_3DES:
		if ((irs->ilen) % 8) {
			ipsp->ips_errs.ips_encsize_errs += 1;
			printk("klips_error:ipsec_rcv: "
			       "got packet with esplen = %d from %s -- should be on 8 octet boundary, packet dropped\n",
			       irs->ilen,
			       irs->ipsaddr_txt);
			if(irs->stats) {
				irs->stats->rx_errors++;
			}
			return IPSEC_RCV_3DES_BADBLOCKING;
		}
		des_ede3_cbc_encrypt((des_cblock *)idat,
				     (des_cblock *)idat,
				     irs->ilen,
				     ((struct des_eks *)(ipsp->ips_key_e))[0].ks,
				     ((struct des_eks *)(ipsp->ips_key_e))[1].ks,
				     ((struct des_eks *)(ipsp->ips_key_e))[2].ks,
				     (des_cblock *)iv, 0);
		break;
	}

	ipsec_rcv_dmp("postdecrypt", skb->data, skb->len);

	irs->next_header = idat[irs->ilen - 1];
	padlen = idat[irs->ilen - 2];
	pad = padlen + 2 + irs->authlen;

	KLIPS_PRINT(debug_rcv & DB_RX_IPAD,
		    "klips_debug:ipsec_rcv: "
		    "padlen=%d, contents: 0x<offset>: 0x<value> 0x<value> ...\n",
		    padlen);

	for (i = 1; i <= padlen; i++) {
		if((i % 16) == 1) {
			KLIPS_PRINT(debug_rcv & DB_RX_IPAD,
				    "klips_debug:           %02x:",
				    i - 1);
		}
		KLIPS_PRINTMORE(debug_rcv & DB_RX_IPAD,
				" %02x",
				idat[irs->ilen - 2 - padlen + i - 1]);
		if(i != idat[irs->ilen - 2 - padlen + i - 1]) {
			badpad = 1;
		}
		if((i % 16) == 0) {
			KLIPS_PRINTMORE(debug_rcv & DB_RX_IPAD,
					"\n");
		}
	}
	if((i % 16) != 1) {
		KLIPS_PRINTMORE(debug_rcv & DB_RX_IPAD,
						"\n");
	}
	if(badpad) {
		KLIPS_PRINT(debug_rcv & DB_RX_IPAD,
			    "klips_debug:ipsec_rcv: "
			    "warning, decrypted packet from %s has bad padding\n",
			    irs->ipsaddr_txt);
		KLIPS_PRINT(debug_rcv & DB_RX_IPAD,
			    "klips_debug:ipsec_rcv: "
			    "...may be bad decryption -- not dropped\n");
		ipsp->ips_errs.ips_encpad_errs += 1;
	}

	KLIPS_PRINT(debug_rcv & DB_RX_IPAD,
		    "klips_debug:ipsec_rcv: "
		    "packet decrypted from %s: next_header = %d, padding = %d\n",
		    irs->ipsaddr_txt,
		    irs->next_header,
		    pad - 2 - irs->authlen);

	irs->ipp->tot_len = htons(ntohs(irs->ipp->tot_len) - (esphlen + pad));

	/*
	 * move the IP header forward by the size of the ESP header, which
	 * will remove the the ESP header from the packet.
	 */
	memmove((void *)(skb->data + esphlen),
		(void *)(skb->data), irs->iphlen);

	ipsec_rcv_dmp("esp postmove", skb->data, skb->len);

	/* skb_pull below, will move up by esphlen */

	/* XXX not clear how this can happen, as the message indicates */
	if(skb->len < esphlen) {
		printk(KERN_WARNING
		       "klips_error:ipsec_rcv: "
		       "tried to skb_pull esphlen=%d, %d available.  This should never happen, please report.\n",
		       esphlen, (int)(skb->len));
		return IPSEC_RCV_ESP_DECAPFAIL;
	}
	skb_pull(skb, esphlen);

	irs->ipp = (struct iphdr *)skb->data;

	ipsec_rcv_dmp("esp postpull", skb->data, skb->len);

	/* now, trip off the padding from the end */
	KLIPS_PRINT(debug_rcv & DB_RX_PKTRX,
		    "klips_debug:ipsec_rcv: "
		    "trimming to %d.\n",
		    irs->len - esphlen - pad);
	if(pad + esphlen <= irs->len) {
		skb_trim(skb, irs->len - esphlen - pad);
	} else {
		KLIPS_PRINT(debug_rcv & DB_RX_PKTRX,
			    "klips_debug:ipsec_rcv: "
			    "bogus packet, size is zero or negative, dropping.\n");
		return IPSEC_RCV_DECAPFAIL;
	}

	return IPSEC_RCV_OK;
}

enum ipsec_xmit_value
ipsec_xmit_esp_setup(struct ipsec_xmit_state *ixs)
{
  __u32 iv[2];
  struct esphdr *espp;
  int ilen = 0;
  int padlen = 0, i;
  unsigned char *dat;
  unsigned char *idat, *pad;
  __u8 hash[AH_AMAX];
  union {
#ifdef CONFIG_IPSEC_AUTH_HMAC_MD5
    MD5_CTX md5;
#endif /* CONFIG_IPSEC_AUTH_HMAC_MD5 */
#ifdef CONFIG_IPSEC_AUTH_HMAC_SHA1
    SHA1_CTX sha1;
#endif /* CONFIG_IPSEC_AUTH_HMAC_SHA1 */
  } tctx;

  dat = (unsigned char *)ixs->iph;

  espp = (struct esphdr *)(dat + ixs->iphlen);
  espp->esp_spi = ixs->ipsp->ips_said.spi;
  espp->esp_rpl = htonl(++(ixs->ipsp->ips_replaywin_lastseq));
  
  switch(ixs->ipsp->ips_encalg) {
#if defined(CONFIG_IPSEC_ENC_3DES)
#ifdef CONFIG_IPSEC_ENC_3DES
  case ESP_3DES:
#endif /* CONFIG_IPSEC_ENC_3DES */
    iv[0] = *((__u32*)&(espp->esp_iv)    ) =
      ((__u32*)(ixs->ipsp->ips_iv))[0];
    iv[1] = *((__u32*)&(espp->esp_iv) + 1) =
      ((__u32*)(ixs->ipsp->ips_iv))[1];
    break;
#endif /* defined(CONFIG_IPSEC_ENC_3DES) */
  default:
    ixs->stats->tx_errors++;
    return IPSEC_XMIT_ESP_BADALG;
  }
		
  idat = dat + ixs->iphlen + sizeof(struct esphdr);
  ilen = ixs->skb->len - (ixs->iphlen + sizeof(struct esphdr) + ixs->authlen);
  
  /* Self-describing padding */
  pad = &dat[ixs->skb->len - ixs->tailroom];
  padlen = ixs->tailroom - 2 - ixs->authlen;
  for (i = 0; i < padlen; i++) {
    pad[i] = i + 1; 
  }
  dat[ixs->skb->len - ixs->authlen - 2] = padlen;
  
  dat[ixs->skb->len - ixs->authlen - 1] = ixs->iph->protocol;
  ixs->iph->protocol = IPPROTO_ESP;
  
  switch(ixs->ipsp->ips_encalg) {
#ifdef CONFIG_IPSEC_ENC_3DES
  case ESP_3DES:
    des_ede3_cbc_encrypt((des_cblock *)idat,
			 (des_cblock *)idat,
			 ilen,
			 ((struct des_eks *)(ixs->ipsp->ips_key_e))[0].ks,
			 ((struct des_eks *)(ixs->ipsp->ips_key_e))[1].ks,
			 ((struct des_eks *)(ixs->ipsp->ips_key_e))[2].ks,
			 (des_cblock *)iv, 1);
    break;
#endif /* CONFIG_IPSEC_ENC_3DES */
  default:
    ixs->stats->tx_errors++;
    return IPSEC_XMIT_ESP_BADALG;
  }
  
  switch(ixs->ipsp->ips_encalg) {
#if defined(CONFIG_IPSEC_ENC_3DES)
#ifdef CONFIG_IPSEC_ENC_3DES
  case ESP_3DES:
#endif /* CONFIG_IPSEC_ENC_3DES */
    /* XXX update IV with the last 8 octets of the encryption */
#if KLIPS_IMPAIRMENT_ESPIV_CBC_ATTACK
    ((__u32*)(ixs->ipsp->ips_iv))[0] =
      ((__u32 *)(idat))[(ilen >> 2) - 2];
    ((__u32*)(ixs->ipsp->ips_iv))[1] =
      ((__u32 *)(idat))[(ilen >> 2) - 1];
#else /* KLIPS_IMPAIRMENT_ESPIV_CBC_ATTACK */
    prng_bytes(&ipsec_prng, (char *)ixs->ipsp->ips_iv, EMT_ESPDES_IV_SZ); 
#endif /* KLIPS_IMPAIRMENT_ESPIV_CBC_ATTACK */
    break;
#endif /* defined(CONFIG_IPSEC_ENC_3DES) */
  default:
    ixs->stats->tx_errors++;
    return IPSEC_XMIT_ESP_BADALG;
  }
  
  switch(ixs->ipsp->ips_authalg) {
#ifdef CONFIG_IPSEC_AUTH_HMAC_MD5
  case AH_MD5:
    ipsec_xmit_dmp("espp", (char*)espp, ixs->skb->len - ixs->iphlen - ixs->authlen);
    tctx.md5 = ((struct md5_ctx*)(ixs->ipsp->ips_key_a))->ictx;
    ipsec_xmit_dmp("ictx", (char*)&tctx.md5, sizeof(tctx.md5));
    MD5Update(&tctx.md5, (caddr_t)espp, ixs->skb->len - ixs->iphlen - ixs->authlen);
    ipsec_xmit_dmp("ictx+dat", (char*)&tctx.md5, sizeof(tctx.md5));
    MD5Final(hash, &tctx.md5);
    ipsec_xmit_dmp("ictx hash", (char*)&hash, sizeof(hash));
    tctx.md5 = ((struct md5_ctx*)(ixs->ipsp->ips_key_a))->octx;
    ipsec_xmit_dmp("octx", (char*)&tctx.md5, sizeof(tctx.md5));
    MD5Update(&tctx.md5, hash, AHMD596_ALEN);
    ipsec_xmit_dmp("octx+hash", (char*)&tctx.md5, sizeof(tctx.md5));
    MD5Final(hash, &tctx.md5);
    ipsec_xmit_dmp("octx hash", (char*)&hash, sizeof(hash));
    memcpy(&(dat[ixs->skb->len - ixs->authlen]), hash, ixs->authlen);
    
    /* paranoid */
    memset((caddr_t)&tctx.md5, 0, sizeof(tctx.md5));
    memset((caddr_t)hash, 0, sizeof(*hash));
    break;
#endif /* CONFIG_IPSEC_AUTH_HMAC_MD5 */
#ifdef CONFIG_IPSEC_AUTH_HMAC_SHA1
  case AH_SHA:
    tctx.sha1 = ((struct sha1_ctx*)(ixs->ipsp->ips_key_a))->ictx;
    SHA1Update(&tctx.sha1, (caddr_t)espp, ixs->skb->len - ixs->iphlen - ixs->authlen);
    SHA1Final(hash, &tctx.sha1);
    tctx.sha1 = ((struct sha1_ctx*)(ixs->ipsp->ips_key_a))->octx;
    SHA1Update(&tctx.sha1, hash, AHSHA196_ALEN);
    SHA1Final(hash, &tctx.sha1);
    memcpy(&(dat[ixs->skb->len - ixs->authlen]), hash, ixs->authlen);
    
    /* paranoid */
    memset((caddr_t)&tctx.sha1, 0, sizeof(tctx.sha1));
    memset((caddr_t)hash, 0, sizeof(*hash));
    break;
#endif /* CONFIG_IPSEC_AUTH_HMAC_SHA1 */
  case AH_NONE:
    break;
  default:
    ixs->stats->tx_errors++;
    return IPSEC_XMIT_AH_BADALG;
  }
#ifdef NET_21
  ixs->skb->h.raw = (unsigned char*)espp;
#endif /* NET_21 */

  return IPSEC_XMIT_OK;
}


struct xform_functions esp_xform_funcs[]={
	{	rcv_checks:         ipsec_rcv_esp_checks,
		rcv_setup_auth:     ipsec_rcv_esp_decrypt_setup,
		rcv_calc_auth:      ipsec_rcv_esp_authcalc,
		rcv_decrypt:        ipsec_rcv_esp_decrypt,

		xmit_setup:         ipsec_xmit_esp_setup,
		xmit_headroom:      sizeof(struct esphdr),
		xmit_needtailroom:  1,
	},
};

struct inet_protocol esp_protocol =
{
	ipsec_rcv,			/* ESP handler		*/
	NULL,				/* TUNNEL error control */
#ifdef NETDEV_25
	1,				/* no policy */
#else
	0,				/* next */
	IPPROTO_ESP,			/* protocol ID */
	0,				/* copy */
	NULL,				/* data */
	"ESP"				/* name */
#endif
};



#endif /* !CONFIG_IPSEC_ESP */


/*
 * $Log: ipsec_esp.c,v $
 * Revision 1.2  2005/02/22 01:58:10  rupert
 * +: merge openswan-2.2.0
 *
 * Revision 1.2  2004/04/06 02:49:25  mcr
 * 	pullup of algo code from alg-branch.
 *
 *
 *
 */
