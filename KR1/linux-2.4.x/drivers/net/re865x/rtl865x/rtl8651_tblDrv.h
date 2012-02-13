/*
* Copyright c                  Realtek Semiconductor Corporation, 2002  
* All rights reserved.
* 
* Program : Switch core table driver rtl8651_tblDrv.c
* Abstract : 
* Author : Edward Jin-Ru Chen (jzchen@realtek.com.tw)               
* $Id: rtl8651_tblDrv.h,v 1.85 2005/03/29 15:08:25 chenyl Exp $
*
*/

#ifndef RTL8651_TBLDRV_H
#define RTL8651_TBLDRV_H

#include "types.h"

#include "rtl8651_layer2.h"
#include "rtl_errno.h"
#ifdef __KERNEL__
#include "rtl8651_tblDrvProto.h"
#include "mbuf.h"
#endif

#define RTL8651_IPINTF_NAME_LEN		 16
#define RTL8651_MAX_DIALSESSION      2
#define RTL8651_DSID_ALL			RTL8651_MAX_DIALSESSION
#define RTL8651_DSID_SUPPORT	     1

/***********************************************
 * To roll-back to old-fashion (Louis), use:
 * #define LEGACY_AGING_CODE
 * #define EVENT_TRIGGER_TIMEUPDATE 0
 * #undef PERIOD_SYNC_AGE
 *
 * To use new-fashion (cfliu):
 * #undef LEGACY_AGING_CODE
 * #define EVENT_TRIGGER_TIMEUPDATE 1
 * #undef PERIOD_SYNC_AGE
 *
 * To use new-fashion (Louis):
 * #undef LEGACY_AGING_CODE
 * #define EVENT_TRIGGER_TIMEUPDATE 1
 * #define PERIOD_SYNC_AGE
 ***********************************************/
/*
 * If defined LEGACY_AGING_CODE, ROME driver scans 1024 ASIC entries every second.
 * If not defined, ROME driver only scans inused ASIC entries for better performance.
 */
#undef LEGACY_AGING_CODE

/* EVENT_TRIGGER_TIMEUPDATE
 * If defined 0, ROME driver executes timeupdate-related functions every second by timer interrupt.
 * If defined 1, ROME driver refreshes timer1 only when packet arrives.
 *    When 1 second passed, timeupdate-related functions will be executed.
 * If defined 2, ROME driver provides more precise timer1 timing than 1.
 */
#define EVENT_TRIGGER_TIMEUPDATE 2

/*************************************************/
/*
 * NEVER_REFILL_TIMER1 is defined when EVENT_TRIGGER_TIMEUPDATE==2.
 * If not defined, TC1CNT will be refilled every time rtl8651_timeUpdate() executes.
 * If defined, Timer1 will be configured to timer mode(not counter mode), and TC1CNT will be refilled by ASIC.
 */
#define NEVER_REFILL_TIMER1


/*
 * If defined PERIOD_SYNC_AGE, ROME driver syncs age of software and ASIC periodically.
 * If not defined, ROMe driver syncs when every packet arrives.
 */
#define PERIOD_SYNC_AGE

   
/*************************************************/
/*
 * If defined CACHE_FIND_NAPT_RESULT, ROME driver will caches the result of getOutboundNaptEntry.
 * If not defined, ROME driver do not cache.
 */
#undef CACHE_FIND_NAPT_RESULT


/*************************************************/
/*
 * If defined FLUSH_NAPT_OLDEST_CONNECTION2,
 *    _rtl8651_softwareNaptAging() will record the connection with minimum age value.
 *    So that we can delete connection immediately when addNaptConnection() overflow.
 * If not defined, ROME driver will delete a connection that created first (FIFO).
 */
#define FLUSH_NAPT_OLDEST_CONNECTION2


/*************************************************/
/*
 * ALLOW_SP is defined for _rtl8651_findNaptConnection() and _rtl8651_findNaptConnectionByExtIpPort().
 * This is a bitmask for isTcp parameter.
 * Defined non-zero can enable server port functionality of NAPT.
 * Defined zero can roll-back to original code.
 */
#define ALLOW_SP (1<<1)


/*************************************************/
/*
 * SERVERPORT_PARK_TCPUDP is defined for ASIC Server Port Table.
 * In ASIC design, Server Port Table will forward packets no matter TCP or UDP.
 * However, it will cause error in test tools, such as CDRouter.
 * If defined SERVERPORT_PARK_TCPUDP, tblDrv uses ASIC Server Port Table only when both TCP and UDP connections are added.
 *    Otherwise, fwdEng uses HASH2 to handle TCP/UDP connections.
 * If not defined, fwdEng uses ASIC Server Port Table to accelerate even only TCP or UDP connection is added.
 */
#define SERVERPORT_PARK_TCPUDP


/*************************************************/
/*
 * ADD_FINITE_HASH2_SP is defined for HASH2 that simulates Server Port Function.
 * We can use HASH2 entry just like Server Port do.
 * If defined ADD_FINITE_HASH2_SP, addNaptConnection() adds the 5-tuple connection with finite age (will be deleted).
 * If not defined, addNaptConnection() adds the 5-tuple connection that will never be aged out.
 */
#define ADD_FINITE_HASH2_SP


/*************************************************/
/*
 * PPPOE_PASSTHRU_NEW_MODEL is defined for PPPoE Passthru.
 * If defined 0, the original model is compiled (complex model).
 * If defined 1, new model is forwarding packet according to PPPoE Passthru VLAN.
 */
#define PPPOE_PASSTHRU_NEW_MODEL 1


/*************************************************/
/*
 * TRAP_TCP_SYN_FIN_RST is defined to trap TCP SYN/FIN/RST packets.
 * If not defined, the original model is only trapping SYN packet at WAN side.
 * If defined, new model is trapping SYN/FIN/RST packets at both WAN/LAN side.
 */
#undef TRAP_TCP_SYN_FIN_RST


/*************************************************/
/*
 * SUPPORT_HASH3 is defined to support HASH3.
 * If not defined, we dont support HASH3.
 * If defined, we support HASH3.
 */
#ifndef RTL865X_TEST
#undef SUPPORT_HASH3
#endif


typedef struct rtl8651_tblDrvInitPara_s {
	uint32	macAddressDbSize;
	uint32	filterDbSize;
	uint32	networkIntfTableSize;
	uint32	ipIntfEntryNumber;
	uint32	ipIntfIpAddrNumber;
	uint32	arpTableSize;
	uint32	routingTableSize;
	uint32	natSize;
	uint32	natCtlSize;
	uint32	naptServerPortEntryNumber;
	uint32	naptTcpUdpFlowSize;
	uint32	naptIcmpFlowSize;
	uint32	aclSize;
	uint32	algSize;
	uint32 	urlfilterDBSize;
	uint32 	urlfilterStringLen;
	uint32	urlPktfilterDBSize;	/* for url filter extension: filter for IP/MAC....etc */
	uint32     dialSessionNumber; /* supported number of dial sessions. valid dsid: 0,1,...,(dialSessionNumber-1) */
	uint32     dmzHostsNum;
	uint32	policyRules;
	uint32	dynamicPolicy;
	uint32	pptpSize;
	uint32	l2tpSize;
	uint32	pppoePassthruSize;
	uint32	protocolBasedNatSize;
	uint32	mCastTblSize;
	uint32	macAccessControllSize;
#if EVENT_TRIGGER_TIMEUPDATE==1
	#define RTL8651_TICKS_PER_SECOND    100  /*default assume host OS's timer ticks every 100ms. User can define 'ticksPerSecond' field of rtl8651_fwdEngineInitPara_t when init system. Must recompile if this define changed */
	uint32 ticksPerSecond;
#endif
	uint32	ipUnnumberSize;
	
} rtl8651_tblDrvInitPara_t;


