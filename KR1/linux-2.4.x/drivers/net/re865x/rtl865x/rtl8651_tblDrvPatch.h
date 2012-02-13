/*
* Copyright c                  Realtek Semiconductor Corporation, 2002  
* All rights reserved.
* 
* Program : Patching Switch core table driver 
* Abstract : 
* Author : chih-hua huang (chhuang@realtek.com.tw)               
* $Id: rtl8651_tblDrvPatch.h,v 1.4 2004/07/28 14:02:39 yjlou Exp $
* $Log: rtl8651_tblDrvPatch.h,v $
* Revision 1.4  2004/07/28 14:02:39  yjlou
* +: convert_getAsicNaptTcpUdpTable() supports HASH2
*
* Revision 1.3  2004/04/02 06:41:52  chhuang
* rollback RTL8650B compiler flag
*
* Revision 1.2  2004/03/03 10:40:38  yjlou
* *: commit for mergence the difference in rtl86xx_tbl/ since 2004/02/26.
*
* Revision 1.1  2004/02/25 14:26:33  chhuang
* *** empty log message ***
*
* Revision 1.2  2004/02/25 14:24:52  chhuang
* *** empty log message ***
*
* Revision 1.4  2004/02/18 13:54:37  chhuang
* *** empty log message ***
*
* Revision 1.3  2003/09/30 06:07:50  orlando
* check in RTL8651BLDRV_V20_20030930
*
* Revision 1.4  2003/07/16 06:32:15  cfliu
* Retire L2/L4 table write patch APIs
*
* Revision 1.3  2003/06/11 02:34:58  jzchen
* Change napt sec parameter from uint16 to uint32
*
* Revision 1.2  2003/05/16 09:09:03  cfliu
* Add 2 missing function prototype
*
* Revision 1.1  2003/05/07 08:46:24  chhuang
* patch hardward bug:
* napt and arp aging time reset/downcount by asic
*
*
*/


#ifndef RTL8651_TBLDRV_PATCH_H
#define RTL8651_TBLDRV_PATCH_H

#include "rtl8651_tblDrvLocal.h"


#if 0
int32 rtl8651_setAsicNaptTcpUdpTable_Patch(int8 forced, ipaddr_t sip, uint16 sport, uint16 gport,
		uint32 ageSec, int8 isStatic, int8 isTcp, int8 tcpFlag, int8 isColl, int8 isValid) ;
int32 rtl8651_setAsicNaptIcmpTable_Patch(int8 forced, ipaddr_t sip, uint16 sID, uint16 gID, 
		uint32 ageSec, int8 isStatic, int16 count, int8 isColl, int8 isValid);
int32 rtl8651_getAsicNaptIcmpTable_Patch(int8 preID, uint16 tgID, ipaddr_t *sip, uint16 *sID, 
		uint16 *gID, uint32 *ageSec, int8 *entryType, uint16 *count, int8 *isColl, int8 *isValid);
#endif

#define __INLINE__ inline


__INLINE__ int32 RTL8651_SETASICVLAN( rtl8651_tblDrv_vlanTable_t *vt );
__INLINE__ int32 convert_getAsicVlan( uint16 vid, ether_addr_t * mac, uint32 * mbr,
                                      uint32 * inAclStart, uint32 * inAclEnd, uint32 * outAclStart,
                                      uint32 * outAclEnd, int8 * internalIntf, int8 * enableRoute,
                                      int8 *portState, int8 * broadcastToCpu, int8 * promiscuous, 
                                      uint32 * untagPortMask, uint32 * macNumber, uint32 * mtu );

__INLINE__ int32 convert_setAsicPppoe( uint32 index, uint16 sid );
__INLINE__ int32 convert_getAsicPppoe( uint32 index, uint16 *sid );

__INLINE__ int32 convert_setAsicExtIntIpTable( uint32 ipIdx, ipaddr_t ExtIp, ipaddr_t IntIp,
                                   uint32 localPublic, uint32 nat);
__INLINE__ int32 convert_getAsicExtIntIpTable( uint32 index, ipaddr_t *ExtIp, ipaddr_t *IntIp, 
                                               int8 *localPublic, int8 *nat );

