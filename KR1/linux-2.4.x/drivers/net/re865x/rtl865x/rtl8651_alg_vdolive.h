/*
* Copyright c                  Realtek Semiconductor Corporation, 2004
* All rights reserved.
* 
* Program : VDOlive
* Abstract : 
* Creator : 
* Author :
* $Id: rtl8651_alg_vdolive.h,v 1.2 2004/04/20 03:44:03 tony Exp $
* $Log: rtl8651_alg_vdolive.h,v $
* Revision 1.2  2004/04/20 03:44:03  tony
* if disable define "RTL865X_OVER_KERNEL" and "RTL865X_OVER_LINUX", __KERNEL__ and __linux__ will be undefined.
*
* Revision 1.1  2004/03/12 07:36:11  tony
* add new ALG for VDOlive
*
*
*/

#ifndef _RTL8651_ALG_VDOLIVE
#define _RTL8651_ALG_VDOLIVE

int32 rtl8651_l4AliasHandleVDOliveClientOutbound(struct rtl_pktHdr *, struct ip *,struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s *);

#endif /* _RTL8651_ALG_VDOLIVE */
