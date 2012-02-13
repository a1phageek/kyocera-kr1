/*
 * IPSEC Tunneling code. Heavily based on drivers/net/new_tunnel.c
 * Copyright (C) 1996, 1997  John Ioannidis.
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003  Richard Guy Briggs.
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

char ipsec_tunnel_c_version[] = "RCSID $Id: ipsec_tunnel.c,v 1.1 2004/08/02 02:09:59 rupert Exp $";

#define __NO_VERSION__
#include <linux/module.h>
#include <linux/config.h>	/* for CONFIG_IP_FORWARD */
#include <linux/version.h>
#include <linux/kernel.h> /* printk() */

#include "freeswan/ipsec_param.h"

#ifdef MALLOC_SLAB
# include <linux/slab.h> /* kmalloc() */
#else /* MALLOC_SLAB */
# include <linux/malloc.h> /* kmalloc() */
#endif /* MALLOC_SLAB */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/interrupt.h> /* mark_bh */
#include <net/tcp.h>
#include <net/udp.h>

#include <linux/netdevice.h>   /* struct device, struct net_device_stats, dev_queue_xmit() and other headers */
#include <linux/etherdevice.h> /* eth_type_trans */
#include <linux/ip.h>          /* struct iphdr */
#include <linux/skbuff.h>
#include <freeswan.h>
#ifdef NET_21
# include <asm/uaccess.h>
# include <linux/in6.h>
# define ip_chk_addr inet_addr_type
# define IS_MYADDR RTN_LOCAL
# include <net/dst.h>
# undef dev_kfree_skb
# define dev_kfree_skb(a,b) kfree_skb(a)
# define PHYSDEV_TYPE
#endif /* NET_21 */
#include <asm/checksum.h>
#include <net/icmp.h>		/* icmp_send() */
#include <net/ip.h>
#ifdef NETDEV_23
# include <linux/netfilter_ipv4.h>
#endif /* NETDEV_23 */

#include <linux/if_arp.h>

#include "freeswan/radij.h"
#include "freeswan/ipsec_life.h"
#include "freeswan/ipsec_xform.h"
#include "freeswan/ipsec_eroute.h"
#include "freeswan/ipsec_encap.h"
#include "freeswan/ipsec_radij.h"
#include "freeswan/ipsec_sa.h"
#include "freeswan/ipsec_tunnel.h"
#include "freeswan/ipsec_xmit.h"
#include "freeswan/ipsec_ipe4.h"
#include "freeswan/ipsec_esp.h"

#include <pfkeyv2.h>
#include <pfkey.h>

#include "freeswan/ipsec_proto.h"

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

static __u32 zeroes[64];

#ifdef CONFIG_KLIPS_DEBUG
int debug_tunnel = 0;
#endif /* CONFIG_KLIPS_DEBUG */

DEBUG_NO_STATIC int
ipsec_tunnel_open(struct net_device *dev)
{
	struct ipsecpriv *prv = dev->priv;
	
	/*
	 * Can't open until attached.
	 */

	KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
		    "klips_debug:ipsec_tunnel_open: "
		    "dev = %s, prv->dev = %s\n",
		    dev->name,
		    prv->dev?prv->dev->name:"NONE");

	if (prv->dev == NULL)
		return -ENODEV;
	
	MOD_INC_USE_COUNT;
	return 0;
}

DEBUG_NO_STATIC int
ipsec_tunnel_close(struct net_device *dev)
{
	MOD_DEC_USE_COUNT;
	return 0;
}

#ifdef NETDEV_23
static inline int ipsec_tunnel_xmit2(struct sk_buff *skb)
{
#ifdef NETDEV_25	/* 2.6 kernels */
	return dst_output(skb);
#else
	return ip_send(skb);
#endif
}
#endif /* NETDEV_23 */

enum ipsec_xmit_value
ipsec_tunnel_strip_hard_header(struct ipsec_xmit_state *ixs)
{
	/* while ixs->physdev->hard_header_len is unreliable and
	 * should not be trusted, it accurate and required for ATM
	 * work. Thanks to Willy Tarreau <willy@w.ods.org>.
	 */
 	if (ixs->physdev->type == ARPHRD_ATM) {
 		/* otherwise it panics in clip_start_xmit()/skb_push() */
 		ixs->hard_header_len = ixs->physdev->hard_header_len;
 		ixs->hard_header_stripped = 1;
 	}
 	else {
	  ixs->hard_header_len = (unsigned char *)(ixs->iph) - ixs->skb->data;
	}

	if(ixs->hard_header_len < 0) {
		KLIPS_PRINT(debug_tunnel & DB_TN_XMIT,
			    "klips_error:ipsec_xmit_strip_hard_header: "
			    "Negative hard_header_len (%d)?!\n", ixs->hard_header_len);
		ixs->stats->tx_dropped++;
		return IPSEC_XMIT_BADHHLEN;
	}

	if(ixs->hard_header_len == 0) { /* no hard header present */
		ixs->hard_header_stripped = 1;
	}

#ifdef CONFIG_KLIPS_DEBUG
	if (debug_tunnel & DB_TN_XMIT) {
		int i;
		char c;
		
		printk(KERN_INFO "klips_debug:ipsec_xmit_strip_hard_header: "
		       ">>> skb->len=%ld hard_header_len:%d",
		       (unsigned long int)ixs->skb->len, ixs->hard_header_len);
		c = ' ';
		for (i=0; i < ixs->hard_header_len; i++) {
			printk("%c%02x", c, ixs->skb->data[i]);
			c = ':';
		}
		printk(" \n");
	}
#endif /* CONFIG_KLIPS_DEBUG */

	KLIPS_IP_PRINT(debug_tunnel & DB_TN_XMIT, ixs->iph);

	KLIPS_PRINT(debug_tunnel & DB_TN_CROUT,
		    "klips_debug:ipsec_xmit_strip_hard_header: "
		    "Original head,tailroom: %d,%d\n",
		    skb_headroom(ixs->skb), skb_tailroom(ixs->skb));

	return IPSEC_XMIT_OK;
}

