/*
 * Copyright c                Realtek Semiconductor Corporation, 2003
 * All rights reserved.                                                    
 * 
 * $Header: /home/cvsroot/uClinux-dist/user/goahead-2.1.4/LINUX/tblDrv_mapping.c,v 1.70 2004/08/04 14:48:56 yjlou Exp $
 *
 * $Author: yjlou $
 *
 * Abstract:
 *
 *   tblDrv_mapping: call kernel space table driver via user space 
 *
 * $Log: tblDrv_mapping.c,v $
 * Revision 1.70  2004/08/04 14:48:56  yjlou
 * *: merge rtl_bdinfo.h into flashdrv.h
 *
 * Revision 1.69  2004/07/27 07:23:53  chenyl
 * +: DoS ignore type setting
 *
 * Revision 1.68  2004/07/18 11:27:10  chhuang
 * *: modify M-N-1 Queue's web page
 *
 * Revision 1.67  2004/07/14 13:55:59  chenyl
 * +: web page for MN queue
 *
 * Revision 1.66  2004/07/12 04:25:36  chenyl
 * *: extend existing port scan mechanism
 *
 * Revision 1.65  2004/07/08 10:51:41  tony
 * *: change ipUp routine to new arch.
 *
 * Revision 1.64  2004/07/06 06:21:16  chhuang
 * +: add rate limit
 *
 * Revision 1.63  2004/06/29 09:43:05  cfliu
 * *: change set MTU API prototype
 *
 * Revision 1.62  2004/06/29 07:35:42  chenyl
 * +: igmp-proxy group filter
 *
 * Revision 1.61  2004/06/17 07:51:26  tony
 * +: add call setNetMtu in PPTP/L2TP mode.
 *
 * Revision 1.60  2004/06/11 10:53:59  cfliu
 * +: Add Cable Meter and Interface counter webpage
 *
 * Revision 1.59  2004/05/31 14:39:51  jzchen
 * Add a layer of API to separate user space and kernel IO control
 *
 * Revision 1.58  2004/05/28 06:11:51  yjlou
 * +: add IOCTL for rtl8651_addProtocolBasedNAT() and rtl8651_delProtocolBasedNAT()
 *
 * Revision 1.57  2004/05/21 12:08:50  chenyl
 * *: TCP/UDP/ICMP spoof -> check for (sip) == (dip)
 * *: modify DOS prevention webpage
 *
 * Revision 1.56  2004/05/20 08:43:55  chhuang
 * add Web Page for QoS
 *
 * Revision 1.55  2004/05/12 07:20:15  chenyl
 * +: source ip blocking
 * *: modify dos mechanism
 *
 * Revision 1.54  2004/05/12 05:15:05  tony
 * support PPTP/L2TP in RTL865XB.
 *
 * Revision 1.53  2004/05/10 05:49:20  chhuang
 * add src ip blocking for url-filtering
 *
 * Revision 1.52  2004/05/05 08:27:07  tony
 * new features: add remote management spec
 *
 * Revision 1.51  2004/04/30 13:00:32  chenyl
 * *: remove CONFIG_RTL865X_MULTICAST and always include multicast/igmp function
 *
 * Revision 1.50  2004/04/29 05:42:18  chhuang
 * add new API rtl8651_addNaptMappingExt()
 *
 * Revision 1.49  2004/04/19 13:45:18  tony
 * add resetPptpProprity and resetL2tpProprity IOCTL.
 *
 * Revision 1.48  2004/04/14 02:45:50  tony
 * add ioctl RTL8651_IOCTL_SETLOOPBACKPORTPHY
 *
 * Revision 1.47  2004/04/13 14:50:56  chhuang
 * correct setLoopBackPort
 *
 * Revision 1.46  2004/04/13 08:15:53  tony
 * fix bug: make PPTP/L2TP run stably.
 *
 * Revision 1.45  2004/04/09 02:26:12  tony
 * remove rtl8651_specifyNetworkIntfLinkLayerType_1 IOCTL.
 *
 * Revision 1.44  2004/04/08 13:18:12  tony
 * add PPTP/L2TP routine for MII lookback port.
 *
 * Revision 1.43  2004/04/08 12:30:08  cfliu
 * add rtl8651a_delAclRule()
 *
 * Revision 1.42  2004/02/09 09:18:01  tony
 * add function: user is able to add protocol stack Server used TCP ports by
 * rtl8651_addProtoStackServerUsedTcpPort(incoming_src_port); and flush the
 * table by rtl8651_flushProtoStackServerUsedTcpPort();
 *
 * Revision 1.41  2004/02/04 06:23:49  rupert
 * Add UDP Block Support
 *
 * Revision 1.40  2004/01/30 12:03:05  tony
 * make Special Application is able to support multiple session.
 *
 * Revision 1.39  2004/01/29 12:17:09  rupert
 * Add Multiple rtl8651a_setWanStatus() support
 *
 * Revision 1.38  2004/01/29 09:38:02  tony
 * add case RTL8651_IOCTL_ADDNAPTSERVERPORTRANGE ,RTL8651_IOCTL_DELNAPTSERVERPORTRANGE
 *
 * Revision 1.37  2004/01/16 12:16:46  tony
 * modify ALG cfg params and web UI,
 * rearrange rtl8651a_setAlgStatus() must called after rtl8651_addIpIntf()
 *
 * Revision 1.36  2004/01/14 03:43:08  tony
 * modify rtl8651_setAlgStatus to rtl8651a_setAlgStatus
 *
 * Revision 1.35  2004/01/13 08:38:30  orlando
 * modify rtl8651_addAclRule()'s inputCheck
 *
 * Revision 1.34  2004/01/12 09:50:32  orlando
 * add mapping for rtl8651_setPppoeDefaultDialSessionId()
 *
 * Revision 1.33  2004/01/09 08:26:52  orlando
 * checkin acl related api change
 *
 * Revision 1.32  2004/01/09 08:03:55  tony
 * make the code architecture of ALG is able to support multiple dial session.
 *
 * Revision 1.31  2004/01/09 06:12:01  orlando
 * checkin serverport api change
 *
 * Revision 1.30  2004/01/09 05:03:59  orlando
 * checkin dmz api change
 *
 * Revision 1.29  2004/01/07 05:38:30  orlando
 * logging feature sercomm phase2 spec 1st milestone checkin
 *
 * Revision 1.28  2004/01/07 03:43:53  orlando
 * checkin sercomm phase2 spec 1st milestone api
 *
 * Revision 1.27  2003/12/18 13:17:57  orlando
 * change rtl8651_flushPolicyRoute(int32) to (uint32)
 *
 * Revision 1.26  2003/12/12 02:43:03  tony
 * add rtl8651_setMtu() function.
 *
 * Revision 1.25  2003/12/10 03:08:58  tony
 * make logging function support time zone in kernel space.
 *
 * Revision 1.24  2003/12/09 03:05:44  tony
 * add a param in rtl8651_setPppoeMtu function.
 *
 * Revision 1.23  2003/12/08 13:54:03  orlando
 * add rtl8651_enableLogging()
 *
 * Revision 1.22  2003/12/03 12:08:08  tony
 * fix bug: add a new field in rtl8651_addTriggerPortRule.
 *
 * Revision 1.21  2003/12/02 10:24:47  tony
 * Add Routine: support DoS is able to set threshold by user(Webs GUI).
 *
 * Revision 1.20  2003/12/01 12:35:52  tony
 * make ALG is able to be configured by users(Web GUI).
 *
 * Revision 1.19  2003/11/28 01:49:06  rupert
 * FIX MULTIPLE HANGUP
 *
 * Revision 1.18  2003/11/21 01:56:48  tony
 * add function: user is able to add protocol stack used UDP ports by
 * rtl8651_addProtoStackUsedUdpPort(incoming_src_port);
 *
 * Revision 1.17  2003/11/18 13:25:39  rupert
 * add dial demand
 *
 * Revision 1.16  2003/11/18 09:04:47  tony
 * add routine: support mtu configure by user in pppoe.
 *
 * Revision 1.15  2003/11/06 12:14:59  rupert
 * add add/del policy route API
 *
 * Revision 1.14  2003/10/30 11:14:40  orlando
 * add command "addflow"
 *
 * Revision 1.13  2003/10/28 08:33:45  orlando
 * rtl8651_setAsicNaptAutoAddDelete() obsoleted by rtl8651_enableNaptAutoAdd()
 *
 * Revision 1.12  2003/10/24 10:25:58  tony
 * add DoS attack interactive webpage,
 * FwdEngine is able to get WAN status by rtl8651_wanStatus(0:disconnect,1:connect)
 *
 * Revision 1.11  2003/10/20 04:50:28  orlando
 * add _RTL_LOGGING related stuff
 *
 * Revision 1.10  2003/10/09 13:23:44  tony
 * TRIGGER_PORT: add function rtl8651_flushTriggerPortRules,rtl8651_addTriggerPortRule
 *
 * Revision 1.9  2003/10/01 13:40:39  tony
 * solve all the compiler warnning message in tblDrv_mapping.c
 *
 * Revision 1.8  2003/10/01 09:41:32  orlando
 * check in dial-on-demand and silent timeout related code
 *
 * Revision 1.7  2003/10/01 05:57:32  tony
 * add URL Filter routine
 *
 * Revision 1.6  2003/09/30 08:05:08  orlando
 * change from RTL8651_FLUSHACL to RTL8651_FLUSHACLRULE
 *
 * Revision 1.5  2003/09/26 05:50:43  orlando
 * fix bug: delRoute used wrong IOCTL_ADDROUTE constant
 *
 * Revision 1.4  2003/09/25 02:15:32  orlando
 * Big Change
 *
 * Revision 1.3  2003/09/19 01:43:50  tony
 * add dmz routine
 *
 *
 */

