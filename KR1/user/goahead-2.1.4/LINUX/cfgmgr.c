/*
 * Copyright c                Realtek Semiconductor Corporation, 2003
 * All rights reserved. 
 * 
 * $Header: /home/cvsroot/uClinux-dist/user/goahead-2.1.4/LINUX/cfgmgr.c,v 1.54 2004/08/13 06:02:02 cfliu Exp $
 *
 * $Author: cfliu $
 *
 * Abstract: cfgmgr.c -- configuration manager main source file
 * Module assumption:
 *   1. Fixed size for each module configuration table
 *   2. nv-ram area should match exactly 1 flash memory block
 *   3. array index in this module is from 1 to max. this means
 *      any array size has to be declared to be real size plus 1.
 *      and array[0] is always not used.
 * $Log: cfgmgr.c,v $
 * Revision 1.54  2004/08/13 06:02:02  cfliu
 * -: Remove WLAN card 2 factory default function
 *
 * Revision 1.53  2004/08/11 04:00:17  yjlou
 * *: _flash_init() is renamed to flashdrv_init()
 *
 * Revision 1.52  2004/08/04 14:51:18  yjlou
 * +: support booting from single 8MB/16MB flash (_SUPPORT_LARGE_FLASH_)
 *
 * Revision 1.51  2004/07/14 13:55:59  chenyl
 * +: web page for MN queue
 *
 * Revision 1.50  2004/07/13 07:13:01  cfliu
 * no message
 *
 * Revision 1.49  2004/07/06 06:19:39  chhuang
 * +: add rate limit
 *
 * Revision 1.48  2004/07/01 09:38:02  yjlou
 * *: fixed the bug taht MMU user-mode program CANNOT soft-reboot.
 *
 * Revision 1.47  2004/06/14 13:48:17  rupert
 * +: Add work properly with MMU kernel
 *
 * Revision 1.46  2004/06/11 09:20:18  tony
 * +: using share memory instead of direct access for pRomeCfgParam.
 *
 * Revision 1.45  2004/06/08 10:54:42  cfliu
 * +: Add WLAN dual mode webpages. Not yet completed...
 *
 * Revision 1.44  2004/05/28 09:49:16  yjlou
 * +: support Protocol-Based NAT
 *
 *
 * Revision 1.42  2004/05/20 08:43:55  chhuang
 * add Web Page for QoS
 *
 * Revision 1.41  2004/05/19 08:40:14  orlando
 * checkin init button,diag led,bicolor led related code
 *
 * Revision 1.40  2004/05/13 13:27:01  yjlou
 * +: loader version is migrated to "00.00.07".
 * +: new architecture for INTEL flash (code is NOT verified).
 * *: FLASH_BASE is decided by IS_REV_A()
 * -: remove flash_map.h (content moved to flashdrv.h)
 * -: remove un-necessary calling setIlev()
 *
 * Revision 1.39  2004/05/12 05:15:05  tony
 * support PPTP/L2TP in RTL865XB.
 *
 * Revision 1.38  2004/05/11 10:40:34  yjlou
 * +: runtime code supports auto-detect BDINFO and CCFG address.
 *
 * Revision 1.37  2004/04/08 13:18:12  tony
 * add PPTP/L2TP routine for MII lookback port.
 *
 * Revision 1.36  2004/03/31 01:59:36  tony
 * add L2TP wan type UI pages.
 *
 * Revision 1.35  2004/03/03 03:43:31  tony
 * add static routing table in turnkey.
 *
 * Revision 1.34  2004/02/03 08:14:34  tony
 * add UDP Blocking web UI configuration.
 *
 * Revision 1.33  2004/01/07 09:10:04  tony
 * add PPTP Client UI in Config Wizard.
 *
 * Revision 1.32  2003/12/19 04:33:01  tony
 * add Wireless Lan config pages: Basic Setting, Advance Setting, Security, Access Control, WDS
 *
 * Revision 1.30  2003/12/01 12:35:52  tony
 * make ALG is able to be configured by users(Web GUI).
 *
 * Revision 1.29  2003/10/24 10:25:58  tony
 * add DoS attack interactive webpage,
 * FwdEngine is able to get WAN status by rtl8651_wanStatus(0:disconnect,1:connect)
 *
 * Revision 1.28  2003/10/14 08:38:14  tony
 * add RomeDriver version information
 *
 * Revision 1.27  2003/10/08 10:41:43  tony
 * update Board_info address
 *
 * Revision 1.26  2003/10/03 12:27:35  tony
 * add NTP time sync routine in management web page.
 *
 * Revision 1.25  2003/10/01 13:33:10  tony
 * solve all the compiler warning message in cfgmgr.c
 *
 * Revision 1.24  2003/10/01 09:12:03  tony
 * move all the extern declare of factory deafult function to rtl_board.h
 *
 * Revision 1.23  2003/10/01 05:57:31  tony
 * add URL Filter routine
 *
 * Revision 1.22  2003/09/30 08:56:29  tony
 * remove newServerpCfgParam[] from flash, rename ram PPPoeCfg to ramPppoeCfgParam
 *
 * Revision 1.21  2003/09/25 05:54:01  tony
 * support from loader 0.0.3
 *
 * Revision 1.20  2003/09/25 03:48:35  orlando
 * add romecfg.txt file to pass romeCfgParam structure pointer for other user space application
 *
 * Revision 1.19  2003/09/25 02:15:32  orlando
 * Big Change
 *
 * Revision 1.18  2003/09/24 07:10:30  rupert
 * rearrange init sequence
 *
 * Revision 1.16  2003/09/24 05:24:34  rupert
 * remove cfgmgr_main
 *
 * Revision 1.15  2003/09/23 11:44:33  tony
 * add UPNP daemon start/stop routine.
 *
 * Revision 1.14  2003/09/23 03:47:29  tony
 * add ddnsCfgParam,ddnsDefaultFactory,ddns_init
 *
 * Revision 1.13  2003/09/22 08:01:45  tony
 * add UPnP Web Configuration Function Routine
 *
 * Revision 1.12  2003/09/22 06:33:37  tony
 * add syslogd process start/stop by CGI.
 * add eventlog download/clear routine.
 *
 * Revision 1.11  2003/09/19 06:06:51  tony
 * *** empty log message ***
 *
 * Revision 1.10  2003/09/19 01:43:50  tony
 * add dmz routine
 *
 * Revision 1.9  2003/09/18 05:12:00  hiwu
 * vfork()'s child should use _exit()
 *
 * Revision 1.8  2003/09/18 02:27:10  rupert
 * add ip-up-down.c for ppp
 *
 * Revision 1.7  2003/09/18 02:05:45  tony
 * modify pppoeCfgParam to array
 *
 * Revision 1.6  2003/09/17 12:28:04  rupert
 * Add PPPoE Advance Page and redefinition some function
 *
 * Revision 1.5  2003/09/12 02:25:58  rupert
 * pppd for auto reconnection
 *
 * Revision 1.4  2003/09/04 09:33:40  rupert
 * Move Versoin To Top Makefile
 *
 * Revision 1.3  2003/09/04 07:38:46  rupert
 * Add PPPoE Initialization
 *
 * Revision 1.2  2003/08/29 06:32:54  rupert
 *
 *
 * 		remove unecesarry file
 *
 * Revision 1.1.1.1  2003/08/27 06:20:15  rupert
 * uclinux initial
 *
 * Revision 1.1.1.1  2003/08/27 03:08:53  rupert
 *  initial version 
 *
 * Revision 1.24  2003/07/21 05:06:51  orlando
 * check-in for v0.3.1.
 *
 * Revision 1.23  2003/07/09 05:22:13  orlando
 * reset to factory default if new firmware's table size is
 * different from existing size. this occurs when new module
 * or new table structure is being designed.
 * after a factory default during read, system restart is done
 * automatically. these will make new firmware boot smoothly over
 * old firmare on the flashmemory.
 *
 * Revision 1.22  2003/07/08 12:41:51  orlando
 * previous (1.21) version cvs log error.
 * 1.21 log: change cfgmgr task priority from 200 to 50, which
 * is equal to netTask priority.
 *
 * Revision 1.21  2003/07/08 12:38:49  orlando
 * tune "if_init"'s static wan and dhcp client wan init code.
 *
 * Revision 1.20  2003/07/01 10:26:55  orlando
 * remove #pragma pack(1) and #pragma pack(), which might
 * cause problem in vxwork gnu compiler.
 *
 * Revision 1.19  2003/07/01 03:08:18  tony
 * add aclGlobalCfgParam factory default.
 *
 * Revision 1.18  2003/06/30 07:46:27  tony
 * add ACL and Server_Port structure
 *
 * Revision 1.17  2003/06/24 13:59:15  orlando
 * modify CFGMGR_BASE_ADDR definition from hard-coded
 * to FLASH_MAP_CCFG_IMAGE_ADDR.
 *
 * Revision 1.16  2003/06/23 11:11:48  elvis
 * change include path of  rtl_types.h
 *
 * Revision 1.15  2003/06/20 13:02:49  tony
 * add dhcp client
 *
 * Revision 1.14  2003/06/17 07:00:01  orlando
 * trying to avoid word aligned load/store.
 *
 * Revision 1.13  2003/06/16 08:09:19  tony
 * add dhcps & dns function
 *
 * Revision 1.12  2003/06/06 11:58:04  orlando
 * add pppoe cfg.
 *
 * Revision 1.11  2003/06/06 06:16:43  idayang
 * add mgmt table in cfgmgr.
 *
 * Revision 1.10  2003/06/03 10:58:14  orlando
 * add nat table in cfgmgr.
 *
 * Revision 1.9  2003/05/23 06:10:27  elvis
 * avoid multiple definition for macros max() and min()
 *
 * Revision 1.8  2003/05/20 08:52:03  elvis
 * change the include path of "rtl_types.h"
 *
 * Revision 1.7  2003/05/19 07:34:45  orlando
 * remove cfgmgr_factoryDefault()'s message.
 *
 * Revision 1.6  2003/05/09 15:34:37  orlando
 * patch due to sysHwInit() cannot start the cfgmgr maintenance task.
 *
 * Revision 1.5  2003/05/06 12:30:44  orlando
 * modify to work with new boot/loader.
 *
 * Revision 1.4  2003/05/06 07:32:08  orlando
 * remove cfgmgr_integrityCheck() message during
 * normal case.
 *
 * Revision 1.3  2003/05/02 09:20:48  orlando
 * make cfgmgr_init() called implicitly by 1st cfgmgr_read()
 * or cfgmgr_write() access.
 *
 * Revision 1.2  2003/04/30 01:44:24  orlando
 * adjust helpful message.
 *
 * Revision 1.1  2003/04/29 14:15:30  orlando
 * cfgmgr module initial check-in.
 *
 */

