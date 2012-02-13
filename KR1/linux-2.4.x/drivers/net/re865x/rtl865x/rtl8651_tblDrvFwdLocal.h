/*
* Copyright c                  Realtek Semiconductor Corporation, 2003
* All rights reserved.
* 
* Program : Internal Header file for  fowrading engine rtl8651_tblDrvFwd.c
* Abstract : 
* Author :  Chun-Feng Liu(cfliu@realtek.com.tw)
* $Id: rtl8651_tblDrvFwdLocal.h,v 1.20 2005/03/28 12:17:17 chenyl Exp $
* $Log: rtl8651_tblDrvFwdLocal.h,v $
* Revision 1.20  2005/03/28 12:17:17  chenyl
* +: Exception List for NAPT-Redirect system
*
* Revision 1.19  2005/03/28 06:01:51  chenyl
* *: NAPT redirect without DIp/Dport modification
*
* Revision 1.18  2005/03/14 15:33:12  chenyl
* *: modify IP fragment packet trapping mechanism for ProtocolStack cached flow
*
* Revision 1.17  2005/03/14 14:40:15  chenyl
* *: bug fix: protocol-stack flow cache, fragment packet forwarding
*
* Revision 1.16  2005/03/03 03:00:49  chenyl
* *: Only trap L3/4 processed IP packet to CPU for IP-Range ACL rules
*
* Revision 1.15  2005/02/16 02:15:12  chenyl
* +: Forwarding Engine Trapping Dispatch system:
* 	When one packet must to be trapped to upper-layer protocol stack,
* 	User can decide how to dispatch this packet into different "categories"
* 	according to its 'l4protocol/source IP/destination IP/source Port (TCP/UDP)/
* 	destination Port (TCP/UDP)' with wide-card allowed. The category
* 	number is decided by user and will be set into ph_category in packet header.
* 	Fragment packet's test is passed.
*
* +: Forwarding Engine QoS Send:
* 	Protocol stack packet send with high TX queue used.
*
* Revision 1.14  2005/02/02 01:37:57  chenyl
* *: fix L2TP ICCN/ZLB erroneous judgement problem
* +: forge L2TP header if ICCN has sent by protocol stack and sensed by ROMEDRV
*
* Revision 1.13  2005/01/26 08:27:57  chenyl
* *: fix PPTP/L2TP WAN type bugs:
*
* 	1. After decapsulating, and the DIP is broadcast IP (0xffffffff), we must let DMAC been ff-ff-ff-ff-ff-ff
* 	2. System MUST forbid L2 broadcasting if packet is from PPTP/L2TP loopback Port.
* 	3. Before trapping packet to protocol stack, system MUST modify packet's source VLAN if this packet is from
* 	   PPTP/L2TP loopback Port.
*
* Revision 1.12  2005/01/18 08:50:22  ghhuang
* +: Diffserv (DSCP) remarking support
*
* Revision 1.11  2004/12/22 05:19:15  tony
* +: support new features: make ALG(FTP,SIP,NetMeeting) support MNQueue.
*
* Revision 1.10  2004/12/14 08:29:17  yjlou
* *: add a delta parameter for _rtl8651_l4TcpAdjustMss().
*
* Revision 1.9  2004/12/10 08:45:20  yjlou
* +: support ACLDB and IPSEC (20Mbps)
*
* Revision 1.8  2004/12/08 10:20:24  chenyl
* *: rearrange variable/function declarations in rtl8651_tblDrvFwd.c
* +: apply reason-bit for Ingress ACL check --
* 	- If one Ingress ACL rule is matched by ASIC, the rules checked by ASIC(index less than matched rule)
* 	  will not be checked again
* 	- If All ingress ACL rules are checked and permitted by ASIC, then all ingress ACL rules will not be checked again.
*
* Revision 1.7  2004/11/29 07:35:43  chenyl
* *: fix bug : multiple-PPPoE MTU setting bug
* *: fix bug : Never add second session's ip into ASIC in multiple-PPPoE WAN type.
* *: fix bug : Tunekey abnormal behavior when multiple-PPPoE Dial-on-demand Dynamically turn ON/OFF
* *: in forwarding engine Init function, the parameter == 0xffffffff means user want to set as default value
* *: add Mutex-Lock/Unlock checking in testing code
*
* Revision 1.6  2004/11/10 07:40:18  chenyl
* *: fragment cache re-write (1. code re-arrange, 2. check org-sip/dip when getting cache)
*
* Revision 1.5  2004/10/04 02:16:23  chenyl
* +: pptp/l2tp header refill function
*
* Revision 1.4  2004/09/30 07:59:12  chenyl
* *: function mtuFragment() modification
* +: apply forwarding engine to generate ICMP-Host-Unreachable message for packets fragmented by gateway
* +: add the switch to turn on/off "always reply ICMP error message for all packets fragmented by gateway regardless of
* 	Dont fragment bit"
*
* Revision 1.3  2004/09/15 16:14:28  chenyl
* +: enable multicast forward cache
*
* Revision 1.2  2004/09/13 04:17:36  chenyl
* +: napt redirect inbound fragment cache
*
* Revision 1.1  2004/09/07 14:52:13  chenyl
* *: bug fix: napt-redirect fragment packet checksum-recalculate
* *: bug fix: conflict between protocol stack flow cache and protocol stack action
* *: bug fix: protocol stack action mis-process to UDP-zero-checksum packet
* *: separate the header file:
*         - internal : rtl8651_tblDrvFwdLocal.h
*         - external : rtl8651_tblDrvFwd.h
*
*/
#ifndef RTL8651_FWDENGINE_LOCAL_H
#define RTL8651_FWDENGINE_LOCAL_H