#include <string.h>
//#include "board.h"
//#include <rt_stack/h/sys/porting.h>
//#include "rtl_cfgmgr.h"
#include "rtl_board.h" /* exported function prototypes */
#include <rtl8651_tblDrv.h>
#include <re865x.h>
#include <linux/config.h>

// Dino Chang, 2004/11/08
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

extern int re865xIoctl(char *name,uint32 arg0, uint32 arg1, uint32 arg2, uint32 arg3);

/*
 * re865x Ioctl (to call kernel functions including rtl8651 table driver APIs
 */
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
int32 rtl8651a_addAclRuleExt(uint32 dsid,int8 * ifName, int8 inputCheck, rtl8651_tblDrvAclRule_t * rule)
{
        uint32 ret;
        uint32 ptr[4];
        //re865xIoctl(ifName,RTL8651_ADDIPACL,(uint32)(ifName),(uint32)inputCheck,(uint32*)rule);
        ptr[0]=(uint32)dsid;
        ptr[1]=(uint32)ifName;
        ptr[2]=(uint32)inputCheck;
        ptr[3]=(uint32)rule;
        re865xIoctl(ifName,RTL8651_ADDACLEXT,(uint32)(ptr),0,(uint32)&ret);
        return ret;
}

int32 rtl8651a_delAclRuleExt(uint32 dsid,int8 * ifName, int8 inputCheck, rtl8651_tblDrvAclRule_t * rule)
{
        uint32 ret;
        uint32 ptr[4];
        ptr[0]=(uint32)dsid;
        ptr[1]=(uint32)ifName;
        ptr[2]=(uint32)inputCheck;
        ptr[3]=(uint32)rule;
        re865xIoctl(ifName,RTL8651_DELACLEXT,(uint32)(ptr),0,(uint32)&ret);
        return ret;
}
// Dino Chang

//ip-acl 
int32 rtl8651a_addAclRule(uint32 dsid,int8 * ifName, int8 inputCheck, rtl8651_tblDrvAclRule_t * rule)
{
	uint32 ret;
	uint32 ptr[4];
	//re865xIoctl(ifName,RTL8651_ADDIPACL,(uint32)(ifName),(uint32)inputCheck,(uint32*)rule);
	ptr[0]=(uint32)dsid;
	ptr[1]=(uint32)ifName;
	ptr[2]=(uint32)inputCheck;
	ptr[3]=(uint32)rule;
	
	re865xIoctl(ifName,RTL8651_ADDACL,(uint32)(ptr),0,(uint32)&ret);
	return ret;
}

int32 rtl8651a_delAclRule(uint32 dsid,int8 * ifName, int8 inputCheck, rtl8651_tblDrvAclRule_t * rule)
{
	uint32 ret;
	uint32 ptr[4];
	ptr[0]=(uint32)dsid;
	ptr[1]=(uint32)ifName;
	ptr[2]=(uint32)inputCheck;
	ptr[3]=(uint32)rule;
	re865xIoctl(ifName,RTL8651_DELACL,(uint32)(ptr),0,(uint32)&ret);
	return ret;
}


int32 rtl8651a_flushAclRule(uint32 dsid,int8 * ifName, int8 inputCheck)
{
	uint32 ret;
	uint32 ptr[3];
	ptr[0]=(uint32)dsid;
	ptr[1]=(uint32)ifName;
	ptr[2]=(uint32)inputCheck;
	re865xIoctl(ifName,RTL8651_FLUSHACLRULE,(uint32)ptr,0,(uint32)&ret);	
	return ret;
}


int32 rtl8651a_setDefaultAcl(uint32 dsid,int8 *ifName, int8 isDrop)
{
	uint32 ret;
	uint32 ptr[3];
	ptr[0]=(uint32)dsid;
	ptr[1]=(uint32)ifName;
	ptr[2]=(uint32)isDrop;
	re865xIoctl(ifName,RTL8651_SETDEFAULTACL ,(uint32)ptr,0,(uint32)&ret);
	//re865xIoctl(ifName,RTL8651_SETDEFAULTACL ,(uint32)(ifName),isDrop,(uint32)&ret);
	return ret;
}

//int32 rtl8651_delIpAcl(int8 * ifName, int8 inputCheck, rtl8651_tblDrvIpAclRule_t * rule);
//int32 rtl8651_delVlanIpAcl(uint16 vid, int8 inputCheck, rtl8651_tblDrvIpAclRule_t * rule);
//int32 rtl8651_flushVlanIpAcl(uint16 vid, int8 inputCheck);
//int32 rtl8651_addVlanIpAcl(uint16 vid, int8 inputCheck, rtl8651_tblDrvIpAclRule_t * rule);

int32 rtl8651a_addNaptServerPortMapping(uint32 dsid,int8 isTcp, ipaddr_t extIpAddr, uint16 extPort, ipaddr_t intIpAddr, uint16 intPort)
{
	uint32 ret;
	uint32 ptr[6];	
	ptr[0]=(uint32)dsid;
	ptr[1]=(uint32)&isTcp;
	ptr[2]=(uint32)&extIpAddr;
	ptr[3]=(uint32)&extPort;
	ptr[4]=(uint32)&intIpAddr;
	ptr[5]=(uint32)&intPort;
	re865xIoctl("eth0",RTL8651_ADDNAPTSERVERPORTMAPPING,(uint32)(ptr),0,(uint32)&ret);
	return ret;	
}

int32 rtl8651a_delNaptServerPortMapping(uint32 dsid,int8 isTcp, ipaddr_t extIpAddr, uint16 extPort, ipaddr_t intIpAddr, uint16 intPort)
{
	uint32 ret;
	uint32 ptr[6];
	ptr[0]=(uint32)dsid;
	ptr[1]=(uint32)&isTcp;
	ptr[2]=(uint32)&extIpAddr;
	ptr[3]=(uint32)&extPort;
	ptr[4]=(uint32)&intIpAddr;
	ptr[5]=(uint32)&intPort;
	re865xIoctl("eth0",RTL8651_DELNAPTSERVERPORTMAPPING,(uint32)(ptr),0,(uint32)&ret);
	return ret;		
}


int32 rtl8651a_addNaptServerPortRange(uint32 dsid,int8 isTcp, ipaddr_t extIpAddr, uint16 extPort, ipaddr_t intIpAddr, uint16 intPort, uint16 nCount)
{
	uint32 ret;
	uint32 ptr[6];	
	ptr[0]=(uint32)dsid;
	ptr[1]=(uint32)&isTcp;
	ptr[2]=(uint32)&extIpAddr;
	ptr[3]=(uint32)&extPort;
	ptr[4]=(uint32)&intIpAddr;
	ptr[5]=(uint32)&intPort;
	ptr[6]=(uint32)&nCount;	
	re865xIoctl("eth0",RTL8651_IOCTL_ADDNAPTSERVERPORTRANGE,(uint32)(ptr),0,(uint32)&ret);
	return ret;	
}

int32 rtl8651a_delNaptServerPortRange(uint32 dsid,int8 isTcp, ipaddr_t extIpAddr, uint16 extPort, ipaddr_t intIpAddr, uint16 intPort, uint16 nCount)
{
	uint32 ret;
	uint32 ptr[6];
	ptr[0]=(uint32)dsid;
	ptr[1]=(uint32)&isTcp;
	ptr[2]=(uint32)&extIpAddr;
	ptr[3]=(uint32)&extPort;
	ptr[4]=(uint32)&intIpAddr;
	ptr[5]=(uint32)&intPort;
	ptr[6]=(uint32)&nCount;	
	re865xIoctl("eth0",RTL8651_IOCTL_DELNAPTSERVERPORTRANGE,(uint32)(ptr),0,(uint32)&ret);
	return ret;		
}

