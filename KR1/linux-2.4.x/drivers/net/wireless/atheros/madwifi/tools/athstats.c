/*-
 * Copyright (c) 2002-2004 Sam Leffler, Errno Consulting
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
 * $FreeBSD: src/tools/tools/ath/athstats.c,v 1.4 2003/12/07 21:40:52 sam Exp $
 */

/*
 * Simple Atheros-specific tool to inspect and monitor network traffic
 * statistics.
 *	athstats [-i interface] [interval]
 * (default interface is ath0).  If interval is specified a rolling output
 * is displayed every interval seconds.
 */
#include <sys/types.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/wireless.h>

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>

#include "ah_desc.h"
#include "if_athioctl.h"

static const struct {
	u_int		phyerr;
	const char*	desc;
} phyerrdescriptions[] = {
	{ HAL_PHYERR_UNDERRUN,		"transmit underrun" },
	{ HAL_PHYERR_TIMING,		"timing error" },
	{ HAL_PHYERR_PARITY,		"illegal parity" },
	{ HAL_PHYERR_RATE,		"illegal rate" },
	{ HAL_PHYERR_LENGTH,		"illegal length" },
	{ HAL_PHYERR_RADAR,		"radar detect" },
	{ HAL_PHYERR_SERVICE,		"illegal service" },
	{ HAL_PHYERR_TOR,		"transmit override receive" },
	{ HAL_PHYERR_OFDM_TIMING,	"OFDM timing" },
	{ HAL_PHYERR_OFDM_SIGNAL_PARITY,"OFDM illegal parity" },
	{ HAL_PHYERR_OFDM_RATE_ILLEGAL,	"OFDM illegal rate" },
	{ HAL_PHYERR_OFDM_POWER_DROP,	"OFDM power drop" },
	{ HAL_PHYERR_OFDM_SERVICE,	"OFDM illegal service" },
	{ HAL_PHYERR_OFDM_RESTART,	"OFDM restart" },
	{ HAL_PHYERR_CCK_TIMING,	"CCK timing" },
	{ HAL_PHYERR_CCK_HEADER_CRC,	"CCK header crc" },
	{ HAL_PHYERR_CCK_RATE_ILLEGAL,	"CCK illegal rate" },
	{ HAL_PHYERR_CCK_SERVICE,	"CCK illegal service" },
	{ HAL_PHYERR_CCK_RESTART,	"CCK restart" },
};

