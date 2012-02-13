/* ifconfig
 *
 * Similar to the standard Unix ifconfig, but with only the necessary
 * parts for AF_INET, and without any printing of if info (for now).
 *
 * Bjorn Wesen, Axis Communications AB
 *
 *
 * Authors of the original ifconfig was:      
 *              Fred N. van Kempen, <waltje@uwalt.nl.mugnet.org>
 *
 * This program is free software; you can redistribute it
 * and/or  modify it under  the terms of  the GNU General
 * Public  License as  published  by  the  Free  Software
 * Foundation;  either  version 2 of the License, or  (at
 * your option) any later version.
 *
 * $Id: ifconfig.c,v 1.1.1.1 2003/08/27 06:20:15 rupert Exp $
 *
 */

/*
 * Heavily modified by Manuel Novoa III       Mar 6, 2001
 *
 * From initial port to busybox, removed most of the redundancy by
 * converting to a table-driven approach.  Added several (optional)
 * args missing from initial port.
 *
 * Still missing:  media, tunnel.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>   // strcmp and friends
#include <ctype.h>    // isdigit and friends
#include <stddef.h>				/* offsetof */
#include <sys/types.h>
#include <sys/socket.h>
#include <net/route.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <linux/if_ether.h>

#ifdef BB_FEATURE_IFCONFIG_SLIP
#include <linux/if_slip.h>
#endif

/* I don't know if this is needed for busybox or not.  Anyone? */
#define QUESTIONABLE_ALIAS_CASE

#define RTACTION_ADD   1
#define RTACTION_DEL   2
#define RTACTION_HELP  3
#define RTACTION_FLUSH 4
#define RTACTION_SHOW  5
#define E_LOOKUP	-1
#define E_OPTERR	-2
#define E_SOCK	-3
#define mask_in_addr(x) (((struct sockaddr_in *)&((x).rt_genmask))->sin_addr.s_addr)
#define full_mask(x) (x)


/* Defines for glibc2.0 users. */
#ifndef SIOCSIFTXQLEN
#define SIOCSIFTXQLEN      0x8943
#define SIOCGIFTXQLEN      0x8942
#endif

/* ifr_qlen is ifru_ivalue, but it isn't present in 2.0 kernel headers */
#ifndef ifr_qlen
#define ifr_qlen        ifr_ifru.ifru_mtu
#endif

#ifndef IFF_DYNAMIC
#define IFF_DYNAMIC     0x8000  /* dialup device with changing addresses */
#endif

/*
 * Here are the bit masks for the "flags" member of struct options below.
 * N_ signifies no arg prefix; M_ signifies arg prefixed by '-'.
 * CLR clears the flag; SET sets the flag; ARG signifies (optional) arg.
 */
#define N_CLR            0x01
#define M_CLR            0x02
#define N_SET            0x04
#define M_SET            0x08
#define N_ARG            0x10
#define M_ARG            0x20

#define M_MASK           (M_CLR | M_SET | M_ARG)
#define N_MASK           (N_CLR | N_SET | N_ARG)
#define SET_MASK         (N_SET | M_SET)
#define CLR_MASK         (N_CLR | M_CLR)
#define SET_CLR_MASK     (SET_MASK | CLR_MASK)
#define ARG_MASK         (M_ARG | N_ARG)

/*
 * Here are the bit masks for the "arg_flags" member of struct options below.
 */

/*
 * cast type:
 *   00 int
 *   01 char *
 *   02 HOST_COPY in_ether
 *   03 HOST_COPY INET_resolve
 */
#define A_CAST_TYPE      0x03
/*
 * map type:
 *   00 not a map type (mem_start, io_addr, irq)
 *   04 memstart (unsigned long)
 *   08 io_addr  (unsigned short)
 *   0C irq      (unsigned char)
 */
#define A_MAP_TYPE       0x0C
#define A_ARG_REQ        0x10	/* Set if an arg is required. */
#define A_NETMASK        0x20	/* Set if netmask (check for multiple sets). */
#define A_SET_AFTER      0x40	/* Set a flag at the end. */
#define A_COLON_CHK      0x80	/* Is this needed?  See below. */

/*
 * These defines are for dealing with the A_CAST_TYPE field.
 */
#define A_CAST_CHAR_PTR  0x01
#define A_CAST_RESOLVE   0x01
#define A_CAST_HOST_COPY 0x02
#define A_CAST_HOST_COPY_IN_ETHER    A_CAST_HOST_COPY
#define A_CAST_HOST_COPY_RESOLVE     (A_CAST_HOST_COPY | A_CAST_RESOLVE)

