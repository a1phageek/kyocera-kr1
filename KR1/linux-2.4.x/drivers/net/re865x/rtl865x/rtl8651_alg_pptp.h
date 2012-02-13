/*
* Copyright c                  Realtek Semiconductor Corporation, 2002  
* All rights reserved.
* 
* Program : 
* Abstract : 
* Creator : 
* Author :  
* $Id: rtl8651_alg_pptp.h,v 1.4 2004/07/21 12:45:51 tony Exp $
* $Log: rtl8651_alg_pptp.h,v $
* Revision 1.4  2004/07/21 12:45:51  tony
* +: add new features: PPTP Pass-through support server in.
*
* Revision 1.3  2004/06/24 03:24:21  jzchen
* +: Add dump function for command line debugging purpose
*
* Revision 1.2  2004/04/20 03:44:03  tony
* if disable define "RTL865X_OVER_KERNEL" and "RTL865X_OVER_LINUX", __KERNEL__ and __linux__ will be undefined.
*
* Revision 1.1  2004/02/25 14:26:33  chhuang
* *** empty log message ***
*
* Revision 1.2  2004/02/25 14:24:52  chhuang
* *** empty log message ***
*
* Revision 1.2  2004/01/09 08:03:21  tony
* make the code architecture of ALG is able to support multiple dial session.
*
* Revision 1.1  2003/10/23 09:01:43  hiwu
* initial version
*
*
*/

#ifndef _RTL8651_ALG_PPTP
#define _RTL8651_ALG_PPTP

int32 rtl8651_l4AliasPptpInit(void *);
int32 rtl8651_l4AliasHandlePptpClientInbound(struct rtl_pktHdr *, struct ip *,struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s *);
int32 rtl8651_l4AliasHandlePptpClientOutbound(struct rtl_pktHdr *, struct ip *,struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s *);
int32 rtl8651_l4AliasHandlePptpServerInbound(struct rtl_pktHdr *, struct ip *,struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s *);
int32 rtl8651_l4AliasHandlePptpServerOutbound(struct rtl_pktHdr *, struct ip *,struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s *);

void  rtl8651_l4AliasPptpHouseKeeping(void);
void rtl8651_l4AliasPptpDump(void);

/* GRE NAPT plugin function */
extern int32 (*rtl8651_l4GreNaptAliasIn)(struct rtl_pktHdr *, struct ip *);
extern int32 (*rtl8651_l4GreNaptAliasOut)(struct rtl_pktHdr *, struct ip *);

#endif /* _RTL8651_ALG_PPTP */