enum ipsec_xmit_value
ipsec_tunnel_SAlookup(struct ipsec_xmit_state *ixs)
{
	unsigned int bypass;

	bypass = FALSE;

	/*
	 * First things first -- look us up in the erouting tables.
	 */
	ixs->matcher.sen_len = sizeof (struct sockaddr_encap);
	ixs->matcher.sen_family = AF_ENCAP;
	ixs->matcher.sen_type = SENT_IP4;
	ixs->matcher.sen_ip_src.s_addr = ixs->iph->saddr;
	ixs->matcher.sen_ip_dst.s_addr = ixs->iph->daddr;
	
	/*
	 * The spinlock is to prevent any other process from accessing or deleting
	 * the eroute while we are using and updating it.
	 */
	spin_lock(&eroute_lock);
	
	ixs->eroute = ipsec_findroute(&ixs->matcher);

 	/* extract information for later */
 	if(ixs->iph->protocol == IPPROTO_UDP) {
		struct udphdr *t = NULL;

		KLIPS_PRINT(debug_tunnel & DB_TN_XMIT,
			    "klips_debug:udp port check: "
			    "fragoff: %d len: %d>%d \n",
			    ntohs(ixs->iph->frag_off) & IP_OFFSET,
			    (ixs->skb->len - ixs->hard_header_len),
			    (ixs->iph->ihl << 2) + sizeof(struct udphdr));
		
		if((ntohs(ixs->iph->frag_off) & IP_OFFSET) == 0 &&
		   ((ixs->skb->len - ixs->hard_header_len) >=
		    ((ixs->iph->ihl << 2) + sizeof(struct udphdr))))
		{
			t =((struct udphdr*)((caddr_t)ixs->iph+(ixs->iph->ihl<<2)));
			KLIPS_PRINT(debug_tunnel & DB_TN_XMIT,
				    "klips_debug:udp port in packet: "
				    "port %d -> %d\n",
				    ntohs(t->source), ntohs(t->dest));
		}

		ixs->sport=0; ixs->dport=0;

		if(ixs->skb->sk) {
#ifdef NET_26
			struct udp_sock *us;
			
			us = (struct udp_sock *)ixs->skb->sk;

			ixs->sport = ntohs(us->inet.sport);
			ixs->dport = ntohs(us->inet.dport);
#else
			ixs->sport = ntohs(ixs->skb->sk->sport);
			ixs->dport = ntohs(ixs->skb->sk->dport);
#endif

		} 

		if(t != NULL) {
			if(ixs->sport == 0) {
				ixs->sport = ntohs(t->source);
			}
			if(ixs->dport == 0) {
				ixs->dport = ntohs(t->dest);
			}
		}
	}
	
	/*
	 * practically identical to above, but let's be careful about
	 * tcp vs udp headers
	 */
	if(ixs->iph->protocol == IPPROTO_TCP) {
		struct tcphdr *t = NULL;

		if((ntohs(ixs->iph->frag_off) & IP_OFFSET) == 0 &&
		   ((ixs->skb->len - ixs->hard_header_len) >=
		    ((ixs->iph->ihl << 2) + sizeof(struct tcphdr)))) {
			t =((struct tcphdr*)((caddr_t)ixs->iph+(ixs->iph->ihl<<2)));
		}

		ixs->sport=0; ixs->dport=0;

		if(ixs->skb->sk) {
#ifdef NET_26
			struct tcp_tw_bucket *tw;
			
			tw = (struct tcp_tw_bucket *)ixs->skb->sk;

			ixs->sport = ntohs(tw->tw_sport);
			ixs->dport = ntohs(tw->tw_dport);
#else
			ixs->sport = ntohs(ixs->skb->sk->sport);
			ixs->dport = ntohs(ixs->skb->sk->dport);
#endif
		} 

		if(t != NULL) {
			if(ixs->sport == 0) {
				ixs->sport = ntohs(t->source);
			}
			if(ixs->dport == 0) {
				ixs->dport = ntohs(t->dest);
			}
		}
	}
	
	/* default to a %drop eroute */
	ixs->outgoing_said.proto = IPPROTO_INT;
	ixs->outgoing_said.spi = htonl(SPI_DROP);
	ixs->outgoing_said.dst.u.v4.sin_addr.s_addr = INADDR_ANY;

	KLIPS_PRINT(debug_tunnel & DB_TN_XMIT,
		    "klips_debug:ipsec_xmit_SAlookup: "
		    "checking for local udp/500 IKE packet or local DNS "
		    "saddr=%x, er=0p%p, daddr=%x, er_dst=%x, proto=%d sport=%d dport=%d\n",
		    ntohl((unsigned int)ixs->iph->saddr),
		    ixs->eroute,
		    ntohl((unsigned int)ixs->iph->daddr),
		    ixs->eroute ? ntohl((unsigned int)ixs->eroute->er_said.dst.u.v4.sin_addr.s_addr) : 0,
		    ixs->iph->protocol,
		    ixs->sport,
		    ixs->dport);

	/*
	 * cheat for now...are we udp/500? If so, let it through
	 * without interference since it is most likely an IKE packet.
	 */

	if (ip_chk_addr((unsigned long)ixs->iph->saddr) == IS_MYADDR
	    && (ixs->eroute==NULL
		|| ixs->iph->daddr == ixs->eroute->er_said.dst.u.v4.sin_addr.s_addr
		|| INADDR_ANY == ixs->eroute->er_said.dst.u.v4.sin_addr.s_addr)
	    && (ixs->iph->protocol == IPPROTO_UDP && ixs->sport == 500)) {
		/* Whatever the eroute, this is an IKE message 
		 * from us (i.e. not being forwarded).
		 * Furthermore, if there is a tunnel eroute,
		 * the destination is the peer for this eroute.
		 * So %pass the packet: modify the default %drop.
		 */

		ixs->outgoing_said.spi = htonl(SPI_PASS);
		if(!(ixs->skb->sk) && ((ntohs(ixs->iph->frag_off) & IP_MF) != 0)) {
			KLIPS_PRINT(debug_tunnel & DB_TN_XMIT,
				    "klips_debug:ipsec_xmit_SAlookup: "
				    "local UDP/500 (probably IKE) passthrough: base fragment, rest of fragments will probably get filtered.\n");
		}
		bypass = TRUE;
	}

	/*
	 *
	 * if we are udp/53 or tcp/53, also let it through a %trap or %hold,
	 * since it is DNS, but *also* follow the %trap.
	 * 
	 * we do not do this for tunnels, only %trap's and %hold's.
	 *
	 */

	if (ip_chk_addr((unsigned long)ixs->iph->saddr) == IS_MYADDR
	    && (ixs->eroute==NULL
		|| ixs->iph->daddr == ixs->eroute->er_said.dst.u.v4.sin_addr.s_addr
		|| INADDR_ANY == ixs->eroute->er_said.dst.u.v4.sin_addr.s_addr)
	    && ((ixs->iph->protocol == IPPROTO_UDP
		 || ixs->iph->protocol == IPPROTO_TCP)
		&& ixs->dport == 53)) {
		
		KLIPS_PRINT(debug_tunnel & DB_TN_XMIT,
			    "klips_debug:ipsec_xmit_SAlookup: "
			    "possible DNS packet\n");

		if(ixs->eroute)
		{
			if(ixs->eroute->er_said.spi == htonl(SPI_TRAP)
			   || ixs->eroute->er_said.spi == htonl(SPI_HOLD))
			{
				ixs->outgoing_said.spi = htonl(SPI_PASSTRAP);
				bypass = TRUE;
			}
		}
		else
		{
			ixs->outgoing_said.spi = htonl(SPI_PASSTRAP);
			bypass = TRUE;
		}
				
		KLIPS_PRINT(debug_tunnel & DB_TN_XMIT,
			    "klips_debug:ipsec_xmit_SAlookup: "
			    "bypass = %d\n", bypass);

		if(bypass
		   && !(ixs->skb->sk)
		   && ((ntohs(ixs->iph->frag_off) & IP_MF) != 0))
		{
			KLIPS_PRINT(debug_tunnel & DB_TN_XMIT,
				    "klips_debug:ipsec_xmit_SAlookup: "
				    "local port 53 (probably DNS) passthrough:"
				    "base fragment, rest of fragments will "
				    "probably get filtered.\n");
		}
	}

	if (bypass==FALSE && ixs->eroute) {
		ixs->eroute->er_count++;
		ixs->eroute->er_lasttime = jiffies/HZ;
		if(ixs->eroute->er_said.proto==IPPROTO_INT
		   && ixs->eroute->er_said.spi==htonl(SPI_HOLD))
		{
			KLIPS_PRINT(debug_tunnel & DB_TN_XMIT,
				    "klips_debug:ipsec_xmit_SAlookup: "
				    "shunt SA of HOLD: skb stored in HOLD.\n");
			if(ixs->eroute->er_last != NULL) {
				kfree_skb(ixs->eroute->er_last);
			}
			ixs->eroute->er_last = ixs->skb;
			ixs->skb = NULL;
			ixs->stats->tx_dropped++;
			spin_unlock(&eroute_lock);
			return IPSEC_XMIT_STOLEN;
		}
		ixs->outgoing_said = ixs->eroute->er_said;
		ixs->eroute_pid = ixs->eroute->er_pid;

		/* Copy of the ident for the TRAP/TRAPSUBNET eroutes */
		if(ixs->outgoing_said.proto==IPPROTO_INT
		   && (ixs->outgoing_said.spi==htonl(SPI_TRAP)
		       || (ixs->outgoing_said.spi==htonl(SPI_TRAPSUBNET)))) {
			int len;
			
			ixs->ips.ips_ident_s.type = ixs->eroute->er_ident_s.type;
			ixs->ips.ips_ident_s.id = ixs->eroute->er_ident_s.id;
			ixs->ips.ips_ident_s.len = ixs->eroute->er_ident_s.len;
			if (ixs->ips.ips_ident_s.len)
			{
				len = ixs->ips.ips_ident_s.len * IPSEC_PFKEYv2_ALIGN - sizeof(struct sadb_ident);
				KLIPS_PRINT(debug_tunnel & DB_TN_XMIT,
					    "klips_debug:ipsec_xmit_SAlookup: "
					    "allocating %d bytes for ident_s shunt SA of HOLD: skb stored in HOLD.\n",
					    len);
				if ((ixs->ips.ips_ident_s.data = kmalloc(len, GFP_ATOMIC)) == NULL) {
					printk(KERN_WARNING "klips_debug:ipsec_xmit_SAlookup: "
					       "Failed, tried to allocate %d bytes for source ident.\n", 
					       len);
					ixs->stats->tx_dropped++;
					spin_unlock(&eroute_lock);
					return IPSEC_XMIT_ERRMEMALLOC;
				}
				memcpy(ixs->ips.ips_ident_s.data, ixs->eroute->er_ident_s.data, len);
			}
			ixs->ips.ips_ident_d.type = ixs->eroute->er_ident_d.type;
			ixs->ips.ips_ident_d.id = ixs->eroute->er_ident_d.id;
			ixs->ips.ips_ident_d.len = ixs->eroute->er_ident_d.len;
			if (ixs->ips.ips_ident_d.len)
			{
				len = ixs->ips.ips_ident_d.len * IPSEC_PFKEYv2_ALIGN - sizeof(struct sadb_ident);
				KLIPS_PRINT(debug_tunnel & DB_TN_XMIT,
					    "klips_debug:ipsec_xmit_SAlookup: "
					    "allocating %d bytes for ident_d shunt SA of HOLD: skb stored in HOLD.\n",
					    len);
				if ((ixs->ips.ips_ident_d.data = kmalloc(len, GFP_ATOMIC)) == NULL) {
					printk(KERN_WARNING "klips_debug:ipsec_xmit_SAlookup: "
					       "Failed, tried to allocate %d bytes for dest ident.\n", 
					       len);
					ixs->stats->tx_dropped++;
					spin_unlock(&eroute_lock);
					return IPSEC_XMIT_ERRMEMALLOC;
				}
				memcpy(ixs->ips.ips_ident_d.data, ixs->eroute->er_ident_d.data, len);
			}
		}
	}

	spin_unlock(&eroute_lock);
	return IPSEC_XMIT_OK;
}