#include "assert.h"
#include "types.h"
#ifndef CYGWIN
#include <linux/config.h>
#endif /* CYGWIN */
#include "rtl_queue.h"
#include "rtl_errno.h"
#include "mbuf.h"

#include "rtl8651_tblDrvProto.h"
#include "rtl8651_tblDrvLocal.h"
#include "rtl8651_tblDrvFwd.h"
#include "rtl8651_ipQueue_local.h"
#include "rtl8651_multicast_local.h"

/*********************************************************************************************************
	ACL
**********************************************************************************************************/
int32 _rtl8651_ingressAcl(uint32 dsid,uint32 startIdx, rtl8651_tblDrv_vlanTable_t *pVlan,int32 vid, uint8 *m_data, uint8 *ip);
int32 _rtl8651_egressAcl(uint32 dsid,rtl8651_tblDrv_vlanTable_t *pVlan,int32 vid, uint8 *ethHdr, uint8 *ip);

/*********************************************************************************************************
	L2 forward
**********************************************************************************************************/
int32 rtl8651_fwdEngineL2Input(struct rtl_pktHdr *pkthdrPtr,rtl8651_tblDrv_vlanTable_t *local_vlanp, uint8 *m_data, uint16 ethtype);

/*********************************************************************************************************
	L3 routing
**********************************************************************************************************/
int32 _rtl8651_l3FastActRouting(struct rtl_pktHdr *pkthdrPtr, ipaddr_t dst_ip, struct ip *ip, void **);

/*********************************************************************************************************
	ALG
**********************************************************************************************************/
void _rtl8651_l4AddSeq(struct ip *, rtl8651_tblDrv_naptTcpUdpFlowEntry_t *, int32 );
int32 _rtl8651_l4SeqDiff(uint32 , uint32 );
int32 _rtl8651_l4GetDeltaSeqOut(struct ip *, struct tcphdr *, rtl8651_tblDrv_naptTcpUdpFlowEntry_t  *);

/*********************************************************************************************************
	Fragment Shortest Path Forwarding
**********************************************************************************************************/
#define RTL8651_FRAGMENT_SHORTESTPATH_FWD
#ifdef RTL8651_FRAGMENT_SHORTESTPATH_FWD

#define RTL8651_FRAGMENT_SHORTESTPATH_STATUS_HALFOK	1
#define RTL8651_FRAGMENT_SHORTESTPATH_STATUS_SETOK	2

#define RTL8651_FRAGMENT_SHORTESTPATH_MODE_MODIFYSIP	0
#define RTL8651_FRAGMENT_SHORTESTPATH_MODE_MODIFYDIP	1

#define RTL8651_FRAGMENT_SHORTESTPATH_TIMEOUT			3

