/*
 * Copyright c                Realtek Semiconductor Corporation, 2003
 * All rights reserved.                                                    
 * 
 * $Header: /home/cvsroot/uClinux-dist/user/goahead-2.1.4/LINUX/main.c,v 1.62 2004/08/11 07:28:22 tony Exp $
 *
 * $Author: tony $
 *
 * Abstract:
 *
 *   main.c : main.c
 *
 * $Log: main.c,v $
 * Revision 1.62  2004/08/11 07:28:22  tony
 * +: new features: support quickcam web camera.
 *
 * Revision 1.61  2004/08/11 04:00:17  yjlou
 * *: _flash_init() is renamed to flashdrv_init()
 *
 * Revision 1.60  2004/08/04 14:50:18  yjlou
 * *: change type of __flash_base from 'int*' to 'uint32'
 *
 * Revision 1.59  2004/07/16 11:47:20  tony
 * *: make PPTP/L2TP redial usable.
 *
 * Revision 1.58  2004/07/14 13:55:59  chenyl
 * +: web page for MN queue
 *
 * Revision 1.57  2004/07/13 07:13:28  cfliu
 * no message
 *
 * Revision 1.56  2004/07/07 05:12:36  chhuang
 * +: add a new WAN type (DHCP+L2TP). But not complete yet!!
 *
 * Revision 1.55  2004/07/06 06:19:55  chhuang
 * +: add rate limit
 *
 * Revision 1.54  2004/07/01 09:37:12  yjlou
 * *: fixed __flash_base to 0xbe000000 for 865xB.
 *
 * Revision 1.53  2004/06/29 09:41:47  cfliu
 * *: Flash mmap should be 0x100000
 *
 * Revision 1.52  2004/06/28 13:57:34  yjlou
 * +: support Free-style flash editor
 *
 * Revision 1.51  2004/06/14 13:48:17  rupert
 * +: Add work properly with MMU kernel
 *
 * Revision 1.50  2004/06/11 10:55:55  cfliu
 * +: Add Cable Meter and Interface counter webpage
 *
 * Revision 1.49  2004/06/11 09:20:18  tony
 * +: using share memory instead of direct access for pRomeCfgParam.
 *
 * Revision 1.48  2004/06/10 14:35:42  cfliu
 * +: Add port config webpage
 *
 * Revision 1.47  2004/06/07 03:12:20  yjlou
 * *: fixed the bug of '/bin/webs default': call _flash_init() before flashdrv_updateImg()
 *
 * Revision 1.46  2004/05/28 09:49:16  yjlou
 * +: support Protocol-Based NAT
 *
 *
 * Revision 1.44  2004/05/20 08:43:55  chhuang
 * add Web Page for QoS
 *
 * Revision 1.43  2004/05/12 07:20:15  chenyl
 * +: source ip blocking
 * *: modify dos mechanism
 *
 * Revision 1.42  2004/05/12 05:15:05  tony
 * support PPTP/L2TP in RTL865XB.
 *
 * Revision 1.41  2004/05/05 08:27:07  tony
 * new features: add remote management spec
 *
 * Revision 1.40  2004/04/08 13:18:12  tony
 * add PPTP/L2TP routine for MII lookback port.
 *
 * Revision 1.39  2004/03/31 01:59:36  tony
 * add L2TP wan type UI pages.
 *
 * Revision 1.38  2004/03/03 03:43:32  tony
 * add static routing table in turnkey.
 *
 * Revision 1.37  2004/02/03 08:14:34  tony
 * add UDP Blocking web UI configuration.
 *
 * Revision 1.36  2004/01/07 09:10:04  tony
 * add PPTP Client UI in Config Wizard.
 *
 * Revision 1.35  2003/12/19 04:33:01  tony
 * add Wireless Lan config pages: Basic Setting, Advance Setting, Security, Access Control, WDS
 *
 * Revision 1.33  2003/12/01 12:35:52  tony
 * make ALG is able to be configured by users(Web GUI).
 *
 * Revision 1.32  2003/10/24 10:25:58  tony
 * add DoS attack interactive webpage,
 * FwdEngine is able to get WAN status by rtl8651_wanStatus(0:disconnect,1:connect)
 *
 * Revision 1.31  2003/10/14 08:15:01  tony
 * add user account management routine
 *
 * Revision 1.30  2003/10/03 12:27:35  tony
 * add NTP time sync routine in management web page.
 *
 * Revision 1.29  2003/10/02 10:50:03  tony
 * add ping routine in webpage.
 *
 * Revision 1.28  2003/10/02 01:26:11  tony
 * solve all compiler warnning messages in main.c
 *
 * Revision 1.27  2003/10/01 05:57:31  tony
 * add URL Filter routine
 *
 * Revision 1.26  2003/09/29 08:57:32  tony
 * add routine: when pppoe config changed, the dial function will be disabled.
 * add routine: dhcps is able to start/stop server on runtime.
 *
 * Revision 1.25  2003/09/26 12:06:15  tony
 * modify pppoe Dial/Hang_Up routine
 *
 * Revision 1.24  2003/09/26 10:23:52  tony
 * add PPPoE dial,hang-up routine
 *
 * Revision 1.23  2003/09/26 08:43:17  tony
 * update Status Web Page
 *
 * Revision 1.22  2003/09/26 04:41:25  tony
 * modify Wizard page, support pppoe, unnumbered pppoe
 *
 * Revision 1.21  2003/09/25 10:09:44  orlando
 * add "webs save" command to write current shadow back to flash
 *
 * Revision 1.20  2003/09/25 06:06:45  orlando
 * adjust inclue files
 *
 * Revision 1.19  2003/09/25 05:51:47  orlando
 * remove "global.h"
 *
 * Revision 1.18  2003/09/25 02:15:32  orlando
 * Big Change
 *
 * Revision 1.17  2003/09/24 07:10:30  rupert
 * rearrange init sequence
 *
 * Revision 1.16  2003/09/24 05:13:43  rupert
 * add dhcp client deconfig
 *
 * Revision 1.15  2003/09/23 07:13:18  tony
 * rename all webpage asp function
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
 * Revision 1.9  2003/09/18 05:09:28  hiwu
 * change fork() to vfork()
 *
 * Revision 1.8  2003/09/18 05:07:40  tony
 * add event log webpage
 *
 * Revision 1.7  2003/09/18 02:35:31  tony
 * add cvs header
 *
 */
 