enum ipsec_xmit_value
ipsec_tunnel_restore_hard_header(struct ipsec_xmit_state*ixs)
{
	KLIPS_PRINT(debug_tunnel & DB_TN_CROUT,
		    "klips_debug:ipsec_xmit_restore_hard_header: "
		    "After recursive xforms -- head,tailroom: %d,%d\n",
		    skb_headroom(ixs->skb),
		    skb_tailroom(ixs->skb));

	if(ixs->saved_header) {
		if(skb_headroom(ixs->skb) < ixs->hard_header_len) {
			printk(KERN_WARNING
			       "klips_error:ipsec_xmit_restore_hard_header: "
			       "tried to skb_push hhlen=%d, %d available.  This should never happen, please report.\n",
			       ixs->hard_header_len,
			       skb_headroom(ixs->skb));
			ixs->stats->tx_errors++;
			return IPSEC_XMIT_PUSHPULLERR;

		}
		skb_push(ixs->skb, ixs->hard_header_len);
		{
			int i;
			for (i = 0; i < ixs->hard_header_len; i++) {
				ixs->skb->data[i] = ixs->saved_header[i];
			}
		}
	}
	KLIPS_PRINT(debug_tunnel & DB_TN_CROUT,
		    "klips_debug:ipsec_xmit_restore_hard_header: "
		    "With hard_header, final head,tailroom: %d,%d\n",
		    skb_headroom(ixs->skb),
		    skb_tailroom(ixs->skb));

	return IPSEC_XMIT_OK;
}