#include "rtl_types.h"
#include "rtl_flashdrv.h"
#include "flashdrv.h"
#include "rtl_cfgmgr.h"
#include "rtl_board.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <wait.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <linux/if_ether.h>
#include <linux/config.h>
#include <net/route.h>
#include <re865x.h>
#include <version.h>
#include	"../wsIntrn.h"
#include "rtl_image.h"
#include <signal.h>
#include "cfg.h"	//Added by Nick HO

#ifndef PKG_VERSION
#define PKG_VERSION	"9.9.9"
#endif
#ifndef ROMEDRIVER_VERSION
#define ROMDRIVER_VERSION	"9.9"
#endif

#define RIMR	(*((volatile uint32 *)0xbd012000))
struct pppoeCfgParam_s  ramPPPoeCfg[MAX_PPPOE_SESSION];

/* defined in board.c */
#ifdef CFG_TLV
extern romeCfgParam_t * pRomeCfgParam;		//Added by Nick HO
#endif
 
extern int32 bdinfo_getMac(macaddr_t *mac_P);

//void hangup(webs_t wp, char_t *path, char_t *query);
/* NAMING CONSTANT DECLARARTIONS */
enum cfgmgr_version_e {
	CFGMGR_VERSION_V100=0
};

enum cfgmgr_opmode_e {
	CFGMGR_OPMODE_DISABLE=0,
	CFGMGR_OPMODE_ENABLE
};

enum cfgmgr_dirty_e {
	CFGMGR_NOT_DIRTY=0,
	CFGMGR_DIRTY
};
/* NAMING CONSTANT DECLARARTIONS */
#define CFGMGR_SIGNATURE_LENGTH   8
#define CFGMGR_SIGNATURE          "rome" /* length should be < CFGMGR_SIGNATURE_LENGTH -1 */
#define CFGMGR_VERSION            CFGMGR_VERSION_V100
#define CFGMGR_BASE_ADDR          FLASH_MAP_CCFG_IMAGE_ADDR

