/*
* Copyright c                  Realtek Semiconductor Corporation, 2004
* All rights reserved.
* 
* Program : Counter-Strike / Half life
* Abstract : 
* Creator : 
* Author :
* $Id: rtl8651_alg_cs.h,v 1.3 2004/04/20 03:44:02 tony Exp $
* $Log: rtl8651_alg_cs.h,v $
* Revision 1.3  2004/04/20 03:44:02  tony
* if disable define "RTL865X_OVER_KERNEL" and "RTL865X_OVER_LINUX", __KERNEL__ and __linux__ will be undefined.
*
* Revision 1.2  2004/03/18 09:22:26  tony
* add alg reinit function, it will be called when fwdEng ReInit.
*
* Revision 1.1  2004/03/12 07:34:07  tony
* add new ALG for Counter-Strike / Half life
*
*
*
*/

#ifndef _RTL8651_ALG_CS
#define _RTL8651_ALG_CS

int32 rtl8651_l4AliasHandleCSClientOutbound(struct rtl_pktHdr *, struct ip *,struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s *);
void rtl8651_l4AliasHandleCsReInit(void);

#endif /* _RTL8651_ALG_CS */
