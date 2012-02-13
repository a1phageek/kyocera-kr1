/*
 * Copyright (c) 2002-2004 Sam Leffler, Errno Consulting
 * Copyright (c) 2002-2004 Atheros Communications, Inc.
 * All rights reserved.
 *
 * This module contains the regulatory domain table and accessor functions
 * for the information in the table.
 * The channel list creation is also contained in this module.
 *
 * "The country table and respective Regulatory Domain channel and power
 * settings are based on available knowledge as of software release. The
 * underlying global regulatory and spectrum rules change on a regular basis,
 * therefore, no warranty is given that the channel and power information
 * herein is complete, accurate or up to date.  Developers are responsible
 * for regulatory compliance of end-products developed using the enclosed
 * data per all applicable national requirements.  Furthermore, data in this
 * table does not guarantee that spectrum is available and that regulatory
 * approval is possible in every case. Knowldegable regulatory compliance
 * or government contacts should be consulted by the manufacturer to ensure
 * that the most current and accurate settings are used in each end-product.
 * This table was designed so that developers are able to update the country
 * table mappings as well as the Regulatory Domain definitions in order to
 * incorporate the most current channel and power settings in the end-product."
 *
 * $Id: //depot/sw/releases/SuperG_Linux_AP/src/802_11/madwifi/hal/ah_regdomain.c#3 $
 * $Atheros: //depot/sw/src/common/wlanchannel.c#258 $
 */
#include "opt_ah.h"

#include "ah.h"
#include "ah_internal.h"

/*
 * XXX this code needs a audit+review
 */

/* used throughout this file... */
#define	N(a)	(sizeof (a) / sizeof (a[0]))

/*
 * Country/Region Codes from MS WINNLS.H
 * Numbering from ISO 3166
 */
enum CountryCode {
    CTRY_ALBANIA              = 8,       /* Albania */
    CTRY_ALGERIA              = 12,      /* Algeria */
    CTRY_ARGENTINA            = 32,      /* Argentina */
    CTRY_ARMENIA              = 51,      /* Armenia */
    CTRY_AUSTRALIA            = 36,      /* Australia */
    CTRY_AUSTRIA              = 40,      /* Austria */
    CTRY_AZERBAIJAN           = 31,      /* Azerbaijan */
    CTRY_BAHRAIN              = 48,      /* Bahrain */
    CTRY_BELARUS              = 112,     /* Belarus */
    CTRY_BELGIUM              = 56,      /* Belgium */
    CTRY_BELIZE               = 84,      /* Belize */
    CTRY_BOLIVIA              = 68,      /* Bolivia */
    CTRY_BRAZIL               = 76,      /* Brazil */
    CTRY_BRUNEI_DARUSSALAM    = 96,      /* Brunei Darussalam */
    CTRY_BULGARIA             = 100,     /* Bulgaria */
    CTRY_CANADA               = 124,     /* Canada */
    CTRY_CHILE                = 152,     /* Chile */
    CTRY_CHINA                = 156,     /* People's Republic of China */
    CTRY_COLOMBIA             = 170,     /* Colombia */
    CTRY_COSTA_RICA           = 188,     /* Costa Rica */
    CTRY_CROATIA              = 191,     /* Croatia */
    CTRY_CYPRUS               = 196,
    CTRY_CZECH                = 203,     /* Czech Republic */
    CTRY_DENMARK              = 208,     /* Denmark */
    CTRY_DOMINICAN_REPUBLIC   = 214,     /* Dominican Republic */
    CTRY_ECUADOR              = 218,     /* Ecuador */
    CTRY_EGYPT                = 818,     /* Egypt */
    CTRY_EL_SALVADOR          = 222,     /* El Salvador */
    CTRY_ESTONIA              = 233,     /* Estonia */
    CTRY_FAEROE_ISLANDS       = 234,     /* Faeroe Islands */
    CTRY_FINLAND              = 246,     /* Finland */
    CTRY_FRANCE               = 250,     /* France */
    CTRY_FRANCE2              = 255,     /* France2 */
    CTRY_GEORGIA              = 268,     /* Georgia */
    CTRY_GERMANY              = 276,     /* Germany */
    CTRY_GREECE               = 300,     /* Greece */
    CTRY_GUATEMALA            = 320,     /* Guatemala */
    CTRY_HONDURAS             = 340,     /* Honduras */
    CTRY_HONG_KONG            = 344,     /* Hong Kong S.A.R., P.R.C. */
    CTRY_HUNGARY              = 348,     /* Hungary */
    CTRY_ICELAND              = 352,     /* Iceland */
    CTRY_INDIA                = 356,     /* India */
    CTRY_INDONESIA            = 360,     /* Indonesia */
    CTRY_IRAN                 = 364,     /* Iran */
    CTRY_IRAQ                 = 368,     /* Iraq */
    CTRY_IRELAND              = 372,     /* Ireland */
    CTRY_ISRAEL               = 376,     /* Israel */
    CTRY_ITALY                = 380,     /* Italy */
    CTRY_JAMAICA              = 388,     /* Jamaica */
    CTRY_JAPAN                = 392,     /* Japan */
    CTRY_JAPAN1               = 393,     /* Japan (JP1) */
    CTRY_JAPAN2               = 394,     /* Japan (JP0) */
    CTRY_JAPAN3               = 395,     /* Japan (JP1-1) */
    CTRY_JAPAN4               = 396,     /* Japan (JE1) */
    CTRY_JAPAN5               = 397,     /* Japan (JE2) */
    CTRY_JORDAN               = 400,     /* Jordan */
    CTRY_KAZAKHSTAN           = 398,     /* Kazakhstan */
    CTRY_KENYA                = 404,     /* Kenya */
    CTRY_KOREA_NORTH          = 408,     /* North Korea */
    CTRY_KOREA_ROC            = 410,     /* South Korea */
    CTRY_KOREA_ROC2           = 411,     /* South Korea */
    CTRY_KUWAIT               = 414,     /* Kuwait */
    CTRY_LATVIA               = 428,     /* Latvia */
    CTRY_LEBANON              = 422,     /* Lebanon */
    CTRY_LIBYA                = 434,     /* Libya */
    CTRY_LIECHTENSTEIN        = 438,     /* Liechtenstein */
    CTRY_LITHUANIA            = 440,     /* Lithuania */
    CTRY_LUXEMBOURG           = 442,     /* Luxembourg */
    CTRY_MACAU                = 446,     /* Macau */
    CTRY_MACEDONIA            = 807,     /* the Former Yugoslav Republic of Macedonia */
    CTRY_MALAYSIA             = 458,     /* Malaysia */
    CTRY_MEXICO               = 484,     /* Mexico */
    CTRY_MONACO               = 492,     /* Principality of Monaco */
    CTRY_MOROCCO              = 504,     /* Morocco */
    CTRY_NETHERLANDS          = 528,     /* Netherlands */
    CTRY_NEW_ZEALAND          = 554,     /* New Zealand */
    CTRY_NICARAGUA            = 558,     /* Nicaragua */
    CTRY_NORWAY               = 578,     /* Norway */
    CTRY_OMAN                 = 512,     /* Oman */
    CTRY_PAKISTAN             = 586,     /* Islamic Republic of Pakistan */
    CTRY_PANAMA               = 591,     /* Panama */
    CTRY_PARAGUAY             = 600,     /* Paraguay */
    CTRY_PERU                 = 604,     /* Peru */
    CTRY_PHILIPPINES          = 608,     /* Republic of the Philippines */
    CTRY_POLAND               = 616,     /* Poland */
    CTRY_PORTUGAL             = 620,     /* Portugal */
    CTRY_PUERTO_RICO          = 630,     /* Puerto Rico */
    CTRY_QATAR                = 634,     /* Qatar */
    CTRY_ROMANIA              = 642,     /* Romania */
    CTRY_RUSSIA               = 643,     /* Russia */
    CTRY_SAUDI_ARABIA         = 682,     /* Saudi Arabia */
    CTRY_SINGAPORE            = 702,     /* Singapore */
    CTRY_SLOVAKIA             = 703,     /* Slovak Republic */
    CTRY_SLOVENIA             = 705,     /* Slovenia */
    CTRY_SOUTH_AFRICA         = 710,     /* South Africa */
    CTRY_SPAIN                = 724,     /* Spain */
    CTRY_SWEDEN               = 752,     /* Sweden */
    CTRY_SWITZERLAND          = 756,     /* Switzerland */
    CTRY_SYRIA                = 760,     /* Syria */
    CTRY_TAIWAN               = 158,     /* Taiwan */
    CTRY_THAILAND             = 764,     /* Thailand */
    CTRY_TRINIDAD_Y_TOBAGO    = 780,     /* Trinidad y Tobago */
    CTRY_TUNISIA              = 788,     /* Tunisia */
    CTRY_TURKEY               = 792,     /* Turkey */
    CTRY_UAE                  = 784,     /* U.A.E. */
    CTRY_UKRAINE              = 804,     /* Ukraine */
    CTRY_UNITED_KINGDOM       = 826,     /* United Kingdom */
    CTRY_UNITED_STATES        = 840,     /* United States */
    CTRY_URUGUAY              = 858,     /* Uruguay */
    CTRY_UZBEKISTAN           = 860,     /* Uzbekistan */
    CTRY_VENEZUELA            = 862,     /* Venezuela */
    CTRY_VIET_NAM             = 704,     /* Viet Nam */
    CTRY_YEMEN                = 887,     /* Yemen */
    CTRY_ZIMBABWE             = 716      /* Zimbabwe */
};

/* Enumerated Regulatory Domain Information */
enum EnumRd {
	/*
	 * The following regulatory domain definitions are
	 * found in the EEPROM. Each regulatory domain
	 * can operate in either a 5GHz or 2.4GHz wireless mode or
	 * both 5GHz and 2.4GHz wireless modes.
	 * In general, the value holds no special
	 * meaning and is used to decode into either specific
	 * 2.4GHz or 5GHz wireless mode for that particular
	 * regulatory domain.
	 */
	NO_ENUMRD	= 0x00,
	NULL1_WORLD	= 0x03,		/* For 11b-only countries (no 11a allowed) */
	NULL1_ETSIB	= 0x07,		/* Israel */
	NULL1_ETSIC	= 0x08,
	FCC1_FCCA	= 0x10,		/* USA */
	FCC1_WORLD	= 0x11,		/* Hong Kong */

	FCC2_FCCA	= 0x20,		/* Canada */
	FCC2_WORLD	= 0x21,		/* Australia & HK */
	FCC2_ETSIC	= 0x22,
	FRANCE_RES	= 0x31,		/* Legacy France for OEM */
	FCC3_FCCA	= 0x3A,		/* USA & Canada w/5470 band, 11h, DFS enabled */

	ETSI1_WORLD	= 0x37,
	ETSI3_ETSIA	= 0x32,		/* France (optional) */
	ETSI2_WORLD	= 0x35,		/* Hungary & others */
	ETSI3_WORLD	= 0x36,		/* France & others */
	ETSI4_WORLD	= 0x30,
	ETSI4_ETSIC	= 0x38,
	ETSI5_WORLD	= 0x39,
	ETSI6_WORLD	= 0x34,		/* Bulgaria */
	ETSI_RESERVED	= 0x33,		/* Reserved (Do not used) */

	MKK1_MKKA	= 0x40,		/* Japan (JP1) */
	MKK1_MKKB	= 0x41,		/* Japan (JP0) */
	APL4_WORLD	= 0x42,		/* Singapore */
	MKK2_MKKA	= 0x43,		/* Japan with 4.9G channels */
	APL_RESERVED	= 0x44,		/* Reserved (Do not used)  */
	APL2_WORLD	= 0x45,		/* Korea */
	APL2_APLC	= 0x46,
	APL3_WORLD	= 0x47,
	MKK1_FCCA	= 0x48,		/* Japan (JP1-1) */
	APL2_APLD	= 0x49,		/* Korea with 2.3G channels */
	MKK1_MKKA1	= 0x4A,		/* Japan (JE1) */
	MKK1_MKKA2	= 0x4B,		/* Japan (JE2) */

	APL1_WORLD	= 0x52,		/* Latin America */
	APL1_FCCA	= 0x53,
	APL1_APLA	= 0x54,
	APL1_ETSIC	= 0x55,
	APL2_ETSIC	= 0x56,		/* Venezuela */
	APL5_WORLD	= 0x58,		/* Chile */

	/*
	 * World mode SKUs
	 */
	WOR0_WORLD	= 0x60,		/* World0 (WO0 SKU) */
	WOR1_WORLD	= 0x61,		/* World1 (WO1 SKU) */
	WOR2_WORLD	= 0x62,		/* World2 (WO2 SKU) */
	WOR3_WORLD	= 0x63,		/* World3 (WO3 SKU) */
	WOR4_WORLD	= 0x64,		/* World4 (WO4 SKU) */	
	WOR5_ETSIC	= 0x65,		/* World5 (WO5 SKU) */    

	WOR01_WORLD	= 0x66,		/* World0-1 (WW0-1 SKU) */
	WOR02_WORLD	= 0x67,		/* World0-2 (WW0-2 SKU) */
	EU1_WORLD	= 0x68,		/* Same as World0-2 (WW0-2 SKU), except active scan ch1-13. No ch14 */

	WOR9_WORLD	= 0x69,		/* World9 (WO9 SKU) */	
	WORA_WORLD	= 0x6A,		/* WorldA (WOA SKU) */	

	/*
	 * Regulator domains ending in a number (e.g. APL1,
	 * MK1, ETSI4, etc) apply to 5GHz channel and power
	 * information.  Regulator domains ending in a letter
	 * (e.g. APLA, FCCA, etc) apply to 2.4GHz channel and
	 * power information.
	 */
	APL1		= 0x0150,	/* LAT & Asia */
	APL2		= 0x0250,	/* LAT & Asia */
	APL3		= 0x0350,	/* Taiwan */
	APL4		= 0x0450,	/* Singapore */
	APL5		= 0x0550,	/* Chile */

	ETSI1		= 0x0130,	/* Europe & others */
	ETSI2		= 0x0230,	/* Europe & others */
	ETSI3		= 0x0330,	/* Europe & others */
	ETSI4		= 0x0430,	/* Europe & others */
	ETSI5		= 0x0530,	/* Europe & others */
	ETSI6		= 0x0630,	/* Europe & others */
	ETSIA		= 0x0A30,	/* France */
	ETSIB		= 0x0B30,	/* Israel */
	ETSIC		= 0x0C30,	/* Latin America */

	FCC1		= 0x0110,	/* US & others */
	FCC2		= 0x0120,	/* Canada, Australia & New Zealand */
	FCC3		= 0x0160,	/* US w/new middle band & DFS */    
	FCCA		= 0x0A10,	 

