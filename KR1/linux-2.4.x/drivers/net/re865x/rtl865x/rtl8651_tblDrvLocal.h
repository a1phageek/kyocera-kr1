/*
* Copyright c                  Realtek Semiconductor Corporation, 2002  
* All rights reserved.
* 
* Program : Switch core table driver local header rtl8651_tblDrvLocal.h
* Abstract : 
* Creator : Edward Jin-Ru Chen (jzchen@realtek.com.tw)
* Author :  
* $Id: rtl8651_tblDrvLocal.h,v 1.64 2005/03/29 15:08:25 chenyl Exp $

*
*/

#ifndef RTL8651_TBLDRV_LOCAL_H
#define RTL8651_TBLDRV_LOCAL_H


// Please follow the header file inclusion order below....

//first include common data types
#include "rtl_types.h"
#include "mbuf.h"
#include "rtl_queue.h"

//then include exported interface
#include "rtl8651_layer2.h"
#include "rtl8651_tblDrv.h"


//finally include internal data types and interfaces.
#include "rtl8651_aclLocal.h"
#include "rtl8651_layer2local.h"
#include "rtl8651_layer3local.h"
#include "rtl8651_layer4local.h"


#define RTL8651_TBLDRV_PRIVATE_DEFINITIONS
//---------------------------------------------------------------
//All other non-layer specific private datatypes, macros, and  #define 
//---------------------------------------------------------------

/* Collective Driver table structure*/

struct rtl8651_driverTables {
	
	/*Driver tables (with Valid bit)*/
	rtl8651_tblDrv_ethernet_t *phyPort;	/*Pjysical Port Table. was  rtl8651EthernetPortTable */
	rtl8651_tblDrv_linkAggregation_t *aggregator; /*Link Aggregator. was  rtl8651LinkAggregationTable */
	rtl8651_tblDrv_filterDbTable_t *filterDB; /*Filter database table.  was rtl8651FilterDbTable*/
	rtl8651_tblDrv_spanningTreeTable_t *spanningTree;  /*Spanning Tree Instance Table. wasrtl8651SpanningTreeTable*/

	rtl8651_tblDrv_vlanTable_t *vlan; 	/* rtl8651VlanTable*/
	rtl8651_tblDrv_networkIntfTable_t *netIface; /*rtl8651NetworkIntfTable*/
	rtl8651_tblDrv_routeTable_t *route; /* rtl8651RoutingTable*/
	rtl8651_tblDrv_ipEntry_t *iptbl; /* Internal External Table */
	
	rtl8651_tblDrv_naptIcmpFlowEntry_t **icmp; /*rtl8651NaptIcmpFlowTable*/
	rtl8651DrvNaptLine_t * tcpudp; /*rtl8651NaptTcpUdpFlowTable*/

	_rtl8651_tblDrvDynamicPolicy_t dynamicPolicy;
	rtl8651_tblDrv_pppTable_t *pppoe;
	rtl8651_tblDrv_pppTable_t *pptp;
	rtl8651_tblDrv_pppTable_t *l2tp;

	rtl8651_tblDrv_macContrl_t *macACInfo; /* MAC Access Control */
	rtl8651_tblDrv_rateLimit_t *rateLimit; /* Rate Limit Table */
	rtl8651_tblDrv_nextHopEntry_t *nhtbl; /* nexthop Table */
		
	
	/*Linked list for dynamic resources allocation. Should all be rewritten with standard macros in queue.h to avoid bugs */
	struct freeEntryList_s {

		SLIST_HEAD( _FreeFDBEntry, rtl8651_tblDrv_filterDbTableEntry_s) filterDBentry;
		CTAILQ_HEAD( _FreeAclEntry_t, _rtl8651_tblDrvAclRule_s) aclRule;
		CTAILQ_HEAD( _FreeMACAccessEntry, rtl8651_tblDrv_macACEntry_s) freeMacAccessControl;
		CSLIST_HEAD( _FreeArpEntry, rtl8651_tblDrv_arpEntry_s) arp;
		SLIST_HEAD( _FreeIpIfaceEntry, rtl8651_tblDrv_ipIntfEntry_s) ipIface; /* Linked through nextIp pointer */
		SLIST_HEAD( _FreeIpAddrEntry, rtl8651_tblDrv_ipIntfIpAddrEntry_s) ipAddr;
		SLIST_HEAD( _FreeNatEntry, rtl8651_tblDrv_natEntry_s) natMapping;