/*
 * These defines are for dealing with the A_MAP_TYPE field.
 */
#define A_MAP_ULONG      0x04	/* memstart */
#define A_MAP_USHORT     0x08	/* io_addr */
#define A_MAP_UCHAR      0x0C	/* irq */

/*
 * Define the bit masks signifying which operations to perform for each arg.
 */

#define ARG_METRIC       (A_ARG_REQ /*| A_CAST_INT*/)
#define ARG_MTU          (A_ARG_REQ /*| A_CAST_INT*/)
#define ARG_TXQUEUELEN   (A_ARG_REQ /*| A_CAST_INT*/)
#define ARG_MEM_START    (A_ARG_REQ | A_MAP_ULONG)
#define ARG_IO_ADDR      (A_ARG_REQ | A_MAP_ULONG)
#define ARG_IRQ          (A_ARG_REQ | A_MAP_UCHAR)
#define ARG_DSTADDR      (A_ARG_REQ | A_CAST_HOST_COPY_RESOLVE)
#define ARG_NETMASK      (A_ARG_REQ | A_CAST_HOST_COPY_RESOLVE | A_NETMASK)
#define ARG_BROADCAST    (A_ARG_REQ | A_CAST_HOST_COPY_RESOLVE | A_SET_AFTER)
#define ARG_HW           (A_ARG_REQ | A_CAST_HOST_COPY_IN_ETHER)
#define ARG_POINTOPOINT  (A_CAST_HOST_COPY_RESOLVE | A_SET_AFTER)
#define ARG_KEEPALIVE    (A_ARG_REQ | A_CAST_CHAR_PTR)
#define ARG_OUTFILL      (A_ARG_REQ | A_CAST_CHAR_PTR)
#define ARG_HOSTNAME     (A_CAST_HOST_COPY_RESOLVE | A_SET_AFTER | A_COLON_CHK)


/*
 * Set up the tables.  Warning!  They must have corresponding order!
 */

struct arg1opt {
	const char *name;
	unsigned short selector;
	unsigned short ifr_offset;
};

struct options {
	const char *name;
	const unsigned char flags;
	const unsigned char arg_flags;
	const unsigned short selector;
};

#define ifreq_offsetof(x)  offsetof(struct ifreq, x)

static const struct arg1opt Arg1Opt[] = {
	{"SIOCSIFMETRIC",  SIOCSIFMETRIC,  ifreq_offsetof(ifr_metric)},
	{"SIOCSIFMTU",     SIOCSIFMTU,     ifreq_offsetof(ifr_mtu)},
	{"SIOCSIFTXQLEN",  SIOCSIFTXQLEN,  ifreq_offsetof(ifr_qlen)},
	{"SIOCSIFDSTADDR", SIOCSIFDSTADDR, ifreq_offsetof(ifr_dstaddr)},
	{"SIOCSIFNETMASK", SIOCSIFNETMASK, ifreq_offsetof(ifr_netmask)},
	{"SIOCSIFBRDADDR", SIOCSIFBRDADDR, ifreq_offsetof(ifr_broadaddr)},
#ifdef BB_FEATURE_IFCONFIG_HW
	{"SIOCSIFHWADDR",  SIOCSIFHWADDR,  ifreq_offsetof(ifr_hwaddr)},
#endif
	{"SIOCSIFDSTADDR", SIOCSIFDSTADDR, ifreq_offsetof(ifr_dstaddr)},
#ifdef SIOCSKEEPALIVE
	{"SIOCSKEEPALIVE", SIOCSKEEPALIVE, ifreq_offsetof(ifr_data)},
#endif
#ifdef SIOCSOUTFILL
	{"SIOCSOUTFILL",   SIOCSOUTFILL,   ifreq_offsetof(ifr_data)},
#endif
#ifdef BB_FEATURE_IFCONFIG_MEMSTART_IOADDR_IRQ
	{"SIOCSIFMAP",     SIOCSIFMAP,     ifreq_offsetof(ifr_map.mem_start)},
	{"SIOCSIFMAP",     SIOCSIFMAP,     ifreq_offsetof(ifr_map.base_addr)},
	{"SIOCSIFMAP",     SIOCSIFMAP,     ifreq_offsetof(ifr_map.irq)},
#endif
	/* Last entry if for unmatched (possibly hostname) arg. */
	{"SIOCSIFADDR",    SIOCSIFADDR,    ifreq_offsetof(ifr_addr)},
};