enum ipsec_xmit_value
ipsec_tunnel_send(struct ipsec_xmit_state*ixs)
{
#ifdef NETDEV_25
	struct flowi fl;
#endif
  
#ifdef NET_21	/* 2.2 and 2.4 kernels */
	/* new route/dst cache code from James Morris */
	ixs->skb->dev = ixs->physdev;
#ifdef NETDEV_25
 	fl.oif = ixs->physdev->iflink;
 	fl.nl_u.ip4_u.daddr = ixs->skb->nh.iph->daddr;
 	fl.nl_u.ip4_u.saddr = ixs->pass ? 0 : ixs->skb->nh.iph->saddr;
 	fl.nl_u.ip4_u.tos = RT_TOS(ixs->skb->nh.iph->tos);
 	fl.proto = ixs->skb->nh.iph->protocol;
 	if ((ixs->error = ip_route_output_key(&ixs->route, &fl))) {
#else
	/*skb_orphan(ixs->skb);*/
	if((ixs->error = ip_route_output(&ixs->route,
				    ixs->skb->nh.iph->daddr,
				    ixs->pass ? 0 : ixs->skb->nh.iph->saddr,
				    RT_TOS(ixs->skb->nh.iph->tos),
                                    /* mcr->rgb: should this be 0 instead? */
				    ixs->physdev->iflink))) {
#endif
		ixs->stats->tx_errors++;
		KLIPS_PRINT(debug_tunnel & DB_TN_XMIT,
			    "klips_debug:ipsec_xmit_send: "
			    "ip_route_output failed with error code %d, rt->u.dst.dev=%s, dropped\n",
			    ixs->error,
			    ixs->route->u.dst.dev->name);
		return IPSEC_XMIT_ROUTEERR;
	}
	if(ixs->dev == ixs->route->u.dst.dev) {
		ip_rt_put(ixs->route);
		/* This is recursion, drop it. */
		ixs->stats->tx_errors++;
		KLIPS_PRINT(debug_tunnel & DB_TN_XMIT,
			    "klips_debug:ipsec_xmit_send: "
			    "suspect recursion, dev=rt->u.dst.dev=%s, dropped\n",
			    ixs->dev->name);
		return IPSEC_XMIT_RECURSDETECT;
	}
	dst_release(ixs->skb->dst);
	ixs->skb->dst = &ixs->route->u.dst;
	ixs->stats->tx_bytes += ixs->skb->len;
	if(ixs->skb->len < ixs->skb->nh.raw - ixs->skb->data) {
		ixs->stats->tx_errors++;
		printk(KERN_WARNING
		       "klips_error:ipsec_xmit_send: "
		       "tried to __skb_pull nh-data=%ld, %d available.  This should never happen, please report.\n",
		       (unsigned long)(ixs->skb->nh.raw - ixs->skb->data),
		       ixs->skb->len);
		return IPSEC_XMIT_PUSHPULLERR;
	}
	__skb_pull(ixs->skb, ixs->skb->nh.raw - ixs->skb->data);
#ifdef SKB_RESET_NFCT
	if(!ixs->pass) {
	  nf_conntrack_put(ixs->skb->nfct);
	  ixs->skb->nfct = NULL;
	}
#ifdef CONFIG_NETFILTER_DEBUG
	ixs->skb->nf_debug = 0;
#endif /* CONFIG_NETFILTER_DEBUG */
#endif /* SKB_RESET_NFCT */
	KLIPS_PRINT(debug_tunnel & DB_TN_XMIT,
		    "klips_debug:ipsec_xmit_send: "
		    "...done, calling ip_send() on device:%s\n",
		    ixs->skb->dev ? ixs->skb->dev->name : "NULL");
	KLIPS_IP_PRINT(debug_tunnel & DB_TN_XMIT, ixs->skb->nh.iph);
#ifdef NETDEV_23	/* 2.4 kernels */
	{
		int err;

		err = NF_HOOK(PF_INET, NF_IP_LOCAL_OUT, ixs->skb, NULL, ixs->route->u.dst.dev,
			      ipsec_tunnel_xmit2);
		if(err != NET_XMIT_SUCCESS && err != NET_XMIT_CN) {
			if(net_ratelimit())
				printk(KERN_ERR
				       "klips_error:ipsec_xmit_send: "
				       "ip_send() failed, err=%d\n", 
				       -err);
			ixs->stats->tx_errors++;
			ixs->stats->tx_aborted_errors++;
			ixs->skb = NULL;
			return IPSEC_XMIT_IPSENDFAILURE;
		}
	}
#else /* NETDEV_23 */	/* 2.2 kernels */
	ip_send(ixs->skb);
#endif /* NETDEV_23 */
#else /* NET_21 */	/* 2.0 kernels */
	ixs->skb->arp = 1;
	/* ISDN/ASYNC PPP from Matjaz Godec. */
	/*	skb->protocol = htons(ETH_P_IP); */
	KLIPS_PRINT(debug_tunnel & DB_TN_XMIT,
		    "klips_debug:ipsec_xmit_send: "
		    "...done, calling dev_queue_xmit() or ip_fragment().\n");
	IP_SEND(ixs->skb, ixs->physdev);
#endif /* NET_21 */
	ixs->stats->tx_packets++;

	ixs->skb = NULL;
	
	return IPSEC_XMIT_OK;
}

void
ipsec_tunnel_cleanup(struct ipsec_xmit_state*ixs)
{
#if defined(HAS_NETIF_QUEUE) || defined (HAVE_NETIF_QUEUE)
	netif_wake_queue(ixs->dev);
#else /* defined(HAS_NETIF_QUEUE) || defined (HAVE_NETIF_QUEUE) */
	ixs->dev->tbusy = 0;
#endif /* defined(HAS_NETIF_QUEUE) || defined (HAVE_NETIF_QUEUE) */
	if(ixs->saved_header) {
		kfree(ixs->saved_header);
	}
	if(ixs->skb) {
		dev_kfree_skb(ixs->skb, FREE_WRITE);
	}
	if(ixs->oskb) {
		dev_kfree_skb(ixs->oskb, FREE_WRITE);
	}
	if (ixs->ips.ips_ident_s.data) {
		kfree(ixs->ips.ips_ident_s.data);
	}
	if (ixs->ips.ips_ident_d.data) {
		kfree(ixs->ips.ips_ident_d.data);
	}
}

/*
 *	This function assumes it is being called from dev_queue_xmit()
 *	and that skb is filled properly by that function.
 */
int
ipsec_tunnel_start_xmit(struct sk_buff *skb
			, struct net_device *dev)
{
	struct ipsec_xmit_state ixs_mem;
	struct ipsec_xmit_state *ixs = &ixs_mem;
	enum ipsec_xmit_value stat;

	memset((caddr_t)ixs, 0, sizeof(*ixs));
	ixs->oskb = NULL;
	ixs->saved_header = NULL;	/* saved copy of the hard header */
	ixs->route = NULL;
	memset((caddr_t)&(ixs->ips), 0, sizeof(ixs->ips));
	ixs->dev = dev;
	ixs->skb = skb;

	stat = ipsec_xmit_sanity_check_dev(ixs);
	if(stat != IPSEC_XMIT_OK) {
		goto cleanup;
	}

	stat = ipsec_xmit_sanity_check_skb(ixs);
	if(stat != IPSEC_XMIT_OK) {
		goto cleanup;
	}

	stat = ipsec_tunnel_strip_hard_header(ixs);
	if(stat != IPSEC_XMIT_OK) {
		goto cleanup;
	}

	stat = ipsec_tunnel_SAlookup(ixs);
	if(stat != IPSEC_XMIT_OK) {
		KLIPS_PRINT(debug_tunnel & DB_TN_XMIT,
			    "klips_debug:ipsec_tunnel_start_xmit: SAlookup failed: %d\n",
			    stat);
		goto cleanup;
	}
	
	ixs->innersrc = ixs->iph->saddr;
	/* start encapsulation loop here XXX */
	do {
 		stat = ipsec_xmit_encap_bundle(ixs);
	 	if(stat != IPSEC_XMIT_OK) {
			if(stat == IPSEC_XMIT_PASS) {
				goto bypass;
			}
			
			KLIPS_PRINT(debug_tunnel & DB_TN_XMIT,
				    "klips_debug:ipsec_tunnel_start_xmit: encap_bundle failed: %d\n",
				    stat);
 			goto cleanup;
	 	}

		ixs->matcher.sen_ip_src.s_addr = ixs->iph->saddr;
		ixs->matcher.sen_ip_dst.s_addr = ixs->iph->daddr;
		spin_lock(&eroute_lock);
		ixs->eroute = ipsec_findroute(&ixs->matcher);
		if(ixs->eroute) {
			ixs->outgoing_said = ixs->eroute->er_said;
			ixs->eroute_pid = ixs->eroute->er_pid;
			ixs->eroute->er_count++;
			ixs->eroute->er_lasttime = jiffies/HZ;
		}
		spin_unlock(&eroute_lock);

		KLIPS_PRINT((debug_tunnel & DB_TN_XMIT) &&
			    /* ((ixs->orgdst != ixs->newdst) || (ixs->orgsrc != ixs->newsrc)) */
			    (ixs->orgedst != ixs->outgoing_said.dst.u.v4.sin_addr.s_addr) &&
			    ixs->outgoing_said.dst.u.v4.sin_addr.s_addr &&
			    ixs->eroute,
			    "klips_debug:ipsec_tunnel_start_xmit: "
			    "We are recursing here.\n");

	} while(/*((ixs->orgdst != ixs->newdst) || (ixs->orgsrc != ixs->newsrc))*/
		(ixs->orgedst != ixs->outgoing_said.dst.u.v4.sin_addr.s_addr) &&
		ixs->outgoing_said.dst.u.v4.sin_addr.s_addr &&
		ixs->eroute);
	
	stat = ipsec_tunnel_restore_hard_header(ixs);
	if(stat != IPSEC_XMIT_OK) {
		goto cleanup;
	}

 bypass:
	stat = ipsec_tunnel_send(ixs);

 cleanup:
	ipsec_tunnel_cleanup(ixs);

	return 0;
}

DEBUG_NO_STATIC struct net_device_stats *
ipsec_tunnel_get_stats(struct net_device *dev)
{
	return &(((struct ipsecpriv *)(dev->priv))->mystats);
}

/*
 * Revectored calls.
 * For each of these calls, a field exists in our private structure.
 */

DEBUG_NO_STATIC int
ipsec_tunnel_hard_header(struct sk_buff *skb, struct net_device *dev,
	unsigned short type, void *daddr, void *saddr, unsigned len)
{
	struct ipsecpriv *prv = dev->priv;
	struct net_device *tmp;
	int ret;
	struct net_device_stats *stats;	/* This device's statistics */
	
	if(skb == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_hard_header: "
			    "no skb...\n");
		return -ENODATA;
	}

	if(dev == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_hard_header: "
			    "no device...\n");
		return -ENODEV;
	}

	KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
		    "klips_debug:ipsec_tunnel_hard_header: "
		    "skb->dev=%s dev=%s.\n",
		    skb->dev ? skb->dev->name : "NULL",
		    dev->name);
	
	if(prv == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_hard_header: "
			    "no private space associated with dev=%s\n",
			    dev->name ? dev->name : "NULL");
		return -ENODEV;
	}

	stats = (struct net_device_stats *) &(prv->mystats);

	if(prv->dev == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_hard_header: "
			    "no physical device associated with dev=%s\n",
			    dev->name ? dev->name : "NULL");
		stats->tx_dropped++;
		return -ENODEV;
	}

	/* check if we have to send a IPv6 packet. It might be a Router
	   Solicitation, where the building of the packet happens in
	   reverse order:
	   1. ll hdr,
	   2. IPv6 hdr,
	   3. ICMPv6 hdr
	   -> skb->nh.raw is still uninitialized when this function is
	   called!!  If this is no IPv6 packet, we can print debugging
	   messages, otherwise we skip all debugging messages and just
	   build the ll header */
	if(type != ETH_P_IPV6) {
		/* execute this only, if we don't have to build the
		   header for a IPv6 packet */
		if(!prv->hard_header) {
			KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
				    "klips_debug:ipsec_tunnel_hard_header: "
				    "physical device has been detached, packet dropped 0p%p->0p%p len=%d type=%d dev=%s->NULL ",
				    saddr,
				    daddr,
				    len,
				    type,
				    dev->name);
#ifdef NET_21
			KLIPS_PRINTMORE(debug_tunnel & DB_TN_REVEC,
					"ip=%08x->%08x\n",
					(__u32)ntohl(skb->nh.iph->saddr),
					(__u32)ntohl(skb->nh.iph->daddr) );
#else /* NET_21 */
			KLIPS_PRINTMORE(debug_tunnel & DB_TN_REVEC,
					"ip=%08x->%08x\n",
					(__u32)ntohl(skb->ip_hdr->saddr),
					(__u32)ntohl(skb->ip_hdr->daddr) );
#endif /* NET_21 */
			stats->tx_dropped++;
			return -ENODEV;
		}
		
#define da ((struct net_device *)(prv->dev))->dev_addr
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_hard_header: "
			    "Revectored 0p%p->0p%p len=%d type=%d dev=%s->%s dev_addr=%02x:%02x:%02x:%02x:%02x:%02x ",
			    saddr,
			    daddr,
			    len,
			    type,
			    dev->name,
			    prv->dev->name,
			    da[0], da[1], da[2], da[3], da[4], da[5]);
#ifdef NET_21
		KLIPS_PRINTMORE(debug_tunnel & DB_TN_REVEC,
			    "ip=%08x->%08x\n",
			    (__u32)ntohl(skb->nh.iph->saddr),
			    (__u32)ntohl(skb->nh.iph->daddr) );