		SLIST_HEAD( _FreeAlgEntry, rtl8651_tblDrv_algEntry_s) alg;
		CSLIST_HEAD( _FreeAlgSlot, rtl8651_tblDrv_algSlot_t) asicAlgSlot;
		SLIST_HEAD( _FreeServerPortEntry, rtl8651_tblDrv_naptServerPortEntry_s) serverPort;
		rtl8651_tblDrv_naptIcmpFlowEntry_t *icmp; /* rtl8651FreeNaptIcmpFlowEntryList*/
		CTAILQ_HEAD( _FreeNaptEntry, rtl8651_tblDrv_naptTcpUdpFlowEntry_s) tcpudp;

		TAILQ_HEAD(_FreeSessionEntry, rtl8651_tblDrvSession_s) freeSession;
		/* policy */
		TAILQ_HEAD( _FreePolicyEntry, _rtl8651_tblDrvPolicy_s) freePolicy;
		TAILQ_HEAD( _FreeMultiCastEntry, rtl8651_tblDrv_mCast_s) freeMultiCast;

		SLIST_HEAD( _FreeProtocolBasedNat, rtl8651_tblDrv_protocolBasedNat_s) protocolBasedNat;
		SLIST_HEAD(_FreeNextHopEntry, rtl8651_tblDrv_nextHopEntry_s) nexthop;

		SLIST_HEAD(_FreeIpUnnumberEntry, rtl8651_tblDrv_ipUnnumbered_s) freeIpUnnumber;

		SLIST_HEAD(_FreeUrlPktFilterEntry, urlPktFiltering_s) freeUrlPktFilter;
	} freeList;

	struct inuseEntryList_s {
		SLIST_HEAD( _InuseAlgEntry, rtl8651_tblDrv_algEntry_s) alg;
		CTAILQ_HEAD( _InuseNaptEntry, rtl8651_tblDrv_naptTcpUdpFlowEntry_s) tcpudp;

		/* policy */
		TAILQ_HEAD(_InusePolicyEntry, _rtl8651_tblDrvPolicy_s) policyRoute;
		//TAILQ_HEAD(_InusePolicyQos, _rtl8651_tblDrvPolicy_s) policyQos;

		TAILQ_HEAD( _InuseMCast, rtl8651_tblDrv_mCast_s) *mCastTbl;//[RTL8651_MULTICASTTBL_SIZE];
		SLIST_HEAD( _InuseProtocolBasedNat, rtl8651_tblDrv_protocolBasedNat_s) protocolBasedNat;
		SLIST_HEAD(_InuseNextHopEntry, rtl8651_tblDrv_nextHopEntry_s) nexthop;

		SLIST_HEAD(_InuseIpUnnumberEntry, rtl8651_tblDrv_ipUnnumbered_s) ipUnnumber;

		SLIST_HEAD(_InuseUrlPktFilterEntry, urlPktFiltering_s) urlPktFilter;
	} inuseList;

};

struct AsicTableUsage_S{
	rtl8651_tblDrv_macAddressEntry_t * mac;
//	rtl8651_tblDrv_ipTableAsicArrangementEntry_t * ip;
	rtl8651_tblDrv_arpAsicArrangementEntry_t * arp;
	//rtl8651_tblDrv_routeAsicArrangementEntry_t * route;
	rtl8651_tblDrv_serverPortAsicArrangementEntry_t *serverPort;
	rtl8651_tblDrv_algAsicArrangementEntry_t *alg; 
//	rtl8651_tblDrv_aclAsicArrangementEntry_t *acl; 
	////rtl8651_tblDrvprotoTrapAsicArrangementEnty_t *protoTrap;
};

