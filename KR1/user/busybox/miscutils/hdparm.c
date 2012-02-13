/* vi: set sw=4 ts=4: */
/*
 * hdparm implementation for busybox
 *
 *
 * Copyright (C) [2003] by [Matteo Croce] <pinnolo@tin.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA
 *
 *
 * This program is based on the source code of hdparm: see below...
 * hdparm.c - Command line interface to get/set hard disk parameters
 *          - by Mark Lord (C) 1994-2002 -- freely distributable
 */
 
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <endian.h>
#include <sys/ioctl.h>
#include <sys/shm.h>
#include <sys/sysmacros.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/mount.h>
#include "busybox.h"
#include <linux/types.h>
#include <linux/hdreg.h>
#include <linux/major.h>
#include <asm/byteorder.h>


#if __BYTE_ORDER == __BIG_ENDIAN
#define __USE_XOPEN
#endif

/* device types */
/* ------------ */
#define NO_DEV                  0xffff
#define ATA_DEV                 0x0000
#define ATAPI_DEV               0x0001

/* word definitions */
/* ---------------- */
#define GEN_CONFIG		0   /* general configuration */
#define LCYLS			1   /* number of logical cylinders */
#define CONFIG			2   /* specific configuration */
#define LHEADS			3   /* number of logical heads */
#define TRACK_BYTES		4   /* number of bytes/track (ATA-1) */
#define SECT_BYTES		5   /* number of bytes/sector (ATA-1) */
#define LSECTS			6   /* number of logical sectors/track */
#define START_SERIAL            10  /* ASCII serial number */
#define LENGTH_SERIAL           10  /* 10 words (20 bytes or characters) */
#define BUF_TYPE		20  /* buffer type (ATA-1) */
#define BUFFER__SIZE		21  /* buffer size (ATA-1) */
#define RW_LONG			22  /* extra bytes in R/W LONG cmd ( < ATA-4)*/
#define START_FW_REV            23  /* ASCII firmware revision */
#define LENGTH_FW_REV		 4  /*  4 words (8 bytes or characters) */
#define START_MODEL    		27  /* ASCII model number */
#define LENGTH_MODEL    	20  /* 20 words (40 bytes or characters) */
#define SECTOR_XFER_MAX	        47  /* r/w multiple: max sectors xfered */
#define DWORD_IO		48  /* can do double-word IO (ATA-1 only) */
#define CAPAB_0			49  /* capabilities */
#define CAPAB_1			50
#define PIO_MODE		51  /* max PIO mode supported (obsolete)*/
#define DMA_MODE		52  /* max Singleword DMA mode supported (obs)*/
#define WHATS_VALID		53  /* what fields are valid */
#define LCYLS_CUR		54  /* current logical cylinders */
#define LHEADS_CUR		55  /* current logical heads */
#define LSECTS_CUR	        56  /* current logical sectors/track */
#define CAPACITY_LSB		57  /* current capacity in sectors */
#define CAPACITY_MSB		58
#define SECTOR_XFER_CUR		59  /* r/w multiple: current sectors xfered */
#define LBA_SECTS_LSB		60  /* LBA: total number of user */
#define LBA_SECTS_MSB		61  /*      addressable sectors */
#define SINGLE_DMA		62  /* singleword DMA modes */
#define MULTI_DMA		63  /* multiword DMA modes */
#define ADV_PIO_MODES		64  /* advanced PIO modes supported */
				    /* multiword DMA xfer cycle time: */
#define DMA_TIME_MIN		65  /*   - minimum */
#define DMA_TIME_NORM		66  /*   - manufacturer's recommended	*/
				    /* minimum PIO xfer cycle time: */
#define PIO_NO_FLOW		67  /*   - without flow control */
#define PIO_FLOW		68  /*   - with IORDY flow control */
#define PKT_REL			71  /* typical #ns from PKT cmd to bus rel */
#define SVC_NBSY		72  /* typical #ns from SERVICE cmd to !BSY */
#define CDR_MAJOR		73  /* CD ROM: major version number */
#define CDR_MINOR		74  /* CD ROM: minor version number */
#define QUEUE_DEPTH		75  /* queue depth */
#define MAJOR			80  /* major version number */
#define MINOR			81  /* minor version number */
#define CMDS_SUPP_0		82  /* command/feature set(s) supported */
#define CMDS_SUPP_1		83
#define CMDS_SUPP_2		84
#define CMDS_EN_0		85  /* command/feature set(s) enabled */
#define CMDS_EN_1		86
#define CMDS_EN_2		87
#define ULTRA_DMA		88  /* ultra DMA modes */
				    /* time to complete security erase */
#define ERASE_TIME		89  /*   - ordinary */
#define ENH_ERASE_TIME		90  /*   - enhanced */
#define ADV_PWR			91  /* current advanced power management level
				       in low byte, 0x40 in high byte. */  
#define PSWD_CODE		92  /* master password revision code	*/
#define HWRST_RSLT		93  /* hardware reset result */
#define ACOUSTIC  		94  /* acoustic mgmt values ( >= ATA-6) */
#define LBA_LSB			100 /* LBA: maximum.  Currently only 48 */
#define LBA_MID			101 /*      bits are used, but addr 103 */
#define LBA_48_MSB		102 /*      has been reserved for LBA in */
#define LBA_64_MSB		103 /*      the future. */
#define RM_STAT 		127 /* removable media status notification feature set support */
#define SECU_STATUS		128 /* security status */
#define CFA_PWR_MODE		160 /* CFA power mode 1 */
#define START_MEDIA             176 /* media serial number */
#define LENGTH_MEDIA            20  /* 20 words (40 bytes or characters)*/
#define START_MANUF             196 /* media manufacturer I.D. */
#define LENGTH_MANUF            10  /* 10 words (20 bytes or characters) */
#define INTEGRITY		255 /* integrity word */

/* bit definitions within the words */
/* -------------------------------- */

/* many words are considered valid if bit 15 is 0 and bit 14 is 1 */
#define VALID			0xc000
#define VALID_VAL		0x4000
/* many words are considered invalid if they are either all-0 or all-1 */
#define NOVAL_0			0x0000
#define NOVAL_1			0xffff

/* word 0: gen_config */
#define NOT_ATA			0x8000	
#define NOT_ATAPI		0x4000	/* (check only if bit 15 == 1) */
#define MEDIA_REMOVABLE		0x0080
#define DRIVE_NOT_REMOVABLE	0x0040  /* bit obsoleted in ATA 6 */
#define INCOMPLETE		0x0004
#define CFA_SUPPORT_VAL		0x848a	/* 848a=CFA feature set support */
#define DRQ_RESPONSE_TIME	0x0060
#define DRQ_3MS_VAL		0x0000
#define DRQ_INTR_VAL		0x0020
#define DRQ_50US_VAL		0x0040
#define PKT_SIZE_SUPPORTED	0x0003
#define PKT_SIZE_12_VAL		0x0000
#define PKT_SIZE_16_VAL		0x0001
#define EQPT_TYPE		0x1f00
#define SHIFT_EQPT		8

#define CDROM 0x0005

#ifdef CONFIG_FEATURE_HDPARM_GET_IDENTITY
static const char *pkt_str[] = {
	"Direct-access device",			/* word 0, bits 12-8 = 00 */
	"Sequential-access device",		/* word 0, bits 12-8 = 01 */
	"Printer",				/* word 0, bits 12-8 = 02 */
	"Processor",				/* word 0, bits 12-8 = 03 */
	"Write-once device",			/* word 0, bits 12-8 = 04 */
	"CD-ROM",				/* word 0, bits 12-8 = 05 */
	"Scanner",				/* word 0, bits 12-8 = 06 */
	"Optical memory",			/* word 0, bits 12-8 = 07 */
	"Medium changer",			/* word 0, bits 12-8 = 08 */
	"Communications device",		/* word 0, bits 12-8 = 09 */
	"ACS-IT8 device",			/* word 0, bits 12-8 = 0a */
	"ACS-IT8 device",			/* word 0, bits 12-8 = 0b */
	"Array controller",			/* word 0, bits 12-8 = 0c */
	"Enclosure services",			/* word 0, bits 12-8 = 0d */
	"Reduced block command device",		/* word 0, bits 12-8 = 0e */
	"Optical card reader/writer",		/* word 0, bits 12-8 = 0f */
	"",					/* word 0, bits 12-8 = 10 */
	"",					/* word 0, bits 12-8 = 11 */
	"",					/* word 0, bits 12-8 = 12 */
	"",					/* word 0, bits 12-8 = 13 */
	"",					/* word 0, bits 12-8 = 14 */
	"",					/* word 0, bits 12-8 = 15 */
	"",					/* word 0, bits 12-8 = 16 */
	"",					/* word 0, bits 12-8 = 17 */
	"",					/* word 0, bits 12-8 = 18 */
	"",					/* word 0, bits 12-8 = 19 */
	"",					/* word 0, bits 12-8 = 1a */
	"",					/* word 0, bits 12-8 = 1b */
	"",					/* word 0, bits 12-8 = 1c */
	"",					/* word 0, bits 12-8 = 1d */
	"",					/* word 0, bits 12-8 = 1e */
	"Unknown",				/* word 0, bits 12-8 = 1f */
};
static const char *ata1_cfg_str[] = {			/* word 0 in ATA-1 mode */
	"reserved",				/* bit 0 */
	"hard sectored",			/* bit 1 */
	"soft sectored",			/* bit 2 */
	"not MFM encoded ",			/* bit 3 */
	"head switch time > 15us",		/* bit 4 */
	"spindle motor control option",		/* bit 5 */
	"fixed drive",				/* bit 6 */
	"removable drive",			/* bit 7 */
	"disk xfer rate <= 5Mbs",		/* bit 8 */
	"disk xfer rate > 5Mbs, <= 10Mbs",	/* bit 9 */
	"disk xfer rate > 5Mbs",		/* bit 10 */
	"rotational speed tol.",		/* bit 11 */
	"data strobe offset option",		/* bit 12 */
	"track offset option",			/* bit 13 */
	"format speed tolerance gap reqd",	/* bit 14 */
	"ATAPI"					/* bit 14 */
};
#endif

/* word 1: number of logical cylinders */
#define LCYLS_MAX		0x3fff /* maximum allowable value */

/* word 2: specific configureation 
 * (a) require SET FEATURES to spin-up
 * (b) require spin-up to fully reply to IDENTIFY DEVICE
 */
#define STBY_NID_VAL		0x37c8  /*     (a) and     (b) */
#define STBY_ID_VAL		0x738c	/*     (a) and not (b) */
#define PWRD_NID_VAL 		0x8c73	/* not (a) and     (b) */
#define PWRD_ID_VAL		0xc837	/* not (a) and not (b) */

/* words 47 & 59: sector_xfer_max & sector_xfer_cur */
#define SECTOR_XFER		0x00ff  /* sectors xfered on r/w multiple cmds*/
#define MULTIPLE_SETTING_VALID  0x0100  /* 1=multiple sector setting is valid */

/* word 49: capabilities 0 */
#define STD_STBY  	  	0x2000  /* 1=standard values supported (ATA);
					   0=vendor specific values */
#define IORDY_SUP		0x0800  /* 1=support; 0=may be supported */
#define IORDY_OFF		0x0400  /* 1=may be disabled */
#define LBA_SUP			0x0200  /* 1=Logical Block Address support */
#define DMA_SUP			0x0100  /* 1=Direct Memory Access support */
#define DMA_IL_SUP		0x8000  /* 1=interleaved DMA support (ATAPI) */
#define CMD_Q_SUP		0x4000  /* 1=command queuing support (ATAPI) */
#define OVLP_SUP		0x2000  /* 1=overlap operation support (ATAPI) */
#define SWRST_REQ		0x1000  /* 1=ATA SW reset required (ATAPI, obsolete */

/* word 50: capabilities 1 */
#define MIN_STANDBY_TIMER	0x0001  /* 1=device specific standby timer value minimum */

/* words 51 & 52: PIO & DMA cycle times */
#define MODE			0xff00  /* the mode is in the MSBs */

/* word 53: whats_valid */
#define OK_W88     	   	0x0004	/* the ultra_dma info is valid */
#define OK_W64_70		0x0002  /* see above for word descriptions */
#define OK_W54_58		0x0001  /* current cyl, head, sector, cap. info valid */

/*word 63,88: dma_mode, ultra_dma_mode*/
#define MODE_MAX		7	/* bit definitions force udma <=7 (when
					 * udma >=8 comes out it'll have to be
					 * defined in a new dma_mode word!) */

/* word 64: PIO transfer modes */
#define PIO_SUP			0x00ff  /* only bits 0 & 1 are used so far,  */
#define PIO_MODE_MAX		8       /* but all 8 bits are defined        */

/* word 75: queue_depth */
#define DEPTH_BITS		0x001f  /* bits used for queue depth */

/* words 80-81: version numbers */
/* NOVAL_0 or  NOVAL_1 means device does not report version */