#else /* NET_21 */
		KLIPS_PRINTMORE(debug_tunnel & DB_TN_REVEC,
			    "ip=%08x->%08x\n",
			    (__u32)ntohl(skb->ip_hdr->saddr),
			    (__u32)ntohl(skb->ip_hdr->daddr) );
#endif /* NET_21 */
	} else {
		KLIPS_PRINT(debug_tunnel,
			    "klips_debug:ipsec_tunnel_hard_header: "
			    "is IPv6 packet, skip debugging messages, only revector and build linklocal header.\n");
	}                                                                       
	tmp = skb->dev;
	skb->dev = prv->dev;
	ret = prv->hard_header(skb, prv->dev, type, (void *)daddr, (void *)saddr, len);
	skb->dev = tmp;
	return ret;
}

DEBUG_NO_STATIC int
#ifdef NET_21
ipsec_tunnel_rebuild_header(struct sk_buff *skb)
#else /* NET_21 */
ipsec_tunnel_rebuild_header(void *buff, struct net_device *dev,
			unsigned long raddr, struct sk_buff *skb)
#endif /* NET_21 */
{
	struct ipsecpriv *prv = skb->dev->priv;
	struct net_device *tmp;
	int ret;
	struct net_device_stats *stats;	/* This device's statistics */
	
	if(skb->dev == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_rebuild_header: "
			    "no device...");
		return -ENODEV;
	}

	if(prv == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_rebuild_header: "
			    "no private space associated with dev=%s",
			    skb->dev->name ? skb->dev->name : "NULL");
		return -ENODEV;
	}

	stats = (struct net_device_stats *) &(prv->mystats);

	if(prv->dev == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_rebuild_header: "
			    "no physical device associated with dev=%s",
			    skb->dev->name ? skb->dev->name : "NULL");
		stats->tx_dropped++;
		return -ENODEV;
	}

	if(!prv->rebuild_header) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_rebuild_header: "
			    "physical device has been detached, packet dropped skb->dev=%s->NULL ",
			    skb->dev->name);
#ifdef NET_21
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "ip=%08x->%08x\n",
			    (__u32)ntohl(skb->nh.iph->saddr),
			    (__u32)ntohl(skb->nh.iph->daddr) );
#else /* NET_21 */
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "ip=%08x->%08x\n",
			    (__u32)ntohl(skb->ip_hdr->saddr),
			    (__u32)ntohl(skb->ip_hdr->daddr) );
#endif /* NET_21 */
		stats->tx_dropped++;
		return -ENODEV;
	}

	KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
		    "klips_debug:ipsec_tunnel: "
		    "Revectored rebuild_header dev=%s->%s ",
		    skb->dev->name, prv->dev->name);
#ifdef NET_21
	KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
		    "ip=%08x->%08x\n",
		    (__u32)ntohl(skb->nh.iph->saddr),
		    (__u32)ntohl(skb->nh.iph->daddr) );
#else /* NET_21 */
	KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
		    "ip=%08x->%08x\n",
		    (__u32)ntohl(skb->ip_hdr->saddr),
		    (__u32)ntohl(skb->ip_hdr->daddr) );
#endif /* NET_21 */
	tmp = skb->dev;
	skb->dev = prv->dev;
	
#ifdef NET_21
	ret = prv->rebuild_header(skb);
#else /* NET_21 */
	ret = prv->rebuild_header(buff, prv->dev, raddr, skb);
#endif /* NET_21 */
	skb->dev = tmp;
	return ret;
}

DEBUG_NO_STATIC int
ipsec_tunnel_set_mac_address(struct net_device *dev, void *addr)
{
	struct ipsecpriv *prv = dev->priv;
	
	struct net_device_stats *stats;	/* This device's statistics */
	
	if(dev == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_set_mac_address: "
			    "no device...");
		return -ENODEV;
	}

	if(prv == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_set_mac_address: "
			    "no private space associated with dev=%s",
			    dev->name ? dev->name : "NULL");
		return -ENODEV;
	}

	stats = (struct net_device_stats *) &(prv->mystats);

	if(prv->dev == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_set_mac_address: "
			    "no physical device associated with dev=%s",
			    dev->name ? dev->name : "NULL");
		stats->tx_dropped++;
		return -ENODEV;
	}

	if(!prv->set_mac_address) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_set_mac_address: "
			    "physical device has been detached, cannot set - skb->dev=%s->NULL\n",
			    dev->name);
		return -ENODEV;
	}

	KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
		    "klips_debug:ipsec_tunnel_set_mac_address: "
		    "Revectored dev=%s->%s addr=0p%p\n",
		    dev->name, prv->dev->name, addr);
	return prv->set_mac_address(prv->dev, addr);

}

#ifndef NET_21
DEBUG_NO_STATIC void
ipsec_tunnel_cache_bind(struct hh_cache **hhp, struct net_device *dev,
				 unsigned short htype, __u32 daddr)
{
	struct ipsecpriv *prv = dev->priv;
	
	struct net_device_stats *stats;	/* This device's statistics */
	
	if(dev == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_cache_bind: "
			    "no device...");
		return;
	}

	if(prv == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_cache_bind: "
			    "no private space associated with dev=%s",
			    dev->name ? dev->name : "NULL");
		return;
	}

	stats = (struct net_device_stats *) &(prv->mystats);

	if(prv->dev == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_cache_bind: "
			    "no physical device associated with dev=%s",
			    dev->name ? dev->name : "NULL");
		stats->tx_dropped++;
		return;
	}

	if(!prv->header_cache_bind) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_cache_bind: "
			    "physical device has been detached, cannot set - skb->dev=%s->NULL\n",
			    dev->name);
		stats->tx_dropped++;
		return;
	}

	KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
		    "klips_debug:ipsec_tunnel_cache_bind: "
		    "Revectored \n");
	prv->header_cache_bind(hhp, prv->dev, htype, daddr);
	return;
}
#endif /* !NET_21 */


DEBUG_NO_STATIC void
ipsec_tunnel_cache_update(struct hh_cache *hh, struct net_device *dev, unsigned char *  haddr)
{
	struct ipsecpriv *prv = dev->priv;
	
	struct net_device_stats *stats;	/* This device's statistics */
	
	if(dev == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_cache_update: "
			    "no device...");
		return;
	}

	if(prv == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_cache_update: "
			    "no private space associated with dev=%s",
			    dev->name ? dev->name : "NULL");
		return;
	}

	stats = (struct net_device_stats *) &(prv->mystats);

	if(prv->dev == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_cache_update: "
			    "no physical device associated with dev=%s",
			    dev->name ? dev->name : "NULL");
		stats->tx_dropped++;
		return;
	}

	if(!prv->header_cache_update) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_cache_update: "
			    "physical device has been detached, cannot set - skb->dev=%s->NULL\n",
			    dev->name);
		return;
	}

	KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
		    "klips_debug:ipsec_tunnel: "
		    "Revectored cache_update\n");
	prv->header_cache_update(hh, prv->dev, haddr);
	return;
}

#ifdef NET_21
DEBUG_NO_STATIC int
ipsec_tunnel_neigh_setup(struct neighbour *n)
{
	KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
		    "klips_debug:ipsec_tunnel_neigh_setup:\n");

        if (n->nud_state == NUD_NONE) {
                n->ops = &arp_broken_ops;
                n->output = n->ops->output;
        }
        return 0;
}

DEBUG_NO_STATIC int
ipsec_tunnel_neigh_setup_dev(struct net_device *dev, struct neigh_parms *p)
{
	KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
		    "klips_debug:ipsec_tunnel_neigh_setup_dev: "
		    "setting up %s\n",
		    dev ? dev->name : "NULL");

        if (p->tbl->family == AF_INET) {
                p->neigh_setup = ipsec_tunnel_neigh_setup;
                p->ucast_probes = 0;
                p->mcast_probes = 0;
        }
        return 0;
}
#endif /* NET_21 */

/*
 * We call the attach routine to attach another device.
 */