typedef struct ___rtl8651_tblDrvPolicyRoute_t
{
	int type;
	union
	{
		uint32 ip_s;
		uint32 ip_d;
		struct {
			uint16 start ;
			uint16 end;
		}dport;
	}trigger;
	uint32 ip_start;
	uint32 ip_end;
	uint32 ip_alias;

} rtl8651_tblDrvPolicyRoute_t;
typedef struct __rtl8651_tblDrvPolicyRoute_t
{
	int type;
	union
	{
		uint32 ip_s;
		uint32 ip_d;
		struct {
			uint16 start ;
			uint16	end;
		}dport;
	}trigger;
	uint32 ip_start;
	uint32 ip_end;
	uint32 ip_alias;
	void	 *rt;
	struct __rtl8651_tblDrvPolicyRoute_t	*timenext;
	struct __rtl8651_tblDrvPolicyRoute_t	*timeprev;
	struct __rtl8651_tblDrvPolicyRoute_t	*linknext;
	struct __rtl8651_tblDrvPolicyRoute_t	*linkprev;
} _rtl8651_tblDrvPolicyRoute_t;
#if 0 // chenyl
typedef struct _rtl8651_conntrack_info
{
	int ip_s;
	int ip_d;
	int *ip_alias;
	uint16 *ip_aliasp;
	short icmpid;
	
}rtl8651_conntrack_info;
typedef struct __rtl8651_tblDrvSoftIcmpTable_t 
{
	int 	age;
	int     globalIp;
	int     internalIp;
	uint16	internalId;
	uint16  globalId;
}_rtl8651_tblDrvSoftIcmpTable_t;
#endif
typedef struct __rtl8651_tblDrvDemandRoute_t
{
	int type;
	union
	{
		int ip_s;
		int ip_d;
		struct {
			uint16 start ;
			uint16	end;
		}dport;
	}trigger;
	uint32 ip_start;
	uint32 ip_end;
	int identity;
	int32 (*p_handler)(uint32); 				/* call out handler */
} rtl8651_tblDrvDemandRoute_t;





typedef struct rtl8651_tblDrvPolicy_s {
	uint32 		proto;					
	ipaddr_t	sip, sipMask,			
				dip, dipMask;
	uint16		sport_start, sport_end,
				dport_start, dport_end;
	ipaddr_t	naptip;
//	uint32		sessionId;
} rtl8651_tblDrvPolicy_t;

/* proto: */
#define RTL8651_POLICY_IP		0x01
#define RTL8651_POLICY_TCP		0x02
#define RTL8651_POLICY_UDP		0x03










typedef struct rtl8651_tblDrvAclRule_s {
	union {
		/* MAC ACL rule */
		struct {
			ether_addr_t _dstMac, _dstMacMask;
			ether_addr_t _srcMac, _srcMacMask;
			uint16 _typeLen, _typeLenMask;
		} MAC; 
		/* IFSEL ACL rule */
		struct {
			uint8 _gidxSel;
		} IFSEL; 
		/* IP Group ACL rule */
		struct {
			ipaddr_t _srcIpAddr, _srcIpAddrMask;
			ipaddr_t _dstIpAddr, _dstIpAddrMask;
			uint8 _tos, _tosMask;
			union {
				/* IP ACL rle */
				struct {
					uint8 _proto, _protoMask, _flagMask;// flag & flagMask only last 3-bit is meaning ful
#if 1 
//chhuang: #ifdef RTL8650B
					uint32 _FOP:1, _FOM:1, _httpFilter:1, _httpFilterM:1, _identSrcDstIp:1, _identSrcDstIpM:1;
#endif /* RTL8650B */
					union {
						uint8 _flag;
						struct {
							uint8 pend1:5,
								 pend2:1,
								 _DF:1,	//don't fragment flag
								 _MF:1;	//more fragments flag
						} s;
					} un;							
				} ip; 
				/* ICMP ACL rule */
				struct {
					uint8 _type, _typeMask, _code, _codeMask;
				} icmp; 
				/* IGMP ACL rule */
				struct {
					uint8 _type, _typeMask;
				} igmp; 
				/* TCP ACL rule */
				struct {
					ether_addr_t _l2srcMac, _l2srcMacMask;
					uint8 _flagMask;
					uint16 _srcPortUpperBound, _srcPortLowerBound;
					uint16 _dstPortUpperBound, _dstPortLowerBound;					
					union {
						uint8 _flag;
						struct {
							uint8 _pend:2,
								  _urg:1, //urgent bit
								  _ack:1, //ack bit
								  _psh:1, //push bit
								  _rst:1, //reset bit
								  _syn:1, //sync bit
								  _fin:1; //fin bit
						}s;
					}un;					
				}tcp; 
				/* UDP ACL rule */
				struct {					
					ether_addr_t _l2srcMac, _l2srcMacMask;
					uint16 _srcPortUpperBound, _srcPortLowerBound;
					uint16 _dstPortUpperBound, _dstPortLowerBound;					
				}udp; 
			}is;
		}L3L4; 
#if 1 
//chhuang :#ifdef RTL8650B
		/* Source filter ACL rule */
		struct {
			ether_addr_t _srcMac, _srcMacMask;
			uint16 _srcPort, _srcPortMask;
			uint16 _srcVlanIdx, _srcVlanIdxMask;
			ipaddr_t _srcIpAddr, _srcIpAddrMask;
			uint16 _srcPortUpperBound, _srcPortLowerBound;
			uint32 _ignoreL3L4:1, //L2 rule
				  	 _ignoreL4:1; //L3 rule
		} SRCFILTER;
		/* Destination filter ACL rule */
		struct {
			ether_addr_t _dstMac, _dstMacMask;
			uint16 _vlanIdx, _vlanIdxMask;
			ipaddr_t _dstIpAddr, _dstIpAddrMask;
			uint16 _dstPortUpperBound, _dstPortLowerBound;
			uint32 _ignoreL3L4:1, //L2 rule
				  	 _ignoreL4:1; //L3 rule
		} DSTFILTER;
#endif /* RTL8650B */
	}un_ty;

	uint32	ruleType_:4;
	uint32	actionType_:4;
	uint32	matchType_;
} rtl8651_tblDrvAclRule_t;