typedef struct _rtl8651_FragmentShortestPathFwdCache_s
{
	/* information to search flow entry */
	uint8 status;
	ipaddr_t sip;
	ipaddr_t dip;
	uint16 id;
	/* information to forward matched packet */
	uint8 mode;
	ipaddr_t aliasIp;
//	rtl8651_tblDrv_vlanTable_t *dst_vlan;
	struct ether_addr smac;
	struct ether_addr dmac;
	uint16 vlanIdx;
	uint16 portList;
	uint8 extPortList;
	uint8 srcExtPortNum;
	uint16 pppoeIdx;
	uint16 flags;		/* checksum & pppoe_auto_add */
	/* timeout */
	uint8 age;
} _rtl8651_FragmentShortestPathFwdCache_t;

#define RTL8651_FRAGMENTSHORTESTPATH_TBLSIZE	32	/* must be power-of-2 */

inline void _rtl8651_fragmentShortestPathCacheSet_secondHalf(struct rtl_pktHdr *pkthdr, uint8 *m_data, struct ip*pip);
extern _rtl8651_FragmentShortestPathFwdCache_t fragmentShortestPathCache[RTL8651_FRAGMENTSHORTESTPATH_TBLSIZE];
#endif /* RTL8651_FRAGMENT_SHORTESTPATH_FWD */

/*********************************************************************************************************
	ICMP reply
**********************************************************************************************************/
// define parameters passed to fwd-engine ICMP reply functions
typedef struct _rtl8651_IcmpPktField_s{
	uint8	ic_type;
	uint8	ic_code;
	union{
		uint16	next_mtu;
	} ic_hun;
	#define ic_mtu	ic_hun.next_mtu
} _rtl8651_IcmpPktField_t;
int32 _rtl8651_drvIcmpErrorGeneration(struct rtl_pktHdr *, struct ip*, _rtl8651_IcmpPktField_t *, uint16);

/*********************************************************************************************************
	UPnP
**********************************************************************************************************/
#ifdef _RTL_UPNP
struct _RTL8651_UPNP_ENTRY{
	ipaddr_t remote_ip;
	uint16 remote_port; 
	ipaddr_t alias_ip;
	uint16 alias_port;
	ipaddr_t local_ip;
	uint16 local_port;	
	
	uint32 flags;
	uint32 age; /* second */
	uint32 algIdx; /* used by ALG QoS */
};
#define UPNP_VLAID		0x01
#define UPNP_TCP		0x02
#define UPNP_ONESHOT	0x04
#define UPNP_PERSIST	0x08
#define UPNP_ALG_QOS	ALG_QOS


#define UPNP_ITEM 60
#define UPNP_AGE 120
#endif /* _RTL_UPNP */

/*********************************************************************************************************
	Trigger port
**********************************************************************************************************/
#ifdef _RTL_TRIGGER_PORT
#define TRIGGER_PORT_ITEM 64
struct _RTL8651_TRIGGER_PORT_ENTRY{
	uint32 dsid;
	ipaddr_t localIpAddr;
	uint8  inType;
	uint8  outType;
	uint16 inPortStart; 
	uint16 inPortFinish; 
	uint16 outPortStart; 
	uint16 outPortFinish;
};
#endif /* _RTL_TRIGGER_PORT */

/*********************************************************************************************************
	Port bouncing
**********************************************************************************************************/
#ifdef _RTL_PORT_BOUNCING
#define PORT_BOUNCING_ITEM 64
struct _RTL8651_PORT_BOUNCING_ENTRY{
	
	ipaddr_t clientIpAddr;
	ipaddr_t gatewayIpAddr;	
	ipaddr_t serverIpAddr;
	uint16 clientPort; 
	uint16 gatewayPort; 	
	uint16 serverPort;
	uint16 id;					// for ICMP flow: l4 id field
	uint16 flowType;
	uint32 tcpFlag;				// state for tcp flow
	uint32 age;
};
#endif /* _RTL_PORT_BOUNCING */

