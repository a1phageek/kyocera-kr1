/*
 * Copyright c                Realtek Semiconductor Corporation, 2003
 * All rights reserved.
 * 
 * $Header: /home/cvsroot/uClinux-dist/user/goahead-2.1.4/LINUX/rtl_cfgmgr.h,v 1.26 2004/08/13 06:03:19 cfliu Exp $
 *
 * $Author: cfliu $
 *
 * Abstract: rtl_cfgmgr.h -- configuration manager module external header file
 *
 * $Log: rtl_cfgmgr.h,v $
 * Revision 1.26  2004/08/13 06:03:19  cfliu
 * -: Remove WLAN2 cfg table
 *
 * Revision 1.25  2004/07/14 13:55:59  chenyl
 * +: web page for MN queue
 *
 * Revision 1.24  2004/07/07 05:56:39  tony
 * *: fix tabid bug.
 *
 * Revision 1.23  2004/07/07 05:12:36  chhuang
 * +: add a new WAN type (DHCP+L2TP). But not complete yet!!
 *
 * Revision 1.22  2004/07/06 06:20:58  chhuang
 * +: add rate limit
 *
 * Revision 1.21  2004/06/08 10:57:57  cfliu
 * +: Add WLAN dual mode support. Not yet completed...
 *
 * Revision 1.20  2004/05/28 09:49:16  yjlou
 * +: support Protocol-Based NAT
 *
 * Revision 1.19  2004/05/20 08:43:55  chhuang
 * add Web Page for QoS
 *
 * Revision 1.18  2004/05/19 08:40:50  orlando
 * checkin init button,diag led,bicolor led related code
 *
 * Revision 1.17  2004/05/12 05:15:05  tony
 * support PPTP/L2TP in RTL865XB.
 *
 * Revision 1.16  2004/04/08 13:18:12  tony
 * add PPTP/L2TP routine for MII lookback port.
 *
 * Revision 1.15  2004/03/31 01:59:36  tony
 * add L2TP wan type UI pages.
 *
 * Revision 1.14  2004/03/03 03:43:32  tony
 * add static routing table in turnkey.
 *
 * Revision 1.13  2004/02/03 08:14:34  tony
 * add UDP Blocking web UI configuration.
 *
 * Revision 1.12  2004/01/07 09:10:04  tony
 * add PPTP Client UI in Config Wizard.
 *
 * Revision 1.11  2003/12/19 04:33:01  tony
 * add Wireless Lan config pages: Basic Setting, Advance Setting, Security, Access Control, WDS
 *
 * Revision 1.10  2003/12/01 12:35:52  tony
 * make ALG is able to be configured by users(Web GUI).
 *
 * Revision 1.9  2003/10/24 10:25:58  tony
 * add DoS attack interactive webpage,
 * FwdEngine is able to get WAN status by rtl8651_wanStatus(0:disconnect,1:connect)
 *
 * Revision 1.8  2003/10/03 12:27:35  tony
 * add NTP time sync routine in management web page.
 *
 * Revision 1.7  2003/10/01 05:57:31  tony
 * add URL Filter routine
 *
 * Revision 1.6  2003/09/23 03:47:29  tony
 * add ddnsCfgParam,ddnsDefaultFactory,ddns_init
 *
 * Revision 1.5  2003/09/22 08:01:45  tony
 * add UPnP Web Configuration Function Routine
 *
 * Revision 1.4  2003/09/22 06:33:37  tony
 * add syslogd process start/stop by CGI.
 * add eventlog download/clear routine.
 *
 * Revision 1.3  2003/09/19 06:06:51  tony
 * *** empty log message ***
 *
 * Revision 1.2  2003/09/19 01:43:50  tony
 * add dmz routine
 *
 * Revision 1.1.1.1  2003/08/27 06:20:15  rupert
 * uclinux initial
 *
 * Revision 1.1.1.1  2003/08/27 03:08:53  rupert
 *  initial version 
 *
 * Revision 1.13  2003/07/21 05:02:34  orlando
 * check-in for v0.3.1.
 *
 * Revision 1.12  2003/07/01 03:08:25  tony
 * add aclGlobalCfgParam factory default.
 *
 * Revision 1.11  2003/06/30 07:46:40  tony
 * add ACL and Server_Port structure
 *
 * Revision 1.10  2003/06/23 11:03:30  elvis
 * change include path of  rtl_types.h
 *
 * Revision 1.9  2003/06/20 13:00:28  tony
 * add dhcp client
 *
 * Revision 1.8  2003/06/16 08:08:43  tony
 * add dhcps & dns function
 *
 * Revision 1.7  2003/06/06 11:57:41  orlando
 * add pppoe cfg.
 *
 * Revision 1.6  2003/06/06 06:30:26  idayang
 * add mgmt table in cfgmgr.
 *
 * Revision 1.5  2003/06/03 10:57:17  orlando
 * add nat table in cfgmgr.
 *
 * Revision 1.4  2003/05/20 08:45:21  elvis
 * change the include path of "rtl_types.h"
 *
 * Revision 1.3  2003/05/09 15:34:09  orlando
 * patch due to sysHwInit() cannot start the cfgmgr maintenance task.
 *
 * Revision 1.2  2003/05/02 09:21:00  orlando
 * make cfgmgr_init() called implicitly by 1st cfgmgr_read()
 * or cfgmgr_write() access.
 *
 * Revision 1.1  2003/04/29 14:18:10  orlando
 * cfgmgr module initial check-in.
 *
 */