/* word 81: minor version number */
#define MINOR_MAX		0x1C
#ifdef CONFIG_FEATURE_HDPARM_GET_IDENTITY
static const char *minor_str[] = {				/* word 81 value: */
	"device does not report version",		/* 0x0000	*/
	"ATA-1 X3T9.2 781D prior to revision 4",	/* 0x0001	*/
	"ATA-1 published, ANSI X3.221-1994",		/* 0x0002	*/
	"ATA-1 X3T9.2 781D revision 4",			/* 0x0003	*/
	"ATA-2 published, ANSI X3.279-1996",		/* 0x0004	*/
	"ATA-2 X3T10 948D prior to revision 2k",	/* 0x0005	*/
	"ATA-3 X3T10 2008D revision 1",			/* 0x0006	*/
	"ATA-2 X3T10 948D revision 2k",			/* 0x0007	*/
	"ATA-3 X3T10 2008D revision 0",			/* 0x0008	*/
	"ATA-2 X3T10 948D revision 3",			/* 0x0009	*/
	"ATA-3 published, ANSI X3.298-199x",		/* 0x000a	*/
	"ATA-3 X3T10 2008D revision 6",			/* 0x000b	*/
	"ATA-3 X3T13 2008D revision 7 and 7a",		/* 0x000c	*/
	"ATA/ATAPI-4 X3T13 1153D revision 6",		/* 0x000d	*/
	"ATA/ATAPI-4 T13 1153D revision 13",		/* 0x000e	*/
	"ATA/ATAPI-4 X3T13 1153D revision 7",		/* 0x000f	*/
	"ATA/ATAPI-4 T13 1153D revision 18",		/* 0x0010	*/
	"ATA/ATAPI-4 T13 1153D revision 15",		/* 0x0011	*/
	"ATA/ATAPI-4 published, ANSI NCITS 317-1998",	/* 0x0012	*/
	"ATA/ATAPI-5 T13 1321D revision 3",
	"ATA/ATAPI-4 T13 1153D revision 14",		/* 0x0014	*/
	"ATA/ATAPI-5 T13 1321D revision 1",		/* 0x0015	*/
	"ATA/ATAPI-5 published, ANSI NCITS 340-2000",	/* 0x0016	*/
	"ATA/ATAPI-4 T13 1153D revision 17",		/* 0x0017	*/
	"ATA/ATAPI-6 T13 1410D revision 0",		/* 0x0018	*/
	"ATA/ATAPI-6 T13 1410D revision 3a",		/* 0x0019	*/
	"Reserved",					/* 0x001a	*/
	"ATA/ATAPI-6 T13 1410D revision 2",		/* 0x001b	*/
	"ATA/ATAPI-6 T13 1410D revision 1",		/* 0x001c	*/
	"reserved"					/* 0x001d	*/
	"reserved"					/* 0x001e	*/
	"reserved"					/* 0x001f-0xfffe*/
};
#endif
static const char actual_ver[] = { 
			/* word 81 value: */
	0,		/* 0x0000	WARNING: 	*/
	1,		/* 0x0001	WARNING: 	*/
	1,		/* 0x0002	WARNING: 	*/
	1,		/* 0x0003	WARNING: 	*/
	2,		/* 0x0004	WARNING:   This array 		*/
	2,		/* 0x0005	WARNING:   corresponds 		*/
	3,		/* 0x0006	WARNING:   *exactly*		*/
	2,		/* 0x0007	WARNING:   to the ATA/		*/
	3,		/* 0x0008	WARNING:   ATAPI version	*/
	2,		/* 0x0009	WARNING:   listed in	 	*/
	3,		/* 0x000a	WARNING:   the 		 	*/
	3,		/* 0x000b	WARNING:   minor_str 		*/
	3,		/* 0x000c	WARNING:   array		*/
	4,		/* 0x000d	WARNING:   above.		*/
	4,		/* 0x000e	WARNING:  			*/
	4,		/* 0x000f	WARNING:   if you change 	*/
	4,		/* 0x0010	WARNING:   that one,      	*/
	4,		/* 0x0011	WARNING:   change this one	*/
	4,		/* 0x0012	WARNING:   too!!!        	*/
	5,		/* 0x0013	WARNING:	*/
	4,		/* 0x0014	WARNING:	*/
	5,		/* 0x0015	WARNING:	*/
	5,		/* 0x0016	WARNING:	*/
	4,		/* 0x0017	WARNING:	*/
	6,		/* 0x0018	WARNING:	*/
	6,		/* 0x0019	WARNING:	*/
	0,		/* 0x001a	WARNING:	*/
	6,		/* 0x001b	WARNING:	*/
	6,		/* 0x001c	WARNING:	*/
	0		/* 0x001d-0xfffe    		*/
};

/* words 82-84: cmds/feats supported */
#define CMDS_W82		0x77ff  /* word 82: defined command locations*/
#define CMDS_W83		0x3fff  /* word 83: defined command locations*/
#define CMDS_W84		0x002f  /* word 83: defined command locations*/
#define SUPPORT_48_BIT		0x0400  
#define NUM_CMD_FEAT_STR	48

#ifdef CONFIG_FEATURE_HDPARM_GET_IDENTITY
static const char *cmd_feat_str[] = {
	"",					/* word 82 bit 15: obsolete  */
	"NOP cmd",				/* word 82 bit 14 */
	"READ BUFFER cmd",			/* word 82 bit 13 */
	"WRITE BUFFER cmd",			/* word 82 bit 12 */
	"",					/* word 82 bit 11: obsolete  */
	"Host Protected Area feature set",	/* word 82 bit 10 */
	"DEVICE RESET cmd",			/* word 82 bit  9 */
	"SERVICE interrupt",			/* word 82 bit  8 */
	"Release interrupt",			/* word 82 bit  7 */
	"Look-ahead",				/* word 82 bit  6 */
	"Write cache",				/* word 82 bit  5 */
	"PACKET command feature set",		/* word 82 bit  4 */
	"Power Management feature set",		/* word 82 bit  3 */
	"Removable Media feature set",		/* word 82 bit  2 */
	"Security Mode feature set",		/* word 82 bit  1 */
	"SMART feature set",			/* word 82 bit  0 */
						/* --------------*/
	"",					/* word 83 bit 15: !valid bit */
	"",					/* word 83 bit 14:  valid bit */
	"FLUSH CACHE EXT command",		/* word 83 bit 13 */
	"Mandatory FLUSH CACHE command ",	/* word 83 bit 12 */
	"Device Configuration Overlay feature set ",
	"48-bit Address feature set ",		/* word 83 bit 10 */
	"",
	"SET MAX security extension",		/* word 83 bit  8 */
	"Address Offset Reserved Area Boot",	/* word 83 bit  7 */
	"SET FEATURES subcommand required to spinup after power up",
	"Power-Up In Standby feature set",	/* word 83 bit  5 */
	"Removable Media Status Notification feature set",
	"Advanced Power Management feature set",/* word 83 bit  3 */
	"CFA feature set",			/* word 83 bit  2 */
	"READ/WRITE DMA QUEUED",		/* word 83 bit  1 */
	"DOWNLOAD MICROCODE cmd", 		/* word 83 bit  0 */
						/* --------------*/
	"",					/* word 84 bit 15: !valid bit */
	"",					/* word 84 bit 14:  valid bit */
	"",					/* word 84 bit 13:  reserved */
	"",					/* word 84 bit 12:  reserved */
	"",					/* word 84 bit 11:  reserved */
	"",					/* word 84 bit 10:  reserved */
	"",					/* word 84 bit  9:  reserved */
	"",					/* word 84 bit  8:  reserved */
	"",					/* word 84 bit  7:  reserved */
	"",					/* word 84 bit  6:  reserved */
	"General Purpose Logging feature set",	/* word 84 bit  5 */
	"",					/* word 84 bit  4:  reserved */
	"Media Card Pass Through Command feature set ",
	"Media serial number ",			/* word 84 bit  2 */
	"SMART self-test ",			/* word 84 bit  1 */
	"SMART error logging "			/* word 84 bit  0 */
};
#endif


/* words 85-87: cmds/feats enabled */
/* use cmd_feat_str[] to display what commands and features have
 * been enabled with words 85-87 
 */

/* words 89, 90, SECU ERASE TIME */
#define ERASE_BITS		0x00ff

/* word 92: master password revision */
/* NOVAL_0 or  NOVAL_1 means no support for master password revision */

/* word 93: hw reset result */
#define CBLID			0x2000  /* CBLID status */
#define RST0			0x0001  /* 1=reset to device #0 */
#define DEV_DET			0x0006  /* how device num determined */
#define JUMPER_VAL		0x0002  /* device num determined by jumper */
#define CSEL_VAL		0x0004  /* device num determined by CSEL_VAL */

/* word 127: removable media status notification feature set support */
#define RM_STAT_BITS 		0x0003
#define RM_STAT_SUP		0x0001
	
/* word 128: security */
#define SECU_ENABLED	0x0002
#define SECU_LEVEL		0x0010
#define NUM_SECU_STR	6
#ifdef CONFIG_FEATURE_HDPARM_GET_IDENTITY
static const char *secu_str[] = {
	"supported",			/* word 128, bit 0 */
	"enabled",			/* word 128, bit 1 */
	"locked",			/* word 128, bit 2 */
	"frozen",			/* word 128, bit 3 */
	"expired: security count",	/* word 128, bit 4 */
	"supported: enhanced erase"	/* word 128, bit 5 */
};
#endif

/* word 160: CFA power mode */
#define VALID_W160		0x8000  /* 1=word valid */
#define PWR_MODE_REQ		0x2000  /* 1=CFA power mode req'd by some cmds*/
#define PWR_MODE_OFF		0x1000  /* 1=CFA power moded disabled */
#define MAX_AMPS		0x0fff  /* value = max current in ma */

/* word 255: integrity */
#define SIG			0x00ff  /* signature location */
#define SIG_VAL			0x00A5  /* signature value */

#ifdef CONFIG_FEATURE_HDPARM_GET_IDENTITY
static uint8_t mode_loop(uint16_t mode_sup, uint16_t mode_sel, int cc, uint8_t *have_mode) {
	uint16_t ii;
	uint8_t err_dma = 0;
	for(ii = 0; ii <= MODE_MAX; ii++) {
		if(mode_sel & 0x0001) {
			printf("*%cdma%u ",cc,ii);
			if(*have_mode) err_dma = 1;
			*have_mode = 1;
		} else if(mode_sup & 0x0001) {
			printf("%cdma%u ",cc,ii);
		}
		mode_sup >>=1;   mode_sel >>=1;
	}
	return err_dma;
}
static void print_ascii(uint16_t *p, uint8_t length) {
	uint8_t ii;
	char cl;
	
	/* find first non-space & print it */
	for(ii = 0; ii< length; ii++) {
		if(((char) 0x00ff&((*p)>>8)) != ' ') break;
		if((cl = (char) 0x00ff&(*p)) != ' ') {
			if(cl != '\0') printf("%c",cl);
			p++; ii++;
			break;
		}
		p++;
	}
	/* print the rest */
	for(; ii< length; ii++) {
		if(!(*p)) break; /* some older devices have NULLs */
		printf("%c%c",(char)0x00ff&((*p)>>8),(char)(*p)&0x00ff);
		p++;
	}
	printf("\n");
}

/* identify() is the only extern function used across two source files.  The
   others, though, were declared in hdparm.c with global scope; since other
   functions in that file have static (file) scope, I assume the difference is
   intentional. */
