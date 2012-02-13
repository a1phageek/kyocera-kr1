#ifndef RTL8651_LAYER2_H
#define RTL8651_LAYER2_H

#define RTL8651_MAC_NUMBER				6
#define RTL8651_PORT_NUMBER				RTL8651_MAC_NUMBER
#define RTL8651_MII_PORTNUMBER                 	5
#define RTL8651_MII_PORTMASK                    	0x20
#define RTL8651_PHY_NUMBER				5
#define RTL8651_AGGREGATOR_NUMBER		(RTL8651_PORT_NUMBER+rtl8651_totalExtPortNum)
#define RTL8651_ALLPORTMASK				((1<<RTL8651_AGGREGATOR_NUMBER)-1)
#define RTL8651_PHYSICALPORTMASK			((1<<RTL8651_MAC_NUMBER)-1)
#define RTL8651_L2TBL_ROW					256
#define RTL8651_L2TBL_COLUMN				4
#define RTL8651_MAX_AGGREGATION_NUM		1
#define RTL8651_VLAN_NUMBER				8
#define RTL8651_FDB_NUMBER				1
#define RTL8651_STI_NUMBER				RTL8651_FDB_NUMBER
#define RTL8651_PROTOCOLTRAP_SIZE			8
#define RTL8651_HARDWIRED_PROTOTRAP_SIZE	8

#define RTL8651_ETHER_AUTO_100FULL	0x00
#define RTL8651_ETHER_AUTO_100HALF	0x01
#define RTL8651_ETHER_AUTO_10FULL	0x02
#define RTL8651_ETHER_AUTO_10HALF	0x03

#define RTL8651_FORWARD_MAC		0x00
#define RTL8651_DSTBLOCK_MAC	0x01
#define RTL8651_SRCBLOCK_MAC	0x02
#define RTL8651_TOCPU_MAC		0x03

/* Spanning Tree Port State Definition */
#define RTL8651_PORTSTA_DISABLED		0x00
#define RTL8651_PORTSTA_BLOCKING		0x01
#define RTL8651_PORTSTA_LISTENING	0x02
#define RTL8651_PORTSTA_LEARNING		0x03
#define RTL8651_PORTSTA_FORWARDING	0x04

/* Link Layer type Definition */
#define RTL8651_LL_NONE				0x00
#define RTL8651_LL_VLAN				0x01
#define RTL8651_LL_PPPOE				0x02
#define RTL8651_LL_WIPUNNUMBERED		0x03
#define RTL8651_LL_LIPUNNUMBERED		0x04
#define RTL8651_LL_PPTP				0x05
#define RTL8651_LL_L2TP				0x06



/* Protocol Trapping type Definition */
#define RTL8651_PROTOTRAP_ETHER		0x00
#define RTL8651_PROTOTRAP_IP		0x01
#define RTL8651_PROTOTRAP_TCPPORT	0x02
#define RTL8651_PROTOTRAP_UDPPORT	0x03


/* WAN type=PPPoE, short UDP packet will cause NIC descriptor ring damage.
 * set EXTPORT_PPPoE_UDP_PATCH=1: use cfliu's patch, trap UDP packet to CPU.
 * set EXTPORT_PPPoE_UDP_PATCH=2: use yjlou's patch, enable VLAN tag for Extension Port. */
#define EXTPORT_PPPoE_UDP_PATCH 2 




extern uint32 rtl8651_tblDrvCpuMirrorPortMask;
extern int32 rtl8651_totalExtPortNum; //this replaces all rtl8651_totalExtPortNum defines
extern int32 rtl8651_allExtPortMask; //this replaces all RTL8651_EXTPORTMASK defines
extern int32 rtl8651_bridgeEnable;

// phy-port
//when auto-eng is enabled, use negotiated capability and specified max capability to determine what link speed and duplex mode to use.
//when auto-neg is off, the speed and duplex mode given is forced to write to ASIC
int32 rtl8651_installPortStatusChangeNotifier(void (*notifier)(uint32 port, int8 linkUp));
int32 rtl8651_getEthernetPortLinkStatus(uint32 port, int8 * linkUp, uint16 *speed, int8 *fullduplex, int8 *autoNeg);
int32 rtl8651_setEthernetPortDuplexMode(uint32 port, int8 fullDuplex);
int32 rtl8651_setEthernetPortSpeed(uint32 port, uint32 speed);
int32 rtl8651_setEthernetPortAutoNegotiation(uint32 port, int8 autoNegotiation, uint32 advCapability);
int32 rtl8651_setEthernetPortBandwidthControl(uint32 port, int8 input, uint32 rate);