static const struct options OptArray[] = {
	{"metric",       N_ARG,         ARG_METRIC,      0},
    {"mtu",          N_ARG,         ARG_MTU,         0},
	{"txqueuelen",   N_ARG,         ARG_TXQUEUELEN,  0},
	{"dstaddr",      N_ARG,         ARG_DSTADDR,     0},
	{"netmask",      N_ARG,         ARG_NETMASK,     0},
	{"broadcast",    N_ARG | M_CLR, ARG_BROADCAST,   IFF_BROADCAST},
#ifdef BB_FEATURE_IFCONFIG_HW
	{"hw",           N_ARG,         ARG_HW,          0},
#endif
	{"pointopoint",  N_ARG | M_CLR, ARG_POINTOPOINT, IFF_POINTOPOINT},
#ifdef SIOCSKEEPALIVE
	{"keepalive",    N_ARG,         ARG_KEEPALIVE,   0},
#endif
#ifdef SIOCSOUTFILL
	{"outfill",      N_ARG,         ARG_OUTFILL,     0},
#endif
#ifdef BB_FEATURE_IFCONFIG_MEMSTART_IOADDR_IRQ
	{"mem_start",    N_ARG,         ARG_MEM_START,   0},
	{"io_addr",      N_ARG,         ARG_IO_ADDR,     0},
	{"irq",          N_ARG,         ARG_IRQ,         0},
#endif
	{"arp",          N_CLR | M_SET, 0,               IFF_NOARP},
	{"trailers",     N_CLR | M_SET, 0,               IFF_NOTRAILERS},
	{"promisc",      N_SET | M_CLR, 0,               IFF_PROMISC},
	{"multicast",    N_SET | M_CLR, 0,               IFF_MULTICAST},
	{"allmulti",     N_SET | M_CLR, 0,               IFF_ALLMULTI},
	{"dynamic",      N_SET | M_CLR, 0,               IFF_DYNAMIC},
	{"up",           N_SET        , 0,               (IFF_UP | IFF_RUNNING)},
	{"down",         N_CLR        , 0,               IFF_UP},
	{ NULL,          0,             ARG_HOSTNAME,    (IFF_UP | IFF_RUNNING)}
};

/*
 * A couple of prototypes.
 */

#ifdef BB_FEATURE_IFCONFIG_HW
static int in_ether(char *bufp, struct sockaddr *sap);
#endif

#ifdef BB_FEATURE_IFCONFIG_STATUS
extern int interface_opt_a;
extern int display_interfaces(char *ifname);
#endif

/*
 * Our main function.
 */
void show_usage()
{
}
#define error_msg(fmt,args...) 
#define ENOSUPP(fmt,args...) 



static int
INET_resolve(char *name, struct sockaddr *sa)
{
	struct sockaddr_in *s_in = (struct sockaddr_in *)sa;

	s_in->sin_family = AF_INET;
	s_in->sin_port = 0;

	/* Default is special, meaning 0.0.0.0. */
	if (strcmp(name, "default")==0) {
		s_in->sin_addr.s_addr = INADDR_ANY;
		return 1;
	}
	/* Look to see if it's a dotted quad. */
	if (inet_aton(name, &s_in->sin_addr)) {
		return 0;
	}
	/* guess not.. */
	return -1;
}