int32 rtl8651_setAsicNaptAutoAddDelete(int8 autoAdd, int8 autoDelete)
{
	int32  ret;
	uint32 args[2];
	args[0]=(uint32)autoAdd;
	args[1]=(uint32)autoDelete;
	re865xIoctl("eth0",RTL8651_IOCTL_SETASICNAPTAUTOADDDELETE,(uint32)(args),(uint32)&ret,0);
	return ret;
} /* end rtl8651_setAsicNaptAutoAddDelete */

int32 rtl8651_enableNaptAutoAdd(int8 autoAdd)
{
	int32  ret;
	uint32 args[2];
	args[0]=(uint32)autoAdd;
	re865xIoctl("eth0",RTL8651_IOCTL_ENABLENAPTAUTOADD,(uint32)(args),(uint32)&ret,0);
	return ret;
} /* end rtl8651_enableNaptAutoAdd */

int32 rtl8651_addNetworkIntf(int8 *ifName)
{
	int32  ret;
	uint32 args[2];
	args[0]=(uint32)ifName;
	re865xIoctl("eth0",RTL8651_IOCTL_ADDNETWORKINTF,(uint32)(args),(uint32)&ret,0);
	return ret;
} /* end rtl8651_addNetworkIntf */

//If a routing entry/DMZ host/NAT host refers to the network interface to be deleted, delete operation is forbidden. 
int32 rtl8651_delNetworkIntf(int8 *ifName)
{
	int32  ret;
	uint32 args[2];
	args[0]=(uint32)ifName;
	re865xIoctl("eth0",RTL8651_IOCTL_DELNETWORKINTF,(uint32)(args),(uint32)&ret,0);
	return ret;
} /* end rtl8651_delNetworkIntf */

int32 rtl8651_removeNetworkIntfLinkLayerType(int8 * ifName)
{
	int32  ret;
	uint32 args[3];
	args[0]=(uint32)ifName;
	re865xIoctl("eth0",RTL8651_IOCTL_REMOVENETWORKINTFLINKLAYERTYPE,(uint32)(args),(uint32)&ret,0);
	return ret;	
} /* end rtl8651_removeNetworkIntfLinkLayerType */

int32 rtl8651_specifyNetworkIntfLinkLayerType(int8 * ifName, uint32 llType, uint16 vid)
{
	int32  ret;
	uint32 args[3];
	args[0]=(uint32)ifName;
	args[1]=(uint32)llType;
	args[2]=(uint32)vid;
	re865xIoctl("eth0",RTL8651_IOCTL_SPECIFYNETWORKINTFLINKLAYERTYPE,(uint32)(args),(uint32)&ret,0);
	return ret;	
} /* end rtl8651_specifyNetworkIntfLinkLayerType */


int32 rtl8651_setPppoeSessionProperty(uint32 pppoeId, uint16 sid, ether_addr_t * macAddr, uint32 port)
{
	int32  ret;
	uint32 args[4];
	args[0]=(uint32)pppoeId;
	args[1]=(uint32)sid;
	args[2]=(uint32)macAddr;
	args[3]=(uint32)port;
	re865xIoctl("eth0",RTL8651_IOCTL_SETPPPOESESSIONPROPERTY,(uint32)(args),(uint32)&ret,0);
	return ret;
} /* end rtl8651_setPppoeSessionProperty */

int32 rtl8651_resetPppoeSessionProperty(uint32 pppoeId)
{
	int32  ret;
	uint32 args[2];
	args[0]=(uint32)pppoeId;
	re865xIoctl("eth0",RTL8651_IOCTL_RESETPPPOESESSIONPROPERTY,(uint32)(args),(uint32)&ret,0);
	return ret;
} /* end rtl8651_resetPppoeSessionProperty */


int32 rtl8651_addPppoeSession(uint32 pppoeId, uint16 vid)
{
	int32  ret;
	uint32 args[2];
	args[0]=(uint32)pppoeId;
	args[1]=(uint32)vid;
	re865xIoctl("eth0",RTL8651_IOCTL_ADDPPPOESESSION,(uint32)(args),(uint32)&ret,0);
	return ret;
}

int32 rtl8651_delPppoeSession(uint32 pppoeId)
{
	int32  ret;
	uint32 args[2];
	args[0]=(uint32)pppoeId;
	re865xIoctl("eth0",RTL8651_IOCTL_DELPPPOESESSION,(uint32)(args),(uint32)&ret,0);
	return ret;
}

int32 rtl8651_bindPppoeSession(ipaddr_t ipaddr, uint32 pppoeId)
{
	int32  ret;
	uint32 args[2];
	args[0]=(uint32)ipaddr;
	args[1]=(uint32)pppoeId;
	re865xIoctl("eth0",RTL8651_IOCTL_BINDPPPOESESSION,(uint32)(args),(uint32)&ret,0);
	return ret;
}

int32 rtl8651_setPppoeDefaultDialSessionId(uint32 dsid)
{
	int32  ret;
	uint32 args[2];
	args[0]=(uint32)dsid;
	//args[1]=(uint32)pppoeId;
	re865xIoctl("eth0",RTL8651_IOCTL_SETPPPOEDEFAULTDIALSESSIONID,(uint32)(args),(uint32)&ret,0);
	return ret;
}

int32 rtl8651_addIpIntf(int8 * ifName, ipaddr_t ipAddr, ipaddr_t ipMask)
{
	int32  ret;
	uint32 args[3];
	args[0]=(uint32)ifName;
	args[1]=(uint32)ipAddr;
	args[2]=(uint32)ipMask;
	re865xIoctl("eth0",RTL8651_IOCTL_ADDIPINTF,(uint32)(args),(uint32)&ret,0);
	return ret;
}

int32 rtl8651_delIpIntf(int8 * ifName, ipaddr_t ipAddr, ipaddr_t ipMask)
{
	int32  ret;
	uint32 args[3];
	args[0]=(uint32)ifName;
	args[1]=(uint32)ipAddr;
	args[2]=(uint32)ipMask;
	re865xIoctl("eth0",RTL8651_IOCTL_DELIPINTF,(uint32)(args),(uint32)&ret,0);
	return ret;
}

int32 rtl8651_addSession(uint32 type, uint32 sessionId, int8 *ifName, uint16 vid)
{
	int32  ret;
	uint32 args[4];
	args[0]=(uint32)&type;
	args[1]=(uint32)&sessionId;
	args[2]=(uint32)ifName;
	args[3]=(uint32)&vid;
	re865xIoctl("eth0",RTL8651_IOCTL_ADDSESSION,(uint32)(args),(uint32)&ret,0);
	return ret;
}

int32 rtl8651_delSession(uint32 sessionId)
{
	int32 ret;
	re865xIoctl("eth0",RTL8651_IOCTL_DELSESSION,(uint32)(sessionId),(uint32)&ret,0);
	return ret;
}

int32 rtl8651_bindSession(ipaddr_t ipAddr, uint32 sessionId)
{
	int32  ret;		
	re865xIoctl("eth0",RTL8651_IOCTL_BINDSESSION,(uint32)(ipAddr),(uint32)sessionId,(uint32)&ret);
	return ret;
}

#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
int32 rtl8651_setPptpProperty(uint32 sessionId, uint16 callId, uint16 peerCallId, ipaddr_t s_ipaddr)
{
	int32  ret;
	uint32 args[4];
	args[0]=(uint32)&sessionId;
	args[1]=(uint32)&callId;
	args[2]=(uint32)&peerCallId;
	args[3]=(uint32)&s_ipaddr;
	re865xIoctl("eth0",RTL8651_IOCTL_SETPPTPPROPERTY,(uint32)(args),(uint32)&ret,0);
	return ret;
}

int32 rtl8651_setL2tpProperty(uint32 sessionId, uint16 tunnelId, uint16 sid, ipaddr_t s_ipaddr)
{
	int32  ret;
	uint32 args[4];
	args[0]=(uint32)&sessionId;
	args[1]=(uint32)&tunnelId;
	args[2]=(uint32)&sid;
	args[3]=(uint32)&s_ipaddr;
	re865xIoctl("eth0",RTL8651_IOCTL_SETL2TPPROPERTY,(uint32)(args),(uint32)&ret,0);
	return ret;
}