	APLD		= 0x0D50,	/* South Korea */

	MKK1		= 0x0140,	/* Japan */
	MKK2		= 0x0240,	/* Japan Extended */
	MKKA		= 0x0A40,	/* Japan */

	NULL1		= 0x0198,
	WORLD		= 0x0199,
	DEBUG_REG_DMN	= 0x01ff
};

#define DEF_REGDMN		FCC1_FCCA
#define	COUNTRY_ERD_FLAG        0x8000
#define WORLDWIDE_ROAMING_FLAG  0x4000
#define	SUPER_DOMAIN_MASK	0x0fff
#define	COUNTRY_CODE_MASK	0x03ff
#define CF_INTERFERENCE		(CHANNEL_CW_INT | CHANNEL_RADAR_INT)
#define CHANNEL_14		(2484)	/* 802.11g operation is not permitted on channel 14 */
#define IS_11G_CH14(_ch,_cf) \
	(((_ch) == CHANNEL_14) && ((_cf) == CHANNEL_G))

typedef struct RegDmnLegacyNames {
	u_int8_t	regDmnName[8];
	HAL_REG_DOMAIN	regDmn;
	HAL_CTRY_CODE	countryCode;
} REG_DMN_LEGACY_NAMES;

static const REG_DMN_LEGACY_NAMES allLegacyNames[] = {
    {"NONE", NO_ENUMRD,   CTRY_DEBUG          },
    {"FCC",  FCC1_FCCA,   CTRY_UNITED_STATES  },
    {"MKK",  MKK1_MKKA,   CTRY_JAPAN          },
    {"ETSI", ETSI1_WORLD, CTRY_UNITED_KINGDOM }
};

typedef struct {
	HAL_CTRY_CODE	countryCode;	   
	HAL_REG_DOMAIN	regDmnEnum;
	const char*	isoName;
	const char*	name;
	HAL_BOOL	allow11g;
	HAL_BOOL	allow11aTurbo;
	HAL_BOOL	allow11gTurbo;
} COUNTRY_CODE_TO_ENUM_RD;
  
#define	YES	AH_TRUE
#define	NO	AH_FALSE
/* Index into table to avoid DEBUG and NO COUNTRY SET entries */
#define CTRY_ONLY_INDEX 2
/*
 * Country Code Table to Enumerated RD
 */
static const COUNTRY_CODE_TO_ENUM_RD allCountries[] = {
    {CTRY_DEBUG,       NO_ENUMRD,     "DB", "DEBUG",      YES, YES, YES    },
    {CTRY_DEFAULT,     DEF_REGDMN,    "NA", "NO_COUNTRY_SET", YES, YES, YES },
    {CTRY_ALBANIA,     NULL1_WORLD,   "AL", "ALBANIA",    YES, NO, YES     },
    {CTRY_ALGERIA,     NULL1_WORLD,   "DZ", "ALGERIA",    YES, NO, YES     },
    {CTRY_ARGENTINA,   APL3_WORLD,    "AR", "ARGENTINA",   NO, NO, NO      },
    {CTRY_ARMENIA,     ETSI4_WORLD,   "AM", "ARMENIA",    YES, NO, YES     },
    {CTRY_AUSTRALIA,   FCC2_WORLD,    "AU", "AUSTRALIA",  YES, YES, YES    },
    {CTRY_AUSTRIA,     ETSI5_WORLD,   "AT", "AUSTRIA",    YES, NO, YES     },
    {CTRY_AZERBAIJAN,  ETSI4_WORLD,   "AZ", "AZERBAIJAN", YES, YES, YES    },
    {CTRY_BAHRAIN,     NULL1_WORLD,   "BH", "BAHRAIN",    YES, NO, YES     },
    {CTRY_BELARUS,     NULL1_WORLD,   "BY", "BELARUS",    YES, NO, YES     },
    {CTRY_BELGIUM,     ETSI4_WORLD,   "BE", "BELGIUM",    YES, NO, YES     },
    {CTRY_BELIZE,      APL1_ETSIC,    "BZ", "BELIZE",     YES, YES, YES    },
    {CTRY_BOLIVIA,     APL1_ETSIC,    "BO", "BOLVIA",     YES, YES, YES    },
    {CTRY_BRAZIL,      NULL1_ETSIC,   "BR", "BRAZIL",      NO, NO, NO      },
    {CTRY_BRUNEI_DARUSSALAM,APL1_WORLD,"BN", "BRUNEI DARUSSALAM", YES, YES, YES },
    {CTRY_BULGARIA,    ETSI6_WORLD,   "BG", "BULGARIA",   YES, NO, YES     },
    {CTRY_CANADA,      FCC2_FCCA,     "CA", "CANADA",     YES, YES, YES    },
    {CTRY_CHILE,       APL5_WORLD,    "CL", "CHILE",      NO,  YES, NO     },
    {CTRY_CHINA,       APL1_WORLD,    "CN", "CHINA",      YES, YES, YES    },
    {CTRY_COLOMBIA,    FCC1_FCCA,     "CO", "COLOMBIA",   YES, NO, YES     },
    {CTRY_COSTA_RICA,  NULL1_WORLD,   "CR", "COSTA RICA", YES, NO, YES     },
    {CTRY_CROATIA,     ETSI3_WORLD,   "HR", "CROATIA",    YES, NO, YES     },
    {CTRY_CYPRUS,      ETSI1_WORLD,   "CY", "CYPRUS",     YES, YES, YES    },
    {CTRY_CZECH,       ETSI3_WORLD,   "CZ", "CZECH REPUBLIC", NO, NO, NO   },
    {CTRY_DENMARK,     ETSI1_WORLD,   "DK", "DENMARK",    YES, NO, YES     },
    {CTRY_DOMINICAN_REPUBLIC,FCC1_FCCA,"DO", "DOMINICAN REPUBLIC", YES, YES, YES },
    {CTRY_ECUADOR,     NULL1_WORLD,   "EC", "ECUADOR",     NO, NO,NO       },
    {CTRY_EGYPT,       NULL1_WORLD,   "EG", "EGYPT",      YES, NO, YES     },
    {CTRY_EL_SALVADOR, NULL1_WORLD,   "SV", "EL SALVADOR",YES, NO, YES     },    
    {CTRY_ESTONIA,     ETSI1_WORLD,   "EE", "ESTONIA",    YES, NO, YES     },
    {CTRY_FINLAND,     ETSI1_WORLD,   "FI", "FINLAND",    YES, NO, YES     },
    {CTRY_FRANCE,      ETSI3_WORLD,   "FR", "FRANCE",     YES, NO, YES     },
    {CTRY_FRANCE2,     ETSI3_WORLD,   "F2", "FRANCE_RES", YES, NO, YES     },
    {CTRY_GEORGIA,     ETSI4_WORLD,   "GE", "GEORGIA",    YES, YES, YES    },
    {CTRY_GERMANY,     ETSI1_WORLD,   "DE", "GERMANY",    YES, NO, YES     },
    {CTRY_GREECE,      NULL1_WORLD,   "GR", "GREECE",     YES, NO, YES     },
    {CTRY_GUATEMALA,   FCC1_FCCA,     "GT", "GUATEMALA",  YES, YES, YES    },
    {CTRY_HONDURAS,    NULL1_WORLD,   "HN", "HONDURAS",   YES, NO, YES     },
    {CTRY_HONG_KONG,   FCC2_WORLD,    "HK", "HONG KONG",  YES, YES, YES    },
    {CTRY_HUNGARY,     ETSI2_WORLD,   "HU", "HUNGARY",    YES, NO, YES     },
    {CTRY_ICELAND,     ETSI1_WORLD,   "IS", "ICELAND",    YES, NO, YES     },
    {CTRY_INDIA,       NULL1_WORLD,   "IN", "INDIA",      YES, NO, YES     },
    {CTRY_INDONESIA,   NULL1_WORLD,   "ID", "INDONESIA",  YES, NO, YES     },
    {CTRY_IRAN,        APL1_WORLD,    "IR", "IRAN",       YES, YES, YES    },
    {CTRY_IRELAND,     ETSI1_WORLD,   "IE", "IRELAND",    YES, NO, YES     },
    {CTRY_ISRAEL,      NULL1_WORLD,   "IL", "ISRAEL",     YES, NO, YES     },
    {CTRY_ITALY,       ETSI1_WORLD,   "IT", "ITALY",      YES, NO, YES     },
    {CTRY_JAPAN,       MKK1_MKKA,     "JP", "JAPAN",      YES, NO, NO      },
    {CTRY_JAPAN1,      MKK1_MKKB,     "J1", "JAPAN1",     YES, NO, NO      },
    {CTRY_JAPAN2,      MKK1_FCCA,     "J2", "JAPAN2",     YES, NO, NO      },    
    {CTRY_JAPAN3,      MKK2_MKKA,     "J3", "JAPAN3",     YES, NO, NO      },
    {CTRY_JAPAN4,      MKK1_MKKA1,    "J4", "JAPAN4",     YES, NO, NO      },
    {CTRY_JAPAN5,      MKK1_MKKA2,    "J5", "JAPAN5",     YES, NO, NO      },    
    {CTRY_JORDAN,      NULL1_WORLD,   "JO", "JORDAN",     YES, NO, YES     },
    {CTRY_KAZAKHSTAN,  NULL1_WORLD,   "KZ", "KAZAKHSTAN", YES, NO, YES     },
    {CTRY_KOREA_NORTH, APL2_WORLD,    "KP", "NORTH KOREA",YES,YES, YES     },
    {CTRY_KOREA_ROC,   APL2_WORLD,    "KR", "KOREA REPUBLIC", YES, NO, NO  },
    {CTRY_KOREA_ROC2,  APL2_APLD,     "K2", "KOREA REPUBLIC2", YES, NO, NO },
    {CTRY_KUWAIT,      NULL1_WORLD,   "KW", "KUWAIT",     YES, NO, YES     },
    {CTRY_LATVIA,      NULL1_WORLD,   "LV", "LATVIA",     YES, NO, YES     },
    {CTRY_LEBANON,     NULL1_WORLD,   "LB", "LEBANON",    YES, NO, YES     },
    {CTRY_LIECHTENSTEIN,ETSI2_WORLD,  "LI", "LIECHTENSTEIN", YES, NO, YES  },
    {CTRY_LITHUANIA,   ETSI1_WORLD,   "LT", "LITHUANIA",  YES, NO, YES     },
    {CTRY_LUXEMBOURG,  ETSI1_WORLD,   "LU", "LUXEMBOURG", YES, NO, YES     },
    {CTRY_MACAU,       FCC2_WORLD,    "MO", "MACAU",      YES, YES, YES    },
    {CTRY_MACEDONIA,   NULL1_WORLD,   "MK", "MACEDONIA",  YES, NO, YES     },
    {CTRY_MALAYSIA,    NULL1_WORLD,   "MY", "MALAYSIA",    NO, NO, NO      },
    {CTRY_MEXICO,      FCC1_FCCA,     "MX", "MEXICO",     YES, YES, YES    },
    {CTRY_MONACO,      ETSI4_WORLD,   "MC", "MONACO",     YES, YES, YES    },
    {CTRY_MOROCCO,     NULL1_WORLD,   "MA", "MOROCCO",    YES, NO, YES     },
    {CTRY_NETHERLANDS, ETSI1_WORLD,   "NL", "NETHERLANDS",YES, NO, YES     },
    {CTRY_NEW_ZEALAND, FCC2_ETSIC,    "NZ", "NEW ZEALAND",YES, NO, YES     },
    {CTRY_NORWAY,      ETSI1_WORLD,   "NO", "NORWAY",     YES, NO, YES     },
    {CTRY_OMAN,        NULL1_WORLD,   "OM", "OMAN",       YES, NO, YES     },
    {CTRY_PAKISTAN,    NULL1_WORLD,   "PK", "PAKISTAN",   YES, NO, YES     },
    {CTRY_PANAMA,      FCC1_FCCA,     "PA", "PANAMA",     YES, YES, YES    },
    {CTRY_PERU,        NULL1_WORLD,   "PE", "PERU",       YES, NO, YES     },
    {CTRY_PHILIPPINES, FCC1_WORLD,    "PH", "PHILIPPINES",YES, YES, YES    },
    {CTRY_POLAND,      ETSI1_WORLD,   "PL", "POLAND",     YES, NO, YES     },
    {CTRY_PORTUGAL,    ETSI1_WORLD,   "PT", "PORTUGAL",   YES, NO, YES     },
    {CTRY_PUERTO_RICO, FCC1_FCCA,     "PR", "PUERTO RICO",YES, YES, YES    },
    {CTRY_QATAR,       NULL1_WORLD,   "QA", "QATAR",      YES, NO, YES     },
    {CTRY_ROMANIA,     NULL1_WORLD,   "RO", "ROMANIA",    YES, NO, YES     },
    {CTRY_RUSSIA,      NULL1_WORLD,   "RU", "RUSSIA",     YES, NO, YES     },
    {CTRY_SAUDI_ARABIA,NULL1_WORLD,   "SA", "SAUDI ARABIA", YES, NO, YES   },
    {CTRY_SINGAPORE,   APL4_WORLD,    "SG", "SINGAPORE",  YES, YES, YES    },
    {CTRY_SLOVAKIA,    ETSI3_WORLD,   "SK", "SLOVAK REPUBLIC", YES, NO, YES },
    {CTRY_SLOVENIA,    ETSI1_WORLD,   "SI", "SLOVENIA",   YES, NO, YES     },
    {CTRY_SOUTH_AFRICA,ETSI1_WORLD,   "ZA", "SOUTH AFRICA", YES, YES, YES  },
    {CTRY_SPAIN,       ETSI1_WORLD,   "ES", "SPAIN",      YES, NO, YES     },
    {CTRY_SWEDEN,      ETSI1_WORLD,   "SE", "SWEDEN",     YES, NO, YES     },
    {CTRY_SWITZERLAND, ETSI2_WORLD,   "CH", "SWITZERLAND",YES, NO, YES     },
    {CTRY_SYRIA,       NULL1_WORLD,   "SY", "SYRIA",      YES, NO, YES     },
    {CTRY_TAIWAN,      APL3_WORLD,    "TW", "TAIWAN",     YES, YES, YES    },
    {CTRY_THAILAND,    APL2_WORLD,    "TH", "THAILAND",   YES, YES, YES    },
    {CTRY_TRINIDAD_Y_TOBAGO,ETSI4_WORLD,"TT", "TRINIDAD & TOBAGO", YES, NO, YES },
    {CTRY_TUNISIA,     ETSI3_WORLD,   "TN", "TUNISIA",    YES, NO, YES     },
    {CTRY_TURKEY,      ETSI3_WORLD,   "TR", "TURKEY",     YES, NO, YES     },
    {CTRY_UKRAINE,     NULL1_WORLD,   "UA", "UKRAINE",    YES, NO, YES     },
    {CTRY_UAE,         NULL1_WORLD,   "AE", "UNITED ARAB EMIRATES", YES, NO, YES },
    {CTRY_UNITED_KINGDOM, ETSI1_WORLD, "GB", "UNITED KINGDOM", YES, NO, YES },
    {CTRY_UNITED_STATES, FCC1_FCCA,   "US", "UNITED STATES", YES, YES, YES },
    {CTRY_URUGUAY,     APL2_WORLD,    "UY", "URUGUAY",    YES, NO, YES     },
    {CTRY_UZBEKISTAN,  FCC3_FCCA,     "UZ", "UZBEKISTAN", YES, YES, YES    },    
    {CTRY_VENEZUELA,   APL2_ETSIC,    "VE", "VENEZUELA",  YES, NO, YES     },
    {CTRY_VIET_NAM,    NULL1_WORLD,   "VN", "VIET NAM",   YES, NO, YES     },
    {CTRY_YEMEN,       NULL1_WORLD,   "YE", "YEMEN",      YES, NO, YES     },
    {CTRY_ZIMBABWE,    NULL1_WORLD,   "ZW", "ZIMBABWE",   YES, NO, YES     }    
};
#undef	YES
#undef	NO

