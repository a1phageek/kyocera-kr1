
/*
* Copyright c                  Realtek Semiconductor Corporation, 2004
* All rights reserved.
* 
* Program : RealAudio UDP data channel
* Abstract : 
* Creator : 
* Author :
* $Id: rtl8651_alg_rtsp.h,v 1.1 2004/09/30 14:13:27 chhuang Exp $
* $Log: rtl8651_alg_rtsp.h,v $
* Revision 1.1  2004/09/30 14:13:27  chhuang
* +:add rtsp
*
* Revision 1.1  2004/09/14 13:24:29  chhuang
* +: RTSP alg support
*
*
*/

#ifndef _RTL8651_ALG_RTSP
#define _RTL8651_ALG_RTSP



int32 rtl8651_l4AliasHandleRTSPClientOutbound(struct rtl_pktHdr *, struct ip *,struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s *);
int32 rtl8651_l4AliasHandleRTSPClientInbound(struct rtl_pktHdr *, struct ip *,struct rtl8651_tblDrv_naptTcpUdpFlowEntry_s *);
void rtl8651_l4AliasRtspHouseKeeping(void);
int32 rtl8651_l4AliasRTSPInit(void *arg);

#endif /* _RTL8651_ALG_RTSP */