/*********************************************************************************************************
	Fragment cache
**********************************************************************************************************/
#ifdef _RTL_NAPT_FRAG
#define DEFAULT_MAX_FRAGPKT_NUM				32
#define DEFAULT_MAX_FRAGCACHE_AGE			5
#define INBOUND_FRAG							0
#define OUTBOUND_FRAG							1
#define BOUNCING_FRAG							2
#define ICMP_REPLY_FRAG						3
#define PROTOCOL_STACK_FRAG					4
#define NAPT_REDIRECT_FRAG						5
#define NAPT_REDIRECT_FRAG_INBOUND			6
#define TRAP_DISPATCH_FRAG					7
#define RTL8651_FRAGCACHE_TYPE_COUNT			8	/* max count of fragment cache */
typedef struct _rtl8651_frag {
	/* <--- Informations ---> */
	ipaddr_t		new_ip;
	int8			routeIdx;
	void 		*napt_entry_t;
	uint8		dsid;
	/* <--- Control Informations ---> */
	uint8		age;
	/* <--- Index ---> */
	uint16		ip_id;
	ipaddr_t		org_sip;
	ipaddr_t		org_dip;
} _rtl8651_frag_t;
#endif /* _RTL_NAPT_FRAG */

/*********************************************************************************************************
	Pppoe active session table
**********************************************************************************************************/
typedef struct _rtl8651_pppoeActiveSession_entry_s {
	uint16 sessionId;
	uint16 keepAlive;//When value down-count to zero, this entry becomes invalid
	struct _rtl8651_pppoeActiveSession_entry_s * next;
} _rtl8651_pppoeActiveSession_entry_t;

/*********************************************************************************************************
	mbuf related
**********************************************************************************************************/
#define FWDENG_MBUF_HEADROOM	128  /*default headroom reserved*/

/*********************************************************************************************************
	forwarding engine initiation
**********************************************************************************************************/
extern rtl8651_fwdEngineInitPara_t rtl8651_fwdEnginePara;
#define FWD_INIT_CHECK(expr) do {\
	if((expr)!=SUCCESS){\
		rtlglue_printf("Error >>> initialize failed at line %d!!!\n", __LINE__);\
			return FAILED;\
	}\
}while(0)

/*********************************************************************************************************
	protocol stack action
**********************************************************************************************************/
//#define RTL8651_MAX_PROTO_STACK_USED_UDP_PORTS 8
//#define RTL8651_MAX_PROTO_STACK_SERVER_USED_TCP_PORTS 8


/*********************************************************************************************************
	Trapping Dispatch system
**********************************************************************************************************/
#ifdef _RTL_TRAPPING_DISPATCH
#define RTL8651_TRAPPING_DISPATCH_DEFAULT_ENTRYCNT		128

typedef struct _rtl8651_trapping_dispatch_entry_s
{
	uint8 l4Proto;			/* IPPROTO_TCP, IPPROTO_UDP, IPPROTO_ICMP ... etc */

	/* Layer 3 : for IP layer */
	ipaddr_t srcIp;
	ipaddr_t dstIp;

	/* Layer 4 : for TCP and UDP */
	int32 srcPort;
	int32 dstPort;

	/* the category of this specific dispatch entry */
	uint16 category;

	/* <--- data structure ---> */
	struct _rtl8651_trapping_dispatch_entry_s *prev;
	struct _rtl8651_trapping_dispatch_entry_s *next;
} _rtl8651_trapping_dispatch_entry_t;

typedef struct _rtl8651_trapping_dispatch_cache_s
{
	uint8 l4Proto;
	ipaddr_t srcIp;
	ipaddr_t dstIp;
	uint16 srcPort;
	uint16 dstPort;

	_rtl8651_trapping_dispatch_entry_t *ptr;

} _rtl8651_trapping_dispatch_cache_t;

#endif /* _RTL_TRAPPING_DISPATCH */

/*********************************************************************************************************
	ARP Queue
**********************************************************************************************************/
#ifdef _RTL_QUEUE_ARP_PACKET

#define		QUEUE_PACKET_ITEM	32
#define		Q_ARP					1
#define		Q_TIME					3

struct _RTL8651_QUEUE_PACKET_ENTRY
{
	uint8 q_type;
	uint8 q_time;
	uint16 q_id;
	uint32 q_waitIp;
	struct rtl_pktHdr * q_pkthdrPtr;
	struct ip * q_ip;
};
#endif /* _RTL_QUEUE_ARP_PACKET */

/*********************************************************************************************************
	Checksum
**********************************************************************************************************/
uint16  _rtl8651_ipChecksum(struct ip * pip);
uint16 _rtl8651_icmpChecksum(struct ip * pip);
uint16 _rtl8651_tcpChecksum(struct ip * pip);	