/* MAC ACL rule Definition */
#define dstMac_				un_ty.MAC._dstMac
#define dstMacMask_			un_ty.MAC._dstMacMask
#define srcMac_				un_ty.MAC._srcMac
#define srcMacMask_			un_ty.MAC._srcMacMask
#define typeLen_				un_ty.MAC._typeLen
#define typeLenMask_			un_ty.MAC._typeLenMask

/* IFSEL ACL rule Definition */
#define gidxSel_				un_ty.IFSEL._gidxSel

/* Common IP ACL Rule Definition */
#define srcIpAddr_				un_ty.L3L4._srcIpAddr
#define srcIpAddrMask_			un_ty.L3L4._srcIpAddrMask
#define dstIpAddr_				un_ty.L3L4._dstIpAddr
#define dstIpAddrMask_			un_ty.L3L4._dstIpAddrMask
#define tos_					un_ty.L3L4._tos
#define tosMask_				un_ty.L3L4._tosMask
/* IP Rrange */
#define srcIpAddrStart_			un_ty.L3L4._srcIpAddr
#define srcIpAddrEnd_			un_ty.L3L4._srcIpAddrMask
#define dstIpAddrStart_			un_ty.L3L4._dstIpAddr
#define dstIpAddrEnd_			un_ty.L3L4._dstIpAddrMask

/* IP ACL Rule Definition */
#define ipProto_				un_ty.L3L4.is.ip._proto
#define ipProtoMask_			un_ty.L3L4.is.ip._protoMask
#define ipFlagMask_			un_ty.L3L4.is.ip._flagMask
#if 1 //chhuang: #ifdef RTL8650B
#define ipFOP_      				un_ty.L3L4.is.ip._FOP
#define ipFOM_      				un_ty.L3L4.is.ip._FOM
#define ipHttpFilter_      			un_ty.L3L4.is.ip._httpFilter
#define ipHttpFilterM_			un_ty.L3L4.is.ip._httpFilterM
#define ipIdentSrcDstIp_   		un_ty.L3L4.is.ip._identSrcDstIp
#define ipIdentSrcDstIpM_		un_ty.L3L4.is.ip._identSrcDstIpM
#endif /* RTL8650B */
#define ipFlag_				un_ty.L3L4.is.ip.un._flag
#define ipDF_					un_ty.L3L4.is.ip.un.s._DF
#define ipMF_					un_ty.L3L4.is.ip.un.s._MF

/* ICMP ACL Rule Definition */
#define icmpType_				un_ty.L3L4.is.icmp._type
#define icmpTypeMask_			un_ty.L3L4.is.icmp._typeMask	
#define icmpCode_				un_ty.L3L4.is.icmp._code
#define icmpCodeMask_			un_ty.L3L4.is.icmp._codeMask

/* IGMP ACL Rule Definition */
#define igmpType_				un_ty.L3L4.is.igmp._type
#define igmpTypeMask_			un_ty.L3L4.is.igmp._typeMask

/* TCP ACL Rule Definition */
#define tcpl2srcMac_				un_ty.L3L4.is.tcp._l2srcMac		// for srcMac & destPort ACL rule
#define tcpl2srcMacMask_			un_ty.L3L4.is.tcp._l2srcMacMask
#define tcpSrcPortUB_			un_ty.L3L4.is.tcp._srcPortUpperBound
#define tcpSrcPortLB_			un_ty.L3L4.is.tcp._srcPortLowerBound
#define tcpDstPortUB_			un_ty.L3L4.is.tcp._dstPortUpperBound
#define tcpDstPortLB_			un_ty.L3L4.is.tcp._dstPortLowerBound
#define tcpFlagMask_			un_ty.L3L4.is.tcp._flagMask
#define tcpFlag_				un_ty.L3L4.is.tcp.un._flag
#define tcpURG_				un_ty.L3L4.is.tcp.un.s._urg
#define tcpACK_				un_ty.L3L4.is.tcp.un.s._ack
#define tcpPSH_				un_ty.L3L4.is.tcp.un.s._psh
#define tcpRST_				un_ty.L3L4.is.tcp.un.s._rst
#define tcpSYN_				un_ty.L3L4.is.tcp.un.s._syn
#define tcpFIN_				un_ty.L3L4.is.tcp.un.s._fin

/* UDP ACL Rule Definition */
#define udpl2srcMac_				un_ty.L3L4.is.udp._l2srcMac		// for srcMac & destPort ACL rule
#define udpl2srcMacMask_		un_ty.L3L4.is.udp._l2srcMacMask
#define udpSrcPortUB_			un_ty.L3L4.is.udp._srcPortUpperBound
#define udpSrcPortLB_			un_ty.L3L4.is.udp._srcPortLowerBound
#define udpDstPortUB_			un_ty.L3L4.is.udp._dstPortUpperBound
#define udpDstPortLB_			un_ty.L3L4.is.udp._dstPortLowerBound

#if 1 //chhuang: #ifdef RTL8650B
/* Source Filter ACL Rule Definition */
#define srcFilterMac_			un_ty.SRCFILTER._srcMac
#define srcFilterMacMask_		un_ty.SRCFILTER._srcMacMask
#define srcFilterPort_			un_ty.SRCFILTER._srcPort
#define srcFilterPortMask_		un_ty.SRCFILTER._srcPortMask
#define srcFilterVlanIdx_			un_ty.SRCFILTER._srcVlanIdx
#define srcFilterVlanIdxMask_		un_ty.SRCFILTER._srcVlanIdxMask
#define srcFilterIpAddr_			un_ty.SRCFILTER._srcIpAddr
#define srcFilterIpAddrMask_		un_ty.SRCFILTER._srcIpAddrMask
#define srcFilterPortUpperBound_	un_ty.SRCFILTER._srcPortUpperBound
#define srcFilterPortLowerBound_	un_ty.SRCFILTER._srcPortLowerBound
#define srcFilterIgnoreL3L4_		un_ty.SRCFILTER._ignoreL3L4
#define srcFilterIgnoreL4_		un_ty.SRCFILTER._ignoreL4

/* Destination Filter ACL Rule Definition */
#define dstFilterMac_			un_ty.DSTFILTER._dstMac
#define dstFilterMacMask_		un_ty.DSTFILTER._dstMacMask
#define dstFilterVlanIdx_			un_ty.DSTFILTER._vlanIdx
#define dstFilterVlanIdxMask_		un_ty.DSTFILTER._vlanIdxMask
#define dstFilterIpAddr_			un_ty.DSTFILTER._dstIpAddr
#define dstFilterIpAddrMask_		un_ty.DSTFILTER._dstIpAddrMask
#define dstFilterPortUpperBound_	un_ty.DSTFILTER._dstPortUpperBound
#define dstFilterPortLowerBound_	un_ty.DSTFILTER._dstPortLowerBound
#define dstFilterIgnoreL3L4_		un_ty.DSTFILTER._ignoreL3L4
#define dstFilterIgnoreL4_		un_ty.DSTFILTER._ignoreL4
#endif /* RTL8650B */