DEBUG_NO_STATIC int
ipsec_tunnel_attach(struct net_device *dev, struct net_device *physdev)
{
        int i;
	struct ipsecpriv *prv = dev->priv;

	if(dev == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_attach: "
			    "no device...");
		return -ENODEV;
	}

	if(prv == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_attach: "
			    "no private space associated with dev=%s",
			    dev->name ? dev->name : "NULL");
		return -ENODATA;
	}

	prv->dev = physdev;
	prv->hard_start_xmit = physdev->hard_start_xmit;
	prv->get_stats = physdev->get_stats;

	if (physdev->hard_header) {
		prv->hard_header = physdev->hard_header;
		dev->hard_header = ipsec_tunnel_hard_header;
	} else
		dev->hard_header = NULL;
	
	if (physdev->rebuild_header) {
		prv->rebuild_header = physdev->rebuild_header;
		dev->rebuild_header = ipsec_tunnel_rebuild_header;
	} else
		dev->rebuild_header = NULL;
	
	if (physdev->set_mac_address) {
		prv->set_mac_address = physdev->set_mac_address;
		dev->set_mac_address = ipsec_tunnel_set_mac_address;
	} else
		dev->set_mac_address = NULL;
	
#ifndef NET_21
	if (physdev->header_cache_bind) {
		prv->header_cache_bind = physdev->header_cache_bind;
		dev->header_cache_bind = ipsec_tunnel_cache_bind;
	} else
		dev->header_cache_bind = NULL;
#endif /* !NET_21 */

	if (physdev->header_cache_update) {
		prv->header_cache_update = physdev->header_cache_update;
		dev->header_cache_update = ipsec_tunnel_cache_update;
	} else
		dev->header_cache_update = NULL;

	dev->hard_header_len = physdev->hard_header_len;

#ifdef NET_21
/*	prv->neigh_setup        = physdev->neigh_setup; */
	dev->neigh_setup        = ipsec_tunnel_neigh_setup_dev;
#endif /* NET_21 */
	dev->mtu = 16260; /* 0xfff0; */ /* dev->mtu; */
	prv->mtu = physdev->mtu;

#ifdef PHYSDEV_TYPE
	dev->type = physdev->type; /* ARPHRD_TUNNEL; */
#endif /*  PHYSDEV_TYPE */

	dev->addr_len = physdev->addr_len;
	for (i=0; i<dev->addr_len; i++) {
		dev->dev_addr[i] = physdev->dev_addr[i];
	}
#ifdef CONFIG_KLIPS_DEBUG
	if(debug_tunnel & DB_TN_INIT) {
		printk(KERN_INFO "klips_debug:ipsec_tunnel_attach: "
		       "physical device %s being attached has HW address: %2x",
		       physdev->name, physdev->dev_addr[0]);
		for (i=1; i < physdev->addr_len; i++) {
			printk(":%02x", physdev->dev_addr[i]);
		}
		printk("\n");
	}
#endif /* CONFIG_KLIPS_DEBUG */

	return 0;
}

/*
 * We call the detach routine to detach the ipsec tunnel from another device.
 */

DEBUG_NO_STATIC int
ipsec_tunnel_detach(struct net_device *dev)
{
        int i;
	struct ipsecpriv *prv = dev->priv;

	if(dev == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_detach: "
			    "no device...");
		return -ENODEV;
	}

	if(prv == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_REVEC,
			    "klips_debug:ipsec_tunnel_detach: "
			    "no private space associated with dev=%s",
			    dev->name ? dev->name : "NULL");
		return -ENODATA;
	}

	KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
		    "klips_debug:ipsec_tunnel_detach: "
		    "physical device %s being detached from virtual device %s\n",
		    prv->dev ? prv->dev->name : "NULL",
		    dev->name);

	prv->dev = NULL;
	prv->hard_start_xmit = NULL;
	prv->get_stats = NULL;

	prv->hard_header = NULL;
#ifdef DETACH_AND_DOWN
	dev->hard_header = NULL;
#endif /* DETACH_AND_DOWN */
	
	prv->rebuild_header = NULL;
#ifdef DETACH_AND_DOWN
	dev->rebuild_header = NULL;
#endif /* DETACH_AND_DOWN */
	
	prv->set_mac_address = NULL;
#ifdef DETACH_AND_DOWN
	dev->set_mac_address = NULL;
#endif /* DETACH_AND_DOWN */
	
#ifndef NET_21
	prv->header_cache_bind = NULL;
#ifdef DETACH_AND_DOWN
	dev->header_cache_bind = NULL;
#endif /* DETACH_AND_DOWN */
#endif /* !NET_21 */

	prv->header_cache_update = NULL;
#ifdef DETACH_AND_DOWN
	dev->header_cache_update = NULL;
#endif /* DETACH_AND_DOWN */

#ifdef NET_21
/*	prv->neigh_setup        = NULL; */
#ifdef DETACH_AND_DOWN
	dev->neigh_setup        = NULL;
#endif /* DETACH_AND_DOWN */
#endif /* NET_21 */
	dev->hard_header_len = 0;
#ifdef DETACH_AND_DOWN
	dev->mtu = 0;
#endif /* DETACH_AND_DOWN */
	prv->mtu = 0;
	for (i=0; i<MAX_ADDR_LEN; i++) {
		dev->dev_addr[i] = 0;
	}
	dev->addr_len = 0;
#ifdef PHYSDEV_TYPE
	dev->type = ARPHRD_VOID; /* ARPHRD_TUNNEL; */
#endif /*  PHYSDEV_TYPE */
	
	return 0;
}

/*
 * We call the clear routine to detach all ipsec tunnels from other devices.
 */
DEBUG_NO_STATIC int
ipsec_tunnel_clear(void)
{
	int i;
	struct net_device *ipsecdev = NULL, *prvdev;
	struct ipsecpriv *prv;
	char name[9];
	int ret;

	KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
		    "klips_debug:ipsec_tunnel_clear: .\n");

	for(i = 0; i < IPSEC_NUM_IF; i++) {
		sprintf(name, IPSEC_DEV_FORMAT, i);
		if((ipsecdev = ipsec_dev_get(name)) != NULL) {
			if((prv = (struct ipsecpriv *)(ipsecdev->priv))) {
				prvdev = (struct net_device *)(prv->dev);
				if(prvdev) {
					KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
						    "klips_debug:ipsec_tunnel_clear: "
						    "physical device for device %s is %s\n",
						    name, prvdev->name);
					if((ret = ipsec_tunnel_detach(ipsecdev))) {
						KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
							    "klips_debug:ipsec_tunnel_clear: "
							    "error %d detatching device %s from device %s.\n",
							    ret, name, prvdev->name);
						return ret;
					}
				}
			}
		}
	}
	return 0;
}

DEBUG_NO_STATIC int
ipsec_tunnel_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	struct ipsectunnelconf *cf = (struct ipsectunnelconf *)&ifr->ifr_data;
	struct ipsecpriv *prv = dev->priv;
	struct net_device *them; /* physical device */
#ifdef CONFIG_IP_ALIAS
	char *colon;
	char realphysname[IFNAMSIZ];
#endif /* CONFIG_IP_ALIAS */
	
	if(dev == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
			    "klips_debug:ipsec_tunnel_ioctl: "
			    "device not supplied.\n");
		return -ENODEV;
	}

	KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
		    "klips_debug:ipsec_tunnel_ioctl: "
		    "tncfg service call #%d for dev=%s\n",
		    cmd,
		    dev->name ? dev->name : "NULL");
	switch (cmd) {
	/* attach a virtual ipsec? device to a physical device */
	case IPSEC_SET_DEV:
		KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
			    "klips_debug:ipsec_tunnel_ioctl: "
			    "calling ipsec_tunnel_attatch...\n");
#ifdef CONFIG_IP_ALIAS
		/* If this is an IP alias interface, get its real physical name */
		strncpy(realphysname, cf->cf_name, IFNAMSIZ);
		realphysname[IFNAMSIZ-1] = 0;
		colon = strchr(realphysname, ':');
		if (colon) *colon = 0;
		them = ipsec_dev_get(realphysname);
#else /* CONFIG_IP_ALIAS */
		them = ipsec_dev_get(cf->cf_name);
#endif /* CONFIG_IP_ALIAS */

		if (them == NULL) {
			KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
				    "klips_debug:ipsec_tunnel_ioctl: "
				    "physical device %s requested is null\n",
				    cf->cf_name);
			return -ENXIO;
		}
		
#if 0
		if (them->flags & IFF_UP) {
			KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
				    "klips_debug:ipsec_tunnel_ioctl: "
				    "physical device %s requested is not up.\n",
				    cf->cf_name);
			return -ENXIO;
		}
