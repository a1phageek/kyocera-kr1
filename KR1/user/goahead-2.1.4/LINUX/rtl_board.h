/*
 * Copyright c                Realtek Semiconductor Corporation, 2003
 * All rights reserved.
 * 
 * $Header: /home/cvsroot/uClinux-dist/user/goahead-2.1.4/LINUX/rtl_board.h,v 1.34 2004/07/14 13:55:59 chenyl Exp $
 *
 * $Author: chenyl $
 *
 * Abstract: rtl_board.h -- board config init implementations
 *
 * $Log: rtl_board.h,v $
 * Revision 1.34  2004/07/14 13:55:59  chenyl
 * +: web page for MN queue
 *
 * Revision 1.33  2004/07/07 05:12:36  chhuang
 * +: add a new WAN type (DHCP+L2TP). But not complete yet!!
 *
 * Revision 1.32  2004/07/06 06:20:33  chhuang
 * +: add rate limit
 *
 * Revision 1.31  2004/06/08 10:57:40  cfliu
 * +: Add WLAN dual mode support. Not yet completed...
 *
 * Revision 1.30  2004/05/28 09:49:16  yjlou
 * +: support Protocol-Based NAT
 *
 * Revision 1.29  2004/05/20 08:43:55  chhuang
 * add Web Page for QoS
 *
 * Revision 1.28  2004/05/12 05:15:05  tony
 * support PPTP/L2TP in RTL865XB.
 *
 * Revision 1.27  2004/04/08 13:18:12  tony
 * add PPTP/L2TP routine for MII lookback port.
 *
 * Revision 1.26  2004/03/31 01:59:36  tony
 * add L2TP wan type UI pages.
 *
 * Revision 1.25  2004/03/03 03:43:32  tony
 * add static routing table in turnkey.
 *
 * Revision 1.24  2004/02/03 08:14:34  tony
 * add UDP Blocking web UI configuration.
 *
 * Revision 1.23  2004/01/29 09:46:34  tony
 * modify serverp_tableDriverAccess(), make it support multiple session.
 * add protocol type in server port UI.
 *
 * Revision 1.22  2004/01/08 12:13:45  tony
 * add Port Range into Server Port.
 * support Server Port for multiple session UI.
 *
 * Revision 1.21  2004/01/07 09:10:04  tony
 * add PPTP Client UI in Config Wizard.
 *
 * Revision 1.20  2003/12/19 04:33:01  tony
 * add Wireless Lan config pages: Basic Setting, Advance Setting, Security, Access Control, WDS
 *
 * Revision 1.19  2003/12/01 12:35:52  tony
 * make ALG is able to be configured by users(Web GUI).
 *
 * Revision 1.18  2003/10/24 10:25:58  tony
 * add DoS attack interactive webpage,
 * FwdEngine is able to get WAN status by rtl8651_wanStatus(0:disconnect,1:connect)
 *
 * Revision 1.17  2003/10/03 12:27:35  tony
 * add NTP time sync routine in management web page.
 *
 * Revision 1.16  2003/10/01 09:12:03  tony
 * move all the extern declare of factory deafult function to rtl_board.h
 *
 * Revision 1.15  2003/10/01 05:57:31  tony
 * add URL Filter routine
 *
 * Revision 1.14  2003/09/30 12:19:49  tony
 * arrange board_ipUpEventTableDriverAccess(), let upnp,ddns,dmz be able to start after dhcpc/pppoe started.
 *
 * Revision 1.13  2003/09/30 08:56:29  tony
 * remove newServerpCfgParam[] from flash, rename ram PPPoeCfg to ramPppoeCfgParam
 *
 * Revision 1.12  2003/09/29 12:58:34  tony
 * add DDNS start/stop routine.
 *
 * Revision 1.11  2003/09/29 05:06:27  orlando
 * add board_ipUp/DownEventTableDriverAccess:
 *
 * Revision 1.10  2003/09/25 02:15:32  orlando
 * Big Change
 *
 * Revision 1.9  2003/09/24 07:10:30  rupert
 * rearrange init sequence
 *
 * Revision 1.8  2003/09/23 11:44:33  tony
 * add UPNP daemon start/stop routine.
 *
 * Revision 1.7  2003/09/23 03:47:29  tony
 * add ddnsCfgParam,ddnsDefaultFactory,ddns_init
 *
 * Revision 1.6  2003/09/22 08:01:45  tony
 * add UPnP Web Configuration Function Routine
 *
 * Revision 1.5  2003/09/22 06:33:37  tony
 * add syslogd process start/stop by CGI.
 * add eventlog download/clear routine.
 *
 * Revision 1.4  2003/09/19 06:06:51  tony
 * *** empty log message ***
 *
 * Revision 1.3  2003/09/19 01:43:50  tony
 * add dmz routine
 *
 * Revision 1.2  2003/09/18 02:05:45  tony
 * modify pppoeCfgParam to array
 *
 * Revision 1.1.1.1  2003/08/27 06:20:15  rupert
 * uclinux initial
 *
 * Revision 1.1.1.1  2003/08/27 03:08:53  rupert
 *  initial version 
 *
 * Revision 1.13  2003/07/03 05:59:55  tony
 * add configChanged global param.
 *
 * Revision 1.12  2003/07/01 10:25:00  orlando
 * call table driver server port and acl API in board.c
 *
 * Revision 1.11  2003/07/01 03:09:06  tony
 * add aclGlobalCfgParam structure.
 * modify aclCfgParam and serverpCfgParam structure.
 *
 * Revision 1.10  2003/06/30 07:46:11  tony
 * add ACL and Server_Port structure
 *
 * Revision 1.9  2003/06/23 10:57:23  elvis
 * change include path of  rtl_types.h
 *
 * Revision 1.8  2003/06/20 12:59:45  tony
 * add dhcp client
 *
 * Revision 1.7  2003/06/16 08:08:30  tony
 * add dhcps & dns function
 *
 * Revision 1.6  2003/06/06 11:57:12  orlando
 * add pppoe cfg.
 *
 * Revision 1.5  2003/06/06 06:31:11  idayang
 * add mgmt table in cfgmgr.
 *
 * Revision 1.4  2003/06/03 10:57:05  orlando
 * add nat table in cfgmgr.
 *
 * Revision 1.3  2003/05/20 08:44:35  elvis
 * change the include path of "rtl_types.h"
 *
 * Revision 1.2  2003/05/05 05:02:45  orlando
 * make ifCfgParam[] not extern.
 *
 * Revision 1.1  2003/05/02 08:51:08  orlando
 * merge cfgmgr with old board.c/board.h.
 *
 */