/* Checksum adjustment */
#define	_RTL8651_ADJUST_CHECKSUM(acc, cksum) \
	do { \
		acc += ntohs(cksum); \
		if (acc < 0) { \
			acc = -acc; \
			acc = (acc >> 16) + (acc & 0xffff); \
			acc += acc >> 16; \
			cksum = htons((uint16) ~acc); \
		} else { \
			acc = (acc >> 16) + (acc & 0xffff); \
			acc += acc >> 16; \
			cksum = htons((uint16) acc); \
		} \
	} while (0)

#define _RTL8651_DATA_ADJUST_CHECKSUM(data_mod, data_org, cksum) \
	do { \
		int32 accumulate; \
		accumulate = ((data_org) - (data_mod)); \
		_RTL8651_ADJUST_CHECKSUM( accumulate, (cksum)); \
	} while(0)
	
#define _RTL8651_L4_ADJUST_CHECKSUM(ip_mod, ip_org, port_mod, port_org, cksum) \
	do { \
		int32 accumulate = 0; \
		if (((ip_mod) != 0) && ((ip_org) != 0)){ \
			accumulate = ((ip_org) & 0xffff); \
			accumulate += (( (ip_org) >> 16 ) & 0xffff); \
			accumulate -= ((ip_mod) & 0xffff); \
			accumulate -= (( (ip_mod) >> 16 ) & 0xffff); \
		} \
		if (((port_mod) != 0) && ((port_org) != 0)){ \
			accumulate += (port_org); \
			accumulate -= (port_mod); \
		} \
		_RTL8651_ADJUST_CHECKSUM(accumulate, (cksum)); \
	}while(0)

#define _RTL8651_L3_ADJUST_CHECKSUM(ip_mod, ip_org, cksum) \
	do {\
		_RTL8651_L4_ADJUST_CHECKSUM((ip_mod), (ip_org), 0, 0, (cksum)); \
	}while(0)

/*********************************************************************************************************
	GRE/PPTP
**********************************************************************************************************/
#define hton8(x)  (x)
#define ntoh8(x)  (x)
#define hton16(x) htons(x)
#define ntoh16(x) ntohs(x)
#define hton32(x) htonl(x)
#define ntoh32(x) ntohl(x)

#if 0
#define PPTP_GRE_PROTO  0x880B
#define PPTP_GRE_VER    0x1

#define PPTP_GRE_FLAG_C	0x80
#define PPTP_GRE_FLAG_R	0x40
#define PPTP_GRE_FLAG_K	0x20
#define PPTP_GRE_FLAG_S	0x10
#define PPTP_GRE_FLAG_A	0x80

#define PPTP_GRE_IS_C(f) ((f)& PPTP_GRE_FLAG_C)
#define PPTP_GRE_IS_R(f) ((f)& PPTP_GRE_FLAG_R)
#define PPTP_GRE_IS_K(f) ((f)& PPTP_GRE_FLAG_K)
#define PPTP_GRE_IS_S(f) ((f)& PPTP_GRE_FLAG_S)
#define PPTP_GRE_IS_A(f) ((f)& PPTP_GRE_FLAG_A)
#endif

struct wan_gre_header {
  uint8 flags;			/* bitfield */
  uint8 ver;			/* should be PPTP_GRE_VER (enhanced GRE) */
  uint16 protocol;		/* should be PPTP_GRE_PROTO (ppp-encaps) */
  uint16 payload_len;	/* size of ppp payload, not inc. gre header */
  uint16 call_id;		/* peer's call_id for this session */
  uint32 seq;			/* sequence number.  Present if S==1 */
  uint32 ack;			/* seq number of highest packet recieved by */
  						/*  sender in this session */
};
#if 0
struct _RTL8651_PPTP_SHARE_DATA {
		uint32 seq;
		uint32 ack; 
		uint32 seq_recv;	
		uint32 seq_sent;
		uint32 ack_sent;
		uint32 ack_recv;
		
		uint32 tunnel_dst_ip;
		uint32 tunnel_src_ip;
		
		uint16 pptp_gre_call_id;
		uint16 pptp_gre_peer_call_id;
		
		uint8 reserv[128];
};
#endif

/*********************************************************************************************************
	L2TP
**********************************************************************************************************/
struct wan_l2tp_header {