// resize & relocate CFG in flash
//#define CFGMGR_SIZE  (0xA000UL)   // 40K
//#define CFGMGR_SIZE (0xE000UL)    // 56k 
//#define CFGMGR_SIZE (0x10000UL)   // 64k
//#define CFGMGR_SIZE  (0x2000UL)   /* 8k origional */
#define CFGMGR_SIZE (0x20000UL)     // 128k 



/* MACRO DECLARATIONS */
#define ALIGN_LENGTH(len, boundary) \
          (((uint32) (len) + (uint32) (boundary) -1) & ~((uint32) (boundary) - 1))
#define	CFGMGR_CKSUM_LENGTH		2
#define	CFGMGR_HALT			    while(1)

#if 1 /* turn on/off debug message */
	#define PRINT_MSG(str)      (printf str)
#else
	#define PRINT_MSG(str)      
#endif

//#define ENTER_CRITICAL(a,b) semTake(a,b)
//#define EXIT_CRITICAL(a)    semGive(a)
#define ENTER_CRITICAL(a,b)
#define EXIT_CRITICAL(a)

/* DATA TYPE DEFINITION */
typedef struct cfgmgr_moduletabDescriptor_s {
	uint32 offset;  /* offset is relative to begin of flash momory area for cfg manager */
	uint32 tabSize; /* size of this module table */
} cfgmgr_moduletabDescriptor_t;

//#pragma pack(1)
typedef struct cfgmgr_ctrl_s
{
    uint8                           signature[CFGMGR_SIGNATURE_LENGTH];
    uint8                           version;
    uint8                           rsv[5];
    uint16                          checksum; /* checksum of whole area excluding ctrl header */
    cfgmgr_moduletabDescriptor_t    descriptor[CFGMGR_TABID_MAX];
} cfgmgr_ctrl_t;
//#pragma pack()


/* LOCAL VARIABLES */
#if  defined(__mips__) && defined(__uClinux__)
static uint8 *generalBuf=(uint8*)0x80002000;//[0x10000]; /* general purpose buffer, 0x10000=64*1024=64K */
#else

static uint8 generalBuf[CFGMGR_SIZE]; /* general purpose buffer, 0x10000=64*1024=64K */
#endif
#if	defined(__mips___) && defined(__uClinux__)
#define FIX_ADDR 0x80012000
static uint8 *shadow=(uint8*)FIX_ADDR;//[CFGMGR_SIZE]; /* same size as that of flash memory area */
static cfgmgr_ctrl_t *  pCtrl = (cfgmgr_ctrl_t *)FIX_ADDR;
#else
static uint8 shadow[CFGMGR_SIZE]; /* same size as that of flash memory area */
static cfgmgr_ctrl_t *  pCtrl = (cfgmgr_ctrl_t *)shadow;
#endif


//static cfgmgr_ctrl_t *  pNvramCtrl = (cfgmgr_ctrl_t *)CFGMGR_BASE_ADDR;
#define pNvramCtrl ((cfgmgr_ctrl_t *)CFGMGR_BASE_ADDR)
//=====================================================================================================
//======  The factory Function MUST be has same sequence with the romeCfgParam_t definition  ==========
//=====================================================================================================

#ifdef CFG_TLV
static cfgmgr_factoryDefaultFunction_t factoryDefaultFunctionArray[CFGMGR_TABID_MAX] = 
{	
	{NULL},
	{vlanCfgFactoryDefaultFunction},
	{ifCfgFactoryDefaultFunction},
	{routeCfgFactoryDefaultFunction},
	{arpCfgFactoryDefaultFunction},
	{natCfgFactoryDefaultFunction},
	{pppoeCfgFactoryDefaultFunction},
	{dhcpsCfgFactoryDefaultFunction},
	{pppCfgFactoryDefaultFunction},//peter and howard
	{dhcpcCfgFactoryDefaultFunction},	
	{dnsCfgFactoryDefaultFunction},	
	{mgmtCfgFactoryDefaultFunction},
	{aclGlobalCfgFactoryDefaultFunction},
	{aclCfgFactoryDefaultFunction},
	{serverpCfgFactoryDefaultFunction},
	{specialapCfgFactoryDefaultFunction},
	{dmzCfgFactoryDefaultFunction},	
	{logCfgFactoryDefaultFunction},
	{upnpCfgFactoryDefaultFunction},	
	{ddnsCfgFactoryDefaultFunction},
	{tbldrvCfgFactoryDefaultFunction},
	{urlfilterCfgFactoryDefaultFunction},
	{macfilterCfgFactoryDefaultFunction},	// add by Min Lee
	{tmssCfgFactoryDefaultFunction},	// add by Min Lee
	{tmssCInfoCfgFactoryDefaultFunction},	// Dino Chang 2004/12/30
	{dosCfgFactoryDefaultFunction},	
	{naptCfgFactoryDefaultFunction},
	{timeCfgFactoryDefaultFunction},		
	{algCfgFactoryDefaultFunction},	
	{wlanCfgFactoryDefaultFunction},
	{udpblockCfgFactoryDefaultFunction},
	{routingCfgFactoryDefaultFunction},
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
	{pptpCfgFactoryDefaultFunction},
	{l2tpCfgFactoryDefaultFunction},	
#endif
	{qosCfgFactoryDefaultFunction},
	{rateLimitCfgFactoryDefaultFunction},
	{ratioQoSCfgFactoryDefaultFunction},
	{pbnatCfgFactoryDefaultFunction}
}; /* array of callback function pointers, order must the same with TableID */