#endif
		
		if (prv && prv->dev) {
			KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
				    "klips_debug:ipsec_tunnel_ioctl: "
				    "virtual device is already connected to %s.\n",
				    prv->dev->name ? prv->dev->name : "NULL");
			return -EBUSY;
		}
		return ipsec_tunnel_attach(dev, them);

	case IPSEC_DEL_DEV:
		KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
			    "klips_debug:ipsec_tunnel_ioctl: "
			    "calling ipsec_tunnel_detatch.\n");
		if (! prv->dev) {
			KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
				    "klips_debug:ipsec_tunnel_ioctl: "
				    "physical device not connected.\n");
			return -ENODEV;
		}
		return ipsec_tunnel_detach(dev);
	       
	case IPSEC_CLR_DEV:
		KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
			    "klips_debug:ipsec_tunnel_ioctl: "
			    "calling ipsec_tunnel_clear.\n");
		return ipsec_tunnel_clear();

	default:
		KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
			    "klips_debug:ipsec_tunnel_ioctl: "
			    "unknown command %d.\n",
			    cmd);
		return -EOPNOTSUPP;
	}
}

int
ipsec_device_event(struct notifier_block *unused, unsigned long event, void *ptr)
{
	struct net_device *dev = ptr;
	struct net_device *ipsec_dev;
	struct ipsecpriv *priv;
	int i;

	if (dev == NULL) {
		KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
			    "klips_debug:ipsec_device_event: "
			    "dev=NULL for event type %ld.\n",
			    event);
		return(NOTIFY_DONE);
	}

	/* check for loopback devices */
	if (dev && (dev->flags & IFF_LOOPBACK)) {
		return(NOTIFY_DONE);
	}

	switch (event) {
	case NETDEV_DOWN:
		/* look very carefully at the scope of these compiler
		   directives before changing anything... -- RGB */
#ifdef NET_21
	case NETDEV_UNREGISTER:
		switch (event) {
		case NETDEV_DOWN:
#endif /* NET_21 */
			KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
				    "klips_debug:ipsec_device_event: "
				    "NETDEV_DOWN dev=%s flags=%x\n",
				    dev->name,
				    dev->flags);
			if(strncmp(dev->name, "ipsec", strlen("ipsec")) == 0) {
				printk(KERN_CRIT "IPSEC EVENT: KLIPS device %s shut down.\n",
				       dev->name);
			}
#ifdef NET_21
			break;
		case NETDEV_UNREGISTER:
			KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
				    "klips_debug:ipsec_device_event: "
				    "NETDEV_UNREGISTER dev=%s flags=%x\n",
				    dev->name,
				    dev->flags);
			break;
		}
#endif /* NET_21 */
		
		/* find the attached physical device and detach it. */
		for(i = 0; i < IPSEC_NUM_IF; i++) {
			ipsec_dev = ipsecdevices[i];

			if(ipsec_dev) {
				priv = (struct ipsecpriv *)(ipsec_dev->priv);
				if(priv) {
					;
					if(((struct net_device *)(priv->dev)) == dev) {
						/* dev_close(ipsec_dev); */
						/* return */ ipsec_tunnel_detach(ipsec_dev);
						KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
							    "klips_debug:ipsec_device_event: "
							    "device '%s' has been detached.\n",
							    ipsec_dev->name);
						break;
					}
				} else {
					KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
						    "klips_debug:ipsec_device_event: "
						    "device '%s' has no private data space!\n",
						    ipsec_dev->name);
				}
			}
		}
		break;
	case NETDEV_UP:
		KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
			    "klips_debug:ipsec_device_event: "
			    "NETDEV_UP dev=%s\n",
			    dev->name);
		break;
#ifdef NET_21
	case NETDEV_REBOOT:
		KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
			    "klips_debug:ipsec_device_event: "
			    "NETDEV_REBOOT dev=%s\n",
			    dev->name);
		break;
	case NETDEV_CHANGE:
		KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
			    "klips_debug:ipsec_device_event: "
			    "NETDEV_CHANGE dev=%s flags=%x\n",
			    dev->name,
			    dev->flags);
		break;
	case NETDEV_REGISTER:
		KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
			    "klips_debug:ipsec_device_event: "
			    "NETDEV_REGISTER dev=%s\n",
			    dev->name);
		break;
	case NETDEV_CHANGEMTU:
		KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
			    "klips_debug:ipsec_device_event: "
			    "NETDEV_CHANGEMTU dev=%s to mtu=%d\n",
			    dev->name,
			    dev->mtu);
		break;
	case NETDEV_CHANGEADDR:
		KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
			    "klips_debug:ipsec_device_event: "
			    "NETDEV_CHANGEADDR dev=%s\n",
			    dev->name);
		break;
	case NETDEV_GOING_DOWN:
		KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
			    "klips_debug:ipsec_device_event: "
			    "NETDEV_GOING_DOWN dev=%s\n",
			    dev->name);
		break;
	case NETDEV_CHANGENAME:
		KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
			    "klips_debug:ipsec_device_event: "
			    "NETDEV_CHANGENAME dev=%s\n",
			    dev->name);
		break;
#endif /* NET_21 */
	default:
		KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
			    "klips_debug:ipsec_device_event: "
			    "event type %ld unrecognised for dev=%s\n",
			    event,
			    dev->name);
		break;
	}
	return NOTIFY_DONE;
}

/*
 *	Called when an ipsec tunnel device is initialized.
 *	The ipsec tunnel device structure is passed to us.
 */
 
int
ipsec_tunnel_init(struct net_device *dev)
{
	int i;

	KLIPS_PRINT(debug_tunnel,
		    "klips_debug:ipsec_tunnel_init: "
		    "allocating %lu bytes initialising device: %s\n",
		    (unsigned long) sizeof(struct ipsecpriv),
		    dev->name ? dev->name : "NULL");

	/* Add our tunnel functions to the device */
	dev->open		= ipsec_tunnel_open;
	dev->stop		= ipsec_tunnel_close;
	dev->hard_start_xmit	= ipsec_tunnel_start_xmit;
	dev->get_stats		= ipsec_tunnel_get_stats;

	dev->priv = kmalloc(sizeof(struct ipsecpriv), GFP_KERNEL);
	if (dev->priv == NULL)
		return -ENOMEM;
	memset((caddr_t)(dev->priv), 0, sizeof(struct ipsecpriv));

	for(i = 0; i < sizeof(zeroes); i++) {
		((__u8*)(zeroes))[i] = 0;
	}
	
#ifndef NET_21
	/* Initialize the tunnel device structure */
	for (i = 0; i < DEV_NUMBUFFS; i++)
		skb_queue_head_init(&dev->buffs[i]);
#endif /* !NET_21 */

	dev->set_multicast_list = NULL;
	dev->do_ioctl		= ipsec_tunnel_ioctl;
	dev->hard_header	= NULL;
	dev->rebuild_header 	= NULL;
	dev->set_mac_address 	= NULL;
#ifndef NET_21
	dev->header_cache_bind 	= NULL;
#endif /* !NET_21 */
	dev->header_cache_update= NULL;

#ifdef NET_21
/*	prv->neigh_setup        = NULL; */
	dev->neigh_setup        = ipsec_tunnel_neigh_setup_dev;
#endif /* NET_21 */
	dev->hard_header_len 	= 0;
	dev->mtu		= 0;
	dev->addr_len		= 0;
	dev->type		= ARPHRD_VOID; /* ARPHRD_TUNNEL; */ /* ARPHRD_ETHER; */
	dev->tx_queue_len	= 10;		/* Small queue */
	memset((caddr_t)(dev->broadcast),0xFF, ETH_ALEN);	/* what if this is not attached to ethernet? */

	/* New-style flags. */
	dev->flags		= IFF_NOARP /* 0 */ /* Petr Novak */;

	/* We're done.  Have I forgotten anything? */
	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*  Module specific interface (but it links with the rest of IPSEC)  */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int
ipsec_tunnel_probe(struct net_device *dev)
{
	ipsec_tunnel_init(dev); 
	return 0;
}

struct net_device *ipsecdevices[IPSEC_NUM_IF];

int 
ipsec_tunnel_init_devices(void)
{
	int i;
	char name[IFNAMSIZ];
	struct net_device *dev_ipsec;
	
	KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
		    "klips_debug:ipsec_tunnel_init_devices: "
		    "creating and registering IPSEC_NUM_IF=%u devices, allocating %lu per device, IFNAMSIZ=%u.\n",
		    IPSEC_NUM_IF,
		    (unsigned long) (sizeof(struct net_device) + IFNAMSIZ),
		    IFNAMSIZ);

	for(i = 0; i < IPSEC_NUM_IF; i++) {
		sprintf(name, IPSEC_DEV_FORMAT, i);

		dev_ipsec = (struct net_device*)kmalloc(sizeof(struct net_device), GFP_KERNEL);

		if (dev_ipsec == NULL) {
			KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
				    "klips_debug:ipsec_tunnel_init_devices: "
				    "failed to allocate memory for device %s, quitting device init.\n",
				    name);
			return -ENOMEM;
		}
		memset((caddr_t)dev_ipsec, 0, sizeof(struct net_device));

#ifdef NETDEV_23
		strncpy(dev_ipsec->name, name, sizeof(dev_ipsec->name));
#else /* NETDEV_23 */
		dev_ipsec->name = (char*)kmalloc(IFNAMSIZ, GFP_KERNEL);
		if (dev_ipsec->name == NULL) {
			KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
				    "klips_debug:ipsec_tunnel_init_devices: "
				    "failed to allocate memory for device %s name, quitting device init.\n",
				    name);
			return -ENOMEM;
		}
		memset((caddr_t)dev_ipsec->name, 0, IFNAMSIZ);
		strncpy(dev_ipsec->name, name, IFNAMSIZ);