  uint16 flags_type:1;
  uint16 flags_length:1;
  uint16 flags_rsv1:2;
  uint16 flags_sequence:1;
  uint16 flags_rsv2:1;
  uint16 flags_offset:1;
  uint16 flags_priority:1;  
  uint16 flags_rsv3:4;  
  uint16 flags_version:4;
  
  uint16 length;
  uint16 tunnel_id;
  uint16 session_id;  
};

/* AVP attribute type */
#define L2TP_AVP_ATTRIBUTE_CTRLMSG		0

/*
	L2TP control message type
	RFC 2661 (3.2)
*/
#define L2TP_CTRLMSG_RESV1					0	/* reserved */
#define L2TP_CTRLMSG_SCCRQ					1	/* Start-Control-Connection-Request */
#define L2TP_CTRLMSG_SCCRP					2	/* Start-Control-Connection-Reply */
#define L2TP_CTRLMSG_SCCCN					3	/* Start-Control-Connection-Connected */
#define L2TP_CTRLMSG_StopCCN				4	/* Stop-Control-Connection-Notification */
#define L2TP_CTRLMSG_RESV2					5	/* reserved */
#define L2TP_CTRLMSG_HELLO					6	/* Hello */

#define L2TP_CTRLMSG_OCRQ					7	/* Outgoing-Call-Request */
#define L2TP_CTRLMSG_OCRP					8	/* Outgoing-Call-Reply */
#define L2TP_CTRLMSG_OCCN					9	/* Outgoing-Call-Connected */
#define L2TP_CTRLMSG_ICRQ					10	/* Incoming-Call-Request */
#define L2TP_CTRLMSG_ICRP					11	/* Incoming-Call-Reply */
#define L2TP_CTRLMSG_ICCN					12	/* Incoming-Call-Connected */
#define L2TP_CTRLMSG_RESV3					13	/* reserved */
#define L2TP_CTRLMSG_CDN					14	/* Call-Disconnect-Notify */

#define L2TP_CTRLMSG_WEN					15	/* WAN-Error-Notify */

#define L2TP_CTRLMSG_SLI					16	/* Set-Link-Info */


/*********************************************************************************************************
	Port Scan
**********************************************************************************************************/
#if 0
//for ACK Scan, SYN Scan, FIN Scan
#define PORT_SCAN_WAIT_RST 2
struct portScanInfo {
	ipaddr_t sip;
	ipaddr_t dip;
	uint32 dsid;
	uint16 sport;
	uint16 dport;
	uint8 flags;
	uint8 age; 
};
#endif

/*********************************************************************************************************
	NAPT Redirect
**********************************************************************************************************/
/* time out of napt redirect cache */
#define RTL8651_NAPTREDIRECT_CACHETIMEOUT				32
#define RTL8651_NAPTREDIRECT_DEFAULT_TABLESIZE			16
#define RTL8651_NAPTREDIRECT_DEFAULT_FLOWCACHESIZE		32

/* Redirect entry */
typedef struct _rtl8651_naptRedirectEntry_s
{
	uint8	isExceptionList;
	uint8	isTcp;
	ipaddr_t	sip;
	uint16	sport;
	ipaddr_t	dip;
	uint16	dport;
	ipaddr_t	newDip;
	int32	newDport;
	/******* linked list *******/
	struct _rtl8651_naptRedirectEntry_s *next;
} _rtl8651_naptRedirectEntry_t;

/* Redirected flow */
typedef struct _rtl8651_naptRedirectFlow_s
{
	uint8	isTcp;
	ipaddr_t	sip;
	uint16	sport;
	ipaddr_t	dip;
	uint16	dport;
	ipaddr_t	newDip;
	uint16	newDport;
	uint32	age;
	uint32	queryCnt;			/* query count from protocol stack */
	/******* linked list *******/
	struct _rtl8651_naptRedirectFlow_s *next;
} _rtl8651_naptRedirectFlow_t;


/*********************************************************************************************************
	PPTP-Header Cache
**********************************************************************************************************/
#ifdef _RTL_PPTPL2TP_HDR_REFILL
typedef struct _rtl8651_pptpHdrCacheEntry_s
{
	/* <-- data to be cached --> */
	uint8 type;
	uint32 seq;
	uint32 ack;
	/* <-- key - we only support ip-in-GRE type, so we can store id/offset in ip-header as the key to be hashed --> */
	uint16 id;
	uint16 offset;
	/* <-- for linked list -->*/
	struct _rtl8651_pptpHdrCacheEntry_s **hdr;
	struct _rtl8651_pptpHdrCacheEntry_s *prev;
	struct _rtl8651_pptpHdrCacheEntry_s *next;
	struct _rtl8651_pptpHdrCacheEntry_s *lru_prev;
	struct _rtl8651_pptpHdrCacheEntry_s *lru_next;
} _rtl8651_pptpHdrCacheEntry_t;