int32 rtl8651_resetPptpProperty(uint32 sessionId)
{
	int32  ret;	
	re865xIoctl("eth0",RTL8651_IOCTL_RESETPPTPPROPERTY,(uint32)(sessionId),(uint32)&ret,0);
	return ret;
}

int32 rtl8651_resetL2tpProperty(uint32 sessionId)
{
	int32  ret;	
	re865xIoctl("eth0",RTL8651_IOCTL_RESETL2TPPROPERTY,(uint32)(sessionId),(uint32)&ret,0);
	return ret;
}
#endif
int32 rtl8651_setLoopbackPortPHY(uint32 phy, int32 flag)
{
	int32  ret;
	re865xIoctl("eth0",RTL8651_IOCTL_SETLOOPBACKPORTPHY,(uint32)(phy),(int32)flag,(uint32)&ret);
	return ret;
}

int32 rtl8651_addNaptMapping(ipaddr_t extIpAddr)
{
	int32  ret;
	uint32 args[2];
	args[0]=(uint32)extIpAddr;
	re865xIoctl("eth0",RTL8651_IOCTL_ADDNAPTMAPPING,(uint32)(args),(uint32)&ret,0);
	return ret;
}

int32 rtl8651_delNaptMapping(ipaddr_t extIpAddr)
{
	int32  ret;
	uint32 args[2];
	args[0]=(uint32)extIpAddr;
	re865xIoctl("eth0",RTL8651_IOCTL_DELNAPTMAPPING,(uint32)(args),(uint32)&ret,0);
	return ret;
}

int32 rtl8651_addExtNetworkInterface(int8 * ifName)
{
	int32  ret;
	uint32 args[2];
	args[0]=(uint32)ifName;
	re865xIoctl("eth0",RTL8651_IOCTL_ADDEXTNETWORKINTERFACE,(uint32)(args),(uint32)&ret,0);
	return ret;
}

int32 rtl8651_delExtNetworkInterface(int8 * ifName)
{
	int32  ret;
	uint32 args[2];
	args[0]=(uint32)ifName;
	re865xIoctl("eth0",RTL8651_IOCTL_DELEXTNETWORKINTERFACE,(uint32)(args),(uint32)&ret,0);
	return ret;
}


int32 rtl8651_addRoute(ipaddr_t ipAddr, ipaddr_t ipMask, int8 * ifName, ipaddr_t nextHop)
{
	int32  ret;
	uint32 args[4];
	args[0]=(uint32)ipAddr;
	args[1]=(uint32)ipMask;
	args[2]=(uint32)ifName;
	args[3]=(uint32)nextHop;
	re865xIoctl("eth0",RTL8651_IOCTL_ADDROUTE,(uint32)(args),(uint32)&ret,0);
	return ret;
}

int32 rtl8651_delRoute(ipaddr_t ipAddr, ipaddr_t ipMask)
{
	int32  ret;
	uint32 args[2];
	args[0]=(uint32)ipAddr;
	args[1]=(uint32)ipMask;
	re865xIoctl("eth0",RTL8651_IOCTL_DELROUTE,(uint32)(args),(uint32)&ret,0);
	return ret;
}
int32 rtl8651_addPolicyRoute(rtl8651_tblDrvPolicyRoute_t *rt)
{
	int32  ret;
	re865xIoctl("eth0",RTL8651_IOCTL_ADDPOLICYROUTE,(uint32)(rt),(uint32)&ret,0);
	return ret;
}
int32 rtl8651_addDemandRoute(rtl8651_tblDrvDemandRoute_t *rt,int identity,int32(*p_callBack)(uint32) )
{
	int32  ret;
	re865xIoctl("eth0",RTL8651_IOCTL_ADDDEMANDROUTE,(uint32)(rt),identity,(uint32)&ret);
	return ret;
}


int32 rtl8651_flushDemandRoute(int identity)
{
	int32  ret;
	re865xIoctl("eth0",RTL8651_IOCTL_FLUSHDEMANDROUTE,identity,(uint32)&ret,0);
	return ret;
}

int32 rtl8651_flushPolicyRoute(uint32 ip)
{
	int32  ret;
	re865xIoctl("eth0",RTL8651_IOCTL_FLUSHPOLICYROUTE,ip,(uint32)&ret,0);
	return ret;
}


int32 rtl8651_addNatMapping(ipaddr_t extIpAddr, ipaddr_t intIpAddr)
{
	int32  ret;
	uint32 args[2];
	args[0]=(uint32)extIpAddr;
	args[1]=(uint32)intIpAddr;
	re865xIoctl("eth0",RTL8651_IOCTL_ADDNATMAPPING,(uint32)(args),(uint32)&ret,0);
	return ret;
}

int32 rtl8651_delNatMapping(ipaddr_t extIpAddr, ipaddr_t intIpAddr)
{
	int32  ret;
	uint32 args[2];
	args[0]=(uint32)extIpAddr;
	args[1]=(uint32)intIpAddr;
	re865xIoctl("eth0",RTL8651_IOCTL_DELNATMAPPING,(uint32)(args),(uint32)&ret,0);
	return ret;
}

int32 rtl8651_addIpUnnumbered(int8 *wanIfName, int8 *lanIfName, ipaddr_t netMask)
{
	int32  ret;
	uint32 args[3];
	args[0]=(uint32)wanIfName;
	args[1]=(uint32)lanIfName;
	args[2]=(uint32)netMask;
	re865xIoctl("eth0",RTL8651_IOCTL_ADDIPUNNUMBERED,(uint32)(args),(uint32)&ret,0);
	return ret;
} /* end rtl8651_addIpUnnumbered */

int32 rtl8651_delIpUnnumbered(int8 *wanIfName, int8 *lanIfName)
{
	int32  ret;
	uint32 args[2];
	args[0]=(uint32)wanIfName;
	args[1]=(uint32)lanIfName;
	re865xIoctl("eth0",RTL8651_IOCTL_DELIPUNNUMBERED,(uint32)(args),(uint32)&ret,0);
	return ret;
} /* end rtl8651_delIpUnnumbered */

int32 rtl8651_setLanSideExternalIpInterface(int8 * ifName, ipaddr_t ipAddr, ipaddr_t ipMask, int8 isExternal)
{
	int32  ret;
	uint32 args[4];
	args[0]=(uint32)ifName;
	args[1]=(uint32)ipAddr;
	args[2]=(uint32)ipMask;
	args[3]=(uint32)isExternal;
	re865xIoctl("eth0",RTL8651_IOCTL_SETLANSIDEEXTERNALIPINTERFACE,(uint32)(args),(uint32)&ret,0);
	return ret;	
} /* end rtl8651_setLanSideExternalIpInterface */

// dmz
int32 rtl8651a_addDmzHost(uint32 dsid,ipaddr_t naptIp, ipaddr_t dmzHostIp)
{
	uint32 ret;
	uint32 ptr[3];
	ptr[0]=(uint32)dsid;
	ptr[1]=(uint32)&naptIp;
	ptr[2]=(uint32)&dmzHostIp;
	re865xIoctl("eth0",RTL8651_IOCTL_ADDDMZHOST,(uint32)(ptr),0,(uint32)&ret);
	return ret;			
}

int32 rtl8651a_delDmzHost(uint32 dsid,ipaddr_t naptIp)
{
	uint32 ret;
	uint32 ptr[2];
	ptr[0]=(uint32)dsid;
	ptr[1]=(uint32)&naptIp;	
	re865xIoctl("eth0",RTL8651_IOCTL_DELDMZHOST,(uint32)(ptr),0,(uint32)&ret);
	return ret;			
}

int32 rtl8651_setDmzL4Fwd(uint32 enable)
{
	uint32 ret;
	uint32 ptr;
	ptr = enable;
	re865xIoctl("eth0", RTL8651_IOCTL_SETDMZHOSTL4FWD, (uint32)&ptr, 0, (uint32)&ret);
	return ret;
}

int32 rtl8651_setDmzIcmpFwd(uint32 enable)
{
	uint32 ret;
	uint32 ptr;
	ptr = enable;
	re865xIoctl("eth0", RTL8651_IOCTL_SETDMZHOSTICMPFWD, (uint32)&ptr, 0, (uint32)&ret);
	return ret;
}

// url filter
int32 rtl8651_delURLFilterRule(uint32 dsid, int8 *string, int32 strlen, uint32 ip_start, uint32 ip_end)
{
	uint32 ret;
	uint32 ptr[5];
	ptr[0]=(uint32)dsid;
	ptr[1]=(uint32)string;
	ptr[2]=(uint32)&strlen;
	ptr[3]=(uint32)&ip_start;
	ptr[4]=(uint32)&ip_end;
	re865xIoctl("eth0",RTL8651_IOCTL_DELURLFILTER,(uint32)(ptr),0,(uint32)&ret);
	return ret;
}