#else // Non CFG_TLV
static cfgmgr_factoryDefaultFunction_t factoryDefaultFunctionArray[CFGMGR_TABID_MAX] = 
{	
	{NULL},
	{vlanCfgFactoryDefaultFunction},
	{ifCfgFactoryDefaultFunction},
	{routeCfgFactoryDefaultFunction},
	{arpCfgFactoryDefaultFunction},
	{natCfgFactoryDefaultFunction},
	{mgmtCfgFactoryDefaultFunction},
	{pppoeCfgFactoryDefaultFunction},
	{dhcpsCfgFactoryDefaultFunction},
	{dhcpcCfgFactoryDefaultFunction},
	{dnsCfgFactoryDefaultFunction},
	{dmzCfgFactoryDefaultFunction},
	{logCfgFactoryDefaultFunction},
	{upnpCfgFactoryDefaultFunction},	
	{ddnsCfgFactoryDefaultFunction},	
	{aclCfgFactoryDefaultFunction},
	{aclGlobalCfgFactoryDefaultFunction},
	{serverpCfgFactoryDefaultFunction},
	{specialapCfgFactoryDefaultFunction},	
	{tbldrvCfgFactoryDefaultFunction},
	{urlfilterCfgFactoryDefaultFunction},
	{macfilterCfgFactoryDefaultFunction},	// add by Min Lee
	{tmssCfgFactoryDefaultFunction},	// add by Min Lee
	{tmssCInfoCfgFactoryDefaultFunction},	// Dino Chang 2004/12/30
	{dosCfgFactoryDefaultFunction},		
	{algCfgFactoryDefaultFunction},			
	{timeCfgFactoryDefaultFunction},
	{wlanCfgFactoryDefaultFunction},
	{udpblockCfgFactoryDefaultFunction},
	{routingCfgFactoryDefaultFunction},
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
	{pptpCfgFactoryDefaultFunction},
	{l2tpCfgFactoryDefaultFunction},	
#endif
	{qosCfgFactoryDefaultFunction},
	{pbnatCfgFactoryDefaultFunction},
	{rateLimitCfgFactoryDefaultFunction},
	{ratioQoSCfgFactoryDefaultFunction},
	{naptCfgFactoryDefaultFunction}
}; /* array of callback function pointers, order must the same with TableID */
#endif

static uint8 cfgmgr_opmode = CFGMGR_OPMODE_DISABLE;
static uint8 dirtyArray[CFGMGR_TABID_MAX];

/* LOCAL SUBPROGRAM SPECIFICATION */
//static uint32 cfgmgr_init(void);
static uint32 cfgmgr_integrityCheck(void);
void cfgmgr_task(void);
static uint16 cfgmgr_computeChecksum(uint16 * ptr, uint32 len);

#if defined(CONFIG_RTL865X_INIT_BUTTON)
static int re865xIoctl(char *name,uint32 arg0, uint32 arg1, uint32 arg2, uint32 arg3);
#endif /* CONFIG_RTL865X_INIT_BUTTON */

/* LOCAL FUNCTION IMPLEMENTATION */
/*
 * Routine Name: cfgmgr_computeChecksum
 * Description: calculate checksum from nvram (not from shadow ram!!)
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   uint16 checksum value
 * ThreadSafe: don't know yet
 * Assumption: 1. the checksum covered area's length should be even sized
 */
static uint16 cfgmgr_computeChecksum(uint16 * ptr, uint32 len)
{
	uint16 sum = 0;
	uint32 i;
	uint16 * ptr16 = (uint16*)ptr;
	
	/* check checksum */
	for (i=0; i<len; i+=2)
	{
		sum ^= *ptr16;
		ptr16 += 1;
	}
	return sum; /* integrity ok */
} /* end cfgmgr_computeChecksum */

/*
 * Routine Name: cfgmgr_preInit
 * Description: cfgmgr module pre-initialization 
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   OK      if okay
 *   NOT_OK  if not okay
 * ThreadSafe: don't know yet
 * Note: 1. should be called before init of those modules who uses cfgmgr.
 *       2. if memory are all cleared to zero during bsp, this preInit might be skipped!!
 */
static uint32 cfgmgr_integrityCheck(void)
{	
	uint16 * ptr;
	uint32 len;
	/* check signature */
	if (memcmp((void*)pNvramCtrl->signature,
	           (void*)CFGMGR_SIGNATURE,
	           strlen(CFGMGR_SIGNATURE)) != 0)
		goto ERR;
	/* check checksum */
	ptr = (uint16*)((uint32)CFGMGR_BASE_ADDR + (uint32)sizeof(cfgmgr_ctrl_t));
	len = CFGMGR_SIZE - sizeof(cfgmgr_ctrl_t);
	if (cfgmgr_computeChecksum(ptr,len) != pNvramCtrl->checksum)
		goto ERR;
	PRINT_MSG(("\ncfgmgr_integrityCheck: ok\n"));
	return OK; /* integrity ok */
ERR:
	PRINT_MSG(("\ncfgmgr_integrityCheck: fail\n"));
	return NOT_OK;
} /* end cfgmgr_integrityCheck */


/* GLOBAL FUNCTION IMPLEMENTATION */
/*
 * Routine Name: cfgmgr_preInit
 * Description: cfgmgr module pre-initialization 
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 * Note: 1. should be called before init of those modules who uses cfgmgr.
 *       2. if memory are all cleared to zero during bsp, this preInit might be skipped!!
 */
//void cfgmgr_preInit(void)
//{
//	memset((void*)factoryDefaultFunctionArray,0,sizeof(factoryDefaultFunctionArray));
//	cfgmgr_opmode = CFGMGR_OPMODE_PREINITED;
//	return;
//} /* end cfgmgr_preInit */

/*
 * Routine Name: cfgmgr_factoryDefault
 * Description: to perform an "active" factory default 
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   0           if okay
 *   1           if fail
 * ThreadSafe: don't know yet
 * Note:
 *   this function can be called by UI or triggered by corrupted nvram
 */
 
#ifdef CFG_TLV