/* ACL Rule Action type Definition */
#define RTL8651_ACL_PERMIT			0x01
#define RTL8651_ACL_DROP				0x02
#define RTL8651_ACL_CPU				0x03
#define RTL8651_ACL_DROP_LOG			0x04
#define RTL8651_ACL_DROP_NOTIFY		0x05
#define RTL8651_ACL_L34_DROP			0x06	/* special for default ACL rule */

/* ACL Rule type Definition */
#define RTL8651_ACL_MAC				0x00
#define RTL8651_ACL_IP				0x01
#define RTL8651_ACL_ICMP				0x02
#define RTL8651_ACL_IGMP				0x03
#define RTL8651_ACL_TCP				0x04
#define RTL8651_ACL_UDP				0x05


/* User ACL rule match type definition */
#define RTL8651_ACL_GENERIC		0x00
#define RTL8651_ACL_L4NEWFLOW		0x01

typedef struct rtl865x_tblDrv_basicCounterParam_s {
	uint32	txPackets;
	uint32	txBytes;
	uint32	rxPackets;
	uint32	rxBytes;
	uint32	rxErrors;
	uint32	drops;
} rtl865x_tblDrv_basicCounterParam_t;


/*===============================================================================
 * Exported API categorized by usage
 *===============================================================================*/

// system 
void  rtl8651_timeUpdate(uint32 secPassed);
int32 rtl8651_enableNaptAutoAdd(int8 enable);

//acl
int32 rtl8651a_addAclRule(uint32 SessionID,int8 * ifName, int8 Ingress, rtl8651_tblDrvAclRule_t *rule);
#define rtl8651_addAclRule(ifName,Ingress,rule) rtl8651a_addAclRule(0,ifName,Ingress,rule) 
int32 rtl8651a_delAclRule(uint32 SessionID,int8 *ifName, int8 Ingress, rtl8651_tblDrvAclRule_t *rule);
#define rtl8651_delAclRule(ifName,Ingress,rule) rtl8651a_delAclRule(0,ifName,Ingress,rule)
int32 rtl8651a_addAclRuleExt(uint32 sessionId, int8 *ifName, int8 Ingress, rtl8651_tblDrvAclRule_t *rule);
#define rtl8651_addAclRuleExt(ifName, Ingress, rule) rtl8651a_addAclRuleExt(0, ifName, Ingress, rule)
int32 rtl8651a_delAclRuleExt(uint32 sessionID,int8 *ifName, int8 Ingress, rtl8651_tblDrvAclRule_t *rule);
#define rtl8651_delAclRuleExt(ifName, Ingress, rule) rtl8651a_delAclRuleExt(0, ifName, Ingress, rule)


int32 rtl8651a_flushAclRule(uint32 SessionID,int8 *ifName, int8 Ingress);
#define rtl8651_flushAclRule(ifName,Ingress) rtl8651a_flushAclRule(0,ifName,Ingress)
int32 rtl8651a_setDefaultAcl(uint32 SessionID,int8 *ifName, int8 defAction);
#define rtl8651_setDefaultAcl(ifName,defAction) rtl8651a_setDefaultAcl(0,ifName,defAction)

//pppoe 
//add a PPPoE object, an ASIC PPPoE  entry allocated here. Session ID should be set later.
int32 rtl8651_addPppoeSession(uint32 SessionID, uint16 vid);
int32 rtl8651_delPppoeSession(uint32 SessionID);
//config properties of a PPPoE interface. PPPoE asic entry not written to ASIC until this function called.
int32 rtl8651_setPppoeSessionProperty(uint32 SessionID, uint16 sid, ether_addr_t * macAddr, uint32 port);
//linear scan routing entry to find which route refers to this PPPoE session. Both located routing entry 
//and PPPoE entry got reset.
int32 rtl8651_resetPppoeSessionProperty(uint32 SessionID);
int32 rtl8651_getPppoeSessionMac(uint32 SessionID, ether_addr_t * macAddr);
int32 rtl8651_bindPppoeSession(ipaddr_t ipaddr, uint32 SessionID);
int32 rtl8651_setPppoeSessionHangUp(uint32 SessionID, int32 enable, uint32 sec, int32 (*p_callBack)(uint32));

uint32 * rtl8651_getNextPppoeSession(uint32 * curSessionId, uint32 *retSessionId);
int32 rtl8651_getPppoeCounter(uint32 curSessionId, rtl865x_tblDrv_basicCounterParam_t * counter);
int32 rtl8651_setPppoeSessionHangUp(uint32 SessionID, int32 enable, uint32 sec, int32 (*p_callBack)(uint32)) ;
int32 rtl8651_setPppoeDefaultSessionId(uint32 SessionID);
int32 rtl8651_addDemandRoute(rtl8651_tblDrvDemandRoute_t * rtAdd, int identity,int32(*p_callBack)(uint32));
uint32 _rtl8651_getDemandRoutingEntry(uint32 ip_s,uint32 ip_d,uint8 proto,uint16 ip_dp);
int32 rtl8651_flushDemandRoute(int identity);
void _rtl8651_recordTraffic(int i);
int32 _rtl8651_softIcmpPolicyRoute(ipaddr_t sip, uint16 sID, ipaddr_t dip, ipaddr_t *gip, uint16 *gID);
#if 0 // chenyl
int32 _rtl8651_softIcmpPolicyRoute(rtl8651_conntrack_info *info);
int32 _rtl8651_getSoftIcmpNaptEntry(rtl8651_conntrack_info *info,int *ip,uint16 *id); 
#endif
int32 _rtl8651_softNaptPolicyRoute(int type, ipaddr_t ip_s, ipaddr_t ip_d,uint16 ip_dp,
				int *ip_alias, uint16 *ip_aliasp);
int32 rtl8651_flushPolicyRoute(uint32 ip);
int32 rtl8651_addPolicyRoute(rtl8651_tblDrvPolicyRoute_t *rtAdd);


//interface 
//network interface: an IP broadcast domain. This is a pseudo layer between IP interface and VLAN 
//network interface is "external" if at least one NAT host, or one NAPT address, or one DMZ host refers to this network interface, 
//otherwise it is an "internal " interface by default.
int32 rtl8651_addNetworkIntf(int8 *ifName); //a route will be created  for each IP interface added
//If a routing entry/DMZ host/NAT host refers to the network interface to be deleted, delete operation is forbidden. 
int32 rtl8651_delNetworkIntf(int8 *ifName);
//specify this network interface''s default ACL action is drop or permit for this vlan
int32 rtl8651_setNetIfAclMismatchDrop(int8 *ifName, int8 isDrop); //chhuang
int32 rtl8651_addExtNetworkInterface(int8 * ifName); //make this network interface  for NAPT
//free ExtIPTable table entries occupied and makes this interfafe internal
int32 rtl8651_delExtNetworkInterface(int8 * ifName);