static void
printstats(FILE *fd, const struct ath_stats *stats)
{
#define	N(a)	(sizeof(a) / sizeof(a[0]))
#define	STAT(x,fmt) \
	if (stats->ast_##x) fprintf(fd, "%u " fmt "\n", stats->ast_##x)
#define	STATI(x,fmt) \
	if (stats->ast_##x) fprintf(fd, "%d " fmt "\n", stats->ast_##x)
	int i, j;

	STAT(watchdog, "watchdog timeouts");
	STAT(hardware, "hardware error interrupts");
	STAT(bmiss, "beacon miss interrupts");
	STAT(rxorn, "recv overrun interrupts");
	STAT(rxeol, "recv eol interrupts");
	STAT(txurn, "txmit underrun interrupts");
	STAT(tx_mgmt, "tx management frames");
	STAT(tx_discard, "tx frames discarded prior to association");
	STAT(tx_invalid, "tx frames discarded 'cuz device gone");
	STAT(tx_qstop, "tx queue stopped because full");
	STAT(tx_encap, "tx encapsulation failed");
	STAT(tx_nonode, "tx failed 'cuz no node");
	STAT(tx_nobuf, "tx failed 'cuz no tx buffer (data)");
	STAT(tx_nobufmgt, "tx failed 'cuz no tx buffer (mgt)");
	STAT(tx_xretries, "tx failed 'cuz too many retries");
	STAT(tx_fifoerr, "tx failed 'cuz FIFO underrun");
	STAT(tx_filtered, "tx failed 'cuz xmit filtered");
	STAT(tx_shortretry, "short on-chip tx retries");
	STAT(tx_longretry, "long on-chip tx retries");
	STAT(tx_badrate, "tx failed 'cuz bogus xmit rate");
	STAT(tx_noack, "tx frames with no ack marked");
	STAT(tx_rts, "tx frames with rts enabled");
	STAT(tx_cts, "tx frames with cts enabled");
	STAT(tx_shortpre, "tx frames with short preamble");
	STAT(tx_altrate, "tx frames with an alternate rate");
	STAT(tx_protect, "tx frames with 11g protection");
	STAT(rx_orn, "rx failed 'cuz of desc overrun");
	STAT(rx_crcerr, "rx failed 'cuz of bad CRC");
	STAT(rx_fifoerr, "rx failed 'cuz of FIFO overrun");
	STAT(rx_badcrypt, "rx failed 'cuz decryption");
	STAT(rx_badmic, "rx failed 'cuz MIC failure");
	STAT(rx_tooshort, "rx failed 'cuz frame too short");
	STAT(rx_nobuf, "rx setup failed 'cuz no skbuff");
	STAT(rx_mgt, "rx management frames");
	STAT(rx_ctl, "rx control frames");
	STAT(rx_phyerr, "PHY errors");
	if (stats->ast_rx_phyerr != 0) {
		for (i = 0; i < 32; i++) {
			if (stats->ast_rx_phy[i] == 0)
				continue;
			for (j = 0; j < N(phyerrdescriptions); j++)
				if (phyerrdescriptions[j].phyerr == i)
					break;
			if (j == N(phyerrdescriptions))
				fprintf(fd,
					"    %u (unknown phy error code %u)\n",
					stats->ast_rx_phy[i], i);
			else
				fprintf(fd, "    %u %s\n",
					stats->ast_rx_phy[i],
					phyerrdescriptions[j].desc);
		}
	}
	STAT(be_nobuf, "no skbuff available for beacon");
	STAT(per_cal, "periodic calibrations");
	STAT(per_calfail, "periodic calibration failures");
	STAT(per_rfgain, "rfgain value change");
	STAT(rate_calls, "rate control checks");
	STAT(rate_raise, "rate control raised xmit rate");
	STAT(rate_drop, "rate control dropped xmit rate");
	if (stats->ast_tx_rssi)
		fprintf(fd, "rssi of last ack: %u\n", stats->ast_tx_rssi);
	if (stats->ast_rx_rssi)
		fprintf(fd, "rssi of last rcv: %u\n", stats->ast_rx_rssi);
	STAT(ant_defswitch, "switched default/rx antenna");
	STAT(ant_txswitch, "tx used alternate antenna");
	fprintf(fd, "Antenna profile:\n");
	for (i = 0; i < 8; i++)
		if (stats->ast_ant_rx[i] || stats->ast_ant_tx[i])
			fprintf(fd, "[%u] tx %8u rx %8u\n", i,
				stats->ast_ant_tx[i], stats->ast_ant_rx[i]);
#undef STAT
#undef STATI
#undef N
}

static u_int
getifrate(int s, const char* ifname)
{
	struct iwreq wrq;

	(void) memset(&wrq, 0, sizeof(wrq));
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);
	if (ioctl(s, SIOCGIWRATE, &wrq) < 0)
		return 0;
	else
		return wrq.u.bitrate.value / 1000000;
}

static u_int
getrssi(int s, const char* ifname)
{
	struct iw_statistics stats;
	struct iwreq wrq;

	(void) memset(&wrq, 0, sizeof(wrq));
	wrq.u.data.pointer = (caddr_t) &stats;
	wrq.u.data.flags = 1;
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);
	if (ioctl(s, SIOCGIWSTATS, &wrq) < 0)
		return 0;
	else
		return stats.qual.qual;
}

static int
getifstats(const char *ifname, u_long *iframes, u_long *oframes)
{
	FILE * fd = fopen("/proc/net/dev", "r");
	if (fd != NULL) {
		char line[256];
		while (fgets(line, sizeof(line), fd)) {
			char *cp, *tp;

			for (cp = line; isspace(*cp); cp++)
				;
			if (cp[0] != ifname[0])
				continue;
			for (tp = cp; *tp != ':' && *tp; tp++)
				;
			if (*tp == ':') {
				*tp++ = '\0';
				if (strcmp(cp, ifname) != 0)
					continue;
				sscanf(tp, "%*llu %lu %*u %*u %*u %*u %*u %*u %*llu %lu",
					iframes, oframes);
				fclose(fd);
				return 1;
			}
		}
		fclose(fd);
	}
	return 0;
}

