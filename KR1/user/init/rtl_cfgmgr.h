/*
 * Copyright c                Realtek Semiconductor Corporation, 2003
 * All rights reserved.
 * 
 * $Header: /home/cvsroot/uClinux-dist/user/init/rtl_cfgmgr.h,v 1.1.1.1 2003/08/27 06:20:19 rupert Exp $
 *
 * $Author: rupert $
 *
 * Abstract: rtl_cfgmgr.h -- configuration manager module external header file
 *
 * $Log: rtl_cfgmgr.h,v $
 * Revision 1.1.1.1  2003/08/27 06:20:19  rupert
 * uclinux initial
 *
 * Revision 1.1.1.1  2003/08/27 03:08:52  rupert
 *  initial version 
 *
 * Revision 1.13  2003/07/21 05:02:34  orlando
 * check-in for v0.3.1.
 *
 * Revision 1.12  2003/07/01 03:08:25  tysu
 * add aclGlobalCfgParam factory default.
 *
 * Revision 1.11  2003/06/30 07:46:40  tysu
 * add ACL and Server_Port structure
 *
 * Revision 1.10  2003/06/23 11:03:30  elvis
 * change include path of  rtl_types.h
 *
 * Revision 1.9  2003/06/20 13:00:28  tysu
 * add dhcp client
 *
 * Revision 1.8  2003/06/16 08:08:43  tysu
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
	CFGMGR_TABID_DNS,
	CFGMGR_TABID_ACL,
	CFGMGR_TABID_ACL_GLOBAL,
	CFGMGR_TABID_SERVERP,
	CFGMGR_TABID_SPECIALAP,
	CFGMGR_TABID_TBLDRV,
	/* insert your module's ID above */
	CFGMGR_TABID_MAX /* do not remove this line */
};

/* DATA TYPE DECLARACTION */
typedef struct {
	void (*pFactoryDefaultFunction)(void*,uint32*); /* callback funtion's prototype */
} cfgmgr_factoryDefaultFunction_t;


/* MODULE EXPORTED FUNCTION SPECIFICATION */
uint32 cfgmgr_factoryDefault(void); /* set all module's config to factory default values */
uint32 cfgmgr_read(uint32 tabId, void * pBuf, uint32 bufSize);
uint32 cfgmgr_write(uint32 tabId, void * pBuf, uint32 bufSize);
//uint32 cfgmgr_installFactoryDefault(uint32 tabid, void (*factoryDefaultFunction)(void*,uint32*)); /* so far, callback is only used for factoryDefault */
uint32 cfgmgr_init(void);

#endif /* _rtl_cfgmgr_h_ */