static int
INET_setroute(int action, int options, char **args)
{
	struct rtentry rt;
	char target[128], gateway[128] = "NONE", netmask[128] = "default";
	int xflag, isnet;
	int skfd;

	xflag = 0;

	if (*args == NULL)
	    show_usage();
	if (strcmp(*args, "-net")==0) {
		xflag = 1;
		args++;
	} else if (strcmp(*args, "-host")==0) {
		xflag = 2;
		args++;
	}
	strncpy(target, *args++, (sizeof target));

	/* Clean out the RTREQ structure. */
	memset((char *) &rt, 0, sizeof(struct rtentry));


	if ((isnet = INET_resolve(target, &rt.rt_dst)) < 0) {
		printf("can't resolve %s", target);
		return -1;   /* XXX change to E_something */
	}

	switch (xflag) {
		case 1:
			isnet = 1;
			break;

		case 2:
			isnet = 0;
			break;

		default:
			break;
	}

	/* Fill in the other fields. */
	rt.rt_flags = (RTF_UP | RTF_HOST);
	if (isnet)
		rt.rt_flags &= ~RTF_HOST;

	while (*args) {
		if (strcmp(*args, "metric")==0) {
			int metric;

			args++;
			if (!*args || !isdigit(**args))
				show_usage();
			metric = atoi(*args);
#if HAVE_NEW_ADDRT
			rt.rt_metric = metric + 1;
#else
			ENOSUPP("inet_setroute", "NEW_ADDRT (metric)");  /* XXX Fixme */
#endif
			args++;
			continue;
		}

		if (strcmp(*args, "netmask")==0) {
			struct sockaddr mask;

			args++;
			if (!*args || mask_in_addr(rt))
				show_usage();
			strncpy(netmask, *args, (sizeof netmask));
			if ((isnet = INET_resolve(netmask, &mask)) < 0) {
				error_msg(_("can't resolve netmask %s"), netmask);
				return E_LOOKUP;
			}
			rt.rt_genmask = full_mask(mask);
			args++;
			continue;
		}

		if (strcmp(*args, "gw")==0 || strcmp(*args, "gateway")==0) {
			args++;
			if (!*args)
				show_usage();
			if (rt.rt_flags & RTF_GATEWAY)
				show_usage();
			strncpy(gateway, *args, (sizeof gateway));
			if ((isnet = INET_resolve(gateway, &rt.rt_gateway)) < 0) {
				error_msg(_("can't resolve gw %s"), gateway);
				return E_LOOKUP;
			}
			if (isnet) {
				error_msg(
					_("%s: cannot use a NETWORK as gateway!"),
					gateway);
				return E_OPTERR;
			}
			rt.rt_flags |= RTF_GATEWAY;
			args++;
			continue;
		}

		if (strcmp(*args, "mss")==0) {
			args++;
			rt.rt_flags |= RTF_MSS;
			if (!*args)
				show_usage();
			rt.rt_mss = atoi(*args);
			args++;
			if (rt.rt_mss < 64 || rt.rt_mss > 32768) {
				error_msg(_("Invalid MSS."));
				return E_OPTERR;
			}
			continue;
		}

		if (strcmp(*args, "window")==0) {
			args++;
			if (!*args)
				show_usage();
			rt.rt_flags |= RTF_WINDOW;
			rt.rt_window = atoi(*args);
			args++;
			if (rt.rt_window < 128) {
				error_msg(_("Invalid window."));
				return E_OPTERR;
			}
			continue;
		}

		if (strcmp(*args, "irtt")==0) {
			args++;
			if (!*args)
				show_usage();
			args++;
#if HAVE_RTF_IRTT
			rt.rt_flags |= RTF_IRTT;
			rt.rt_irtt = atoi(*(args - 1));
			rt.rt_irtt *= (HZ / 100);       /* FIXME */
#if 0                           /* FIXME: do we need to check anything of this? */
			if (rt.rt_irtt < 1 || rt.rt_irtt > (120 * HZ)) {
				error_msg(_("Invalid initial rtt."));
				return E_OPTERR;
			}
#endif
#else
			ENOSUPP("inet_setroute", "RTF_IRTT"); /* XXX Fixme */
#endif
			continue;
		}

		if (strcmp(*args, "reject")==0) {
			args++;
#if HAVE_RTF_REJECT
			rt.rt_flags |= RTF_REJECT;
#else
			ENOSUPP("inet_setroute", "RTF_REJECT"); /* XXX Fixme */
#endif
			continue;
		}
		if (strcmp(*args, "mod")==0) {
			args++;
			rt.rt_flags |= RTF_MODIFIED;
			continue;
		}
		if (strcmp(*args, "dyn")==0) {
			args++;
			rt.rt_flags |= RTF_DYNAMIC;
			continue;
		}
		if (strcmp(*args, "reinstate")==0) {
			args++;
			rt.rt_flags |= RTF_REINSTATE;
			continue;
		}
		if (strcmp(*args, "device")==0 || strcmp(*args, "dev")==0) {
			args++;
			if (rt.rt_dev || *args == NULL)
				show_usage();
			rt.rt_dev = *args++;
			continue;
		}
		/* nothing matches */
		if (!rt.rt_dev) {
			rt.rt_dev = *args++;
			if (*args)
				show_usage();   /* must be last to catch typos */
		} else {
			show_usage();
		}
	}

#if HAVE_RTF_REJECT
	if ((rt.rt_flags & RTF_REJECT) && !rt.rt_dev)
		rt.rt_dev = "lo";
#endif

	/* sanity checks.. */
	if (mask_in_addr(rt)) {
		unsigned long mask = mask_in_addr(rt);
		mask = ~ntohl(mask);
		if ((rt.rt_flags & RTF_HOST) && mask != 0xffffffff) {
			error_msg(
				_("netmask %.8x doesn't make sense with host route"),
				(unsigned int)mask);
			return E_OPTERR;
		}
		if (mask & (mask + 1)) {
			error_msg(_("bogus netmask %s"), netmask);
			return E_OPTERR;
		}
		mask = ((struct sockaddr_in *) &rt.rt_dst)->sin_addr.s_addr;
		if (mask & ~mask_in_addr(rt)) {
			error_msg(_("netmask doesn't match route address"));
			return E_OPTERR;
		}
	}
	/* Fill out netmask if still unset */
	if ((action == RTACTION_ADD) && rt.rt_flags & RTF_HOST)
		mask_in_addr(rt) = 0xffffffff;

	/* Create a socket to the INET kernel. */
	if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return E_SOCK;
	}
	/* Tell the kernel to accept this route. */
	if (action == RTACTION_DEL) {
		if (ioctl(skfd, SIOCDELRT, &rt) < 0) {
			perror("SIOCDELRT");
			close(skfd);
			return E_SOCK;
		}
	} else {
		if (ioctl(skfd, SIOCADDRT, &rt) < 0) {
			perror("SIOCADDRT");
			close(skfd);
			return E_SOCK;
		}
	}

	/* Close the socket. */
	(void) close(skfd);
	return EXIT_SUCCESS;
}