int32 rtl8651_addURLFilterRule(uint32 dsid, int8 *string, int32 strlen, uint32 ip_start, uint32 ip_end)
{
	uint32 ret;
	uint32 ptr[5];
	ptr[0]=(uint32)dsid;
	ptr[1]=(uint32)string;
	ptr[2]=(uint32)&strlen;
	ptr[3]=(uint32)&ip_start;
	ptr[4]=(uint32)&ip_end;
	re865xIoctl("eth0",RTL8651_IOCTL_ADDURLFILTER,(uint32)(ptr),0,(uint32)&ret);
	return ret;
}


int32 rtl8651a_delURLfilterString(uint32 dsid,int8 *string, int32 strlen)
{
	uint32 ret;
	uint32 ptr[3];
	ptr[0]=(uint32)dsid;
	ptr[1]=(uint32)string;
	ptr[2]=(uint32)&strlen;
	re865xIoctl("eth0",RTL8651_IOCTL_DELURLFILTERSTRING,(uint32)(ptr),0,(uint32)&ret);
	return ret;
}

int32 rtl8651a_addURLfilterString(uint32 dsid,int8 *string, int32 strlen)
{
	uint32 ret;
	uint32 ptr[3];
	ptr[0]=(uint32)dsid;
	ptr[1]=(uint32)string;
	ptr[2]=(uint32)&strlen;
	re865xIoctl("eth0",RTL8651_IOCTL_ADDURLFILTERSTRING,(uint32)(ptr),0,(uint32)&ret);
	return ret;	
}

int32 rtl8651_setPppoeSessionHangUp(uint32 PPPoEID, int32 enable, uint32 sec, int32 (*p_callBack)(uint32))
{
	int32  ret;
	uint32 args[4];
	args[0]=(uint32)PPPoEID;
	args[1]=(uint32)enable;
	args[2]=(uint32)sec;
	args[3]=(uint32)p_callBack;
	re865xIoctl("eth0",RTL8651_IOCTL_SETPPPOESESSIONHANGUP,(uint32)(args),(uint32)&ret,0);
	return ret;
}


void rtl8651a_setUdpSizeThreshValue(uint32 dsid, uint32 value)
{
	re865xIoctl("eth0", RTL8651_IOCTL_SETUDPSIZETHRESH,dsid,value,0);  
}


void rtl8651a_setWanStatus(uint32 dsid, uint32 Status)
{
	re865xIoctl("eth0", RTL8651_IOCTL_SETWANSTATUS,dsid,Status,0);  
}

void rtl8651a_setDosStatus(uint32 dsid,uint32 enableItem)
{
	re865xIoctl("eth0", RTL8651_IOCTL_SETDOSSTATUS,dsid,enableItem,0);
}

void rtl8651a_setDosThreshold(uint32 dsid,uint32 type,uint32 threshold)
{
	re865xIoctl("eth0", RTL8651_IOCTL_SETDOSTHRESHOLD,dsid,type,threshold);
}

void rtl8651a_setPerSrcDosThreshold(uint32 dsid, uint32 type, uint32 threshold)
{
	re865xIoctl("eth0", RTL8651_IOCTL_SETPERSRCDOSTHRESHOLD, dsid, type, threshold);
}

int32 rtl8651_dosScanTrackingSetPara(uint32 dsid, uint32 windowSize, uint32 threshold)
{
	uint32 args[3];
	int32 ret;
	args[0] = (uint32)&dsid;
	args[1] = (uint32)&windowSize;
	args[2] = (uint32)&threshold;
	re865xIoctl("eth0", RTL8651_IOCTL_SETDOSSCANTRACKINGPARA, (uint32)args, &ret, 0);
	return ret;
}

int32 rtl8651_addDosIgnoreType(uint32 ignoreType)
{
	uint32 args[2];
	uint32 ret;
	args[0] = (uint32)&ignoreType;
	re865xIoctl("eth0", RTL8651_IOCTL_ADDDOSIGNORETYPE, (uint32)args, &ret, 0);
	return ret;
}

int32 rtl8651_delDosIgnoreType(uint32 ignoreType)
{
	uint32 args[2];
	uint32 ret;
	args[0] = (uint32)&ignoreType;
	re865xIoctl("eth0", RTL8651_IOCTL_DELDOSIGNORETYPE, (uint32)args, &ret, 0);
	return ret;
}

uint32 rtl8651a_setAlgStatus(uint32 dsid,uint32 Status,ipaddr_t *serverip)
{
	uint32 args[3];
	uint32 ret;
	args[0]=(uint32)&dsid;
	args[1]=(uint32)&Status;
	args[2]=(uint32)serverip;
	re865xIoctl("eth0", RTL8651_IOCTL_SETALGSTATUS,(uint32)(args),&ret,0);
	return ret;
}


void rtl8651_setPppoeMtu(uint32 pppObjId,uint16 mtu)
{
	re865xIoctl("eth0", RTL8651_IOCTL_SETPPPOEMTU,pppObjId,(uint32)mtu,0);  
}

void rtl8651_setMtu(uint16 mtu)
{
	re865xIoctl("eth0", RTL8651_IOCTL_SETMTU,(uint32)mtu,0,0);  
}

int32 rtl8651_setNetMtu(char *iface,uint16 mtu)
{
	re865xIoctl("eth0", RTL8651_IOCTL_SETNETMTU,(uint32)iface,(uint32)mtu,0);
}


void rtl8651_setTimeZone(int32 timezone_diff)
{
	re865xIoctl("eth0", RTL8651_IOCTL_SETTIMEZONE,(int32)timezone_diff,0,0);  
}


// special application
void rtl8651_flushTriggerPortRules(void)
{
	re865xIoctl("eth0",RTL8651_IOCTL_FLUSHTRIGGERPORTRULES,NULL,NULL,0);
}

int rtl8651_addTriggerPortRule(uint32 dsid,uint8 inType,uint16 inPortStart,uint16 inPortFinish,uint8 outType,uint16 outPortStart,uint16 outPortFinish,uint32 address)
{
	int32  ret;
	uint32 args[7];
	args[0]=(uint32)&dsid;
	args[1]=(uint32)&inType;
	args[2]=(uint32)&inPortStart;
	args[3]=(uint32)&inPortFinish;
	args[4]=(uint32)&outType;
	args[5]=(uint32)&outPortStart;
	args[6]=(uint32)&outPortFinish;	
	args[7]=(uint32)&address;		
	re865xIoctl("eth0",RTL8651_IOCTL_ADDTRIGGERPORTRULE,(uint32)args,0,(uint32)&ret);
	return ret;
}

#if 0

void rtl8651_flushProtoStackUsedUdpPorts(void)
{
	re865xIoctl("eth0",RTL8651_IOCTL_FLUSHPROTOSTACKUSEDUDPPORTS,0,0,0);
}

int32 rtl8651_addProtoStackUsedUdpPort(uint16  s_port)
{
	int32  ret;
	uint32 args[1];
	args[0]=(uint32)&s_port;
	re865xIoctl("eth0",RTL8651_IOCTL_ADDPROTOSTACKUSEDUDPPORT,(uint32)args,0,(uint32)&ret);
	return ret;
}

void rtl8651_flushProtoStackServerUsedTcpPorts(void)
{
	re865xIoctl("eth0",RTL8651_IOCTL_FLUSHPROTOSTACKSERVERUSEDTCPPORTS,0,0,0);
}

int32 rtl8651_addProtoStackServerUsedTcpPort(uint16  s_port)
{
	int32  ret;
	uint32 args[1];
	args[0]=(uint32)&s_port;
	re865xIoctl("eth0",RTL8651_IOCTL_ADDPROTOSTACKSERVERUSEDTCPPORT,(uint32)args,0,(uint32)&ret);
	return ret;
}

#endif

int32 rtl8651_installLoggingFunction(void * pMyLoggingFunc)
{
	int32  ret;
	uint32 args[2];
	args[0]=(uint32)pMyLoggingFunc;
	//args[1]=(uint32)0;
	re865xIoctl("eth0",RTL8651_IOCTL_INSTALLLOGGINGFUNCTION,(uint32)(args),(uint32)&ret,0);
	return ret;
}

int32 rtl8651a_enableLogging(uint32 dsid,uint32 moduleId,int8 enable)
{
	int32  ret;
	uint32 args[3];
	args[0]=(uint32)dsid;
	args[1]=(uint32)moduleId;
	args[2]=(uint32)enable;
	re865xIoctl("eth0",RTL8651_IOCTL_ENABLELOGGING,(uint32)(args),(uint32)&ret,0);
	return ret;
}