//Add a DMZ network makes this internal interface external.
//a DMZ host won't be written to ExtIPTable unless its ARP is learned.
int32 rtl8651_addDmzNetworkInterface(int8 *ifName);
int32 rtl8651_delDmzNetworkInterface(int8 *ifName);
int32 rtl8651_specifyNetworkIntfLinkLayerType(int8 * ifName, uint32 llType, uint16 vid);
int32 rtl8651_removeNetworkIntfLinkLayerType(int8 * ifName);

//IP interface: an IP subnet. multiple IP subnets can bind on the same network interface
int32 rtl8651_addIpIntf(int8 * ifName, ipaddr_t ipAddr, ipaddr_t ipMask);//
int32 rtl8651_delIpIntf(int8 * ifName, ipaddr_t ipAddr, ipaddr_t ipMask);

//arp 
int32 rtl8651_addArp(ipaddr_t ipAddr, ether_addr_t * macAddr, int8 * ifName, uint32 port);
int32 rtl8651_delArp(ipaddr_t ipAddr);
int32 rtl8651_setArpAgingTime(uint16 agTime);

//nat 
int32 rtl8651_addNatMapping(ipaddr_t extIpAddr, ipaddr_t intIpAddr); //....for NAT addresses
int32 rtl8651_delNatMapping(ipaddr_t extIpAddr, ipaddr_t intIpAddr);

//local-server 
int32 rtl8651_addLocalServer(ipaddr_t ipAddr); // No NAT between Private IP and Local Public IPs
int32 rtl8651_delLocalServer(ipaddr_t ipAddr);

//route 
int32 rtl8651_addRoute(ipaddr_t ipAddr, ipaddr_t ipMask, int8 * ifName, ipaddr_t nextHop);
int32 rtl8651_delRoute(ipaddr_t ipAddr, ipaddr_t ipMask);

//napt
int32 rtl8651_addNaptMapping(ipaddr_t extIpAddr);
int32 rtl8651_delNaptMapping(ipaddr_t extIpAddr);
int32 rtl8651_setNaptIcmpTimeout(uint32 timeout);
int32 rtl8651_setNaptUdpTimeout(uint32 timeout);
int32 rtl8651_setNaptTcpLongTimeout(uint32 timeout);
int32 rtl8651_setNaptTcpMediumTimeout(uint32 timeout);
int32 rtl8651_setNaptTcpFastTimeout(uint32 timeout);
int32 rtl8651_setNaptTcpUdpPortRange(uint16 start, uint16 end);
int32 rtl8651_is4WayHashEnable(uint8 *isEnable);
int32 rtl8651_enable4WayHash(uint8 isEnable);




//napt server port
int32 rtl8651a_addNaptServerPortMapping(uint32 SessionID,int8 isTcp, ipaddr_t extIpAddr, uint16 extPort, ipaddr_t intIpAddr, uint16 intPort);
#define rtl8651_addNaptServerPortMapping(isTcp,extIpAddr,extPort,intIpAddr,intPort) rtl8651a_addNaptServerPortMapping(0,isTcp,extIpAddr,extPort,intIpAddr,intPort)
int32 rtl8651a_delNaptServerPortMapping(uint32 SessionID,int8 isTcp, ipaddr_t extIpAddr, uint16 extPort, ipaddr_t intIpAddr, uint16 intPort);
#define rtl8651_delNaptServerPortMapping(isTcp,extIpAddr,extPort,intIpAddr,intPort) rtl8651a_delNaptServerPortMapping(0,isTcp,extIpAddr,extPort,intIpAddr,intPort)
int32 rtl8651a_addNaptServerPortRange(uint32 SessionID,int8 isTcp, ipaddr_t extIp, uint16 extPort, ipaddr_t intIp, uint16 intPort, uint16 nCount);
#define rtl8651_addNaptServerPortRange(isTcp,extIp,extPort,intIp,intPort,nCount) rtl8651a_addNaptServerPortRange(0,isTcp,extIp,extPort,intIp,intPort,nCount)
int32 rtl8651a_delNaptServerPortRange(uint32 SessionID,int8 isTcp, ipaddr_t extIp, uint16 extPort, ipaddr_t intIp, uint16 intPort, uint16 nCount);
#define rtl8651_delNaptServerPortRange(isTcp,extIp,extPort,intIp,intPort,nCount) rtl8651a_delNaptServerPortRange(0,isTcp,extIp,extPort,intIp,intPort,nCount)
int32 rtl8651_flushNaptServerPortbyExtIp(ipaddr_t extIp);



int32 rtl8651_flushAllNaptConnections(void);
int32 rtl8651_addNaptConnection(int16 assigned, int8 flowType, ipaddr_t insideLocalIpAddr, uint16 insideLocalPort, 
			ipaddr_t *insideGlobalIpAddr, uint16 *insideGlobalPort, ipaddr_t dstIpAddr, uint16 dstPort);
#define UDP_FLOW							0 	/* Create a UDP entry */
//All these values, when stored in ASIC, is decremented by 1
#define TCP_LISTEN_FLOW						1	/* Reserved entry for TCP connection in LISTEN_STATE*/
#define TCP_OUTBOUND_SYN_RCVD_FLOW			2	/* Create entry due to outbound SYN rcvd */
#define TCP_INBOUND_SYN_RCVD_FLOW			3	/* Create entry due to inbound SYN rcvd */
#define TCP_ESTABLISHED_FLOW				5	/* Create entry after 3-way handshaking completed */
#define TCP_OUTBOUND_FIN_RCVD_FLOW			6	/* Create entry due to outbound SYN rcvd */
#define TCP_INBOUND_FIN_RCVD_FLOW			7	/* Create entry due to inbound SYN rcvd */
#define TCP_CLOSED_FLOW						8	/*A terminated flow */

int32 rtl8651_delNaptConnection(int8 isTcp, ipaddr_t insideLocalIpAddr,
			uint16 insideLocalPort, ipaddr_t dstIpAddr, uint16 dstPort);
void rtl8651_updateAsicNaptTable(void);
int32 rtl8651_addNaptIcmpFlow(int8 assigned, ipaddr_t insideLocalIpAddr, uint16 insideLocalId, 
			ipaddr_t *insideGlobalIpAddr, uint16 *insideGlobalId, ipaddr_t dstIpAddr);
int32 rtl8651_delNaptIcmpFlow(ipaddr_t insideLocalIpAddr, uint16 insideLocalId, ipaddr_t dstIpAddr);
void rtl8651_updateAsicIcmpTable(void);
struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s;
void _rtl8651_refillOrClearAsicNaptEntry(uint32 tblIdx,struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s * selectedRefillFlow);


//alg-trap 
int32 rtl8651_addALGRule(uint16 startPort, uint16 endPort, int8 isTcp, int8 isServer, int8 isClient);
int32 rtl8651_delALGRule(uint16 startPort, uint16 endPort, int8 isTcp, int8 isServer, int8 isClient);

//url filter.

#define RTL8651_URLPKTFILTER_TYPE_SIP		0x01
#define RTL8651_URLPKTFILTER_TYPE_SMAC	0x02