typedef struct {
	u_int16_t	regDmnEnum;
	u_int16_t	regDmnWirelessMode5;
	u_int16_t	regDmnWirelessMode2;
} ENUM_RD_TO_WIRELESS_MODE_RD;

static const ENUM_RD_TO_WIRELESS_MODE_RD allEnumRds[] = {
    {NO_ENUMRD,     DEBUG_REG_DMN, DEBUG_REG_DMN},
    {NULL1_WORLD,   NULL1,   WORLD   },
    {NULL1_ETSIB,   NULL1,   ETSIB   },
    {NULL1_ETSIC,   NULL1,   ETSIC   },

    {FCC2_FCCA,     FCC2,    FCCA    },
    {FCC2_WORLD,    FCC2,    WORLD   },
    {FCC2_ETSIC,    FCC2,    ETSIC   },

    {FCC3_FCCA,     FCC3,    FCCA    },
    {ETSI1_WORLD,   ETSI1,   WORLD   },
    {ETSI2_WORLD,   ETSI2,   WORLD   },
    {ETSI3_WORLD,   ETSI3,   WORLD   },
    {ETSI4_WORLD,   ETSI4,   WORLD   },
    {ETSI5_WORLD,   ETSI5,   WORLD   },
    {ETSI6_WORLD,   ETSI6,   WORLD   },

    {ETSI3_ETSIA,   ETSI3,   WORLD   },
    {FRANCE_RES,    ETSI3,   WORLD   },

    {FCC1_WORLD,    FCC1,    WORLD   },
    {FCC1_FCCA,     FCC1,    FCCA    },

    {APL1_WORLD,    APL1,    WORLD   },
    {APL2_WORLD,    APL2,    WORLD   },
    {APL3_WORLD,    APL3,    WORLD   },
    {APL4_WORLD,    APL4,    WORLD   },
    {APL5_WORLD,    APL5,    WORLD   },    

    {APL1_ETSIC,    APL1,    ETSIC   },
    {APL2_ETSIC,    APL2,    ETSIC   },
    {APL2_APLD,     APL2,    APLD    },

    {MKK1_MKKA,     MKK1,    MKKA    },
    {MKK1_MKKB,     MKK1,    MKKA    },
    {MKK2_MKKA,     MKK2,    MKKA    },
    {MKK1_FCCA,     MKK1,    FCCA    },    
    {MKK1_MKKA1,    MKK1,    MKKA    },
    {MKK1_MKKA2,    MKK1,    MKKA    },

    /*
     * The area below is reserved for super domains
     */
    {WOR0_WORLD,    WOR0_WORLD,   WOR0_WORLD    },
    {WOR1_WORLD,    WOR1_WORLD,   WOR1_WORLD    },
    {WOR2_WORLD,    WOR2_WORLD,   WOR2_WORLD    },
    {WOR3_WORLD,    WOR3_WORLD,   WOR3_WORLD    },
    {WOR4_WORLD,    WOR4_WORLD,   WOR4_WORLD    },
    {WOR5_ETSIC,    WOR5_ETSIC,   WOR5_ETSIC    },
    {WOR01_WORLD,   WOR01_WORLD,  WOR01_WORLD   },
    {WOR02_WORLD,   WOR02_WORLD,  WOR02_WORLD   },
    {EU1_WORLD,     EU1_WORLD,    EU1_WORLD     },
    {WOR9_WORLD,    WOR9_WORLD,   WOR9_WORLD    },
    {WORA_WORLD,    WORA_WORLD,   WORA_WORLD    },
};

#define TURBO_CHANNEL_SEPARATION    (40)
#define OFDM5_CHANNEL_SEPARATION    (20)
#define LOW2GHZ_CHANNEL_SEPARATION  (5)
#define HIGH2GHZ_CHANNEL_SEPARATION (20)
#define IS24DEBUG(ch)               (((ch) >= 2512) && ((ch) <= 2732))

/* 5 GHz table settings */
typedef struct {
	u_int16_t	lowChannel;	/* Low Channel center in MHz */
	u_int16_t	highChannel;	/* High Channel center in MHz */
	u_int8_t	powerDfs;	/* Maximum power (dBm) for this channel
					   range when using DFS */
	u_int8_t	antennaMax;	/* Maximum allowed antenna gain */
} REG_DMN_WM_CHANNEL_GROUP;

typedef struct {
	u_int16_t	regDmnWirelessMode;
	u_int16_t	entries;
	u_int8_t	conformanceTestLimit;
	REG_DMN_WM_CHANNEL_GROUP chanGroup[4];
} REG_DMN_WM_FREQ_TABLE;

enum {					/* conformance test limits */
	FCC	= 0x10,
	MKK	= 0x40,
	ETSI	= 0x30,
	NO_CTL	= 0xff,
	CTL_11B	= 1,
	CTL_11G	= 2,
	CTL_TURBO = 3,
	CTL_108G = 4
};

/*
 * Regulatory Domain Tables
 *
 * (Information is based on "Country Table 8-29-02.xls")
 * Tables have entries ordered with AP/Adhoc planting preference in mind.
 *  -Indoor channels are ordered before outdoor
 *  -Higher power level indoor channels are first
 */