#endif /* NETDEV_23 */

		SET_MODULE_OWNER(dev_ipsec);

		dev_ipsec->next = NULL;
		dev_ipsec->init = &ipsec_tunnel_probe;
		KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
			    "klips_debug:ipsec_tunnel_init_devices: "
			    "registering device %s\n",
			    dev_ipsec->name);

		/* reference and hold the device reference */
		dev_hold(dev_ipsec);
		ipsecdevices[i]=dev_ipsec;

		if (register_netdev(dev_ipsec) != 0) {
			KLIPS_PRINT(1 || debug_tunnel & DB_TN_INIT,
				    "klips_debug:ipsec_tunnel_init_devices: "
				    "registering device %s failed, quitting device init.\n",
				    dev_ipsec->name);
			return -EIO;
		} else {
			KLIPS_PRINT(debug_tunnel & DB_TN_INIT,
				    "klips_debug:ipsec_tunnel_init_devices: "
				    "registering device %s succeeded, continuing...\n",
				    dev_ipsec->name);
		}
	}
	return 0;
}

/* void */
int
ipsec_tunnel_cleanup_devices(void)
{
	int error = 0;
	int i;
	char name[32];
	struct net_device *dev_ipsec;
	
	for(i = 0; i < IPSEC_NUM_IF; i++) {
   	        dev_ipsec = ipsecdevices[i];
		if(dev_ipsec == NULL) {
		  continue;
		}

		/* release reference */
		ipsecdevices[i]=NULL;
		ipsec_dev_put(dev_ipsec);

		KLIPS_PRINT(debug_tunnel, "Unregistering %s (refcnt=%d)\n",
			    name,
			    atomic_read(&dev_ipsec->refcnt));
		unregister_netdev(dev_ipsec);
		KLIPS_PRINT(debug_tunnel, "Unregisted %s\n", name);
#ifndef NETDEV_23
		kfree(dev_ipsec->name);
		dev_ipsec->name=NULL;
#endif /* !NETDEV_23 */
		kfree(dev_ipsec->priv);
		dev_ipsec->priv=NULL;
	}
	return error;
}

/*
 * $Log: ipsec_tunnel.c,v $
 * Revision 1.1  2004/08/02 02:09:59  rupert
 * +: Add Freeswan IPSec 2.06
 *
 * Revision 1.223  2004/02/24 17:17:04  mcr
 * 	s/CONFIG_IPSEC/CONFIG_KLIPS/ as 26sec uses "CONFIG_IPSEC" to
 * 	turn it on/off as well.
 *
 * Revision 1.222  2004/02/22 06:50:42  mcr
 * 	kernel 2.6 port - merged with 2.4 code.
 *
 * Revision 1.221.2.2  2004/02/22 04:50:12  mcr
 * 	restored 2.4, "ipsec%d" creation code.
 *
 * Revision 1.221.2.1  2004/02/20 14:10:18  mcr
 * 	moved code to net/ipsec/ to make 2.6 happy.
 *
 * Revision 1.221  2004/02/16 04:41:19  mcr
 * 	re-factored IPPROTO_INT code into ipsec_xmit_encap_internal()
 * 	fixed code - if we are going to send and %trap (therefore
 * 	save), then we should copy it. Note, we are going to send the
 * 	packet *TWICE*. A second time through the possible tunnel.
 * 	maybe this is wrong.
 * 	more debugging for port-53 examination - pick which which
 * 	source of port information is non-zero.
 *
 * Revision 1.220  2004/02/14 21:59:52  mcr
 * 	TRAPPASS code - port-53 code.
 *
 * Revision 1.219  2004/02/14 21:17:21  mcr
 * 	re-factored port-500 hole to permit port-53 trap/pass hole
 * 	to be added easily.
 *
 * Revision 1.217  2003/12/13 04:09:21  mcr
 * 	AH transform removed.
 *
 * Revision 1.216  2003/12/04 19:05:55  mcr
 * 	cleaned up "sa_id" structure to use "ip_said" only.
 *
 * Revision 1.215  2003/12/03 18:48:04  mcr
 * 	additional fix for ARPATM code.
 *
 * Revision 1.214  2003/12/01 21:13:52  mcr
 * 	trust hardheader for ARPATM case.
 *
 * Revision 1.213  2003/11/07 02:58:06  mcr
 * 	backout of port-selector and X.509 patches
 *
 * Revision 1.211  2003/09/10 16:46:30  mcr
 * 	patches for 2.4 backport/2.6 existence.
 *
 * Revision 1.210  2003/07/31 22:47:16  mcr
 * 	preliminary (untested by FS-team) 2.5 patches.
 *
 * Revision 1.209  2003/06/22 21:28:43  mcr
 * 	inability to unload module was caused by calls to dev_get
 * 	(ipsec_dev_get), to gather a device from a name. There is
 * 	simply no reason to look the devices up - they should be kept
 * 	in a nice array, ready for use.
 *
 * Revision 1.208  2003/06/22 21:25:07  mcr
 * 	all staticly counted ipsecXXX device support removed.
 *
 * Revision 1.207  2003/04/02 20:15:37  mcr
 * 	fix for PR#204 - do not clear connection tracking info if we
 * 	the packet is being sent in the clear.
 *
 * Revision 1.206  2003/02/12 19:32:51  rgb
 * Refactored file to:
 * ipsec_xmit.c
 * ipsec_xmit.h
 * ipsec_mast.c
 *
 * Revision 1.205  2003/02/06 17:47:00  rgb
 *
 * Remove unused ipsec_tunnel_lock() and ipsec_tunnel_unlock() code.
 * Refactor ipsec_tunnel_start_xmit() further into:
 *         ipsec_xmit_sanity_check_dev()
 *         ipsec_xmit_sanity_check_skb()
 *         ipsec_xmit_strip_hard_header()
 *         ipsec_xmit_restore_hard_header()
 *         ipsec_xmit_send()
 *         ipsec_xmit_cleanup()
 * and start a skeletal ipsec_mast_start_xmit() .
 *
 * Revision 1.204  2003/02/06 06:43:46  rgb
 *
 * Refactor ipsec_tunnel_start_xmit, bringing out:
 *     ipsec_xmit_SAlookup
 *     ipsec_xmit_encap_once
 *     ipsec_xmit_encap_bundle
 *
 * Revision 1.203  2003/02/06 02:21:34  rgb
 *
 * Moved "struct auth_alg" from ipsec_rcv.c to ipsec_ah.h .
 * Changed "struct ah" to "struct ahhdr" and "struct esp" to "struct esphdr".
 * Removed "#ifdef INBOUND_POLICY_CHECK_eroute" dead code.
 *
 * Revision 1.202  2003/01/03 07:38:01  rgb
 *
 * Start to refactor ipsec_tunnel_start_xmit() by putting local variables
 * into struct ipsec_xmit_state and renaming a few variables to give more
 * unique or searchable names.
 *
 * Revision 1.201  2003/01/03 00:31:28  rgb
 *
 * Clean up memset usage, including fixing 2 places where keys were not
 * properly wiped.
 *
 * *** removed old history from file, as it was 50% of file ***
 *
 * Local Variables:
 * c-style: linux
 * End:
 */
