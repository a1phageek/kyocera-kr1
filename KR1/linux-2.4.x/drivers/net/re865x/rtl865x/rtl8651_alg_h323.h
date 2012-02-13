/*
* Copyright c                  Realtek Semiconductor Corporation, 2002  
* All rights reserved.
* 
* Program : 
* Abstract : 
* Creator : 
* Author :  
* $Id: rtl8651_alg_h323.h,v 1.3 2004/10/04 05:08:54 tony Exp $
* $Log: rtl8651_alg_h323.h,v $
* Revision 1.3  2004/10/04 05:08:54  tony
* *: fix bug for NetMeeting Video in/out.
*
* Revision 1.2  2004/04/20 03:44:02  tony
* if disable define "RTL865X_OVER_KERNEL" and "RTL865X_OVER_LINUX", __KERNEL__ and __linux__ will be undefined.
*
* Revision 1.1  2004/02/25 14:26:33  chhuang
* *** empty log message ***
*
* Revision 1.2  2004/02/25 14:24:52  chhuang
* *** empty log message ***
*
* Revision 1.8  2004/01/15 07:18:19  tony
* support incoming call for H323 ALG.
*
* Revision 1.7  2004/01/09 08:03:21  tony
* make the code architecture of ALG is able to support multiple dial session.
*
* Revision 1.6  2003/10/23 07:09:41  hiwu
* arrange include file sequence
*
* Revision 1.5  2003/10/20 05:12:06  hiwu
* add new h245 alg support
*
* Revision 1.4  2003/10/17 08:40:29  hiwu
* merge new function type
*
* Revision 1.3  2003/10/02 10:39:52  hiwu
* fix header conflict problem
*
* Revision 1.2  2003/10/02 10:01:16  hiwu
* fix h323 tcp parser bug
*
* Revision 1.1  2003/10/02 07:08:11  hiwu
* initial version
*
*/

#ifndef _RTL8651_ALG_H323
#define _RTL8651_ALG_H323

int32 rtl8651_l4AliasH323Init(void *);
int32 rtl8651_l4AliasHandleClientH225In(struct rtl_pktHdr *, struct ip *,struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s *);
int32 rtl8651_l4AliasHandleClientH225Out(struct rtl_pktHdr *, struct ip *,struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s *);		
int32 rtl8651_l4AliasHandleClientH245Out(struct rtl_pktHdr *, struct ip *,	struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s *);
int32 rtl8651_l4AliasHandleClientH245In(struct rtl_pktHdr *, struct ip *,	struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s *);

int32 rtl8651_l4AliasHandleServerH225Out(struct rtl_pktHdr *, struct ip *,struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s *);
int32 rtl8651_l4AliasHandleServerH225In(struct rtl_pktHdr *, struct ip *,struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s *);
int32 rtl8651_l4AliasHandleServerH245Out(struct rtl_pktHdr *, struct ip *,	struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s *);
int32 rtl8651_l4AliasHandleServerH245In(struct rtl_pktHdr *, struct ip *,	struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s *);


#endif /* _RTL8651_ALG_H323 */