__INLINE__ int32 convert_setAsicArp( uint32 ArpPos, uint32 ArpIdx, uint32 asicPos );
__INLINE__ int32 convert_getAsicArp( uint32 index, uint32 *nextHopRow, uint32 *nextHopColumn );

__INLINE__ int32 convert_setAsicRouting( uint32 idx, ipaddr_t ipAddr, ipaddr_t ipMask,
                                         uint32 process, uint32 vidx, uint32 arpStart, uint32 arpEnd,
                                         uint32 nextHopRow, uint32 nextHopColumn, uint32 pppoeIdx );
__INLINE__ int32 convert_getAsicRouting( uint32 index, ipaddr_t * ipAddr, ipaddr_t * ipMask, uint32 * process, //0: pppoe, 1:direct, 2:indirect, 4:Strong CPU, 
                                         uint32 * vidx, uint32 * arpStart, uint32 * arpEnd, 
                                         uint32 * nextHopRow, uint32 * nextHopColumn, uint32 * pppoeIdx) ;

__INLINE__ int32 convert_setAsicServerPortTable( uint32 index, ipaddr_t ExtIp, uint16 ExtPort,
                                                 ipaddr_t IntIp, uint16 IntPort );
__INLINE__ int32 convert_getAsicServerPortTable( uint32 index, ipaddr_t *ExtIp, uint16 *ExtPort,
                                                 ipaddr_t *IntIp, uint16 *IntPort );

__INLINE__ int32 convert_setAsicNaptTcpUdpTable( int8 forced,
                                                 ipaddr_t IntIp, uint16 IntPort, uint16 ExtPort,
                                                 uint32 ageSec, int8 entryType, int8 isTcp, int8 tcpFlag, 
                                                 int8 isCollision, int8 isCollision2, int8 isValid );
__INLINE__ int32 convert_getAsicNaptTcpUdpTable( uint8 *extIpIdx, uint16 index,
                                                 ipaddr_t *intIPAddr, uint16 *intPort, uint16 *extPort,
                                                 uint32 *ageSec,  int8 * entryType, int8 *isTcp, int8 *flags,
                                                 int8 *isCollision, int8 *isValid,int8 *isCollision2,
                                                 int8 *isDedicated, uint16 *selEIdx, uint8 *selExtIpIdx );

__INLINE__ int32 convert_setAsicNaptIcmpTable(int8 forced,
                ipaddr_t IntIp, uint16 IntId, uint16 ExtId,
                uint32 ageSec, int8 entryType, int16 count, int8 isCollision, int8 isValid );
__INLINE__ int32 convert_getAsicNaptIcmpTable( int8 precisePort, uint16 tarId,
                                               ipaddr_t *IntIp, uint16 *IntId,
                                               uint16 *ExtId, uint32 *ageSec, int8 *entryType,
                                               uint16 *count, int8 *isCollision, int8 *isValid );

__INLINE__ int32 convert_setAsicAlg( uint32 index, uint16 port );
__INLINE__ int32 convert_getAsicAlg( uint32 index, uint16 *port );

__INLINE__ int32 convertt_setAsicProtoTrap( uint32 index, uint8 type, uint16 content );
__INLINE__ int32 convert_getAsicProtoTrap( uint32 index, uint8 *type, uint16 *content);

__INLINE__ int32 convert_setAsicL2Table(uint32 row, uint32 column, ether_addr_t * mac, int8 cpu, 
		int8 srcBlk, uint32 mbr, uint32 ageSec, int8 isStatic, int8 nhFlag);
		


int32 rtl8651_setAsicL2Table_Patch(uint32 row, uint32 column, ether_addr_t * mac, int8 cpu, 
		int8 srcBlk, uint32 mbr, uint32 ageSec, int8 isStatic, int8 nhFlag);
//int32 rtl8651_getAsicL2Table_Patch(uint32 row, uint32 column, ether_addr_t * mac, int8 * cpu, 
//		int8 * srcBlk, int8 * isStatic, uint32 * mbr, uint32 * ageSec, int8 *nhFlag);
int32 rtl8651_getAsicL2Table_Patch(uint32 row, uint32 column, rtl865x_tblAsicDrv_l2Param_t *asic_l2_t);


#endif /* RTL8651_TBLDRV_PATCH_H */