#ifndef _rtl_board_h_
#define _rtl_board_h_

#define CONFIG_CHANGE_FW_VER	1
#define CONFIG_CHANGE_LD_VER	(1<<1)
#define CONFIG_CHANGE_NAT		(1<<2)
#define CONFIG_CHANGE_HW_ACCEL	(1<<3)
#define CONFIG_CHANGE_LAN_IP	(1<<4)
#define CONFIG_CHANGE_LAN_MASK	(1<<5)
#define CONFIG_CHANGE_LAN_MAC	(1<<6)
#define CONFIG_CHANGE_DHCPS		(1<<7)
#define CONFIG_CHANGE_CONN_TYPE	(1<<8)
#define CONFIG_CHANGE_WAN_IP	(1<<9)
#define CONFIG_CHANGE_WAN_MASK	(1<<10)
#define CONFIG_CHANGE_WAN_GW	(1<<11)
#define CONFIG_CHANGE_WAN_DNS	(1<<12)
#define CONFIG_CHANGE_WAN_MAC	(1<<13)

#include "rtl_types.h"
#include "board.h"
#include <linux/config.h>


/* EXPORTED FUNCTION PROTOTYPES */
extern void vlanCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void ifCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void routeCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void arpCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void natCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void mgmtCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void pppoeCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void aclCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void aclGlobalCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void serverpCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void dhcpsCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void dhcpcCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void pppCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);//peter and howard
extern void dnsCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void naptCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void dosCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void algCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void dmzCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void logCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void upnpCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void urlfilterCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void macfilterCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void tmssCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void tmssCInfoCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize); // Dino
extern void ddnsCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void specialapCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void timeCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void wlanCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void wlanCfgFactoryDefaultFunction2(void * pBuf, uint32 * pBufSize);
extern void tbldrvCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
extern void pptpCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void l2tpCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void dhcpL2tpCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
#endif
extern void udpblockCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void routingCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void qosCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void pbnatCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
extern void rateLimitCfgFactoryDefaultFunction(void *pBuf, uint32 *pBufSize);
extern void ratioQoSCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize);
/* semaphore */
uint32 rtl8651_sem_init(uint8 val);
void rtl8651_sem_wait(uint32 sem_type);
void rtl8651_sem_post(uint32 sem_type);

uint32 vlan_init(void);
uint32 if_init(void);
uint32 staticroute_init(void);
uint32 arp_init(void);
uint32 nat_init(void);
uint32 dhcpc_init(void);
uint32 dns_init(void);
uint32 dmz_init(void);
uint32 log_init(void);
uint32 upnp_init(void);
uint32 time_init(void);
uint32 urlfilter_init(void);
uint32 ddns_init(void);
uint32 dhcps_init(void);
uint32 pppoecfg_init(void);
uint32 acl_init(void);
uint32 acl_tableDriverAccess(void);
uint32 serverp_init(void);
uint32 serverp_tableDriverAccess(uint32 dsid);
uint32 specialap_init(void);
uint32 wlan_init(void);
uint32 udpblock_init(void);
uint32 routing_init(void);
uint32 board_ipUpEventTableDriverAccess(void);
uint32 board_ipDownEventTableDriverAccess(void);
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)	
uint32 pptp_init(void);
uint32 l2tp_init(void);
#endif
uint32 pbnat_init(void);

extern ifCfgParam_t			ramWanCfgParam;
extern ifCfgParam_t			ramLanCfgParam;
extern dhcpsCfgParam_t		ramDhcpsCfgParam;
extern dhcpcCfgParam_t		ramDhcpcCfgParam;
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
extern pptpCfgParam_t		ramPptpCfgParam;
extern l2tpCfgParam_t		ramL2tpCfgParam;
#endif
extern serverpCfgParam_t	ramServerpCfgParam[][];
extern pppoeCfgParam_t		ramPppoeCfgParam[];
extern uint32 				configChanged;
#endif /* _rtl_board_h_ */