static void identify (uint16_t *id_supplied, const char *devname)
{

	char *id_file = NULL, fmt[]="/proc/ide/%s/identify";
	FILE *fl;
	uint16_t val[256], ii, jj, kk;
	uint16_t like_std = 1, std = 0, min_std = 0xffff;
	uint16_t dev = NO_DEV, eqpt = NO_DEV;
	uint8_t  have_mode = 0, err_dma = 0;
	uint8_t  chksum = 0;
	uint32_t ll, mm, nn, oo;
	__u64 bbbig; /* (:) */

	if (id_supplied) {
#if __BYTE_ORDER == __BIG_ENDIAN
		swab(id_supplied, val, sizeof(val));
#else
		memcpy(val, id_supplied, sizeof(val));
#endif
	} else {
		id_file = xcalloc(1, strlen(devname)+1+strlen(fmt));
		sprintf(id_file, fmt, devname);

		/* open the file, read in all the info and close it */
		if (id_file == NULL) {
			fl = stdin;
		} else if(NULL == (fl = fopen(id_file, "r")))
     			bb_perror_msg_and_die(id_file);
		/* calculate checksum over all bytes */
		for(ii = GEN_CONFIG; ii<=INTEGRITY; ii++) {
			unsigned int scratch;
			if(1 != fscanf(fl,"%04x",&scratch)) break;
			val[ii] = (uint16_t)scratch;
			chksum += val[ii] + (val[ii] >> 8);
		}
		fclose(fl);  
		if(ii < (INTEGRITY+1))
			bb_error_msg_and_die("Input file wrong format or length");
	}
	chksum &= 0xff;

	/* check if we recognise the device type */
	printf("\n");
	if(!(val[GEN_CONFIG] & NOT_ATA)) {
		dev = ATA_DEV;
		printf("ATA device, with ");
	} else if(val[GEN_CONFIG]==CFA_SUPPORT_VAL) {
		dev = ATA_DEV;
		like_std = 4;
		printf("CompactFlash ATA device, with ");
	} else if(!(val[GEN_CONFIG] & NOT_ATAPI)) {
		dev = ATAPI_DEV;
		eqpt = (val[GEN_CONFIG] & EQPT_TYPE) >> SHIFT_EQPT;
		printf("ATAPI %s, with ", pkt_str[eqpt]);
		like_std = 3;
	} else {
		bb_error_msg_and_die("Unknown device type:\n\tbits 15&14 of general configuration word 0 both set to 1.\n");
	}
	if(!(val[GEN_CONFIG] & MEDIA_REMOVABLE))
		printf("non-");
	printf("removable media\n");


	/* Info from the specific configuration word says whether or not the
	 * ID command completed correctly.  It is only defined, however in
	 * ATA/ATAPI-5 & 6; it is reserved (value theoretically 0) in prior 
	 * standards.  Since the values allowed for this word are extremely
	 * specific, it should be safe to check it now, even though we don't
	 * know yet what standard this device is using.
	 */
	if((val[CONFIG]==STBY_NID_VAL) || (val[CONFIG]==STBY_ID_VAL) ||
	   (val[CONFIG]==PWRD_NID_VAL) || (val[CONFIG]==PWRD_ID_VAL) ) {
	   	like_std = 5;
		if((val[CONFIG]==STBY_NID_VAL) || (val[CONFIG]==STBY_ID_VAL))
			printf("powers-up in standby; SET FEATURES subcmd spins-up.\n");
		if(((val[CONFIG]==STBY_NID_VAL) || (val[CONFIG]==PWRD_NID_VAL)) &&
		   (val[GEN_CONFIG] & INCOMPLETE)) 
			printf("\n\tWARNING: ID response incomplete.\n\tWARNING: Following data may be incorrect.\n\n");
	}

	/* output the model and serial numbers and the fw revision */
	if(val[START_MODEL]) {
		printf("\t%-20s","Model Number:");
		print_ascii(&val[START_MODEL], LENGTH_MODEL);
	}
	if(val[START_SERIAL]) {
		printf("\t%-20s","Serial Number:");
		print_ascii( &val[START_SERIAL], LENGTH_SERIAL);
	}
	if(val[START_FW_REV]) {
		printf("\t%-20s","Firmware Revision:");
		print_ascii(&val[START_FW_REV], LENGTH_FW_REV);
	}
	if(val[START_MEDIA]) {
		printf("\t%-20s","Media Serial Num:");
		print_ascii(&val[START_MEDIA], LENGTH_MEDIA);
	}
	if(val[START_MANUF]) {
		printf("\t%-20s","Media Manufacturer:");
		print_ascii(&val[START_MANUF], LENGTH_MANUF);
	}

	/* major & minor standards version number (Note: these words were not
	 * defined until ATA-3 & the CDROM std uses different words.) */
	printf("Standards:");
	if(eqpt != CDROM) {
		if(val[MINOR] && (val[MINOR] <= MINOR_MAX)){
			if(like_std < 3) like_std = 3;
			std = actual_ver[val[MINOR]];
			if(std) {
				printf("\n\tUsed: ");
				printf("%s ",minor_str[val[MINOR]]);
			}
		}
		/* looks like when they up-issue the std, they obsolete one;
		 * thus, only the newest 4 issues need be supported. (That's 
		 * what "kk" and "min_std" are all about.) */
		if(val[MAJOR] && (val[MAJOR] !=NOVAL_1)) {
			printf("\n\tSupported: ");
			jj = val[MAJOR] << 1;
			kk = like_std >4 ? like_std-4: 0;
			for(ii = 14; (ii >0)&&(ii>kk); ii--) {
				if(jj & 0x8000) {
					printf("%u ", ii);
					if(like_std < ii) {
						like_std = ii;
						kk = like_std >4 ? like_std-4: 0;
					}
					if(min_std > ii) min_std = ii;
				}
				jj <<= 1;
			}
			if(like_std < 3) like_std = 3;
		}
		/* Figure out what standard the device is using if it hasn't told
		 * us.  If we know the std, check if the device is using any of
		 * the words from the next level up.  It happens.
		 */
		if(like_std < std) like_std = std;
		if(((std == 5) || (!std && (like_std < 6))) &&
		   ( (((val[CMDS_SUPP_1] & VALID) == VALID_VAL) &&
		     ((val[CMDS_SUPP_1] & CMDS_W83) > 0x00ff)) ||
		    (((val[CMDS_SUPP_2] & VALID) == VALID_VAL) &&
		     (val[CMDS_SUPP_2] & CMDS_W84) ) ) ) {
			like_std = 6;
		}  else	if(((std == 4) || (!std && (like_std < 5))) &&
		   ((((val[INTEGRITY] & SIG) == SIG_VAL) && !chksum) ||
		    ((val[HWRST_RSLT] & VALID) == VALID_VAL) ||
		    (((val[CMDS_SUPP_1] & VALID) == VALID_VAL) &&
		     ((val[CMDS_SUPP_1] & CMDS_W83) > 0x001f)) ) ) {
			like_std = 5;
		}  else if(((std == 3) || (!std && (like_std < 4))) &&
			   ((((val[CMDS_SUPP_1] & VALID) == VALID_VAL) &&
			     (((val[CMDS_SUPP_1] & CMDS_W83) > 0x0000) ||  
			      ((val[CMDS_SUPP_0] & CMDS_W82) > 0x000f))) ||
			    ((val[CAPAB_1] & VALID) == VALID_VAL) ||
			    ((val[WHATS_VALID] & OK_W88) && val[ULTRA_DMA]) ||
			    ((val[RM_STAT] & RM_STAT_BITS) == RM_STAT_SUP) ) ) {
			like_std = 4;
		}  else if(((std == 2) || (!std && (like_std < 3))) &&
			   ((val[CMDS_SUPP_1] & VALID) == VALID_VAL) ) {
			like_std = 3;
		}  else if(((std == 1) || (!std && (like_std < 2))) &&
		   	   ((val[CAPAB_0] & (IORDY_SUP | IORDY_OFF)) ||
		   	    (val[WHATS_VALID] & OK_W64_70)) ) {
			like_std = 2;
		}
		if(!std) {
			printf("\n\tLikely used: %u\n",like_std);
		} else if(like_std > std) {
			printf("& some of %u\n",like_std);
		} else  printf("\n");
	} else {
		/* TBD: do CDROM stuff more thoroughly.  For now... */
	  	kk = 0;
		if(val[CDR_MINOR] == 9) {
			kk = 1;
			printf("\n\tUsed: ATAPI for CD-ROMs, SFF-8020i, r2.5");
		}
		if(val[CDR_MAJOR] && (val[CDR_MAJOR] !=NOVAL_1)) {
			kk = 1;
			printf("\n\tSupported: CD-ROM ATAPI");
			jj = val[CDR_MAJOR] >> 1;
			for(ii = 1; ii <15; ii++) {
				if(jj & 0x0001) {
					printf("-%u ", ii);
				}
				jj >>= 1;
			}
		}
		if(!kk) printf("\n\tLikely used CD-ROM ATAPI-1\n");
		else	printf("\n");
		/* the cdrom stuff is more like ATA-2 than anything else, so: */
		like_std = 2;
	}

	if(min_std == 0xffff) min_std = like_std > 4 ? like_std - 3 : 1;

	printf("Configuration:\n");
	/* more info from the general configuration word */
	if((eqpt != CDROM) && (like_std == 1)) {
		jj = val[GEN_CONFIG] >> 1;
		for(ii = 1; ii < 15; ii++) {
			if(jj & 0x0001) printf("\t%s\n",ata1_cfg_str[ii]);
			jj >>=1;
		}
	}
	if(dev == ATAPI_DEV) {
		printf("\tDRQ response: "); /* Data Request (DRQ) */
		switch(val[GEN_CONFIG] & DRQ_RESPONSE_TIME) {
		case DRQ_3MS_VAL : printf("3ms.\n"); break;
		case DRQ_INTR_VAL : printf("<=10ms with INTRQ\n"); break;
		case DRQ_50US_VAL : printf("50us.\n"); break;
		default : printf("unknown.\n"); break;
		}
		printf("\tPacket size: ");
		switch(val[GEN_CONFIG] & PKT_SIZE_SUPPORTED) {
		case PKT_SIZE_12_VAL : printf("12 bytes\n"); break;
		case PKT_SIZE_16_VAL : printf("16 bytes\n"); break;
		default : printf("Unknown\n"); break;
		}
	} else {
	  	/* addressing...CHS? See section 6.2 of ATA specs 4 or 5 */
		ll = (uint32_t)val[LBA_SECTS_MSB] << 16 | val[LBA_SECTS_LSB];
		mm = 0; bbbig = 0;
	  	if ( (ll > 0x00FBFC10) && (!val[LCYLS])) {
		  	printf("\tCHS addressing not supported\n");
		} else {
			jj = val[WHATS_VALID] & OK_W54_58;
			printf("\tLogical\t\tmax\tcurrent\n");
			printf("\tcylinders\t%u\t%u\n",val[LCYLS],jj?val[LCYLS_CUR]:0);
			printf("\theads\t\t%u\t%u\n",val[LHEADS],jj?val[LHEADS_CUR]:0);
			printf("\tsectors/track\t%u\t%u\n",val[LSECTS],jj?val[LSECTS_CUR]:0);
			printf("\t--\n");
			if((min_std == 1) && (val[TRACK_BYTES] || val[SECT_BYTES])) {
				printf("\tbytes/track: %u",val[TRACK_BYTES]);
				printf("\tbytes/sector: %u\n",val[SECT_BYTES]);
			}
			if(jj) {
				mm = (uint32_t)val[CAPACITY_MSB] << 16 | val[CAPACITY_LSB];
				if(like_std < 3) { /* check Endian of capacity bytes */
					nn = val[LCYLS_CUR] * val[LHEADS_CUR] * val[LSECTS_CUR];
					oo = (uint32_t)val[CAPACITY_LSB] << 16 | val[CAPACITY_MSB];
					if(abs(mm - nn) > abs(oo - nn))
						mm = oo;
				}
				printf("\tCHS current addressable sectors:%11u\n",mm);
			} 
		}
		/* LBA addressing */
		printf("\tLBA    user addressable sectors:%11u\n",ll);
		if( ((val[CMDS_SUPP_1] & VALID) == VALID_VAL) &&
		     (val[CMDS_SUPP_1] & SUPPORT_48_BIT) ) {
			bbbig = (__u64)val[LBA_64_MSB] << 48 | 
			        (__u64)val[LBA_48_MSB] << 32 |
			        (__u64)val[LBA_MID] << 16 | 
				val[LBA_LSB] ;
			printf("\tLBA48  user addressable sectors:%11llu\n",bbbig);
		}
		if (!bbbig) bbbig = (__u64)(ll>mm ? ll : mm); /* # 512 byte blocks */
		printf("\tdevice size with M = 1024*1024: %11llu MBytes\n",bbbig>>11);
		bbbig = (bbbig<<9)/1000000;
		printf("\tdevice size with M = 1000*1000: %11llu MBytes ",bbbig);
		if(bbbig > 1000) printf("(%llu GB)\n",bbbig/1000);
		else printf("\n");

	}

	/* hw support of commands (capabilities) */
	printf("Capabilities:\n");
	printf("\t");
	if(dev == ATAPI_DEV) {
		if(eqpt != CDROM) {
			if(val[CAPAB_0] & CMD_Q_SUP) printf("Cmd queuing, ");
		}
		if(val[CAPAB_0] & OVLP_SUP) printf("Cmd overlap, ");
	}
	if(val[CAPAB_0] & LBA_SUP) printf("LBA, ");
	if(like_std != 1) {
		printf("IORDY");
		if(!(val[CAPAB_0] & IORDY_SUP)) printf("(may be)");
		if(val[CAPAB_0] & IORDY_OFF) printf("(can");
		else			     printf("(cannot");
		printf(" be disabled)");
	} else printf("no IORDY"); 
	printf("\n");
	if((like_std == 1) && val[BUF_TYPE]) {
		kk = val[BUF_TYPE];
		printf("\tBuffer type: %04x: ",kk);
		if (kk < 2) 	printf("single port, single-sector");
		else		printf("dual port, multi-sector");
		if (kk > 2)	printf(" with read caching ability");
		printf("\n");
	}
	jj = 0;
	if((min_std == 1) && (val[BUFFER__SIZE] && (val[BUFFER__SIZE] != NOVAL_1))) {
		printf("\tBuffer size: %.1fkB",(float)val[BUFFER__SIZE]/2);
		jj = 1;
	}
	if((min_std < 4) && (val[RW_LONG])) {
		printf("\tbytes avail on r/w long: %u",val[RW_LONG]);
		jj = 1;
	}
	if((eqpt != CDROM) && (like_std > 3)) {
		printf("\tQueue depth: %u",(val[QUEUE_DEPTH] & DEPTH_BITS)+1);
		jj = 1;
	}
	if(jj) printf("\n");
	if(dev == ATA_DEV) {
		if(like_std == 1) {
			printf("\tCan");
			if(!val[DWORD_IO]) printf("not");
			printf(" perform double-word IO\n");
		} else {
			printf("\tStandby timer values: spec'd by ");
			if(val[CAPAB_0] & STD_STBY) printf("Standard");
			else 			    printf("Vendor");
			if((like_std > 3) && ((val[CAPAB_1] & VALID) == VALID_VAL)) {
			   	if(val[CAPAB_1] & MIN_STANDBY_TIMER) printf(", with ");
				else 				     printf(", no ");
				printf("device specific minimum\n");
			} else  printf("\n");
		}
		printf("\tR/W multiple sector transfer: ");
		if((like_std < 3) && !(val[SECTOR_XFER_MAX] & SECTOR_XFER)) {
			printf("not supported\n");
		} else {
			printf("Max = %u\t",val[SECTOR_XFER_MAX] & SECTOR_XFER);
			printf("Current = ");
			if(val[SECTOR_XFER_CUR] & MULTIPLE_SETTING_VALID)
				printf("%u\n",val[SECTOR_XFER_CUR] & SECTOR_XFER);
			else	printf("?\n");
		}
		if((like_std > 3) && (val[CMDS_SUPP_1] & 0x0008)) {
			/* We print out elsewhere whether the APM feature is enabled or
			   not.  If it's not enabled, let's not repeat the info; just print
			   nothing here. */
			printf("\tAdvanced power management level: ");
			if ( (val[ADV_PWR] & 0xFF00) == 0x4000 ) {
				uint8_t apm_level = val[ADV_PWR] & 0x00FF;

				printf("%u (0x%x)\n", apm_level, apm_level);
			} else {
				printf("unknown setting (0x%04x)\n", val[ADV_PWR]);
			}
		}
		if(like_std > 5) {
			if(val[ACOUSTIC]) {
				printf("\tRecommended acoustic management value: %u, current value: %u\n", (val[ACOUSTIC] >> 8) & 0x00ff, val[ACOUSTIC] & 0x00ff);
			}
		}
	} else { /* ATAPI */
		if(eqpt != CDROM) {
			if(val[CAPAB_0] & SWRST_REQ) printf("\tATA sw reset required\n");
		}
		if(val[PKT_REL] || val[SVC_NBSY]) {
			printf("\tOverlap support:");
			if(val[PKT_REL]) printf(" %uus to release bus.",val[PKT_REL]);
			if(val[SVC_NBSY]) printf(" %uus to clear BSY after SERVICE cmd.",val[SVC_NBSY]);
			printf("\n");
		}
	}

	/* DMA stuff. Check that only one DMA mode is selected. */
	printf("\tDMA: ");
	if(!(val[CAPAB_0] & DMA_SUP)) {
		printf("not supported\n");
	} else {
		if(val[DMA_MODE] && !val[SINGLE_DMA] && !val[MULTI_DMA]) 
			printf(" sdma%u\n",(val[DMA_MODE] & MODE) >> 8);
		if(val[SINGLE_DMA]) {
			jj = val[SINGLE_DMA];   kk = val[SINGLE_DMA] >> 8;
			err_dma += mode_loop(jj,kk,'s',&have_mode);
		}
		if(val[MULTI_DMA]) {
			jj = val[MULTI_DMA];   kk = val[MULTI_DMA] >> 8;
			err_dma += mode_loop(jj,kk,'m',&have_mode);
		}
		if((val[WHATS_VALID] & OK_W88) && val[ULTRA_DMA]) {
			jj = val[ULTRA_DMA];   kk = val[ULTRA_DMA] >> 8;
			err_dma += mode_loop(jj,kk,'u',&have_mode);
		}
		if(err_dma || !have_mode) printf("(?)");
		printf("\n");

		if((dev == ATAPI_DEV) && (eqpt != CDROM) && (val[CAPAB_0] & DMA_IL_SUP)) 
			printf("\t     Interleaved DMA support\n");

		if((val[WHATS_VALID] & OK_W64_70) && 
		   (val[DMA_TIME_MIN] || val[DMA_TIME_NORM])) {
			printf("\t     Cycle time:");
			if(val[DMA_TIME_MIN])
				printf(" min=%uns",val[DMA_TIME_MIN]);
			if(val[DMA_TIME_NORM])
				printf(" recommended=%uns",val[DMA_TIME_NORM]);
			printf("\n");
		}
	}

	/* Programmed IO stuff */
	printf("\tPIO: ");
        /* If a drive supports mode n (e.g. 3), it also supports all modes less
	 * than n (e.g. 3, 2, 1 and 0).  Print all the modes. */
	if((val[WHATS_VALID] & OK_W64_70) && (val[ADV_PIO_MODES] & PIO_SUP)) {
		jj = ((val[ADV_PIO_MODES] & PIO_SUP) << 3) | 0x0007;
		for(ii = 0; ii <= PIO_MODE_MAX ; ii++) {
			if(jj & 0x0001)
				printf("pio%d ",ii);
			jj >>=1;
		}
		printf("\n");
	} else if(((min_std < 5) || (eqpt == CDROM)) && (val[PIO_MODE] & MODE) ) {
		for(ii = 0; ii <= val[PIO_MODE]>>8; ii++) {
			printf("pio%d ",ii);
		}
		printf("\n");
	} else  printf("unknown\n");
	if(val[WHATS_VALID] & OK_W64_70) {
		if(val[PIO_NO_FLOW] || val[PIO_FLOW]) {
			printf("\t     Cycle time:");
			if(val[PIO_NO_FLOW])
				printf(" no flow control=%uns", val[PIO_NO_FLOW]);
			if(val[PIO_FLOW])
				printf("  IORDY flow control=%uns", val[PIO_FLOW]);
			printf("\n");
		}
	}

	if((val[CMDS_SUPP_1] & VALID) == VALID_VAL){
		printf("Commands/features:\n\tEnabled\tSupported:\n");
		jj = val[CMDS_SUPP_0];
		kk = val[CMDS_EN_0];
		for(ii = 0; ii < NUM_CMD_FEAT_STR; ii++) {
			if((jj & 0x8000) && (*cmd_feat_str[ii] != '\0')) {
				if(kk & 0x8000) printf("\t   *");
				else		printf("\t");
				printf("\t%s\n",cmd_feat_str[ii]);
			}
			jj <<=1; kk<<=1;
			if(ii%16 == 15) {
				jj = val[CMDS_SUPP_0+1+(ii/16)];
				kk = val[CMDS_EN_0+1+(ii/16)];
			}
			if(ii == 31) {
				if((val[CMDS_SUPP_2] & VALID) != VALID_VAL) ii +=16;
			}
		}
	}
	if((val[RM_STAT] & RM_STAT_BITS) == RM_STAT_SUP) 
		printf("\tRemovable Media Status Notification feature set supported\n");
	
	
	/* security */
	if((eqpt != CDROM) && (like_std > 3) && 
	   (val[SECU_STATUS] || val[ERASE_TIME] || val[ENH_ERASE_TIME])) {
		printf("Security: \n");
		if(val[PSWD_CODE] && (val[PSWD_CODE] != NOVAL_1))
			printf("\tMaster password revision code = %u\n",val[PSWD_CODE]);
		jj = val[SECU_STATUS];
		if(jj) {
			for(ii = 0; ii < NUM_SECU_STR; ii++) {
				if(!(jj & 0x0001)) printf("\tnot\t");
				else		   printf("\t\t");
				printf("%s\n",secu_str[ii]);
				jj >>=1;
			}
			if(val[SECU_STATUS] & SECU_ENABLED) {
				printf("\tSecurity level ");
				if(val[SECU_STATUS] & SECU_LEVEL) printf("maximum\n");
				else				  printf("high\n");
			}
		}
		jj =  val[ERASE_TIME]     & ERASE_BITS;
		kk =  val[ENH_ERASE_TIME] & ERASE_BITS;
		if(jj || kk) {
			printf("\t");
			if(jj) printf("%umin for SECURITY ERASE UNIT. ", jj==ERASE_BITS ? 508 : jj<<1);
			if(kk) printf("%umin for ENHANCED SECURITY ERASE UNIT.", kk==ERASE_BITS ? 508 : kk<<1);
			printf("\n");
		}
	}

	/* reset result */
	if((val[HWRST_RSLT] & VALID) == VALID_VAL) {
		printf("HW reset results:\n");
		if(val[HWRST_RSLT] & CBLID) printf("\tCBLID- above Vih\n");
		else			    printf("\tCBLID- below Vih\n");
		if(val[HWRST_RSLT] & RST0)  {
			printf("\tDevice num = 0");
			jj = val[HWRST_RSLT];
		} else {
			printf("\tDevice num = 1");
			jj = val[HWRST_RSLT] >> 8;
		}
		if((jj & DEV_DET) == JUMPER_VAL) 
			printf(" determined by the jumper");
		else if((jj & DEV_DET) == CSEL_VAL)
			printf(" determined by CSEL");
		printf("\n");
	}

	/* more stuff from std 5 */
	if((like_std > 4) && (eqpt != CDROM)) {
		if(val[CFA_PWR_MODE] & VALID_W160) {
			printf("CFA power mode 1:\n\t");
			if(val[CFA_PWR_MODE] & PWR_MODE_OFF) printf("dis");
			else				     printf("en");
			printf("abled");
			if(val[CFA_PWR_MODE] & PWR_MODE_REQ)
				printf(" and required by some commands");
			printf("\n");
			if(val[CFA_PWR_MODE] & MAX_AMPS)
				printf("\tMaximum current = %uma\n",val[CFA_PWR_MODE] & MAX_AMPS);
		}
		if((val[INTEGRITY] & SIG) == SIG_VAL) {
			printf("Checksum: ");
			if(chksum) printf("in");
			printf("correct\n");
		}
	}

	exit(0);
}
#endif