//=== reset default value to pRomeCfgParam, temp buf use the shadow
#if defined(CONFIG_RTL865X_INIT_BUTTON)
uint32 cfgmgr_factoryDefault(uint32 calledByWho)
#else
uint32 cfgmgr_factoryDefault(void)
#endif /* CONFIG_RTL865X_INIT_BUTTON */
{
	uint32 tabSize=0;
	uint32 curOffset;
	uint32 i;
	unsigned long chksum=0;
	char *pch;
	void *pDst = (void*)CFGMGR_BASE_ADDR;

	/* set shadow to initial value: all 0 (for flash nvram, 0 as initial is better) */
	memset( (void*)pRomeCfgParam,0,sizeof(romeCfgParam_t) );
	memset( shadow,0,sizeof(shadow) );
		
	/* 1. call each module's factory default callback to obtain
	 *    factory default value
	 * 2. write contents in ram (the "pRomeCfgParam") 
	 */	
	
	#ifdef TLV_DEBUG
		printf("Entry cfgmgr_factoryDefault() \n");
	#endif
	
	curOffset = 0;
	for (i=1;i<CFGMGR_TABID_MAX;i++){
		
		if (factoryDefaultFunctionArray[i].pFactoryDefaultFunction)
	 		(*(factoryDefaultFunctionArray[i].pFactoryDefaultFunction))((void*)generalBuf,(uint32*)&tabSize);
		else {
			PRINT_MSG(("cfgmgr_factoryDefault: factoryDefault function of tabid[%u] is NULL\n",i));
			CFGMGR_HALT;
		}
		if (curOffset + tabSize > CFGMGR_SIZE) 	{
			PRINT_MSG(("cfgmgr_factoryDefault: \n\ttable total size[%u|0x%x] up to tabid[%u] exceeds max[%u|0x%x]!\n",curOffset+tabSize,curOffset+tabSize,i,CFGMGR_SIZE,CFGMGR_SIZE));
			CFGMGR_HALT;
		}
		
		#ifdef TLV_DEBUG
			printf("NICK HO-->In cfgmgr_factoryDefault Function[%u].Default,curOffset=%d \n",i, curOffset);
		#endif
		memcpy((void*)&(shadow[curOffset]),(void*)generalBuf,tabSize);
		curOffset += tabSize;
		#ifdef TLV_DEBUG
			printf("NICK HO-->In cfgmgr_factoryDefault Function[%u].Default,tabSize=%d \n",i, tabSize);		
		#endif
		
	} /* end for */	
	
	
	memcpy(pRomeCfgParam, shadow, sizeof(romeCfgParam_t) );
	
	#ifdef TLV_DEBUG
		printf("In cfgmgr_factoryDefault \n");
		show_cfg_value(pRomeCfgParam);	
	#endif
	
	memset(shadow, 0, sizeof(shadow) );		
	if ( (i=cfg2tlv( (char *)shadow, pRomeCfgParam))!=SUCCESS ) {	//After cfg2tlv, shadow contant the TLV strcture
		printf("In cfgmgr_factoryDefault() cfg2tlv fail ret=%d !!\n", i);
	}
	
	#ifdef TLV_DEBUG
		printf("In cfgmgr_factoryDefault() cfg2tlv Successful, ret=%d \n",i );
	#endif
	//=======  Calculate chksum Start   =======
	pch = (char *)shadow;
	for ( i=0; i<(sizeof(shadow)-sizeof(unsigned long)); i+=sizeof(unsigned long))
		chksum ^= *((unsigned long *)&pch[i]);

	chksum ^= CHKSUM_PATTERN;
	*((unsigned long *)&pch[i]) = chksum;
	//===============================================
	
		
	ENTER_CRITICAL(cfgmgrSemId,WAIT_FOREVER); /* enter critical section */
	if (flashdrv_updateImg((void *)shadow, /* srcAddr_P */
	        	    	           (void *)pDst, /* dstAddr_P */
	            	    	       (uint32)CFGMGR_SIZE)!=0)	{
				PRINT_MSG(("cfgmgr_task write nvram Fail !!!\n"));
				EXIT_CRITICAL(cfgmgrSemId); /* exit critical section */
				CFGMGR_HALT;
	}		
	EXIT_CRITICAL(cfgmgrSemId); /* exit critical section */
	
	#if defined(CONFIG_RTL865X_INIT_BUTTON)
	if (calledByWho==1)	{
		re865xIoctl("eth0",RTL8651_IOCTL_DIAG_LED,100,0,0);
	}
	#endif /* CONFIG_RTL865X_INIT_BUTTON */
	
	return 0;
} /* end cfgmgr_factoryDefault */


#else //==== Not the CFG_TLV
 
#if defined(CONFIG_RTL865X_INIT_BUTTON)
uint32 cfgmgr_factoryDefault(uint32 calledByWho)
#else
uint32 cfgmgr_factoryDefault(void)
#endif /* CONFIG_RTL865X_INIT_BUTTON */
{
	uint32 tabSize=0;
	uint32 curOffset;
	uint32 i;

	/* set shadow to initial value: all 0xff (for flash nvram, 0xff as initial is better) */
	memset((void*)shadow,0xff,sizeof(shadow));

	memcpy((void*)pCtrl->signature,CFGMGR_SIGNATURE,strlen(CFGMGR_SIGNATURE));

	pCtrl->version = CFGMGR_VERSION;
	
	/* 1. call each module's factory default callback to obtain
	 *    factory default value
	 * 2. write contents in ram (the "shadow") back to flash
	 */		
	curOffset=sizeof(cfgmgr_ctrl_t);
	for (i=1;i<CFGMGR_TABID_MAX;i++)
	{
		
		if (factoryDefaultFunctionArray[i].pFactoryDefaultFunction)
	 		(*(factoryDefaultFunctionArray[i].pFactoryDefaultFunction))((void*)generalBuf,(uint32*)&tabSize);
		else
	 	{
			PRINT_MSG(("cfgmgr_factoryDefault: factoryDefault function of tabid[%u] is NULL\n",i));
			CFGMGR_HALT;
		}
		if (curOffset + tabSize > CFGMGR_SIZE)
	 	{
			PRINT_MSG(("cfgmgr_factoryDefault: \n\ttable total size[%u|0x%x] up to tabid[%u] exceeds max[%u|0x%x]!\n",curOffset+tabSize,curOffset+tabSize,i,CFGMGR_SIZE,CFGMGR_SIZE));
			CFGMGR_HALT;
		}
		pCtrl->descriptor[i].offset = curOffset;
		pCtrl->descriptor[i].tabSize = tabSize;
		memcpy((void*)&(shadow[curOffset]),(void*)generalBuf,tabSize);
		curOffset += tabSize;
	} /* end for */

	{
		uint32 len = CFGMGR_SIZE - sizeof(cfgmgr_ctrl_t);
		uint16 * ptr = (uint16*)((uint32)pCtrl + (uint32)sizeof(cfgmgr_ctrl_t));
		pCtrl->checksum = cfgmgr_computeChecksum(ptr,len);
	}
	
	ENTER_CRITICAL(cfgmgrSemId,WAIT_FOREVER); /* enter critical section */	

#ifdef _SUPPORT_LARGE_FLASH_
	/* Load bdinfo first */
	flashdrv_read((void*)&bdinfo,(void*)FLASH_MAP_BOARD_INFO_ADDR,sizeof(bdinfo));
#endif/*_SUPPORT_LARGE_FLASH_*/
	
	if (flashdrv_updateImg((void *)shadow,           /* srcAddr_P */
		    	           (void *)CFGMGR_BASE_ADDR, /* dstAddr_P */
	    	    	       (uint32)CFGMGR_SIZE)!=0)
	{
		PRINT_MSG(("cfgmgr_factoryDefault: read nvram fail, CFGMGR_BASE_ADDR=%x CFGMGR_SIZE=%x\n",CFGMGR_BASE_ADDR,CFGMGR_SIZE));
		CFGMGR_HALT;
	}
	EXIT_CRITICAL(cfgmgrSemId); /* exit critical section */
	//PRINT_MSG(("cfgmgr_factoryDefault: curOffset[0x%x] ok\n",curOffset));
	#if defined(CONFIG_RTL865X_INIT_BUTTON)
	if (calledByWho==1)	{
		re865xIoctl("eth0",RTL8651_IOCTL_DIAG_LED,100,0,0);
	}
	#endif /* CONFIG_RTL865X_INIT_BUTTON */
	return 0;
} /* end cfgmgr_factoryDefault */
#endif