typedef struct rtl8651_tblDrv_urlPktFilter_s {
	uint32 rule_type;

	ipaddr_t sip_start;
	ipaddr_t sip_end;

	ether_addr_t smac;
 
} rtl8651_tblDrv_urlPktFilter_t;

int32 rtl8651a_delURLfilterString(uint32 SessionID,int8 *string, int32 strlen);
#define rtl8651_delURLfilterString(string, strlen) rtl8651a_delURLfilterString(0,string, strlen)
int32 rtl8651a_delURLExactlyfilterString(uint32 SessionID,int8 *string, int32 strlen);
#define rtl8651_delURLExactlyfilterString(string, strlen) rtl8651a_delURLfilterString(0,string, strlen)
int32 rtl8651a_addURLfilterString(uint32 SessionID,int8 *string, int32 strlen);
#define rtl8651_addURLfilterString(string, strlen) rtl8651a_addURLfilterString(0,string, strlen)
int32 rtl8651a_addURLExactlyfilterString(uint32 SessionID,int8 *string, int32 strlen);
#define rtl8651_addURLExactlyfilterString(string, strlen) rtl8651a_addURLExactlyfilterString(0,string, strlen)
int32 rtl8651_addURLFilterRule(uint32 sessionId, int8 *string, int32 strlen, ipaddr_t ip_start, ipaddr_t ip_end);
int32 rtl8651_addURLExactlyFilterRule(uint32 sessionId, int8 *string, int32 strlen, ipaddr_t ip_start, ipaddr_t ip_end);
int32 rtl8651_delURLFilterRule(uint32 sessionId, int8 *string, int32 strlen, ipaddr_t ip_start, ipaddr_t ip_end);
int32 rtl8651_delURLExactlyFilterRule(uint32 sessionId, int8 *string, int32 strlen, ipaddr_t ip_start, ipaddr_t ip_end);
int32 rtl8651_addGenericURLFilterRule(uint32 sessionId, int8 *string, int32 strlen, uint8 isExactlyMatch, rtl8651_tblDrv_urlPktFilter_t *pktFilter);
int32 rtl8651_delGenericURLFilterRule(uint32 sessionId, int8 *string, int32 strlen, uint8 isExactlyMatch, rtl8651_tblDrv_urlPktFilter_t *pktFilter);

#define _RTL_LOGGING        1 /* enable logging mechanism support */
#define _RTL_NEW_LOGGING_MODEL
#ifdef _RTL_LOGGING
#ifndef _RTL_NEW_LOGGING_MODEL
#define RTL8651_LOGACTION_DROP    1
#define RTL8651_LOGACTION_RESET   2
#define RTL8651_LOGACTION_FORWARD 4
#endif
/**********************************************************
	direction
**********************************************************/
#define RTL8651_DIRECTION_NO	2
#define RTL8651_LAN2WAN   		0
#define RTL8651_WAN2LAN   		1
/**********************************************************
	Module facility types
**********************************************************/
/* Rome Driver Related Log module */
#define RTL8651_LOGMODULE_SYS 		0x00000001
#define RTL8651_LOGMODULE_ACL 		0x00000002
#define RTL8651_LOGMODULE_DOS 		0x00000004
#define RTL8651_LOGMODULE_URL 		0x00000008
#define RTL8651_LOGMODULE_NEWFLOW	0x00000010
/**********************************************************
	Action types
**********************************************************/
#ifdef _RTL_NEW_LOGGING_MODEL
#define RTL8651_LOGACTION_NO			4
#define RTL8651_LOGACTION_NONE		0
#define RTL8651_LOGACTION_DROP    		1
#define RTL8651_LOGACTION_RESET   		2
#define RTL8651_LOGACTION_FORWARD 	3
#endif
/***************************************************************************
					Log No.
***************************************************************************/
#define RTL8651_TOTAL_USERLOG_NO	37	/* total number of added log */

/* RTL8651_LOGMODULE_NEWFLOW */
#define RTL8651_LOG_NEWFLOW_NewTcpNaptOutbound		1
#define RTL8651_LOG_NEWFLOW_NewTcpNaptInbound		2
#define RTL8651_LOG_NEWFLOW_NewUdpNaptOutbound		3
#define RTL8651_LOG_NEWFLOW_NewUdpNaptInbound		4
/* RTL8651_LOGMODULE_ACL */
#define RTL8651_LOG_ACL_EgressAclDropLog			5
#define RTL8651_LOG_ACL_IngressAclDropLog			6
/* RTL8651_LOGMODULE_URL */
#define RTL8651_LOG_URL_MatchUrlFilter				7
/* RTL8651_LOGMODULE_DOS */
#define RTL8651_LOG_DOS_IpSpoof				8
#define RTL8651_LOG_DOS_UdpFlood				9
#define RTL8651_LOG_DOS_HostUdpFlood			10
#define RTL8651_LOG_DOS_UdpLand				11
#define RTL8651_LOG_DOS_UdpBomb				12
#define RTL8651_LOG_DOS_UdpEchoChargen		13
#define RTL8651_LOG_DOS_IcmpLand				14
#define RTL8651_LOG_DOS_IcmpPingOfDeath		15
#define RTL8651_LOG_DOS_IcmpFlood				16
#define RTL8651_LOG_DOS_HostIcmpFlood			17
#define RTL8651_LOG_DOS_IcmpSmurf				18
#define RTL8651_LOG_DOS_SynFlood				19
#define RTL8651_LOG_DOS_HostSynFlood			20
#define RTL8651_LOG_DOS_StealthFin				21
#define RTL8651_LOG_DOS_HostStealthFin			22
#define RTL8651_LOG_DOS_TcpLand				23
#define RTL8651_LOG_DOS_TcpScan				24
#define RTL8651_LOG_DOS_TcpSynWithData		25
#define RTL8651_LOG_DOS_TearDrop				26
#define RTL8651_LOG_DOS_TcpUdpScan_SYN		27
#define RTL8651_LOG_DOS_TcpUdpScan_FIN		28
#define RTL8651_LOG_DOS_TcpUdpScan_ACK		29
#define RTL8651_LOG_DOS_TcpUdpScan_UDP		30
#define RTL8651_LOG_DOS_TcpUdpScan_HYBRID	31
/***************************************************************************
					Logging Data structure definition
***************************************************************************/
#define RTL8651_LOG_INFO_PKT			1	/* record the pkt's information */
#define RTL8651_LOG_INFO_URL			2	/* record the url blocking's information */
typedef struct rtl8651_logInfo_s {
	uint32 infoType;
	/************ data structure different Log Information ************/
	union {
		struct urlLogInfo {
			uint32	dsid;
			uint8	protocol;
			uint8	direction;
			ipaddr_t	sip;
			ipaddr_t	dip;
			uint16	sport;
			uint16	dport;
			char *	string;	/* matched url string */
		} url;
		struct packetLogInfo {
			uint32	dsid;
			uint8	protocol;
			uint8	direction;
			ipaddr_t	sip;
			ipaddr_t	dip;
			uint16	sport;
			uint16	dport;
			uint8	icmpType;
			uint16	icmpId;
		} pkt;
	}un;

	#define pkt_dsid			un.pkt.dsid
	#define pkt_proto		un.pkt.protocol
	#define pkt_direction		un.pkt.direction
	#define pkt_sip			un.pkt.sip
	#define pkt_dip			un.pkt.dip
	#define pkt_sport			un.pkt.sport
	#define pkt_dport		un.pkt.dport
	#define pkt_icmpType	un.pkt.icmpType
	#define pkt_icmpId		un.pkt.icmpId

	#define url_dsid			un.url.dsid
	#define url_proto			un.url.protocol
	#define url_direction		un.url.direction
	#define url_sip			un.url.sip
	#define url_dip			un.url.dip
	#define url_sport			un.url.sport
	#define url_dport			un.url.dport
	#define url_string		un.url.string

	uint32 action;	/* action of this logging event */
} rtl8651_logInfo_t;