/******************************** Description *********************************/

/*
 *	Main program for for the GoAhead WebServer. This is a demonstration
 *	main program to initialize and configure the web server.
 */

/********************************* Includes ***********************************/

#include	"../uemf.h"
#include	"../wsIntrn.h"
#include	<signal.h>
#include	<unistd.h> 
#include	<sys/types.h>
#include	<sys/wait.h>
#include <sys/shm.h>
#include	<string.h>
#include    	<board.h>
#include <version.h>
#ifdef WEBS_SSL_SUPPORT
#include	"../websSSL.h"
#include <linux/config.h>
#endif

#include "cfg.h"	//Added by Nick HO, 01/26/2005

#ifdef USER_MANAGEMENT_SUPPORT
#include	"../um.h"
void	formDefineUserMgmt(void);
#endif

/* Add by Dick Tam */

/* Add by Dick Tam end */

/*********************************** Locals ***********************************/
/*
 *	Change configuration here
 */

static char_t		*rootWeb = T("/www");			/* Root web directory */
static char_t		*password = T("");				/* Security password */
static int			port = 80;						/* Server port */
static int			retries = 5;					/* Server port retries */
static int			finished;						/* Finished flag */

/****************************** Forward Declarations **************************/

static int 	initWebs();
/* Mark by Dick Tam */
/*
static int	aspTest(int eid, webs_t wp, int argc, char_t **argv);
static void formTest(webs_t wp, char_t *path, char_t *query);
*/
/* Mark by Dick Tam end */
static int  websHomePageHandler(webs_t wp, char_t *urlPrefix, char_t *webDir,
				int arg, char_t *url, char_t *path, char_t *query);


extern void cfgmgr_main(void);
extern void dhcpcIPBound();
extern void dhcpcDeconfig();



