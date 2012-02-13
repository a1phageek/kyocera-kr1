/*
 * Copyright (c) 2002-2005 Sam Leffler, Errno Consulting
 * Copyright (c) 2002-2005 Atheros Communications, Inc.
 * All rights reserved.
 *
 * $Id: //depot/sw/releases/linuxsrc/src/802_11/madwifi/hal/main/diag/radar.c#3 $
 */

#include "diag.h"

#include "ah.h"
#include "ah_devid.h"
#include "ah_internal.h"
#include "ar5212/ar5212.h"
#include "ar5212/ar5212reg.h"

#include <string.h>
#include <stdlib.h>
#include <err.h>

struct radarhandler {
	int	s;
	struct ath_diag atd;
	struct ifreq ifr;
};

static void
radarGetThresholds(struct radarhandler *radar, struct ar5212RadarState *rs)
{
	radar->atd.ad_id = HAL_DIAG_GET_RADAR | ATH_DIAG_DYN;
	radar->atd.ad_out_data = (caddr_t) rs;
	radar->atd.ad_out_size = sizeof(struct ar5212RadarState);
	if (ioctl(radar->s, SIOCGATHDIAG, &radar->atd) < 0)
		err(1, radar->atd.ad_name);
}

static int
radarGetNol(struct radarhandler *radar)
{
	u_int32_t result;

	radar->atd.ad_id = HAL_DIAG_GET_USENOL | ATH_DIAG_DYN;
	radar->atd.ad_in_data = NULL;
	radar->atd.ad_in_size = 0;
	radar->atd.ad_out_data = (caddr_t) &result;
	radar->atd.ad_out_size = sizeof(u_int32_t);
	if (ioctl(radar->s, SIOCGATHDIAG, &radar->atd) < 0)
		err(1, radar->atd.ad_name);
	return(result);
}

radarset(struct radarhandler *radar, int op, u_int32_t param)
{
	if (op != HAL_RADAR_PARAM_NOL) {
		u_int32_t args[2];

		args[0] = (u_int32_t) op;
		args[1] = param;

		radar->atd.ad_id = HAL_DIAG_SET_RADAR | ATH_DIAG_IN;
		radar->atd.ad_out_data = NULL;
		radar->atd.ad_out_size = 0;
		radar->atd.ad_in_data = (caddr_t) args;
		radar->atd.ad_in_size = sizeof(args);
	} else {
		u_int32_t args;

		args = param;
		radar->atd.ad_id = HAL_DIAG_USENOL | ATH_DIAG_IN;
		radar->atd.ad_out_data = NULL;
		radar->atd.ad_out_size = 0;
		radar->atd.ad_in_data = (caddr_t) &args;
		radar->atd.ad_in_size = sizeof(args);
	}
	if (ioctl(radar->s, SIOCGATHDIAG, &radar->atd) < 0)
		err(1, radar->atd.ad_name);
}

static void
usage(void)
{
	const char *msg = "\
Usage: radar [-i device] [cmd]\n\
firpwr X            set firpwr (thresh to check radar sig is gone) to X (int32)\n\
rrssi X             set radar rssi (start det) to X dB (u_int32)\n\
height X            set threshold for pulse height to X dB (u_int32)\n\
prssi               set threshold to checkif pulse is gone to X dB (u_int32)\n\
inband X            set threshold to check if pulse is inband to X (0.5 dB) (u_int32)\n\
nol on/off          turn on/off radar nol recording in HAL\n";
	fprintf(stderr, "%s", msg);
}

int
main(int argc, char *argv[])
{
#define	streq(a,b)	(strcasecmp(a,b) == 0)
	struct radarhandler radar;
	HAL_REVS revs;

	memset(&radar, 0, sizeof(radar));
	radar.s = socket(AF_INET, SOCK_DGRAM, 0);
	if (radar.s < 0)
		err(1, "socket");
	if (argc > 1 && strcmp(argv[1], "-i") == 0) {
		if (argc < 2) {
			fprintf(stderr, "%s: missing interface name for -i\n",
				argv[0]);
			exit(-1);
		}
		strncpy(radar.atd.ad_name, argv[2], sizeof (radar.atd.ad_name));
		argc -= 2, argv += 2;
	} else
		strncpy(radar.atd.ad_name, ATH_DEFAULT, sizeof (radar.atd.ad_name));
	strncpy(radar.ifr.ifr_name, radar.atd.ad_name, sizeof (radar.ifr.ifr_name));

	radar.atd.ad_id = HAL_DIAG_REVS;
	radar.atd.ad_out_data = (caddr_t) &revs;
	radar.atd.ad_out_size = sizeof(revs);
	if (ioctl(radar.s, SIOCGATHDIAG, &radar.atd) < 0)
		err(1, radar.atd.ad_name);
	switch (revs.ah_devid) {
	case AR5210_PROD:
	case AR5210_DEFAULT:
		printf("No radar detection yet for a 5210\n");
		exit(0);
	case AR5211_DEVID:
	case AR5311_DEVID:
	case AR5211_DEFAULT:
	case AR5211_FPGA11B:
		printf("No radar detecton yet for a 5211\n");
		exit(0);
	case AR5212_FPGA:
	case AR5212_DEVID:
	case AR5212_DEVID_IBM:
	case AR5212_DEFAULT:
	case AR5212_AR5312_REV2:
	case AR5212_AR5312_REV7:
		break;
	default:
		printf("No radar detection for device 0x%x\n", revs.ah_devid);
		exit(0);
	}
	if (argc > 2) {
		if(streq(argv[1], "firpwr")) {
			radarset(&radar, HAL_RADAR_PARAM_FIRPWR, (u_int32_t) atoi(argv[2]));
		} else if (streq(argv[1], "rrssi")) {
			radarset(&radar, HAL_RADAR_PARAM_RRSSI, strtoul(argv[2], NULL, 0));
		} else if (streq(argv[1], "height")) {
			radarset(&radar, HAL_RADAR_PARAM_HEIGHT, strtoul(argv[2], NULL, 0));
		} else if (streq(argv[1], "prssi")) {
			radarset(&radar, HAL_RADAR_PARAM_PRSSI, strtoul(argv[2], NULL, 0));
		} else if (streq(argv[1], "inband")) {
			radarset(&radar, HAL_RADAR_PARAM_INBAND, strtoul(argv[2], NULL, 0));
		} else if (streq(argv[1], "nol")) {
			if (streq(argv[2],"on"))
				radarset(&radar, HAL_RADAR_PARAM_NOL, 1);
			else
				radarset(&radar, HAL_RADAR_PARAM_NOL, 0);
		} else if (streq(argv[1],"-h")) {
			usage();
		}
	} else if (argc == 1) {
		struct ar5212RadarState rs;
		u_int32_t nol;
		radarGetThresholds(&radar, &rs);
		nol = radarGetNol(&radar);
		
		printf ("Radar;\nUse NOL: %s\n",nol ? "yes" : "no");
		printf ("Firpwr (thresh to see if radar sig is gone):  %d\n",rs.rs_firpwr);
		printf ("Radar Rssi (thresh to start radar det in dB): %u\n",rs.rs_radarRssi);
		printf ("Height (thresh for pulse height (dB):         %u\n",rs.rs_height);
		printf ("Pulse rssi (thresh if pulse is gone in dB):   %u\n",rs.rs_pulseRssi);
		printf ("Inband (thresh if pulse is inband (in 0.5dB): %u\n",rs.rs_inband);
	} else {
		usage ();
	}
	return 0;
}