int32 rtl8651_addNaptConnection
(
int16 assigned,
int8 flowType,
ipaddr_t insideLocalIpAddr,
uint16 insideLocalPort, 
ipaddr_t *insideGlobalIpAddr,
uint16 *insideGlobalPort,
ipaddr_t dstIpAddr,
uint16 dstPort
)
{
	int32  ret;
	uint32 args[8];
	args[0]=(uint32)assigned;
	args[1]=(uint32)flowType;
	args[2]=(uint32)insideLocalIpAddr;
	args[3]=(uint32)insideLocalPort;
	args[4]=(uint32)insideGlobalIpAddr;
	args[5]=(uint32)insideGlobalPort;
	args[6]=(uint32)dstIpAddr;
	args[7]=(uint32)dstPort;
	//args[1]=(uint32)0;
	re865xIoctl("eth0",RTL8651_IOCTL_ADDNAPTCONNECTION,(uint32)(args),(uint32)&ret,0);
	return ret;
}

int32 rtl8651_flushNaptServerPortbyExtIp(ipaddr_t extIp)
{
	int32 ret;
	re865xIoctl("eth0",RTL8651_IOCTL_FLUSHNAPTSERVERPORTBYEXTIP,(uint32)(extIp),(uint32)&ret,0);
	return ret;
}
int32 rtl8651_multicastSetUpStream(int8 *netName, uint32 para)
{
	int32 ret;
	int8	netIfName[RTL8651_IPINTF_NAME_LEN];
	uint32 args[2] = {&netIfName[0], para};

	if (strlen(netName) > RTL8651_IPINTF_NAME_LEN)
	{
		return FAILED;
	}

	bzero(netIfName, sizeof(netIfName));
	strncpy(netIfName, netName, strlen(netName));

	re865xIoctl("eth0",RTL8651_IOCTL_SETDEFAULTIGMPUPSTREAM, (uint32)(args), (uint32)&ret, 0);
	return ret;
}
int32 rtl8651_multicastAddFilter(ipaddr_t gip, uint32 action)
{
	int32 ret;
	uint32 args[2] = {(uint32)gip, action};

	re865xIoctl("eth0", RTL8651_IOCTL_ADDMULTICASTFILTER, (uint32)args, (uint32)&ret, 0);
	return ret;
}
int32 rtl8651_multicastDelFilter(ipaddr_t gip, uint32 action)
{
	int32 ret;
	uint32 args[2] = {(uint32)gip, action};

	re865xIoctl("eth0", RTL8651_IOCTL_DELMULITCASTFILTER, (uint32)args, (uint32)&ret, 0);
	return ret;
}int32 rtl8651_addNaptMappingExt(ipaddr_t extIp)
{
	int32 ret;
	re865xIoctl("eth0", RTL8651_IOCTL_ADDNAPTMAPPINGEXT, (uint32)extIp, (uint32)&ret, 0);
	return ret;
}

void rtl8651_flushProtoStackActions(void)
{	
	re865xIoctl("eth0", RTL8651_IOCTL_FLUSHPROTOSTACKACTIONS, 0, 0, 0);
}

int32 rtl8651_addProtoStackActions(ipaddr_t ip,ipaddr_t mask,uint8 action,uint8 role,uint8 protocol,uint16 orgPort,uint16 fwdPort)
{
	int32  ret;
	uint32 args[7];
	args[0]=(uint32)&ip;
	args[1]=(uint32)&mask;
	args[2]=(uint32)&action;
	args[3]=(uint32)&role;
	args[4]=(uint32)&protocol;
	args[5]=(uint32)&orgPort;
	args[6]=(uint32)&fwdPort;
	re865xIoctl("eth0",RTL8651_IOCTL_ADDPROTOSTACKACTIONS,(uint32)(args),(uint32)&ret,0);
	return ret;
}

// PPPoE WAN port Issue, 12/27/2004
int32 rtl8651_addProtoStackServerPortRange(ipaddr_t ip,ipaddr_t mask,uint8 protocol,uint16 PortStart,uint16 PortEnd)
{
	int32  ret;
	uint32 args[7];
	args[0]=(uint32)&ip;
	args[1]=(uint32)&mask;
	args[2]=(uint32)&protocol;
	args[3]=(uint32)&PortStart;
	args[4]=(uint32)&PortEnd;
	re865xIoctl("eth0",RTL8651_IOCTL_ADDPROTOSTACKSERVERPORTRANGE,(uint32)(args),(uint32)&ret,0);
	return ret;
}

/* DOS process */
void rtl8651_dosProc_blockSip_enable(uint32 dsid, uint32 enable)
{
	re865xIoctl("eth0", RTL8651_IOCTL_ENABLESOURCEIPBLOCK, dsid, enable, 0);
}
void rtl8651_dosProc_blockSip_setPrisonTime(uint32 prisonTime)
{
	re865xIoctl("eth0", RTL8651_IOCTL_SETSOUCEIPBLOCKTIMEOUT, prisonTime, 0, 0);
}

void rtl8651_dosProc_blockSip_freeAll(void)
{
	re865xIoctl("eth0", RTL8651_IOCTL_FREEBLOCKEDSOURCEIP, 0, 0, 0);
}

void rtl8651_flushPolicyBasedQoS(void)
{
	re865xIoctl("eth0", RTL8651_IOCTL_FLUSHABQOS, 0, 0, 0);
}

int32 rtl8651_addPolicyBasedQoS(int8 *ifName, rtl8651_tblDrvPolicy_t *policy_t, int8 highPriority)
{
	uint32 args[3];
	int32 ret;

	args[0] = (uint32)ifName;
	args[1] = (uint32)policy_t;
	args[2] = (uint32)(highPriority&0xff);
	re865xIoctl("eth0", RTL8651_IOCTL_ADDPOLICYBASEDQOS, (uint32)args, 0, (uint32)&ret);
	return ret;
}


int32 rtl8651_setFlowControl(uint32 port, int8 enable)
{
	uint32 args[2];
	int32 ret;

	args[0] = (uint32)port;
	args[1] = (uint32)(enable&0xff);
	re865xIoctl("eth0", RTL8651_IOCTL_SETFLOWCTRL, (uint32)args, 0, (uint32)&ret);
	return ret;
}


int32 rtl8651_setPortPriorityQueue(uint32 port, int8 highPriority)
{
	uint32 args[2];
	int32 ret;

	args[0] = (uint32)port;
	args[1] = (uint32)(highPriority&0xff);
	re865xIoctl("eth0", RTL8651_IOCTL_SETPRIORITYQUEUE, (uint32)args, 0, (uint32)&ret);
	return ret;
}


int32 rtl8651_setEthernetPortBandwidthControl(uint32 port, int8 input, uint32 rate)
{
	uint32 args[3];
	int32 ret;

	args[0] = (uint32)port;
	args[1] = (uint32)(input &0xff);
	args[2] = (uint32)rate;
	re865xIoctl("eth0", RTL8651_IOCTL_SETBANDWIDTHCTRL, (uint32)args, 0, (uint32)&ret);
	return ret;
}


int32 rtl8651_addRateLimitGroup(uint32 GroupId, int8 * ifName, uint32 rate, uint32 burst, int8 isByteCount)
{
	uint32 args[5];
	int32 ret;

	args[0] = GroupId;
	args[1] = (uint32)ifName;
	args[2] = rate;
	args[3] = burst;
	args[4] = (uint32)isByteCount;
	re865xIoctl("eth0", RTL8651_IOCTL_ADDRATELIMITGROUP, (uint32)args, 0, (uint32)&ret);
	return ret;
}


int32 rtl8651_addRateLimitRule(rtl8651_tblDrvAclRule_t * rule, uint32 GroupId)
{
	uint32 args[2];
	int32 ret;

	args[0] = (uint32)rule;
	args[1] = GroupId;
	re865xIoctl("eth0", RTL8651_IOCTL_ADDRATELIMIT, (uint32)args, 0, (uint32)&ret);
	return ret;
}


int32 rtl8651_flushRateLimitGroup(int8 * ifName)
{
	uint32 args[1];
	int32 ret;

	args[0] = (uint32)ifName;
	re865xIoctl("eth0", RTL8651_IOCTL_FLUSHRATELIMITGROUP, (uint32)args, 0, (uint32)&ret);
	return ret;
}