#define RTL8651_TBLDRV_PRIVATE_EXTERN_VARIABLES
//---------------------------------------------------------------
//Driver private extern variables.
//---------------------------------------------------------------

extern rtl8651_tblDrv_global_t	rtl8651GlobalControl;
extern void (*rtl8651_linkStatusNotifier)(uint32 port, int8 linkUp);
extern struct AsicTableUsage_S AsicTableUsage;
extern uint32 linkChangePendingCount;
extern int32 miiPhyAddress;
extern rtl8651_tblAsic_ethernet_t 	rtl8651AsicEthernetTable[9];//RTL8651_PORT_NUMBER+rtl8651_totalExtPortNum
extern rtl8651_tblDrvInitPara_t rtl8651_tblDrvPara;
extern uint32 arpAgingTime; /* default 5 mins */
extern struct rtl8651_driverTables DrvTbl;
extern uint32 *_loggingModule;
extern int8 pureSoftFwd;
extern void (* rtl8651_tblDrvFwdSend)(void * data);//void * assumed to be packet header
extern ether_addr_t cachedDA;
extern uint32 phyPortLinkStatus;
extern struct rtl8651_driverTables DrvTbl;
extern uint32 _sysUpSeconds; /* System uptime in seconds */
//extern rtl8651_tblDrvFwdEngineCounts_t rtl8651_fwdEngineCounter;
extern int32 (*p_mtuHandler)(struct rtl_pktHdr *);
extern int32 rtl8651_mss; 
extern uint16 pppoe_mss[];
#ifdef RTL865XB_URL_FILTER
extern int32 _urlFilterAclRuleNum;
#endif
extern uint32 rtl8651_externalMulticastPortMask;

/* spanning tree */
extern uint32 rtl8651_tblDrvSpanningTreeEnable;			/* enable/disable spanning tree */
extern uint32 rtl8651_tblDrvSpanningTreePortFwdStatus;	/* the port list at the forwarding state */

/* idle timeout */
extern rtl8651_softpppoeHangUp_t softpppoeHangUp[8];

#define RTL8651_TBLDRV_PRIVATE_MACROS
//---------------------------------------------------------------
//Driver private macros
//---------------------------------------------------------------


#define GET_NETIF_BY_VID(_netif, _vid)	(_netif) = DrvTbl.vlan[(_vid)&(RTL8651_VLAN_NUMBER-1)].netif_t

#define TBL_MEM_ALLOC(tbl, type, size)	 \
	{ \
	(tbl) = (type *)rtlglue_malloc((size) * sizeof(type)); \
		if(!(tbl)){\
			rtlglue_printf("MEM alloc failed at line %d\n", __LINE__);\
			return FAILED;\
		}\
	}

#define LOCK_ERROR_CHECK(condition, reason)\
	if (condition) {\
		rtlglue_drvMutexUnlock();\
		return reason;\
	}
#define UNLOCK_ERROR_CHECK(condition, reason)\
	if (condition)\
		return reason;

#define rtl8651_vlanTableIndex(vid)  (vid& (RTL8651_VLAN_NUMBER-1))




#define RTL8651_TBLDRV_PRIVATE_API_PROTOTYPE
//---------------------------------------------------------------
//Private API Prototype and related constant definition...
//---------------------------------------------------------------

//void rtl8651_installArpAgent(int32  (*arpAgent)(struct rtl_pktHdr *pkthdrPtr,struct rtl_mBuf *mbufPtr,uint16 vid));
int32 rtl8651_fwdEngineL2Input(struct rtl_pktHdr *pkthdrPtr,rtl8651_tblDrv_vlanTable_t *local_vlanp, uint8 *m_data, uint16 ethtype);
int32 _rtl8651_delVlanRefFilterDatabaseEntry(uint16 l2Type, uint16 fid, ether_addr_t * macAddr);
int8 * _strncpy(int8 *dst0, const int8 *src0, int32 count);
int32 _strncmp(const int8 *s1, const int8 *s2, int32 n);