int main(int argc, char **argv)
{
	struct ifreq ifr;
	struct sockaddr_in sai;
	int sockfd;  /* socket fd we use to manipulate stuff with */
	char host[128];


	/* Create a channel to the NET kernel. */
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("erro socket");
	}

	/* get interface name */
	memset((char*)&ifr,0,sizeof(ifr));
	memset((char*)&host,0,sizeof(host));
	memset((char*)&sai,0,sizeof(sai));
	strcpy(ifr.ifr_name, "eth0");
	strcpy(host, "172.19.31.145");
	sai.sin_family = AF_INET;
	sai.sin_addr.s_addr = INADDR_ANY;
	sai.sin_port = 0;

	if(inet_aton(host, &sai.sin_addr) == 0) 
	{
		printf("not dotted\n");
	}
	memcpy((char*)&ifr+ifreq_offsetof(ifr_addr),(char*)&sai,
		       sizeof(struct sockaddr));
	if (ioctl(sockfd, SIOCSIFADDR, &ifr) < 0) 
		perror("error ioctl\n");
	
	
	
	strcpy(host, "255.255.255.0");
	sai.sin_family = AF_INET;
	sai.sin_addr.s_addr = INADDR_ANY;
	sai.sin_port = 0;

	if(inet_aton(host, &sai.sin_addr) == 0) 
	{
		printf("not dotted\n");
	}
	memcpy((char*)&ifr+ifreq_offsetof(ifr_netmask),(char*)&sai,
		       sizeof(struct sockaddr));

	if (ioctl(sockfd, SIOCSIFNETMASK, &ifr) < 0) 
		perror("error ioctl\n");

	ioctl(sockfd, SIOCGIFFLAGS, &ifr);
	ifr.ifr_flags|=IFF_UP|IFF_RUNNING;
	if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) < 0) 
		perror("SIOCSIFFLAGS"); 

	//INET_setroute(RTACTION_ADD, 0, ++argv);




}

#ifdef BB_FEATURE_IFCONFIG_HW
/* Input an Ethernet address and convert to binary. */
static int
in_ether(char *bufp, struct sockaddr *sap)
{
	unsigned char *ptr;
	int i, j;
	unsigned char val;
	unsigned char c;
	
	sap->sa_family = ARPHRD_ETHER;
	ptr = sap->sa_data;
	
	for (i = 0 ; i < ETH_ALEN ; i++) {
		val = 0;

		/* We might get a semicolon here - not required. */
		if (i && (*bufp == ':')) {
			bufp++;
		}

		for (j=0 ; j<2 ; j++) {
			c = *bufp;
			if (c >= '0' && c <= '9') {
				c -= '0';
			} else if (c >= 'a' && c <= 'f') {
				c -= ('a' - 10);
			} else if (c >= 'A' && c <= 'F') {
				c -= ('A' - 10);
			} else if (j && (c == ':' || c == 0)) {
				break;
			} else {
				return -1;
			}
			++bufp;
			val <<= 4;
			val += c;
		}
		*ptr++ = val;
	}

	return (int) (*bufp);		/* Error if we don't end at end of string. */
}
#endif
