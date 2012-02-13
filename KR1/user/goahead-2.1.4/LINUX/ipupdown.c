/*
* --------------------------------------------------------------------------
* Copyright c                  Realtek Semiconductor Corporation, 2003
* All rights reserved.
* 
* $Header: /home/cvsroot/uClinux-dist/user/goahead-2.1.4/LINUX/ipupdown.c,v 1.25 2004/07/12 07:03:47 tony Exp $
*
* Abstract: pppoe/ppp interface up/down event handler
*
* $Author: tony $
*
* $Log: ipupdown.c,v $
* Revision 1.25  2004/07/12 07:03:47  tony
* +: add wan type dhcpl2tp.
*
* Revision 1.24  2004/07/07 05:12:36  chhuang
* +: add a new WAN type (DHCP+L2TP). But not complete yet!!
*
* Revision 1.23  2004/06/11 09:20:18  tony
* +: using share memory instead of direct access for pRomeCfgParam.
*
* Revision 1.22  2004/06/10 10:34:52  tony
* +: add PPTP dial status: disconnecting, make redial function correct.
*
* Revision 1.21  2004/06/09 12:32:16  tony
* +: add PPTP/L2TP UI page.(Dial and Hang_Up).
*
* Revision 1.20  2004/05/12 05:15:05  tony
* support PPTP/L2TP in RTL865XB.
*
* Revision 1.19  2004/04/08 13:18:12  tony
* add PPTP/L2TP routine for MII lookback port.
*
* Revision 1.18  2004/01/14 02:47:12  hiwu
* add PPTP configuration
*
* Revision 1.17  2004/01/09 09:24:28  hiwu
* fix pRomeCfgParam get incorrect pointer
*
* Revision 1.16  2003/11/19 02:40:15  rupert
* fix pppd_start_dial
*
* Revision 1.15  2003/10/16 09:06:58  orlando
* add association between pppoe table entry (pppObjId) and pppx by
* parameter parsing argv[9]
*
* Revision 1.14  2003/10/15 12:12:08  orlando
* add pppoeCfgParam[].pppx to keep track of hw ppp obj id (0,1,2,3,...)
* in association with linux ppp dial up interface (ppp0,ppp1,ppp2,ppp3,...)
*
* Revision 1.13  2003/10/08 10:38:23  orlando
* pppoe session was treated as hexadecimal, should be deciaml.
*
* Revision 1.12  2003/10/02 10:50:17  orlando
* add manualHangup and whichPppObjId fields in pppoeCfgParam_t for auto
* reconnect implementation
*
* Revision 1.11  2003/10/01 12:39:31  tony
* solve all the compiler warnning messages in ipupdown.c
*
* Revision 1.10  2003/09/29 13:28:19  rupert
* add primary and secondary fields of ifCfgParam_s
*
* Revision 1.9  2003/09/29 12:58:34  tony
* add DDNS start/stop routine.
*
* Revision 1.8  2003/09/29 05:04:47  orlando
* board_ipUp/DownEventTableDriverAccess()
*
* Revision 1.7  2003/09/26 06:02:44  orlando
* take care of pppoeCfgParam[].dialState upon up/down event
*
* Revision 1.6  2003/09/26 05:51:27  orlando
* tune ip/down sequences
*
* Revision 1.5  2003/09/26 01:54:44  orlando
* pppeCfgParam adds more fields. tune code to adapt the change
*
* Revision 1.4  2003/09/25 06:06:45  orlando
* adjust inclue files
*
* Revision 1.3  2003/09/25 05:21:43  orlando
* change file abstract in file header
*
* Revision 1.2  2003/09/25 05:20:31  orlando
* add file header for cvs control
*
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <re865x.h>
#include <board.h>
#include <string.h>
#include <stdlib.h>
#include "rtl_board.h"
#include <linux/config.h>

/* global cfg structure pointer */
extern romeCfgParam_t *pRomeCfgParam;