//user logging function plugin
#ifndef _RTL_NEW_LOGGING_MODEL
typedef int32 (*rtl8651_loggingFuncPtr_t)
(
unsigned long  dsid,
unsigned long  moduleId,
unsigned char  proto,
char           direction,
unsigned long  sip,
unsigned long  dip,
unsigned short sport,
unsigned short dport,
unsigned char  type,
unsigned char  action,
char         * msg
);
#else
typedef int32 (*rtl8651_loggingFuncPtr_t)
(
	uint32			moduleId,
	uint32			logNo,
	rtl8651_logInfo_t*	info
);
#endif

int32 rtl8651_installLoggingFunction(void * pMyLoggingFunc);
int32 rtl8651a_enableLogging(uint32 SessionID,uint32 moduleId,int8 enable);
#define rtl8651_enableLogging(moduleId,enable) rtl8651a_enableLogging(0,moduleId,enable)
extern rtl8651_loggingFuncPtr_t _pUserLoggingFunc;
#endif /* _RTL_LOGGING */

// Other functions
int32 rtl8651_tblDrvReset(void);
int32 rtl8651_tblDrvReinit(void);

int32 rtl8651_tblDrvInit(rtl8651_tblDrvInitPara_t * configPara);

int32 rtl8651_getPppoeSessionSourceMac(uint32 SessionID, ether_addr_t * macAddr);
ipaddr_t _rtl8651_getNaptInsideGlobalIpAddr(int8 isTcp, ipaddr_t insideLocalIpAddr, uint16 insideLocalPortOrId, ipaddr_t dstIpAddr, uint16 dstPort);


//DMZ Host
int32 rtl8651a_addDmzHost(uint32 SessionID,ipaddr_t naptIp, ipaddr_t dmzHostIp);
#define rtl8651_addDmzHost(naptIp,dmzHostIp) rtl8651a_addDmzHost(0,naptIp,dmzHostIp)
int32 rtl8651a_delDmzHost(uint32 SessionID,ipaddr_t naptIp);
#define rtl8651_delDmzHost(naptIp) rtl8651a_delDmzHost(0,naptIp)

//IP Unnumbered
int32 rtl8651_addIpUnnumbered(int8 *wanIfName, int8 *lanIfName, ipaddr_t netMask);
int32 rtl8651_delIpUnnumbered(int8 *wanIfName, int8 *lanIfName);
int32 rtl8651_getIpUnnumbered(int8 *wanIfName, int8 *ifName, ipaddr_t *netMask);
int32 rtl8651_setLanSideExternalIpInterface(int8 * ifName, ipaddr_t ipAddr, ipaddr_t ipMask, int8 isExternal);

//QoS
int32 rtl8651_setFlowControl(uint32 port, int8 enable);
int32 rtl8651_setQueueScheduling(uint32 type, uint32 weight);
int32 rtl8651_setPortPriorityQueue(uint32 port, int8 highPriority);
int32 rtl8651_setDiffServDSCP(uint32 dscp, int8 highPriority);
int32 rtl8651_getMNQueueEntry(rtl8651_tblDrvAclRule_t *rule,uint32 GroupId,uint32 index);

/* PPPoE Passthru */
int8 rtl8651_EnablePppoePassthru( int8 enable );
int8 rtl8651_enableDropUnknownPppoePADT( int8 enable );
int32 rtl8651_getPppoePassthruVid(void);
int32 rtl8651_getExternalInterfaceforPPPoEPassthru(void);

/* IPv6 passthru */
int8 rtl8651_EnableIpv6Passthru(int8 enable);
int32 rtl8651_getIpv6PassthruVid(void);
int32 rtl8651_getExternalInterfaceforIPv6Passthru(void);

/* IPX passthru */
int8 rtl8651_EnableIpxPassthru(int8 enable);
int32 rtl8651_getIpxPassthruVid(void);
int32 rtl8651_getExternalInterfaceforIPXPassthru(void);

/* Netbios passthru */
int8 rtl8651_EnableNetbiosPassthru(int8 enable);
int32 rtl8651_getNetbiosPassthruVid(void);
int32 rtl8651_getExternalInterfaceforNetbiosPassthru(void);

/* Passthru Generic API */
int32 rtl8651_transformPasstheuVlanId( uint32 oldVlanIdx, uint32* newVlanIdx );

void rtl8651_setMtu(uint16 mtu);
int32 rtl8651_setNetMtu(int8 *ifName, uint16 mtu);
void rtl8651_getMtu(uint16 * mtu);

uint8 rtl8651_EnableProtocolBasedNAT( uint8 enable );
int32 rtl8651_addProtocolBasedNAT( uint8 protocol, ipaddr_t ExtIp, ipaddr_t IntIp );
int32 rtl8651_delProtocolBasedNAT( uint8 protocol, ipaddr_t ExtIp, ipaddr_t IntIp );

int32 rtl8651_enableMacAccessControl(int8 enable);
int32 rtl8651_setMacAccessControlDefaultPermit(int8 isPermit);
int32 rtl8651_delMacAccessControl(ether_addr_t *sMac, ether_addr_t *macMask);
int32 rtl8651_flushMacAccessControl(void);
int32 rtl8651_addMacAccessControl(ether_addr_t *sMac, ether_addr_t *macMask, int8 isPermit);


// multicast
int32 rtl8651_addExternalMulticastPort(uint32 extMPort);
int32 rtl8651_delExternalMulticastPort(uint32 extMPort);

int32 rtl8651_autoCrossOverDetect(uint32 isEnable);
int32 rtl8651_portAutoCrossOverDetect(uint32 port, uint32 isEnable);
int32 rtl8651_setMdiMdix(uint32 isMdi);
int32 rtl8651_setPortMdiMdix(uint32 port, uint32 isMdi);




extern int8 rtl8651_tblDrv_Id[];
extern int8 rtl8651_tblDrvFwd_Id[];