#define RTL8651_PPTPHDR_SEQ_SET	0x01
#define RTL8651_PPTPHDR_ACK_SET	0x02

#define RTL8651_PPTPHDR_CACHE_HASH_TBLSIZE	64
#define RTL8651_PPTPHDR_CACHE_DEFAULT_ENTRYCNT	192
/* easy hash --> hash table size MUST be power of 2 */
#define RTL8651_IP_ID_EASYHASH(sip, dip, id, hashTblSize)	(((uint32)(sip)+(uint32)(dip)+(uint32)(htons(id))) & (hashTblSize-1))

void _rtl8651_pptpHdrCache_Add(uint16 ip_id, uint16 ip_off, uint32 seq, uint32 ack, uint8 type);
#endif /* _RTL_PPTPL2TP_HDR_REFILL */
/*********************************************************************************************************
	mtu
**********************************************************************************************************/
int32 _rtl8651_mtuFragment(struct rtl_pktHdr *pkthdrPtr, uint32 mtu, uint16 dvid, int8 isNapt, uint16 srcPortList);

/*********************************************************************************************************
	misc
**********************************************************************************************************/
/* hash function */
#define TWO_ITEM_EASYHASH(item1,item2,hashTblSize)		(((uint32)(item1) + (uint32)(item2)) & ((hashTblSize) - 1))
#define THREE_ITEM_EASYHASH(item1,item2,item3,hashTblSize)		(((uint32)(item1) + (uint32)(item2) + (uint32)(item3)) & ((hashTblSize) - 1))
/* fragment check */
#define DONT_FRAG(off)				(ntohs((uint16)(off)) & (IP_DF))
#define IS_FRAGMENT(off)				(ntohs((uint16)(off)) & (IP_OFFMASK|IP_MF))
#define IS_FRAGMENT_FIRST(off)		((ntohs((uint16)(off)) & (IP_OFFMASK|IP_MF)) == IP_MF)
#define IS_FRAGMENT_REMAIN(off)	(ntohs((uint16)(off)) & IP_OFFMASK)
/* ip header length related */
#define IPHDRLEN_IS_TWENTY(iphdr)		(((*(uint8*)iphdr)&0xf) == 5)					/* check IP header Length is 20 or not */
#define IPHDR_LEN(iphdr)					(((*(uint8*)iphdr)&0xf) << 2)					/* get the ip length */
#define L4HDR_ADDR(iphdr)				((int8 *)iphdr + (((*(uint8*)iphdr)&0xf) << 2))	/* calculate L4 header address by ip header */
#define _RTL8651_TTL_DEC_1(ip_ttl, ip_sum) \
	do { \
		int32 ttl_mod = 0x0100; \
		(ip_ttl) --; \
		_RTL8651_ADJUST_CHECKSUM(ttl_mod, (ip_sum)); \
	} while(0)


/* set broadcast mac */
#define BCAST_MAC(pMAC) \
		do { \
			memset(pMAC, 0xff, 6); \
		} while (0);

/* extern variables */
extern int8 rtl8651_drvMulticastProcessEnable;
extern int8 rtl8651_drvPacketFastSend;
extern int8 rtl8651_drvIcmpRoutingMsgEnable;
extern int8 rtl8651_drvSoftwareBroadcastEnable[RTL8651_VLAN_NUMBER];	/* enable/disable SW broadcast */
extern rtl8651_tblDrvFwdEngineCounts_t rtl8651_fwdEngineCounter;

inline void _rtl8651_l4TcpAdjustMss(struct ip *ip, struct tcphdr *tc, ipaddr_t intfIp, int32 delta);

/*********************************************************************************************************
	Diffserv callback function
**********************************************************************************************************/
int _rtl8651_markDiffserv( int32, struct rtl_pktHdr *, struct ip *, void * );


#endif /* RTL8651_FWDENGINE_LOCAL_H */