//extern uint32 getRomeCfgParamPtr(romeCfgParam_t ** ptr);

void ipupdown(int argc, char*argv[])
{
	uint32   i;
	FILE   *f;
    //unsigned long args[4];	
	char     tempStr[30];
	uint32   pppObjId;
	ipaddr_t myIp;
	ipaddr_t myGw;
	ipaddr_t myDns;
	uint16   mySessionId;
	uint8    myMac[6];
	uint8    pppx;
	//rtl8651_sem_wait(SEMAPHORE_IPUPDOWN);
	
	printf("entering ipupdown ...\n");

 	
	if (strstr(argv[0],"ip-up"))
	{
		system("/bin/priority CSQ&");
		
		memset(tempStr,0,sizeof(tempStr));
		for (i=0;i<6;i++)
		{
			memcpy(tempStr,(char*)&argv[7][i*3],3);
			myMac[i]=(unsigned char)strtoul(tempStr,NULL,16);
		}
		
		myIp = (ipaddr_t)inet_addr(argv[5]);
		myGw = (ipaddr_t)inet_addr(argv[6]);
		myDns = (ipaddr_t)inet_addr(argv[8]);
		mySessionId = (uint16)strtoul(argv[4],NULL,10);

		f = fopen("/var/tmp/ip.txt", "w");
		if(f != NULL){
			fprintf(f, "%s\n", argv[6]);
			fprintf(f, "%s\n", argv[8]);
			fprintf(f, "%s\n", argv[9]);
			fclose(f);
		}
#if 0
		if(getRomeCfgParamPtr(&pRomeCfgParam)){
			i=strtoul(argv[3],NULL,16);
			pRomeCfgParam = (romeCfgParam_t *)i;			
		}		
#endif		

		#if 0		
		if (strstr("ppp0",argv[1]))
		{
			pppObjId = 0;
		}
		else if (strstr("ppp1",argv[1]))
		{
			pppObjId = 1;
		}
		else if (strstr("ppp2",argv[1]))
		{
			pppObjId = 2;
		}
		else if (strstr("ppp3",argv[1]))
		{
			pppObjId = 3;
		}
		else if (strstr("ppp4",argv[1]))
		{
			pppObjId = 4;
		}
		else
		{
			pppObjId = 0;
		}
		#else		
		pppx = (uint8)(argv[1][3] - '0');
	
		//pppObjId = 0; /* default harmless value */
		//for(i=0;i<MAX_PPPOE_SESSION;i++)
		//	if (pRomeCfgParam->pppoeCfgParam[i].dialState == PPPOECFGPARAM_DIALSTATE_DIALED_TRYING)
		//	{
		//		pRomeCfgParam->pppoeCfgParam[i].pppx = pppx;
		//		pppObjId = i;
		//	}
		
		#endif
				
		if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_PPPOE)
		{						
			pppObjId = (uint32)strtoul(argv[9],NULL,10);
			pRomeCfgParam->pppoeCfgParam[pppObjId].pppx = pppx;
			
			memcpy(pRomeCfgParam->ifCfgParam[0].ipAddr,&myIp,4);
			i=0xffffffff;memcpy(pRomeCfgParam->ifCfgParam[0].ipMask,&i,4);
			memcpy(pRomeCfgParam->ifCfgParam[0].gwAddr,&myGw,4);
			memcpy(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,&myDns,4);
			memcpy(pRomeCfgParam->pppoeCfgParam[0].svrMac,myMac,6);
			pRomeCfgParam->pppoeCfgParam[0].sessionId=mySessionId;
		}
		else if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)
		{			
			pppObjId = (uint32)strtoul(argv[9],NULL,10);
			pRomeCfgParam->pppoeCfgParam[pppObjId].pppx = pppx;
			
			memcpy(pRomeCfgParam->pppoeCfgParam[pppObjId].ipAddr,&myIp,4);
			i=0xffffffff;memcpy(pRomeCfgParam->pppoeCfgParam[pppObjId].ipMask,&i,4);
			memcpy(pRomeCfgParam->pppoeCfgParam[pppObjId].gwAddr,&myGw,4);
			memcpy(pRomeCfgParam->pppoeCfgParam[pppObjId].dnsAddr,&myDns,4);
			memcpy(pRomeCfgParam->pppoeCfgParam[pppObjId].svrMac,myMac,6);
			pRomeCfgParam->pppoeCfgParam[pppObjId].sessionId=mySessionId;
			pRomeCfgParam->pppoeCfgParam[0].whichPppObjId = pppObjId;
		}
		else if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED)
		{
			pppObjId = (uint32)strtoul(argv[9],NULL,10);
			pRomeCfgParam->pppoeCfgParam[pppObjId].pppx = pppx;
						
			memcpy(pRomeCfgParam->ifCfgParam[0].ipAddr,&myIp,4);
			memcpy(pRomeCfgParam->ifCfgParam[0].ipMask,pRomeCfgParam->pppoeCfgParam[0].unnumberedIpMask,4);
			memcpy(pRomeCfgParam->ifCfgParam[0].gwAddr,&myGw,4);
			memcpy(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,&myDns,4);
			pRomeCfgParam->pppoeCfgParam[0].sessionId=mySessionId;
			memcpy(pRomeCfgParam->pppoeCfgParam[0].svrMac,myMac,6);
		}
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
		else if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_PPTP)
		{
			memcpy(pRomeCfgParam->ifCfgParam[0].ipAddr,&myIp,4);
			i=0xffffffff;memcpy(pRomeCfgParam->ifCfgParam[0].ipMask,&i,4);
			memcpy(pRomeCfgParam->ifCfgParam[0].gwAddr,&myGw,4);
			memcpy(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,&myDns,4);			
			//system("/bin/iptables -t nat -A POSTROUTING -o ppp0 -j MASQUERADE");			
		}
		else if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_L2TP)
		{
			memcpy(pRomeCfgParam->ifCfgParam[0].ipAddr,&myIp,4);
			i=0xffffffff;memcpy(pRomeCfgParam->ifCfgParam[0].ipMask,&i,4);
			memcpy(pRomeCfgParam->ifCfgParam[0].gwAddr,&myGw,4);
			memcpy(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,&myDns,4);			
			//system("/bin/iptables -t nat -A POSTROUTING -o ppp0 -j MASQUERADE");			
		}
		else if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_DHCPL2TP)
		{
			memcpy(pRomeCfgParam->ifCfgParam[0].ipAddr,&myIp,4);
			i=0xffffffff;memcpy(pRomeCfgParam->ifCfgParam[0].ipMask,&i,4);
			memcpy(pRomeCfgParam->ifCfgParam[0].gwAddr,&myGw,4);
			memcpy(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,&myDns,4);	
		}