//From rtl8651_tblDrv.c
rtl8651_tblDrv_arpEntry_t * _rtl8651_getArpEntry(rtl8651_tblDrv_networkIntfTable_t *netif_t, ipaddr_t ipAddr, int8 isRefresh);
rtl8651_tblDrv_networkIntfTable_t * _rtl8651_getNetworkIntfEntryByAddr(ipaddr_t ipAddr);
rtl8651_tblDrv_networkIntfTable_t * _rtl8651_getNetworkIntfEntryByVid(uint16 vid);
rtl8651_tblDrv_ipIntfEntry_t * _rtl8651_getIpIntfEntryByAddr(ipaddr_t ipAddr);
int32 _rtl8651_addLocalArp(int8 fromDrv, ipaddr_t ipAddr, ether_addr_t * macAddr, rtl8651_tblDrv_networkIntfTable_t *netIfPtr, uint32 port);
int8 _rtl8651_addLocalArpSpecifyNetworkInterface(int8 fromDrv, ipaddr_t ipAddr, ether_addr_t * macAddr, rtl8651_tblDrv_networkIntfTable_t * netIntfPtr, uint32 port);
int8 _rtl8651_getNetworkIntfLinkLayerType(int8 *ifName, uint32 *llType, uint32 *llIndex);
int8 _rtl8651_extIpAddr(ipaddr_t ipAddr, uint32 *netIdx);
int8 _rtl8651_extNetIpAddr(ipaddr_t ipAddr, uint32 *netIdx);
int8 _rtl8651_localServerIpAddr(ipaddr_t ipAddr);
int8 _rtl8651_intIpAddr(ipaddr_t ipAddr);
int8 _rtl8651_intNetIpAddr(ipaddr_t ipAddr);
int8 _rtl8651_isInternal(uint16 vid);
int32 _rtl8651_queryProtoTrap(uint8 protoType, uint16 protoContent);
int8 _rtl8651_getUsableExtIpAndPort(int8 , ipaddr_t , uint16 , ipaddr_t , uint16 , ipaddr_t *, uint16 *);
int8 _rtl8651_getIcmpUsableExtIpAndID(ipaddr_t sip, uint16 sID, ipaddr_t dip, ipaddr_t *gip, uint16 *gID);
int8 _rtl8651_getAllNatMapping(int32 *entry, rtl8651_tblDrv_natEntry_t *natMap);
int8 _rtl8651_getAllServerPort(int32 *entry, rtl8651_tblDrv_naptServerPortEntry_t *serverPort);
int8 _rtl8651_getUsableExtIpAndPort(int8 isTcp, ipaddr_t sip, uint16 sport, ipaddr_t dip, uint16 dport, ipaddr_t *extip, uint16 *extport);
int8 _rtl8651_getVIDByGWIpAddr(ipaddr_t gwIpAddr, uint32 *vid);
rtl8651_tblDrv_naptTcpUdpFlowEntry_t  * _rtl8651_addAlgCtrlConnection(int8 force, int8 flowType, ipaddr_t insideLocalIpAddr, uint16 insideLocalPort, 
		ipaddr_t *insideGlobalIpAddr, uint16 *insideGlobalPort, ipaddr_t dstIpAddr, uint16 dstPort);
int32 _rtl8651_addNaptConnection(int8 fromDrv, int16 assigned, int8 flowType, ipaddr_t insideLocalIpAddr, uint16 insideLocalPort, 
			ipaddr_t *insideGlobalIpAddr, uint16 *insideGlobalPort, ipaddr_t dstIpAddr, uint16 dstPort);
inline int32 _rtl8651_addAndGetNaptConnection(int8 fromDrv, int16 assigned, int8 flowType, ipaddr_t insideLocalIpAddr, uint16 insideLocalPort, 
			ipaddr_t *insideGlobalIpAddr, uint16 *insideGlobalPort, ipaddr_t dstIpAddr, uint16 dstPort,
			struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s **ppe );