/* For Policy Route*/
#define TYPE_BITS		0xf
#define DYNAMIC_POLICY_ROUTE	0x2
#define STATIC_POLICY_ROUTE	0x4
#define TRIGGER_DSTIP 	0x10	
#define TRIGGER_DSTTCPPORT	0x20
#define TRIGGER_SRCIP 	0x30	
#define TRIGGER_DSTUDPPORT	0x40
#define TRIGGER_SRCGLOBALIP	0x50
#define RTENTRY_FOUND	1
#define RTENTRY_FULL	2


/* For QoS: Scheduling type */
#define RTL8651_QOS_RR		0x01
#define RTL8651_QOS_ALG_HIGH_QUEUE 100
#define RTL8651_QOS_ALG_LOW_QUEUE 200

/* For round robin ration */
#define RTL8651_RR_H_FIRST		0x03
#define RTL8651_RR_H16_L1		0x02
#define RTL8651_RR_H8_L1		0x01
#define RTL8651_RR_H4_L1		0x00



#define RTL8651_PORT_0                  0x00
#define RTL8651_PORT_1                  0x01
#define RTL8651_PORT_2                  0x02
#define RTL8651_PORT_3                  0x03
#define RTL8651_PORT_4                  0x04
#define RTL8651_MII_PORT                0x05
#define RTL8651_CPU_PORT                0x07

#define RTL8651_PORT_EXT1				0x06
#define RTL8651_PORT_EXT2				0x07
#define RTL8651_PORT_EXT3				0x08

#define RTL8651_PPPOE			0x01
#define RTL8651_PPTP			0x02
#define RTL8651_L2TP			0x03

//Ethernet port bandwidth control
#define RTL8651_BC_FULL		0x00
#define RTL8651_BC_128K		0x01
#define RTL8651_BC_256K		0x02
#define RTL8651_BC_512K		0x03
#define RTL8651_BC_1M		0x04
#define RTL8651_BC_2M		0x05
#define RTL8651_BC_4M		0x06
#define RTL8651_BC_8M		0x07
#define RTL8651_BC_16M		0x08
#define RTL8651_BC_32M		0x09
#define RTL8651_BC_64M		0x0A


#define PS_ACTION_SERVER 0
#define PS_ACTION_CLIENT 1
#define PS_ACTION_NONE 0
#define PS_ACTION_DROP 1
#define PS_ACTION_TO_PS 2
#define PS_ACTION_DONT_TIMEOUT 3
#define RTL8651_MAX_PROTO_STACK_ACTIONS 8
struct _RTL8651_PS_ACTION_ENTRY{
	ipaddr_t ip;
	ipaddr_t mask;
	uint16	orgPort;
	uint16	orgPortEnd;
	uint16	fwdPort;
	uint8	protocol; // id the same as IP header
	uint8	action; // PS_ACTION_NONE,PS_ACTION_DROP,PS_ACTION_TO_PS
	uint8	role; // PS_ACTION_SERVER,PS_ACTION_CLIENT
};


extern int32 wlan_acc_debug;


/*CONFIG_RTL865XB_EXP_PPPOE_PASSTHRU*/
extern uint8 rtl8651_gPPPoEExternalVlanIndex;  /* External VLAN for PPPoE dialup */
extern ether_addr_t rtl8651_gPPPoEExternalGatewayMAC; 
extern uint16 rtl8651_gPPPoEPassthruVlanID;  /* VLAN ID used for PPPoE Passthru */
#define RTL8651_PPPOEPASSTHRU_VLANID_IS_NOT_SET (4096+1)

#define RTL8651_PPPOEPASSTHRU_DEFAULT_TIMEOUT 300 /* Unit: seconds */

uint32 _rtl8651_setPPPoEPassthruTimeout( uint32 timeout );
int32 rtl8651_addPPPoEPassthru( uint16 sid, ether_addr_t* clientMac, ether_addr_t* serverMac,
                                uint16 clientVid, uint16 serverVid );
int32 rtl8651_getPPPoEPassthru( uint16 sid, ether_addr_t* clientMac, ether_addr_t* serverMac,
                                uint16 *clientVid, uint16 *serverVid, int32 updateTouch );
int32 rtl8651_delPPPoEPassthru( uint16 sid );

extern uint32 gPPPoEPassthruTimeout;



/* CONFIG_RTL865XB_EXP_IPV6_PASSTHRU */
extern uint8 rtl8651_gIPv6ExternalVlanIndex;  /* External VLAN for PPPoE dialup */
extern ether_addr_t rtl8651_gIPv6ExternalGatewayMAC; 
extern uint16 rtl8651_gIPv6PassthruVlanID;  /* VLAN ID used for PPPoE Passthru */
extern uint32 rtl8651_pbv_ipv6passthru;      /* Protocol-based VLAN control register: 5 and 6 are valid. */
#define RTL8651_IPV6PASSTHRU_VLANID_IS_NOT_SET (4096+1)
#define RTL8651_IPV6PASSTHRU_PBVID_IS_NOT_SET RTL8651_PBV_RULE_MAX


/* CONFIG_RTL865XB_EXP_IPX_PASSTHRU */
extern uint8 rtl8651_gIPXExternalVlanIndex;  /* External VLAN for IPX */
extern ether_addr_t rtl8651_gIPXExternalGatewayMAC; 
extern uint16 rtl8651_gIPXPassthruVlanID;  /* VLAN ID used for PPPoE Passthru */
extern uint32 rtl8651_pbv_ipxpassthru;      /* Protocol-based VLAN control register: 5 and 6 are valid. */
#define RTL8651_IPXPASSTHRU_VLANID_IS_NOT_SET (4096+1)
#define RTL8651_IPXPASSTHRU_PBVID_IS_NOT_SET RTL8651_PBV_RULE_MAX


/* CONFIG_RTL865XB_EXP_NETBIOS_PASSTHRU */
extern uint8 rtl8651_gNetbiosExternalVlanIndex;  /* External VLAN for IPX */
extern ether_addr_t rtl8651_gNetbiosExternalGatewayMAC; 
extern uint16 rtl8651_gNetbiosPassthruVlanID;  /* VLAN ID used for PPPoE Passthru */
extern uint32 rtl8651_pbv_netbiospassthru;      /* Protocol-based VLAN control register: 5 and 6 are valid. */
#define RTL8651_NETBIOSPASSTHRU_VLANID_IS_NOT_SET (4096+1)
#define RTL8651_NETBIOSPASSTHRU_PBVID_IS_NOT_SET RTL8651_PBV_RULE_MAX


extern int8 rtl8651_drvProtocolBasedNatEnable;

/* for all kind of Passthru */
#define RTL8651_PASSTHRU_VLANID_IS_NOT_SET (4096+1)

/* for napt connection add */
#define	EXTIPPORT_ASSIGNED		0x01
#define	PURE_SW					0x02
#define	DONT_CHECK_INTIP			0x04
#define	TO_PROTOCOL_STACK		0x08
#define	DONT_BIRD_CDROUTER		0x10
#define	DOS_NAPT_ENCOUNT			0x20
#define	DOS_NAPT_FROMINTERNAL	0x40
#define	ALG_QOS					0x80

#endif