#endif		
#ifdef CONFIG_RTL865XB_3G
		else if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_3G)
		{
			memcpy(pRomeCfgParam->ifCfgParam[0].ipAddr,&myIp,4);
			i=0xffffffff;memcpy(pRomeCfgParam->ifCfgParam[0].ipMask,&i,4);
			memcpy(pRomeCfgParam->ifCfgParam[0].gwAddr,&myGw,4);
			memcpy(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,&myDns,4);
			memcpy(pRomeCfgParam->ifCfgParam[0].dnsSecondaryAddr,&myDns,4);
			
			
			f=fopen("/var/resolv.conf", "w");
			if( f != NULL ){
				fprintf(f, "nameserver %s\n", argv[8]);
				fprintf(f, "nameserver %s\n", argv[9]);
				fclose(f);
			}
			// used in new SDK, need to be patched,
			//if (pRomeCfgParam->ifCfgParam[0].fwdWins == 1)
			//{
				//pRomeCfgParam->ifCfgParam[0].winsPrimaryAddr = myPrimaryWins;
				//pRomeCfgParam->ifCfgParam[0].winsSecondaryAddr = mySecondaryWins;
			//}
			//system("/bin/iptables -t nat -A POSTROUTING -o ppp0 -j MASQUERADE");	
			
			// WAN interface is UP, luminesce WAN_EVDO & STATUS LEDs, dim DIAL LED
			system("gpio 1 1 5 4");
			system("gpio 1 1 10 0");
			system("gpio 1 1 2 2");
			
				
				
			// Testing for keeping connection alive
			//system("/bin/ping 66.174.6.7 &");		
		}
	