/*
 * Routine Name: cfgmgr_init
 * Description: cfgmgr module initialization 
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   OK          if success
 *   NOT_OK      if fail
 * ThreadSafe: don't know yet
 * Note: should be called by userAppInit()
 */
 
#ifdef CFG_TLV
 
uint32 cfgmgr_init(void)
{
	unsigned long chksum=0;
	unsigned long *p;
	int i, size;

	/* init flashdrv */
	flashdrv_init();
	/* verify flash blocks */
	
	#ifdef TLV_DEBUG
		printf("NICK HO--> Entry cfgmgr_init() \n");
	#endif
	
	set_allitems();
	
	
	if (flashdrv_read((void*)shadow,           /* dstAddr_P */
		                  (void*)CFGMGR_BASE_ADDR, /* srcAddr_P */
		                  (uint32)CFGMGR_SIZE) != 0) {
	 		PRINT_MSG(("cfgmgr_init: read nvram Fail !!!\n"));
	 		goto INIT_ERR;
	}
	PRINT_MSG(("cfgmgr_init: read nvram Successful \n"));
	p = (unsigned long *)shadow;
	size = sizeof(shadow);
	
	for ( i=0; i<(size/sizeof(unsigned long)); i++)  {
        chksum ^= *p;
        p ++;
    }

    if ( chksum != CHKSUM_PATTERN) {
		
    	PRINT_MSG(("cfgmgr_init: checksum Error !!!\n"));
    	
    	printf("NICKHO--> In cfgmgr_init(), chksun Error !!!\n");
    	
    	//=======   Checksum Error restore_default()   ========
    	//cfgmgr_factoryDefault filled the value to pRomeCfgParam (bye by bye)
    	//tlv2cfg(): pRomeCfgParam --> shadow
    	#if defined(CONFIG_RTL865X_INIT_BUTTON)
			cfgmgr_factoryDefault(0);			
		#else
			cfgmgr_factoryDefault();	//now pRomeCfgParam has the default setting, it will save the default to Flash
		#endif
		
		#ifdef TLV_DEBUG
			printf("In cfgmgr_init(), After cfg_fyDefault() \n");
			printf("A, vlanCfgParam[0].vid=%d \n",  pRomeCfgParam->vlanCfgParam[0].vid);
		#endif        
     }
	 //======  Transfer TLV to pRomeCfgParam  =========
	 tlv2cfg(shadow, sizeof(shadow), pRomeCfgParam);	//Transfer the shadow to pRomeCfgParam, Nick HO
	 	
	cfgmgr_opmode = CFGMGR_OPMODE_ENABLE;

	return OK;
INIT_ERR:
	cfgmgr_opmode = CFGMGR_OPMODE_DISABLE;
	return NOT_OK;
} /* end cfgmgr_init */
#else	//Non CFG_TLV
 
uint32 cfgmgr_init(void)
{
	#if 0
	if (cfgmgr_opmode != CFGMGR_OPMODE_DISABLE)
	{
		PRINT_MSG(("cfgmgr_init: cfgmgr already inited.\n"));
		goto ERR;
	}
	#endif

	/* init flashdrv */
	flashdrv_init();
	/* verify flash blocks */

	if (cfgmgr_integrityCheck()==NOT_OK)
	{
		/* passive factory default triggered by corrupted nvram */
		#if defined(CONFIG_RTL865X_INIT_BUTTON)
		cfgmgr_factoryDefault(0);
		#else
		cfgmgr_factoryDefault();
		#endif /* CONFIG_RTL865X_INIT_BUTTON */
		{
			DEBUG_P("\nSystem restarting...\n");
			sleep(2); re865xIoctl("eth0",RTL8651_IOCTL_REBOOT,0,0,0); // MMU user-mode program cannot jump to FLASH_BASE.
		}
	}
	else
	{
		if (flashdrv_read((void*)shadow,           /* dstAddr_P */
		                  (void*)CFGMGR_BASE_ADDR, /* srcAddr_P */
		                  (uint32)CFGMGR_SIZE) != 0)
		{
	 		PRINT_MSG(("cfgmgr_init: read nvram fail!\n"));
	 		goto ERR;
		}
	} /* end if cfgmgr_integrityCheck() */

	memset((void*)dirtyArray,CFGMGR_NOT_DIRTY,sizeof(dirtyArray));
	
	cfgmgr_opmode = CFGMGR_OPMODE_ENABLE;


#if 0
	printf("cfgmgr_init: romeCfgParam size: %d(0x%x)\n", sizeof(romeCfgParam_t),sizeof(romeCfgParam_t));
	printf("cfgmgr_init: pRomeCfgParam addr: %u(0x%x)\n",(uint32)pRomeCfgParam,(uint32)pRomeCfgParam);
	
	{		
		FILE * fp;
		fp = fopen(ROMECFGFILE,"w+");
		if (fp)
		{
			fprintf(fp,"%x\n",(uint32)pRomeCfgParam);
		}
		fclose(fp);	
	}
#endif	



	
	/* create nvram updating task
	 */
#if 0
	taskSpawn("tCfgMgr",	/* task name */
			  50,			/* priority */
			  0,			/* option word */
			  2048,		    /* stack size 2K */
			  cfgmgr_task   /* entry point */

		 );
#endif

	return OK;
ERR:
	cfgmgr_opmode = CFGMGR_OPMODE_DISABLE;
	return NOT_OK;
} /* end cfgmgr_init */
#endif