extern int asp_flashGetCfgParam(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_flashGetCloneMac(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_flashGetString(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_flashGetIpElement(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_flashGetPppoeParam(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_getWanAddress(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_dhcpServerLeaseList(int ejid, webs_t wp, int argc, char_t **argv);
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
extern int asp_pptpWizard(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_l2tpWizard(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_dhcpL2tpWizard(int ejid, webs_t wp, int argc, char_t **argv);
#endif
extern int asp_acl(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_dmz(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_naptOpt(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_dos(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_dosProc(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_upnp(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_user(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_remoteMgmt(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_udpBlocking(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_urlFilter(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_ddns(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_eventLog(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_pppoe(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_ping(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_routing(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_serverPort(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_specialApplication(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_statusExtended(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_TZ(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_TZ2(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_alg(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_wlanBasic(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_wlanAdvance(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_wlanSecurity(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_wlanAccessCtrl(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_wlanWds(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_configChanged(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_qos(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_flash(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_ratio_qos(int ejid, webs_t wp, int argc, char_t **argv);
extern int asp_webcam(int ejid, webs_t wp, int argc, char_t **argv);


extern void asp_dhcpServerAdvance(webs_t wp, char_t *path, char_t *query);
//extern void asp_hangUp(webs_t wp, char_t *path, char_t *query);
extern void asp_pppoeAdvance(webs_t wp, char_t *path, char_t *query);
extern void asp_setStaticWizard(webs_t wp, char_t *path, char_t *query);
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
extern void asp_setPptpWizard(webs_t wp, char_t *path, char_t *query);
extern void asp_setL2tpWizard(webs_t wp, char_t *path, char_t *query);
extern void asp_setDhcpL2tpWizard(webs_t wp, char_t *path, char_t *query);
#endif
extern void asp_wanIp(webs_t wp, char_t *path, char_t *query);
extern void asp_upload(webs_t wp, char_t *path, char_t *query);
extern void asp_restart(webs_t wp, char_t *path, char_t *query);
extern void asp_port(webs_t wp, char_t *path, char_t *query);
extern int asp_getLanPortStatus(webs_t wp, char_t *path, char_t *query);
extern int asp_getWanPortStatus(webs_t wp, char_t *path, char_t *query);
extern void asp_systemDefault(webs_t wp, char_t *path, char_t *query);
extern void asp_setAcl(webs_t wp, char_t *path, char_t *query);
extern void asp_setDdns(webs_t wp, char_t *path, char_t *query);
extern void asp_setDhcpClient(webs_t wp, char_t *path, char_t *query);
extern void asp_setDmz(webs_t wp, char_t *path, char_t *query);
extern void asp_setEventLog(webs_t wp, char_t *path, char_t *query);
extern void asp_setLan(webs_t wp, char_t *path, char_t *query);
extern void asp_setNat(webs_t wp, char_t *path, char_t *query);
extern void asp_setPing(webs_t wp, char_t *path, char_t *query);
extern void asp_setPppoe(webs_t wp, char_t *path, char_t *query);
extern void asp_setPppoeWizard(webs_t wp, char_t *path, char_t *query);
extern void asp_setRouting(webs_t wp, char_t *path, char_t *query);
extern void asp_setServerPort(webs_t wp, char_t *path, char_t *query);
extern void asp_setSpecialApplication(webs_t wp, char_t *path, char_t *query);
extern void asp_setTZ(webs_t wp, char_t *path, char_t *query);
extern void asp_setAlg(webs_t wp, char_t *path, char_t *query);
extern void asp_setPort(webs_t wp, char_t *path, char_t *query);
extern void asp_setUnnumberedWizard(webs_t wp, char_t *path, char_t *query);
extern void asp_setUpnp(webs_t wp, char_t *path, char_t *query);
extern void asp_setUdpBlocking(webs_t wp, char_t *path, char_t *query);
extern void asp_setNaptOpt(webs_t wp, char_t *path, char_t *query);
extern void asp_setDos(webs_t wp, char_t *path, char_t *query);
extern void asp_setDosProc(webs_t wp, char_t *path, char_t *query);
extern void asp_setUser(webs_t wp, char_t *path, char_t *query);
extern void asp_setRemoteMgmt(webs_t wp, char_t *path, char_t *query);
extern void asp_setUrlFilter(webs_t wp, char_t *path, char_t *query);
extern void asp_setWlanBasic(webs_t wp, char_t *path, char_t *query);
extern void asp_setWlanAdvance(webs_t wp, char_t *path, char_t *query);
extern void asp_setWlanSecurity(webs_t wp, char_t *path, char_t *query);
extern void asp_setWlanAccessCtrl(webs_t wp, char_t *path, char_t *query);
extern void asp_setWlanWds(webs_t wp, char_t *path, char_t *query);
extern void asp_setQos(webs_t wp, char_t *path, char_t *query);
extern void asp_setQos1(webs_t wp, char_t *path, char_t *query);
extern void asp_pbnat(webs_t wp, char_t *path, char_t *query);
extern void asp_setPbnat(webs_t wp, char_t *path, char_t *query);
extern void asp_setFlash(webs_t wp, char_t *path, char_t *query);
extern void asp_rateLimit(int ejid, webs_t wp, int argc, char_t **argv);
extern void asp_setRateLimit(webs_t wp, char_t *path, char_t *query);
extern void asp_setRatio_qos(webs_t wp, char_t*path, char_t *query);


extern void defaultErrorHandler(int etype, char_t *msg);
extern void defaultTraceHandler(int level, char_t *buf);
#ifdef B_STATS
static void printMemStats(int handle, char_t *fmt, ...);
static void memLeaks();
#endif

extern int ipupdown(int argc, char*argv[]);
uint32 cfgmgr_factoryDefault(void);
void cfgmgr_task(void);
uint32 sysInit(void);

// dino 2004/10/29
// GST web server main entry
extern int web_server_main(void);
// dino
/*********************************** Code *************************************/
/*
 *	Main -- entry point from LINUX
 */

romeCfgParam_t *pRomeCfgParam;

#ifndef __uClinux
uint32 __flash_base;
//uint32 __crmr_addr;
#endif
int main(int argc, char** argv)
{

	// add share memory for pRomeCfgParam
	int shmid=0;
#ifndef __uClinux__
	__flash_base=rtl865x_mmap(0x1e000000,0x1000000); /* 0xbe000000~0xbeffffff */
	//__crmr_addr=rtl865x_mmap(0x1c805000,0x1000)+0x0104; /* 0xbc805104 */
	//printf("__flash_base=0x%08x\n",__flash_base);
	//printf("__crmr_addr=0x%08x\n",__crmr_addr);
#endif
	shmid=shmget(SHM_PROMECFGPARAM,sizeof(romeCfgParam_t),0666|IPC_CREAT);
	pRomeCfgParam=shmat(shmid,(void*)0,0);

/*
 *	Initialize the memory allocator. Allow use of malloc and start 
 *	with a 60K heap.  For each page request approx 8KB is allocated.
 *	60KB allows for several concurrent page requests.  If more space
 *	is required, malloc will be used for the overflow.
 */
#ifdef __mips__	
	if (strstr(argv[0],"ip-up") || strstr(argv[0],"ip-down"))
	{
		ipupdown(argc,argv);
		goto main_end;
	}
	if (argc>1)
	{
		if (!strcmp(argv[1],"default"))
		{
			#ifndef CFG_TLV
				flashdrv_init();
			#endif
			cfgmgr_factoryDefault();
			printf("cfgmgr_factoryDefault() done!\n");
		}

		if (!strcmp(argv[1],"save"))
		{
			#ifndef CFG_TLV
				flashdrv_init();
			#endif
			cfgmgr_task();
			printf("call cfgmgr_task() to save romeCfgParam to flash done!\n");
		}

		else if (!strcmp(argv[1],"start"))
		{
			/* demo system configuration inialization entry point
			 * defined in board.c
			 */
			sysInit();
			goto conti;
		}
		
		else if (!strcmp(argv[1],"bound"))
		{
			dhcpcIPBound();
		}
#if 0
		else if (!strcmp(argv[1], "renew"))
		{
			dhcpcIPRenew();
		}
#endif
		else if (!strcmp(argv[1],"deconfig"))
		{
			dhcpcDeconfig();
		}
		goto main_end;
		
	}
#endif
	
conti:
	//bopen(NULL,60*1024, B_USE_MALLOC);// Goahead block module routine
	
	signal(SIGPIPE, SIG_IGN);

/*
 *	Initialize the web server
 */

	//if (initWebs() < 0) // origional web server routine
	if(web_server_main() < 0)
	{
		return -1;//0
	}
	
#ifdef WEBS_SSL_SUPPORT
	websSSLOpen();
#endif

/*
 *	Basic event loop. SocketReady returns true when a socket is ready for
 *	service. SocketSelect will block until an event occurs. SocketProcess
 *	will actually do the servicing.
 */
	while (!finished) 
	{
		if (socketReady(-1) || socketSelect(-1, 1000)) 
		{
			socketProcess(-1);
		}
		//websCgiCleanup();
		emfSchedProcess();
	}
	
#ifdef WEBS_SSL_SUPPORT
	websSSLClose();
#endif

#ifdef USER_MANAGEMENT_SUPPORT
	umClose();
#endif

/*
 *	Close the socket module, report memory leaks and close the memory allocator
 */
	websCloseServer();
	socketClose();
#ifdef B_STATS
	memLeaks();
#endif
	//bclose(); // Goahead block module routine


main_end:
	shmdt(pRomeCfgParam);

	return 0;
}

/******************************************************************************/
/*
 *	Initialize the web server.
 */

static int initWebs()
{
	struct hostent	*hp;
	struct in_addr	intaddr;
	char			host[128], webdir[128];
	char			*cp;
	char_t			wbuf[128];
	int noIp = FALSE;	// david


/*
 *	Initialize the socket subsystem
 */
	socketOpen();

//#ifdef USER_MANAGEMENT_SUPPORT
/*
 *	Initialize the User Management database
 */
//	umOpen();
//	umRestore(T("umconfig.txt"));
//#endif

/*
 *	Define the local Ip address, host name, default home page and the 
 *	root web directory.
 */
	if (gethostname(host, sizeof(host)) < 0) {
		printf("[webs] Can't get hostname!\n");
		error(E_L, E_LOG, T("Can't get hostname"));
		return -1;
	}
	DEBUG_P("[webs] gethostname host=%s\n",host);
	
/*
 *	Dick Modified 
 *	use getInAddr to replace gethostbyname in ucLinux
 */
#ifdef __mips__
	if ((hp = gethostbyname("localhost")) == NULL) {
		error(E_L, E_LOG, T("Can't get host address"));
		return -1;
	}
#else
	if ((hp = gethostbyname(host)) == NULL) {
		error(E_L, E_LOG, T("Can't get host address"));
		return -1;
	}
#endif
/*
 *	Dick Modified 
 */

	memcpy((char *) &intaddr, (char *) hp->h_addr_list[0],
		(size_t) hp->h_length);

/*
 *	Init MIB
 */


/*
 *	Set ../web as the root web. Modify this to suit your needs
 */
	sprintf(webdir, "%s", rootWeb);

/*
 *	Configure the web server options before opening the web server
 */
	websSetDefaultDir(webdir);
#ifdef __mips__
	if ( !noIp ) { // david
		cp = inet_ntoa(intaddr);
		ascToUni(wbuf, cp, min(strlen(cp) + 1, sizeof(wbuf)));
		websSetIpaddr(wbuf);
	}
#else
	cp = inet_ntoa(intaddr);
	ascToUni(wbuf, cp, min(strlen(cp) + 1, sizeof(wbuf)));
	websSetIpaddr(wbuf);
#endif
	ascToUni(wbuf, host, min(strlen(host) + 1, sizeof(wbuf)));
	websSetHost(wbuf);


/*
 *	Configure the web server options before opening the web server
 */
	websSetDefaultPage(T("default.asp"));
	websSetPassword(password);

/* 
 *	Open the web server on the given port. If that port is taken, try
 *	the next sequential port for up to "retries" attempts.
 */
	websOpenServer(port, retries); // remove GoAhead Web Server 

/*
 * 	First create the URL handlers. Note: handlers are called in sorted order
 *	with the longest path handler examined first. Here we define the security 
 *	handler, forms handler and the default web page handler.
 */
	websUrlHandlerDefine(T(""), NULL, 0, websSecurityHandler, 
		WEBS_HANDLER_FIRST);
	websUrlHandlerDefine(T("/goform"), NULL, 0, websFormHandler, 0);
	//websUrlHandlerDefine(T("/cgi-bin"), NULL, 0, websCgiHandler, 0);
	websUrlHandlerDefine(T(""), NULL, 0, websDefaultHandler, 
		WEBS_HANDLER_LAST); 

/*
 *	Init user management
 */

/*
 *	ASP script procedures and form functions.
 */

	websFormDefine(T("asp_setLan"), asp_setLan);
	websFormDefine(T("asp_setDmz"), asp_setDmz);
	websFormDefine(T("asp_dhcpServerAdvance"), asp_dhcpServerAdvance);
	websFormDefine(T("asp_setDhcpClient"), asp_setDhcpClient);
	websFormDefine(T("asp_setPppoe"), asp_setPppoe);
	websFormDefine(T("asp_setPppoeWizard"), asp_setPppoeWizard);
	websFormDefine(T("asp_setUnnumberedWizard"), asp_setUnnumberedWizard);
	websFormDefine(T("asp_setNat"), asp_setNat);
	websFormDefine(T("asp_setPing"), asp_setPing);
	websFormDefine(T("asp_setRouting"), asp_setRouting);
	websFormDefine(T("asp_setPort"), asp_setPort);
	websFormDefine(T("asp_setStaticWizard"), asp_setStaticWizard);
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
	websFormDefine(T("asp_setPptpWizard"), asp_setPptpWizard);	
	websFormDefine(T("asp_setL2tpWizard"), asp_setL2tpWizard);	
	websFormDefine(T("asp_setDhcpL2tpWizard"), asp_setDhcpL2tpWizard);	
#endif	
	websFormDefine(T("asp_upload"), asp_upload);
	websFormDefine(T("asp_setUrlFilter"), asp_setUrlFilter);
	websFormDefine(T("asp_restart"), asp_restart);
	websFormDefine(T("asp_systemDefault"), asp_systemDefault);
	websFormDefine(T("asp_wanIp"), asp_wanIp);
	websFormDefine(T("asp_setAcl"), asp_setAcl);
	websFormDefine(T("asp_setTZ"), asp_setTZ);
	websFormDefine(T("asp_setDdns"), asp_setDdns);
	websFormDefine(T("asp_setSpecialApplication"), asp_setSpecialApplication);
	websFormDefine(T("asp_setServerPort"), asp_setServerPort);
	websFormDefine(T("asp_setEventLog"), asp_setEventLog);
	websFormDefine(T("asp_setUpnp"), asp_setUpnp);
	websFormDefine(T("asp_setDos"), asp_setDos);	
	websFormDefine(T("asp_setDosProc"), asp_setDosProc);	
	websFormDefine(T("asp_setAlg"), asp_setAlg);		
	websFormDefine(T("asp_setUser"), asp_setUser);
	websFormDefine(T("asp_setRemoteMgmt"), asp_setRemoteMgmt);
	websFormDefine(T("asp_setWlanBasic"), asp_setWlanBasic);	
	websFormDefine(T("asp_setWlanAdvance"), asp_setWlanAdvance);	
	websFormDefine(T("asp_setWlanSecurity"), asp_setWlanSecurity);	
	websFormDefine(T("asp_setWlanAccessCtrl"), asp_setWlanAccessCtrl);		
	websFormDefine(T("asp_setWlanWds"), asp_setWlanWds);
	websFormDefine(T("asp_setUdpBlocking"), asp_setUdpBlocking);	
	websFormDefine(T("asp_setQos"), asp_setQos);
	websFormDefine(T("asp_setQos1"), asp_setQos1);
	websFormDefine(T("asp_setPbnat"), asp_setPbnat);
	websFormDefine(T("asp_setFlash"), asp_setFlash);
	websFormDefine(T("asp_setRateLimit"), asp_setRateLimit);
	websFormDefine(T("asp_setRatio_qos"), asp_setRatio_qos);

	websAspDefine(T("asp_rateLimit"), asp_rateLimit);
	websAspDefine(T("asp_acl"), asp_acl);
	websAspDefine(T("asp_dmz"), asp_dmz);
	websAspDefine(T("asp_ddns"), asp_ddns);
	websAspDefine(T("asp_upnp"), asp_upnp);
	websAspDefine(T("asp_dos"), asp_dos);
	websAspDefine(T("asp_dosProc"), asp_dosProc);
	websAspDefine(T("asp_alg"), asp_alg);
	websAspDefine(T("asp_user"), asp_user);
	websAspDefine(T("asp_remoteMgmt"), asp_remoteMgmt);	
	websAspDefine(T("asp_urlFilter"), asp_urlFilter);
	websAspDefine(T("asp_pppoe"), asp_pppoe);	
	websAspDefine(T("asp_ping"), asp_ping);	
	websAspDefine(T("asp_routing"), asp_routing);		
	websAspDefine(T("asp_port"), asp_port);		
	websAspDefine(T("asp_TZ"), asp_TZ);	
	websAspDefine(T("asp_TZ2"), asp_TZ2);	
	websAspDefine(T("asp_eventLog"), asp_eventLog);	
	websAspDefine(T("asp_serverPort"), asp_serverPort);	
	websAspDefine(T("asp_specialApplication"), asp_specialApplication);
	websAspDefine(T("asp_statusExtended"), asp_statusExtended);
	websAspDefine(T("asp_configChanged"), asp_configChanged);
	websAspDefine(T("asp_flashGetCfgParam"), asp_flashGetCfgParam);
	websAspDefine(T("asp_dhcpServerLeaseList"), asp_dhcpServerLeaseList);
	websAspDefine(T("asp_flashGetPppoeParam"), asp_flashGetPppoeParam);
	websAspDefine(T("asp_getWanAddress"), asp_getWanAddress);
	websAspDefine(T("asp_flashGetCloneMac"), asp_flashGetCloneMac);	
	websAspDefine(T("asp_flashGetString"), asp_flashGetString);
	websAspDefine(T("asp_flashGetIpElement"), asp_flashGetIpElement);
	websAspDefine(T("asp_wlanBasic"), asp_wlanBasic);	
	websAspDefine(T("asp_wlanAdvance"), asp_wlanAdvance);	
	websAspDefine(T("asp_wlanSecurity"), asp_wlanSecurity);	
	websAspDefine(T("asp_wlanAccessCtrl"), asp_wlanAccessCtrl);	
	websAspDefine(T("asp_wlanWds"), asp_wlanWds);
	websAspDefine(T("asp_udpBlocking"), asp_udpBlocking);			
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
	websAspDefine(T("asp_pptpWizard"), asp_pptpWizard);		
	websAspDefine(T("asp_l2tpWizard"), asp_l2tpWizard);			
	websAspDefine(T("asp_dhcpL2tpWizard"), asp_dhcpL2tpWizard);
#endif
	websAspDefine(T("asp_qos"), asp_qos);
	websAspDefine(T("asp_getLanPortStatus"), asp_getLanPortStatus);
	websAspDefine(T("asp_getWanPortStatus"), asp_getWanPortStatus);
	websAspDefine(T("asp_pbnat"), asp_pbnat);
	websAspDefine(T("asp_flash"), asp_flash);
	websAspDefine(T("asp_ratio_qos"), asp_ratio_qos);
	websAspDefine(T("asp_webcam"), asp_webcam);
	websAspDefine(T("asp_naptOpt"), asp_naptOpt);
	websFormDefine(T("asp_setNaptOpt"), asp_setNaptOpt);

/*
 *	Create the Form handlers for the User Management pages
 */
//#ifdef USER_MANAGEMENT_SUPPORT
//	formDefineUserMgmt();
//#endif

/*
 *	Create a handler for the default home page
 */
	websUrlHandlerDefine(T("/"), NULL, 0, websHomePageHandler, 0); 
	DEBUG_P("[webs] initWebs completed\n");
	return 0;
}

/******************************************************************************/
/*
 *	Test Javascript binding for ASP. This will be invoked when "aspTest" is
 *	embedded in an ASP page. See web/asp.asp for usage. Set browser to 
 *	"localhost/asp.asp" to test.
 */

/* Mark by Dick Tam */
/*
static int aspTest(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t	*name, *address;

	if (ejArgs(argc, argv, T("%s %s"), &name, &address) < 2) {
		websError(wp, 400, T("Insufficient args\n"));
		return -1;
	}
	return websWrite(wp, T("Name: %s, Address %s"), name, address);
}
*/
/* Mark by Dick Tam end */

/******************************************************************************/
/*
 *	Test form for posted data (in-memory CGI). This will be called when the
 *	form in web/forms.asp is invoked. Set browser to "localhost/forms.asp" to test.
 */

/* Mark by Dick Tam */
/*
static void formTest(webs_t wp, char_t *path, char_t *query)
{
	char_t	*name, *address;

	name = websGetVar(wp, T("name"), T("Joe Smith")); 
	address = websGetVar(wp, T("address"), T("1212 Milky Way Ave.")); 

	websHeader(wp);
	websWrite(wp, T("<body><h2>Name: %s, Address: %s</h2>\n"), name, address);
	websFooter(wp);
	websDone(wp, 200);
}
*/
/* Mark by Dick Tam end */

/******************************************************************************/
/*
 *	Home page handler
 */

static int websHomePageHandler(webs_t wp, char_t *urlPrefix, char_t *webDir,
	int arg, char_t *url, char_t *path, char_t *query)
{
/*
 *	If the empty or "/" URL is invoked, redirect default URLs to the home page
 */
	if (*url == '\0' || gstrcmp(url, T("/")) == 0) {
		websRedirect(wp, T("home.asp"));
		return 1;
	}
	return 0;
}

/******************************************************************************/
/*
 *	Default error handler.  The developer should insert code to handle
 *	error messages in the desired manner.
 */

void defaultErrorHandler(int etype, char_t *msg)
{
#if 1
	write(1, msg, gstrlen(msg));
#endif
}

/******************************************************************************/
/*
 *	Trace log. Customize this function to log trace output
 */

void defaultTraceHandler(int level, char_t *buf)
{
/*
 *	The following code would write all trace regardless of level
 *	to stdout.
 */
#if 0
	if (buf) {
		write(1, buf, gstrlen(buf));
	}
#endif
}

/******************************************************************************/
/*
 *	Returns a pointer to an allocated qualified unique temporary file name.
 *	This filename must eventually be deleted with bfree();
 */


/******************************************************************************/
/*
 *	Launch the CGI process and return a handle to it.
 */

int websLaunchCgiProc(char_t *cgiPath, char_t **argp, char_t **envp,
					  char_t *stdIn, char_t *stdOut)
{
	int	pid, fdin, fdout, hstdin, hstdout, rc;

	fdin = fdout = hstdin = hstdout = rc = -1; 
	if ((fdin = open(stdIn, O_RDWR | O_CREAT, 0666)) < 0 ||
		(fdout = open(stdOut, O_RDWR | O_CREAT, 0666)) < 0 ||
		(hstdin = dup(0)) == -1 ||
		(hstdout = dup(1)) == -1 ||
		dup2(fdin, 0) == -1 ||
		dup2(fdout, 1) == -1) {
		goto DONE;
	}
		
 	rc = pid = vfork();
 	if (pid == 0) {
/*
 *		if pid == 0, then we are in the child process
 */
		if (execve(cgiPath, argp, envp) == -1) {
			printf("content-type: text/html\n\n"
				"Execution of cgi process failed\n");
		}
		_exit (0);
	} 

DONE:
	if (hstdout >= 0) {
		dup2(hstdout, 1);
	}
	if (hstdin >= 0) {
		dup2(hstdin, 0);
	}
	if (fdout >= 0) {
		close(fdout);
	}
	if (fdin >= 0) {
		close(fdin);
	}
	return rc;
}

/******************************************************************************/
/*
 *	Check the CGI process.  Return 0 if it does not exist; non 0 if it does.
 */

int websCheckCgiProc(int handle)
{
/*
 *	Check to see if the CGI child process has terminated or not yet.  
 */
	if (waitpid(handle, NULL, WNOHANG) == handle) {
		return 0;
	} else {
		return 1;
	}
}

/******************************************************************************/

#ifdef B_STATS
static void memLeaks() 
{
	int		fd;
	if ((fd = gopen(T("/var/leak.txt"), O_CREAT | O_TRUNC | O_WRONLY)) >= 0) {
		bstats(fd, printMemStats);
		close(fd);
	}
}

/******************************************************************************/
/*
 *	Print memory usage / leaks
 */

static void printMemStats(int handle, char_t *fmt, ...)
{
	va_list		args;
	char_t		buf[256];

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);
	write(handle, buf, strlen(buf));
}
#endif

/******************************************************************************/