#define VERSION "v5.4"

#undef DO_FLUSHCACHE		/* under construction: force cache flush on -W0 */
#define TIMING_BUF_MB		2
#define TIMING_BUF_BYTES	(TIMING_BUF_MB * 1024 * 1024)
#define BUFCACHE_FACTOR		2

static int verbose = 0, get_identity = 0, get_geom = 0, noisy = 1, quiet = 0;
static int flagcount = 0, do_flush = 0, is_scsi_hd = 0, is_xt_hd = 0;
static int do_ctimings, do_timings = 0;

static unsigned long set_readahead= 0, get_readahead= 0, readahead= 0;     
static unsigned long set_readonly = 0, get_readonly = 0, readonly = 0;
static unsigned long set_unmask   = 0, get_unmask   = 0, unmask   = 0;     
static unsigned long set_mult     = 0, get_mult     = 0, mult     = 0;     
static unsigned long set_dma      = 0, get_dma      = 0, dma      = 0;
static unsigned long set_dma_q	  = 0, get_dma_q    = 0, dma_q	  = 0;
static unsigned long set_nowerr   = 0, get_nowerr   = 0, nowerr   = 0;
static unsigned long set_keep     = 0, get_keep     = 0, keep     = 0;
static unsigned long set_io32bit  = 0, get_io32bit  = 0, io32bit  = 0;
static unsigned long set_piomode  = 0, noisy_piomode= 0;
static int piomode = 0;
#ifdef HDIO_DRIVE_CMD
static unsigned long set_dkeep    = 0, get_dkeep    = 0, dkeep    = 0;
static unsigned long set_standby  = 0, get_standby  = 0, standby_requested= 0;
static unsigned long set_xfermode = 0, get_xfermode = 0;
static int xfermode_requested= 0;
static unsigned long set_lookahead= 0, get_lookahead= 0, lookahead= 0;
static unsigned long set_prefetch = 0, get_prefetch = 0, prefetch = 0;
static unsigned long set_defects  = 0, get_defects  = 0, defects  = 0;
static unsigned long set_wcache   = 0, get_wcache   = 0, wcache   = 0;
static unsigned long set_doorlock = 0, get_doorlock = 0, doorlock = 0;
static unsigned long set_seagate  = 0, get_seagate  = 0;
static unsigned long set_standbynow = 0, get_standbynow = 0;
static unsigned long set_sleepnow   = 0, get_sleepnow   = 0;
static unsigned long get_powermode  = 0;
static unsigned long set_apmmode = 0, get_apmmode= 0, apmmode = 0;
#endif
#ifdef CONFIG_FEATURE_HDPARM_GET_IDENTITY
static int get_IDentity = 0;
#endif
#ifdef CONFIG_FEATURE_HDPARM_HDIO_UNREGISTER_HWIF
static int	unregister_hwif = 0;
static int	hwif = 0;
#endif
#ifdef CONFIG_FEATURE_HDPARM_HDIO_SCAN_HWIF
static int	scan_hwif = 0;
static int	hwif_data = 0;
static int	hwif_ctrl = 0;
static int	hwif_irq = 0;
#endif
#ifdef CONFIG_FEATURE_HDPARM_HDIO_TRISTATE_HWIF
static int	set_busstate = 0, get_busstate = 0, busstate = 0;
#endif
static int	reread_partn = 0;

#ifdef CONFIG_FEATURE_HDPARM_HDIO_DRIVE_RESET
static int	perform_reset = 0;
#endif /* CONFIG_FEATURE_HDPARM_HDIO_DRIVE_RESET */
#ifdef CONFIG_FEATURE_HDPARM_HDIO_TRISTATE_HWIF
static int	perform_tristate = 0,	tristate = 0;
#endif /* CONFIG_FEATURE_HDPARM_HDIO_TRISTATE_HWIF */

// Historically, if there was no HDIO_OBSOLETE_IDENTITY, then
// then the HDIO_GET_IDENTITY only returned 142 bytes.
// Otherwise, HDIO_OBSOLETE_IDENTITY returns 142 bytes,
// and HDIO_GET_IDENTITY returns 512 bytes.  But the latest
// 2.5.xx kernels no longer define HDIO_OBSOLETE_IDENTITY
// (which they should, but they should just return -EINVAL).
//
// So.. we must now assume that HDIO_GET_IDENTITY returns 512 bytes.
// On a really old system, it will not, and we will be confused.
// Too bad, really.

#ifdef CONFIG_FEATURE_HDPARM_GET_IDENTITY
static const char *cfg_str[] =
{	"",	     " HardSect",   " SoftSect",  " NotMFM",
	" HdSw>15uSec", " SpinMotCtl", " Fixed",     " Removeable",
	" DTR<=5Mbs",   " DTR>5Mbs",   " DTR>10Mbs", " RotSpdTol>.5%",
	" dStbOff",     " TrkOff",     " FmtGapReq", " nonMagnetic"
};