inline int32 _rtl8651_delNaptConnection( rtl8651_tblDrv_naptTcpUdpFlowEntry_t *delConn);
int32 _rtl8651_removeNaptConnectionByIP( ipaddr_t ip );
int32 _rtl8651_countNaptConnectionByIP( ipaddr_t ip, uint32 *tcpCnt, uint32 *udpCnt );
rtl8651_tblDrv_naptTcpUdpFlowEntry_t * _rtl8651_findNaptConnectionByExtIpPort( int8 isTcp, ipaddr_t ExtIp, uint16 ExtPort,
                                        ipaddr_t remoteIp, uint16 remotePort );
rtl8651_tblDrv_naptTcpUdpFlowEntry_t * _rtl8651_findNaptConnection( uint32 tblIndex, int8 isTcp, ipaddr_t intIp, uint16 intPort,
                                                          ipaddr_t remoteIp, uint16 remotePort );
int8 _rtl8651_addNaptIcmpCandidateFlow(int8 fromDrv, ipaddr_t insideLocalIpAddr, uint16 insideLocalId, 
			ipaddr_t insideGlobalIpAddr, uint16 insideGlobalId, ipaddr_t dstIpAddr);
#if 0
	/* old */
	rtl8651_tblDrv_natEntry_t * _rtl8651_getNatExternalMappingEntry(ipaddr_t extIpAddr);
	rtl8651_tblDrv_natEntry_t * _rtl8651_getNatInternalMappingEntry(ipaddr_t intIpAddr);
#else
	/* new patched for mutiple/unnumber 2004/2/6 */
	rtl8651_tblDrv_natEntry_t * _rtl8651_getNatExternalMappingEntry(ipaddr_t extIpAddr,struct rtl_pktHdr *phdr);
	rtl8651_tblDrv_natEntry_t * _rtl8651_getNatInternalMappingEntry(ipaddr_t intIpAddr,struct rtl_pktHdr *phdr);
#endif

rtl8651_tblDrv_naptServerPortEntry_t *  _rtl8651_getOutsideNaptServerPortMapping(int8 isTcp, ipaddr_t extIpAddr, uint16 extPort);								   
rtl8651_tblDrv_naptServerPortEntry_t *  _rtl8651_getInsideNaptServerPortMapping(int8 isTcp, ipaddr_t intIpAddr, uint16 intPort);
rtl8651_tblDrv_algEntry_t *_rtl8651_getAlgEntry(int8 isTcp, int8 *isServer, int8 *isClient, uint16 sport, uint16 eport);
rtl8651_tblDrv_routeTable_t * _rtl8651_getRoutingEntry(ipaddr_t dstIpAddr);
rtl8651_tblDrv_routeTable_t * _rtl8651_getSoftRoutingEntry(uint32,uint32,uint8,int);
int32  _rtl8651_getOutboundNaptFlow(int8 isTcp, ipaddr_t sip, uint16 sport, ipaddr_t dip, uint16 dport, struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s **ppEntry);
int32  _rtl8651_getInboundNaptFlow(int8 isTcp, ipaddr_t gip, uint16 gport, ipaddr_t dip, uint16 dport,  struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s **ppEntry);
rtl8651_tblDrv_naptIcmpFlowEntry_t * _rtl8651_getInsideNaptIcmpEntry(ipaddr_t sip, uint16 sID, ipaddr_t dip, int8 isRefresh);
rtl8651_tblDrv_naptIcmpFlowEntry_t * _rtl8651_getOutsideNaptIcmpEntry(ipaddr_t gip, uint16 gID, ipaddr_t dip, int8 isRefresh);
void _rtl8651_collideNaptEntry(rtl8651DrvNaptLine_t *line,int32 *c1, int32 *c2);
_rtl8651_tblDrvAclRule_t * _rtl8651_aclLookUp(uint32 dsid,rtl8651_tblDrv_networkIntfTable_t *, int8 , uint32, int8 *, int8 *, int8 *);
int32 _rtl8651_getDefaultAcl(int8 *ifName, int8 *defAction);
rtl8651_tblDrv_pppTable_t *_rtl8651_getPppoeSessionByIpAddr(ipaddr_t ipaddr);
rtl8651_tblDrv_pppTable_t * _rtl8651_getPppoeSessionByPppoeSessionId(uint16 pppoeSessionId);
int32 _rtl8651_getDmzHost(uint32 dsid,ipaddr_t naptIp, ipaddr_t *dmzHostIp);
int32 _rtl8651_fromDmzHost(uint32 dsid, ipaddr_t dmzHostIp, ipaddr_t *naptIp);
int32 _rtl8651_isLocalBcastAddr(uint32 vid, ipaddr_t ipAddr);
int32 _rtl8651_enableDoS(uint32 dostype, int8 enable, uint32 vlanMask);
uint32 _rtl8651_SessionIDToDSID(uint32 SessionID);
int32 _rtl8651_urlFilter(uint32  dsid,struct rtl_pktHdr * phdr,struct ip * pip,uint8 dir, int8 * string,int32 * strlen);
 void _rtl8651_refillOrClearAsicNaptEntry(uint32 tblIdx,struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s * selectedRefillFlow);
 int32 _rtl8651_addVlanRefFilterDatabaseEntry(uint16 l2Type, uint16 fid, uint16 vid, ether_addr_t * macAddr, uint32 type, uint32 portMask);
 int32 _rtl8651_modifyVlanRefFilterDatabaseEntry(uint32 fid, ether_addr_t *mac, uint32 *memberPortMask, uint16 *process, uint16 *linkId, uint32 attr);