int32 rtl8651_addProtocolBasedNAT( uint8 protocol, ipaddr_t ExtIp, ipaddr_t IntIp )
{
	uint32 args[3];
	int32 ret;

	args[0] = (uint32)protocol;
	args[1] = (uint32)ExtIp;
	args[2] = (uint32)IntIp;
	re865xIoctl("eth0", RTL8651_IOCTL_ADDPBNAT, (uint32)args, 0, (uint32)&ret);
	return ret;
}


int32 rtl8651_delProtocolBasedNAT( uint8 protocol, ipaddr_t ExtIp, ipaddr_t IntIp )
{
	uint32 args[3];
	int32 ret;

	args[0] = (uint32)protocol;
	args[1] = (uint32)ExtIp;
	args[2] = (uint32)IntIp;
	re865xIoctl("eth0", RTL8651_IOCTL_DELPBNAT, (uint32)args, 0, (uint32)&ret);
	return ret;
}

void rtl8651_setsinglecolor5ledmode(void)
{	
	re865xIoctl("eth0", RTL8651_IOCTL_SINGLECOLOR5LEDMODE, 0, 0, 0);
}


int32 rtl8651_softfwd(int32 action)
{
	uint32 args[2];
	int32 ret;

	args[0] = (uint32)action;
	re865xIoctl("eth0", RTL8651_IOCTL_SOFTFWD, (uint32)args, 0, (uint32)&ret);
	return ret;
}


int32 rtl8651_ResetCounter(int32 counterIdx)
{
	uint32 args[2];
	int32 ret;

	args[0] = (uint32)counterIdx;
	re865xIoctl("eth0", RTL8651_IOCTL_RESETCONTER, (uint32)args, 0, (uint32)&ret);
	return ret;
}

int32 rtl8651_cfgCounter(int32 counterIdx, uint32 mbrMask)
{
	uint32 args[2];
	int32 ret;

	args[0] = (uint32)counterIdx;
	args[1] = (uint32)mbrMask;
	re865xIoctl("eth0", RTL8651_IOCTL_CFGCONTER, (uint32)args, 0, (uint32)&ret);
	return ret;
}

int32 rtl8651_checkEthernetPortStatus(uint32 port, int32 *up,int32 *speed, int32 *duplex, int32 *range){
	int32 args[3];
	int32 ret;
	args[0] = (uint32)port;
	re865xIoctl("eth0", RTL8651_IOCTL_GETLINKSTATUS, (uint32)args, 0, (uint32)&ret);
	*up=args[1]&0x4? 1:0;
	*speed=args[1]&0x2? 1:0;
	*duplex=args[1]&0x1?1:0;
	*range=args[2] ;
	return ret;
}

int32 rtl8651_getIntfCounters(uint32 net,uint32 *counters ){
	uint32 args[7];
	int32 ret;
	args[0] = (uint32)net;
	re865xIoctl("eth0", RTL8651_IOCTL_GETCOUNTER, (uint32)args, 0, (uint32)&ret);
	memcpy(counters, &args[1],sizeof(uint32)*6);
	return ret;
}


int32 rtl8651_setUpstreamBandwidth(int8 *ifName, uint32 bps)
{
	uint32 args[2];
	int32 ret;
	args[0] = ifName;
	args[1] = bps;
	re865xIoctl("eth0", RTL8651_IOCTL_SETMNQUEUEUPSTREAMBW, (uint32)args, 0, (uint32)&ret);
	return ret;
}


int32 rtl8651_addFlowToUnlimitedQueue(int8 *ifName, rtl8651_tblDrvAclRule_t *rule, uint32 ratio, uint32 queueID)
{
	uint32 args[4];
	int32 ret;
	args[0] = ifName;
	args[1] = (uint32)rule;
	args[2] = ratio;
	args[3] = queueID;
	re865xIoctl("eth0", RTL8651_IOCTL_ADDUNLIMITEDQUEUE, (uint32)args, 0, (uint32)&ret);
	return ret;
}

int32 rtl8651_addFlowToLimitedQueue(int8 *ifName, rtl8651_tblDrvAclRule_t *rule, uint32 ratio, uint32 queueID)
{
	uint32 args[4];
	int32 ret;
	args[0] = ifName;
	args[1] = (uint32)rule;
	args[2] = ratio;
	args[3] = queueID;
	re865xIoctl("eth0", RTL8651_IOCTL_ADDLIMITEDQUEUE, (uint32)args, 0, (uint32)&ret);
	return ret;
}

int32 rtl8651_setDscpToMNQueue(uint32 queueID, uint32 queueDSCP)
{
	uint32 args[2];
	int32 ret;
	args[0] = queueID;
	args[1] = queueDSCP;
	re865xIoctl("eth0", RTL8651_IOCTL_SETDSCPTOMNQUEUE, (uint32)args, 0, (uint32)&ret);
	return ret;
}


/*
	enable / disable "TCP Weak NAPT" mode
*/
int32 rtl8651_WeakNaptCtrl(int8 enable)
{
	int32  ret;
	uint32 args;
	args = (uint32)enable;
	re865xIoctl("eth0", RTL8651_IOCTL_WEAKNAPTCTL, (uint32)args, 0, (uint32)&ret);
	return ret;
} /* end rtl8651_WeakNaptCtrl */

int32 rtl8651_looseUdpCtrl(int8 enable)
{
	int32  ret;
	uint32 args;
	args = (uint32)enable;
	re865xIoctl("eth0", RTL8651_IOCTL_LOOSEUDPCTL, (uint32)args, 0, (uint32)&ret);
	return ret;
} /* end rtl8651_WeakNaptCtrl */

/*
	enable / disable TCP "NAPT port Static mapping" mode
*/
int32 rtl8651_TcpNaptStaticPortMappingCtrl(int8 enable)
{
	int32  ret;
	uint32 args;
	args = (uint32)enable;
	re865xIoctl("eth0", RTL8651_IOCTL_TCPNAPTSTATICPORTMAPPINGCTL, (uint32)args, 0, (uint32)&ret);
	return ret;
} /* end rtl8651_TcpNaptStaticPortMappingCtrl */

/*
	enable / disable UDP "NAPT port Static mapping" mode
*/
int32 rtl8651_UdpNaptStaticPortMappingCtrl(int8 enable)
{
	int32  ret;
	uint32 args;
	args = (uint32)enable;
	re865xIoctl("eth0", RTL8651_IOCTL_UDPNAPTSTATICPORTMAPPINGCTL, (uint32)args, 0, (uint32)&ret);
	return ret;
} /* end rtl8651_UdpNaptStaticPortMappingCtrl */

// Dino Chang add for MAC filter 2004/11/14
int32 rtl8651_enableMacAccessControl(int8 enable)
{
        int32  ret;
        uint32 args;
        args = (uint32)enable;
        re865xIoctl("eth0", RTL8651_ENABLEMACFILTER, (uint32)args, 0, (uint32)&ret);                                                                                    return ret;
} /* end rtl8651_enableMacAccessControl */
int32 rtl8651_setMacAccessControlDefaultPermit(int8 permit)
{
        int32  ret;
        uint32 args;
        args = (uint32)permit;
        re865xIoctl("eth0", RTL8651_SETMACFILTERDEFAULTPERMIT, (uint32)args, 0, (uint32)&ret);
        return ret;
} /* end rtl8651_setMacAccessControlDefaultPermit */

int32 rtl8651_flushMacAccessControl(void) {
        int32 ret;
        re865xIoctl("eth0", RTL8651_FLUSHMACFILTER, NULL, NULL, (uint32)&ret);
        return ret;
}

int32 rtl8651_addMacAccessControl(ether_addr_t *sMac, ether_addr_t *macMask, int8 isPermit) {
        int32 ret;
        uint32 args[3];
        args[0] = (uint32)sMac;
        args[1] = (uint32)macMask;
        args[2] = (uint32)isPermit;
              
        re865xIoctl("eth0", RTL8651_ADDMACFILTER, (uint32)args, 0, (uint32)&ret);
        return ret;
}

int32 rtl8651_delMacAccessControl(ether_addr_t *sMac, ether_addr_t *macMask) {
        int32 ret;
        uint32 args[2];
        args[0] = (uint32)sMac;
        args[1] = (uint32)macMask;
        re865xIoctl("eth0", RTL8651_DELMACFILTER, (uint32)args, 0, (uint32)&ret);
        return ret;
}
// Dino Chang

int32 rtl8651_addSubVlan(uint16 fvid, uint16 vid, uint32 portMask)
{
	int32 ret;
	uint32 args[3];

	args[0] = (uint32)fvid;
	args[1] = (uint32)vid;
	args[2] = (uint32)portMask;
	re865xIoctl("eth0", RTL8651_IOCTL_ADDSUBVLAN, (uint32)args, 0, (uint32)&ret);
	return ret;
}