static const char *BuffType[]	= {"unknown", "1Sect", "DualPort", "DualPortCache"};

#define YN(b)	(((b)==0)?"no":"yes")

static void dmpstr (const char *prefix, unsigned int i, const char *s[], unsigned int maxi)
{
	if (i > maxi)
		printf("%s%u", prefix, i);
	else
		printf("%s%s", prefix, s[i]);
}

static void dump_identity (const struct hd_driveid *id)
{
	int i;
	char pmodes[64] = {0,}, dmodes[128]={0,}, umodes[128]={0,};
	const unsigned short int *id_regs= (const void*) id;
	unsigned long capacity;

	printf("\n Model=%.40s, FwRev=%.8s, SerialNo=%.20s",
		id->model, id->fw_rev, id->serial_no);
	printf("\n Config={");
	for (i=0; i<=15; i++) {
		if (id->config & (1<<i))
			printf("%s", cfg_str[i]);
	}
	printf(" }\n");
	printf(" RawCHS=%u/%u/%u, TrkSize=%u, SectSize=%u, ECCbytes=%u\n",
		id->cyls, id->heads, id->sectors,
		id->track_bytes, id->sector_bytes, id->ecc_bytes);
	dmpstr(" BuffType=",id->buf_type,BuffType,3);
	printf(", BuffSize=%ukB, MaxMultSect=%u", id->buf_size/2, id->max_multsect);
	if (id->max_multsect) {
		printf(", MultSect=");
		if (!(id->multsect_valid&1))
			printf("?%u?", id->multsect);
		else if (id->multsect)
			printf("%u", id->multsect);
		else
			printf("off");
	}
	putchar('\n');
	if (id->tPIO <= 5) {
		strcat(pmodes, "pio0 ");
		if (id->tPIO >= 1) strcat(pmodes, "pio1 ");
		if (id->tPIO >= 2) strcat(pmodes, "pio2 ");
	}
	if (!(id->field_valid&1))
		printf(" (maybe):");
#if __BYTE_ORDER == __BIG_ENDIAN
	capacity = (id->cur_capacity0 << 16) | id->cur_capacity1;
#else
	capacity = (id->cur_capacity1 << 16) | id->cur_capacity0;
#endif
	printf(" CurCHS=%u/%u/%u, CurSects=%lu", id->cur_cyls, id->cur_heads, id->cur_sectors, capacity);
	printf(", LBA=%s", YN(id->capability&2));
	if (id->capability&2)
 		printf(", LBAsects=%u", id->lba_capacity);

	if (id->capability&1) {
		if (id->dma_1word | id->dma_mword) {
			if (id->dma_1word & 0x100)	strcat(dmodes,"*");
			if (id->dma_1word & 1)		strcat(dmodes,"sdma0 ");
			if (id->dma_1word & 0x200)	strcat(dmodes,"*");
			if (id->dma_1word & 2)		strcat(dmodes,"sdma1 ");
			if (id->dma_1word & 0x400)	strcat(dmodes,"*");
			if (id->dma_1word & 4)		strcat(dmodes,"sdma2 ");
			if (id->dma_1word & 0xf800)	strcat(dmodes,"*");
			if (id->dma_1word & 0xf8)	strcat(dmodes,"sdma? ");
			if (id->dma_mword & 0x100)	strcat(dmodes,"*");
			if (id->dma_mword & 1)		strcat(dmodes,"mdma0 ");
			if (id->dma_mword & 0x200)	strcat(dmodes,"*");
			if (id->dma_mword & 2)		strcat(dmodes,"mdma1 ");
			if (id->dma_mword & 0x400)	strcat(dmodes,"*");
			if (id->dma_mword & 4)		strcat(dmodes,"mdma2 ");
			if (id->dma_mword & 0xf800)	strcat(dmodes,"*");
			if (id->dma_mword & 0xf8)	strcat(dmodes,"mdma? ");
		}
	}
	printf("\n IORDY=");
	if (id->capability&8)
		printf((id->capability&4) ? "on/off" : "yes");
	else
		printf("no");
	if ((id->capability&8) || (id->field_valid&2)) {
		if (id->field_valid&2) {
			printf(", tPIO={min:%u,w/IORDY:%u}", id->eide_pio, id->eide_pio_iordy);
			if (id->eide_pio_modes & 1)	strcat(pmodes, "pio3 ");
			if (id->eide_pio_modes & 2)	strcat(pmodes, "pio4 ");
			if (id->eide_pio_modes &~3)	strcat(pmodes, "pio? ");
		}
		if (id->field_valid&4) {
			if (id->dma_ultra & 0x100)	strcat(umodes,"*");
			if (id->dma_ultra & 0x001)	strcat(umodes,"udma0 ");
			if (id->dma_ultra & 0x200)	strcat(umodes,"*");
			if (id->dma_ultra & 0x002)	strcat(umodes,"udma1 ");
			if (id->dma_ultra & 0x400)	strcat(umodes,"*");
			if (id->dma_ultra & 0x004)	strcat(umodes,"udma2 ");
#ifdef __NEW_HD_DRIVE_ID
			if (id->hw_config & 0x2000) {
#else /* !__NEW_HD_DRIVE_ID */
			if (id->word93 & 0x2000) {
#endif /* __NEW_HD_DRIVE_ID */
				if (id->dma_ultra & 0x0800)	strcat(umodes,"*");
				if (id->dma_ultra & 0x0008)	strcat(umodes,"udma3 ");
				if (id->dma_ultra & 0x1000)	strcat(umodes,"*");
				if (id->dma_ultra & 0x0010)	strcat(umodes,"udma4 ");
				if (id->dma_ultra & 0x2000)	strcat(umodes,"*");
				if (id->dma_ultra & 0x0020)	strcat(umodes,"udma5 ");
				if (id->dma_ultra & 0x4000)	strcat(umodes,"*");
				if (id->dma_ultra & 0x0040)	strcat(umodes,"udma6 ");
				if (id->dma_ultra & 0x8000)	strcat(umodes,"*");
				if (id->dma_ultra & 0x0080)	strcat(umodes,"udma7 ");
			}
		}
	}
	if ((id->capability&1) && (id->field_valid&2))
		printf(", tDMA={min:%u,rec:%u}", id->eide_dma_min, id->eide_dma_time);
	printf("\n PIO modes:  %s", pmodes);
	if (*dmodes)
		printf("\n DMA modes:  %s", dmodes);
	if (*umodes)
		printf("\n UDMA modes: %s", umodes);

	printf("\n AdvancedPM=%s",YN(id_regs[83]&8));
	if (id_regs[83] & 8) { 
		if (!(id_regs[86]&8)) 
			printf(": disabled (255)"); 
		else if ((id_regs[91]&0xFF00)!=0x4000) 
			printf(": unknown setting"); 
		else 
			printf(": mode=0x%02X (%u)",id_regs[91]&0xFF,id_regs[91]&0xFF); 
	} 
	if (id_regs[82]&0x20)
		printf(" WriteCache=%s",(id_regs[85]&0x20) ? "enabled" : "disabled");
#ifdef __NEW_HD_DRIVE_ID
	if ((id->minor_rev_num && id->minor_rev_num <= 31) || (id->major_rev_num && id->minor_rev_num <= 31)) {
		printf("\n Drive conforms to: ");
		if (id->minor_rev_num <= 31)
			printf("%s: ", minor_str[id->minor_rev_num]);
		else
			printf("unknown: ");
		if (id->major_rev_num < 31) {
			for (i=0; i <= 15; i++) {
				if (id->major_rev_num & (1<<i))
					printf(" %u", i);
			}
		}
	}
#endif /* __NEW_HD_DRIVE_ID */
	printf("\n");	
	printf("\n * signifies the current active mode\n");
	printf("\n");
}
#endif

static void flush_buffer_cache (int fd)
{
	fsync (fd);				/* flush buffers */
	if (ioctl(fd, BLKFLSBUF, NULL))		/* do it again, big time */
		bb_perror_msg("BLKFLSBUF failed");
#ifdef HDIO_DRIVE_CMD
	if (is_scsi_hd || is_xt_hd) {
		sleep(1);
	} else {
		if (ioctl(fd, HDIO_DRIVE_CMD, NULL) && errno != EINVAL)	/* await completion */
			bb_perror_msg("HDIO_DRIVE_CMD(null) (wait for flush complete) failed");
	}
#endif
}

static int seek_to_zero (int fd)
{
	if (lseek(fd, (off_t) 0, SEEK_SET)) {
		bb_perror_msg("lseek() failed");
		return 1;
	}
	return 0;
}

static int read_big_block (int fd, char *buf)
{
	int i, rc;
	if ((rc = read(fd, buf, TIMING_BUF_BYTES)) != TIMING_BUF_BYTES) {
		if (rc) {
			if (rc == -1)
				bb_perror_msg("read() failed");
			else
				bb_error_msg("read(%u) returned %u bytes", TIMING_BUF_BYTES, rc);
		} else {
			fputs ("read() hit EOF - device too small\n", stderr);
		}
		return 1;
	}
	/* access all sectors of buf to ensure the read fully completed */
	for (i = 0; i < TIMING_BUF_BYTES; i += 512)
		buf[i] &= 1;
	return 0;
}

static void time_cache (int fd)
{
	char *buf;
	struct itimerval e1, e2;
	int shmid;
	double elapsed, elapsed2;
	unsigned int iterations, total_MB;

	if ((shmid = shmget(IPC_PRIVATE, TIMING_BUF_BYTES, 0600)) == -1) {
		bb_perror_msg ("could not allocate sharedmem buf");
		return;
	}
	if (shmctl(shmid, SHM_LOCK, NULL) == -1) {
		bb_perror_msg ("could not lock sharedmem buf");
		(void) shmctl(shmid, IPC_RMID, NULL);
		return;
	}
	if ((buf = shmat(shmid, (char *) 0, 0)) == (char *) -1) {
		bb_perror_msg ("could not attach sharedmem buf");
		(void) shmctl(shmid, IPC_RMID, NULL);
		return;
	}
	if (shmctl(shmid, IPC_RMID, NULL) == -1)
		bb_perror_msg ("shmctl(,IPC_RMID,) failed");

	/* Clear out the device request queues & give them time to complete */
	sync();
	sleep(3);

	/*
	 * getitimer() is used rather than gettimeofday() because
	 * it is much more consistent (on my machine, at least).
	 */
	setitimer(ITIMER_REAL, &(struct itimerval){{1000,0},{1000,0}}, NULL);
	if (seek_to_zero (fd)) return;
	if (read_big_block (fd, buf)) return;
	printf(" Timing buffer-cache reads:   ");
	fflush(stdout);

	/* Clear out the device request queues & give them time to complete */
	sync();
	sleep(1);

	/* Now do the timing */
	iterations = 0;
	getitimer(ITIMER_REAL, &e1);
	do {
		++iterations;
		if (seek_to_zero (fd) || read_big_block (fd, buf))
			goto quit;
		getitimer(ITIMER_REAL, &e2);
		elapsed = (e1.it_value.tv_sec - e2.it_value.tv_sec)
		 + ((e1.it_value.tv_usec - e2.it_value.tv_usec) / 1000000.0);
	} while (elapsed < 2.0);
	total_MB = iterations * TIMING_BUF_MB;

	elapsed = (e1.it_value.tv_sec - e2.it_value.tv_sec)
	 + ((e1.it_value.tv_usec - e2.it_value.tv_usec) / 1000000.0);

	/* Now remove the lseek() and getitimer() overheads from the elapsed time */
	getitimer(ITIMER_REAL, &e1);
	do {
		if (seek_to_zero (fd))
			goto quit;
		getitimer(ITIMER_REAL, &e2);
		elapsed2 = (e1.it_value.tv_sec - e2.it_value.tv_sec)
		 + ((e1.it_value.tv_usec - e2.it_value.tv_usec) / 1000000.0);
	} while (--iterations);

	elapsed -= elapsed2;

	if ((BUFCACHE_FACTOR * total_MB) >= elapsed)  /* more than 1MB/s */
		printf("%3u MB in %5.2f seconds = %6.2f MB/sec\n",
			(BUFCACHE_FACTOR * total_MB), elapsed,
			(BUFCACHE_FACTOR * total_MB) / elapsed);
	else
		printf("%3u MB in %5.2f seconds = %6.2f kB/sec\n",
			(BUFCACHE_FACTOR * total_MB), elapsed,
			(BUFCACHE_FACTOR * total_MB) / elapsed * 1024);

	flush_buffer_cache(fd);
	sleep(1);
quit:
	if (-1 == shmdt(buf))
		bb_perror_msg ("could not detach sharedmem buf");
}

static void time_device (int fd)
{
	char *buf;
	double elapsed;
	struct itimerval e1, e2;
	int shmid;
	unsigned int max_iterations = 1024, total_MB, iterations;
	static long parm;

	//
	// get device size
	//
	if (do_ctimings || do_timings) {
		if (ioctl(fd, BLKGETSIZE, &parm))
			bb_perror_msg(" BLKGETSIZE failed");
		else
			max_iterations = parm / (2 * 1024) / TIMING_BUF_MB;
	}

	if ((shmid = shmget(IPC_PRIVATE, TIMING_BUF_BYTES, 0600)) == -1) {
		bb_perror_msg ("could not allocate sharedmem buf");
		return;
	}
	if (shmctl(shmid, SHM_LOCK, NULL) == -1) {
		bb_perror_msg ("could not lock sharedmem buf");
		(void) shmctl(shmid, IPC_RMID, NULL);
		return;
	}
	if ((buf = shmat(shmid, (char *) 0, 0)) == (char *) -1) {
		bb_perror_msg ("could not attach sharedmem buf");
		(void) shmctl(shmid, IPC_RMID, NULL);
		return;
	}
	if (shmctl(shmid, IPC_RMID, NULL) == -1)
		bb_perror_msg ("shmctl(,IPC_RMID,) failed");

	/* Clear out the device request queues & give them time to complete */
	sync();
	sleep(3);

	printf(" Timing buffered disk reads:  ");
	fflush(stdout);

	/*
	 * getitimer() is used rather than gettimeofday() because
	 * it is much more consistent (on my machine, at least).
	 */
	setitimer(ITIMER_REAL, &(struct itimerval){{1000,0},{1000,0}}, NULL);

	/* Now do the timings for real */
	iterations = 0;
	getitimer(ITIMER_REAL, &e1);
	do {
		++iterations;
		if (read_big_block (fd, buf))
			goto quit;
		getitimer(ITIMER_REAL, &e2);
		elapsed = (e1.it_value.tv_sec - e2.it_value.tv_sec)
		 + ((e1.it_value.tv_usec - e2.it_value.tv_usec) / 1000000.0);
	} while (elapsed < 3.0 && iterations < max_iterations);

	total_MB = iterations * TIMING_BUF_MB;
	if ((total_MB / elapsed) > 1.0)  /* more than 1MB/s */
		printf("%3u MB in %5.2f seconds = %6.2f MB/sec\n",
			total_MB, elapsed, total_MB / elapsed);
	else
		printf("%3u MB in %5.2f seconds = %6.2f kB/sec\n",
			total_MB, elapsed, total_MB / elapsed * 1024);
quit:
	if (-1 == shmdt(buf))
		bb_perror_msg ("could not detach sharedmem buf");
}

static void no_scsi (void)
{
	if (is_scsi_hd) {
		bb_error_msg_and_die(" operation not supported on SCSI disks\n");
	}
}

static void no_xt (void)
{
	if (is_xt_hd) {
		bb_error_msg_and_die (" operation not supported on XT disks\n");
	}
}

static void on_off (unsigned int value)
{
	printf(value ? " (on)\n" : " (off)\n");
}

#ifdef CONFIG_FEATURE_HDPARM_HDIO_TRISTATE_HWIF
static void bus_state_value (unsigned int value)
{
	const char *string;

	switch (value) {
	case BUSSTATE_ON:
		string = " (on)\n";
		break;
	case BUSSTATE_OFF:
		string = " (off)\n";
		break;
	case BUSSTATE_TRISTATE:
		string = " (tristate)\n";
		break;
	default:
		string = " (unknown: %d)\n";
		break;
	}
	printf(string, value);
}
#endif

#ifdef HDIO_DRIVE_CMD
static void interpret_standby (unsigned int standby)
{
	printf(" (");
	switch(standby) {
		case 0:		printf("off");
				break;
		case 252:	printf("21 minutes");
				break;
		case 253:	printf("vendor-specific");
				break;
		case 254:	printf("?reserved");
				break;
		case 255:	printf("21 minutes + 15 seconds");
				break;
		default:
			if (standby <= 240) {
				unsigned int secs = standby * 5;
				unsigned int mins = secs / 60;
				secs %= 60;
				if (mins)	  printf("%u minutes", mins);
				if (mins && secs) printf(" + ");
				if (secs)	  printf("%u seconds", secs);
			} else if (standby <= 251) {
				unsigned int mins = (standby - 240) * 30;
				unsigned int hrs  = mins / 60;
				mins %= 60;
				if (hrs)	  printf("%u hours", hrs);
				if (hrs && mins)  printf(" + ");
				if (mins)	  printf("%u minutes", mins);
			} else
				printf("illegal value");
			break;
	}
	printf(")\n");
}

struct xfermode_entry {
    int val;
    const char *name;
};

static const struct xfermode_entry xfermode_table[] = {
    { 8,    "pio0" },
    { 9,    "pio1" },
    { 10,   "pio2" },
    { 11,   "pio3" },
    { 12,   "pio4" },
    { 13,   "pio5" },
    { 14,   "pio6" },
    { 15,   "pio7" },
    { 16,   "sdma0" },
    { 17,   "sdma1" },
    { 18,   "sdma2" },
    { 19,   "sdma3" },
    { 20,   "sdma4" },
    { 21,   "sdma5" },
    { 22,   "sdma6" },
    { 23,   "sdma7" },
    { 32,   "mdma0" },
    { 33,   "mdma1" },
    { 34,   "mdma2" },
    { 35,   "mdma3" },
    { 36,   "mdma4" },
    { 37,   "mdma5" },
    { 38,   "mdma6" },
    { 39,   "mdma7" },
    { 64,   "udma0" },
    { 65,   "udma1" },
    { 66,   "udma2" },
    { 67,   "udma3" },
    { 68,   "udma4" },
    { 69,   "udma5" },
    { 70,   "udma6" },
    { 71,   "udma7" },
    { 0, NULL }
};

static int translate_xfermode(char * name)
{
    const struct xfermode_entry *tmp;
    char *endptr;
    int val = -1;


    for (tmp = xfermode_table; tmp->name != NULL; ++tmp) {
	if (!strcmp(name, tmp->name))
	    return tmp->val;
    }

    val = strtol(name, &endptr, 10);
    if (*endptr == '\0')
	return val;

    return -1;
}

static void interpret_xfermode (unsigned int xfermode)
{
	printf(" (");
	switch(xfermode) {
		case 0:		printf("default PIO mode");
				break;
		case 1:		printf("default PIO mode, disable IORDY");
				break;
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:	printf("PIO flow control mode%u", xfermode-8);
				break;
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:	printf("singleword DMA mode%u", xfermode-16);
				break;
		case 32:
		case 33:
		case 34:
		case 35:
		case 36:
		case 37:
		case 38:
		case 39:	printf("multiword DMA mode%u", xfermode-32);
				break;
		case 64:
		case 65:
		case 66:
		case 67:
		case 68:
		case 69:
		case 70:
		case 71:	printf("UltraDMA mode%u", xfermode-64);
				break;
		default:
				printf("unknown, probably not valid");
				break;
	}
	printf(")\n");
}
#endif /* HDIO_DRIVE_CMD */

#ifndef VXVM_MAJOR
#define VXVM_MAJOR 199
#endif

#ifndef CCISS_MAJOR
#define CCISS_MAJOR 104
#endif

static void process_dev (char *devname)
{
	int fd;
	static long parm, multcount;
	struct stat stat_buf;
#ifndef HDIO_DRIVE_CMD
	int force_operation = 0;
#endif
	if (stat(devname,&stat_buf))
		bb_perror_msg_and_die(devname);

	switch(major(stat_buf.st_rdev)) {
#ifdef SCSI_DISK0_MAJOR
	case (SCSI_DISK0_MAJOR):
	case (SCSI_DISK1_MAJOR):
	case (SCSI_DISK2_MAJOR):
	case (SCSI_DISK3_MAJOR):
	case (SCSI_DISK4_MAJOR):
	case (SCSI_DISK5_MAJOR):
	case (SCSI_DISK6_MAJOR):
	case (SCSI_DISK7_MAJOR):
#else
	case (SCSI_DISK_MAJOR):
#endif
#ifdef MD_MAJOR
	case (MD_MAJOR):
#endif
	case (VXVM_MAJOR):
#ifdef LVM_BLK_MAJOR
	case (LVM_BLK_MAJOR):
#endif
	case (CCISS_MAJOR):
		is_scsi_hd = 1;
		break;
#ifdef XT_DISK_MAJOR
	case (XT_DISK_MAJOR):
		is_xt_hd = 1;
		break;
#endif
	case IDE0_MAJOR:
	case IDE1_MAJOR:
#ifdef IDE2_MAJOR
	case IDE2_MAJOR:
#endif
#ifdef IDE3_MAJOR
	case IDE3_MAJOR:
#endif
#ifdef IDE4_MAJOR
	case IDE4_MAJOR:
#endif
#ifdef IDE5_MAJOR
	case IDE5_MAJOR:
#endif
#ifdef IDE6_MAJOR
	case IDE6_MAJOR:
#endif
#ifdef IDE7_MAJOR
	case IDE7_MAJOR:
#endif
#ifdef IDE8_MAJOR
	case IDE8_MAJOR:
#endif
#ifdef IDE9_MAJOR
	case IDE9_MAJOR:
#endif
		break;	/* do nothing */
	default:
		bb_error_msg_and_die("%s not supported by hdparm",devname);
	}

	fd = open (devname, O_RDONLY|O_NONBLOCK);
	if (fd < 0)
		bb_perror_msg_and_die(devname);
	if (!quiet)
		printf("\n%s:\n", devname);

	if (set_readahead) {
		if (get_readahead)
			printf(" setting fs readahead to %ld\n", readahead);
		if (ioctl(fd, BLKRASET, readahead)) 
			bb_perror_msg(" BLKRASET failed");
	}
#ifdef CONFIG_FEATURE_HDPARM_HDIO_UNREGISTER_HWIF
	if (unregister_hwif) {
		no_scsi();
		printf(" attempting to unregister hwif#%u\n", hwif);
		if (ioctl(fd, HDIO_UNREGISTER_HWIF, hwif)) 
			bb_perror_msg(" HDIO_UNREGISTER_HWIF failed");
	}
#endif
#ifdef CONFIG_FEATURE_HDPARM_HDIO_SCAN_HWIF
	if (scan_hwif) {
		int	args[3];
		no_scsi();
		printf(" attempting to scan hwif (0x%x, 0x%x, %u)\n", hwif_data, hwif_ctrl, hwif_irq);
		args[0] = hwif_data;
		args[1] = hwif_ctrl;
		args[2] = hwif_irq;
		if (ioctl(fd, HDIO_SCAN_HWIF, args)) 
			bb_perror_msg(" HDIO_SCAN_HWIF failed");
	}
#endif
	if (set_piomode) {
		no_scsi();
		no_xt();
		if (noisy_piomode) {
			if (piomode == 255)
				printf(" attempting to auto-tune PIO mode\n");
			else if (piomode < 100)
				printf(" attempting to set PIO mode to %d\n", piomode);
			else if (piomode < 200)
				printf(" attempting to set MDMA mode to %d\n", (piomode-100));
			else
				printf(" attempting to set UDMA mode to %d\n", (piomode-200));
		}
		if (ioctl(fd, HDIO_SET_PIO_MODE, piomode)) 
			bb_perror_msg(" HDIO_SET_PIO_MODE failed");
	}
	if (set_io32bit) {
		no_scsi();
		no_xt();
		if (get_io32bit)
			printf(" setting 32-bit IO_support flag to %ld\n", io32bit);
		if (ioctl(fd, HDIO_SET_32BIT, io32bit)) 
			bb_perror_msg(" HDIO_SET_32BIT failed");
	}
	if (set_mult) {
		no_scsi();
		no_xt();
		if (get_mult)
			printf(" setting multcount to %ld\n", mult);
		if (ioctl(fd, HDIO_SET_MULTCOUNT, mult)) 
			bb_perror_msg(" HDIO_SET_MULTCOUNT failed");
#ifndef HDIO_DRIVE_CMD
		else force_operation = 1;
#endif
	}
	if (set_readonly) {
		if (get_readonly) {
			printf(" setting readonly to %ld", readonly);
			on_off(readonly);
		}
		if (ioctl(fd, BLKROSET, &readonly))
			bb_perror_msg(" BLKROSET failed");
	}
	if (set_unmask) {
		no_scsi();
		no_xt();
		if (get_unmask) {
			printf(" setting unmaskirq to %ld", unmask);
			on_off(unmask);
		}
		if (ioctl(fd, HDIO_SET_UNMASKINTR, unmask))
			bb_perror_msg(" HDIO_SET_UNMASKINTR failed");
	}
	if (set_dma) {
		no_scsi();
		if (get_dma) {
			printf(" setting using_dma to %ld", dma);
			on_off(dma);
		}
		if (ioctl(fd, HDIO_SET_DMA, dma))
			bb_perror_msg(" HDIO_SET_DMA failed");
	}
	if (set_dma_q) {
		no_scsi();
		if (get_dma_q) {
			printf(" setting DMA queue_depth to %ld", dma_q);
			on_off(dma_q);
		}
		if (ioctl(fd, HDIO_SET_QDMA, dma_q))
			bb_perror_msg(" HDIO_SET_QDMA failed");
	}
	if (set_nowerr) {
		no_scsi();
		no_xt();
		if (get_nowerr) {
			printf(" setting nowerr to %ld", nowerr);
			on_off(nowerr);
		}
		if (ioctl(fd, HDIO_SET_NOWERR, nowerr))
			bb_perror_msg(" HDIO_SET_NOWERR failed");
	}
	if (set_keep) {
		no_scsi();
		no_xt();
		if (get_keep) {
			printf(" setting keep_settings to %ld", keep);
			on_off(keep);
		}
		if (ioctl(fd, HDIO_SET_KEEPSETTINGS, keep))
			bb_perror_msg(" HDIO_SET_KEEPSETTINGS failed");
	}
#ifdef HDIO_DRIVE_CMD
	if (set_doorlock) {
		unsigned char args[4] = {0,0,0,0};
		no_scsi();
		no_xt();
		args[0] = doorlock ? WIN_DOORLOCK : WIN_DOORUNLOCK;
		if (get_doorlock) {
			printf(" setting drive doorlock to %ld", doorlock);
			on_off(doorlock);
		}
		if (ioctl(fd, HDIO_DRIVE_CMD, &args))
			bb_perror_msg(" HDIO_DRIVE_CMD(doorlock) failed");
	}
	if (set_dkeep) {
		/* lock/unlock the drive's "feature" settings */
		unsigned char args[4] = {WIN_SETFEATURES,0,0,0};
		no_scsi();
		no_xt();
		if (get_dkeep) {
			printf(" setting drive keep features to %ld", dkeep);
			on_off(dkeep);
		}
		args[2] = dkeep ? 0x66 : 0xcc;
		if (ioctl(fd, HDIO_DRIVE_CMD, &args))
			bb_perror_msg(" HDIO_DRIVE_CMD(keepsettings) failed");
	}
	if (set_defects) {
		unsigned char args[4] = {WIN_SETFEATURES,0,0x04,0};
		no_scsi();
		args[2] = defects ? 0x04 : 0x84;
		if (get_defects)
			printf(" setting drive defect-mgmt to %ld\n", defects);
		if (ioctl(fd, HDIO_DRIVE_CMD, &args))
			bb_perror_msg(" HDIO_DRIVE_CMD(defectmgmt) failed");
	}
	if (set_prefetch) {
		unsigned char args[4] = {WIN_SETFEATURES,0,0xab,0};
		no_scsi();
		no_xt();
		args[1] = prefetch;
		if (get_prefetch)
			printf(" setting drive prefetch to %ld\n", prefetch);
		if (ioctl(fd, HDIO_DRIVE_CMD, &args))
			bb_perror_msg(" HDIO_DRIVE_CMD(setprefetch) failed");
	}
	if (set_xfermode) {
		unsigned char args[4] = {WIN_SETFEATURES,0,3,0};
		no_scsi();
		no_xt();
		args[1] = xfermode_requested;
		if (get_xfermode) {
			printf(" setting xfermode to %d", xfermode_requested);
			interpret_xfermode(xfermode_requested);
		}
		if (ioctl(fd, HDIO_DRIVE_CMD, &args))
			bb_perror_msg(" HDIO_DRIVE_CMD(setxfermode) failed");
	}
	if (set_lookahead) {
		unsigned char args[4] = {WIN_SETFEATURES,0,0,0};
		no_scsi();
		no_xt();
		args[2] = lookahead ? 0xaa : 0x55;
		if (get_lookahead) {
			printf(" setting drive read-lookahead to %ld", lookahead);
			on_off(lookahead);
		}
		if (ioctl(fd, HDIO_DRIVE_CMD, &args))
			bb_perror_msg(" HDIO_DRIVE_CMD(setreadahead) failed");
	}
	if (set_apmmode) {
		unsigned char args[4] = {WIN_SETFEATURES,0,0,0};
		no_scsi();
		if (apmmode<1) apmmode=1;
		if (apmmode>255) apmmode=255;
		if (get_apmmode) 
			printf(" setting Advanced Power Management level to");
		if (apmmode==255) { 
			/* disable Advanced Power Management */
			args[2] = 0x85; /* feature register */ 
			if (get_apmmode) printf(" disabled\n");
		} else {
			/* set Advanced Power Management mode */
			args[2] = 0x05; /* feature register */  
			args[1] = apmmode; /* sector count register */
			if (get_apmmode) printf(" 0x%02lX (%ld)\n",apmmode,apmmode);
		} 
		if (ioctl(fd, HDIO_DRIVE_CMD, &args))
			bb_perror_msg(" HDIO_DRIVE_CMD failed");
	}
	if (set_wcache) {
#ifdef DO_FLUSHCACHE
#ifndef WIN_FLUSHCACHE
#define WIN_FLUSHCACHE 0xe7
#endif
		unsigned char flushcache[4] = {WIN_FLUSHCACHE,0,0,0};
#endif /* DO_FLUSHCACHE */
		unsigned char args[4] = {WIN_SETFEATURES,0,0,0};
		no_scsi();
		no_xt();
		args[2] = wcache ? 0x02 : 0x82;
		if (get_wcache) {
			printf(" setting drive write-caching to %ld", wcache);
			on_off(wcache);
		}
#ifdef DO_FLUSHCACHE
		if (!wcache && ioctl(fd, HDIO_DRIVE_CMD, &flushcache))
			bb_perror_msg (" HDIO_DRIVE_CMD(flushcache) failed");
#endif /* DO_FLUSHCACHE */
		if (ioctl(fd, HDIO_DRIVE_CMD, &args))
			bb_perror_msg(" HDIO_DRIVE_CMD(setcache) failed");
#ifdef DO_FLUSHCACHE
		if (!wcache && ioctl(fd, HDIO_DRIVE_CMD, &flushcache))
			bb_perror_msg (" HDIO_DRIVE_CMD(flushcache) failed");
#endif /* DO_FLUSHCACHE */
	}
	if (set_standbynow) {
#ifndef WIN_STANDBYNOW1
#define WIN_STANDBYNOW1 0xE0
#endif
#ifndef WIN_STANDBYNOW2
#define WIN_STANDBYNOW2 0x94
#endif
		unsigned char args1[4] = {WIN_STANDBYNOW1,0,0,0};
		unsigned char args2[4] = {WIN_STANDBYNOW2,0,0,0};
		no_scsi();
		if (get_standbynow)
			printf(" issuing standby command\n");
		if (ioctl(fd, HDIO_DRIVE_CMD, &args1)
		 && ioctl(fd, HDIO_DRIVE_CMD, &args2))
			bb_perror_msg(" HDIO_DRIVE_CMD(standby) failed");
	}
	if (set_sleepnow) {
#ifndef WIN_SLEEPNOW1
#define WIN_SLEEPNOW1 0xE6
#endif
#ifndef WIN_SLEEPNOW2
#define WIN_SLEEPNOW2 0x99
#endif
		unsigned char args1[4] = {WIN_SLEEPNOW1,0,0,0};
		unsigned char args2[4] = {WIN_SLEEPNOW2,0,0,0};
		no_scsi();
		if (get_sleepnow)
			printf(" issuing sleep command\n");
		if (ioctl(fd, HDIO_DRIVE_CMD, &args1)
		 && ioctl(fd, HDIO_DRIVE_CMD, &args2))
			bb_perror_msg(" HDIO_DRIVE_CMD(sleep) failed");
	}
	if (set_seagate) {
		unsigned char args[4] = {0xfb,0,0,0};
		no_scsi();
		no_xt();
		if (get_seagate)
			printf(" disabling Seagate auto powersaving mode\n");
		if (ioctl(fd, HDIO_DRIVE_CMD, &args))
			bb_perror_msg(" HDIO_DRIVE_CMD(seagatepwrsave) failed");
	}
	if (set_standby) {
		unsigned char args[4] = {WIN_SETIDLE1,standby_requested,0,0};
		no_scsi();
		no_xt();
		if (get_standby) {
			printf(" setting standby to %lu", standby_requested);
			interpret_standby(standby_requested);
		}
		if (ioctl(fd, HDIO_DRIVE_CMD, &args))
			bb_perror_msg(" HDIO_DRIVE_CMD(setidle1) failed");
	}
#else	/* HDIO_DRIVE_CMD */
	if (force_operation) {
		char buf[512];
		flush_buffer_cache(fd);
		if (-1 == read(fd, buf, sizeof(buf)))
			bb_perror_msg(" access failed");
	}
#endif	/* HDIO_DRIVE_CMD */

	if (!flagcount)
		verbose = 1;

	if ((verbose && !is_scsi_hd && !is_xt_hd) || get_mult || get_identity) {
		no_scsi();
		multcount = -1;
		if (ioctl(fd, HDIO_GET_MULTCOUNT, &multcount)) {
			if ((verbose && !is_xt_hd) || get_mult)
				bb_perror_msg(" HDIO_GET_MULTCOUNT failed");
		} else if (verbose | get_mult) {
			printf(" multcount    = %2ld", multcount);
			on_off(multcount);
		}
	}
	if ((verbose && !is_scsi_hd && !is_xt_hd) || get_io32bit) {
		no_scsi();
		no_xt();
		if (ioctl(fd, HDIO_GET_32BIT, &parm))
			bb_perror_msg(" HDIO_GET_32BIT failed");
		else {
			printf(" IO_support   =%3ld (", parm);
			switch (parm) {
				case 0:	printf("default ");
				case 2: printf("16-bit)\n");
					break;
				case 1:	printf("32-bit)\n");
					break;
				case 3:	printf("32-bit w/sync)\n");
					break;
				case 8:	printf("Request-Queue-Bypass)\n");
					break;
				default:printf("\?\?\?)\n");
			}
		}
	}
	if ((verbose && !is_scsi_hd && !is_xt_hd) || get_unmask) {
		no_scsi();
		no_xt();
		if (ioctl(fd, HDIO_GET_UNMASKINTR, &parm))
			bb_perror_msg(" HDIO_GET_UNMASKINTR failed");
		else {
			printf(" unmaskirq    = %2ld", parm);
			on_off(parm);
		}
	}

#ifdef CONFIG_FEATURE_HDPARM_HDIO_GET_DMA
	if ((verbose && !is_scsi_hd) || get_dma) {
		no_scsi();
		if (ioctl(fd, HDIO_GET_DMA, &parm))
			bb_perror_msg(" HDIO_GET_DMA failed");
		else {
			printf(" using_dma    = %2ld", parm);
			if (parm == 8)
				printf(" (DMA-Assisted-PIO)\n");
			else
				on_off(parm);
		}
	}
#endif
	if (get_dma_q) {
		no_scsi();
		if(ioctl(fd, HDIO_GET_QDMA, &parm)) {
			bb_perror_msg(" HDIO_GET_QDMA failed");
		} else {
			printf(" queue_depth  = %2ld", parm);
			on_off(parm);
		}
	}
	if ((verbose && !is_scsi_hd && !is_xt_hd) || get_keep) {
		no_scsi();
		no_xt();
		if (ioctl(fd, HDIO_GET_KEEPSETTINGS, &parm))
			bb_perror_msg(" HDIO_GET_KEEPSETTINGS failed");
		else {
			printf(" keepsettings = %2ld", parm);
			on_off(parm);
		}
	}

	if (get_nowerr) {
		no_scsi();
		no_xt();
		if (ioctl(fd, HDIO_GET_NOWERR, &parm))
			bb_perror_msg(" HDIO_GET_NOWERR failed");
		else {
			printf(" nowerr       = %2ld", parm);
			on_off(parm);
		}
	}
	if (verbose || get_readonly) {
		if (ioctl(fd, BLKROGET, &parm))
			bb_perror_msg(" BLKROGET failed");
		else {
			printf(" readonly     = %2ld", parm);
			on_off(parm);
		}
	}
	if ((verbose && !is_scsi_hd) || get_readahead) {
		if (ioctl(fd, BLKRAGET, &parm))
			bb_perror_msg(" BLKRAGET failed");
		else {
			printf(" readahead    = %2ld", parm);
			on_off(parm);
		}
	}
	if (verbose || get_geom) {
		static const char msg[] = " geometry     = %u/%u/%u, sectors = %ld, start = %ld\n";
		static struct hd_geometry g;
#ifdef HDIO_GETGEO_BIG
		static struct hd_big_geometry bg;
#endif

		if (ioctl(fd, BLKGETSIZE, &parm))
			bb_perror_msg(" BLKGETSIZE failed");
#ifdef HDIO_GETGEO_BIG
		else if (!ioctl(fd, HDIO_GETGEO_BIG, &bg))
			printf(msg, bg.cylinders, bg.heads, bg.sectors, parm, bg.start);
#endif
		else if (ioctl(fd, HDIO_GETGEO, &g))
			bb_perror_msg(" HDIO_GETGEO failed");
		else	printf(msg, g.cylinders, g.heads, g.sectors, parm, g.start);
	}
#ifdef HDIO_DRIVE_CMD
	if (get_powermode) {
#ifndef WIN_CHECKPOWERMODE1
#define WIN_CHECKPOWERMODE1 0xE5
#endif
#ifndef WIN_CHECKPOWERMODE2
#define WIN_CHECKPOWERMODE2 0x98
#endif
		unsigned char args[4] = {WIN_CHECKPOWERMODE1,0,0,0};
		const char *state;
		no_scsi();
		if (ioctl(fd, HDIO_DRIVE_CMD, &args)
		 && (args[0] = WIN_CHECKPOWERMODE2) /* try again with 0x98 */
		 && ioctl(fd, HDIO_DRIVE_CMD, &args)) {
			if (errno != EIO || args[0] != 0 || args[1] != 0)
				state = "unknown";
			else
				state = "sleeping";
		} else {
			state = (args[2] == 255) ? "active/idle" : "standby";
		}
		printf(" drive state is:  %s\n", state);
	}
#endif
#ifdef CONFIG_FEATURE_HDPARM_HDIO_DRIVE_RESET
	if (perform_reset) {
		no_scsi();
		no_xt();
		if (ioctl(fd, HDIO_DRIVE_RESET, NULL))
			bb_perror_msg(" HDIO_DRIVE_RESET failed");
	}
#endif /* CONFIG_FEATURE_HDPARM_HDIO_DRIVE_RESET */
#ifdef CONFIG_FEATURE_HDPARM_HDIO_TRISTATE_HWIF
	if (perform_tristate) {
		unsigned char args[4] = {0,tristate,0,0};
		no_scsi();
		no_xt();
		if (ioctl(fd, HDIO_TRISTATE_HWIF, &args))
			bb_perror_msg(" HDIO_TRISTATE_HWIF failed");
	}
#endif /* CONFIG_FEATURE_HDPARM_HDIO_TRISTATE_HWIF */
#ifdef CONFIG_FEATURE_HDPARM_GET_IDENTITY
	if (get_identity) {
		static struct hd_driveid id;

		no_scsi();
		no_xt();

		if (!ioctl(fd, HDIO_GET_IDENTITY, &id)) {
			if (multcount != -1) {
				id.multsect = multcount;
				id.multsect_valid |= 1;
			} else
				id.multsect_valid &= ~1;
			dump_identity(&id);
		} else if (errno == -ENOMSG)
			printf(" no identification info available\n");
		else
			bb_perror_msg(" HDIO_GET_IDENTITY failed");
	}
	if (get_IDentity) {
		unsigned char args[4+512] = {WIN_IDENTIFY,0,0,1,};
		unsigned i;
		no_scsi();
		no_xt();
		if (ioctl(fd, HDIO_DRIVE_CMD, &args)) {
			args[0] = WIN_PIDENTIFY;
			if (ioctl(fd, HDIO_DRIVE_CMD, &args)) {
				bb_perror_msg(" HDIO_DRIVE_CMD(identify) failed");
				goto identify_abort;
			}
		}
		for(i=0; i<(sizeof args)/2; i+=2) {
		    __le16_to_cpus((uint16_t *)(&args[i]));
		}    
		identify((void *)&args[4], NULL);
identify_abort:	;
	}
#endif
#ifdef CONFIG_FEATURE_HDPARM_HDIO_TRISTATE_HWIF
	if (set_busstate) {
		no_scsi();
		if (get_busstate) {
			printf(" setting bus state to %d", busstate);
			bus_state_value(busstate);
		}
		if (ioctl(fd, HDIO_SET_BUSSTATE, busstate))
			bb_perror_msg(" HDIO_SET_BUSSTATE failed");
	}
#endif
#ifdef CONFIG_FEATURE_HDPARM_HDIO_TRISTATE_HWIF
	if (get_busstate) {
		no_scsi();
		if (ioctl(fd, HDIO_GET_BUSSTATE, &parm))
			bb_perror_msg(" HDIO_GET_BUSSTATE failed");
		else {
			printf(" busstate     = %2ld", parm);
			bus_state_value(parm);
		}
	}
#endif
	if (reread_partn) {
		if (ioctl(fd, BLKRRPART, NULL)) {
			bb_perror_msg(" BLKRRPART failed");
		}
	}

	if (do_ctimings)
		time_cache (fd);
	if (do_timings)
		time_device (fd);
	if (do_flush)
		flush_buffer_cache (fd);
	close (fd);
}

#define GET_NUMBER(flag,num)	num = 0; \
				if (!*p && argc && isdigit(**argv)) \
					p = *argv++, --argc; \
				while (isdigit(*p)) { \
					flag = 1; \
					num = (num * 10) + (*p++ - '0'); \
				}

#define GET_STRING(flag, num) tmpstr = name; \
				tmpstr[0] = '\0'; \
				if (!*p && argc && isalnum(**argv)) \
					p = *argv++, --argc; \
				while (isalnum(*p) && (tmpstr - name) < 31) { \
					tmpstr[0] = *p++; \
					tmpstr[1] = '\0'; \
					++tmpstr; \
				} \
				num = translate_xfermode(name); \
				if (num == -1) \
					flag = 0; \
				else \
					flag = 1;

#ifdef CONFIG_FEATURE_HDPARM_GET_IDENTITY
static int fromhex (unsigned char c)
{
	if (c >= 'a' && c <= 'f')
		return 10 + (c - 'a');
	if (c >= '0' && c <= '9')
		return (c - '0');
	bb_error_msg_and_die("bad char: '%c' 0x%02x", c, c);
}

static int identify_from_stdin (void)
{
	unsigned short sbuf[800];
	unsigned char  buf[1600], *b = (unsigned char *)buf;
	int i, count = read(0, buf, 1280);

	if (count != 1280)
		bb_error_msg_and_die("read(1280 bytes) failed (rc=%d)", count);
	for (i = 0; count >= 4; ++i) {
		sbuf[i] = (fromhex(b[0]) << 12) | (fromhex(b[1]) << 8) | (fromhex(b[2]) << 4) | fromhex(b[3]);
		__le16_to_cpus((uint16_t *)(&sbuf[i]));
		b += 5;
		count -= 5;
	}
	identify(sbuf, NULL);
	return 0;
}
#endif

/* our main() routine: */
int hdparm_main(int argc, char **argv)
{
	char c, *p;
	char *tmpstr;
	char name[32];
	int neg;

	++argv;
	if (!--argc)
		bb_show_usage();
	while (argc--) {
#ifdef CONFIG_FEATURE_HDPARM_GET_IDENTITY
		if (!strcmp("-Istdin", *argv)) {
			exit(identify_from_stdin());
		}
#endif
		p = *argv++;
		if (*p == '-') {
			if (!*++p)
				bb_show_usage();
			while ((c = *p++)) {
				++flagcount;
				switch (c) {
					case 'V':
						bb_error_msg_and_die("%s", VERSION);
					case 'v':
						verbose = 1;
						break;
#ifdef CONFIG_FEATURE_HDPARM_GET_IDENTITY
					case 'I':
						get_IDentity = 1;
						break;
					case 'i':
						get_identity = 1;
						break;
#endif
					case 'g':
						get_geom = 1;
						break;
					case 'f':
						do_flush = 1;
						break;
					case 'q':
						quiet = 1;
						noisy = 0;
						break;
					case 'u':
						get_unmask = noisy;
						noisy = 1;
						if (!*p && argc && isdigit(**argv))
							p = *argv++, --argc;
						if (*p == '0' || *p == '1') {
							set_unmask = 1;
							unmask = *p++ - '0';
						}
						break;
					case 'd':
						get_dma = noisy;
						noisy = 1;
						if (!*p && argc && isdigit(**argv))
							p = *argv++, --argc;
						if (*p >= '0' && *p <= '9') {
							set_dma = 1;
							dma = *p++ - '0';
						}
						break;
					case 'n':
						get_nowerr = noisy;
						noisy = 1;
						if (!*p && argc && isdigit(**argv))
							p = *argv++, --argc;
						if (*p == '0' || *p == '1') {
							set_nowerr = 1;
							nowerr = *p++ - '0';
						}
						break;
					case 'p':
						noisy_piomode = noisy;
						noisy = 1;
						GET_STRING(set_piomode,piomode);
						break;
					case 'r':
						get_readonly = noisy;
						noisy = 1;
						if (!*p && argc && isdigit(**argv))
							p = *argv++, --argc;
						if (*p == '0' || *p == '1') {
							set_readonly = 1;
							readonly = *p++ - '0';
						}
						break;
					case 'm':
						get_mult = noisy;
						noisy = 1;
						GET_NUMBER(set_mult,mult);
						break;
					case 'c':
						get_io32bit = noisy;
						noisy = 1;
						GET_NUMBER(set_io32bit,io32bit);
						break;
#ifdef HDIO_DRIVE_CMD
					case 'S':
						get_standby = noisy;
						noisy = 1;
						GET_NUMBER(set_standby,standby_requested);
						if (!set_standby)
							bb_error_msg("-S: missing value");
						break;

					case 'D':
						get_defects = noisy;
						noisy = 1;
						GET_NUMBER(set_defects,defects);
						if (!set_defects)
							bb_error_msg("-D: missing value");
						break;
					case 'P':
						get_prefetch = noisy;
						noisy = 1;
						GET_NUMBER(set_prefetch,prefetch);
						if (!set_prefetch)
							bb_error_msg("-P: missing value");
						break;

					case 'X':
						get_xfermode = noisy;
						noisy = 1;
						GET_STRING(set_xfermode,xfermode_requested);
						if (!set_xfermode)
							bb_error_msg("-X: missing value");
						break;

					case 'K':
						get_dkeep = noisy;
						noisy = 1;
						if (!*p && argc && isdigit(**argv))
							p = *argv++, --argc;
						if (*p == '0' || *p == '1') {
							set_dkeep = 1;
							dkeep = *p++ - '0';
						} else
							bb_error_msg("-K: missing value (0/1)");
						break;

					case 'A':
						get_lookahead = noisy;
						noisy = 1;
						if (!*p && argc && isdigit(**argv))
							p = *argv++, --argc;
						if (*p == '0' || *p == '1') {
							set_lookahead = 1;
							lookahead = *p++ - '0';
						} else
							bb_error_msg("-A: missing value (0/1)");
						break;

					case 'L':
						get_doorlock = noisy;
						noisy = 1;
						if (!*p && argc && isdigit(**argv))
							p = *argv++, --argc;
						if (*p == '0' || *p == '1') {
							set_doorlock = 1;
							doorlock = *p++ - '0';
						} else
							bb_error_msg("-L: missing value (0/1)");
						break;

					case 'W':
						get_wcache = noisy;
						noisy = 1;
						if (!*p && argc && isdigit(**argv))
							p = *argv++, --argc;
						if (*p == '0' || *p == '1') {
							set_wcache = 1;
							wcache = *p++ - '0';
						} else
							bb_error_msg("-W: missing value (0/1)");
						break;

					case 'C':
						get_powermode = noisy;
						noisy = 1;
						break;

					case 'y':
						get_standbynow = noisy;
						noisy = 1;
						set_standbynow = 1;
						break;

					case 'Y':
						get_sleepnow = noisy;
						noisy = 1;
						set_sleepnow = 1;
						break;

					case 'z':
						reread_partn = 1;
						break;

					case 'Z':
						get_seagate = noisy;
						noisy = 1;
						set_seagate = 1;
						break;
#endif /* HDIO_DRIVE_CMD */
					case 'k':
						get_keep = noisy;
						noisy = 1;
						if (!*p && argc && isdigit(**argv))
							p = *argv++, --argc;
						if (*p == '0' || *p == '1') {
							set_keep = 1;
							keep = *p++ - '0';
						}
						break;
#ifdef CONFIG_FEATURE_HDPARM_HDIO_UNREGISTER_HWIF
					case 'U':
						if (!*p && argc && isdigit(**argv))
							p = *argv++, --argc;

						if(! p) {
							bb_error_msg_and_die("expected hwif_nr");
						}

						sscanf(p++, "%i", &hwif);

						unregister_hwif = 1;
						break;
#endif /* CONFIG_FEATURE_HDPARM_HDIO_UNREGISTER_HWIF */
#ifdef CONFIG_FEATURE_HDPARM_HDIO_SCAN_HWIF
					case 'R':
						if (!*p && argc && isdigit(**argv))
							p = *argv++, --argc;

						if(! p) {
							bb_error_msg_and_die("expected hwif_data");
						}

						sscanf(p++, "%i", &hwif_data);

						if (argc && isdigit(**argv))
							p = *argv++, --argc;
						else {
							bb_error_msg_and_die("expected hwif_ctrl");
						}

						sscanf(p, "%i", &hwif_ctrl);

						if (argc && isdigit(**argv))
							p = *argv++, --argc;
						else {
							bb_error_msg_and_die("expected hwif_irq");
						}

						sscanf(p, "%i", &hwif_irq);

						*p = '\0';

						scan_hwif = 1;
						break;
#endif /* CONFIG_FEATURE_HDPARM_HDIO_SCAN_HWIF */
					case 'Q':
						get_dma_q = noisy;
						noisy = 1;
						neg = 0;
						GET_NUMBER(set_dma_q, dma_q);
						if (neg)
							dma_q = -dma_q;
						break;
#ifdef CONFIG_FEATURE_HDPARM_HDIO_DRIVE_RESET
					case 'w':
						perform_reset = 1;
						break;
#endif /* CONFIG_FEATURE_HDPARM_HDIO_DRIVE_RESET */
#ifdef CONFIG_FEATURE_HDPARM_HDIO_TRISTATE_HWIF
					case 'x':
						if (!*p && argc && isdigit(**argv))
							p = *argv++, --argc;
						if (*p == '0' || *p == '1') {
							perform_tristate = 1;
							tristate = *p++ - '0';
						} else
							bb_error_msg("-x: missing value (0/1)");
						break;

#endif /* CONFIG_FEATURE_HDPARM_HDIO_TRISTATE_HWIF */
					case 'a':
						get_readahead = noisy;
						noisy = 1;
						GET_NUMBER(set_readahead,readahead);
						break;
					case 'B':
						get_apmmode = noisy;
						noisy = 1;
						GET_NUMBER(set_apmmode,apmmode);
						if (!set_apmmode)
						printf("-B: missing value (1-255)");
						break;
					case 't':
						do_timings = 1;
						do_flush = 1;
						break;
					case 'T':
						do_ctimings = 1;
						do_flush = 1;
						break;
#ifdef CONFIG_FEATURE_HDPARM_HDIO_TRISTATE_HWIF
					case 'b':
						get_busstate = noisy;
						noisy = 1;
						if (!*p && argc && isdigit(**argv))
							p = *argv++, --argc;
						switch (*p) {
							case '0':
							case '1':
							case '2':
								set_busstate = 1;
								busstate = *p++ - '0';
								break;
							default:
								break;
						}
						break;
#endif
					case 'h':
					default:
						bb_show_usage();
				}
			}
			if (!argc)
				bb_show_usage();
		} else {
			process_dev (p);
		}
	}
	return 0 ;
}