#endif
		else {
			return ;
		}	
		/* configure the asic thru table driver APIs */
		
		board_ipUpEventTableDriverAccess();

		
	} /* end "ip-up" */

	
	/********************************************************************/
	/********************************************************************/
	/********************************************************************/
	
	if (strstr(argv[0],"ip-down"))
	{
		myIp = (ipaddr_t)inet_addr(argv[5]);
		myGw = (ipaddr_t)inet_addr(argv[6]);
		myDns = (ipaddr_t)inet_addr(argv[8]);


		/* fetch pRomeCfgParam from argument list */

#if 0
		if(getRomeCfgParamPtr(&pRomeCfgParam)){
			i=strtoul(argv[3],NULL,16);
			pRomeCfgParam = (romeCfgParam_t *)i;
		}
#endif
pppx=0;
pppObjId=0;
		
		if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_DHCPC)
		{
			/* dhcp renew */
		}				
		else if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_PPPOE)
		{
			/* single pppoe hangup case */			
			pRomeCfgParam->pppoeCfgParam[0].dialState = PPPOECFGPARAM_DIALSTATE_OFF;
		}
		else if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)
		{
			pppx = (uint8)(argv[1][3]-'0');
			pppObjId = (uint32)strtoul(argv[9],NULL,10);	
			/* multiple pppoe hangup case */
			pRomeCfgParam->pppoeCfgParam[0].whichPppObjId = pppObjId;
			pRomeCfgParam->pppoeCfgParam[pppObjId].dialState = PPPOECFGPARAM_DIALSTATE_OFF;
		}		
		else if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED)
		{
			/* ip unnumbered pppoe hangup case */			
			pRomeCfgParam->pppoeCfgParam[0].dialState = PPPOECFGPARAM_DIALSTATE_OFF;
		} 
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
		else if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_PPTP)
		{
			/* single pptp hangup case */			
			pRomeCfgParam->pptpCfgParam.dialState = PPTPCFGPARAM_DIALSTATE_DISCONNECTING;
			//system("/bin/iptables -t nat -F");			
		}
		else if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_L2TP)
		{
			/* single l2tp hangup case */			
			pRomeCfgParam->l2tpCfgParam.dialState = L2TPCFGPARAM_DIALSTATE_OFF;
			//system("/bin/iptables -t nat -F");			
		}
		else if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_DHCPL2TP)
		{			
			pRomeCfgParam->l2tpCfgParam.dialState = L2TPCFGPARAM_DIALSTATE_OFF;
		}
#endif		
#ifdef CONFIG_RTL865XB_3G
		else if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_3G)
		{
			// dim WAN EVDO LED and STATUS LED
			system("rm /var/tmp/cardinfo");  // 2005-10-09: Peter add 1 line for 3G
			system("gpio 1 1 5 0");
			system("gpio 1 1 10 8");
		}
	
#endif
		else {
			return ;
		}

		/* configure the asic thru table driver APIs */
		board_ipDownEventTableDriverAccess();

	} /* end "ip-down" */

	//rtl8651_sem_post(SEMAPHORE_IPUPDOWN);
} /* end ipupdown() */

/* end ipupdown.c */