int32 rtl8651_delSubVlan(uint16 vid)
{
	int32 ret;
	uint32 args;

	args = (uint32)vid;
	re865xIoctl("eth0", RTL8651_IOCTL_DELSUBVLAN, (uint32)args, 0, (uint32)&ret);
	return ret;
}



/*
	register napt-redirect rule
*/
int32 rtl8651_registerRedirectOutboundNaptFlow(
		uint8	isTcp,			/* TRUE: tcp, FALSE: udp */
		ipaddr_t	sip,				/* val 0 for wildcard */
		uint16	sport,			/* val 0 for wildcard */
		ipaddr_t	originalDip,		/* val 0 for wildcard */
		uint16	originalDport,	/* val 0 for wildcard */
		ipaddr_t	newDip,			/* val 0 for don't care, automatically assigned by gateway's LAN IP */
		uint16	newDport		/* must assign a new Dport to protocol stack */
)
{
	uint32 args[7];
	int32 ret;

	args[0] = (uint32)isTcp;
	args[1] = sip;
	args[2] = (uint32)sport;
	args[3] = originalDip;
	args[4] = (uint32)originalDport;
	args[5] = newDip;
	args[6] = (uint32)newDport;
	
	re865xIoctl("eth0", RTL8651_IOCTL_NAPTREDIRECT_REGISTER_RULE, (uint32)args, 0, (uint32)&ret);

	return ret;
}
/*
	unregister napt-redirect rule
*/
int32 rtl8651_unregisterRedirectOutboundNaptFlow(
		uint8	isTcp,			/* TRUE: tcp, FALSE: udp */
		ipaddr_t	sip,				/* val 0 for wildcard */
		uint16	sport,			/* val 0 for wildcard */
		ipaddr_t	originalDip,		/* val 0 for wildcard */
		uint16	originalDport,	/* val 0 for wildcard */
		ipaddr_t	newDip,			/* val 0 for wildcard */
		uint16	newDport		/* val 0 for wildcard */
)
{
	uint32 args[7];
	int32 ret;

	args[0] = (uint32)isTcp;
	args[1] = sip;
	args[2] = (uint32)sport;
	args[3] = originalDip;
	args[4] = (uint32)originalDport;
	args[5] = newDip;
	args[6] = (uint32)newDport;
	
	re865xIoctl("eth0", RTL8651_IOCTL_NAPTREDIRECT_UNREGISTER_RULE, (uint32)args, 0, (uint32)&ret);

	return ret;
}
/*
	query napt-redirect flow
*/
int32 rtl8651_queryRedirectOutboundNaptFlow(
		uint8	isTcp,
		ipaddr_t	sip,
		uint16	sport,
		ipaddr_t	*pOriginalDip,
		uint16	*pOriginalDport,
		ipaddr_t	newDip,
		uint16	newDport
)
{
	uint32 args[7];
	int32 ret;

	args[0] = (uint32)isTcp;
	args[1] = sip;
	args[2] = (uint32)sport;
	args[3] = pOriginalDip;
	args[4] = pOriginalDport;
	args[5] = newDip;
	args[6] = (uint32)newDport;
	
	re865xIoctl("eth0", RTL8651_IOCTL_NAPTREDIRECT_QUERY_FLOW, (uint32)args, 0, (uint32)&ret);

	return ret;
}

/*
	add multicast external port
*/
int32 rtl8651_addExternalMulticastPort(uint32 extMPort)
{
	int32 ret;
	uint32 args;

	args = (uint32)extMPort;
	re865xIoctl("eth0", RTL8651_IOCTL_ADDEXTMCASTPORT, (uint32)args, 0, (uint32)&ret);
	return ret;
}

/*
	delete multicast external port
*/
int32 rtl8651_delExternalMulticastPort(uint32 extMPort)
{
	int32 ret;
	uint32 args;

	args = (uint32)extMPort;
	re865xIoctl("eth0", RTL8651_IOCTL_DELEXTMCASTPORT, (uint32)args, 0, (uint32)&ret);
	return ret;
}


int8 rtl8651_EnablePppoePassthru(int8 enable)
{
	int8 ret;
	uint32 args;

	args = (uint32)enable;
	re865xIoctl("eth0", RTL8651_IOCTL_ENABLE_PPPOE_PASSTHRU, (uint32)args, 0, (uint32)&ret);
	return ret;
}


int8 rtl8651_enableDropUnknownPppoePADT(int8 enable)
{
	int8 ret;
	uint32 args;

	args = (uint32)enable;
	re865xIoctl("eth0", RTL8651_IOCTL_ENABLE_DROP_UNKNOWN_PPPOE_DROP, (uint32)args, 0, (uint32)&ret);
	return ret;
}


int8 rtl8651_EnableIpv6Passthru(int8 enable)
{
	int8 ret;
	uint32 args;

	args = (uint32)enable;
	re865xIoctl("eth0", RTL8651_IOCTL_ENABLE_IPV6_PASSTHRU, (uint32)args, 0, (uint32)&ret);
	return ret;
}

int8 rtl8651_EnableIpxPassthru(int8 enable)
{
	int8 ret;
	uint32 args;

	args = (uint32)enable;
	re865xIoctl("eth0", RTL8651_IOCTL_ENABLE_IPX_PASSTHRU, (uint32)args, 0, (uint32)&ret);
	return ret;
}

int8 rtl8651_EnableNetbiosPassthru(int8 enable)
{
	int8 ret;
	uint32 args;

	args = (uint32)enable;
	re865xIoctl("eth0", RTL8651_IOCTL_ENABLE_NETBIOS_PASSTHRU, (uint32)args, 0, (uint32)&ret);
	return ret;
}

int8 rtl8651_EnablIpMulticast(int8 enable)
{
	int8 ret;
	uint32 args;

	args = (uint32)enable;
	re865xIoctl("eth0", RTL8651_IOCTL_ENABLE_IPMULTICAST, (uint32)args, 0, (uint32)&ret);
	return ret;
}

uint32 rtl8651_setAlgQosQueueId(uint32 alg_idx,uint32 queueID,uint32 isHigh)
{
	int32 ret;
	uint32 args[3];

	args[0] = (uint32)&alg_idx;
	args[1] = (uint32)&queueID;
	args[2] = (uint32)&isHigh;

	re865xIoctl("eth0", RTL8651_IOCTL_RTL8651_SETALGQOSQUEUEID, (uint32)args, 0, (uint32)&ret);
	return ret;
}

int32 rtl8651_delRateLimitGroup(uint32 GroupId)
{
	int8 ret;
	uint32 args;
	args = (uint32)GroupId;
	re865xIoctl("eth0", RTL8651_IOCTL_DELRATELIMITGROUP, (uint32)args, 0, (uint32)&ret);
	return ret;
}

int32 rtl8651_getMNQueueEntry(rtl8651_tblDrvAclRule_t *rule,uint32 GroupId,uint32 index)
{
	int32 ret;
	uint32 args[3];

	args[0] = (uint32)rule;
	args[1] = (uint32)&GroupId;
	args[2] = (uint32)&index;

	re865xIoctl("eth0", RTL8651_IOCTL_GETMNQUEUEENTRY, (uint32)args, 0, (uint32)&ret);
	return ret;
}

int32 rtl8651_delRateLimitRule(rtl8651_tblDrvAclRule_t *rule, uint32 GroupId)
{
	int32 ret;
	uint32 args[2];

	args[0] = (uint32)rule;
	args[1] = (uint32)&GroupId;

	re865xIoctl("eth0", RTL8651_IOCTL_DELRATELIMITRULE, (uint32)args, 0, (uint32)&ret);
	return ret;
}

int32 rtl8651_delFlowFromMNQueue(rtl8651_tblDrvAclRule_t *rule, uint32 GroupId)
{
	int32 ret;
	uint32 args[2];

	args[0] = (uint32)rule;
	args[1] = (uint32)&GroupId;

	re865xIoctl("eth0", RTL8651_IOCTL_DELFLOWFROMMNQUEUE, (uint32)args, 0, (uint32)&ret);
	return ret;
}

int32 rtl8651_addArp(ipaddr_t ipAddr, ether_addr_t * macAddr, int8 * ifName, uint32 port) 
{
	int32 ret;
	uint32 args[4];
	args[0] = (uint32)&ipAddr;
	args[1] = (uint32)macAddr;
	args[2] = (uint32)ifName;
	args[3] = (uint32)&port;
	re865xIoctl("eth0", RTL8651_IOCTL_ADDARP, (uint32)args, 0, (uint32)&ret);
	return ret;
}
/* end tblDrv_mapping.c */