// spanning-tree 
//not yet tested when protocol work is on. protocol working default disabled so port state won't change dynamically.
int32 rtl8651_addSpanningTreeInstance(uint16 sid);
int32 rtl8651_setSpanningTreeInstanceProtocolWorking(uint16 sid, int8 working);
int32 rtl8651_delSpanningTreeInstance(uint16 sid);
int32 rtl8651_setSpanningTreeInstancePortState(uint16 stid, uint16 port, uint8 portState);

//filter-database 
int32 rtl8651_addFilterDatabase(uint16 fid);
int32 rtl8651_delFilterDatabase(uint16 fid);
int32 rtl8651_specifyFilterDatabaseSpanningTreeInstance(uint16 fid, uint16 sid);
int32 rtl8651_addFilterDatabaseEntry(uint16 fid, ether_addr_t * macAddr, uint32 type, uint32 portMask);
int32 rtl8651_delFilterDatabaseEntry(uint16 fid, ether_addr_t * macAddr);
int32 rtl8651_installFDBEntryChangeNotifier( void (*notify)(void *l2entry_t,void *arg));


// port 
//this 'port' is actually a logical port(=aggregator), that is, not physical port number. 
//RTL8651 support only one aggregator group. port VID is binded on logical port
int32 rtl8651_setAggregatorIndividual(uint16 aggregator, int8 individual);
int32 rtl8651_setPortAggregator(uint32 port, uint16 aggregator);
int32 rtl8651_getAggregatorActiveStatus(uint16 aggregator, int8 * isActive);
int32 rtl8651_setPvid(uint32 aggregator, uint16 vid);

//vlan 
//allocate a VLAN, together with ingress/egress ACL rule space allocated.
int32 rtl8651_addVlan(uint16 vid);
//free ACL rules allocated and reset member port's pvid to default VID
int32 rtl8651_delVlan(uint16 vid);
//valid mac addr num is 1,2,4,8 (0 is not allowed, all VLANs need at least one MAC)
int32 rtl8651_allocateVlanMacAddress(uint16 vid, uint16 macAddrNumber);
int32 rtl8651_getVlanMacAddress(uint16 vid, ether_addr_t * macAddr, uint16 * macAddrNumber);
//if user wants to assign MAC addresses, ( last byte of MAC address given ) must be multiple of  (macAddrNumber of this VLAN ) 
int32 rtl8651_assignVlanMacAddress(uint16 vid, ether_addr_t * macAddress, uint16 macAddrNumber);
int32 rtl8651_delVlanPortMember(uint16 vid, uint32 port);
int32 rtl8651_addVlanPortMember(uint16 vid, uint32 port);
int32 rtl8651_specifyVlanFilterDatabase(uint16 vid, uint16 fid);
//when a vlan is in promiscuous mode, no network/IP interface can be attached on this VLAN and routing to/from
//this VLAN is automatically disabled.
int32 rtl8651_setVlanPromiscuous(uint16 vid, int8 isPromiscuous);
int32 rtl8651_setVlanFwdTxMirror(uint16 vid, int8 isFwdTxMirror);
int32 rtl8651_getVlanActiveStatus(uint16 vid, int8 * isActive);
int32 rtl8651_setVlanPortUntag(uint16 vid, uint32 port, int8 untag);



int32 rtl8651_pureSoftwareFwd(int8 isSoftFwd);
//Protocol Trap
int32 rtl8651_addProtoTrap(uint8 protoType, uint16 protoContent);
int32 rtl8651_delProtoTrap(uint8 protoType, uint16 protoContent);
//Misc.
int32 rtl8651_enableBroadCastStormPrevention(int8 enable);
struct rtl_pktHdr;
struct rtl_mBuf;
void rtl8651_installArpAgent(int32  (*arpAgent)(struct rtl_pktHdr *pkthdrPtr,struct rtl_mBuf *mbufPtr,uint16 vid));

int32 rtl8651_resetRxTxMirror(void);
int32 rtl8651_addRxTxMirror(int8 isRxMirror, uint32 port);
int32 rtl8651_addMirrorPort(uint32 port);
int32 rtl8651_delMirrorPort(uint32 port);

extern uint32 rtl8651_l2protoPassThrough;
#define PASSTHRU_PPPOE			(1<<0)
#define PASSTHRU_IPV6			(1<<1)
#define PASSTHRU_IPX			(1<<2)
#define PASSTHRU_NETBIOS		(1<<3)

extern int8 rtl8651_drvDropUnknownPppoePADT;
extern struct ether_header rtl8651_knownPppoePassthruEthHdr;

#endif