/*
 * Routine Name: cfgmgr_read
 * Description: read module config table 
 * Input:  
 *   tabId     - defined in cfgmgr_tabId_e
 *   pBuf      - buffer pointer (buffer prepared by caller)
 *   bufSize   - size to be read
 * Output:
 *   none
 * Return:
 *   OK         if okay
 *   NOT_OK     if not okay
 * ThreadSafe: don't know yet
 * Note:
 *   none
 */
 
#ifdef CFG_TLV
uint32 cfgmgr_read(uint32 tabId, void * pBuf, uint32 bufSize)
{
	uint32 offset;
	if (cfgmgr_opmode != CFGMGR_OPMODE_ENABLE)
	{
		if (cfgmgr_init()!=OK)
		goto ERR;
	}
	if (tabId >= CFGMGR_TABID_MAX)
	{
		PRINT_MSG(("cfgmgr_read: invalid tabid %u\n",tabId));
		goto ERR;
	}
	if (pBuf == NULL)
	{
		PRINT_MSG(("cfgmgr_read: argument pBuf is NULL\n"));
		goto ERR;
	}
	return OK;
ERR:
	return NOT_OK;
} /* end cfgmgr_read */

#else	//Non CFG_TLV
 
uint32 cfgmgr_read(uint32 tabId, void * pBuf, uint32 bufSize)
{
	uint32 offset;
	if (cfgmgr_opmode != CFGMGR_OPMODE_ENABLE)
	{
		if (cfgmgr_init()!=OK)
		goto ERR;
	}
	if (tabId >= CFGMGR_TABID_MAX)
	{
		PRINT_MSG(("cfgmgr_read: invalid tabid %u\n",tabId));
		goto ERR;
	}
	if (pBuf == NULL)
	{
		PRINT_MSG(("cfgmgr_read: argument pBuf is NULL\n"));
		goto ERR;
	}

	if (bufSize != pCtrl->descriptor[tabId].tabSize)
	{
		PRINT_MSG(("cfgmgr_read: arg bufSize[0x%08x] != size[0x%08x] id=%d\n",bufSize,pCtrl->descriptor[tabId].tabSize,tabId));
		/* to cope with different table size during development
		 * -> factory default
		 * -> read again!
		 */
		//goto ERR;
		#if defined(CONFIG_RTL865X_INIT_BUTTON)
		cfgmgr_factoryDefault(0);
		#else
		cfgmgr_factoryDefault();
		#endif /* CONFIG_RTL865X_INIT_BUTTON */
     	// call restart procedure here..
		{
			void  (*rtl_app_start)(void);
			DEBUG_P("\nSystem restarting...\n");
			sleep(2); re865xIoctl("eth0",RTL8651_IOCTL_REBOOT,0,0,0); // MMU user-mode program cannot jump to FLASH_BASE.
		}		
	}
	/* patch: to avoid compiler generate word-aligned load/store instructions */
	/*offset = pCtrl->descriptor[tabId].offset; */
	memcpy((void*)&offset,(void*)&(pCtrl->descriptor[tabId].offset),sizeof(uint32));
	
	memcpy((void*)pBuf,(void*)(&shadow[offset]),bufSize);

	return OK;
ERR:
	return NOT_OK;
} /* end cfgmgr_read */

#endif
/*
 * Routine Name: cfgmgr_write
 * Description: save module config table 
 * Input:  
 *   tabId     - defined in cfgmgr_tabId_e
 *   pBuf      - buffer pointer (buffer prepared by caller)
 *   bufSize   - size to be saved
 * Output:
 *   none
 * Return:
 *   OK         if okay
 *   NOT_OK     if not okay
 * ThreadSafe: don't know yet
 * Note:
 *   none
 */
#ifdef CFG_TLV
uint32 cfgmgr_write(uint32 tabId, void * pBuf, uint32 bufSize)
{
	uint32 offset;
	if (cfgmgr_opmode != CFGMGR_OPMODE_ENABLE)
	{
		if (cfgmgr_init()!=OK)	
		goto W_ERR;
	}
	if (tabId >= CFGMGR_TABID_MAX)
		goto W_ERR;
	if (pBuf == NULL)
		goto W_ERR;

	return OK;	
W_ERR:
	return NOT_OK;
} /* end cfgmgr_write */
 
#else //Non CFG_TLV
uint32 cfgmgr_write(uint32 tabId, void * pBuf, uint32 bufSize)
{
	uint32 offset;
	if (cfgmgr_opmode != CFGMGR_OPMODE_ENABLE)
	{
		if (cfgmgr_init()!=OK)	
		goto W_ERR;
	}
	if (tabId >= CFGMGR_TABID_MAX)
		goto W_ERR;
	if (pBuf == NULL)
		goto W_ERR;
	

//#if 0
	if (bufSize != pCtrl->descriptor[tabId].tabSize)
		goto W_ERR;
	
	/* patch: to avoid compiler generate word-aligned load/store instructions */
	/* offset = pCtrl->descriptor[tabId].offset; */
	memcpy((void*)&offset,(void*)&(pCtrl->descriptor[tabId].offset),sizeof(uint32));
	
	/* update shadow and dirtyArray only if shadow != caller's buffer */
	if (memcmp((void*)(&shadow[offset]),(void*)pBuf,bufSize) != 0)
	{
		memcpy((void*)(&shadow[offset]),(void*)pBuf,bufSize);
		dirtyArray[tabId] = CFGMGR_DIRTY;
	}
//#endif
	return OK;
	
W_ERR:
	return NOT_OK;
} /* end cfgmgr_write */
#endif

/*
 * Routine Name: cfgmgr_task
 * Description: cfgmgr module task to flush shadow back nvram if
 *              it is dirty (i.e., cfgmgr_write() has been called
 *              and a dirty flag has ever been set to 1)
 *              
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 * Note:
 *   current implementation only assumes exactly one flash block used
 *   as the cfgmgr area, therefore if any module's dirty flag is set,
 *   write the whole shadow ram back to flash memory block, and that's
 *   all!! if the cfgmgr area covers multiple flash memory blocks, the
 *   implementation needs to be enhanced. 2003/4/28
 */
 