#ifndef _rtl_cfgmgr_h_
#define _rtl_cfgmgr_h_

#include "rtl_types.h"
#include <linux/config.h>

/* NAMING CONSTANT DECLARATIONS */

/* module table id (tid) enumerations
 * each cfgmgr users have to register module id here !!'
 * example:
 * CGMGR_TABID_PPPOE,
 * CFGMGR_TABID_NAT,
 * CFGMGR_TABID_DHCPS,
 * CFGMGR_TABID_DHCPC,
 * CFGMGR_TABID_WEB, 
 */
enum cfgmgr_tabId_e {
	CFGMGR_TABID_NOT_USED = 0, /* do not remove this line */
	/* inter your module's ID below */
	CFGMGR_TABID_VLAN,
	CFGMGR_TABID_IF,
	CFGMGR_TABID_ROUTE,
	CFGMGR_TABID_ARP,
	CFGMGR_TABID_NAT,
	CFGMGR_TABID_MGMT,
	CFGMGR_TABID_PPPOE,
	CFGMGR_TABID_DHCPS,
	CFGMGR_TABID_DHCPC,
	CFGMGR_TABID_PPP, //peter and howard
	CFGMGR_TABID_DNS,
	CFGMGR_TABID_DMZ,
	CFGMGR_TABID_LOG,
	CFGMGR_TABID_UPNP,
	CFGMGR_TABID_DDNS,
	CFGMGR_TABID_ACL,
	CFGMGR_TABID_ACL_GLOBAL,
	CFGMGR_TABID_SERVERP,
	CFGMGR_TABID_SPECIALAP,	
	CFGMGR_TABID_TBLDRV,
	CFGMGR_TABID_URLFILTER,	
	CFGMGR_TABID_MACFILTER,          // add by Min Lee
	CFGMGR_TABID_TMSS,		 // add by Min Lee
	CFGMGR_TABID_TMSS_CLIENTINFO,    // Dino Chang 2004/12/30
	CFGMGR_TABID_DOS,	
	CFGMGR_TABID_ALG,		
	CFGMGR_TABID_TIME,
	CFGMGR_TABID_WLAN,	
	/* insert your module's ID above */
	CFGMGR_TABID_UDPBLOCK,
	CFGMGR_TABID_ROUTING,
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
	CFGMGR_TABID_PPTP,
	CFGMGR_TABID_L2TP,
#endif	
	CFGMGR_TABID_QOS,
	CFGMGR_TABID_PBNAT,
	CFGMGR_TABID_RATELIMIT,
	CFGMGR_TABID_RATIOQOS,
	CFGMGR_TABID_NAPT,	
	CFGMGR_TABID_MAX /* do not remove this line */
};

/* DATA TYPE DECLARACTION */
typedef struct {
	void (*pFactoryDefaultFunction)(void*,uint32*); /* callback funtion's prototype */
} cfgmgr_factoryDefaultFunction_t;


/* MODULE EXPORTED FUNCTION SPECIFICATION */
#if defined(CONFIG_RTL865X_INIT_BUTTON)
uint32 cfgmgr_factoryDefault(uint32); /* set all module's config to factory default values */
#else
uint32 cfgmgr_factoryDefault(void); /* set all module's config to factory default values */
#endif /* CONFIG_RTL865X_INIT_BUTTON */
uint32 cfgmgr_read(uint32 tabId, void * pBuf, uint32 bufSize);
uint32 cfgmgr_write(uint32 tabId, void * pBuf, uint32 bufSize);
uint32 cfgmgr_init(void);

#endif /* _rtl_cfgmgr_h_ */