static int signalled;

static void
catchalarm(int signo)
{
	signalled = 1;
}

int
main(int argc, char *argv[])
{
	int s;
	struct ifreq ifr;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
		err(1, "socket");
	if (argc > 1 && strcmp(argv[1], "-i") == 0) {
		if (argc < 2) {
			fprintf(stderr, "%s: missing interface name for -i\n",
				argv[0]);
			exit(-1);
		}
		strncpy(ifr.ifr_name, argv[2], sizeof (ifr.ifr_name));
		argc -= 2, argv += 2;
	} else
		strncpy(ifr.ifr_name, "ath0", sizeof (ifr.ifr_name));
	if (argc > 1) {
		u_long interval = strtoul(argv[1], NULL, 0);
		u_long off;
		int line, omask;
		u_int rate, rssi;
		struct ath_stats cur, total;
		u_long icur, ocur;
		u_long itot, otot;

		if (interval < 1)
			interval = 1;
		signal(SIGALRM, catchalarm);
		signalled = 0;
		alarm(interval);
	banner:
		printf("%8s %8s %7s %7s %7s %6s %6s %6s %7s %4s %4s"
			, "input"
			, "output"
			, "altrate"
			, "short"
			, "long"
			, "xretry"
			, "crcerr"
			, "crypt"
			, "phyerr"
			, "rssi"
			, "rate"
		);
		putchar('\n');
		fflush(stdout);
		line = 0;
	loop:
		rate = getifrate(s, ifr.ifr_name);
		rssi = getrssi(s, ifr.ifr_name);
		if (line != 0) {
			ifr.ifr_data = (caddr_t) &cur;
			if (ioctl(s, SIOCGATHSTATS, &ifr) < 0)
				err(1, ifr.ifr_name);
			if (!getifstats(ifr.ifr_name, &icur, &ocur))
				err(1, ifr.ifr_name);
			printf("%8u %8u %7u %7u %7u %6u %6u %6u %7u %4u %3uM\n"
				, (icur - itot) -
					(cur.ast_rx_mgt - total.ast_rx_mgt)
				, ocur - otot
				, cur.ast_tx_altrate - total.ast_tx_altrate
				, cur.ast_tx_shortretry - total.ast_tx_shortretry
				, cur.ast_tx_longretry - total.ast_tx_longretry
				, cur.ast_tx_xretries - total.ast_tx_xretries
				, cur.ast_rx_crcerr - total.ast_rx_crcerr
				, cur.ast_rx_badcrypt - total.ast_rx_badcrypt
				, cur.ast_rx_phyerr - total.ast_rx_phyerr
				, rssi
				, rate
			);
			total = cur;
			itot = icur;
			otot = ocur;
		} else {
			ifr.ifr_data = (caddr_t) &total;
			if (ioctl(s, SIOCGATHSTATS, &ifr) < 0)
				err(1, ifr.ifr_name);
			if (!getifstats(ifr.ifr_name, &itot, &otot))
				err(1, ifr.ifr_name);
			printf("%8u %8u %7u %7u %7u %6u %6u %6u %7u %4u %3uM\n"
				, itot - total.ast_rx_mgt
				, otot
				, total.ast_tx_altrate
				, total.ast_tx_shortretry
				, total.ast_tx_longretry
				, total.ast_tx_xretries
				, total.ast_rx_crcerr
				, total.ast_rx_badcrypt
				, total.ast_rx_phyerr
				, rssi
				, rate
			);
		}
		fflush(stdout);
		omask = sigblock(sigmask(SIGALRM));
		if (!signalled)
			sigpause(0);
		sigsetmask(omask);
		signalled = 0;
		alarm(interval);
		line++;
		if (line == 21)		/* XXX tty line count */
			goto banner;
		else
			goto loop;
		/*NOTREACHED*/
	} else {
		struct ath_stats stats;

		ifr.ifr_data = (caddr_t) &stats;
		if (ioctl(s, SIOCGATHSTATS, &ifr) < 0)
			err(1, ifr.ifr_name);
		printstats(stdout, &stats);
	}
	return 0;
}