#ifdef CFG_TLV

void cfgmgr_task(void)
{
	void *pDst = (void*)CFGMGR_BASE_ADDR;
	uint32 idx;
	unsigned long chksum=0;
	int	i;	
	char *pch;
	
	
	if (cfgmgr_opmode != CFGMGR_OPMODE_ENABLE){
		PRINT_MSG(("cfgmgr_task: cfgmgr module not inited yet. quit the task!\n"));
		return;
	}
	//uint16 * ptr = (uint16*)((uint32)pCtrl + (uint32)sizeof(cfgmgr_ctrl_t));
		
	
		
	ENTER_CRITICAL(cfgmgrSemId,WAIT_FOREVER); /* enter critical section */
	//pCtrl->checksum = cfgmgr_computeChecksum(ptr,len);
		
	#ifdef TLV_DEBUG
	 /****
		printf("======================= In cfgmgr_task Start ================================\n");
		printf("NICKHO--> Entry cfgmgr_task() \n");
		show_cfg_value(pRomeCfgParam);
		printf("======================= In cfgmgr_task End  ================================\n");
		*****/
	#endif	
	memset(shadow, 0, sizeof(shadow) );		
	if (cfg2tlv( (char *)shadow, pRomeCfgParam)!=SUCCESS ) {
		PRINT_MSG(("cfgmgr_task cfg2tlv fail !!\n"));
	}
	//PRINT_MSG(("cfgmgr_task cfg2tlv Successful\n"));
		
	#ifdef TLV_DEBUG	
		printf("NICKHO--> In cfgmgr_task() \n");
		tlv2cfg(shadow, sizeof(shadow), pRomeCfgParam);
		printf(" ===================    After Call tlv2cfg()  ===========\n");
		show_cfg_value(pRomeCfgParam);
	#endif	
		
	//=======  Calculate chksum Start   =======
	pch = (char *)shadow;
	for ( i=0; i<(sizeof(shadow)-sizeof(unsigned long)); i+=sizeof(unsigned long))
		chksum ^= *((unsigned long *)&pch[i]);

	chksum ^= CHKSUM_PATTERN;
	*((unsigned long *)&pch[i]) = chksum;
	//===============================================
		
	if (flashdrv_updateImg((void *)shadow, /* srcAddr_P */
	       	    	           (void *)pDst, /* dstAddr_P */
	           	    	       (uint32)CFGMGR_SIZE)!=0)	{
			PRINT_MSG(("cfgmgr_task write nvram Fail !!!\n"));
			EXIT_CRITICAL(cfgmgrSemId); /* exit critical section */
			CFGMGR_HALT;
	}		
	EXIT_CRITICAL(cfgmgrSemId); /* exit critical section */

	PRINT_MSG(("cfgmgr_task write nvram successful\n"));
} /* end cfgmgr_task */

#else //Non CFG_TLV 
void cfgmgr_task(void)
{
	void *pSrc = (void*)shadow;
	void *pDst = (void*)CFGMGR_BASE_ADDR;
	uint32 idx;
	
	//while (1)
	{
		if (cfgmgr_opmode != CFGMGR_OPMODE_ENABLE)
		{
			PRINT_MSG(("cfgmgr_task: cfgmgr module not inited yet. quit the task!\n"));
			return;
		}
		
	
		for (idx=1; idx<CFGMGR_TABID_MAX; idx++)
		{
			if (dirtyArray[idx] == CFGMGR_DIRTY)
			{
				uint32 len = CFGMGR_SIZE - sizeof(cfgmgr_ctrl_t);
				uint16 * ptr = (uint16*)((uint32)pCtrl + (uint32)sizeof(cfgmgr_ctrl_t));
				ENTER_CRITICAL(cfgmgrSemId,WAIT_FOREVER); /* enter critical section */
				pCtrl->checksum = cfgmgr_computeChecksum(ptr,len);

#ifdef _SUPPORT_LARGE_FLASH_
				/* Load bdinfo first */
				flashdrv_read((void*)&bdinfo,(void*)FLASH_MAP_BOARD_INFO_ADDR,sizeof(bdinfo));
#endif/*_SUPPORT_LARGE_FLASH_*/
	
				if (flashdrv_updateImg((void *)pSrc, /* srcAddr_P */
		        	    	           (void *)pDst, /* dstAddr_P */
		            	    	       (uint32)CFGMGR_SIZE)!=0)
				{
					PRINT_MSG(("cfgmgr_task write nvram fail\n"));
					EXIT_CRITICAL(cfgmgrSemId); /* exit critical section */
					CFGMGR_HALT;
				}
				/* clear all the dirty flags in dirtyArray after a successful flush-back */
				memset((void*)dirtyArray,CFGMGR_NOT_DIRTY,sizeof(dirtyArray));
				EXIT_CRITICAL(cfgmgrSemId); /* exit critical section */
				break; /* exit the for loop */
			} /* end if dirtyArray */
		} /* end for */
				
		
	} /* end while */

	PRINT_MSG(("cfgmgr_task write nvram successful\n"));
} /* end cfgmgr_task */

#endif

/*
 * re865x Ioctl (to call kernel functions including rtl8651 table driver APIs
 */
#if defined(CONFIG_RTL865X_INIT_BUTTON)
int re865xIoctl(char *name,uint32 arg0, uint32 arg1, uint32 arg2, uint32 arg3)
{
	uint32 args[4];
	struct ifreq ifr;
	int sockfd;

	args[0] = arg0;
	args[1] = arg1;
	args[2] = arg2;
	args[3] = arg3;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		DEBUG_P("fatal error socket\n");
		return -3;
	}
	strcpy((char*)&ifr.ifr_name, name);
	((uint32 *)(&ifr.ifr_data))[0] = (uint32)args;
	if (ioctl(sockfd, SIOCDEVPRIVATE, &ifr)<0)
	{
		perror("device ioctl:");
		DEBUG_P("\nerror %s line %d\n",__FUNCTION__,__LINE__);
		close(sockfd);
		return -1;
	}
	close(sockfd);
	return 0;
} /* end re865xIoctl */
#endif /* CONFIG_RTL865X_INIT_BUTTON */

/* end of cfgmgr.c */