static const REG_DMN_WM_FREQ_TABLE regDmnWm5[] = {
    {DEBUG_REG_DMN, 2, FCC, {
    {5120, 5700,  5,  6},
    {5745, 5825,  5,  6},
    {   0,    0,  0,  0},
    {   0,    0,  0,  0}}},

    {APL1, 1, FCC, {
    {5745, 5825, 30, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {APL2, 1, FCC, {
    {5745, 5805, 23, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {APL3, 2, FCC, {
    {5280, 5320, 17, 6},
    {5745, 5805, 30, 6},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {APL4, 2, FCC, {
    {5180, 5240, 20, 0},
    {5745, 5825, 20, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {APL5, 2, FCC, {
    {5745, 5825, 17, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    /*
     * The ETSI1 domain requires TPC
     * As the TPC specification are unfinished, 3dBi are
     * removed from each of the ETSI1 power selections
     */
    {ETSI1, 2, ETSI, {
    {5180, 5320, 20, 0},  /* Should be 23 with TPC */
    {5500, 5700, 27, 0},  /* Should be 30 with TPC */
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {ETSI2, 1, ETSI, {
    {5180, 5240, 18, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {ETSI3, 1, ETSI, {
    {5180, 5320, 20, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {ETSI4, 1, ETSI, {
    {5180, 5320, 18, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {ETSI5, 1, ETSI, {
    {5180, 5240, 15, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {ETSI6, 2, ETSI, {
    {5180, 5280, 23, 0},
    {5500, 5700, 30, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    /*
     * Values artificially decreased to meet FCC testing
     * procedures
     */
    {FCC1, 3, FCC, {
    {5260, 5320, 23, 6},
    {5180, 5240, 17, 6},
    {5745, 5825, 30, 6},
    {   0,    0,  0, 0}}},

    {FCC2, 2, FCC, {
    {5180, 5320, 23, 6},
    {5745, 5825, 30, 6},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {FCC3, 4, FCC, {
    {5180, 5240, 17, 6},
    {5260, 5320, 20, 6},
    {5500, 5700, 20, 6},
    {5745, 5825, 30, 6}}},

    {MKK1, 1, MKK, {
    {5170, 5230, 23, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {MKK2, 3, MKK, {
    {5170, 5230, 23, 0},
    {5040, 5080, 23, 0},
    {4920, 4980, 23, 0},
    {   0,    0,  0, 0}}}
};

static const REG_DMN_WM_FREQ_TABLE regDmnWm5Turbo[] = {
    {DEBUG_REG_DMN, 2, FCC | CTL_TURBO, {
    {5130, 5670,  5, 6},
    {5150, 5650,  5, 6},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {FCC1, 3, FCC | CTL_TURBO, {
    {5250, 5290, 23, 6},
    {5210, 5210, 17, 6},
    {5760, 5800, 30, 6},
    {   0,    0,  0, 0}}},

    {FCC3, 3, FCC | CTL_TURBO, {
    {5250, 5290, 23, 6},
    {5210, 5210, 17, 6},
    {5760, 5800, 30, 6},
    {   0,    0,  0, 0}}}
};

/* Special scan tables for new turbo prime interference checks */ 
#define NUM_PRIME_A_CHANNELS 9  /* number additional to static turbo channels */
static const REG_DMN_WM_FREQ_TABLE regDmnWm5TurboPrime[] = {
    {FCC3, 4, FCC | CTL_TURBO, {
    {5240, 5280, 23, 6},
    {5200, 5200, 17, 6},
    {5765, 5805, 30, 6},
    {5540, 5660, 30, 6}}}
};

/* This number is currently 0 since 'g' base ch same as turbo. */
#define NUM_PRIME_G_CHANNELS 0  /* number additional to static turbo channels */
static const REG_DMN_WM_FREQ_TABLE regDmnWm2TurboPrime[] = {
    {FCCA, 1, FCC | CTL_11B, {
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}}
};

static const REG_DMN_WM_FREQ_TABLE regDmnWm2Turbo[] = {
    {DEBUG_REG_DMN, 3, NO_CTL, {
    {2312, 2372,  5, 6},        /* Channel -19 to -7 */
    {2437, 2437,  5, 6},        /* Channel 6 only */
    {2512, 2732,  5, 6},        /* Channel 15 and higher  */
    {0,       0,  0, 0}}},      /* */

    {FCCA, 1, FCC | CTL_108G, {
    {2437, 2437, 20, 6},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {ETSIA, 1, ETSI | CTL_108G, {
    {2437, 2437, 20, 6},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {ETSIB, 1, ETSI | CTL_108G, {
    {2437, 2437, 20, 6},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {ETSIC, 1, ETSI | CTL_108G, {
    {2437, 2437, 20, 6}, 
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {MKKA, 1, MKK | CTL_108G, {
    {2437, 2437, 20, 6}, 
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {WORLD, 1, ETSI | CTL_108G, {
    {2437, 2437, 20, 6}, 
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}}
    
};

static const REG_DMN_WM_FREQ_TABLE regDmnWm2[] = {
    {DEBUG_REG_DMN, 4, NO_CTL, {
    {2312, 2372,  5, 6},      /* Channel -19 to -7 */
    {2412, 2472,  5, 6},
    {2484, 2484,  5, 6},
    {2512, 2732,  5, 6}}},

    {APLD, 2, NO_CTL, {
    {2412, 2472, 20, 0},        /* Channel 1 - 13 */
    {2312, 2372, 20, 0},        /* Negative Channels */
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {ETSIA, 1, NO_CTL, {
    {2457, 2472, 20, 0},        /* Channel 10 - 13 */
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {ETSIB, 1, ETSI | CTL_11B, {
    {2432, 2442, 20, 0},        /* Channel 5 - 7 */
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {ETSIC, 1, ETSI | CTL_11B, {
    {2412, 2472, 30, 0},        /* Channel 1 - 13 */
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {FCCA, 1, FCC | CTL_11B, {
    {2412, 2462, 27, 6},        /* Channel 1 - 11 */
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {MKKA, 2, MKK | CTL_11B, {
    {2412, 2472, 20, 0},        /* Channel 1 - 13 */
    {2484, 2484, 20, 0},        /* Channel 14 */
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},

    {WORLD, 1, ETSI | CTL_11B, {
    {2412, 2472, 20, 0},        /* Channel 1 - 13 */
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},
};

static const REG_DMN_WM_FREQ_TABLE regDmnNone[] = {
    {0, 0, NO_CTL, {
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0},
    {   0,    0,  0, 0}}},
};

/*
 * 802.11a Common Mode Power Levels
 */
typedef struct {
	u_int16_t	lchan;	/* Low channel */
	u_int16_t	hchan;	/* Highest channel */
	u_int8_t	pwrLvl;	/* power level in dBm */
} CM_PWR;

static CM_PWR cmPwrTbl[] = {
	{ 4900, 5000, 17 },
	{ 5000, 5100, 17 },
	{ 5150, 5250, 15 },	/* ETSI & MKK */
	{ 5250, 5350, 18 },	/* ETSI */
	{ 5470, 5725, 20 },	/* ETSI */
	{ 5725, 5825, 20 },	/* Singapore */
	{ 5825, 5850, 23 }	/* Korea */
};

#ifdef notyet
typedef struct {
	u_int16_t	rd;	/* 2.4 or 5GHz regulatory domain */
	u_int16_t	cflags;	/* 2.4 or 5GHz channel flag */
	u_int16_t	mode;	/* 2.4 or 5GHz wireless mode flag(s) */
} ADHOC_DISALLOW_RD;

/*
 * The following table defines the wireless mode that DOES
 * NOT supported adHoc network in its regulatory domain
 *
 * If adhoc network is not supported in 802.11a, then in
 * the same token, it won't supported the turbo mode also.
 */
static ADHOC_DISALLOW_RD adHocDisallowRdTbl[] = {
	{ WOR1_WORLD, CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ WOR2_WORLD, CHANNEL_T, HAL_MODE_11A                  },
	{ WOR4_WORLD, CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ WOR5_ETSIC, CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },    
	{ WOR9_WORLD, CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ WORA_WORLD, CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ ETSI1_WORLD,CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ ETSI2_WORLD,CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ ETSI3_WORLD,CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ ETSI3_ETSIA,CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ ETSI4_WORLD,CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ ETSI5_WORLD,CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ ETSI6_WORLD,CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ ETSI1,      CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ ETSI2,      CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ ETSI3,      CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ ETSI4,      CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ ETSI5,      CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ ETSI6,      CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ ETSIA,      CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ ETSIB,      CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ ETSIC,      CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ MKK1_MKKA,  CHANNEL_T,                HAL_MODE_TURBO },
	{ MKK2_MKKA,  CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ MKK1_MKKB,  CHANNEL_A, HAL_MODE_11A | HAL_MODE_TURBO },
	{ MKK1_FCCA,  CHANNEL_T,                HAL_MODE_TURBO },
	{ MKK1_MKKA1, CHANNEL_T,                HAL_MODE_TURBO },
	{ MKK1_MKKA2, CHANNEL_T,                HAL_MODE_TURBO },
	{ MKK1,       CHANNEL_T,                HAL_MODE_TURBO },
	{ MKK2,       CHANNEL_T,                HAL_MODE_TURBO },    
	{ MKKA,       CHANNEL_T,                HAL_MODE_TURBO }
};
#endif

/*
 * WorldWide Roaming Channels
 * The channels are organized in order of the passive scan.
 */
typedef u_int16_t ROAMING_CHANNEL;

typedef struct {
	u_int16_t	mode;
	ROAMING_CHANNEL	*pList;
} WWR_CLIST;

typedef ROAMING_CHANNEL    ADHOC_CLIST;

typedef struct {
	u_int16_t		domain;
	START_ADHOC_OPTION	adHocMode;
	ADHOC_CLIST		*adHocClist;  /* Ad Hoc default channel list */
	u_int16_t		rclTblSize;
	WWR_CLIST		*RclTbl[10];
} SUPER_DOMAIN;

static ROAMING_CHANNEL rcl1[] =         /* 8 FCC channel: 52, 56, 60, 64, 36, 40, 44, 48 */
{ 5260, 5280, 5300, 5320, 5180, 5200, 5220, 5240, 0 };
static ROAMING_CHANNEL rcl2[] =         /* 4 MKK channels: 34, 38, 42, 46 */
{ 5170, 5190, 5210, 5230, 0 };
static ROAMING_CHANNEL rcl3[] =         /* 2.4Ghz ch: 1,6,11,7,13 */
{ 2412, 2437, 2462, 2442, 2472, 0 };
static ROAMING_CHANNEL rcl3x[] =        /* 2.4Ghz ch: 1,6,11,7 */
{ 2412, 2437, 2462, 2442, 0 };
static ROAMING_CHANNEL rcl3a[] =        /* 2.4Ghz ch: 1,6,11,7 */
{ 2412, 2437, 2462, 2442, 0 };
static ROAMING_CHANNEL rcl3ax[] =       /* 2.4Ghz ch: 1,6,11,7 */
{ 2412, 2437, 2462, 2442, 0 };
static ROAMING_CHANNEL rcl4[] =         /* 5 FCC channel: 149, 153, 161, 165 */
{ 5745, 5765, 5785, 5805, 5825, 0 };
static ROAMING_CHANNEL rcl5[] =         /* 3 turbo channels */
{ 5210, 5250, 5290, 0 };
static ROAMING_CHANNEL rcl6[] =         /* 2 turbo channels */
{ 5760, 5800, 0 };
static ROAMING_CHANNEL rcl7[] =         /* 11 ETSI channel: 100,104,108,112,116,120,124,128,132,136,140 */
{ 5500, 5520, 5540, 5560, 5580, 5600, 5620, 5640, 5660, 5680, 5700, 0 };
static ROAMING_CHANNEL rcl8[] =         /* 2.4Ghz ch: 2,3,4,5,8,9,10,12 */
{ 2417, 2422, 2427, 2432, 2447, 2452, 2457, 2467, 0 };
static ROAMING_CHANNEL rcl8x[] =        /* 2.4Ghz ch: 2,3,4,5,8,9,10 */
{ 2417, 2422, 2427, 2432, 2447, 2452, 2457, 0 };
static ROAMING_CHANNEL rcl8a[] =        /* 2.4Ghz ch: 2,3,4,5,8,9,10 */
{ 2417, 2422, 2427, 2432, 2447, 2452, 2457, 0 };
static ROAMING_CHANNEL rcl8ax[] =       /* 2.4Ghz ch: 2,3,4,5,8,9,10 */
{ 2417, 2422, 2427, 2432, 2447, 2452, 2457, 0 };
static ROAMING_CHANNEL rcl9[] =         /* 2.4Ghz ch: 14 */
{ 2484, 0 };
#ifdef notyet
static ROAMING_CHANNEL rcl10[] =        /* Added Korean channels 2312-2372 */
{ 2312, 2317, 2322, 2327, 2332, 2337, 2342, 2347, 2352, 2357, 2362, 2367, 2372, 0 };
static ROAMING_CHANNEL rcl11[] =        /* Added Japan channels in 4.9/5.0 spectrum */
{ 5040, 5060, 5080, 4920, 4940, 4960, 4980, 0 };
#endif
static ROAMING_CHANNEL rcl12[] =        /* 2.4Ghz Turbo channel 6 */
{ 2437, 0 };

static ADHOC_CLIST adhocWorldClist[] =  /* Channel: 36, 40, 44, 48 */
{ 5180, 5200, 5220, 5240, 0 };
#ifdef notyet
static ADHOC_CLIST adhocFcc3Clist[] =	/* Channel: 36, 40, 44, 48, and 145, 149, 153, 157, 161, 165 */
{ 5180, 5200, 5220, 5240, 5725, 5745, 5765, 5785, 5805, 5825, 0 };
static ADHOC_CLIST adhocMkkClist[] =    /* Channel: 34, 38, 42, 46 */
{ 5170, 5190, 5210, 5230, 0 };
static ADHOC_CLIST adhoc11bClist[] =    /* Channel: 10, 11 */
{ 2457, 2462, 0 };
#endif

static WWR_CLIST wwrcl1     = { HAL_MODE_11A,   rcl1  };
static WWR_CLIST wwrcl2     = { HAL_MODE_11A,   rcl2  };
static WWR_CLIST wwrcl3     = { HAL_MODE_11B,   rcl3  };
static WWR_CLIST wwrcl3x    = { HAL_MODE_11B,   rcl3x  };
static WWR_CLIST wwrcl3a    = { HAL_MODE_11B,   rcl3a };
static WWR_CLIST wwrcl3ax   = { HAL_MODE_11B,   rcl3ax };
static WWR_CLIST wwrcl4     = { HAL_MODE_11A,   rcl4  };
static WWR_CLIST wwrcl5     = { HAL_MODE_TURBO, rcl5  };
static WWR_CLIST wwrcl6     = { HAL_MODE_TURBO, rcl6  };
static WWR_CLIST wwrcl7     = { HAL_MODE_11A,   rcl7  };
static WWR_CLIST wwrcl8     = { HAL_MODE_11B,   rcl8  };
static WWR_CLIST wwrcl8x    = { HAL_MODE_11B,   rcl8x  };
static WWR_CLIST wwrcl8a    = { HAL_MODE_11B,   rcl8a };
static WWR_CLIST wwrcl8ax   = { HAL_MODE_11B,   rcl8ax };
static WWR_CLIST wwrcl9     = { HAL_MODE_11B,   rcl9  };
#ifdef notyet
static WWR_CLIST wwrcl10    = { HAL_MODE_11B,   rcl10 };
static WWR_CLIST wwrcl11    = { HAL_MODE_11A,   rcl11 };
#endif
static WWR_CLIST wwrcl12    = { HAL_MODE_108G,  rcl12 };

#ifdef notyet
/*
 * Scanning order of Extended Channel Mode channels,
 * Extended Channel Mode channels are channels 12 - 14.
 */
static const WWR_CLIST * socTbl [] = {
    &wwrcl3,  &wwrcl1,  &wwrcl2, &wwrcl8,  &wwrcl9, &wwrcl4, &wwrcl5, &wwrcl6,
    &wwrcl7, &wwrcl10, &wwrcl11, &wwrcl12, AH_NULL
};
#endif

/*
 *  Extended Channel Mode World SKU Table
 */
static const SUPER_DOMAIN superDomainTbl [] = {
    { WOR0_WORLD,  START_ADHOC_PER_11D, AH_NULL,            10,
      { &wwrcl1,   &wwrcl2, &wwrcl3,  &wwrcl4, &wwrcl5, &wwrcl6,  &wwrcl7, &wwrcl8,  &wwrcl9, &wwrcl12 } },
    { WOR01_WORLD, START_ADHOC_PER_11D, AH_NULL,           9,
      { &wwrcl1,   &wwrcl2, &wwrcl3x, &wwrcl4, &wwrcl5, &wwrcl6,  &wwrcl7, &wwrcl8x, &wwrcl12 } },
    { WOR02_WORLD, START_ADHOC_PER_11D, AH_NULL,           9,
      { &wwrcl1,   &wwrcl2, &wwrcl3,  &wwrcl4, &wwrcl5, &wwrcl6,  &wwrcl7, &wwrcl8,  &wwrcl12 } },
    { EU1_WORLD,   START_ADHOC_PER_11D, AH_NULL,           9,
      { &wwrcl1,   &wwrcl2, &wwrcl3,  &wwrcl4, &wwrcl5, &wwrcl6,  &wwrcl7, &wwrcl8,  &wwrcl12 } },
    { WOR1_WORLD,  START_ADHOC_NO_11A,  AH_NULL,            8,
      { &wwrcl1,   &wwrcl2, &wwrcl3,  &wwrcl4, &wwrcl7, &wwrcl8,  &wwrcl9, &wwrcl12, AH_NULL } },
    { WOR2_WORLD,  START_ADHOC_IN_11A,  adhocWorldClist, 10,
      { &wwrcl1,   &wwrcl2, &wwrcl3,  &wwrcl4, &wwrcl5, &wwrcl6,  &wwrcl7, &wwrcl8,  &wwrcl9, &wwrcl12 } },
    { WOR3_WORLD,  START_ADHOC_PER_11D, AH_NULL,            8,
      { &wwrcl1,   &wwrcl2, &wwrcl3,  &wwrcl4, &wwrcl5, &wwrcl6,  &wwrcl8, &wwrcl12, AH_NULL } },
    { WOR4_WORLD,  START_ADHOC_NO_11A, AH_NULL,             7,
      { &wwrcl1,   &wwrcl3a, &wwrcl4, &wwrcl5, &wwrcl6, &wwrcl8a, &wwrcl12, AH_NULL,    AH_NULL } },    
    { WOR5_ETSIC,  START_ADHOC_NO_11A, AH_NULL,             5,
      { &wwrcl1,   &wwrcl3,  &wwrcl4, &wwrcl8, &wwrcl12, AH_NULL } },
    { WOR9_WORLD,  START_ADHOC_NO_11A, AH_NULL,             8,
      { &wwrcl1,   &wwrcl3a, &wwrcl4, &wwrcl5, &wwrcl6, &wwrcl7,  &wwrcl8a, &wwrcl12, AH_NULL } },
    { WORA_WORLD,  START_ADHOC_NO_11A, AH_NULL,             6,
      { &wwrcl1,   &wwrcl3,  &wwrcl4, &wwrcl7, &wwrcl8, &wwrcl12, AH_NULL } }
};

#ifdef notyet
/*
 * Scanning order of non-Extended Channel Mode channels
 * Same as Extended Channel Mode channels but exclude channels 12 - 14.
 */
static const WWR_CLIST * NonEcmSocTbl [] = {
    &wwrcl3x,  &wwrcl1,  &wwrcl2, &wwrcl8x, &wwrcl4, &wwrcl5, &wwrcl6, &wwrcl7,
    &wwrcl10, &wwrcl11, &wwrcl12, AH_NULL
};
#endif

/*
 *  Non-Extended Channel Mode World SKU Table
 */
static const SUPER_DOMAIN NonEcmSuperDomainTbl [] = {
    { WOR0_WORLD, START_ADHOC_PER_11D, AH_NULL,            9,
      { &wwrcl1,  &wwrcl2, &wwrcl3x, &wwrcl4, &wwrcl5, &wwrcl6,   &wwrcl7,  &wwrcl8x, &wwrcl12 } },
    { WOR01_WORLD, START_ADHOC_PER_11D, AH_NULL,           9,
      { &wwrcl1,  &wwrcl2, &wwrcl3x, &wwrcl4, &wwrcl5, &wwrcl6,   &wwrcl7,  &wwrcl8x, &wwrcl12 } },
    { WOR02_WORLD, START_ADHOC_PER_11D, AH_NULL,           9,
      { &wwrcl1,  &wwrcl2, &wwrcl3,  &wwrcl4, &wwrcl5, &wwrcl6,   &wwrcl7,  &wwrcl8x, &wwrcl12 } },
    { EU1_WORLD,  START_ADHOC_PER_11D, AH_NULL,           9,
      { &wwrcl1,  &wwrcl2, &wwrcl3,  &wwrcl4, &wwrcl5, &wwrcl6,   &wwrcl7,  &wwrcl8x, &wwrcl12 } },
    { WOR1_WORLD, START_ADHOC_NO_11A,  AH_NULL,            7,
      { &wwrcl1,  &wwrcl2, &wwrcl3x, &wwrcl4, &wwrcl7, &wwrcl8x,  &wwrcl12, AH_NULL,     AH_NULL } },
    { WOR2_WORLD, START_ADHOC_IN_11A,  adhocWorldClist, 9,
      { &wwrcl1,  &wwrcl2, &wwrcl3x, &wwrcl4, &wwrcl5, &wwrcl6,   &wwrcl7,  &wwrcl8x, &wwrcl12 } },
    { WOR3_WORLD, START_ADHOC_PER_11D, AH_NULL,            8,
      { &wwrcl1,  &wwrcl2, &wwrcl3x, &wwrcl4, &wwrcl5, &wwrcl6,   &wwrcl8x, &wwrcl12, AH_NULL } },
    { WOR4_WORLD, START_ADHOC_NO_11A, AH_NULL,             7,
      { &wwrcl1, &wwrcl3ax, &wwrcl4, &wwrcl5, &wwrcl6, &wwrcl8ax, &wwrcl12, AH_NULL,     AH_NULL } },    
    { WOR5_ETSIC, START_ADHOC_NO_11A, AH_NULL,             5,
      { &wwrcl1, &wwrcl3ax, &wwrcl4, &wwrcl8ax, &wwrcl12, AH_NULL } },
    { WOR9_WORLD,  START_ADHOC_NO_11A, AH_NULL,             8,
      { &wwrcl1, &wwrcl3ax, &wwrcl4, &wwrcl5, &wwrcl6, &wwrcl7,  &wwrcl8ax, &wwrcl12, AH_NULL } },
    { WORA_WORLD,  START_ADHOC_NO_11A, AH_NULL,             6,
      { &wwrcl1, &wwrcl3ax, &wwrcl4, &wwrcl7, &wwrcl8ax, &wwrcl12, AH_NULL } }
};

#ifdef notyet
typedef struct {
	u_int16_t	rd;	/* EEPROM regulatory domain */
	ADHOC_CLIST	*pClist;
} ADHOC_CHAN_RD;

/*
 * The following table defines the adhoc channel list of
 * the regulatory domain
 */
static ADHOC_CHAN_RD adhocChanRdTbl[] = {
    { FCC1_FCCA, adhocWorldClist },
    { FCC3_FCCA, adhocFcc3Clist  },
    { MKK1_MKKA, adhocMkkClist   },
    { MKK1_FCCA, adhocMkkClist   },
    { MKK1_MKKA1,adhocMkkClist   },
    { MKK1_MKKA2,adhocMkkClist   }    
};
#endif

/*
 * Implicit Regulatory Domain Channel Lists and Table
 */
typedef struct {
	HAL_REG_DOMAIN	regDmn;
	HAL_CTRY_CODE   countryCode;
	u_int16_t	channelFlags;
	ROAMING_CHANNEL	*cList;
} IMPLICIT_RD_CHANNEL;

static ROAMING_CHANNEL irdList1[] = {
	5170, 5190, 5210, 5230, 0			/* 4 MKK channels */
};
static ROAMING_CHANNEL irdList2[] = {
	5210, 5250, 5290, 5760, 5800, 0		 	/* 5 turbo channels */
};
static ROAMING_CHANNEL irdList3[] = {
	5500, 5520, 5540, 5560, 5580, 5600, 5620, 5640,	/* 5.47 - 5.72 ETSI */
	5660, 5680, 5700, 0
};
static const IMPLICIT_RD_CHANNEL irdChannelTbl[] = {
	{ MKK1,  CTRY_JAPAN,          CHANNEL_A, irdList1 },
	{ FCC1,  CTRY_UNITED_STATES,  CHANNEL_T, irdList2 },
	/*
	 * Change from United Kingdom to Netherlands to match with ETSI1
	 * A ETSI1 country will be needed if Netherlands changes its
	 * regulatory domain
	 */
	{ ETSI1, CTRY_NETHERLANDS,     CHANNEL_A, irdList3 }
};

#if 0
/***************************************************************************
 *	  Cisco's non-ISO view of the 802.11d country code element
 *
	  ID  Len   Country		 Base Num  Pwr 
								 Chn Chn 
	0x07, 0x06, 0x4E, 0x41, 0x00,  1, 11, 200   ; Americas 'NA\0' 
	0x07, 0x06, 0x45, 0x55, 0x00,  1, 13,  50   ; Europe 'EU\0' 
	0x07, 0x06, 0x4a, 0x41, 0x50, 14,  1,  90   ; Japan 'JAP' 
	0x07, 0x06, 0x53, 0x50, 0x41, 10,  2,  50   ; Spain 'SPA' 
	0x07, 0x06, 0x46, 0x52, 0x41, 10,  4,  50   ; France 'FRA' 
	0x07, 0x06, 0x42, 0x45, 0x4c, 12,  2,  50   ; Belgium 'BEL' 
	0x07, 0x06, 0x49, 0x53, 0x52,  3,  7,  50   ; Israel 'ISR' 
	0x07, 0x06, 0x43, 0x41, 0x4e,  1,  8,  50   ; Canada 'CAN' (Outdoor) 
	0x07, 0x06, 0x41, 0x55, 0x53,  1, 11, 200   ; Australia 'AUS' 
	0x07, 0x06, 0x4a, 0x41, 0x57,  1, 14,  50   ; Japan wideband 'JPW' 
	0x07, 0x06, 0x43, 0x4E, 0x20,  1, 11,   5   ; China 'CN '
 *
 ***************************************************************************/
typedef struct {
	HAL_CTRY_CODE   countryCode;
	HAL_REG_DOMAIN  regDmn;	
	COUNTRY_INFO_LIST   info;
} CISCO_COUNTRY_INFO_LIST;

static CISCO_COUNTRY_INFO_LIST ciscoCcTbl[] = {
    { CTRY_UNITED_STATES, FCC1_FCCA,
      { 0x07, 0x06, { 0x4E, 0x41, 0x00 },         /* Americas 'NA\0' */
        { {  1, 11, 200 }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} }, 0 } },
    { CTRY_UNITED_KINGDOM, ETSI1_WORLD,
      { 0x07, 0x06, { 0x45, 0x55, 0x00 },         /* Europe 'EU\0' */
        { {  1, 13,  50 }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} }, 0 } },
    { CTRY_JAPAN, MKK1_MKKA,
      { 0x07, 0x06, { 0x4a, 0x41, 0x50, },        /*  Japan 'JAP' */
        { { 14,  1,  90 }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} }, 0 } },
    { CTRY_SPAIN, ETSI1_WORLD,
      { 0x07, 0x06, { 0x53, 0x50, 0x41 },         /* Spain 'SPA' */
        { { 10,  2,  50 }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} }, 0 } },
    { CTRY_FRANCE, ETSI3_WORLD,
      { 0x07, 0x06, { 0x46, 0x52, 0x41 },       /* France 'FRA' */
        { { 10,  4,  50 }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} }, 0 } },
    { CTRY_BELGIUM, ETSI2_WORLD,
      { 0x07, 0x06, { 0x42, 0x45, 0x4c },         /* Belgium 'BEL' */
        { { 12,  2,  50 }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} }, 0 } },
    { CTRY_ISRAEL, NULL1_ETSIB,
      { 0x07, 0x06, { 0x49, 0x53, 0x52 },         /* Israel 'ISR' */
        { {  3,  7,  50 }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} }, 0 } },
    { CTRY_CANADA, FCC2_FCCA,
      { 0x07, 0x06, { 0x43, 0x41, 0x4e },         /* Canada 'CAN' (Outdoor) */
        { {  1,  8,  50 }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} }, 0 } },
    { CTRY_AUSTRALIA, FCC2_WORLD,
      { 0x07, 0x06, { 0x41, 0x55, 0x53 },         /* Australia 'AUS' */
        { {  1, 11, 200 }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} }, 0 } },
    { CTRY_JAPAN, MKK1_MKKA,
      { 0x07, 0x06, { 0x4a, 0x41, 0x57 },         /* Japan wideband 'JPW' */
        { {  1, 14,  50 }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} }, 0 } },
    { CTRY_CHINA, APL1_WORLD,
      { 0x07, 0x06, { 0x43, 0x4E, 0x20 },         /* China 'CN ' */
        { { 1, 11,   5  }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} }, 0 } }
};
#endif

static u_int16_t
getEepromRD(struct ath_hal *ah)
{
	return AH_PRIVATE(ah)->ah_currentRD &~ WORLDWIDE_ROAMING_FLAG;
}

static int16_t
getRDIndex(struct ath_hal *ah, HAL_REG_DOMAIN rde)
{
	int16_t i;

	for (i = 0; i < N(allEnumRds); i++)
		if (allEnumRds[i].regDmnEnum == rde)
			return i;
#ifdef AH_DEBUG
	ath_hal_printf(ah, "%s: unknown regulatory domain %u\n", __func__, rde);
#endif
	return -1;
}

static int16_t
getCountryIndex(struct ath_hal *ah, HAL_CTRY_CODE cc)
{
	int16_t i;

	for (i = 0; i < N(allCountries); i++)
		if (allCountries[i].countryCode == cc)
			return i;
#ifdef AH_DEBUG
	ath_hal_printf(ah, "%s: unknown country code %u\n", __func__, cc);
#endif
	return -1;
}

static HAL_BOOL
getSupDomain(struct ath_hal *ah, HAL_REG_DOMAIN *prd, u_int *pix)
{
	u_int16_t rd = getEepromRD(ah);
	if (rd & COUNTRY_ERD_FLAG)
		return AH_FALSE;
	rd &= SUPER_DOMAIN_MASK;
	if (rd) {
		int i;
		for (i = 0; i < N(superDomainTbl); i++) {
			if (superDomainTbl[i].domain == rd) {
				if (prd)
					*prd = rd;
				if (pix)
					*pix = i;
				return AH_TRUE;
			}
		}
	}
	if (AH_PRIVATE(ah)->ah_opmode == HAL_M_HOSTAP) {
		/*
		 * Per Atheros convention; always return something
		 * when operating as an AP.  In this case we return
		 * the first super domain in the table.
		 *
		 * XXX perhaps it should be the default regdomain?
		 */
		if (prd)
			*prd = superDomainTbl[0].domain;
		if (pix)
			*pix = 0;
		return AH_TRUE;
	}
	return AH_FALSE;
}

/*
 * Return the Wireless Mode Regulatory Domain based
 * on the country code and the wireless mode.
 */
static u_int16_t
getWmRD(struct ath_hal *ah, HAL_CTRY_CODE cc, u_int16_t cflags)
{
	int16_t rdix;

	/*
	 * Use country code to lookup regulator domain for known country
	 * codes.  Otherwise, use the default country code and regulator
	 * domain if EEPROM is unprogrammed, or EEPROM's regulator domain
	 * is unknown.
	 */
	if (cc == CTRY_DEFAULT) {
		u_int16_t rdcc;

		rdix = getRDIndex(ah, DEF_REGDMN);
		rdcc = getEepromRD(ah);
		if (rdcc) {
			int16_t rd = getRDIndex(ah, rdcc);
			if (rd != -1)
				rdix = rd;
		}
	} else {
		int16_t cix;

		cix = getCountryIndex(ah, cc);
		/* XXX what to do w/ unknown country */
		rdix = getRDIndex(ah, allCountries[cix].regDmnEnum);
	}
	if (cflags & CHANNEL_2GHZ)
		return allEnumRds[rdix].regDmnWirelessMode2;
	else
		return allEnumRds[rdix].regDmnWirelessMode5;
}

static u_int16_t
getChannelSpread(struct ath_hal *ah, u_int16_t cflags)
{
	HALASSERT((cflags & (CHANNEL_2GHZ|CHANNEL_5GHZ)) != 0);

	if (cflags & CHANNEL_TURBO) {
		return TURBO_CHANNEL_SEPARATION;
	} else if (cflags & CHANNEL_2GHZ) {
		/* XXX return same value of CCK or OFDM at 2GHz */
		return LOW2GHZ_CHANNEL_SEPARATION;
	} else if (cflags & CHANNEL_5GHZ) {
		return OFDM5_CHANNEL_SEPARATION;
	}
	return 20;		/* XXX something other than 0 */
}

static u_int
getCcEntries(struct ath_hal *ah, u_int cflags)
{
	HALASSERT((cflags & (CHANNEL_2GHZ|CHANNEL_5GHZ)) != 0);

	if (cflags & CHANNEL_5GHZ) {
		return (cflags & CHANNEL_TURBO) ?
			N(regDmnWm5Turbo) : N(regDmnWm5);
	}
	if (cflags & CHANNEL_2GHZ) {
		return (cflags & CHANNEL_TURBO) ?
			N(regDmnWm2Turbo) : N(regDmnWm2);
	}
	return 0;
}

static const REG_DMN_WM_FREQ_TABLE *
getCcTable(struct ath_hal *ah, u_int cflags)
{
	HALASSERT((cflags & (CHANNEL_2GHZ|CHANNEL_5GHZ)) != 0);

	if (cflags & CHANNEL_5GHZ) {
		return (cflags & CHANNEL_TURBO) ?
			regDmnWm5Turbo : regDmnWm5;
	}
	if (cflags & CHANNEL_2GHZ) {
		return (cflags & CHANNEL_TURBO) ?
			regDmnWm2Turbo : regDmnWm2;
	}
	return AH_NULL;
}

const static struct cmode {
	u_int16_t	mode;
	u_int16_t	flags;
} modes[] = {
	{ HAL_MODE_TURBO,	CHANNEL_T },
	{ HAL_MODE_11A,		CHANNEL_A },
	{ HAL_MODE_11B,		CHANNEL_B },
	{ HAL_MODE_11G,		CHANNEL_G },
	{ HAL_MODE_108G,	CHANNEL_108G },
};

static u_int16_t
getChannelFlags(struct ath_hal *ah, u_int16_t modeSelect, u_int16_t freq)
{
	const REG_DMN_WM_FREQ_TABLE *ccTable;
	const struct cmode *cm;
	u_int16_t c, cspread;
	int i, j, n;

	for (cm = modes; cm < &modes[N(modes)]; cm++) {
		if ((cm->mode & modeSelect) == 0) {
			/* wrong type of channel */
			continue;
		}
		ccTable = getCcTable(ah, cm->flags);
		n = getCcEntries(ah, cm->flags);
		cspread = getChannelSpread(ah, cm->flags);
		for (i = 0; i < n; i++) {
			for (j = 0; j < ccTable[i].entries; j++) {
				for (c = ccTable[i].chanGroup[j].lowChannel;
				     c <= ccTable[i].chanGroup[j].highChannel;
				     c += IS24DEBUG(c) ?
					HIGH2GHZ_CHANNEL_SEPARATION : cspread) {
					if (c == freq)
						return cm->flags;
				}
			}
		}
	}
	HALDEBUG(ah, "%s: no entry, mode 0x%x freq %u\n",
		__func__, modeSelect, freq);
	return 0;
}

static u_int
mode2Cflags(struct ath_hal *ah, u_int mode)
{
	switch (mode) {
	case HAL_MODE_11A:	return CHANNEL_A;
	case HAL_MODE_11B:	return CHANNEL_B;
	case HAL_MODE_11G:	return CHANNEL_G;
	case HAL_MODE_TURBO:	return CHANNEL_T;
	case HAL_MODE_108G:	return CHANNEL_108G;
	}
#ifdef AH_DEBUG
	ath_hal_printf(ah, "%s: unknown mode %x\n", __func__, mode);
#endif
	return 0;
}

static const REG_DMN_WM_FREQ_TABLE*
getRdTable(struct ath_hal *ah, HAL_CTRY_CODE cc, u_int16_t cflags)
{
	const REG_DMN_WM_FREQ_TABLE* table = getCcTable(ah, cflags);
	int n = getCcEntries(ah, cflags);
	u_int16_t wmode = getWmRD(ah, cc, cflags);
	int i;

	for (i = 0; i < n; i++)
		if (table[i].regDmnWirelessMode == wmode)
			return &table[i];
	return regDmnNone;
}

static u_int16_t
getRdMode(struct ath_hal *ah, HAL_CTRY_CODE cc, u_int16_t cflags)
{
	return getRdTable(ah, cc, cflags)->regDmnWirelessMode;
}

/*
 * Calculate a default country based on the EEPROM setting.
 */
static u_int16_t
getDefaultCountry(struct ath_hal *ah)
{
	u_int16_t rd;
	int i;

	rd = getEepromRD(ah);
	if (rd & COUNTRY_ERD_FLAG) {
		u_int16_t cc = rd &~ COUNTRY_ERD_FLAG;
		for (i = 0; i < N(allCountries); i++)
			if (allCountries[i].countryCode == cc)
				return cc;
	}
	/*
	 * Check regulatory domains that have only one country.
	 */
	switch (rd) {
	case MKK1_MKKA:	return CTRY_JAPAN;
	case MKK1_MKKB:	return CTRY_JAPAN;
	case MKK2_MKKA:	return CTRY_JAPAN3;
	case MKK1_FCCA:	return CTRY_JAPAN2;
	case MKK1_MKKA1:return CTRY_JAPAN4;
	case MKK1_MKKA2:return CTRY_JAPAN5;
	}
	return CTRY_DEFAULT;
}

static HAL_BOOL
isWwrSKU(struct ath_hal *ah)
{
	return getSupDomain(ah, AH_NULL, AH_NULL);
}

/*
 * Does the specified regulatory domain support 11b only?
 */
static HAL_BOOL
is11bOnlyRD(HAL_REG_DOMAIN rd)
{
	switch (rd) {
	case NULL1_WORLD:
	case NULL1_ETSIB:
	case NULL1_ETSIC:
	case NULL1:
		return AH_TRUE;
	}
	return AH_FALSE;
}

/*
 * Does the specified country permit use of 11g?
 */
static HAL_BOOL
isCountry11g(struct ath_hal *ah, HAL_CTRY_CODE cc)
{
	int i;

	for (i = 0; i < N(allCountries); i++)
		if (allCountries[i].countryCode == cc)
			return allCountries[i].allow11g;
	return AH_FALSE;
}

/*
 * Does the specified country permit use of 11a?
 */
static HAL_BOOL
isCountry11a(struct ath_hal *ah, HAL_CTRY_CODE cc)
{
	HAL_REG_DOMAIN rd = getWmRD(ah, cc, CHANNEL_A);
	return !is11bOnlyRD(rd);
}

/*
 * Checks it is ok to use turbo mode
 *
 * If global SKU, then verify whether turbo is allowed or not
 * Otherwise, get regulatrory domain or country code.
 * If country code, then extract the country's regulatory domain
 * that country.
 * Get the 5GHz of the regulatory domain and check if the
 * regulatory domain is in the turbo regulatory domain table
 * or not.
 */
static HAL_BOOL
is11aTurboAllowed(struct ath_hal *ah)
{
	HAL_REG_DOMAIN rd;
	int i, j;

	if (isWwrSKU(ah)) {
		rd = AH_PRIVATE(ah)->ah_currentRD;
		for (i = 0; i < N(superDomainTbl); i++) {
			if (superDomainTbl[i].domain == rd) {
				for (j = 0; j < superDomainTbl[i].rclTblSize; j++) {
					if (superDomainTbl[i].RclTbl[j]) {
						if (superDomainTbl[i].RclTbl[j]->mode & HAL_MODE_TURBO)
							return AH_TRUE;
					}
				}
			}
		}
		return AH_FALSE;
	} else {
		u_int16_t eepRdCc = getEepromRD(ah);

		if (eepRdCc & COUNTRY_ERD_FLAG) {
			u_int16_t ccode = eepRdCc & ~COUNTRY_ERD_FLAG;
			for (i = 0; i < N(allCountries); i++) {
				if (ccode == allCountries[i].countryCode) {
					if (allCountries[i].allow11aTurbo)
						return AH_TRUE;
				}
			}
			return AH_FALSE;
		} else {
			const REG_DMN_WM_FREQ_TABLE *pTbl =
				getCcTable(ah, CHANNEL_T);
			int tblLen = getCcEntries(ah, CHANNEL_T);

			rd = allEnumRds[getRDIndex(ah, eepRdCc)].regDmnWirelessMode5;
			for (i = 0; i < tblLen; i++) {
				if (pTbl[i].regDmnWirelessMode == rd)
					return AH_TRUE;
			}
			return AH_FALSE;
		}
	}
}

/*
 * Does the specified country permit use of 5GHz Turbo mode?
 */
static HAL_BOOL
isCountry11aTurbo(struct ath_hal *ah, HAL_CTRY_CODE cc)
{
	int i;

	if (cc == CTRY_DEFAULT)
		return is11aTurboAllowed(ah);
	for (i = 0; i < N(allCountries); i++)
		if (allCountries[i].countryCode == cc)
			return allCountries[i].allow11aTurbo;
	return AH_FALSE;
}

/*
 * Does the specified country permit use of 2.4Ghz Turbo mode?
 */
static HAL_BOOL
isCountry11gTurbo(struct ath_hal *ah, HAL_CTRY_CODE cc)
{
	int i;

	for (i = 0; i < N(allCountries); i++)
		if (allCountries[i].countryCode == cc)
			return allCountries[i].allow11gTurbo;
	return AH_FALSE;
}

static HAL_BOOL
isPassiveScanChannel(struct ath_hal *ah, HAL_REG_DOMAIN rd, const HAL_CHANNEL *c)
{
	typedef struct {
		u_int16_t	mode;
		u_int16_t	numClist;    
		const ROAMING_CHANNEL *pClTbl[4];
	} FCC3_PSCAN_CLIST;
	/* FCC3 passive scan channel list: 52, 56, 60, 64 */
	static const ROAMING_CHANNEL fcc3Passive1[] = {
		5260, 5280, 5300, 5320, 0	/* NB: 0-terminated */
	};
	/* FCC3 passive scan channel list: 100,104,108,112,116,120,124,128,132,136,140 */
	static const ROAMING_CHANNEL fcc3Passive2[] = {
		5500, 5520, 5540, 5560, 5580, 5600,
		5620, 5640, 5660, 5680, 5700, 0	/* NB: 0-terminated */
	};
	static const FCC3_PSCAN_CLIST fcc3PassiveScanTbl = {
		CHANNEL_A, 2, { fcc3Passive1, fcc3Passive2 }
	};
	static const struct {
		u_int16_t	rd;		/* 2.4/5GHz regulatory domain */
		u_int16_t	eepromRD;	/* EEPROM regulatory domain */
		u_int16_t	cflags;
		const FCC3_PSCAN_CLIST *pList;
	} passiveScanRdTbl[] = {
		{ FCC3,  FCC3_FCCA, CHANNEL_A, &fcc3PassiveScanTbl },
		{ ETSI1, 0,         CHANNEL_A, AH_NULL },
		{ ETSI2, 0,         CHANNEL_A, AH_NULL },
		{ ETSI3, 0,         CHANNEL_A, AH_NULL },
		{ ETSI4, 0,         CHANNEL_A, AH_NULL },
		{ ETSI5, 0,         CHANNEL_A, AH_NULL },
		{ ETSI6, 0,         CHANNEL_A, AH_NULL },
		{ ETSIA, 0,         CHANNEL_A, AH_NULL },
		{ ETSIB, 0,         CHANNEL_A, AH_NULL },
		{ ETSIC, 0,         CHANNEL_A, AH_NULL },

		{ EU1_WORLD, EU1_WORLD, CHANNEL_A, AH_NULL },    

		{ MKK2,  MKK2_MKKA, CHANNEL_A, AH_NULL },
		{ MKKA,  MKK2_MKKA, CHANNEL_B, AH_NULL },
		{ MKKA,  MKK2_MKKA, CHANNEL_G, AH_NULL },    
		{ MKK1,  MKK1_MKKB, CHANNEL_A, AH_NULL },
		{ MKKA,  MKK1_MKKB, CHANNEL_B, AH_NULL },
		{ MKKA,  MKK1_MKKB, CHANNEL_G, AH_NULL }
	};
	HAL_BOOL found = AH_FALSE;
	u_int16_t flags = c->channelFlags & CHANNEL_ALL;
	int i;

	for (i = 0; i < N(passiveScanRdTbl); i++)
		if (passiveScanRdTbl[i].rd == rd) {
			/*
			 * Check the table for a match.  Also accept an
			 * unprogrammed EEPROM domain setting.
			 */
			if ((passiveScanRdTbl[i].eepromRD == getEepromRD(ah) ||
			     passiveScanRdTbl[i].eepromRD == NO_ENUMRD) &&
			    passiveScanRdTbl[i].cflags == flags) {
				found = AH_TRUE;
				break;
			}
		}
	if (found && passiveScanRdTbl[i].pList) {
		const FCC3_PSCAN_CLIST *pList= passiveScanRdTbl[i].pList;
		found = AH_FALSE;
		for (i = 0; i < pList->numClist; i++) {
			const ROAMING_CHANNEL *pc;
			for (pc = pList->pClTbl[i]; *pc; pc++)
				if (*pc == c->channel)
					return AH_TRUE;
		}
	}
	return found;
}

#ifdef notyet
/*
 * Does the regulatory domain require DFS?
 */
static HAL_BOOL
isDfs(HAL_REG_DOMAIN rd)
{
	switch (rd) {
	case FCC3:
	case ETSI1:
	case ETSI2:
	case ETSI3:
	case ETSI4:
	case ETSI5:
	case ETSI6:
	case ETSIA:
	case ETSIB:
	case ETSIC:
		return AH_TRUE;
	}
	return AH_FALSE;
}

static HAL_BOOL
isDfsChannel(struct ath_hal *ah, HAL_CHANNEL *chan)
{
	HAL_REG_DOMAIN rd;

	if (!IS_CHAN_5GHZ(chan))
		return AH_FALSE;
	rd = getWmRD(ah, cc, chan->channelFlags);
	if (rd == FCC3) {
		static u_int16_t fcc3DfsChannelTbl[] = {
		     /* Base */
		     5260,
		     5280,
		     5300,
		     5320,
		     5340,
		     5360,
		     5380,
		     5400,
		     5420,
		     5440,
		     5460,
		     5480,
		     5500,
		     5520,
		     5540,
		     5560,
		     5580,
		     5600,
		     5620,
		     5640,
		     5660,
		     5680,
		     5700,
		     /* Turbo */
		     5240,
		     5250,
		     5280,
		     5290,
		     5540,
		     5580,
		     5620,
		     5660
		};
		int i;
		for (i = 0; i < N(fcc3DfsChannelTbl); i++)
			if (chan->channel == fcc3DfsChannelTbl[i])
				return AH_TRUE;
		return AH_FALSE;
	}
	return isDfs(rd);
}
#endif

static inline HAL_BOOL
isEcmChannel(u_int freq, HAL_BOOL enableExtendedChannels)
{
	switch (freq) {
	case 2472:	/* 2.4Ghz channels 12-14 */
	case 2467:
	case 2484:
		return enableExtendedChannels;
	}
	return AH_TRUE;
}

static HAL_BOOL
isEcmChannelPassive(struct ath_hal *ah, u_int16_t channel)
{
#define A_SCAN	AH_FALSE
#define P_SCAN	AH_TRUE
	static	struct {
		u_int16_t	sku;
		struct {
			ROAMING_CHANNEL	chan;
			HAL_BOOL	scanMode;
		} tbl[3];
	} ecmSkuTbl[7] = {
		{ WOR0_WORLD,
		    { {2472, P_SCAN}, {2467, P_SCAN}, {2484, P_SCAN} } },
		{ WOR02_WORLD,
		    { {2472, P_SCAN}, {2467, P_SCAN}, {2484, P_SCAN} } },
		{ WOR1_WORLD,
		    { {2472, P_SCAN}, {2467, P_SCAN}, {2484, P_SCAN} } },
		{ WOR5_ETSIC,
		    { {2472, P_SCAN}, {2467, P_SCAN}, {2484, P_SCAN} } },
		{ MKK1_MKKA1,
		    { {2472, A_SCAN}, {2467, A_SCAN}, {2484, P_SCAN} } },
		{ MKK1_MKKA2,
		    { {2472, P_SCAN}, {2467, P_SCAN}, {2484, P_SCAN} } },
		{ WORA_WORLD,
		    { {2472, P_SCAN}, {2467, P_SCAN}, {2484, P_SCAN} } }
	};
	u_int16_t rd = getEepromRD(ah);
	int i, j;

	for (i = 0; i < N(ecmSkuTbl); i++) {
		if (rd == ecmSkuTbl[i].sku) {
			for (j = 0; j < N(ecmSkuTbl[i].tbl); j++) {
				if (channel == ecmSkuTbl[i].tbl[j].chan)
					return ecmSkuTbl[i].tbl[j].scanMode;
			}
		}
	}
	return AH_FALSE;
#undef P_SCAN
#undef A_SCAN
}

/*
 * Return whether or not worldwide roaming is
 * allowed by the EEPROM setting.
 */
static HAL_BOOL
isWorldWideRoaming(struct ath_hal *ah)
{
	if (AH_PRIVATE(ah)->ah_currentRD & WORLDWIDE_ROAMING_FLAG)
		return AH_TRUE;
	if (AH_PRIVATE(ah)->ah_opmode != HAL_M_HOSTAP) {
		/*
		 * Only check the super domain table when operating
		 * as a station (or station-like thing).  AP's don't
		 * roam.
		 */
		return getSupDomain(ah, AH_NULL, AH_NULL);
	}
	return AH_FALSE;
}

/*
 * Returns whether or not the specified country code
 * is allowed by the EEPROM setting
 */
static HAL_BOOL
isCountryCodeValid(struct ath_hal *ah, HAL_CTRY_CODE cc)
{
	u_int16_t rd;

	/* Default setting requires no checks */
	if (cc == CTRY_DEFAULT)
		return AH_TRUE;
#ifdef AH_DEBUG_COUNTRY
	if (cc == CTRY_DEBUG)
		return AH_TRUE;
#endif
	rd = getEepromRD(ah);
	HALDEBUG(ah, "%s: EEPROM regdomain 0x%x\n", __func__, rd);

	if (rd & COUNTRY_ERD_FLAG) {
		/* EEP setting is a country - config shall match */
		HALDEBUG(ah, "%s: EEPROM setting is country code %u\n",
			__func__, rd &~ COUNTRY_ERD_FLAG);
		return (cc == (rd & ~COUNTRY_ERD_FLAG));
	} else if (rd == DEBUG_REG_DMN || rd == NO_ENUMRD) {
		/* Set to Debug or AllowAnyCountry mode - allow any setting */
		HALDEBUG(ah, "%s: DEBUG or NO\n", __func__);
		return AH_TRUE;
	} else {
		int i;
		for (i = 0; i < N(allCountries); i++) {
			if (cc == allCountries[i].countryCode &&
				allCountries[i].regDmnEnum == rd)
				return AH_TRUE;
		}
	}
	return AH_FALSE;
}

/*
 * Return whether or not the regulatory domain/country in EEPROM
 * is acceptable.
 */
static HAL_BOOL
isEepromValid(struct ath_hal *ah)
{
	u_int16_t rd = getEepromRD(ah);
	int i;

	if (rd & COUNTRY_ERD_FLAG) {
		u_int16_t cc = rd &~ COUNTRY_ERD_FLAG;
		for (i = 0; i < N(allCountries); i++)
			if (allCountries[i].countryCode == cc)
				return AH_TRUE;
	} else {
		for (i = 0; i < N(allEnumRds); i++)
			if (allEnumRds[i].regDmnEnum == rd)
				return AH_TRUE;
	}
#ifdef AH_DEBUG
	ath_hal_printf(ah, "%s: invalid regulatory domain/country code 0x%x\n",
		__func__, rd);
#endif
	return AH_FALSE;
}

#ifdef AH_DEBUG
static const char *
modename(int m)
{
	switch (m) {
	case HAL_MODE_11A:	return "HAL_MODE_11A";
	case HAL_MODE_TURBO:	return "HAL_MODE_TURBO";
	case HAL_MODE_11B:	return "HAL_MODE_11B";
	case HAL_MODE_11G:	return "HAL_MODE_11G";
	case HAL_MODE_108G:	return "HAL_MODE_108G";
	}
	return "mode ??";
}
#endif /* AH_DEBUG */

/*
 * Build a dynamic roaming channel list.
 */
static HAL_BOOL
ath_hal_init_roaming_channels(struct ath_hal *ah,
	HAL_CHANNEL *chans, u_int maxchans, u_int *nchans,
	u_int16_t modeSelect, HAL_BOOL enableExtendedChannels)
{
#define	HAL_MODE_2GHZ	(HAL_MODE_11B | HAL_MODE_11G | HAL_MODE_108G)
#define IS_CHAN_NON_108G_2GHZ(_x) \
	(((_x) & CHANNEL_2GHZ) && ((_x) & CHANNEL_108G) != CHANNEL_108G)
	const SUPER_DOMAIN *table;
	HAL_REG_DOMAIN rd;
	u_int ix, next, hwModes;
	int i;

	if (!getSupDomain(ah, &rd, &ix)) {
		/* XXX we should only ever be called for a super domain */
		HALDEBUG(ah, "%s: no super domain!\n", __func__);
		return AH_FALSE;
	}
	AH_PRIVATE(ah)->ah_adHocMode = superDomainTbl[ix].adHocMode;
	HALDEBUG(ah, "%s: regdomain %u ix %u adhoc mode %u\n",
		__func__, rd, ix, AH_PRIVATE(ah)->ah_adHocMode);

	/*
	 * Initialize the channel list from the roaming list
	 * add channel and channel flags
	 */
	if (!enableExtendedChannels)
		table = &NonEcmSuperDomainTbl[ix];
	else
		table = &superDomainTbl[ix];
	next = 0;			/* next available slot in chans */
	hwModes = ath_hal_getWirelessModes(ah);
	for (i = 0; i < table->rclTblSize; i++) {
		const WWR_CLIST *rc;
		u_int tmode, c, cflags;
		int j;

		rc = table->RclTbl[i];
		tmode = rc->mode;
		for (j = 0; rc->pList[j]; j++) {
			if ((tmode & hwModes) == 0) {
				HALDEBUG(ah, "%s: !avail %s\n",
					__func__, modename(tmode));
				continue;
			}
			if ((tmode & modeSelect) == 0) {
				/* 11b is always available */
				if ((mode2Cflags(ah, tmode) & CHANNEL_2GHZ) == 0 ||
				    (modeSelect & HAL_MODE_2GHZ) == 0) {
					HALDEBUG(ah, "%s: skip %u %s\n",
						__func__, rc->pList[j],
						modename(tmode));
					continue;
				}
			}
			if (next >= maxchans) {
				HALDEBUG(ah, "%s: too many channels for channel"
					" table\n", __func__);
				goto done;
			}
			c = rc->pList[j];
			cflags  = getChannelFlags(ah, tmode, c);
			if (IS_CHAN_NON_108G_2GHZ(cflags)) {
				/*
				 * Fix up the 2.4Ghz channel flag because
				 * the table is set up for 11b only.
				 */
				cflags &= ~CHANNEL_ALL;
				cflags |= (modeSelect & HAL_MODE_11G) ?
					CHANNEL_G : CHANNEL_B;
			}
			if (cflags) {
				/* Add passive scan flags */
				cflags &= ~CHANNEL_PASSIVE;
				if (tmode & (HAL_MODE_11B | HAL_MODE_11G)) {
					if (isEcmChannel(c, AH_TRUE)) {
						if (c == CHANNEL_14) {
							cflags &= ~CHANNEL_G;
							cflags |= CHANNEL_B;
						}
						if (isEcmChannelPassive(ah, c))
							cflags |= CHANNEL_PASSIVE;
					}
				} else
					cflags |= CHANNEL_PASSIVE;
				chans[next].channel = c;
				chans[next].channelFlags = cflags;
				HALDEBUG(ah, "%s: add channel %u flags 0x%x\n",
					__func__, c, chans[next].channelFlags);
				next++;
				/*
				 * If this is an 11g channel and the request
				 * was for 11g _and_ 11b channels then replicate
				 * the 11g channel as an 11b channel as the
				 * tables do no explicitly specify 11b channels.
				 * The Atheros code accepts 11b use on any 11g
				 * channel while we require explicit
				 * mention in the channel table.
				 */
				if (IS_CHAN_G(&chans[next-1]) &&
				    (modeSelect & HAL_MODE_11B)) {
					if (next >= maxchans) {
						HALDEBUG(ah, "%s: too many "
							"channels for channel"
							" table\n", __func__);
						goto done;
					}
					cflags &= ~CHANNEL_G;
					cflags |= CHANNEL_B;
					chans[next].channel = c;
					chans[next].channelFlags = cflags;
					HALDEBUG(ah,
					    "%s: add channel %u flags 0x%x\n",
					    __func__, c, chans[next].channelFlags);
					next++;
				}
				/*
				 * If this is a Turbo channel and the request
				 * was for Turbo _and_ A channels then replicate
				 * the Turbo channel as an A channel as the
				 * tables do no explicitly specify A channels.
				 * The Atheros code accepts A use on any Turbo
				 * channel while we require explicit
				 * mention in the channel table.
				 */
				if (IS_CHAN_T(&chans[next-1]) &&
				    (modeSelect & HAL_MODE_11A)) {
					if (next >= maxchans) {
						HALDEBUG(ah, "%s: too many "
							"channels for channel"
							" table\n", __func__);
						goto done;
					}
					cflags &= ~CHANNEL_T;
					cflags |= CHANNEL_A;
					chans[next].channel = c;
					chans[next].channelFlags = cflags;
					HALDEBUG(ah,
					    "%s: add channel %u flags 0x%x\n",
					    __func__, c, chans[next].channelFlags);
					next++;
				}
			}
		}
	}
done:
	*nchans = next;
	return (next != 0);
#undef IS_CHAN_NON_108G_2GHZ
#undef HAL_MODE_2GHZ
}

static int
chansort(const void *a, const void *b)
{
	const HAL_CHANNEL *ca = a;
	const HAL_CHANNEL *cb = b;

	return (ca->channel == cb->channel) ?
		(ca->channelFlags & CHANNEL_ALL) -
			(cb->channelFlags & CHANNEL_ALL) :
		ca->channel - cb->channel;
}
typedef int ath_hal_cmp_t(const void *, const void *);
static	void ath_hal_sort(void *a, size_t n, size_t es, ath_hal_cmp_t *cmp);

/*
 * Setup the channel list based on the information in the EEPROM and
 * any supplied contry code.  Note that we also do a bunch of EEPROM
 * verification here and setup certain regulatory-related access control
 * data used later on.
 */ 
HAL_BOOL
ath_hal_init_channels(struct ath_hal *ah,
	HAL_CHANNEL *chans, u_int maxchans, u_int *nchans,
	HAL_CTRY_CODE cc, u_int16_t modeSelect,
	HAL_BOOL enableOutdoor, HAL_BOOL enableExtendedChannels)
{
	u_int modesAvail;
	const struct cmode *cm;
	u_int16_t maxChan = 7000;
	u_int next;

	HALDEBUG(ah, "%s: cc %u mode 0x%x%s%s\n", __func__,
		cc, modeSelect,
		enableOutdoor ? " enable outdoor" : "",
		enableExtendedChannels ? " enable xchans" : "");
	/*
	 * Validate the EEPROM setting and setup defaults.
	 */
	if (!isEepromValid(ah)) {
		/*
		 * Don't return any channels if the EEPROM has an
		 * invalid regulatory domain/country code setting.
		 */
		HALDEBUG(ah, "%s: invalid EEPROM contents\n", __func__);
		return AH_FALSE;
	}
	AH_PRIVATE(ah)->ah_countryCode = getDefaultCountry(ah);

	/*
	 * If this is a World Wide Roaming SKU enable common mode.
	 */
	if (isWwrSKU(ah)) {
		HALDEBUG(ah, "%s: world wide roaming SKU, enable common mode\n",
			__func__);
		AH_PRIVATE(ah)->ah_commonMode = AH_TRUE;
	}

	/*
	 * We now have enough state to validate any country
	 * code passed in by the caller.
	 */
	if (AH_PRIVATE(ah)->ah_countryCode == CTRY_DEFAULT) {
		if (!isCountryCodeValid(ah, cc)) {
			/* NB: Atheros silently ignores invalid country codes */
			HALDEBUG(ah, "%s: invalid country code %u\n",
				__func__, cc);
			return AH_FALSE;
		}
		AH_PRIVATE(ah)->ah_countryCode = cc;
	}
	modesAvail = ath_hal_getwirelessmodes(ah,
			AH_PRIVATE(ah)->ah_countryCode);
	HALDEBUG(ah, "%s: available modes 0x%x\n", __func__, modesAvail);

	/* XXX Atheros has netband stuff here */
	/* XXX adhoc mode permission check+config */

	/*
	 * Ok, we're prepared now to construct the channel list.
	 *
	 * First check if world wide roaming is setup; if so we construct
	 * a channel list based on the super- instead of regulatory domain.
	 */
	if (isWorldWideRoaming(ah)) {
		if (!ath_hal_init_roaming_channels(ah,
			chans, maxchans, nchans, modeSelect & modesAvail,
			enableExtendedChannels)) {
			return AH_FALSE;
		}
		next = *nchans;
		goto done;
	}
	if (!enableOutdoor && cc == CTRY_UNITED_STATES) {
		/*
		 * The FCC mandates outdoor use begin at this frequency.
		 */
		maxChan = 5725;
	}
	/*
	 * Walk the tables and construct the channel list for the
	 * regulatory domain.
	 */
	next = 0;			/* next available slot in chans */
	for (cm = modes; cm < &modes[N(modes)]; cm++) {
		const REG_DMN_WM_FREQ_TABLE* ccTable;
		u_int16_t c, chi, clo;
		u_int16_t cspread, wmode;
		u_int j;

		if ((cm->mode & modeSelect) == 0) {
			HALDEBUG(ah, "%s: skip mode 0x%x flags 0x%x\n",
				__func__, cm->mode, cm->flags);
			continue;
		}
		if ((cm->mode & modesAvail) == 0) {
			HALDEBUG(ah, "%s: !avail mode 0x%x flags 0x%x\n",
				__func__, cm->mode, cm->flags);
			continue;
		    }
		/*
		 * Ask the hardware for the range of frequencies it
		 * supports.  If the specified channel falls outside
		 * that range, skip it.
		 */
		if (!ath_hal_getChannelEdges(ah, cm->flags, &clo, &chi)) {
			/* channel not supported by hardware, skip it */
			HALDEBUG(ah, "%s: channels 0x%x not supported by "
				" hardware\n", __func__, cm->flags);
			continue;
		}
		/*
		 * Find the appropriate table for the operating the
		 * mode in this regulatory domain this domain and
		 * walk the available frequencies to see which should
		 * be recorded as potential channels.
		 */
		ccTable = getRdTable(ah,
			AH_PRIVATE(ah)->ah_countryCode, cm->flags);
		cspread = getChannelSpread(ah, cm->flags);
		wmode = getRdMode(ah,
			AH_PRIVATE(ah)->ah_countryCode, cm->flags);
		HALDEBUG(ah, "%s: table %p (%d entries) spread %u wmode 0x%x\n",
			__func__, ccTable, ccTable ? ccTable->entries : -1,
			cspread, wmode);
		for (j = 0; j < ccTable->entries; j++) {
			HALDEBUG(ah, "%s: low %u high %u\n", __func__,
				ccTable->chanGroup[j].lowChannel,
				ccTable->chanGroup[j].highChannel);
			for (c = ccTable->chanGroup[j].lowChannel;
			     c <= ccTable->chanGroup[j].highChannel;
			     c += cspread) {
				HAL_CHANNEL cv;

				if (!(clo <= c && c <= chi)) {
					HALDEBUG(ah, "%s: c %u out of range [%u..%u]\n",
						__func__, c, clo, chi);
					continue;
				}
				if (c > maxChan) {
					HALDEBUG(ah, "%s: c %u > maxChan %u\n",
						__func__, c, maxChan);
					continue;
				}
				if (IS_11G_CH14(c, cm->flags)) {
					HALDEBUG(ah, "%s: IS_11G_CH14\n",
						__func__);
					continue;
				}
				if (!isEcmChannel(c, enableExtendedChannels)) {
					HALDEBUG(ah, "%s: not ECM channel\n",
						__func__);
					continue;
				}
				if (next >= maxchans) {
					HALDEBUG(ah, "%s: too many channels "
					    "for channel table\n", __func__);
					goto done;
				}
				cv.channel = c;
				cv.channelFlags = cm->flags;
				if (c == CHANNEL_14) {
					cv.channelFlags &= ~CHANNEL_G;
					cv.channelFlags |= CHANNEL_B;
				}
				cv.channelFlags &= ~CHANNEL_PASSIVE;
				if (isPassiveScanChannel(ah, wmode, &cv)) {
					cv.channelFlags |= CHANNEL_PASSIVE;
				} else if (
				    (cm->mode & (HAL_MODE_11B|HAL_MODE_11G)) &&
				    isEcmChannel(c, AH_TRUE) &&
				    isEcmChannelPassive(ah, c)) {
					cv.channelFlags |= CHANNEL_PASSIVE;
				}
				HALDEBUG(ah, "%s: add channel %u flags 0x%x\n",
					__func__, c, cv.channelFlags);
				chans[next++] = cv;
				/*
				 * If this is a Turbo channel and the request
				 * was for Turbo _and_ A channels then replicate
				 * the Turbo channel as an A channel as the
				 * tables do no explicitly specify A channels.
				 * The Atheros code accepts A use on any Turbo
				 * channel while we require explicit
				 * mention in the channel table.
				 */
				if (IS_CHAN_T(&chans[next-1]) &&
				    (modeSelect & HAL_MODE_11A)) {
					if (next >= maxchans) {
						HALDEBUG(ah, "%s: too many "
							"channels for channel"
							" table\n", __func__);
						goto done;
					}
					cv.channelFlags &= ~CHANNEL_TURBO;
					chans[next] = cv;
					HALDEBUG(ah,
					    "%s: add channel %u flags 0x%x\n",
					    __func__, c, cv.channelFlags);
					next++;
				}
			}
		}
	}
done:
	/*
	 * Sort channels from low to high frequency.
	 * We do this to optimize later lookups.
	 */
	if (next != 0) {
		int i;

		ath_hal_sort(chans, next, sizeof (HAL_CHANNEL), chansort);
		if (next > N(AH_PRIVATE(ah)->ah_channels)) {
			HALDEBUG(ah, "%s: too many channels %u; "
				"truncating to %u\n", __func__, next,
				N(AH_PRIVATE(ah)->ah_channels));
			next = N(AH_PRIVATE(ah)->ah_channels);
		}
		/*
		 * Keep a private copy of the channel list so we can
		 * constrain future requests to only these channels.
		 */
		AH_PRIVATE(ah)->ah_nchan = next;
		for (i = 0; i < next; i++) {
			HAL_CHANNEL_INTERNAL *ichan =
				&AH_PRIVATE(ah)->ah_channels[i];
			ichan->channel = chans[i].channel;
			ichan->channelFlags = chans[i].channelFlags;
		}
	}
	*nchans = next;
	/* XXX copy private setting to public area */
	ah->ah_countryCode = AH_PRIVATE(ah)->ah_countryCode;
	return (next != 0);		/* FALSE if no channels found */
}

/*
 * Return whether or not the specified channel is ok to use
 * based on the current regulatory domain constraints.
 */
HAL_CHANNEL_INTERNAL *
ath_hal_checkchannel(struct ath_hal *ah, const HAL_CHANNEL *c)
{
	HAL_CHANNEL_INTERNAL *base, *cc;
	/* NB: be wary of user-specified channel flags */
	int flags = c->channelFlags & CHANNEL_ALL;
	int n, lim;

	HALDEBUG(ah, "%s: channel %u/0x%x (0x%x) requested\n",
		__func__, c->channel, c->channelFlags, flags);

	/*
	 * Check current channel to avoid the lookup.
	 */
	cc = AH_PRIVATE(ah)->ah_curchan;
	if (cc != AH_NULL && cc->channel == c->channel &&
	    (cc->channelFlags & CHANNEL_ALL) == flags)
		return cc;

	/* binary search based on known sorting order */
	base = AH_PRIVATE(ah)->ah_channels;
	n = AH_PRIVATE(ah)->ah_nchan;
	/* binary search based on known sorting order */
	for (lim = n; lim != 0; lim >>= 1) {
		HAL_CHANNEL_INTERNAL *cc = &base[lim>>1];
		int d = c->channel - cc->channel;
		if (d == 0) {
			if ((cc->channelFlags & CHANNEL_ALL) == flags)
				return cc;
			d = flags - (cc->channelFlags & CHANNEL_ALL);
		}
		HALDEBUG(ah, "%s: channel %u/0x%x d %d\n", __func__,
			cc->channel, cc->channelFlags, d);
		if (d > 0) {
			base = cc + 1;
			lim--;
		}
	}
	HALDEBUG(ah, "%s: no match\n", __func__);
	return AH_NULL;
}

/*
 * If super domain is configured in the EEPROM but the
 * the country code remains undefined  and we can't find
 * any 11d beacon or any info that can tell us what the
 * regulatory domain is, then use the channel to identify
 * a regulatory domain table.
 */
static const REG_DMN_WM_CHANNEL_GROUP *
findChannelGroup(struct ath_hal *ah, HAL_CHANNEL *chan)
{
	const REG_DMN_WM_FREQ_TABLE *ccTable;
	u_int16_t c = chan->channel;
	int i;

	if (AH_PRIVATE(ah)->ah_countryCode == CTRY_DEFAULT) {
		u_int16_t rd = getEepromRD(ah);
		rd = (rd & COUNTRY_ERD_FLAG) ? 0 : rd & SUPER_DOMAIN_MASK;
		if (!rd)
			goto done;
		for (i = 0; i < N(superDomainTbl); i++) {
			int j, k, numRd;

			if (superDomainTbl[i].domain != rd)
				continue;
			ccTable = getCcTable(ah, chan->channelFlags);
			numRd  = getCcEntries(ah, chan->channelFlags);
			for (j = 0; j < numRd; j++) {
				for (k = 0; k < ccTable[j].entries; k++) {
					const REG_DMN_WM_CHANNEL_GROUP*
						cg = &ccTable[j].chanGroup[k];
					if (cg->lowChannel <= c &&
					    c <= cg->highChannel)
						return &ccTable[j].chanGroup[k];
				}
			}
		}
	}
done:
	ccTable = getRdTable(ah, AH_PRIVATE(ah)->ah_countryCode,
		chan->channelFlags);
	for (i = 0; i < ccTable->entries; i++) {
		/* Find this channel's entry */
		if (ccTable->chanGroup[i].lowChannel <= c &&
		    c <= ccTable->chanGroup[i].highChannel)
			return &ccTable->chanGroup[i];
	}
	return AH_NULL;
}

/*
 * Return the max allowed antenna gain and apply any regulatory
 * domain specific changes.
 *
 * NOTE: a negative reduction is possible in RD's that only
 * measure radiated power (e.g., ETSI) which would increase
 * that actual conducted output power (though never beyond
 * the calibrated target power).
 */
u_int
ath_hal_getantennareduction(struct ath_hal *ah, HAL_CHANNEL *chan, u_int twiceGain)
{
	const REG_DMN_WM_CHANNEL_GROUP *cg = findChannelGroup(ah, chan);
	if (cg) {
		int8_t antennaMax = twiceGain - (cg->antennaMax * 2);
		return (antennaMax < 0) ? 0 : antennaMax;
	} else {
		/* Failed to find the correct index - may be a debug channel */
		return 0;
	}
}

/*
 * Search the 802.11d country information element for a
 * max tx power setting for the specified channel.  If none
 * is found then we return the setting from the regulatory
 * domain table.
 */
static u_int8_t
get11dPower(struct ath_hal *ah, HAL_CHANNEL *chan,
	const REG_DMN_WM_CHANNEL_GROUP *cg)
{
	COUNTRY_INFO_LIST *cl;
	u_int16_t c;
	int j, k, len;
	u_int8_t maxTxPwr;

	HALASSERT(AH_PRIVATE(ah)->ah_cc11d);

	cl = &AH_PRIVATE(ah)->ah_cc11dInfo;
	len = cl->len - 3;
	for (j = 0; len >= 3; len -= 3, j++) {
		c = cl->band[j].schan;
		maxTxPwr = cl->band[j].maxtxpwr;
		for (k = 0; k < cl->band[j].nchan; k++) {
			if (maxTxPwr && maxTxPwr <= cg->powerDfs &&
			    chan->channel == ath_hal_ieee2mhz(c, chan->channelFlags))
				return maxTxPwr;
		}
	}
	/* fallback to regdomain table setting */
	return cg->powerDfs;
}

/*
 * Return the channel's power (in dBm) based on the regulatory table.
 *
 * For 802.11d, use the 802.11d max tx power setting if the
 * channel matches and its value is less than the one in table,
 * For common mode, use the tx power setting for that frequency
 * Otherwise, use the regulatory domain's table setting
 */
u_int
ath_hal_getchannelpower(struct ath_hal *ah, HAL_CHANNEL *chan)
{
	const REG_DMN_WM_CHANNEL_GROUP *cg;

	if (AH_PRIVATE(ah)->ah_commonMode) {
		int j;

		if (IS_CHAN_2GHZ(chan))
			return 18;
		for (j = 0; j < N(cmPwrTbl); j++)
			if (chan->channel >= cmPwrTbl[j].lchan &&
			    chan->channel < cmPwrTbl[j].hchan)
				return cmPwrTbl[j].pwrLvl;
	}
	cg = findChannelGroup(ah, chan);
	if (cg) {
		if (AH_PRIVATE(ah)->ah_cc11d)
			return get11dPower(ah, chan, cg);
		else
			return cg->powerDfs;
	} else {
		/* Failed to find the correct index - may be a debug channel */
		HALDEBUG(ah, "%s: unknown channel, defaulting to 5dBm\n",
			__func__);
		return 5;
	}
}

/*
 * If super domain is configured in the EEPROM but the
 * the country code remains undefined  and we can't find
 * any 11d beacon or any info that can tell us what the
 * regulatory domain is.
 * Then use the channel to track down a regulatory domain
 * table.
 */
static const REG_DMN_WM_FREQ_TABLE *
findRdTable(struct ath_hal *ah, HAL_CHANNEL *chan)
{
	if (AH_PRIVATE(ah)->ah_countryCode == CTRY_DEFAULT) {
		u_int16_t rd = getEepromRD(ah);
		u_int16_t c = chan->channel;
		int i;

		rd = (rd & COUNTRY_ERD_FLAG) ? 0 : rd & SUPER_DOMAIN_MASK;
		if (!rd)
			goto done;
		for (i = 0; i < N(superDomainTbl); i++) {
			const REG_DMN_WM_FREQ_TABLE *ccTable;
			int j, k, numRd;

			if (superDomainTbl[i].domain != rd)
				continue;
			ccTable = getCcTable(ah, chan->channelFlags);
			numRd  = getCcEntries(ah, chan->channelFlags);
			for (j = 0; j < numRd; j++) {
				for (k = 0; k < ccTable[j].entries; k++) {
					const REG_DMN_WM_CHANNEL_GROUP*
						cg = &ccTable[j].chanGroup[k];
					if (cg->lowChannel <= c &&
					    c <= cg->highChannel)
						return &ccTable[j];
				}
			}
		}
	}
done:
	return getRdTable(ah, AH_PRIVATE(ah)->ah_countryCode,
		chan->channelFlags);
}

/*
 * Return the test group from the specified channel from
 * the regulatory table.
 *
 * TODO: CTL for 11B CommonMode when regulatory domain is unknown
 */
u_int
ath_hal_getctl(struct ath_hal *ah, HAL_CHANNEL *chan)
{
	u_int ctl;

#ifdef notyet
	/* Special CTL to signify WWR SKU without a known country */
	if (AH_PRIVATE(ah)->ah_countryCode == CTRY_DEFAULT && isWwrSKU(ah)) {
		if (IS_CHAN_B(cflags)) {
			ctl = SD_NO_CTL | CTL_11B;
		} else if (IS_CHAN_G(cflags)) {
			ctl = SD_NO_CTL | CTL_11G;
		} else if (IS_CHAN_108G(cflags)) {
			ctl = SD_NO_CTL | CTL_108G;
		} else if (IS_CHAN_T(cflags)) {
			ctl = SD_NO_CTL | CTL_TURBO;
		} else {
			ctl = SD_NO_CTL | CTL_11A;
		}
	} else {
#endif
		ctl = findRdTable(ah, chan)->conformanceTestLimit;
		/* Atheros change# 73449: limit 11G OFDM power */
		if (IS_CHAN_PUREG(chan) && (ctl & 0xf) == CTL_11B)
			ctl = (ctl &~ 0xf) | CTL_11G;
#ifdef notyet
	}
#endif
	return ctl;
}

/*
 * Return the mask of available modes based on the hardware
 * capabilities and the specified country code.
 */
u_int
ath_hal_getwirelessmodes(struct ath_hal *ah, HAL_CTRY_CODE cc)
{
	u_int mode;

	mode = ath_hal_getWirelessModes(ah);
	if (mode & HAL_MODE_TURBO) {
		if (!isCountry11aTurbo(ah, cc))
		    mode &= ~HAL_MODE_TURBO;
		if (!isCountry11gTurbo(ah, cc))
		    mode &= ~HAL_MODE_108G;
	}
	if ((mode & HAL_MODE_11A) && !isCountry11a(ah, cc))
		mode &= ~HAL_MODE_11A;
	if ((mode & HAL_MODE_11G) && !isCountry11g(ah, cc))
		mode &= ~HAL_MODE_11G;
	return mode;
}

/*
 * Return whether or not a noise floor check is required in
 * the current regulatory domain for the specified channel.
 */
u_int
ath_hal_getnfcheckrequired(struct ath_hal *ah, HAL_CHANNEL *chan)
{
	/*
	 * The following table defines regulatory domain that require
	 * noise floor calibration
	 */
	static const HAL_REG_DOMAIN nfcRdTbl[] = {
		MKK1_MKKA,
		MKK2_MKKA,
		MKK1_MKKB,
		MKK1_FCCA,
		MKK1_MKKA1,
		MKK1_MKKA2    
	};
	HAL_REG_DOMAIN rd;
	int i;

	rd = getWmRD(ah, AH_PRIVATE(ah)->ah_countryCode, chan->channelFlags);
	for (i = 0; i < N(nfcRdTbl); i++)
		if (nfcRdTbl[i] == rd)
			return AH_TRUE;
	return AH_FALSE;
}

/*
 * Insertion sort.
 */
#define swap(_a, _b, _size) {			\
	u_int8_t *s = _b;			\
	int i = _size;				\
	do {					\
		u_int8_t tmp = *_a;		\
		*_a++ = *s;			\
		*s++ = tmp;			\
	} while (--i);				\
	_a -= _size;				\
}

static void
ath_hal_sort(void *a, size_t n, size_t size, ath_hal_cmp_t *cmp)
{
	u_int8_t *aa = a;
	u_int8_t *ai, *t;

	for (ai = aa+size; --n >= 1; ai += size)
		for (t = ai; t > aa; t -= size) {
			u_int8_t *u = t - size;
			if (cmp(u, t) <= 0)
				break;
			swap(u, t, size);
		}
}