int32 _rtl8651_addVlanRefFilterDatabaseEntry(uint16 l2Type, uint16 fid, uint16 vid, ether_addr_t * macAddr, uint32 type, uint32 portMask) ;
 rtl8651_tblDrv_filterDbTableEntry_t * _rtl8651_getVlanFilterDatabaseEntry(uint16 vid, ether_addr_t * macAddr);
void _rtl8651_removeArpAndNaptFlowWhenLinkDown(int32 port, int32 *linkId) ;
void _rtl8651_removeArpByL2Entry(rtl8651_tblDrv_filterDbTableEntry_t *l2entry_t);


int32 _rtl8651_delVlanRefFilterDatabaseEntry(uint16 l2Type, uint16 fid, ether_addr_t * macAddr);

rtl8651_tblDrv_algEntry_t*_rtl8651_l4FindLinkAlg(uint16 , uint16 );


void _rtl8651_addUpnpMap(uint32, ipaddr_t , uint16 ,  ipaddr_t , uint16 , ipaddr_t , uint16 );
void _rtl8651_delUpnpMap(uint32 , ipaddr_t , uint16 ,  ipaddr_t , uint16 , ipaddr_t , uint16 );
void _rtl8651_findUpnpMap(uint32, ipaddr_t , uint16 ,  ipaddr_t , uint16 , ipaddr_t *, uint16 *,uint32 *,uint32 *);
int32 _rtl8651_checkInusedUpnpExtIpPort(uint32, ipaddr_t, uint32);

int32 _rtl8651_updateNaptConnection(struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s *, int8 , uint32 );
int32 _rtl8651_getOutboundAlgCtrlFlow(int8 , ipaddr_t , uint16 , ipaddr_t , uint16 ,  struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s **); 

int32 _rtl8651_getPppoeTblPtr(uint32 pppoeId,rtl8651_tblDrv_pppTable_t ** pppoePtr_P);
 void _rtl8651_naptTimerAdaptiveAdjust(int8 enabled, uint32 threshold);
void _rtl8651_naptSmartFlowSwapByPPS(int8 enabled, uint32 threshold);
int32 _rtl8651_isMacAccessControlPermit(ether_addr_t *sMac);

/* ip unnumber */
int32 _rtl8651_setLanSideExternalIpInterface(rtl8651_tblDrv_networkIntfTable_t *netif_t, ipaddr_t ipAddr, ipaddr_t ipMask, int8 isExternal);
void _rtl8651_reArrangeLanSideExternalIpInterfacePatch(void);

/* multicast */
void _rtl8651_arrangeMulticastPortStatus(void);

/* Type of Dos Definition */
#define RTL8651_TCP_SYNFLOOD			0x00000001
#define RTL8651_ICMP_FLOOD				0x00000002
#define RTL8651_UDP_FLOOD				0x00000004
#define RTL8651_IP_SPOOFING			0x00000008
/*#define RTL8651_TCP_SYNWITHDATA		0x00000010 */
/* #define RTL8651_TCP_LANDATTACK		0x00000020 */
/* #define RTL8651_ICMP_SMURF				0x00000040 */
#define RTL8651_UDP_ECHOCHARGENBOMB	0x00000080
#define RTL8651_TCP_FULLXMASSCAN		0x00000100
#define RTL8651_TCP_FULLSAPUSCAN		0x00000200
#define RTL8651_TCP_FINSCAN			0x00000400
#define RTL8651_TCP_SYNFINSCAN		0x00000800
#define RTL8651_TCP_NULLSCAN			0x00001000
#define RTL8651_TCP_NMAPXMASSCAN		0x00002000
#define RTL8651_TCP_VECNASCAN			0x00004000
#define RTL8651_TCP_STEALTHFIN			0x00008000
#define RTL8651_TCPUDP_PORTSCAN		0x00010000

#if 0
/* for forwarding engien, to distinguish fast path,slow path */
#define RTL8651_FLOWTYPE_FAST           0x00000001
#define RTL8651_FLOWTYPE_SLOW           0x00000002
#endif



/* Switch for Features */
#define RTL865X_PSEUDO_VLAN

/* For Netmeeting ALG */
int32 _rtl8651_removeNaptConnectionByIntIPandPort( int8 isTcp, ipaddr_t intIp, uint16 intPort );

/* Timer1 used */
extern uint32 tuRemind;
extern uint32 accJiffies;
extern uint32 timer1Sec;


/* for test code */
#ifdef RTL865X_TEST
int32 rtl8651_getDrvMutex(void);
int32 rtl8651_reinitDrvMutex(void);
int32 rtl8651_getMbufMutex(void);
int32 rtl8651_reinitMbufMutex(void);
#endif /* RTL865X_TEST */



/* ACL DB */
typedef int32 (*acldb_callback_t) (int32 , struct rtl_pktHdr *, struct ip *, void *);

struct rtl8651_ACLDB_s
{
	acldb_callback_t func;
	void *userDefined;
	uint32 count;
};
typedef struct rtl8651_ACLDB_s rtl8651_ACLDB_t;

extern rtl8651_ACLDB_t acldb[];

int32 _rtl8651_resetACLDB(void);
int32 _rtl8651_registerACLDB( int32 ruleNo, 
                              acldb_callback_t func,
                              void *userDefined );
int32 _rtl8651_jumptoACLDB( int32 ruleNo, struct rtl_pktHdr *pkthdr, struct ip *pip );
#define RTL865XB_ACLDB


/* rate limit counter */
int32 _rtl8651_addRateLimitCounter(rtl8651_tblDrv_networkIntfTable_t *netif_t, uint8 counterType);
int32 _rtl8651_delRateLimitCounter(rtl8651_tblDrv_networkIntfTable_t *netif_t, uint8 counterType);
int32 _rtl8651_getRateLimitCounter(rtl8651_tblDrv_networkIntfTable_t *netif_t, uint8 counterType, uint8 refill);
int32 _rtl8651_addRateLimitCounterRule(rtl8651_tblDrvAclRule_t *rule, rtl8651_tblDrv_networkIntfTable_t *netif_t, uint8 counterType);
int32 _rtl8651_delRateLimitCounterRule(rtl8651_tblDrvAclRule_t *rule, rtl8651_tblDrv_networkIntfTable_t *netif_t, uint8 counterType);
void _rtl8651_rateLimitCounterTimeUpdate(uint32 secpassed);
void _rtl8651_flushRateLimitCounter(void);


#endif /* RTL8651_TBLDRV_LOCAL_H */


