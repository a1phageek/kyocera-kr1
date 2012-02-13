/*
 * Copyright c                Realtek Semiconductor Corporation, 2003
 * All rights reserved.                                                    
 * 
 * $Header: /home/cvsroot/uClinux-dist/user/goahead-2.1.4/LINUX/rome_asp.c,v 1.193 2004/08/13 06:02:51 cfliu Exp $
 *
 * $Author: cfliu $
 *
 * Abstract:
 *
 *   rome_asp.c : rome_asp
 *
 * $Log: rome_asp.c,v $
 * Revision 1.193  2004/08/13 06:02:51  cfliu
 * +: Rewrite WLAN pages for RTL8185
 *
 * Revision 1.192  2004/08/11 07:28:22  tony
 * +: new features: support quickcam web camera.
 *
 * Revision 1.191  2004/08/04 14:51:18  yjlou
 * +: support booting from single 8MB/16MB flash (_SUPPORT_LARGE_FLASH_)
 *
 * Revision 1.190  2004/07/27 13:38:29  tony
 * *: make pppoe support hang up when connecting...
 *
 * Revision 1.189  2004/07/27 07:23:52  chenyl
 * +: DoS ignore type setting
 *
 * Revision 1.188  2004/07/23 09:29:14  cfliu
 * +: Add WLAN access control webpage
 *
 * Revision 1.187  2004/07/21 12:46:18  tony
 * +: add new features: PPTP Pass-through support server in mode.
 *
 * Revision 1.186  2004/07/18 11:27:10  chhuang
 * *: modify M-N-1 Queue's web page
 *
 * Revision 1.185  2004/07/16 11:47:20  tony
 * *: make PPTP/L2TP redial usable.
 *
 * Revision 1.184  2004/07/15 13:16:01  cfliu
 * +: Update Webpage message
 *
 * Revision 1.183  2004/07/14 14:27:31  chenyl
 * *: modify interface
 *
 * Revision 1.182  2004/07/14 13:55:59  chenyl
 * +: web page for MN queue
 *
 * Revision 1.181  2004/07/12 11:22:21  cfliu
 * +:Update Webpage messages
 *
 * Revision 1.180  2004/07/12 07:35:32  chhuang
 * *: modify rate limit web page
 *
 * Revision 1.179  2004/07/12 07:03:47  tony
 * +: add wan type dhcpl2tp.
 *
 * Revision 1.178  2004/07/12 05:56:31  cfliu
 * *: correct OFDM set error
 *
 * Revision 1.177  2004/07/12 04:25:36  chenyl
 * *: extend existing port scan mechanism
 *
 * Revision 1.176  2004/07/11 11:28:54  cfliu
 * +: Update WDS and Wireless access control webpage
 *
 * Revision 1.175  2004/07/08 10:51:41  tony
 * *: change ipUp routine to new arch.
 *
 * Revision 1.174  2004/07/07 05:56:39  tony
 * *: fix tabid bug.
 *
 * Revision 1.173  2004/07/07 05:12:36  chhuang
 * +: add a new WAN type (DHCP+L2TP). But not complete yet!!
 *
 * Revision 1.172  2004/07/06 06:20:12  chhuang
 * +: add rate limit
 *
 * Revision 1.171  2004/07/05 11:58:44  tony
 * *: modify UI control for UPNPD.
 *
 * Revision 1.170  2004/07/05 03:39:38  jzchen
 * +: New bandwidth control may configured fail due to rate range limitaion, must check the configure success and then new setting can be accepted
 * +: Extend port bandwidth control range to 16M and 32M
 *
 * Revision 1.169  2004/07/02 10:30:08  chenyl
 * *: bug fix: per-source Flooding check bug (counter error)
 * *: bug fix: ip-spoofing ACL add function error
 * *: bug fix: ICMP ping-of-death : threshold from 65500 to 65535 (follows cisco)
 * *: bug fix: item name change of DOS check: UDP Bomb -> UDP EchoChargen (follows cisco)
 * +: new feature: New DoS check item, UDP Bomb
 * 		- Triggered if (Length of UDP header) < (Length of IP header)
 * 		- allow (Length of UDP header) > (Length of IP header)
 * 		- follows cisco
 *
 * Revision 1.168  2004/06/29 16:16:06  cfliu
 * +: define USERAPP_NOMMU only when MMU is not enabled
 *
 * Revision 1.167  2004/06/29 09:42:31  cfliu
 * +: Add auto probe WLAN card mechanism.
 *
 * Revision 1.166  2004/06/29 03:38:34  yjlou
 * *: fixed the bug of free-style flash editor: support multi-block update.
 *
 * Revision 1.165  2004/06/28 13:57:34  yjlou
 * +: support Free-style flash editor
 *
 * Revision 1.164  2004/06/18 08:11:33  tony
 * +: add new features: SYN,FIN,ACK Port Scan.
 *
 * Revision 1.163  2004/06/16 04:42:18  tony
 * *: support reboot and get loader version for MMU.
 *
 * Revision 1.162  2004/06/15 13:06:02  tony
 * *: fix log bug, for mmu model.
 *
 * Revision 1.161  2004/06/15 10:08:46  tony
 * *: using share memory instead of direct access in syslog.
 *
 * Revision 1.160  2004/06/14 06:15:24  chenyl
 * +: USERAPP_NOMMU for upnp application
 * +: _UPNP_MM_ for upnp memory management
 *
 * Revision 1.159  2004/06/11 10:53:59  cfliu
 * +: Add Cable Meter and Interface counter webpage
 *
 * Revision 1.158  2004/06/11 02:06:45  cfliu
 * +: Add NAT throughput webpage
 *
 * Revision 1.157  2004/06/11 00:44:48  cfliu
 * +: Add port ON/Off webpage
 *
 * Revision 1.156  2004/06/10 14:36:03  cfliu
 * +: Add port config webpage
 *
 * Revision 1.155  2004/06/10 10:34:52  tony
 * +: add PPTP dial status: disconnecting, make redial function correct.
 *
 * Revision 1.154  2004/06/10 02:20:21  tony
 * +: add compiler flag for pptp/l2tp.
 *
 * Revision 1.153  2004/06/09 12:32:16  tony
 * +: add PPTP/L2TP UI page.(Dial and Hang_Up).
 *
 * Revision 1.152  2004/06/08 10:57:20  cfliu
 * +: Add WLAN dual mode webpages. Not yet completed...
 *
 * Revision 1.151  2004/06/02 12:51:46  tony
 * make PPPoE/L2TP is able to run multi-times in single process.
 * and fix pppoe can't run dial-on-demand problem.
 *
 * Revision 1.150  2004/05/28 09:49:16  yjlou
 * +: support Protocol-Based NAT
 *
 * Revision 1.149  2004/05/27 05:19:17  cfliu
 * update WLAN setting command
 *
 * Revision 1.148  2004/05/27 05:12:36  tony
 * fix multiple pppoe dial problem.
 *
 * Revision 1.147  2004/05/26 13:25:31  tony
 * fix pppd idle timeout bug.(for new pppd)
 *
 * Revision 1.146  2004/05/26 10:11:55  tony
 * make pppoe daemon always run at the same process.
 *
 * Revision 1.145  2004/05/24 02:19:58  chhuang
 * bug fixed for url-filtering
 *
 * Revision 1.144  2004/05/22 03:21:58  chenyl
 * * bug fix of web page setting/initial setting
 *
 * Revision 1.143  2004/05/21 12:08:50  chenyl
 * *: TCP/UDP/ICMP spoof -> check for (sip) == (dip)
 * *: modify DOS prevention webpage
 *
 * Revision 1.142  2004/05/20 08:43:55  chhuang
 * add Web Page for QoS
 *
 * Revision 1.141  2004/05/19 08:42:10  orlando
 * init button related change
 *
 * Revision 1.140  2004/05/19 06:25:26  cfliu
 * Highlight ASIC hardware features.
 *
 * Revision 1.139  2004/05/17 12:17:28  cfliu
 * Add AC Name field
 *
 * Revision 1.138  2004/05/17 07:22:17  tony
 * fix pppoe silent timeout bug and make PPTP/L2TP support silent timeout.
 *
 * Revision 1.137  2004/05/14 10:39:32  chenyl
 * *: fix bugs of system log
 *
 * Revision 1.136  2004/05/13 13:27:01  yjlou
 * +: loader version is migrated to "00.00.07".
 * +: new architecture for INTEL flash (code is NOT verified).
 * *: FLASH_BASE is decided by IS_REV_A()
 * -: remove flash_map.h (content moved to flashdrv.h)
 * -: remove un-necessary calling setIlev()
 *
 * Revision 1.135  2004/05/12 08:09:56  chhuang
 * +: dhcp static binding
 *
 * Revision 1.134  2004/05/12 07:20:15  chenyl
 * +: source ip blocking
 * *: modify dos mechanism
 *
 * Revision 1.133  2004/05/12 05:15:05  tony
 * support PPTP/L2TP in RTL865XB.
 *
 * Revision 1.132  2004/05/10 05:50:19  chhuang
 * 1: add src ip blocking for url-filtering
 * 2: bug fixed for logging
 *
 * Revision 1.131  2004/05/05 08:27:07  tony
 * new features: add remote management spec
 *
 * Revision 1.130  2004/04/13 08:25:02  tony
 * set the limit MTU size in PPTP/L2TP mode is 1452.
 *
 * Revision 1.129  2004/04/08 13:18:12  tony
 * add PPTP/L2TP routine for MII lookback port.
 *
 * Revision 1.128  2004/04/08 12:29:39  cfliu
 * Add WDS support on extension port API
 *
 * Revision 1.127  2004/03/31 01:59:36  tony
 * add L2TP wan type UI pages.
 *
 * Revision 1.126  2004/03/21 07:32:00  cfliu
 * no message
 *
 * Revision 1.125  2004/03/19 09:51:18  tony
 * make the 'lan permit' acl is able to select by user.
 *
 * Revision 1.124  2004/03/12 08:23:22  tony
 * add new ALGs.
 *
 * Revision 1.123  2004/03/03 03:43:32  tony
 * add static routing table in turnkey.
 *
 * Revision 1.122  2004/02/27 05:14:11  tony
 * fix ACL bug: when set IP to 0.0.0.0 and port=0, ACL will drop all packets which go to external interface.
 *
 * Revision 1.121  2004/02/11 08:49:19  tony
 * modify UI: when LAN Type is Unnumbered IP, turn Destination Network off.
 *
 * Revision 1.120  2004/02/06 11:42:35  tony
 * fix bug: mIRC don't list in ALG UI.
 *
 * Revision 1.119  2004/02/05 07:11:16  tony
 * add acl filter field: IP.
 *
 * Revision 1.118  2004/02/04 08:35:52  tony
 * add field Unnumbered IP Address , Unnumbered Network Mask  in multiple pppoe mode.
 *
 * Revision 1.117  2004/02/04 06:23:49  rupert
 * Add UDP Block Support
 *
 * Revision 1.116  2004/02/04 02:30:50  tony
 * when addDmzHost failed, disable the function in UI.
 *
 * Revision 1.115  2004/02/04 02:10:30  tony
 * fix bug: lanType only be used in multiple pppoe mode.
 *
 * Revision 1.114  2004/02/03 09:17:33  tony
 * add field "LAN Type" in multiple PPPoE.
 *
 * Revision 1.113  2004/02/03 08:14:34  tony
 * add UDP Blocking web UI configuration.
 *
 * Revision 1.112  2004/01/30 12:03:05  tony
 * make Special Application is able to support multiple session.
 *
 * Revision 1.111  2004/01/30 02:53:09  orlando
 * turnon setDosThreshold() in asp_setDos()
 *
 * Revision 1.110  2004/01/29 10:56:08  orlando
 * turnon dos related settings
 *
 * Revision 1.109  2004/01/29 09:46:33  tony
 * modify serverp_tableDriverAccess(), make it support multiple session.
 * add protocol type in server port UI.
 *
 * Revision 1.108  2004/01/28 07:58:52  tony
 * add ICUII ALG.
 *
 * Revision 1.107  2004/01/16 12:16:46  tony
 * modify ALG cfg params and web UI,
 * rearrange rtl8651a_setAlgStatus() must called after rtl8651_addIpIntf()
 *
 * Revision 1.106  2004/01/14 03:37:18  tony
 * Support single session in rtl8651_setAlgStatus (for old user)
 *
 * Revision 1.105  2004/01/14 02:48:40  hiwu
 * add PPTP configuration
 *
 * Revision 1.104  2004/01/12 09:49:27  orlando
 * update asp_setUrlFilter()
 *
 * Revision 1.103  2004/01/09 09:03:24  tony
 * change rtl8651_add(del)DmzHost  to rtl8651a_add(del)DmzHost
 *
 * Revision 1.102  2004/01/09 08:59:57  tony
 * change define
 *
 * Revision 1.101  2004/01/09 08:03:55  tony
 * make the code architecture of ALG is able to support multiple dial session.
 *
 * Revision 1.100  2004/01/08 13:26:02  orlando
 * url/dos/log related changes
 *
 * Revision 1.99  2004/01/08 12:13:44  tony
 * add Port Range into Server Port.
 * support Server Port for multiple session UI.
 *
 * Revision 1.98  2004/01/08 07:23:21  tony
 * support multiple session UI for ACL, URL Filter, DoS log.
 *
 * Revision 1.97  2004/01/07 10:59:56  tony
 * Support multiple session UI plugin for ACL.
 *
 * Revision 1.96  2004/01/07 09:10:04  tony
 * add PPTP Client UI in Config Wizard.
 *
 * Revision 1.95  2004/01/07 07:36:36  tony
 * Support multiple session UI plugin for ALG.
 *
 * Revision 1.92  2004/01/06 13:50:25  tony
 * Support multiple session UI plugin for the following functions:
 * DMZ, URL Filter, Dos Prevention
 *
 * Revision 1.91  2003/12/19 07:56:46  tony
 * make "Wireless Security Setup" support interactive web page.
 *
 * Revision 1.90  2003/12/19 05:03:49  tony
 * Should allow only 578-1492 in mtu field.
 *
 * Revision 1.89  2003/12/19 04:33:01  tony
 * add Wireless Lan config pages: Basic Setting, Advance Setting, Security, Access Control, WDS
 *
 * Revision 1.88  2003/12/17 08:29:08  tony
 * add "parse_ipRange()" fucntion.
 *
 * Revision 1.87  2003/12/15 01:55:51  tony
 * fix bug: MTU will set to 150 when config by wizard.
 *
 * Revision 1.86  2003/12/12 01:34:54  tony
 * add NAPT toggle in Unnumbered PPPoE.
 *
 * Revision 1.85  2003/12/10 03:08:58  tony
 * make logging function support time zone in kernel space.
 *
 * Revision 1.84  2003/12/09 13:44:34  tony
 * add ACL,DoS,URL Filter logging function in kernel space.
 *
 * Revision 1.83  2003/12/09 03:05:44  tony
 * add a param in rtl8651_setPppoeMtu function.
 *
 * Revision 1.82  2003/12/09 02:06:18  tony
 * enable mtu configure in each session
 *
 * Revision 1.81  2003/12/08 13:56:52  tony
 * add new ALG: support SIP protocol (for single session)
 *
 * Revision 1.80  2003/12/08 03:37:26  tony
 * add new ALG: L2TP v2 multiple-session
 *
 * Revision 1.79  2003/12/02 12:30:45  tony
 * the common.h is replace by rtl8651_dos.h and rtl8651_alg_init.h
 *
 * Revision 1.78  2003/12/02 10:24:47  tony
 * Add Routine: support DoS is able to set threshold by user(Webs GUI).
 *
 * Revision 1.77  2003/12/02 09:29:15  tony
 * rename alg.h to common.h(used for RomeDrvier and Turnkey)
 *
 * Revision 1.76  2003/12/01 12:35:52  tony
 * make ALG is able to be configured by users(Web GUI).
 *
 * Revision 1.75  2003/11/27 03:26:57  tony
 * fix bug:if default session is changed, old route table will can't delete correctly.
 * must del old route before setting new default session.
 *
 * Revision 1.74  2003/11/21 02:41:17  tony
 * add check function when un-checked dial-on-demand in PPPoE.
 *
 * Revision 1.73  2003/11/19 03:52:09  tony
 * modify unnumbered pppoe netmask gui.
 *
 * Revision 1.72  2003/11/18 09:04:47  tony
 * add routine: support mtu configure by user in pppoe.
 *
 * Revision 1.71  2003/11/18 04:30:48  tony
 * add PPPoE GUI input check
 *
 * Revision 1.70  2003/11/17 03:21:30  tony
 * small change
 *
 * Revision 1.69  2003/11/17 03:17:30  tony
 * support circular buffer in Eventlog (16k buffer)
 *
 * Revision 1.68  2003/11/14 10:40:01  rupert
 * add dhcp client list
 *
 * Revision 1.67  2003/11/14 09:13:36  tony
 * modify multiple pppoe web page
 *
 * Revision 1.66  2003/11/13 13:33:58  tony
 * pppoe: automaic Hang up all the connections after save.
 *
 * Revision 1.65  2003/11/13 12:52:06  tony
 * add MTU and MRU field in pppoe.
 *
 * Revision 1.64  2003/11/12 03:32:26  tony
 * fix bug: PPPoE can't dial
 *
 * Revision 1.63  2003/11/10 13:49:36  tony
 * fix multi-pppoe bugs:
 * 1:dialState set to zero when boot.
 * 2:disable Dial-on-Demand and Silent Timeout.
 * 3:must hang up all the connection before save configuration.
 * 4:only default session dial-on when boot.
 *
 * Revision 1.62  2003/11/07 11:05:59  rupert
 * for multiple pppoe
 *
 * Revision 1.61  2003/11/07 06:31:22  tony
 * add type PPPOECFGPARAM_DESTNETTYPE_NONE in MultiPPPoE dest network.
 *
 * Revision 1.60  2003/11/07 05:34:36  tony
 * modify YMSG ALG, it'll not have to cooperate with trigger port.
 *
 * Revision 1.59  2003/11/06 13:50:21  tony
 * add parse_ipNet parse_portRange functions
 *
 * Revision 1.58  2003/11/06 12:42:00  tony
 * fix bug in asp_setPppoe
 *
 * Revision 1.57  2003/11/06 02:25:08  tony
 * add field in multi-pppoe cfg.
 *
 * Revision 1.56  2003/10/29 10:20:41  tony
 * modify acl policy list.
 *
 * Revision 1.55  2003/10/29 01:48:24  tony
 * fix trigger port bug: when ouside host send a SYN to internal computer,
 * gateway return a RST packet problem.
 *
 * Revision 1.54  2003/10/24 10:25:58  tony
 * add DoS attack interactive webpage,
 * FwdEngine is able to get WAN status by rtl8651_wanStatus(0:disconnect,1:connect)
 *
 * Revision 1.53  2003/10/21 08:07:02  tony
 * add DoS attack log routine
 *
 * Revision 1.52  2003/10/16 09:23:30  tony
 * add check/uncheck all in acl page
 *
 * Revision 1.51  2003/10/16 05:12:41  tony
 * check Makefile param "MULTIPLE_PPPOE_SUPPORT" in Config Wizard
 *
 * Revision 1.50  2003/10/16 03:43:49  tony
 * add routine: check Makefile flag "MULTIPLE_PPPOE_SUPPORT"
 *
 * Revision 1.49  2003/10/15 13:58:36  tony
 * fix bug: mutliple pppoe default session status error when pppoe disconnect
 *
 * Revision 1.48  2003/10/14 13:11:56  tony
 * modify trigger port overlap checking function
 *
 * Revision 1.47  2003/10/14 12:46:05  tony
 * modify trigger port overlap checking function
 *
 * Revision 1.46  2003/10/14 08:38:15  tony
 * add RomeDriver version information
 *
 * Revision 1.45  2003/10/14 08:15:01  tony
 * add user account management routine
 *
 * Revision 1.44  2003/10/14 03:37:16  tony
 * fix small bug
 *
 * Revision 1.43  2003/10/14 03:24:42  tony
 * add Trigger port range overlap checking.
 *
 * Revision 1.42  2003/10/14 02:28:30  tony
 * add trigger port routine
 *
 * Revision 1.41  2003/10/09 13:23:44  tony
 * TRIGGER_PORT: add function rtl8651_flushTriggerPortRules,rtl8651_addTriggerPortRule
 *
 * Revision 1.40  2003/10/07 06:18:17  rupert
 * servie name
 *
 * Revision 1.39  2003/10/03 12:27:35  tony
 * add NTP time sync routine in management web page.
 *
 * Revision 1.38  2003/10/03 03:08:15  rupert
 * fix upload firmware
 *
 * Revision 1.37  2003/10/03 02:50:34  rupert
 * update filename
 *
 * Revision 1.36  2003/10/03 02:36:10  rupert
 * add upload firmware function
 *
 * Revision 1.35  2003/10/03 02:32:18  tony
 * change interactive messages in PPPoE.
 *
 * Revision 1.34  2003/10/03 01:26:42  tony
 * add ServiceName field in pppoeCfgParam_t.
 * add dynamic check in PPPoE/Unnumbered PPPoE/Multiple PPPoE web page.
 *
 * Revision 1.33  2003/10/02 10:50:03  tony
 * add ping routine in webpage.
 *
 * Revision 1.32  2003/10/02 07:53:49  orlando
 * asp_setPppoe() will pass valid argument to pppd_stop() now
 *
 * Revision 1.31  2003/10/02 02:16:43  tony
 * let wizard can show original wan setting in static mode.
 *
 * Revision 1.30  2003/10/01 13:25:49  tony
 * fix connection type can't change in wizard.
 *
 * Revision 1.29  2003/10/01 12:25:53  tony
 * solve all the compiler warnning messages in rome_asp.c
 *
 * Revision 1.28  2003/10/01 09:41:32  orlando
 * check in dial-on-demand and silent timeout related code
 *
 * Revision 1.27  2003/10/01 09:05:18  tony
 * debug: user set to static mode in wizard, all the wan data will be cleared when connection type is dhcpc.
 *
 * Revision 1.26  2003/10/01 07:48:06  orlando
 * modify asp_setpppoe to set pppoeCfgParam[].dialState to ..._DIALED_TRYING before
 * calling /bin/pppd
 *
 * Revision 1.25  2003/10/01 05:57:31  tony
 * add URL Filter routine
 *
 * Revision 1.24  2003/09/30 12:19:49  tony
 * arrange board_ipUpEventTableDriverAccess(), let upnp,ddns,dmz be able to start after dhcpc/pppoe started.
 *
 * Revision 1.23  2003/09/30 09:57:38  tony
 * make dmz support multiple conntion types.
 *
 * Revision 1.22  2003/09/30 09:46:38  tony
 * add dmz_tableDriverAccess()
 *
 * Revision 1.21  2003/09/30 08:56:29  tony
 * remove newServerpCfgParam[] from flash, rename ram PPPoeCfg to ramPppoeCfgParam
 *
 * Revision 1.20  2003/09/30 02:14:55  tony
 * small change in ddns
 *
 * Revision 1.19  2003/09/29 13:28:19  rupert
 * add primary and secondary fields of ifCfgParam_s
 *
 * Revision 1.18  2003/09/29 12:58:34  tony
 * add DDNS start/stop routine.
 *
 * Revision 1.17  2003/09/29 08:57:32  tony
 * add routine: when pppoe config changed, the dial function will be disabled.
 * add routine: dhcps is able to start/stop server on runtime.
 *
 * Revision 1.16  2003/09/29 03:13:23  tony
 * debug - view dhcps webpage crash when dhcps didn't start.
 *
 * Revision 1.15  2003/09/26 12:06:15  tony
 * modify pppoe Dial/Hang_Up routine
 *
 * Revision 1.14  2003/09/26 10:23:52  tony
 * add PPPoE dial,hang-up routine
 *
 * Revision 1.13  2003/09/26 08:43:17  tony
 * update Status Web Page
 *
 * Revision 1.12  2003/09/26 04:41:25  tony
 * modify Wizard page, support pppoe, unnumbered pppoe
 *
 * Revision 1.11  2003/09/26 02:09:48  tony
 * add/del UPnP default route in upnp_init() and asp_setUpnp()
 *
 * Revision 1.10  2003/09/25 10:44:50  tony
 * small change
 *
 * Revision 1.9  2003/09/25 10:14:50  tony
 * modify pppoe webpage, support unnumbered PPPoE, PPPoE, Multiple PPPoE.
 *
 * Revision 1.8  2003/09/25 06:54:00  tony
 * changae dhcpc lease list page
 *
 * Revision 1.7  2003/09/25 02:15:32  orlando
 * Big Change
 *
 * Revision 1.5  2003/09/23 11:44:33  tony
 * add UPNP daemon start/stop routine.
 *
 * Revision 1.4  2003/09/23 07:27:51  tony
 * modify all form action name.
 *
 * Revision 1.3  2003/09/23 07:13:18  tony
 * rename all webpage asp function
 *
 * Revision 1.2  2003/09/23 05:31:17  tony
 * move common.c to rome_asp.c
 *
 * Revision 1.1  2003/09/23 03:46:21  tony
 * move ASP page from net_asp.c,cfgmgr.c to rome_asp.c
 *
 * Revision 1.8  2003/09/22 08:01:45  tony
 * add UPnP Web Configuration Function Routine
 *
 * Revision 1.7  2003/09/22 06:33:37  tony
 * add syslogd process start/stop by CGI.
 * add eventlog download/clear routine.
 *
 * Revision 1.6  2003/09/19 09:06:38  tony
 * add special application webpage
 *
 * Revision 1.5  2003/09/19 06:06:51  tony
 * *** empty log message ***
 *
 * Revision 1.4  2003/09/19 01:43:50  tony
 * add dmz routine
 *
 * Revision 1.3  2003/09/18 05:07:40  tony
 * add event log webpage
 *
 *
 */


#include "rtl_types.h"
#include "rtl_flashdrv.h"
#include "flashdrv.h"
//#include "rtl_cfgmgr.h"
//#include "rtl_board.h"
#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <time.h>
#include <linux/if_ether.h>
#include <linux/config.h>
#include <net/route.h>
#include <re865x.h>
#include <version.h>
#include "../wsIntrn.h"
#include "rtl_image.h"
#include <signal.h>
#include "rtl_cfgmgr.h"
#include "rtl_board.h"
#include <rtl8651_tblDrv.h>
#include <rtl8651_alg_init.h>
#include <rtl8651_dos.h>
#include <sys/shm.h>
#define RIMR	(*((volatile uint32 *)0xbd012000))


/* defined in board.c */
extern romeCfgParam_t * pRomeCfgParam;
extern int INET_setroute(int action, int options, char **args);

extern flashdrv_filewrite(FILE *,int,void *);
struct env_t
{ 
	char *name;
	int idx;
	int off;
};

struct env_t env_tuple[]=
{
	{"lan11",1,0},
	{"lan12",1,1},
};

struct env_t env_tuplec[]=
{
	{"lan111",1,0},
	{"lan112",1,1},
	{"lan113",1,2},
	{"lan114",1,3},
	{"mask111",1,4},
	{"mask112",1,5},
	{"mask113",1,6},
	{"mask114",1,7},
	{"wan111",0,0},
	{"wan112",0,1},
	{"wan113",0,2},
	{"wan114",0,3},
	{"mask211",0,4},
	{"mask212",0,5},
	{"mask213",0,6},
	{"mask214",0,7},
	{"g1",0,8},
	{"g2",0,9},
	{"g3",0,10},
	{"g4",0,11},
	{"dn1",0,12},
	{"dn2",0,13},
	{"dn3",0,14},
	{"dn4",0,15},
};

int total_tuple = sizeof(env_tuple)/sizeof(struct env_t);

extern int re865xIoctl(char *name,unsigned long arg0, unsigned long arg1, unsigned long arg2, unsigned long arg3);

// local use
char *inet_string(uint32 dst);
void cfgmgr_task(void);
void pppd_start(uint32 identity);
int32 pppd_stop(uint32 pppObjId);
void dhcps_start(void);
int do_kill(char *filename);
uint32 upnp_start(void);
void upnp_stop(void);
void ddns_start(void);

#define MULTIPLE_PPPOE_SUPPORT 1

/************************************************************
 *	
 *	Local_Function
 *	
 ************************************************************/
static void Sys_Restart(webs_t wp)
{
	void (*start)(void);
	uint32 reboot;

	websHeader(wp);

	websWrite(wp, T("<html><head><style>\n" \
		"#cntdwn{ border-color: white;border-width: 0px;font-size: 12pt;color: red;text-align:left; font-weight:bold; }\n" \
		"</style><script language=javascript>\n" \
		"var h=30;\n" \
		"function stop() { clearTimeout(id); }\n"\
		"function start() { h--; if (h >= 0) { frm.time.value = h; id=setTimeout(\"start()\",1000); }\n" \
		"if (h == 0) { window.open(\"http://%u.%u.%u.%u/status.asp\",target=\"show_area\"); }}\n" \
		"</script></head><body bgcolor=white  onLoad=\"start();\" onUnload=\"stop();\">" \
		"<form name=frm><B><font color=red>System restarting, please wait ...\n" \
		"<INPUT TYPE=text NAME=time size=1 id=\"cntdwn\"></form></body></html>"),
		ramLanCfgParam.ipAddr[0],ramLanCfgParam.ipAddr[1],ramLanCfgParam.ipAddr[2],ramLanCfgParam.ipAddr[3]);
	websFooter(wp);
	websDone(wp, 200);	
	sleep(2);	
	re865xIoctl("eth0",RTL8651_IOCTL_REBOOT,0,0,0);

	RIMR=0;
	while(1){};
}

int send_signal(char *name,int id)
{

	FILE *fp;
	int pid;
	fp = fopen(name,"r");
	if (fp)
	{
		fscanf(fp,"%d",&pid);
		printf("pid1 %d\n",pid);
		kill(pid,id);
		fclose(fp);
		return 0;
	}
	return -1;
}

void getDialStateString(uint8 session,char *str)
{
	int dialState;
	if(session<MAX_PPPOE_SESSION) dialState=pRomeCfgParam->pppoeCfgParam[session].dialState;
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)	
	else if(session==MAX_PPPOE_SESSION) dialState=pRomeCfgParam->pptpCfgParam.dialState; //for pptp
	else if(session==MAX_PPPOE_SESSION+1) dialState=pRomeCfgParam->l2tpCfgParam.dialState; //for l2tp	
	
#endif	
	else return;
	
	switch(dialState)
	{	
		case PPPOECFGPARAM_DIALSTATE_OFF:
		case L2TPCFGPARAM_DIALSTATE_DIALED_DHCP_DISCOVER:			
			sprintf(str,"Disconnect");
			break;

		case PPPOECFGPARAM_DIALSTATE_DIALED_WAITING:			
			sprintf(str,"Dial on Demand...");
			break;
			
		
		case PPPOECFGPARAM_DIALSTATE_DIALED_TRYING:			
			sprintf(str,"Connecting...");
			break;
				
		case PPPOECFGPARAM_DIALSTATE_DIALED_SUCCESS:
			sprintf(str,"Connected");
			break;
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
		case PPTPCFGPARAM_DIALSTATE_DISCONNECTING:
			sprintf(str,"Disconnecting...");
			break;			
#endif		
		default:
			sprintf(str,"UNKNOW");
			break;			
	}
}


#if 0
void parse_ipNet(char *temp,ipaddr_t *ip,ipaddr_t *netmask)
{
	char ip_str[32],ip_str2[32];
	char netmask_str[32]={0};
	int flag=0,ipflag=0,j;
	memcpy(ip_str,temp,32);
	*ip=0;
				
	for(j=0;j<strlen(ip_str)+1;j++)
	{
											
		if(flag!=0)  netmask_str[j-flag]=ip_str[j]; 
		ip_str2[j-ipflag]=ip_str[j];	
		if((ip_str[j]=='.')||(ip_str[j]=='/')||(ip_str[j]==0)) 
			{
				if((flag!=0)&&(ip_str[j]==0)) break;
				ip_str2[j-ipflag]=0;
				*ip=(*ip<<8)|(atoi(ip_str2)&0xff);
				ipflag=j+1;
				if(ip_str[j]==0) {ip_str[flag-1]=0; break;}
				else if(ip_str[j]=='/')  flag=j+1;												
			}						
	}
	j=atoi(netmask_str);
	if(netmask_str[0]==0) j=32;
	*netmask=0xffffffff<<(32-j);										
}
#else
void parse_ipRange(char *temp, ipaddr_t *ip, ipaddr_t *ip2)
{
	int j,ipflag=0;
	ipaddr_t temp_ip;
	char ip_str[32],ip_str2[32];
	memcpy(ip_str,temp,32);
	*ip=0;
	*ip2=0;

	for(j=0;j<strlen(ip_str)+1;j++)
	{		
		ip_str2[j-ipflag]=ip_str[j];	
		if((ip_str[j]=='.')||(ip_str[j]=='-')||(ip_str[j]==0)) 
		{			
			ip_str2[j-ipflag]=0;
			*ip=(*ip<<8)|(atoi(ip_str2)&0xff);
			ipflag=j+1;				
			if(ip_str[j]=='-') { *ip2=*ip; *ip=0; }
			if(ip_str[j]==0) break;
		}						
	}

	if(*ip2==0) *ip2=*ip;
	if(*ip2<*ip) {temp_ip=*ip; *ip=*ip2; *ip2=temp_ip; }
	
	
}
#endif

void parse_portRange(char *temp,uint16 *port_start,uint16 *port_finish)
{
	char temp2[32];
	int j;
	uint16 port_temp;	
	memcpy(temp2,temp,32);
	*port_start=atoi(temp);
	*port_finish=atoi(temp);
	for(j=0;j<strlen(temp2);j++)
	{
		if(temp2[j]=='-') 
		{
			temp2[j]=0;
			*port_start=atoi(temp2);
			*port_finish=atoi(temp2+j+1);
			break;
		}
	}
	if(*port_finish<*port_start) { port_temp=*port_finish; *port_finish=*port_start; *port_start=port_temp; }
}


void parse_mac(char *temp,uint8 *mac)
{
	int i,j=0;
	char temp2[32];

	for(i=0;i<6;i++) mac[i]=0;
	
	memcpy(temp2,temp,32);
	for(i=0;i<17;i++)
	{
		if((temp2[i]>='0')&&(temp2[i]<='9'))		
			mac[j]=(mac[j]<<4)+(temp2[i]-'0');		
		else if((temp2[i]>='a')&&(temp2[i]<='f'))
			mac[j]=(mac[j]<<4)+(temp2[i]-'a')+10;				
		else if((temp2[i]>='A')&&(temp2[i]<='F'))
			mac[j]=(mac[j]<<4)+(temp2[i]-'A')+10;
		else
			{ j++; if(j>=6) break; }		
	}
	
}


/************************************************************
 *	
 *	Multiple Session Configuration (return which session)
 *	
 ************************************************************/
int asp_multiSession( webs_t wp, int argc, char_t **argv)
{

#ifdef RTL8651_DSID_SUPPORT
	int ms=atoi(websGetVar(wp,"ms",""));
	int setted=0;
	int i;

	if(strcmp(argv[0],"ms")==0)
	{
		if(pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE) 
		{
			websWrite(wp,"<form name=msform><B>Dial Session: </B><select name=\"ms\"  onchange=\"changeMs(ms.value);\">");
			for(i=0;i<MAX_PPPOE_SESSION;i++)
			{
				if(pRomeCfgParam->pppoeCfgParam[i].enable==1)
				{
					if(setted==0&&ms==0) {ms=i; setted=1;}
					websWrite(wp,"<option value=\"%d\"%s>PPPoE %d</option>",i,(i==ms)?" selected":"",i);
				}
			}
			websWrite(wp,"</select></form>");
		}
	}
	else
	{
		if((pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)&&(ms==0)) 
		{
			for(i=0;i<MAX_PPPOE_SESSION;i++)
			{
				if(pRomeCfgParam->pppoeCfgParam[i].enable==1)
				{
					if(setted==0) {ms=i; setted=1;}
				}
			}
		}
	}
#else
	int ms=0;
#endif
	return ms;

}

/************************************************************
 *	
 *	Status Extended for ( Multiple PPPoE )
 *	
 ************************************************************/
int asp_statusExtended(int ejid, webs_t wp, int argc, char_t **argv)
{
	int i;
	char str[30];
	switch(pRomeCfgParam->ifCfgParam[0].connType)
	{
		case IFCFGPARAM_CONNTYPE_PPPOE:
		case IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED:
			getDialStateString(0,str);
			websWrite(wp,"<tr><td bgColor=#aaddff>Dial State</td><td bgColor=#ddeeff><font color=black>%s</td></tr>",str);
			break;
		case IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE:
			websWrite(wp,"<tr><td bgColor=#aaddff>Dial State</td><td bgColor=#ddeeff><font color=black>");
			for(i=0;i<MAX_PPPOE_SESSION;i++)
			{
				if(pRomeCfgParam->pppoeCfgParam[i].defaultSession==1)
				{
					getDialStateString(i,str);
					websWrite(wp,"%s",str);
					websWrite(wp,"&nbsp;&nbsp;&nbsp;&nbsp;<B>[<font color=blue>Default</font>]</b>");
					break;
				}
			}			
			websWrite(wp,"</td></tr>");
			
			for(i=0;i<MAX_PPPOE_SESSION;i++)
			{				
				if((pRomeCfgParam->pppoeCfgParam[i].defaultSession!=1)&&(pRomeCfgParam->pppoeCfgParam[i].enable==1))
				{
					websWrite(wp,"<tr><td colspan=2>&nbsp;</td></tr>\n");
					getDialStateString(i,str);
					websWrite(wp,"<tr><td bgColor=#aaddff>IP Address</td><td bgColor=#ddeeff><font color=black>%s</td></tr>\n"
					,(pRomeCfgParam->pppoeCfgParam[i].dialState==PPPOECFGPARAM_DIALSTATE_DIALED_SUCCESS)?(inet_string((uint32)&(pRomeCfgParam->pppoeCfgParam[i].ipAddr))):"0.0.0.0");
					websWrite(wp,"<tr><td bgColor=#aaddff>Subnet Mask</td><td bgColor=#ddeeff><font color=black>%s</td></tr>\n"
					,(pRomeCfgParam->pppoeCfgParam[i].dialState==PPPOECFGPARAM_DIALSTATE_DIALED_SUCCESS)?(inet_string((uint32)&(pRomeCfgParam->pppoeCfgParam[i].ipMask))):"0.0.0.0");
					websWrite(wp,"<tr><td bgColor=#aaddff>Dial State</td><td bgColor=#ddeeff><font color=black>");
					websWrite(wp,"%s&nbsp;&nbsp;&nbsp;&nbsp;<B>[<font color=blue>%d</font>]</b></td></tr>\n",str,i);
				}
			}
			break;
	} //end switch
	return 0;
}

/************************************************************
 *	
 *	Status Config Changed 
 *	
 ************************************************************/
int asp_configChanged(int ejid, webs_t wp, int argc, char_t **argv)
{
	int flag=0;
	if(!strcmp(argv[0],"CONFIG_CHANGE_MESSAGE"))
	{
		if(configChanged)
		{
			websWrite(wp,"<BR><font color=red><B>Configuration has been changed, please restart system<BR>to make new configureation take effect.");
		}
		return 0;
	}
	if(!strcmp(argv[0],"CONFIG_CHANGE_FW_VER")) {if(configChanged&CONFIG_CHANGE_FW_VER) flag=1;}
	//else if(!strcmp(argv[0],"CONFIG_CHANGE_LD_VER")) {if(configChanged&CONFIG_CHANGE_LD_VER) flag=1;}
	//else if(!strcmp(argv[0],"CONFIG_CHANGE_NAT")) {if(configChanged&CONFIG_CHANGE_NAT) flag=1;}
	//else if(!strcmp(argv[0],"CONFIG_CHANGE_HW_ACCEL")) {if(configChanged&CONFIG_CHANGE_HW_ACCEL) flag=1;}
	//else if(!strcmp(argv[0],"CONFIG_CHANGE_LAN_IP")) {if(configChanged&CONFIG_CHANGE_LAN_IP) flag=1;}
	//else if(!strcmp(argv[0],"CONFIG_CHANGE_LAN_MASK")) {if(configChanged&CONFIG_CHANGE_LAN_MASK) flag=1;}
	//else if(!strcmp(argv[0],"CONFIG_CHANGE_LAN_MAC")) {if(configChanged&CONFIG_CHANGE_LAN_MAC) flag=1;}
	//else if(!strcmp(argv[0],"CONFIG_CHANGE_DHCPS")) {if(configChanged&CONFIG_CHANGE_DHCPS) flag=1;}
	else if(!strcmp(argv[0],"CONFIG_CHANGE_CONN_TYPE")) {if(configChanged&CONFIG_CHANGE_CONN_TYPE) flag=1;}
	//else if(!strcmp(argv[0],"CONFIG_CHANGE_WAN_IP")) {if(configChanged&CONFIG_CHANGE_WAN_IP) flag=1;}
	//else if(!strcmp(argv[0],"CONFIG_CHANGE_WAN_MASK")) {if(configChanged&CONFIG_CHANGE_WAN_MASK) flag=1;}
	//else if(!strcmp(argv[0],"CONFIG_CHANGE_WAN_GW")) {if(configChanged&CONFIG_CHANGE_WAN_GW) flag=1;}
	//else if(!strcmp(argv[0],"CONFIG_CHANGE_WAN_DNS")) {if(configChanged&CONFIG_CHANGE_WAN_DNS) flag=1;}
	//else if(!strcmp(argv[0],"CONFIG_CHANGE_WAN_MAC")) {if(configChanged&CONFIG_CHANGE_WAN_MAC) flag=1;}
	websWrite(wp,"%s",(flag==1)?"<font color=red><b>":"<font color=black>");
	return 0;
}

/************************************************************
 *	
 *	NAT & Hardware Accelerator
 *	
 ************************************************************/
void asp_setNat(webs_t wp, char_t *path, char_t *query)
{
	int result;
	char *valstr;

	DEBUG_P("hwaccel string %s\n",websGetVar(wp,T("hwaccel"),T("")));
	valstr=(websGetVar(wp,T("hwaccel"),T("")));
	if (!gstrcmp(valstr,"on"))
		pRomeCfgParam->natCfgParam.hwaccel=1;
	else
		pRomeCfgParam->natCfgParam.hwaccel=0;

	result = cfgmgr_write(CFGMGR_TABID_NAT,(void*)&(pRomeCfgParam->natCfgParam), sizeof(natCfgParam_t));
	cfgmgr_task();
	websRedirect(wp, T("nat.asp"));
	return;
}



/************************************************************
 *	
 *	PPPoE/Unnumbered PPPoE/Multiple PPPoE setup for Advanced
 *	
 ************************************************************/
int pppoe_status;
void asp_setPppoe(webs_t wp, char_t *path, char_t *query)
{
	char temp1[32],*temp2,*temp3;
	int i,j,k,newDefaultSession=0;
	int pppoeType=atoi(websGetVar(wp,"pppoeType",""));
	pppoeCfgParam_t tempPppoeCfgParam[4];

	temp3=websGetVar(wp,"submit2","");
	if(memcmp(temp3,"Dial",4)==0)
	{
		int session_id=0;
		printf("Dial: %s\n",websGetVar(wp,"sessionID",""));
		/* replace following code to call pppd_start() defined in board.c
		 * cause we have to overwrite /ppp/options[x] every time we call pppd
		 */
		//do_exec("/bin/pppd",websGetVar(wp,"sessionID",""));
		//pRomeCfgParam->pppoeCfgParam[atoi(websGetVar(wp,"sessionID",""))].dialState=PPPOECFGPARAM_DIALSTATE_DIALED_TRYING;		
		session_id=atoi(websGetVar(wp,"sessionID",""));
		pppd_start(session_id);		
		sleep(1);
	}
	else if(memcmp(temp3,"Hang up",7)==0)
	{
		uint32 pppObjId;
		printf("Hang_Up: %s\n",websGetVar(wp,"sessionID",""));
		//sprintf(temp1,"/var/run/pppd%s.pid",websGetVar(wp,"sessionID",""));
		//pRomeCfgParam->pppoeCfgParam[atoi(websGetVar(wp,"sessionID",""))].dialState=PPPOECFGPARAM_DIALSTATE_OFF;
		//do_kill(temp1);
		pppObjId = (uint32)atoi(websGetVar(wp,"sessionID",""));		
		pppd_stop(pppObjId);

	}
	else if(memcmp(temp3,"Refresh",7)==0)
	{
		printf("Refresh\n");
	}	
	else
	{
		temp2=websGetVar(wp,temp1,"");
				
		for(i=0;i<MAX_PPPOE_SESSION;i++)
		{						
			if((pppoeType!=IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)&&(i>0)) break;

			memcpy((void *)&(tempPppoeCfgParam[i]),(void *)&(pRomeCfgParam->pppoeCfgParam[i]),sizeof(struct pppoeCfgParam_s));
			
			sprintf(temp1,"username%d",i);
			temp2=websGetVar(wp,temp1,"");
			memcpy((char*)&(pRomeCfgParam->pppoeCfgParam[i].username),temp2,strlen(temp2)+1);
				
			sprintf(temp1,"password%d",i);
			temp2=websGetVar(wp,temp1,"");
			memcpy((char*)&(pRomeCfgParam->pppoeCfgParam[i].password),temp2,strlen(temp2)+1);
			
			sprintf(temp1,"serviceName%d",i);
			temp2=websGetVar(wp,temp1,"");
			if (temp2)
			memcpy((char*)&(pRomeCfgParam->pppoeCfgParam[i].serviceName),temp2,strlen(temp2)+1);
			else pRomeCfgParam->pppoeCfgParam[i].serviceName[0]=0x0;

			sprintf(temp1,"acName%d",i);
			temp2=websGetVar(wp,temp1,"");
			if (temp2)
			memcpy((char*)&(pRomeCfgParam->pppoeCfgParam[i].acName),temp2,strlen(temp2)+1);
			else pRomeCfgParam->pppoeCfgParam[i].acName[0]=0x0;

		
			if(pppoeType==IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)
			{

				sprintf(temp1,"lanType%d",i);
				pRomeCfgParam->pppoeCfgParam[i].lanType=atoi(websGetVar(wp,temp1,""));

				if(pRomeCfgParam->pppoeCfgParam[i].lanType==PPPOECFGPARAM_LANTYPE_UNNUMBERED)
				{
					uint32 netmask=0xffffffff;
					uint8 *netmask_p=(uint8 *)&netmask;
					for(j=0;j<4;j++)
					{
						sprintf(temp1,"unnumberedIpAddr%d%d",i,j+1);
						temp2=websGetVar(wp,temp1,"");
						pRomeCfgParam->pppoeCfgParam[i].unnumberedIpAddr[j]=atoi(temp2);
					}	

					sprintf(temp1,"unnumberedIpMask%d",i);
					netmask=(netmask>>(32-atoi(websGetVar(wp,temp1,""))))<<(32-atoi(websGetVar(wp,temp1,"")));
					pRomeCfgParam->pppoeCfgParam[i].unnumberedIpMask[0]=netmask_p[0];
					pRomeCfgParam->pppoeCfgParam[i].unnumberedIpMask[1]=netmask_p[1];
					pRomeCfgParam->pppoeCfgParam[i].unnumberedIpMask[2]=netmask_p[2];
					pRomeCfgParam->pppoeCfgParam[i].unnumberedIpMask[3]=netmask_p[3];					
				}
			
				for(k=0;k<4;k++)
				{
					if(pRomeCfgParam->pppoeCfgParam[i].defaultSession==1) continue;
					sprintf(temp1,"destnet%d%d",i,k+1);
					temp2=websGetVar(wp,temp1,"");
					memcpy((char*)&(pRomeCfgParam->pppoeCfgParam[i].destnet[k]),temp2,strlen(temp2)+1);

					sprintf(temp1,"destnettype%d%d",i,k+1);
					pRomeCfgParam->pppoeCfgParam[i].destnetType[k]=atoi(websGetVar(wp,temp1,""));

					if(strlen(pRomeCfgParam->pppoeCfgParam[i].destnet[k])!=0)
						{
							switch(pRomeCfgParam->pppoeCfgParam[i].destnetType[k])
							{
								case PPPOECFGPARAM_DESTNETTYPE_IP:
									{
										ipaddr_t ip;
#if 0										
										ipaddr_t netmask;
										int j=0;
										parse_ipNet(pRomeCfgParam->pppoeCfgParam[i].destnet[k], &ip, &netmask);
										if(netmask!=0xffffffff) while(netmask<<j) j++;
										else j=32;
										sprintf(pRomeCfgParam->pppoeCfgParam[i].destnet[k],"%u.%u.%u.%u/%u",NIPQUAD(ip),j);
#else
										ipaddr_t ip2;
										parse_ipRange(pRomeCfgParam->pppoeCfgParam[i].destnet[k], &ip, &ip2);
										sprintf(pRomeCfgParam->pppoeCfgParam[i].destnet[k],"%u.%u.%u.%u-%u.%u.%u.%u",NIPQUAD(ip),NIPQUAD(ip2));
#endif

									}
									break;
								case PPPOECFGPARAM_DESTNETTYPE_DOMAIN:
									//pRomeCfgParam->pppoeCfgParam[i].destnet[k]
									break;
								case PPPOECFGPARAM_DESTNETTYPE_TCPPORT:
								case PPPOECFGPARAM_DESTNETTYPE_UDPPORT:
									{
										uint16 port_start,port_finish;
										parse_portRange(pRomeCfgParam->pppoeCfgParam[i].destnet[k],&port_start,&port_finish);
										sprintf(pRomeCfgParam->pppoeCfgParam[i].destnet[k],"%u-%u",port_start,port_finish);
									}
									break;
							}
					}
					
				}	
	
				
				sprintf(temp1,"enable%d",i);
				pRomeCfgParam->pppoeCfgParam[i].enable=atoi(websGetVar(wp,temp1,""));
				
				
			}
			
			if(pppoeType==IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED)
			{
				uint32 netmask=0xffffffff;
				uint8 *netmask_p=(uint8 *)&netmask;
				for(j=0;j<4;j++)
				{
					sprintf(temp1,"unnumberedIpAddr%d",j+1);
					temp2=websGetVar(wp,temp1,"");
					pRomeCfgParam->pppoeCfgParam[i].unnumberedIpAddr[j]=atoi(temp2);
				}	
				netmask=(netmask>>(32-atoi(websGetVar(wp,"unnumberedIpMask",""))))<<(32-atoi(websGetVar(wp,"unnumberedIpMask","")));
				pRomeCfgParam->pppoeCfgParam[i].unnumberedIpMask[0]=netmask_p[0];
				pRomeCfgParam->pppoeCfgParam[i].unnumberedIpMask[1]=netmask_p[1];
				pRomeCfgParam->pppoeCfgParam[i].unnumberedIpMask[2]=netmask_p[2];
				pRomeCfgParam->pppoeCfgParam[i].unnumberedIpMask[3]=netmask_p[3];
				pRomeCfgParam->pppoeCfgParam[i].unnumberedNapt=atoi(websGetVar(wp,"unnumberedNapt",""));
				
			}
			
			sprintf(temp1,"demand%d",i);
			pRomeCfgParam->pppoeCfgParam[i].demand=atoi(websGetVar(wp,temp1,""));
			
			sprintf(temp1,"silentTimeout%d",i);
			pRomeCfgParam->pppoeCfgParam[i].silentTimeout=atoi(websGetVar(wp,temp1,""));
			
			sprintf(temp1,"autoReconnect%d",i);
			pRomeCfgParam->pppoeCfgParam[i].autoReconnect=atoi(websGetVar(wp,temp1,""));

			sprintf(temp1,"mtu%d",i);
			pRomeCfgParam->pppoeCfgParam[i].mtu=atoi(websGetVar(wp,temp1,""));
			if(pRomeCfgParam->pppoeCfgParam[i].mtu>1492) pRomeCfgParam->pppoeCfgParam[i].mtu=1492;
			if(pRomeCfgParam->pppoeCfgParam[i].mtu<576) pRomeCfgParam->pppoeCfgParam[i].mtu=576;
		
			
			//pRomeCfgParam->pppoeCfgParam[i].mru=atoi(websGetVar(wp,"mru",""));
			
		}


		// set default session -- for mutliple pppoe
		
		if(pppoeType==IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)
		{				
			if(pRomeCfgParam->pppoeCfgParam[atoi(websGetVar(wp,"default_session",""))].enable==1)
			{
				newDefaultSession=atoi(websGetVar(wp,"default_session",""));
				rtl8651_setPppoeDefaultDialSessionId((uint32)newDefaultSession);
			}
			else
			{
				for(i=0;i<MAX_PPPOE_SESSION;i++)
				{
					if(pRomeCfgParam->pppoeCfgParam[i].enable==1)
					{
						newDefaultSession=i;
						break;
					}
				}
			}
		}

		//disconnect pppoe when cfg be changed
		for(i=0;i<MAX_PPPOE_SESSION;i++)
		{
			if((pppoeType!=IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)&&(i>0)) break;

			if((memcmp((void *)&(tempPppoeCfgParam[i]),(void *)&(pRomeCfgParam->pppoeCfgParam[i]),sizeof(struct pppoeCfgParam_s)))||
				(pRomeCfgParam->pppoeCfgParam[i].defaultSession!=(newDefaultSession==i?1:0)))
			{
				if(pRomeCfgParam->pppoeCfgParam[i].dialState!=PPPOECFGPARAM_DIALSTATE_OFF) 
				{
					board_ipDownEventTableDriverAccess(); 
					//tony: if default session is changed, old route table will can't delete correctly.
					// must del old route before set new default session.
					pppd_stop(i);			
				}
			}
		}
				
		if(pppoeType==IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)
		{				
			for(i=0;i<MAX_PPPOE_SESSION;i++)	
			{					
				pRomeCfgParam->pppoeCfgParam[i].defaultSession=0;
			}
			pRomeCfgParam->pppoeCfgParam[newDefaultSession].defaultSession=1;
			pRomeCfgParam->pppoeCfgParam[newDefaultSession].lanType=PPPOECFGPARAM_LANTYPE_NAPT;
		}
		
		
		if(pppoeType!=pRomeCfgParam->ifCfgParam[0].connType) configChanged|=CONFIG_CHANGE_CONN_TYPE;
		if(pppoeType==IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE) pRomeCfgParam->ifCfgParam[0].connType=IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE;
		if(pppoeType==IFCFGPARAM_CONNTYPE_PPPOE) pRomeCfgParam->ifCfgParam[0].connType=IFCFGPARAM_CONNTYPE_PPPOE;
		if(pppoeType==IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED) pRomeCfgParam->ifCfgParam[0].connType=IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED;

		cfgmgr_write(CFGMGR_TABID_IF,(void*)pRomeCfgParam->ifCfgParam, sizeof(pRomeCfgParam->ifCfgParam));
		cfgmgr_write(CFGMGR_TABID_PPPOE,(void*)pRomeCfgParam->pppoeCfgParam,sizeof(struct pppoeCfgParam_s)*MAX_PPPOE_SESSION);
		cfgmgr_task();
		
	}
	sprintf(temp1,"pppoe.asp?pppoeType=%d",pppoeType);
	websRedirect(wp, temp1);
}

int asp_flashGetPppoeParam(int ejid, webs_t wp, int argc, char_t **argv)
{
	char str[30];
	int i;
	
	if (!strcmp(argv[0],"username") )
		return (websWrite(wp, "%s",pRomeCfgParam->pppoeCfgParam[0].username));

	if (!strcmp(argv[0],"password") )
		return (websWrite(wp, "%s",pRomeCfgParam->pppoeCfgParam[0].password));

	if (!strcmp(argv[0],"demand") )
		return (websWrite(wp,"%s",(pRomeCfgParam->pppoeCfgParam[0].demand==1)?"checked":""));

	if (!strcmp(argv[0],"silentTimeout") )
		return (websWrite(wp,"%d",pRomeCfgParam->pppoeCfgParam[0].silentTimeout));

	if (!strcmp(argv[0],"autoReconnect") )
		return (websWrite(wp,"%s",(pRomeCfgParam->pppoeCfgParam[0].autoReconnect==1)?"checked":""));
		
	
	for(i=0;i<4;i++)
	{
		sprintf(str,"unnumberedIpAddr%d",i+1);
		if (!strcmp(argv[0],str))
			return (websWrite(wp,"%d",pRomeCfgParam->pppoeCfgParam[0].unnumberedIpAddr[i]));
		
		sprintf(str,"unnumberedIpMask%d",i+1);
		if (!strcmp(argv[0],str))
			return (websWrite(wp,"%d",pRomeCfgParam->pppoeCfgParam[0].unnumberedIpMask[i]));
	}

	for(i=0;i<MAX_PPPOE_SESSION;i++)
	{
		sprintf(str,"dialState%d",i);
		if (!strcmp(argv[0],str))
		{
			getDialStateString(i,str);
			return(websWrite(wp,"%s",str));
		}
	}

	return 0;
}


/************************************************************
 *	
 *	Management
 *	
 ************************************************************/
void asp_systemDefault(webs_t wp, char_t * path, char_t * query)
{
	#if defined(CONFIG_RTL865X_INIT_BUTTON)
	cfgmgr_factoryDefault(0);
	#else
	cfgmgr_factoryDefault();
	#endif /* CONFIG_RTL865X_INIT_BUTTON */
	Sys_Restart(wp);
}

void asp_restart(webs_t wp, char_t * path, char_t * query)
{
	Sys_Restart(wp);
}



void asp_upload(webs_t wp, char_t * path, char_t * query)
{
	uint32 startPos,endPos;
	FILE	*fp;
	struct stat	fileStat;
	unsigned char c; 


	fp=fopen("/var/run.bix","r");
	if (!fp)return;
	do
	{
		if(feof(fp))goto fail;
		c= fgetc(fp);
		if (c!=0xd)
			continue;
		c= fgetc(fp);
		if (c!=0xa)
			continue;
		c= fgetc(fp);
		if (c!=0xd)
			continue;
		c= fgetc(fp);
		if (c!=0xa)
			continue;
		break;


	}while(1);
	startPos=ftell(fp);
	stat("/var/run.bix",&fileStat);
	fseek(fp,fileStat.st_size-0x100,SEEK_SET);
	do
	{
		if(feof(fp))goto fail;
		c= fgetc(fp);
		if (c!=0xd)
			continue;
		c= fgetc(fp);
		if (c!=0xa)
			continue;
		c= fgetc(fp);
		if (c!='-')
			continue;
		c= fgetc(fp);
		if (c!='-')
			continue;
		break;


	}while(1);
	endPos=ftell(fp);
	printf("Upload Firmsize %d \n",endPos-4-startPos);	
	fseek(fp,startPos,SEEK_SET);

#ifdef _SUPPORT_LARGE_FLASH_
	/* Load bdinfo first */
	flashdrv_read((void*)&bdinfo,(void*)FLASH_MAP_BOARD_INFO_ADDR,sizeof(bdinfo));
#endif/*_SUPPORT_LARGE_FLASH_*/

	printf("Start Flash Writing  \n");	
	flashdrv_filewrite(fp,endPos-4-startPos,(void*)FLASH_MAP_RUN_IMAGE_ADDR);
	fclose(fp);
	Sys_Restart(wp);
	return;
fail:
	printf("Upload Fail\n");
	return;
#if 0
	pEnd=pBuf;
	sstr[2]='-';
	sstr[3]='-';
	while (memcmp(pEnd,sstr,4))
			pEnd++;
	nLen = (uint32)pEnd-(uint32)pBuf;  
	printf("Upload Firmware File, File Size = %d\n",strlen(query));
#endif
#if 0
	flag=RIMR;
	//RIMR = 0;
	
	 if (flashdrv_updateImg((void*)pBuf,(void*)FLASH_MAP_RUN_IMAGE_ADDR,(uint32)nLen))
	 {
	 	RIMR=flag;
		 DEBUG_P("update error\n");
			 for(;;);
	}
	 	RIMR=flag;
	 Sys_Restart(wp);
#endif
	
}

/************************************************************
 *	
 *	Status 
 *	
 ************************************************************/
char *inet_string(uint32 dst)
{
	struct in_addr	in;
	memcpy((char*)&in,(char*)dst,sizeof(in));
	return (inet_ntoa(in));
}


int asp_flashGetCfgParam(int ejid, webs_t wp, int argc, char_t **argv)
{
	int		i;
	char		*Str;
	struct	env_t	*ep;
	struct ifCfgParam_s *pIfCfg;

	a_assert(websValid(wp));
	if (argc>1)	
	{
		a_assert(argv);
		return -1;
	}

	for (i=0;i<total_tuple;i++)
	{
		ep = &env_tuple[i];
		if (!strcmp(ep->name,argv[0]))
		{
			pIfCfg = &(pRomeCfgParam->ifCfgParam[ep->idx]);
			//fprintf(stderr," idx %x addr %x\n",ep->idx,ep->off);
			Str = inet_string((uint32)((char*)(&pIfCfg->ipAddr)+((ep->off)*4)));
			goto fin;
		}
	}
	return 0;
fin:	
	if (websWriteBlock(wp,Str,strlen(Str) ) < 0) 
		return -1;
	else 
		return 0;

}


int total_tuplec = sizeof(env_tuplec)/sizeof(struct env_t);
int asp_flashGetCloneMac(int ejid, webs_t wp, int argc, char_t **argv)
{
	int i=0;
	char str[8];
	for (i=0;i<6;i++)
	{
		sprintf(str,"cmac%d",i+1);
		if(!strcmp(str,argv[0]))
		{
			websWrite(wp,T("%02X"),pRomeCfgParam->dhcpcCfgParam.cloneMac[i]);
			break;
		}
	}
	if(!strcmp("cmacValid",argv[0]))
		if (pRomeCfgParam->dhcpcCfgParam.cmacValid)
			websWrite(wp,T("checked"));
	return 0;
}

/* 
     Get IP address IP mask ...
*/
     
int asp_flashGetIpElement(int ejid, webs_t wp, int argc, char_t **argv)
{
	int		i;
	char	Str[16]={0};
	struct	env_t	*ep;
	struct ifCfgParam_s *pIfCfg;
	//a_assert(websValid(wp));
	if (argc>1)	
	{
		//a_assert(argv);
		return -1;
	}

	for (i=0;i<total_tuplec;i++)
	{
		ep = &env_tuplec[i];
		if (!strcmp(ep->name,argv[0]))
		{
			pIfCfg = &(pRomeCfgParam->ifCfgParam[ep->idx]);
			//fprintf(stderr," idx %x addr %x\n",ep->idx,ep->off);
			sprintf(Str,"%d", *(unsigned char*)((char*)(&(pIfCfg->ipAddr))+(ep->off)));
			goto fin;
		}
	}
	return 0;
fin:	
	if (websWrite(wp,"%s",Str) < 0) 
		return -1;
	else 
		return 0;
	return 0;

}

#define LDR_COMMUN_BASE	0x80000300


/* 
	Get constant string 
*/
int asp_flashGetString(int ejid, webs_t wp, int argc, char_t **argv)
{
	uint32	ver;
	
	fileImageHeader_t *imgHdr;
	if (!gstrcmp(argv[0],T("fwVer")))
	{
		return (websWrite(wp,T("%s"),PKG_VERSION));				
	}
	
	if (!gstrcmp(argv[0],T("wanIpType")))
	{
		switch (pRomeCfgParam->ifCfgParam[0].connType)
		{
			case IFCFGPARAM_CONNTYPE_STATIC:
				websWrite(wp,"Static");
				break;
			case IFCFGPARAM_CONNTYPE_DHCPC :
				websWrite(wp,"DHCP Client");
				break;
			case IFCFGPARAM_CONNTYPE_PPPOE:
				websWrite(wp,T("PPPoE"));
				break;
			case IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED:
				websWrite(wp,"Unnumbered PPPoE");
				break;
			case IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE:
				websWrite(wp,"Multiple PPPoE");
				break;

#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)

			case IFCFGPARAM_CONNTYPE_PPTP:
				websWrite(wp,"PPTP");
				break;
			case IFCFGPARAM_CONNTYPE_L2TP:
				websWrite(wp,"L2TP");
				break;
			case IFCFGPARAM_CONNTYPE_DHCPL2TP:
				websWrite(wp, "DHCPL2TP");
				break;

			case IFCFGPARAM_CONNTYPE_3G:
				websWrite(wp, "3G");
				break;				
#endif				
		}
		
		return 0;
	}
		
	if (!gstrcmp(argv[0],T("wanstatic")))
	{
		if (websWrite(wp,T("%s") ,
			pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_STATIC ? "checked":" ") < 0) 
			return -1;
		else
			return 0;
	}

	if (!gstrcmp(argv[0],T("wandhcp")))
	{
		if (websWrite(wp,T("%s") ,
			pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_DHCPC ? "checked":" ") < 0)
			return -1;
		else
			return 0;
	}
	
	if (!gstrcmp(argv[0],T("wanpppoe")))
	{
		if (websWrite(wp,T("%s") ,
			pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_PPPOE ? "checked":"   ") < 0) 
			return -1;
		else
			return 0;
	}




	if (!gstrcmp(argv[0],T("dhcpmac_1_1")))
	{
		websWrite(wp, "%02x",  pRomeCfgParam->dhcpsCfgParam.hardwareAddr[0][0]);
	}	
	if (!gstrcmp(argv[0],T("dhcpmac_1_2")))
	{
		websWrite(wp, "%02x",  pRomeCfgParam->dhcpsCfgParam.hardwareAddr[0][1]);
	}
	if (!gstrcmp(argv[0],T("dhcpmac_1_3")))
	{
		websWrite(wp, "%02x",  pRomeCfgParam->dhcpsCfgParam.hardwareAddr[0][2]);
	}
	if (!gstrcmp(argv[0],T("dhcpmac_1_4")))
	{
		websWrite(wp, "%02x",  pRomeCfgParam->dhcpsCfgParam.hardwareAddr[0][3]);
	}
	if (!gstrcmp(argv[0],T("dhcpmac_1_5")))
	{
		websWrite(wp, "%02x",  pRomeCfgParam->dhcpsCfgParam.hardwareAddr[0][4]);
	}
	if (!gstrcmp(argv[0],T("dhcpmac_1_6")))
	{
		websWrite(wp, "%02x",  pRomeCfgParam->dhcpsCfgParam.hardwareAddr[0][5]);
	}
	if (!gstrcmp(argv[0],T("dhcpmac_2_1")))
	{
		websWrite(wp, "%02x",  pRomeCfgParam->dhcpsCfgParam.hardwareAddr[1][0]);
	}	
	if (!gstrcmp(argv[0],T("dhcpmac_2_2")))
	{
		websWrite(wp, "%02x",  pRomeCfgParam->dhcpsCfgParam.hardwareAddr[1][1]);
	}
	if (!gstrcmp(argv[0],T("dhcpmac_2_3")))
	{
		websWrite(wp, "%02x",  pRomeCfgParam->dhcpsCfgParam.hardwareAddr[1][2]);
	}
	if (!gstrcmp(argv[0],T("dhcpmac_2_4")))
	{
		websWrite(wp, "%02x",  pRomeCfgParam->dhcpsCfgParam.hardwareAddr[1][3]);
	}
	if (!gstrcmp(argv[0],T("dhcpmac_2_5")))
	{
		websWrite(wp, "%02x",  pRomeCfgParam->dhcpsCfgParam.hardwareAddr[1][4]);
	}
	if (!gstrcmp(argv[0],T("dhcpmac_2_6")))
	{
		websWrite(wp, "%02x",  pRomeCfgParam->dhcpsCfgParam.hardwareAddr[1][5]);
	}	
	if (!gstrcmp(argv[0],T("dhcpip_1_1")))
	{
		websWrite(wp, "%03u",  pRomeCfgParam->dhcpsCfgParam.manualIp[0]>>24);
		
	}
	if (!gstrcmp(argv[0],T("dhcpip_1_2")))
	{
		websWrite(wp, "%03u", ( pRomeCfgParam->dhcpsCfgParam.manualIp[0]&0X00ff0000)>>16);
		
	}	
	if (!gstrcmp(argv[0],T("dhcpip_1_3")))
	{
		websWrite(wp, "%03u",  (pRomeCfgParam->dhcpsCfgParam.manualIp[0]&0x0000ff00)>>8);
		
	}	
	if (!gstrcmp(argv[0],T("dhcpip_1_4")))
	{
		websWrite(wp, "%03u",  pRomeCfgParam->dhcpsCfgParam.manualIp[0]&0xff);
		
	}	
	if (!gstrcmp(argv[0],T("dhcpip_2_1")))
	{
		websWrite(wp, "%03u",  pRomeCfgParam->dhcpsCfgParam.manualIp[1]>>24);
		
	}
	if (!gstrcmp(argv[0],T("dhcpip_2_2")))
	{
		websWrite(wp, "%03u", ( pRomeCfgParam->dhcpsCfgParam.manualIp[1]&0X00ff0000)>>16);
		
	}	
	if (!gstrcmp(argv[0],T("dhcpip_2_3")))
	{
		websWrite(wp, "%03u",  (pRomeCfgParam->dhcpsCfgParam.manualIp[1]&0x0000ff00)>>8);
		
	}	
	if (!gstrcmp(argv[0],T("dhcpip_2_4")))
	{
		websWrite(wp, "%03u",  pRomeCfgParam->dhcpsCfgParam.manualIp[1]&0xff);
		
	}	


	
	if ((!gstrcmp(argv[0],T("wanunnumbered")))&&(MULTIPLE_PPPOE_SUPPORT))
	{
		websWrite(wp,"<tr><td bgColor=#aaddff>Unnumbered PPPoE</td>");
		websWrite(wp,"<td bgColor=#ddeeff><input type=radio name=\"wantype\" value=\"3\" ");
		websWrite(wp,T("%s") ,pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED ? "checked":"   ");
		websWrite(wp,"></td><td bgColor=#eeddff>Wirespeed</td></tr>");
		return 0;
	}	

#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
	if (!gstrcmp(argv[0],"pptpl2tp"))
	{
		websWrite(wp,"<tr><td bgColor=#aaddff>PPTP</td>");
		websWrite(wp,"<td bgColor=#ddeeff><input type=radio name=\"wantype\" value=\"5\" ");
		websWrite(wp,T("%s") ,pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_PPTP ? "checked":"   ");
		websWrite(wp,"></td><td bgColor=#eeddff>Accerelated</td></tr>");
		websWrite(wp,"<tr><td bgColor=#aaddff>L2TP</td>");
		websWrite(wp,"<td bgColor=#ddeeff><input type=radio name=\"wantype\" value=\"6\" ");
		websWrite(wp,T("%s") ,pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_L2TP ? "checked":"   ");
		websWrite(wp,"></td><td bgColor=#eeddff>Accelerated</td></tr>");		
		return 0;
	}

	if (!gstrcmp(argv[0], "dhcpl2tp"))
	{
		websWrite(wp, "<tr><td bgColor=#aaddff>DHCP + L2TP</td>");
		websWrite(wp, "<td bgColor=#ddeeff><input type=radio name=\"wantype\" value=\"7\" ");
		websWrite(wp,T("%s") ,pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_DHCPL2TP ? "checked":"   ");
		websWrite(wp,"></td><td bgColor=#eeddff>Accerelated</td></tr>");
		return 0;
	}
#endif	
	
#ifdef CONFIG_RTL865XB_3G
	if (!strcmp(argv[0], "3g"))
	{
		websWrite(wp, "<tr><td bgColor=#aaddff>3G/GPRS</td>");
		websWrite(wp, "<td bgColor=#ddeeff><input type=radio name=\"wantype\" value=\"8\" ");
		websWrite(wp,T("%s") ,pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_3G ? "checked":"   ");
		websWrite(wp,"></td><td bgColor=#eeddff>Accerelated</td></tr>");
		return 0;
	}
#endif	
	if (!gstrcmp(argv[0],T("dhcpsS")))
	{
		if (websWrite(wp,T("%d %s") ,
			pRomeCfgParam->dhcpsCfgParam.startIp,pRomeCfgParam->dhcpsCfgParam.enable==1 ? "enable":"disable") < 0) 
			return -1;
		else
			return 0;
	}
	if (!gstrcmp(argv[0],T("dhcpsE")))
	{
		if (websWrite(wp,T("%d %s") ,
			pRomeCfgParam->dhcpsCfgParam.endIp,pRomeCfgParam->dhcpsCfgParam.enable==1?"enable":"disable")< 0) 
			return -1;
		else
			return 0;
	}
	
	if (!gstrcmp(argv[0],T("dhcpsToggle")))
	{
		if (websWrite(wp,T("%s") ,
			pRomeCfgParam->dhcpsCfgParam.enable ==1 ?"Enable":"Disable"	 ) < 0) 
			return -1;
		else
			return 0;
	}

	if (!gstrcmp(argv[0],T("dhcps_check")))
	{
		if (websWrite(wp,T("%s") ,
			pRomeCfgParam->dhcpsCfgParam.enable ==1 ? " checked":"") < 0) 
			return -1;
		else
			return 0;
	}
	
	if (!gstrcmp(argv[0],T("hwacc_check")))
	{
		if (websWrite(wp,T("%s") ,
			pRomeCfgParam->natCfgParam.hwaccel ==1 ? " checked":"") < 0) 
			return -1;
		else
			return 0;
	}

	if (!gstrcmp(argv[0],T("ldVer")))
	{


		re865xIoctl("eth0",RTL8651_IOCTL_GETLDVER,0,0,&ver);

		if (websWrite(wp,T("%u.%u.%u"),(ver>>16)&0xff,(ver>>8)&0xff,ver&0xff ) < 0)
			return -1;
		else
			return 0;
	}

	if (!gstrcmp(argv[0],T("romeDrvVer")))
	{
		return (websWrite(wp,T("%s"),ROMEDRIVER_VERSION));
	}
	
	if (!gstrcmp(argv[0],T("hwAcc")))
	{
		if (websWrite(wp,T("%s"),pRomeCfgParam->natCfgParam.hwaccel==1 ? "Enable":"Disable") < 0)
			return -1;
		else 
			return 0;
	}
	
	if (!gstrcmp(argv[0],T("nat")))
	{
		if (websWrite(wp,T("%s"),pRomeCfgParam->natCfgParam.enable==1 ? "Enable":"Disable") < 0)
			return -1;
		else 
			return 0;
	}
	
	if (!gstrcmp(argv[0],T("fwDate")))
	{
		//memcpy(&imgHdr,(void*)FLASH_MAP_RUN_IMAGE_ADDR,sizeof(imgHdr));
#ifdef __mips__
		imgHdr=(fileImageHeader_t *)(FLASH_MAP_RUN_IMAGE_ADDR);
		//,sizeof(imgHdr));
#else
		imgHdr=(fileImageHeader_t *)(shadow);
#endif
		if (websWrite(wp,T("%04u/%02u/%02u %02u:%02u:%02u"),
					*(uint16*)&imgHdr->date,
					*((uint8*)&imgHdr->date+2),
					*((uint8*)&imgHdr->date+3),
					*((uint8*)&imgHdr->time),
					*((uint8*)&imgHdr->time+1),
					*((uint8*)&imgHdr->time+2))<0)
			return -1;
		else
			return 0;
	}
	
	if (!gstrcmp(argv[0],T("lanMac")))
	{
		uint8 mac[8];
#ifdef __mips__
		
		//bdinfo_getMac((macaddr_t*)&mac);
		re865xIoctl("eth1",GET_MAC,(uint32)&mac,0,0);
#endif
		if (websWrite(wp,T("%02X:%02X:%02X:%02X:%02X:%02X"),mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]) < 0)
			return -1;
		else 
			return 0;
	}
	
	if (!gstrcmp(argv[0],T("wanMac")))
	{
		uint8 mac[8];
#ifdef __mips__
		re865xIoctl("eth0",GET_MAC,(uint32)&mac,0,0);
#endif
		if (websWrite(wp,T("%02X:%02X:%02X:%02X:%02X:%02X"),mac[0],mac[1],mac[2],mac[3],mac[4],mac[5])< 0)
			return -1;
		else 
			return 0;
	}


	
	return 0;
} 

/************************************************************
 *	
 *	Status (WAN IP Info)
 *	
 ************************************************************/
int asp_getWanAddress(int ejid, webs_t wp, int argc, char_t **argv)
{
	int i; 
	switch(pRomeCfgParam->ifCfgParam[0].connType)
	{
		case IFCFGPARAM_CONNTYPE_PPPOE:
		case IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED:
		
			if(pRomeCfgParam->pppoeCfgParam[0].dialState!=PPPOECFGPARAM_DIALSTATE_DIALED_SUCCESS) 
			{
				websWrite(wp,"0.0.0.0");
				return 0;
			}
			if (!gstrcmp(argv[0],"wanip"))
				return(websWrite(wp,"%s", inet_string((uint32)&(pRomeCfgParam->ifCfgParam[0].ipAddr))));
			if (!gstrcmp(argv[0],"wanmask"))
				return(websWrite(wp,"%s", inet_string((uint32)&(pRomeCfgParam->ifCfgParam[0].ipMask))));
			if (!gstrcmp(argv[0],"gateway"))
				return(websWrite(wp,"%s", inet_string((uint32)&(pRomeCfgParam->ifCfgParam[0].gwAddr))));
			if (!gstrcmp(argv[0],"dns"))
				return(websWrite(wp,"%s", inet_string((uint32)&(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr))));
			break;
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)					
		case IFCFGPARAM_CONNTYPE_PPTP:	
			if(pRomeCfgParam->pptpCfgParam.dialState!=PPPOECFGPARAM_DIALSTATE_DIALED_SUCCESS) 
			{
				websWrite(wp,"0.0.0.0");
				return 0;
			}
			if (!gstrcmp(argv[0],"wanip"))
				return(websWrite(wp,"%s", inet_string((uint32)&(pRomeCfgParam->ifCfgParam[0].ipAddr))));
			if (!gstrcmp(argv[0],"wanmask"))
				return(websWrite(wp,"%s", inet_string((uint32)&(pRomeCfgParam->ifCfgParam[0].ipMask))));
			if (!gstrcmp(argv[0],"gateway"))
				return(websWrite(wp,"%s", inet_string((uint32)&(pRomeCfgParam->ifCfgParam[0].gwAddr))));
			if (!gstrcmp(argv[0],"dns"))
				return(websWrite(wp,"%s", inet_string((uint32)&(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr))));
			break;

		case IFCFGPARAM_CONNTYPE_L2TP:
		case IFCFGPARAM_CONNTYPE_DHCPL2TP:	
			if(pRomeCfgParam->l2tpCfgParam.dialState!=PPPOECFGPARAM_DIALSTATE_DIALED_SUCCESS)
			{
				websWrite(wp,"0.0.0.0");
				return 0;
			}
			if (!gstrcmp(argv[0],"wanip"))
				return(websWrite(wp,"%s", inet_string((uint32)&(pRomeCfgParam->ifCfgParam[0].ipAddr))));
			if (!gstrcmp(argv[0],"wanmask"))
				return(websWrite(wp,"%s", inet_string((uint32)&(pRomeCfgParam->ifCfgParam[0].ipMask))));
			if (!gstrcmp(argv[0],"gateway"))
				return(websWrite(wp,"%s", inet_string((uint32)&(pRomeCfgParam->ifCfgParam[0].gwAddr))));
			if (!gstrcmp(argv[0],"dns"))
				return(websWrite(wp,"%s", inet_string((uint32)&(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr))));
			break;
#endif			
			
		case IFCFGPARAM_CONNTYPE_STATIC:
		case IFCFGPARAM_CONNTYPE_DHCPC:	
			
			if (!gstrcmp(argv[0],"wanip"))
				return(websWrite(wp,"%s", inet_string((uint32)&(pRomeCfgParam->ifCfgParam[0].ipAddr))));
			if (!gstrcmp(argv[0],"wanmask"))
				return(websWrite(wp,"%s", inet_string((uint32)&(pRomeCfgParam->ifCfgParam[0].ipMask))));
			if (!gstrcmp(argv[0],"gateway"))
				return(websWrite(wp,"%s", inet_string((uint32)&(pRomeCfgParam->ifCfgParam[0].gwAddr))));
			if (!gstrcmp(argv[0],"dns"))
				return(websWrite(wp,"%s", inet_string((uint32)&(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr))));
			break;
		case IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE:
			// print default pppoe info
			for(i=0;i<MAX_PPPOE_SESSION;i++)
			{
				if(pRomeCfgParam->pppoeCfgParam[i].defaultSession==1) 
				{
					if (!gstrcmp(argv[0],"wanip"))
						return(websWrite(wp,"%s",(pRomeCfgParam->pppoeCfgParam[i].dialState==PPPOECFGPARAM_DIALSTATE_DIALED_SUCCESS)?(inet_string((uint32)&(pRomeCfgParam->pppoeCfgParam[i].ipAddr))):"0.0.0.0"));
					if (!gstrcmp(argv[0],"wanmask"))
						return(websWrite(wp,"%s",(pRomeCfgParam->pppoeCfgParam[i].dialState==PPPOECFGPARAM_DIALSTATE_DIALED_SUCCESS)?(inet_string((uint32)&(pRomeCfgParam->pppoeCfgParam[i].ipMask))):"0.0.0.0"));
					if (!gstrcmp(argv[0],"gateway"))
						return(websWrite(wp,"%s",(pRomeCfgParam->pppoeCfgParam[i].dialState==PPPOECFGPARAM_DIALSTATE_DIALED_SUCCESS)?(inet_string((uint32)&(pRomeCfgParam->pppoeCfgParam[i].gwAddr))):"0.0.0.0"));
					if (!gstrcmp(argv[0],"dns"))
						return(websWrite(wp,"%s",(pRomeCfgParam->pppoeCfgParam[i].dialState==PPPOECFGPARAM_DIALSTATE_DIALED_SUCCESS)?(inet_string((uint32)&(pRomeCfgParam->pppoeCfgParam[i].dnsAddr))):"0.0.0.0"));
					break;
				}			

			}
			
			break;
	} //end switch
	return 0;
}

/************************************************************
 *	
 *	DHCPD Server Client List Action
 *	
 ************************************************************/
void asp_dhcpServerAdvance(webs_t wp, char_t *path, char_t *query)
{
	char	*valstr;
	valstr = websGetVar(wp, T("dhcps_switch"), T("")); 
	if (!gstrcmp(valstr,T("on")))
	{
		FILE *f;		
		ramDhcpsCfgParam.enable=1;
		ramDhcpsCfgParam.startIp = (char)atoi(websGetVar(wp,T("dhcps_ippool_start")," "));
		ramDhcpsCfgParam.endIp =   (char)atoi(websGetVar(wp,T("dhcps_ippool_end")," "));	
		ramDhcpsCfgParam.manualIp[0] = (atoi(websGetVar(wp, T("dip_1_1"), " "))<<24)  | (atoi(websGetVar(wp, T("dip_1_2"), " "))<<16) | (atoi(websGetVar(wp, T("dip_1_3"), " "))<<8) | atoi(websGetVar(wp, T("dip_1_4"), " "));
		ramDhcpsCfgParam.manualIp[1] = (atoi(websGetVar(wp, T("dip_2_1"), " "))<<24)  | (atoi(websGetVar(wp, T("dip_2_2"), " "))<<16) | (atoi(websGetVar(wp, T("dip_2_3"), " "))<<8) | atoi(websGetVar(wp, T("dip_2_4"), " "));
		ramDhcpsCfgParam.hardwareAddr[0][0] = strtol(websGetVar(wp, T("mac_1_1"), " "), NULL, 16); //atoi(websGetVar(wp, T("mac_1_1"), " "));
		ramDhcpsCfgParam.hardwareAddr[0][1] = strtol(websGetVar(wp, T("mac_1_2"), " "), NULL, 16);//atoi(websGetVar(wp, T("mac_1_2"), " "));
		ramDhcpsCfgParam.hardwareAddr[0][2] = strtol(websGetVar(wp, T("mac_1_3"), " "), NULL, 16);//atoi(websGetVar(wp, T("mac_1_3"), " "));
		ramDhcpsCfgParam.hardwareAddr[0][3] = strtol(websGetVar(wp, T("mac_1_4"), " "), NULL, 16);//atoi(websGetVar(wp, T("mac_1_4"), " "));
		ramDhcpsCfgParam.hardwareAddr[0][4] = strtol(websGetVar(wp, T("mac_1_5"), " "), NULL, 16);//atoi(websGetVar(wp, T("mac_1_5"), " "));
		ramDhcpsCfgParam.hardwareAddr[0][5] = strtol(websGetVar(wp, T("mac_1_6"), " "), NULL, 16);//atoi(websGetVar(wp, T("mac_1_6"), " "));
		ramDhcpsCfgParam.hardwareAddr[1][0] = strtol(websGetVar(wp, T("mac_2_1"), " "), NULL, 16);//atoi(websGetVar(wp, T("mac_2_1"), " "));
		ramDhcpsCfgParam.hardwareAddr[1][1] = strtol(websGetVar(wp, T("mac_2_2"), " "), NULL, 16);//atoi(websGetVar(wp, T("mac_2_2"), " "));
		ramDhcpsCfgParam.hardwareAddr[1][2] = strtol(websGetVar(wp, T("mac_2_3"), " "), NULL, 16);//atoi(websGetVar(wp, T("mac_2_3"), " "));
		ramDhcpsCfgParam.hardwareAddr[1][3] = strtol(websGetVar(wp, T("mac_2_4"), " "), NULL, 16);//atoi(websGetVar(wp, T("mac_2_4"), " "));
		ramDhcpsCfgParam.hardwareAddr[1][4] = strtol(websGetVar(wp, T("mac_2_5"), " "), NULL, 16);//atoi(websGetVar(wp, T("mac_2_5"), " "));
		ramDhcpsCfgParam.hardwareAddr[1][5] = strtol(websGetVar(wp, T("mac_2_6"), " "), NULL, 16);//atoi(websGetVar(wp, T("mac_2_6"), " "));
	
		f=fopen("/var/run/udhcpd.pid","r");
		if(f==NULL)
		{
			dhcps_start();
			sleep(1); // waiting for /var/run/udhcpd.pid be created.
		}
		else
			fclose(f);
	}	
	else
	{
		ramDhcpsCfgParam.enable=0;
		do_kill("/var/run/udhcpd.pid");		
	}
	
	//if(memcmp(&ramDhcpsCfgParam,&pRomeCfgParam->dhcpsCfgParam,sizeof(ramDhcpsCfgParam))) configChanged|=CONFIG_CHANGE_DHCPS;
	
	memcpy((void*)&(pRomeCfgParam->dhcpsCfgParam),(void*)&ramDhcpsCfgParam,sizeof(pRomeCfgParam->dhcpsCfgParam));
	cfgmgr_write(CFGMGR_TABID_DHCPS,(void*)&(pRomeCfgParam->dhcpsCfgParam), sizeof(pRomeCfgParam->dhcpsCfgParam));
	cfgmgr_task();
	
	websRedirect(wp, T("dhcpd_adv.asp"));
	return;
}

/************************************************************
 *	
 *	DCHP Server Client List Subroutine
 *	
 ************************************************************/
int asp_dhcpServerLeaseList(int ejid, webs_t wp, int argc, char_t **argv)
{
	#define DHCP_CLIENT_FILE 	"/var/dhcps_client"
	FILE *fp;
	char *linebuf;
	char buf1[20],buf2[20],buf3[32];

	send_signal("/var/run/udhcpd.pid",SIGUSR2);
	

	fp = fopen(DHCP_CLIENT_FILE,"r");
	if(fp==NULL) return 0;


	linebuf=malloc(80);

	websWrite(wp, "<tr><td colspan=2><BR><b>Dynamic DHCP Client List</b></td></tr>\n");	
    websWrite(wp, "<tr><td bgColor=#aaddff>Hardware Address</td>");
	websWrite(wp," <td bgColor=#aaddff><font color=black >%s </td>","Assigned IP");
	websWrite(wp," <td bgColor=#aaddff><font color=black >%s </td></tr>","Hostname");
	  while(!feof(fp))	
	{	
		fgets(linebuf,80,fp);
		memset(buf3,0,sizeof(buf3));
		sscanf(linebuf,"%s %s %s",buf1,buf2,buf3);	
        	if (!memcmp(buf1,"@@",2)) break;
		websWrite(wp, "<tr><td bgColor=#ddeeff>%s</td>",buf1);
		websWrite(wp," <td bgColor=#ddeeff><font color=black >%s </td>",buf2);
		if (!buf3[0])
			strcpy(buf3," ");	
		websWrite(wp," <td bgColor=#ddeeff><font color=black >%s </td></tr>",buf3);
	}
	fclose(fp);	
	free(linebuf);
	unlink(DHCP_CLIENT_FILE);
	return 0;
}

/************************************************************
 *	
 *	Event log
 *	
 ************************************************************/

struct circ_buf {
	int size;			
	int head;			
	int tail;			
	char data[1];		
};

static struct circ_buf *getCircBufPtrByModuleId(int moduleId)
{
		int tmp_moduleId = moduleId;
		int idx = 0,i;
		uint32 ptr;
		int shmid;
		FILE *f;
		struct circ_buf *buf;
		
		while (tmp_moduleId>1)
		{
			idx ++;
			tmp_moduleId = tmp_moduleId >> 1;
		}
		
		switch(moduleId)
		{
			case RTL8651_LOGMODULE_SYS:
				if ((shmid = shmget(0x414e4547, 16000+sizeof(struct circ_buf), IPC_CREAT | 0666)) == -1) {
					return -1;
				}
				if ((buf = shmat(shmid, NULL, 0)) == NULL) {
					return -1;
                }				
				break;
			case RTL8651_LOGMODULE_ACL:
			case RTL8651_LOGMODULE_DOS:
			case RTL8651_LOGMODULE_URL:
			case RTL8651_LOGMODULE_NEWFLOW:
				f = fopen("/proc/log_module","r");
				if(f==NULL) return -1;
				for(i=0;i<idx;i++)
					fscanf(f,"%x",&ptr);
				fscanf(f,"%x",&ptr);
				fclose(f);
				if(ptr==0) return -1;
#ifdef __uClinux__
				buf=ptr;	
#else		
				buf=rtl865x_kmmap(ptr-0x80000000,sizeof(struct circ_buf)+16000);
#endif					
				break;	
		}
		return buf;
}

int asp_eventLog(int ejid, webs_t wp, int argc, char_t **argv)
{
	struct circ_buf *buf;
	int show=0;
	int head,size,tail;
	uint32 moduleId=0;
	char module_name[16];
	uint8 ms;

	if(memcmp(websGetVar(wp, "action", ""),"acl",3)==0) {sprintf(module_name,"ACL"); moduleId=RTL8651_LOGMODULE_ACL;}
	else if(memcmp(websGetVar(wp, "action", ""),"dos",3)==0) {sprintf(module_name,"DoS"); moduleId=RTL8651_LOGMODULE_DOS;}
	else if(memcmp(websGetVar(wp, "action", ""),"urlfilter",9)==0) {sprintf(module_name,"URL Filter"); moduleId=RTL8651_LOGMODULE_URL;}
	else if(memcmp(websGetVar(wp, "action", ""),"new_napt",9)==0) {sprintf(module_name,"New NAPT"); moduleId=RTL8651_LOGMODULE_NEWFLOW;}
	else {sprintf(module_name,"System"); moduleId=RTL8651_LOGMODULE_SYS;}	

	websWrite(wp,
		"<html>\n" \
		"<head>\n" \
		"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=big5\">\n" \
		"<script language=javascript>" \
		"function changeMs(a)" \
		"{" \
		"	fn.ms.value=a;" \
		"	fn.submit();" \
		"}" \
		"</script>" \
		"<title>Event Log</title>\n" \
		"</head>\n" \
		"<body bgcolor=\"#ffffff\" text=\"#000000\">\n" \
		"<B>Event Log Configuration</B><BR>" \
		);

	{
		char *msargv[]={"ms"};
		if(moduleId==RTL8651_LOGMODULE_SYS) ms=0;
		else	 ms=asp_multiSession(wp,1,msargv);
	}
	 
	websWrite(wp,		
		"<form action=\"eventlog.asp\" name=\"fn\"><input type=hidden name=action value=\"%s\"><input type=hidden name=ms value=0></form>" \
		"<form action=\"/goform/asp_setEventLog\">\n" \
		"<input type=hidden name=ms value=%d>\n" \
		"<table cellSpacing=1 cellPadding=2 border=0><tr><td bgColor=#aaddff>%s Log</td><td bgColor=#ddeeff><input type=checkbox name=enable value=1 %s>Enable<input type=hidden name=\"moduleId\" value=%d></td>\n" \
		"<tr><td colspan=2 align=center><input type=submit name=smt value=\"Apply\"></td></tr></table><HR><BR>",websGetVar(wp, "action", ""),ms,module_name,(pRomeCfgParam->logCfgParam[ms].module&moduleId)?"checked":"",moduleId);

	if(pRomeCfgParam->logCfgParam[ms].module&moduleId)
	{
		uint32 index=0;
		int i=-1;


		buf=getCircBufPtrByModuleId(moduleId);


		websWrite(wp,
			"<table cellSpacing=1 cellPadding=2 border=0>\n" \
			"<tr>\n" \
			"	<td bgColor=#aaddff>%s Log</td>\n" \
			"</tr>\n",module_name);

		
		if(buf==-1) 
		{
			return 0;
		}
	
		head=buf->head;
		size=buf->size;
		tail=buf->tail;
			
		if(head>tail) tail+=size;

		websWrite(wp,"<tr><td bgColor=#ddeeff nowrap><font size=-1>");

		/*
			determine show message or not according to the "prefix" of each message:
				show message only when this message is not dsid related ('x') or the dsid is same as the dsid(it's "ms") we select now.
		*/
		if((buf->data[head]==(ms+'0')) || buf->data[head] == 'x')
			show = 1;
		else
			show = 0;
		if(moduleId!=RTL8651_LOGMODULE_SYS) head++;

		for( i = head ; i < tail ; i++)
		{
			int idx = i;
			while (idx >= size)
				idx -= size;

			if (buf->data[idx] == 13)
			{	/* CR: Carriage Return, we ignore it */ 
				continue;
			}

			if(buf->data[idx] == 10) 
			{	/* LF: Line Feed, we add new line in html form */
				if(show == 1 || moduleId == RTL8651_LOGMODULE_SYS)
					websWrite(wp,"</font></td></tr>\n<tr><td bgColor=#ddeeff nowrap><font size=-1>");
				continue;
			}	
			if(buf->data[idx] == 0) 
			{	/* end of string: check next message */
				if((buf->data[(idx+1)%size] == (ms+'0')) || (buf->data[(idx+1)%size] == 'x'))
					show = 1;
				else
					show = 0;
				
				if(moduleId != RTL8651_LOGMODULE_SYS)
					i++;
				continue;						
			}
			if(show == 1||moduleId == RTL8651_LOGMODULE_SYS) websWrite(wp,"%c",buf->data[idx]);
		}						

		websWrite(wp,					
			"</font></td><tr><tr><td align=center>" \
			"<input type=submit name=smt value=\"Save\">&nbsp;&nbsp;&nbsp;&nbsp;" \
			"<input type=submit name=smt value=\"Clear\"></td></tr></form></body></html>\n");

		if(moduleId==RTL8651_LOGMODULE_SYS)
		{
			shmdt(buf);
		}
	}
}

void asp_setEventLog(webs_t wp, char_t *path, char_t *query)
{
	FILE *f;
	uint32 i,shmid;
	uint32 moduleId;
	uint32 tail,size,head;
	char tmpstr[32];
	int	ms=atoi(websGetVar(wp,"ms",""));

	moduleId=atoi(websGetVar(wp,"moduleId",""));
	
	//clear event log
	if(strcmp(websGetVar(wp,"smt",""),"Apply")==0)
	{	
		if(atoi(websGetVar(wp,"enable",""))==1)
		{
			pRomeCfgParam->logCfgParam[ms].module|=moduleId;
			rtl8651a_enableLogging(ms,moduleId, 1);
			switch(moduleId)
			{
				case RTL8651_LOGMODULE_SYS:
					f=fopen("/var/run/syslogd.pid","r");
					if(f==NULL)
					{
						system("/bin/syslogd &");
						system("/bin/klogd &");
					}				
					else
					{			
						fclose(f);
					}					
					break;
				default:
			}
		}
		else //disable
		{
			int pid=0;
			pRomeCfgParam->logCfgParam[ms].module&=(~moduleId);
			rtl8651a_enableLogging(ms,moduleId, 0);
			switch(moduleId)
			{
				case RTL8651_LOGMODULE_SYS:					
					f=fopen("/var/run/syslogd.pid","r");
					if(f!=NULL)
					{
						fscanf(f,"%d",&pid);
						kill(pid,15);
						remove("/var/run/syslogd.pid");						
						fclose(f);
					}
					f=fopen("/var/run/klogd.pid","r");
					if(f!=NULL)
					{
						fscanf(f,"%d",&pid);
						kill(pid,15);
						remove("/var/run/klogd.pid");
						fclose(f);
					}
					break;
			}
		}
		
		/* save to cfgmgr */
		cfgmgr_write(CFGMGR_TABID_LOG,(void*)(pRomeCfgParam->logCfgParam),sizeof(struct logCfgParam_s)*MAX_PPPOE_SESSION);
		cfgmgr_task();
	}

	else if(strcmp(websGetVar(wp,"smt",""),"Save")==0)
	{
		int show=0;
		struct circ_buf *buf;		

		buf=getCircBufPtrByModuleId(moduleId);
		if(buf!=-1)
		{
			tail=buf->tail;
			head=buf->head;
			size=buf->size;
		
			f=fopen("/var/system.log","w");			

			if(head>tail) tail+=size;
			fprintf(f,"Event Log:\r\n\r\n");

			if(buf->data[head]==(ms+'0')) show=1;
			if(moduleId!=RTL8651_LOGMODULE_SYS) head++;
			for(i=head;i<tail;i++)
			{
				int idx=i;
				while(idx>=size) idx-=size;
				if(buf->data[idx]==10) 
				{
					if(show==1||moduleId==RTL8651_LOGMODULE_SYS)
					{ fputc(0x0d,f); fputc(0x0a,f); }
					continue; 
				}
				if(buf->data[idx]==0) 
				{
					if(buf->data[(idx+1)%size]==(ms+'0')) show=1;
					else show=0;
					if(moduleId!=RTL8651_LOGMODULE_SYS) i++;	
					continue;
				}
				fputc(buf->data[idx],f);
			}						
			fclose(f);
		}
		
		if(moduleId==RTL8651_LOGMODULE_SYS)
		{
			shmdt(buf);
		}		
		websRedirect(wp, T("system.log"));
		return;
	}

	else //CLEAR
	{			
		struct circ_buf *buf;		
	
		buf=getCircBufPtrByModuleId(moduleId);		
		if(buf!=-1)
		{
			buf->head=0;
			buf->tail=0;
		}
		if(moduleId==RTL8651_LOGMODULE_SYS)
		{
			shmdt(buf);
		}	
		
	}

	switch(moduleId)
	{
		case RTL8651_LOGMODULE_SYS:	
			sprintf(tmpstr,"eventlog.asp?action=system&ms=%d",ms);
			break;
		case RTL8651_LOGMODULE_ACL:
			sprintf(tmpstr,"eventlog.asp?action=acl&ms=%d",ms);
			break;
		case RTL8651_LOGMODULE_DOS:
			sprintf(tmpstr,"eventlog.asp?action=dos&ms=%d",ms);
			break;
		case RTL8651_LOGMODULE_URL:
			sprintf(tmpstr,"eventlog.asp?action=urlfilter&ms=%d",ms);
			break;
		case RTL8651_LOGMODULE_NEWFLOW:
			sprintf(tmpstr,"eventlog.asp?action=new_napt&ms=%d",ms);
			break;
	}

	websRedirect(wp, tmpstr);
	return;
	
}

/************************************************************
 *	
 *	DMZ host
 *	
 ************************************************************/
int asp_dmz(int ejid, webs_t wp, int argc, char_t **argv)
{

	int ms=asp_multiSession(wp,argc,argv);
	
 	if(strcmp(argv[0],"dmzIp")==0)
 	{
 		uint8 *iplist;
 		iplist=(uint8 *)&(pRomeCfgParam->dmzCfgParam[ms].dmzIp);
 		if(strcmp(argv[1],"1")==0)
 			websWrite(wp,"%u",*iplist);
 		else if(strcmp(argv[1],"2")==0)
 			websWrite(wp,"%u",*(iplist+1));
		else if(strcmp(argv[1],"3")==0)
 			websWrite(wp,"%u",*(iplist+2));
 		else if(strcmp(argv[1],"4")==0)
 			websWrite(wp,"%u",*(iplist+3));
	}
 	else if(strcmp(argv[0],"MS")==0)
 	{
 		websWrite(wp,"%d",ms);
 	}
	else if(strcmp(argv[0],"enable")==0)
	{
		if(pRomeCfgParam->dmzCfgParam[ms].enable==1) websWrite(wp,"checked");
	}
	return 0;
}

void asp_setDmz(webs_t wp, char_t *path, char_t *query)
{
	uint32	ipaddr=0;
	ipaddr_t	naptIp=0;
	uint8	tempip;
	char		tmpstr[32]={0};	
	int		ms=atoi(websGetVar(wp,"ms",""));

	if(pRomeCfgParam->ifCfgParam[0].connType!=IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)
	{
		memcpy(&naptIp,pRomeCfgParam->ifCfgParam[0].ipAddr,4);		
	}
	else //IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE
	{
		memcpy((void*)&naptIp,pRomeCfgParam->pppoeCfgParam[ms].ipAddr,4);		
	}
	
	tempip=atoi(websGetVar(wp,"ip1",""));
	ipaddr|=((tempip<<24)&0xff000000);
	tempip=atoi(websGetVar(wp,"ip2",""));
	ipaddr|=((tempip<<16)&0xff0000);
	tempip=atoi(websGetVar(wp,"ip3",""));
	ipaddr|=((tempip<<8)&0xff00);
	tempip=atoi(websGetVar(wp,"ip4",""));
	ipaddr|=(tempip&0xff);	
	pRomeCfgParam->dmzCfgParam[ms].dmzIp=ipaddr;
	
	if(atoi(websGetVar(wp,"enable",""))==1)
	{
		int r;
		
		rtl8651a_delDmzHost(ms,(ipaddr_t)naptIp);
		r=rtl8651a_addDmzHost(ms,(ipaddr_t)naptIp, (ipaddr_t)ipaddr);
		if(r==0) pRomeCfgParam->dmzCfgParam[ms].enable=1;
	}	
	else
	{		
		rtl8651a_delDmzHost(ms,(ipaddr_t)naptIp);
		pRomeCfgParam->dmzCfgParam[ms].enable=0;
	}
		
	cfgmgr_write(CFGMGR_TABID_DMZ,(void*)pRomeCfgParam->dmzCfgParam, sizeof(struct dmzCfgParam_s)*MAX_PPPOE_SESSION);
	cfgmgr_task();

	sprintf(tmpstr,"dmz.asp?ms=%d",ms);	
	websRedirect(wp, tmpstr);
	
}

/************************************************************
 *	
 *	Special Application
 *	
 ************************************************************/
int asp_specialApplication(int ejid, webs_t wp, int argc, char_t **argv)
{
	int i;

	int ms=asp_multiSession(wp,argc,argv);

	if(strcmp(argv[0],"JS")==0)
	{
		websWrite(wp,	
		"function checkRange()\n" \
		"{\n" \
		" var row_e=%d,max_rows=%d;\n" \
		" var type_idx=%d,trigger_s=%d,trigger_f=%d,enable_idx=%d;\n" \
		" for(i=0;i<max_rows-1;i++)\n",7,MAX_SERVER_PORT,3,4,5,6);
	
		websWrite(wp,
		" {\n" \
		"   if(!formname.elements[i*row_e+enable_idx].checked) continue;\n" \
		"  for(j=i+1;j<max_rows;j++)\n" \
		"  {\n" \
		"  var itype=parseInt(formname.elements[i*row_e+type_idx].value);\n" \
		"  var jtype=parseInt(formname.elements[j*row_e+type_idx].value);\n" \
		"   if(!formname.elements[j*row_e+enable_idx].checked) continue;\n" \
		"  if(itype==jtype) {\n" \
		"    var flag=0;\n" \
		"    var istart=parseInt(formname.elements[i*row_e+trigger_s].value);\n" \
		"    var jstart=parseInt(formname.elements[j*row_e+trigger_s].value);\n" \
		"    var ifinish=parseInt(formname.elements[i*row_e+trigger_f].value);\n" \
		"    var jfinish=parseInt(formname.elements[j*row_e+trigger_f].value);\n" \
		"    if((jstart>=istart)&&(jstart<=ifinish)) flag=1;\n" \
		"    if((jfinish>=istart)&&(jfinish<=ifinish)) flag=1;\n" \
		"    if((jstart<=istart)&&(jfinish>=ifinish)) flag=1;\n" \
		"    if(flag==1) { alert('Trigger port range overlap!'); return false;} }\n" \
		"  }\n" \
		" }\n" \
		"}\n" \
		"</script>\n");
	}
	else if(strcmp(argv[0],"content")==0)
	{
	
		websWrite(wp,
		"<input type=hidden name=ms value=%d>\n" \
/*	"onSubmit=\"return checkRange();\"" \ */
		"<table cellSpacing=1 cellPadding=2 border=0>\n" \
		"<tr><td bgColor=#aaddff>Name</td><td bgColor=#aaddff>Incoming<BR>Type</td><td bgColor=#aaddff>Incoming Port Range</td><td bgColor=#aaddff>Trigger<BR>Type</td><td bgColor=#aaddff>Trigger<BR>Start Port</td><td bgColor=#aaddff>Trigger<BR>Finish Port</td><td bgColor=#aaddff>Enable</td></tr>\n",ms);

	for(i=1;i<=MAX_SPECIAL_AP;i++)
	{
		//name
		websWrite(wp,"<tr><td bgColor=#ddeeff><input type=text name=n%02u size=15 maxlength=15 value=\"%s\"></td>\n",i,pRomeCfgParam->specialapCfgParam[ms][i-1].name);
		
		//incoming type
		websWrite(wp,"<td bgColor=#ddeeff><select name=it%02u><option value=0 %s>TCP</option><option value=1 %s>UDP</option><option value=2 %s>BOTH</option></td>\n",i,(pRomeCfgParam->specialapCfgParam[ms][i-1].inType==SPECIALACPCFGPARAM_PROTOCOL_TCP)?"selected":"",(pRomeCfgParam->specialapCfgParam[ms][i-1].inType==SPECIALACPCFGPARAM_PROTOCOL_UDP)?"selected":"",(pRomeCfgParam->specialapCfgParam[ms][i-1].inType==SPECIALACPCFGPARAM_PROTOCOL_BOTH)?"selected":"");

		//incoming start port
		websWrite(wp,"<td bgColor=#ddeeff><input type=text name=ir%02u size=40 maxlength=80 value=\"%s\"></td>\n",i,pRomeCfgParam->specialapCfgParam[ms][i-1].inRange);
		
		//outgoing type
		websWrite(wp,"<td bgColor=#ddeeff><select name=ot%02u><option value=0 %s>TCP</option><option value=1 %s>UDP</option><option value=2 %s>BOTH</option></td>\n",i,(pRomeCfgParam->specialapCfgParam[ms][i-1].outType==SPECIALACPCFGPARAM_PROTOCOL_TCP)?"selected":"",(pRomeCfgParam->specialapCfgParam[ms][i-1].outType==SPECIALACPCFGPARAM_PROTOCOL_UDP)?"selected":"",(pRomeCfgParam->specialapCfgParam[ms][i-1].outType==SPECIALACPCFGPARAM_PROTOCOL_BOTH)?"selected":"");

		//outgoing start port
		websWrite(wp,"<td bgColor=#ddeeff><input type=text name=os%02u size=5 maxlength=5 value=%u></td>\n",i,pRomeCfgParam->specialapCfgParam[ms][i-1].outStart);
		
		//outgoing finish port
		websWrite(wp,"<td bgColor=#ddeeff><input type=text name=of%02u size=5 maxlength=5 value=%u></td>\n",i,pRomeCfgParam->specialapCfgParam[ms][i-1].outFinish);

		//enable
		websWrite(wp,"<td bgColor=#ddeeff><input type=checkbox name=e%02u value=1 %s></td></tr>\n",i,(pRomeCfgParam->specialapCfgParam[ms][i-1].enable==1)?"checked":"");
	} //end for
    }

		
	return 0;
}

void asp_setSpecialApplication(webs_t wp, char_t *path, char_t *query)
{
	char tmpstr[5]={0};
	char tmpstr2[32]={0};
	uint8 i,j,shift;
	int8 in_range[SPECIAL_AP_MAX_IN_RANGE];
	int ms=atoi(websGetVar(wp,"ms",""));
	
	// delete all trigger port
	//TriggerPortLinkCleanup();

	for(i=0;i<MAX_SPECIAL_AP;i++)
	{
		
		//name
		sprintf(tmpstr,"n%02u",i+1);
		memset(pRomeCfgParam->specialapCfgParam[ms][i].name,0,sizeof(pRomeCfgParam->specialapCfgParam[ms][i].name));
		sprintf(tmpstr2,"%s",websGetVar(wp,tmpstr,""));
		memcpy(pRomeCfgParam->specialapCfgParam[ms][i].name,tmpstr2,sizeof(pRomeCfgParam->specialapCfgParam[ms][i].name)+1);
			
		//incoming type
		sprintf(tmpstr,"it%02u",i+1);
		pRomeCfgParam->specialapCfgParam[ms][i].inType=atoi(websGetVar(wp,tmpstr,""));
		
		//incoming port range
		sprintf(tmpstr,"ir%02u",i+1);
		pRomeCfgParam->specialapCfgParam[ms][i].inRange[0]=0;
		
		memcpy(in_range,(void *)websGetVar(wp,tmpstr,""),strlen(websGetVar(wp,tmpstr,""))+1);
		shift=0;
		for(j=0;j<SPECIAL_AP_MAX_IN_RANGE;j++)
		{
			if(((in_range[j]>='0')&&(in_range[j]<='9'))||(in_range[j]==',')||(in_range[j]=='-')||(in_range[j]==0)||(in_range[j]==' '))
			{
				if(in_range[j]==' ') in_range[j]=',';
				pRomeCfgParam->specialapCfgParam[ms][i].inRange[shift++]=in_range[j];
				if(in_range[j]==0) break;			
			}		
		}

		//outgoing type		
		sprintf(tmpstr,"ot%02u",i+1);
		pRomeCfgParam->specialapCfgParam[ms][i].outType=atoi(websGetVar(wp,tmpstr,""));
		
		//outgoing start port
		sprintf(tmpstr,"os%02u",i+1);
		pRomeCfgParam->specialapCfgParam[ms][i].outStart=atoi(websGetVar(wp,tmpstr,""));
			
		//outgoing finish port
		sprintf(tmpstr,"of%02u",i+1);
		pRomeCfgParam->specialapCfgParam[ms][i].outFinish=atoi(websGetVar(wp,tmpstr,""));

		//enable			
		sprintf(tmpstr,"e%02u",i+1);
		pRomeCfgParam->specialapCfgParam[ms][i].enable=atoi(websGetVar(wp,tmpstr,""));
			
	} /* end for i */
	
	
	// the packets of special application will trap to CPU by ACL
	acl_tableDriverAccess();
	
	/* save to cfgmgr */
	cfgmgr_write(CFGMGR_TABID_SPECIALAP,(void*)pRomeCfgParam->specialapCfgParam,sizeof(struct specialapCfgParam_s)*MAX_PPPOE_SESSION*MAX_SPECIAL_AP);
	cfgmgr_task();

	sprintf(tmpstr2,"specialap.asp?ms=%d\n",ms);
	websRedirect(wp, tmpstr2);	
}

/************************************************************
 *	
 *	UPnP
 *	
 ************************************************************/
int asp_upnp(int ejid, webs_t wp, int argc, char_t **argv)
{
 	if(pRomeCfgParam->upnpCfgParam.enable==1) websWrite(wp,"checked");
 	return 0;
}

#if 0 // PSEUDO_ICSD
#ifdef __uClinux__
#define USERAPP_NOMMU
#endif
#ifdef USERAPP_NOMMU
void asp_setUpnp(webs_t wp, char_t *path, char_t *query)
{
	FILE *f;
	
	pRomeCfgParam->upnpCfgParam.enable = atoi(websGetVar(wp,"enable",""));
	
	if(pRomeCfgParam->upnpCfgParam.enable == 1) //enable
	{
		upnp_start();
	}
	else //disable
	{
		int pid=0;
						
		f=fopen("/var/run/pseudoicsd.pid","r");
		if(f != NULL)
		{
			
			char *argv[6];

			/*
				Stop upnp daemon
			*/
			fscanf(f,"%d",&pid);
			fclose(f);
			if (kill(pid,SIGTERM))
			{
				// process not exist: remove pid file
				printf("pseudoicsd.pid pid error: remove it!\n");
				unlink("/var/run/pseudoicsd.pid");
			}

			/*
				del upnp route
			*/
			argv[0]="-net";
			argv[1]="239.0.0.0";
			argv[2]="netmask";
			argv[3]="255.0.0.0";
			argv[4]="eth1";
			argv[5]=NULL;
			INET_setroute(RTACTION_DEL, 0, argv);
//			if(remove("/var/run/pseudoicsd.pid")) printf("Can't remove /var/run/pseudoicsd.pid\n");	
		}
		
	}

	cfgmgr_write(CFGMGR_TABID_UPNP,(void*)&(pRomeCfgParam->upnpCfgParam), sizeof(pRomeCfgParam->upnpCfgParam));
	cfgmgr_task();
	
	websRedirect(wp, T("upnp.asp"));
}

#else /* USERAPP_NOMMU */
void asp_setUpnp(webs_t wp, char_t *path, char_t *query)
{
	FILE *f;
	
	pRomeCfgParam->upnpCfgParam.enable=atoi(websGetVar(wp,"enable",""));
	
	if(pRomeCfgParam->upnpCfgParam.enable==1) //enable
	{
		upnp_start();
	}
	else //disable
	{
		int pid=0;
						
		f=fopen("/var/run/pseudoicsd.pid","r");
		if(f!=NULL)
		{
			
			char *argv[6];
			fscanf(f,"%d",&pid);
			kill(pid,SIGTERM);
			
			// del upnp route
			argv[0]="-net";
			argv[1]="239.0.0.0";
			argv[2]="netmask";
			argv[3]="255.0.0.0";
			argv[4]="eth1";
			argv[5]=NULL;
			INET_setroute(RTACTION_DEL, 0, argv);
			fclose(f);
			if(remove("/var/run/pseudoicsd.pid")) printf("Can't remove /var/run/pseudoicsd.pid\n");	
		}
		
	}

	cfgmgr_write(CFGMGR_TABID_UPNP,(void*)&(pRomeCfgParam->upnpCfgParam), sizeof(pRomeCfgParam->upnpCfgParam));
	cfgmgr_task();
	
	websRedirect(wp, T("upnp.asp"));
}
#endif /* USERAPP_NOMMU */
#else // LINUX-IGD
void asp_setUpnp(webs_t wp, char_t *path, char_t *query)
{
	FILE *f;
	
	pRomeCfgParam->upnpCfgParam.enable=atoi(websGetVar(wp,"enable",""));
	
	if(pRomeCfgParam->upnpCfgParam.enable==1) //enable
	{
		upnp_start();
	}
	else //disable
	{
		upnp_stop();		
	}

	cfgmgr_write(CFGMGR_TABID_UPNP,(void*)&(pRomeCfgParam->upnpCfgParam), sizeof(pRomeCfgParam->upnpCfgParam));
	cfgmgr_task();
	
	websRedirect(wp, T("upnp.asp"));
}
#endif
/************************************************************
 *	
 *	DDNS
 *	
 ************************************************************/
int asp_ddns(int ejid, webs_t wp, int argc, char_t **argv)
{
	if(strcmp(argv[0],"enable")==0)	
 		if(pRomeCfgParam->ddnsCfgParam.enable==1) websWrite(wp,"checked");
 	if(strcmp(argv[0],"username")==0)
 		websWrite(wp,"%s",pRomeCfgParam->ddnsCfgParam.username);
	if(strcmp(argv[0],"password")==0)
 		websWrite(wp,"%s",pRomeCfgParam->ddnsCfgParam.password);
	if(strcmp(argv[0],"hosts")==0)
	{
		FILE *f;
		char c;
 		f=fopen("/var/run/no-ip2.conf","rb");
		if(f!=NULL)
		{
			while(!feof(f))
			{
				c=fgetc(f);
				if(feof(f)) break;
				if(c=='h')
				{
					c=fgetc(f);
					if(feof(f)) break;
					if(c=='[')
					{
						c=fgetc(f);
						if(feof(f)) break;
						if(c==']')
						{
							c=fgetc(f);
							if(feof(f)) break;
							if(c=='=')
							{
								while(1)
								{
									c=fgetc(f);
									if(c=='&') { websWrite(wp,"<BR>"); break;}
									if(feof(f)) break;
									websWrite(wp,"%c",c);
								}
							}							
						}
					}
				}
			}
			
			fclose(f);
		}
		else
		{
			websWrite(wp,"&lt;NONE&gt;");
		}
	}
 	return 0;		
}

void asp_setDdns(webs_t wp, char_t *path, char_t *query)
{
	pRomeCfgParam->ddnsCfgParam.enable=atoi(websGetVar(wp,"enable",""));
	sprintf(pRomeCfgParam->ddnsCfgParam.username,"%s",websGetVar(wp,"username",""));
	sprintf(pRomeCfgParam->ddnsCfgParam.password,"%s",websGetVar(wp,"password",""));
	
	if(pRomeCfgParam->ddnsCfgParam.enable==1)
	{
		ddns_start();
	}
	else
	{
		remove("/var/run/no-ip2.conf");
		do_kill("/var/run/no-ip2.pid");		
	}
	
	cfgmgr_write(CFGMGR_TABID_DDNS,(void*)&(pRomeCfgParam->ddnsCfgParam), sizeof(pRomeCfgParam->ddnsCfgParam));
	cfgmgr_task();
	
	websRedirect(wp, T("ddns.asp"));
}


/************************************************************
 *	
 *	Wizard Save to Flash and Restart (Local Function)
 *	
 ************************************************************/
void wizardSaveToFlash(uint32 connType)
{
	int result;
	switch(connType)
	{
	case IFCFGPARAM_CONNTYPE_DHCPC:		
		cfgmgr_write(CFGMGR_TABID_DHCPC,(void*)&ramDhcpcCfgParam,sizeof(pRomeCfgParam->dhcpcCfgParam));
		break;
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
	case IFCFGPARAM_CONNTYPE_PPTP:
		cfgmgr_write(CFGMGR_TABID_PPTP,(void*)&ramPptpCfgParam,sizeof(pRomeCfgParam->pptpCfgParam));
		break;
	case IFCFGPARAM_CONNTYPE_L2TP:
	case IFCFGPARAM_CONNTYPE_DHCPL2TP:		
		cfgmgr_write(CFGMGR_TABID_L2TP,(void*)&ramL2tpCfgParam,sizeof(pRomeCfgParam->l2tpCfgParam));
		break;
#endif		
	case IFCFGPARAM_CONNTYPE_PPPOE:		
	case IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED:
		cfgmgr_write(CFGMGR_TABID_PPPOE,(void*)ramPppoeCfgParam,sizeof(pppoeCfgParam_t)*MAX_PPPOE_SESSION);
		break;
	}	
	memcpy((char*)&(pRomeCfgParam->ifCfgParam[1]),(char*)&ramLanCfgParam,sizeof(ramLanCfgParam));
	memcpy((char*)&(pRomeCfgParam->ifCfgParam[0]),(char*)&ramWanCfgParam,sizeof(ramWanCfgParam));
	result = cfgmgr_write(CFGMGR_TABID_VLAN,(void*)&(pRomeCfgParam->vlanCfgParam), sizeof(struct vlanCfgParam_s)*VLAN_LOAD_COUNT);
	result = cfgmgr_write(CFGMGR_TABID_IF,(void*)pRomeCfgParam->ifCfgParam,sizeof(ifCfgParam_t)*MAX_IP_INTERFACE);
	result = cfgmgr_write(CFGMGR_TABID_DHCPS,(void*)&ramDhcpsCfgParam,sizeof(dhcpsCfgParam_t));
	//DEBUG_P("write result %d\n", result);
	cfgmgr_task();
	

}

/************************************************************
 *	
 *	LAN IP Setup (Wizard)
 *	
 ************************************************************/
void asp_setLan(webs_t wp, char_t *path, char_t *query)
{
	//FILE *fp;
	int 	i;
	int	val;
	char	*valstr;	
	char	sstr[8];
	//char  *buf;
	//char_t	*name, *address;

	//name = websGetVar(wp, T("name"), T("Joe Smith")); 
	val=0;
	for (i=0;i<4;i++)
	{
		memset(sstr,0,sizeof(sstr));
		sprintf(sstr,"lanip_%d",i+1);
		valstr = websGetVar(wp, T(sstr), T("")); 
		ramLanCfgParam.ipAddr[i]= atoi(valstr);
	}
	for (i=0;i<4;i++)
	{
		memset(sstr,0,sizeof(sstr));
		sprintf(sstr,"lansm_%d",i+1);
		valstr = websGetVar(wp, T(sstr), T("")); 
		ramLanCfgParam.ipMask[i]= atoi(valstr);
	}

	valstr = websGetVar(wp, T("dhcps_switch"), T("")); 
	if (!gstrcmp(valstr,T("on")))
	{
		ramDhcpsCfgParam.enable=1;
		ramDhcpsCfgParam.startIp = (char)atoi(websGetVar(wp,T("dhcps_ippool_start")," "));
		ramDhcpsCfgParam.endIp =   (char)atoi(websGetVar(wp,T("dhcps_ippool_end")," "));
	
	}	
	else
		ramDhcpsCfgParam.enable=0;
	websRedirect(wp, T("wan.asp"));
	return;
	
	/*
	fp = fopen("/www/wan.asp","r");
	buf = malloc(MAX_LINE_SIZE);
	if (!fp || !buf)
	{
		websError(wp, 400, T("Insufficient args\n"));
		return 1;
	}
	while (!feof(fp))
	{
		fgets(buf,MAX_LINE_SIZE,fp);
		websWrite(wp,T("%s"),buf);
	}
	free (buf);
	fclose(fp);
	websDone(wp, 200);
	*/
}

/************************************************************
 *	
 *	WAN IP Setup (Wizard)
 *	
 ************************************************************/
void asp_wanIp(webs_t wp, char_t *path, char_t *query)
{

	ramWanCfgParam.connType=atoi(websGetVar(wp,"wantype",""));
	switch(ramWanCfgParam.connType)
	{
		case IFCFGPARAM_CONNTYPE_PPPOE :
			websRedirect(wp, T("pppoe.asp?pppoeType=1&wizard=1"));
			break;	
		case IFCFGPARAM_CONNTYPE_DHCPC :
			websRedirect(wp, T("dhcpc.asp"));
			break;
		case IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED :
			websRedirect(wp, T("pppoe.asp?pppoeType=3&wizard=3"));
			break;
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)			
		case IFCFGPARAM_CONNTYPE_PPTP:
			websRedirect(wp, T("wanpptp.asp?wizard=1"));
			break;
		case IFCFGPARAM_CONNTYPE_L2TP:
			websRedirect(wp, T("wanl2tp.asp?wizard=1"));
			break;
		case IFCFGPARAM_CONNTYPE_DHCPL2TP:
			websRedirect(wp, T("wandhcpl2tp.asp?wizard=1"));
			break;			
#endif			
		default:						
			memcpy((void*)pRomeCfgParam->ifCfgParam[0].ipAddr,(void*)ramWanCfgParam.ipAddr,4);
			memcpy((void*)pRomeCfgParam->ifCfgParam[0].ipMask,(void*)ramWanCfgParam.ipMask,4);
			memcpy((void*)pRomeCfgParam->ifCfgParam[0].gwAddr,(void*)ramWanCfgParam.gwAddr,4);
			memcpy((void*)pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,(void*)ramWanCfgParam.dnsPrimaryAddr,4);
			websRedirect(wp, T("wanstatic.asp"));
			break;
	}
	return;
	/*
	fp = fopen("/www/wanstatic.asp","r");
	buf = malloc(MAX_LINE_SIZE);
	if (!fp || !buf)
	{
		websError(wp, 400, T("Insufficient args\n"));
		return ;
	}
	while (!feof(fp))
	{
		fgets(buf,MAX_LINE_SIZE,fp);
		websWrite(wp,T("%s"),buf);
	}
	free (buf);
	fclose(fp);
	//websFooter(wp);
	websDone(wp, 200);
	*/
	
	/*
 	 *	Extract the form name
 	 */
}

/************************************************************
 *	
 *	Static IP (Wizard)
 *	
 ************************************************************/
void asp_setStaticWizard(webs_t wp, char_t *path, char_t *query)
{
	int 	i;
	int	    val;
	char	sstr[8]={0};
	
	val=0;
	for (i=0;i<4;i++)
	{
		sprintf(sstr,"wanip_%d",i+1);
		ramWanCfgParam.ipAddr[i]= atoi(websGetVar(wp, sstr, ""));

		sprintf(sstr,"wandns_%d",i+1);
		ramWanCfgParam.dnsPrimaryAddr[i]= atoi(websGetVar(wp, sstr, ""));

		sprintf(sstr,"wangw_%d",i+1);
		ramWanCfgParam.gwAddr[i]= atoi(websGetVar(wp, sstr, ""));
	
		sprintf(sstr,"wansm_%d",i+1);
		ramWanCfgParam.ipMask[i]= atoi(websGetVar(wp, sstr, ""));
	}
	
	wizardSaveToFlash(IFCFGPARAM_CONNTYPE_STATIC);
	Sys_Restart(wp);	
}

#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
/************************************************************
 *	
 *	PPTP Client Setup Function (Wizard)
 *	
 ************************************************************/
void  asp_setPptpWizard(webs_t wp, char_t *path, char_t *query)
{
	int	i;
	int	val=0;
	int	wizard=atoi(websGetVar(wp,"wizard",""));
	char	*valstr;
	char	sstr[8]={0};

	if(strcmp(websGetVar(wp,"submit2",""),"Refresh")==0)
	{
		websRedirect(wp,"wanpptp.asp");
		return;
	}
	else if(strcmp(websGetVar(wp,"submit2",""),"Dial")==0)
	{
		pptp_start(0);		
		sleep(1);
		websRedirect(wp,"wanpptp.asp");
		return;
	}
	else if(strcmp(websGetVar(wp,"submit2",""),"Hang Up")==0)
	{
		send_signal("/var/run/pppd0.pid",1);
		sleep(1);
#ifdef __uClinux__
#else
		send_signal("/var/run/pptp.pid",9);	
#endif
		pRomeCfgParam->pptpCfgParam.dialState=PPTPCFGPARAM_DIALSTATE_OFF;
		websRedirect(wp,"wanpptp.asp");		
		return;
	}	
	else
	{
		memcpy(&ramPptpCfgParam,&pRomeCfgParam->pptpCfgParam,sizeof(ramPptpCfgParam));
		for (i=0;i<4;i++)
		{
			sprintf(sstr,"wanip_%d",i+1);
			ramPptpCfgParam.ipAddr[i]= atoi(websGetVar(wp, sstr, ""));

			sprintf(sstr,"wansv_%d",i+1);
			ramPptpCfgParam.svAddr[i]= atoi(websGetVar(wp, sstr, ""));
	
			sprintf(sstr,"wansm_%d",i+1);
			ramPptpCfgParam.ipMask[i]= atoi(websGetVar(wp, sstr, ""));
		}

		valstr=(websGetVar(wp,"username",""));
		memset((char*)ramPptpCfgParam.username,0,sizeof(ramPptpCfgParam.username));
		memcpy((char*)ramPptpCfgParam.username,valstr,strlen(valstr));
		valstr=(websGetVar(wp,"password",""));
		memset((char*)&ramPptpCfgParam.password,0,sizeof(ramPptpCfgParam.password));
		memcpy((char*)&ramPptpCfgParam.password,valstr,strlen(valstr));
		ramPptpCfgParam.mtu=atoi(websGetVar(wp,"wanmtu",""));
		ramPptpCfgParam.silentTimeout=atoi(websGetVar(wp,"silentTimeout",""));
		ramPptpCfgParam.autoReconnect=atoi(websGetVar(wp,"autoReconnect",""));
		ramPptpCfgParam.demand=atoi(websGetVar(wp,"dialOnDemand",""));
		if(ramPptpCfgParam.mtu>1452) ramPptpCfgParam.mtu=1452;
		if(wizard==1)
		{
			wizardSaveToFlash(IFCFGPARAM_CONNTYPE_PPTP);
			Sys_Restart(wp);	
		}
		else
		{			
			memcpy(&pRomeCfgParam->pptpCfgParam,&ramPptpCfgParam,sizeof(ramPptpCfgParam));			
			if(pRomeCfgParam->ifCfgParam[0].connType!=IFCFGPARAM_CONNTYPE_PPTP) configChanged|=CONFIG_CHANGE_CONN_TYPE;
			pRomeCfgParam->ifCfgParam[0].connType=IFCFGPARAM_CONNTYPE_PPTP;			
			cfgmgr_write(CFGMGR_TABID_PPTP,(void*)&ramPptpCfgParam,sizeof(ramPptpCfgParam));
			cfgmgr_write(CFGMGR_TABID_IF,(void*)&pRomeCfgParam->ifCfgParam[0], sizeof(ifCfgParam_t)*MAX_IP_INTERFACE);
			cfgmgr_task();			
			websRedirect(wp,"wanpptp.asp");
		}	
	}
}

int asp_pptpWizard(int ejid, webs_t wp, int argc, char_t **argv)
{
	int i;
	for(i=0;i<4;i++)		
	{
		char temp[16]={0};
		sprintf(temp,"ip%d",i+1);
		if (strcmp(argv[0],temp)==0) websWrite(wp,"%u",pRomeCfgParam->pptpCfgParam.ipAddr[i]);
		sprintf(temp,"sm%d",i+1);
		if (strcmp(argv[0],temp)==0) websWrite(wp,"%u",pRomeCfgParam->pptpCfgParam.ipMask[i]);
		sprintf(temp,"sv%d",i+1);
		if (strcmp(argv[0],temp)==0) websWrite(wp,"%u",pRomeCfgParam->pptpCfgParam.svAddr[i]);		
	}
	if(strcmp(argv[0],"username")==0) websWrite(wp,"%s",pRomeCfgParam->pptpCfgParam.username);
	else if(strcmp(argv[0],"password")==0) websWrite(wp,"%s",pRomeCfgParam->pptpCfgParam.password);
	else if(strcmp(argv[0],"mtu")==0) websWrite(wp,"%d",pRomeCfgParam->pptpCfgParam.mtu);
	else if(strcmp(argv[0],"silentTimeout")==0) websWrite(wp,"%d",pRomeCfgParam->pptpCfgParam.silentTimeout);
	else if(strcmp(argv[0],"dialOnDemand")==0) websWrite(wp,"%s",pRomeCfgParam->pptpCfgParam.demand?"checked":"");
	else if(strcmp(argv[0],"autoReconnect")==0) websWrite(wp,"%s",pRomeCfgParam->pptpCfgParam.autoReconnect?"checked":"");
	else if(strcmp(argv[0],"autoRefresh")==0)
	{
		if((pRomeCfgParam->pptpCfgParam.dialState==PPTPCFGPARAM_DIALSTATE_DIALED_TRYING)||(pRomeCfgParam->pptpCfgParam.dialState==PPTPCFGPARAM_DIALSTATE_DISCONNECTING))
    	{
			websWrite(wp,"<META HTTP-EQUIV=Refresh CONTENT=\"2; URL=wanpptp.asp\">");
    	}
	}
	else if(strcmp(argv[0],"dialStatus")==0) 
	{
		char tempStr[32];
		getDialStateString(0+MAX_PPPOE_SESSION,tempStr);
		websWrite(wp,"%s",tempStr);
	}
	else if(strcmp(argv[0],"submit")==0) 
	{
		if(atoi(websGetVar(wp,"wizard",""))==1)
		{
    		websWrite(wp,"<input type=hidden name=\"wizard\" value=1>" \
    		"<input type=\"button\" value=\"< Back\" onclick=\"javascript:history.back();\">" \
	    	"&nbsp;&nbsp;&nbsp;&nbsp;" \
    		"<input type=\"submit\" value=\"Save and Restart\">" \
    		"&nbsp;&nbsp;&nbsp;&nbsp;" \
	    	"<input type=\"reset\" value=\"Reset\">" \
    		"&nbsp;&nbsp;&nbsp;&nbsp;" \
    		"<input type=\"button\" value=\"Cancel\" onclick=\"javascript:location.replace('status.asp');\">");
		}
		else
		{
			int dialOn=0,hangupOn=1;			
			if(pRomeCfgParam->pptpCfgParam.dialState==PPTPCFGPARAM_DIALSTATE_OFF) dialOn=1;
			//if(pRomeCfgParam->pptpCfgParam.dialState==PPTPCFGPARAM_DIALSTATE_DIALED_TRYING) hangupOn=0;
			if(pRomeCfgParam->pptpCfgParam.dialState==PPTPCFGPARAM_DIALSTATE_DISCONNECTING) hangupOn=0;

			if((pRomeCfgParam->ifCfgParam[0].connType!=IFCFGPARAM_CONNTYPE_PPTP)||(configChanged&CONFIG_CHANGE_CONN_TYPE))
			{
				dialOn=0; 
				hangupOn=0;
			}
			
			websWrite(wp,"<input type=submit value=Dial name=submit2 %s>&nbsp;" \
			"<input type=submit value=\"Hang Up\" name=submit2 %s>&nbsp;&nbsp;&nbsp;&nbsp;" \
			"<input type=submit value=\"Save\" name=submit2>&nbsp;" \
			"<input type=submit value=\"Refresh\" name=submit2>&nbsp;" \
			"<input type=reset value=\"Reset\">",(!dialOn)?"disabled":"",(!hangupOn)?"disabled":"");
		}
	}
	
	return 0;
}
#endif

#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
/************************************************************
 *	
 *	L2TP Client Setup Function (Wizard)
 *	
 ************************************************************/
void  asp_setL2tpWizard(webs_t wp, char_t *path, char_t *query)
{
	int 	i;
	int    val=0;
	int	wizard=atoi(websGetVar(wp,"wizard",""));
	char *valstr;
	char	sstr[8]={0};
	
	if(strcmp(websGetVar(wp,"submit2",""),"Refresh")==0)
	{
		websRedirect(wp,"wanl2tp.asp");
		return;
	}
	else if(strcmp(websGetVar(wp,"submit2",""),"Dial")==0)
	{
		l2tp_start(0);		
		sleep(1);
		websRedirect(wp,"wanl2tp.asp");
		return;
	}
	else if(strcmp(websGetVar(wp,"submit2",""),"Hang Up")==0)
	{

		send_signal("/var/run/pppd0.pid",1);
		sleep(1);	
#ifdef __uClinux__
#else		
//		send_signal("/var/run/l2tpd.pid",9);
#endif

		websRedirect(wp,"wanl2tp.asp");
		return;
	}	
	else
	{

		memcpy(&ramL2tpCfgParam,&pRomeCfgParam->l2tpCfgParam,sizeof(l2tpCfgParam_t));
		for (i=0;i<4;i++)
		{
			sprintf(sstr,"wanip_%d",i+1);
			ramL2tpCfgParam.ipAddr[i]= atoi(websGetVar(wp, sstr, ""));
	
			sprintf(sstr,"wansv_%d",i+1);
			ramL2tpCfgParam.svAddr[i]= atoi(websGetVar(wp, sstr, ""));
		
			sprintf(sstr,"wansm_%d",i+1);
			ramL2tpCfgParam.ipMask[i]= atoi(websGetVar(wp, sstr, ""));
		}

		valstr=(websGetVar(wp,"username",""));
		memset((char*)ramL2tpCfgParam.username,0,sizeof(ramL2tpCfgParam.username));
		memcpy((char*)ramL2tpCfgParam.username,valstr,strlen(valstr));
		valstr=(websGetVar(wp,"password",""));
		memset((char*)&ramL2tpCfgParam.password,0,sizeof(ramL2tpCfgParam.password));
		memcpy((char*)&ramL2tpCfgParam.password,valstr,strlen(valstr));
		ramL2tpCfgParam.mtu=atoi(websGetVar(wp,"wanmtu",""));
		ramL2tpCfgParam.silentTimeout=atoi(websGetVar(wp,"silentTimeout",""));
		ramL2tpCfgParam.autoReconnect=atoi(websGetVar(wp,"autoReconnect",""));
		ramL2tpCfgParam.demand=atoi(websGetVar(wp,"dialOnDemand",""));	
		if(ramL2tpCfgParam.mtu>1452) ramL2tpCfgParam.mtu=1452;
		if(wizard==1)
		{
			wizardSaveToFlash(IFCFGPARAM_CONNTYPE_L2TP);
			Sys_Restart(wp);	
		}
		else
		{

			memcpy(&pRomeCfgParam->l2tpCfgParam,&ramL2tpCfgParam,sizeof(l2tpCfgParam_t));
			if(pRomeCfgParam->ifCfgParam[0].connType!=IFCFGPARAM_CONNTYPE_L2TP) configChanged|=CONFIG_CHANGE_CONN_TYPE;
			pRomeCfgParam->ifCfgParam[0].connType=IFCFGPARAM_CONNTYPE_L2TP;			
			cfgmgr_write(CFGMGR_TABID_L2TP,(void*)&ramL2tpCfgParam,sizeof(l2tpCfgParam_t));
			cfgmgr_write(CFGMGR_TABID_IF,(void*)&pRomeCfgParam->ifCfgParam[0], sizeof(ifCfgParam_t)*MAX_IP_INTERFACE);
			cfgmgr_task();
			websRedirect(wp,"wanl2tp.asp");
		}		
	}
}


int asp_l2tpWizard(int ejid, webs_t wp, int argc, char_t **argv)
{
	int i;
	for(i=0;i<4;i++)		
	{
		char temp[16]={0};
		sprintf(temp,"ip%d",i+1);
		if (strcmp(argv[0],temp)==0) websWrite(wp,"%u",pRomeCfgParam->l2tpCfgParam.ipAddr[i]);
		sprintf(temp,"sm%d",i+1);
		if (strcmp(argv[0],temp)==0) websWrite(wp,"%u",pRomeCfgParam->l2tpCfgParam.ipMask[i]);
		sprintf(temp,"sv%d",i+1);
		if (strcmp(argv[0],temp)==0) websWrite(wp,"%u",pRomeCfgParam->l2tpCfgParam.svAddr[i]);		
	}
	if(strcmp(argv[0],"username")==0) websWrite(wp,"%s",pRomeCfgParam->l2tpCfgParam.username);
	else if(strcmp(argv[0],"password")==0) websWrite(wp,"%s",pRomeCfgParam->l2tpCfgParam.password);
	else if(strcmp(argv[0],"mtu")==0) websWrite(wp,"%d",pRomeCfgParam->l2tpCfgParam.mtu);	
	else if(strcmp(argv[0],"silentTimeout")==0) websWrite(wp,"%d",pRomeCfgParam->l2tpCfgParam.silentTimeout);
	else if(strcmp(argv[0],"dialOnDemand")==0) websWrite(wp,"%s",pRomeCfgParam->l2tpCfgParam.demand?"checked":"");	
	else if(strcmp(argv[0],"autoReconnect")==0) websWrite(wp,"%s",pRomeCfgParam->l2tpCfgParam.autoReconnect?"checked":"");
	else if(strcmp(argv[0],"autoRefresh")==0)
	{
		if((pRomeCfgParam->l2tpCfgParam.dialState==L2TPCFGPARAM_DIALSTATE_DIALED_TRYING)||(pRomeCfgParam->l2tpCfgParam.dialState==L2TPCFGPARAM_DIALSTATE_DISCONNECTING))
    	{
			websWrite(wp,"<META HTTP-EQUIV=Refresh CONTENT=\"2; URL=wanl2tp.asp\">");
    	}
	}
	else if(strcmp(argv[0],"dialStatus")==0) 
	{
		char tempStr[32];
		getDialStateString(1+MAX_PPPOE_SESSION,tempStr); //l2tp
		websWrite(wp,"%s",tempStr);
	}
	else if(strcmp(argv[0],"submit")==0) 
	{
		if(atoi(websGetVar(wp,"wizard",""))==1)
		{
    		websWrite(wp,"<input type=hidden name=\"wizard\" value=1>" \
    		"<input type=\"button\" value=\"< Back\" onclick=\"javascript:history.back();\">" \
	    	"&nbsp;&nbsp;&nbsp;&nbsp;" \
    		"<input type=\"submit\" value=\"Save and Restart\">" \
    		"&nbsp;&nbsp;&nbsp;&nbsp;" \
	    	"<input type=\"reset\" value=\"Reset\">" \
    		"&nbsp;&nbsp;&nbsp;&nbsp;" \
    		"<input type=\"button\" value=\"Cancel\" onclick=\"javascript:location.replace('status.asp');\">");
		}
		else
		{
			int dialOn=0,hangupOn=1;			
			if(pRomeCfgParam->l2tpCfgParam.dialState==L2TPCFGPARAM_DIALSTATE_OFF) dialOn=1;
			else if(pRomeCfgParam->l2tpCfgParam.dialState==L2TPCFGPARAM_DIALSTATE_DIALED_DHCP_DISCOVER) dialOn=1;
			else if(pRomeCfgParam->l2tpCfgParam.dialState==L2TPCFGPARAM_DIALSTATE_DIALED_TRYING) hangupOn=0;

			if((!((pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_L2TP)||
				  (pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_DHCPL2TP)))||
				  (configChanged&CONFIG_CHANGE_CONN_TYPE))
			{
				dialOn=0; 
				hangupOn=0;
			}
			
			websWrite(wp,"<input type=submit value=Dial name=submit2 %s>&nbsp;" \
			"<input type=submit value=\"Hang Up\" name=submit2 %s>&nbsp;&nbsp;&nbsp;&nbsp;" \
			"<input type=submit value=\"Save\" name=submit2>&nbsp;" \
			"<input type=submit value=\"Refresh\" name=submit2>&nbsp;" \
			"<input type=reset value=\"Reset\">",(!dialOn)?"disabled":"",(!hangupOn)?"disabled":"");
		}
	}
	
	return 0;
}




void  asp_setDhcpL2tpWizard(webs_t wp, char_t *path, char_t *query)
{
	int 	i;
	int    val=0;
	int	wizard=atoi(websGetVar(wp,"wizard",""));
	char *valstr;
	char	sstr[8]={0};
	
	if(strcmp(websGetVar(wp,"submit2",""),"Refresh")==0)
	{
		websRedirect(wp,"wandhcpl2tp.asp");
		return;
	}
	else if(strcmp(websGetVar(wp,"submit2",""),"Dial")==0)
	{
		l2tp_start(0);		
		sleep(1);
		websRedirect(wp,"wandhcpl2tp.asp");
		return;
	}
	else if(strcmp(websGetVar(wp,"submit2",""),"Hang Up")==0)
	{
		send_signal("/var/run/pppd0.pid",1);		
		sleep(1);	
#ifdef __uClinux__
#else		
//		send_signal("/var/run/l2tpd.pid",9);
#endif		
		websRedirect(wp,"wandhcpl2tp.asp");
		return;
	}	
	else
	{

		memcpy(&ramL2tpCfgParam,&pRomeCfgParam->l2tpCfgParam,sizeof(l2tpCfgParam_t));
		valstr=(websGetVar(wp,"username",""));
		memset((char*)ramL2tpCfgParam.username,0,sizeof(ramL2tpCfgParam.username));
		memcpy((char*)ramL2tpCfgParam.username,valstr,strlen(valstr));
		valstr=(websGetVar(wp,"password",""));
		memset((char*)&ramL2tpCfgParam.password,0,sizeof(ramL2tpCfgParam.password));
		memcpy((char*)&ramL2tpCfgParam.password,valstr,strlen(valstr));
		ramL2tpCfgParam.mtu=atoi(websGetVar(wp,"wanmtu",""));
		ramL2tpCfgParam.silentTimeout=atoi(websGetVar(wp,"silentTimeout",""));
		ramL2tpCfgParam.autoReconnect=atoi(websGetVar(wp,"autoReconnect",""));
		ramL2tpCfgParam.demand=atoi(websGetVar(wp,"dialOnDemand",""));	
		if(ramL2tpCfgParam.mtu>1452) ramL2tpCfgParam.mtu=1452;
		if(wizard==1)
		{
			wizardSaveToFlash(IFCFGPARAM_CONNTYPE_DHCPL2TP);
			Sys_Restart(wp);	
		}
		else
		{

			memcpy(&pRomeCfgParam->l2tpCfgParam,&ramL2tpCfgParam,sizeof(l2tpCfgParam_t));
			if(pRomeCfgParam->ifCfgParam[0].connType!=IFCFGPARAM_CONNTYPE_DHCPL2TP) configChanged|=CONFIG_CHANGE_CONN_TYPE;
			pRomeCfgParam->ifCfgParam[0].connType=IFCFGPARAM_CONNTYPE_DHCPL2TP;			
			cfgmgr_write(CFGMGR_TABID_L2TP,(void*)&ramL2tpCfgParam,sizeof(l2tpCfgParam_t));
			cfgmgr_write(CFGMGR_TABID_IF,(void*)&pRomeCfgParam->ifCfgParam[0], sizeof(ifCfgParam_t)*MAX_IP_INTERFACE);
			cfgmgr_task();
			websRedirect(wp,"wandhcpl2tp.asp");
		}		
	}
}


int asp_dhcpL2tpWizard(int ejid, webs_t wp, int argc, char_t **argv)
{
	int i;
	
	if(strcmp(argv[0],"username")==0) websWrite(wp,"%s",pRomeCfgParam->l2tpCfgParam.username);
	else if(strcmp(argv[0],"password")==0) websWrite(wp,"%s",pRomeCfgParam->l2tpCfgParam.password);
	else if(strcmp(argv[0],"mtu")==0) websWrite(wp,"%d",pRomeCfgParam->l2tpCfgParam.mtu);	
	else if(strcmp(argv[0],"silentTimeout")==0) websWrite(wp,"%d",pRomeCfgParam->l2tpCfgParam.silentTimeout);
	else if(strcmp(argv[0],"dialOnDemand")==0) websWrite(wp,"%s",pRomeCfgParam->l2tpCfgParam.demand?"checked":"");	
	else if(strcmp(argv[0],"autoReconnect")==0) websWrite(wp,"%s",pRomeCfgParam->l2tpCfgParam.autoReconnect?"checked":"");

	else if(strcmp(argv[0],"dialStatus")==0) 
	{
		char tempStr[32];
		getDialStateString(1+MAX_PPPOE_SESSION,tempStr); //l2tp
		websWrite(wp,"%s",tempStr);
	}
	else if(strcmp(argv[0],"submit")==0) 
	{
		if(atoi(websGetVar(wp,"wizard",""))==1)
		{
    		websWrite(wp,"<input type=hidden name=\"wizard\" value=1>" \
    		"<input type=\"button\" value=\"< Back\" onclick=\"javascript:history.back();\">" \
	    	"&nbsp;&nbsp;&nbsp;&nbsp;" \
    		"<input type=\"submit\" value=\"Save and Restart\">" \
    		"&nbsp;&nbsp;&nbsp;&nbsp;" \
	    	"<input type=\"reset\" value=\"Reset\">" \
    		"&nbsp;&nbsp;&nbsp;&nbsp;" \
    		"<input type=\"button\" value=\"Cancel\" onclick=\"javascript:location.replace('status.asp');\">");
		}
		else
		{
			int dialOn=0,hangupOn=1;			
			if(pRomeCfgParam->l2tpCfgParam.dialState==L2TPCFGPARAM_DIALSTATE_OFF) dialOn=1;
			if(pRomeCfgParam->l2tpCfgParam.dialState==L2TPCFGPARAM_DIALSTATE_DIALED_TRYING) hangupOn=0;

			if((pRomeCfgParam->ifCfgParam[0].connType!=IFCFGPARAM_CONNTYPE_DHCPL2TP)||(configChanged&CONFIG_CHANGE_CONN_TYPE))
			{
				dialOn=0; 
				hangupOn=0;
			}
			
			websWrite(wp,"<input type=submit value=Dial name=submit2 %s>&nbsp;" \
			"<input type=submit value=\"Hang Up\" name=submit2 %s>&nbsp;&nbsp;&nbsp;&nbsp;" \
			"<input type=submit value=\"Save\" name=submit2>&nbsp;" \
			"<input type=submit value=\"Refresh\" name=submit2>&nbsp;" \
			"<input type=reset value=\"Reset\">",(!dialOn)?"disabled":"",(!hangupOn)?"disabled":"");
		}
	}
	
	return 0;
}

#endif

/************************************************************
 *	
 *	DHCP Client Setup Function (Wizard)
 *	
 ************************************************************/
void asp_setDhcpClient(webs_t wp, char_t *path, char_t *query)
{	
	int i=0;
	char str[8];
	
	if (!strcmp(websGetVar(wp,T("cmacToggle"),T("")),"on"))
	{
		ramDhcpcCfgParam.cmacValid=1;
		for (i=0;i<6;i++)
		{
			sprintf(str,"cmac%d",i+1);
			ramDhcpcCfgParam.cloneMac[i]=strtol(websGetVar(wp, T(str), T("")),NULL,16 ); 
		}
	}
	else
		ramDhcpcCfgParam.cmacValid=0;
	
	wizardSaveToFlash(IFCFGPARAM_CONNTYPE_DHCPC);
	Sys_Restart(wp);	
}

/************************************************************
 *	
 *	PPPoE (Wizard)
 *	
 ************************************************************/
void asp_setPppoeWizard(webs_t wp, char_t *path, char_t *query)
{
	char *valstr;
	valstr=(websGetVar(wp,"username0",""));
	DEBUG_P(stderr,"user %s\n",valstr);
	memset((char*)&ramPppoeCfgParam[0].username,0,sizeof(ramPppoeCfgParam[0].username));
	memcpy((char*)&ramPppoeCfgParam[0].username,valstr,strlen(valstr));
	valstr=(websGetVar(wp,"password0",""));		
	DEBUG_P(stderr,"passwd %s\n",valstr);
	memset((char*)&ramPppoeCfgParam[0].password,0,sizeof(ramPppoeCfgParam[0].password));
	memcpy((char*)&ramPppoeCfgParam[0].password,valstr,strlen(valstr));
	valstr=(websGetVar(wp,"serviceName0",""));
	memset((char*)&ramPppoeCfgParam[0].serviceName,0,sizeof(ramPppoeCfgParam[0].serviceName));
	memcpy((char*)&ramPppoeCfgParam[0].serviceName,valstr,strlen(valstr));
	valstr=(websGetVar(wp,"acName0",""));
	memset((char*)&ramPppoeCfgParam[0].acName,0,sizeof(ramPppoeCfgParam[0].acName));
	memcpy((char*)&ramPppoeCfgParam[0].acName,valstr,strlen(valstr));

	ramPppoeCfgParam[0].mtu=atoi(websGetVar(wp,"mtu0",""));	
	if(ramPppoeCfgParam[0].mtu>1492) ramPppoeCfgParam[0].mtu=1492;
	if(ramPppoeCfgParam[0].mtu<576) ramPppoeCfgParam[0].mtu=576;
	ramPppoeCfgParam[0].demand=atoi(websGetVar(wp,"demand0",""));
	ramPppoeCfgParam[0].silentTimeout=atoi(websGetVar(wp,"silentTimeout0",""));
	ramPppoeCfgParam[0].autoReconnect=atoi(websGetVar(wp,"autoReconnect0",""));
	
	wizardSaveToFlash(IFCFGPARAM_CONNTYPE_PPPOE);
	Sys_Restart(wp);	
}

/************************************************************
 *	
 *	Unnumbered PPPoE (Wizard)
 *	
 ************************************************************/
void asp_setUnnumberedWizard(webs_t wp, char_t *path, char_t *query)
{
	char *valstr;
	int i;
	char tempStr[20]={0};
	uint32 netmask=0xffffffff;
	uint8 *netmask_p=(uint8 *)&netmask;
	
	valstr=(websGetVar(wp,"username0",""));
	DEBUG_P(stderr,"user %s\n",valstr);
	memset((char*)&ramPppoeCfgParam[0].username,0,sizeof(ramPppoeCfgParam[0].username));
	memcpy((char*)&ramPppoeCfgParam[0].username,valstr,strlen(valstr));
	valstr=(websGetVar(wp,"password0",""));		
	DEBUG_P(stderr,"passwd %s\n",valstr);
	memset((char*)&ramPppoeCfgParam[0].password,0,sizeof(ramPppoeCfgParam[0].password));
	memcpy((char*)&ramPppoeCfgParam[0].password,valstr,strlen(valstr));
	valstr=(websGetVar(wp,"serviceName0",""));
	memset((char*)&ramPppoeCfgParam[0].serviceName,0,sizeof(ramPppoeCfgParam[0].serviceName));
	memcpy((char*)&ramPppoeCfgParam[0].serviceName,valstr,strlen(valstr));
	valstr=(websGetVar(wp,"acName0",""));
	memset((char*)&ramPppoeCfgParam[0].acName,0,sizeof(ramPppoeCfgParam[0].acName));
	memcpy((char*)&ramPppoeCfgParam[0].acName,valstr,strlen(valstr));

	ramPppoeCfgParam[0].mtu=atoi(websGetVar(wp,"mtu0",""));	
	if(ramPppoeCfgParam[0].mtu>1492) ramPppoeCfgParam[0].mtu=1492;
	if(ramPppoeCfgParam[0].mtu<576) ramPppoeCfgParam[0].mtu=576;
		
	ramPppoeCfgParam[0].demand=atoi(websGetVar(wp,"demand0",""));
	ramPppoeCfgParam[0].silentTimeout=atoi(websGetVar(wp,"silentTimeout0",""));
	ramPppoeCfgParam[0].autoReconnect=atoi(websGetVar(wp,"autoReconnect0",""));
	ramPppoeCfgParam[0].unnumberedNapt=atoi(websGetVar(wp,"unnumberedNapt",""));

	for(i=0;i<4;i++)
	{
		sprintf(tempStr,"unnumberedIpAddr%d",i+1);
		ramPppoeCfgParam[0].unnumberedIpAddr[i]=atoi(websGetVar(wp,tempStr,""));
	}

	netmask=(netmask>>(32-atoi(websGetVar(wp,"unnumberedIpMask",""))))<<(32-atoi(websGetVar(wp,"unnumberedIpMask","")));
	ramPppoeCfgParam[0].unnumberedIpMask[0]=netmask_p[0];
	ramPppoeCfgParam[0].unnumberedIpMask[1]=netmask_p[1];
	ramPppoeCfgParam[0].unnumberedIpMask[2]=netmask_p[2];
	ramPppoeCfgParam[0].unnumberedIpMask[3]=netmask_p[3];
		
	wizardSaveToFlash(IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED);
	Sys_Restart(wp);	
}

/************************************************************
 *	
 *	ACL
 *	
 ************************************************************/
void asp_acl(int ejid, webs_t wp, int argc, char_t **argv)
{
	int i,j;
	int ms=0;

#if 0		
	websWrite(wp,		
	"function checkall()\n" \
	"{\n" \
	"	for(i=1;i<=%d;i++)	\n" \
	"		fm.elements[i*8].checked=true;\n" \
	"}\n" \
	"function uncheckall()\n" \
	"{\n" \
	"	for(i=1;i<=%d;i++)\n" \
	"		fm.elements[i*8].checked=false;\n" \
	"}\n" \
	"</script>\n" ,MAX_ACL,MAX_ACL);
#else
	websWrite(wp,"</script>");
#endif
	
	websWrite(wp,
	"<body bgcolor=\"#ffffff\" text=\"#000000\">\n" \
	"<b>Hardware Access Control List</b><BR>");

	ms=asp_multiSession(wp,argc,argv);
	websWrite(wp,		
	"<form action=\"acl.asp\" name=\"fn\"><input type=hidden name=ms value=0></form>\n" \
	"<form method=get action=\"/goform/asp_setAcl\" name=fm>\n" \
	"<B>Policy:</B><select name=\"policy\">" \
	"<option value=0 %s>Allow all except ACL (without log)</option>" \
/*	"<option value=1 %s>Deny all except ACL.</option>" \ */	
	"<option value=2 %s>Allow all except ACL (with log)</select><BR><BR>",(pRomeCfgParam->aclGlobalCfgParam.policy==0)?"selected":"",(pRomeCfgParam->aclGlobalCfgParam.policy==2)?"selected":"");

	websWrite(wp,	
	"<table cellSpacing=1 cellPadding=2 border=0>\n" \
	"<tr><td bgColor=#aaddff>Priority</td><td bgColor=#aaddff>Direction</td><td bgColor=#aaddff>IP</td><td bgColor=#aaddff>Type</td><td bgColor=#aaddff>Port</td><td bgColor=#aaddff>Enable</td></tr>\n");

	websWrite(wp,"<tr><td bgColor=#ddeeff align=center>0</td><td bgColor=#ddeeff align=center>Ingress, Desination IP/Port</td><td bgColor=#ddeeff align=center>%u.%u.%u.%u/%u.%u.%u.%u</td><td bgColor=#ddeeff align=center>IP</td><td bgColor=#ddeeff>&nbsp;</td><td bgColor=#ddeeff align=center><input type=checkbox name=lanPermit value=1 %s>" \
				,pRomeCfgParam->ifCfgParam[1].ipAddr[0]&pRomeCfgParam->ifCfgParam[1].ipMask[0]
				,pRomeCfgParam->ifCfgParam[1].ipAddr[1]&pRomeCfgParam->ifCfgParam[1].ipMask[1]
				,pRomeCfgParam->ifCfgParam[1].ipAddr[2]&pRomeCfgParam->ifCfgParam[1].ipMask[2]
				,pRomeCfgParam->ifCfgParam[1].ipAddr[3]&pRomeCfgParam->ifCfgParam[1].ipMask[3]
				,pRomeCfgParam->ifCfgParam[1].ipMask[0]
				,pRomeCfgParam->ifCfgParam[1].ipMask[1]
				,pRomeCfgParam->ifCfgParam[1].ipMask[2]
				,pRomeCfgParam->ifCfgParam[1].ipMask[3]
				,(pRomeCfgParam->aclGlobalCfgParam.lanPermit==1)?"checked":"");

	for(i=1;i<=MAX_ACL;i++)
	{
		websWrite(wp,
		"<tr><td bgColor=#ddeeff align=center>%d</td><td bgColor=#ddeeff>\n" \
		"<select name=\"d%02d\">\n" \
		"<option value=\"0\" %s>Egress, Destination IP/Port</option>\n" \
		"<option value=\"1\" %s>Ingress, Destination IP/Port</option>\n" \
		"<option value=\"2\" %s>Egress, Source IP/Port</option>\n" \
		"<option value=\"3\" %s>Ingress, Source IP/Port</option>\n" \
		"</td>\n",i,i,(pRomeCfgParam->aclCfgParam[ms][i-1].direction==0)?"selected":""
		             ,(pRomeCfgParam->aclCfgParam[ms][i-1].direction==1)?"selected":""
		             ,(pRomeCfgParam->aclCfgParam[ms][i-1].direction==2)?"selected":""
		             ,(pRomeCfgParam->aclCfgParam[ms][i-1].direction==3)?"selected":"");

		// ip/network
		websWrite(wp,"<td bgColor=#ddeeff>\n");
		for(j=1;j<=4;j++)
		{
			websWrite(wp,"<input type=text name=\"ip%02d%d\" size=3 maxlength=3 value=%u>\n",i,j,pRomeCfgParam->aclCfgParam[ms][i-1].ip[j-1]);
			if(j<4) websWrite(wp,".");
		}
		websWrite(wp,"</td>\n");

		//type
		websWrite(wp,
		"<td bgColor=#ddeeff>" \
			"<select name=\"t%02d\">" \
				"<option value=\"0\" %s>TCP</option>" \
				"<option value=\"1\" %s>UDP</option>" \
				"<option value=\"2\" %s>IP</option>" \
			"</select>" \
		"</td>\n",i,(pRomeCfgParam->aclCfgParam[ms][i-1].type==ACLCFGPARAM_PROTOCOL_TCP)?"selected":""
				   ,(pRomeCfgParam->aclCfgParam[ms][i-1].type==ACLCFGPARAM_PROTOCOL_UDP)?"selected":""
                               ,(pRomeCfgParam->aclCfgParam[ms][i-1].type==ACLCFGPARAM_PROTOCOL_IP)?"selected":"");

		// port
		websWrite(wp,
		"<td bgColor=#ddeeff>" \
			"<input type=text size=5 maxlength=5 name=\"p%02d\" value=%u>" \
		"</td>\n",i,pRomeCfgParam->aclCfgParam[ms][i-1].port);
		
		// enable
		websWrite(wp,
		"<td bgColor=#ddeeff align=center>" \
			"<input type=checkbox name=\"e%02d\" %s value=1>" \
		"</td>\n",i,(pRomeCfgParam->aclCfgParam[ms][i-1].enable==0)?"":"checked");

		websWrite(wp,"<input type=hidden name=ms value=%d>",ms);

	} //end for
	return;
}

void asp_setAcl(webs_t wp, char_t *path, char_t *query)
{
	char tmpstr[16]={0};
	int	i,j;
	int	ms=atoi(websGetVar(wp,"ms",""));
				
	pRomeCfgParam->aclGlobalCfgParam.policy=atoi(websGetVar(wp,"policy",""));
	pRomeCfgParam->aclGlobalCfgParam.lanPermit=atoi(websGetVar(wp,"lanPermit",""));
	
	for(i=0;i<MAX_ACL;i++)
	{
		sprintf(tmpstr,"d%02d",i+1);
		pRomeCfgParam->aclCfgParam[ms][i].direction=atoi(websGetVar(wp,tmpstr,""));
		
		sprintf(tmpstr,"t%02d",i+1);
		pRomeCfgParam->aclCfgParam[ms][i].type=atoi(websGetVar(wp,tmpstr,""));
		
		sprintf(tmpstr,"p%02d",i+1);
		pRomeCfgParam->aclCfgParam[ms][i].port=atoi(websGetVar(wp,tmpstr,""));
		
		for(j=0;j<4;j++)
		{
			sprintf(tmpstr,"ip%02d%d",i+1,j+1);
			pRomeCfgParam->aclCfgParam[ms][i].ip[j]=atoi(websGetVar(wp,tmpstr,""));
		}
			
		sprintf(tmpstr,"e%02d",i+1);		
		if(atoi(websGetVar(wp,tmpstr,""))==1)
			pRomeCfgParam->aclCfgParam[ms][i].enable=1; 
		else
			pRomeCfgParam->aclCfgParam[ms][i].enable=0;			 

		if((pRomeCfgParam->aclCfgParam[ms][i].ip[0]==0)&&(pRomeCfgParam->aclCfgParam[ms][i].ip[1]==0)&&
                  (pRomeCfgParam->aclCfgParam[ms][i].ip[2]==0)&&(pRomeCfgParam->aclCfgParam[ms][i].ip[3]==0)&&
                  (pRomeCfgParam->aclCfgParam[ms][i].port==0)) pRomeCfgParam->aclCfgParam[ms][i].enable=0;		
	}
	
	/* access table driver, return value not checked ? */
	acl_tableDriverAccess();
	
	/* save to cfgmgr */	
	cfgmgr_write(CFGMGR_TABID_ACL,(void*)pRomeCfgParam->aclCfgParam,sizeof(struct aclCfgParam_s)*MAX_ACL*MAX_PPPOE_SESSION);
	cfgmgr_write(CFGMGR_TABID_ACL_GLOBAL,(void*)&(pRomeCfgParam->aclGlobalCfgParam),sizeof(aclGlobalCfgParam_t));	
	cfgmgr_task();

	sprintf(tmpstr,	"acl.asp?ms=%d",ms);
	websRedirect(wp, tmpstr);
}

/************************************************************
 *	
 *	Multiple PPPoE and Unnumbered PPPoE
 *	
 ************************************************************/
void asp_pppoe(int ejid, webs_t wp, int argc, char_t **argv)
{
	int i,j;
	int pppoeType=0;
	int wizard=0;
	char str[32];
	
	pppoeType=atoi(websGetVar(wp,"pppoeType",""));
	wizard=atoi(websGetVar(wp,"wizard",""));	
	if(pppoeType==0)
	{
		pppoeType=pRomeCfgParam->ifCfgParam[0].connType;
	}
	
	websWrite(wp,"<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=big5\">\n" \
	"<title>PPPoE Configuration</title>\n");

	for(i=0;i<MAX_PPPOE_SESSION;i++)
    {
    	if(pRomeCfgParam->pppoeCfgParam[i].dialState==PPPOECFGPARAM_DIALSTATE_DIALED_TRYING)
    	{
			websWrite(wp,"<META HTTP-EQUIV=Refresh CONTENT=\"2; URL=pppoe.asp\">");
			break;
    	}
	}

	websWrite(wp,"<script language=\"javascript\">\n" \
    "function disconnect_alert() { \n" \
    "	msg = \"If configuration have been changed, the PPPoE seesion will disconnect automatic.\\n\\nClick OK to continue or Cancel to abort\"; \n" \
    "	if (confirm(msg)) \n" \
    "	{ formname.submit(); } \n" \
    "}\n" \
    "function changePppoeType(a) { \n" \
    "	formpppoe.pppoeType.value=a; \n" \
    "	formpppoe.submit(); \n" \
    "}\n" \
    "function changeSessionID(b) { \n" \
    "	formname.sessionID.value=b; \n" \
    "}\n");
	
    for(i=0;i<MAX_PPPOE_SESSION;i++)
    {
    	if(pppoeType==IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)
    		sprintf(str,"Multiple PPPoE %d: ",i);
    	else 
    		sprintf(str,"");
    	websWrite(wp,
		"function clickAutoReconnect%d()\n" \
		"{ 	if(formname.autoReconnect%d.checked==1) {\n" \
		"		if((formname.silentTimeout%d.value!=0)&&(formname.demand%d.checked==0))\n" \
		"			{ alert('%sIf you want to turn auto reconnect on, you have to set silent timeout to zero second.');\n" \
		"  			  formname.autoReconnect%d.checked=0; }\n" \
		" } else { formname.demand%d.checked=0; } }\n" \
		"function clickDialOnDemand%d() { if(formname.demand%d.checked==1) formname.autoReconnect%d.checked=1; else clickAutoReconnect%d(); } \n" \
		"function changeSilentTimeout%d()\n" \
		"{ if((formname.silentTimeout%d.value!=0)&&(formname.autoReconnect%d.checked==1)&&(formname.demand%d.checked==0))\n" \
		"	{ alert('%sSilent timeout can\\\'t run with auto reconnect on.');" \
		"		formname.autoReconnect%d.checked=0;	} } \n" \
		,i,i,i,i,str,i,i,i,i,i,i,i,i,i,i,str,i);

	if(pppoeType!=IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE) break;
	
    	websWrite(wp,
	"function lanTypeChange%d()\n" \
	"{ \n" \
	"var tf=false; if(formname.lanType%d.value==0) tf=true; \n" \
	"formname.unnumberedIpAddr%d1.disabled=tf; \n" \
	"formname.unnumberedIpAddr%d2.disabled=tf; \n" \
	"formname.unnumberedIpAddr%d3.disabled=tf; \n" \
	"formname.unnumberedIpAddr%d4.disabled=tf; \n" \
	"for(var k=0;k<formname.unnumberedIpAddr%d1.length;k++)\n" \
	"  { formname.unnumberedIpAddr%d1[k].disabled=tf; } \n" \
	"for(k=0;k<formname.unnumberedIpAddr%d2.length;k++)\n" \
	"  { formname.unnumberedIpAddr%d2[k].disabled=tf; } \n" \
	"for(k=0;k<formname.unnumberedIpAddr%d3.length;k++)\n" \
	"  { formname.unnumberedIpAddr%d3[k].disabled=tf; } \n" \
	"for(k=0;k<formname.unnumberedIpAddr%d4.length;k++)\n" \
	"  { formname.unnumberedIpAddr%d4[k].disabled=tf; } \n" \
	"formname.unnumberedIpMask%d.disabled=tf; \n",i,i,i,i,i,i,i,i,i,i,i,i,i,i,i);

	if(pRomeCfgParam->pppoeCfgParam[i].defaultSession==0)
	{		
    		websWrite(wp,
		"tf=!tf; \n" \
		"formname.destnettype%d1.disabled=tf; \n" \
		"formname.destnettype%d2.disabled=tf; \n" \
		"formname.destnettype%d3.disabled=tf; \n" \
		"formname.destnettype%d4.disabled=tf; \n" \
		"formname.destnet%d1.disabled=tf; \n" \
		"formname.destnet%d2.disabled=tf; \n" \
		"formname.destnet%d3.disabled=tf; \n" \
		"formname.destnet%d4.disabled=tf; \n" \
		"for(var k=0;k<formname.destnet%d1.length;k++)\n" \
		"  { formname.destnet%d1[k].disabled=tf; } \n" \
		"for(k=0;k<formname.destnet%d2.length;k++)\n" \
		"  { formname.destnet%d2[k].disabled=tf; } \n" \
		"for(k=0;k<formname.destnet%d3.length;k++)\n" \
		"  { formname.destnet%d3[k].disabled=tf; } \n" \
		"for(k=0;k<formname.destnet%d4.length;k++)\n" \
		"  { formname.destnet%d4[k].disabled=tf; } \n" \
    		,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i);
	}	

	websWrite(wp,"} \n");

    }
    
    websWrite(wp,
	"</script>\n" \
    "</head>\n" \
    "<body bgcolor=ffffff text=\"#000000\">\n");

    if(wizard==0)
    	{
	    websWrite(wp,
	    "<form action=\"pppoe.asp\" name=formpppoe><input type=hidden value=1 name=pppoeType></form>\n" );
    	}

    switch(wizard)
    	{
    	case IFCFGPARAM_CONNTYPE_PPPOE:
    		websWrite(wp,"<form method=\"get\" action=\"/goform/asp_setPppoeWizard\" name=\"formname\">");
    		break;
    	case IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED:
    		websWrite(wp,"<form method=\"get\" action=\"/goform/asp_setUnnumberedWizard\" name=\"formname\">");
		break;
    	default:
		websWrite(wp,"<form method=\"get\" action=\"/goform/asp_setPppoe\" name=\"formname\">\n");
    		break;
    	}

    if(wizard==0)
    	{
   	 websWrite(wp,
	    "<input type=hidden value=\"0\" name=sessionID>");
    
	websWrite(wp,"<B>PPPoE Type:</b><select name=\"pppoeType\" onchange=\"changePppoeType(formname.pppoeType.value);\">");
	websWrite(wp,"<option value=%d %s>PPPoE</option>",IFCFGPARAM_CONNTYPE_PPPOE,(pppoeType==IFCFGPARAM_CONNTYPE_PPPOE)?"selected":"");
	if(MULTIPLE_PPPOE_SUPPORT!=0)
	{
		websWrite(wp,"<option value=%d %s>Unnumbered PPPoE</option><option value=%d %s>Multiple PPPoE</option>"
		,IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED,(pppoeType==IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED)?"selected":""
		,IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE,(pppoeType==IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)?"selected":"");
	}
	websWrite(wp,"</select>");
       
	   if(pppoeType==IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)
	   { 
   		websWrite(wp,"&nbsp;&nbsp;&nbsp;&nbsp;<b>Default:</b><select name=\"default_session\">");
   		for(i=0;i<MAX_PPPOE_SESSION;i++)
   		{
   			if(pRomeCfgParam->pppoeCfgParam[i].enable==1) websWrite(wp,"<option value=%d %s>PPPoE %d</option>",i,(pRomeCfgParam->pppoeCfgParam[i].defaultSession==1)?"selected":"",i);
   		}
   		websWrite(wp,"</select>");
	    }
   	   websWrite(wp,"<br><HR><BR>");
    	}
   	

	for(i=0;i<MAX_PPPOE_SESSION;i++)
   	{
   		int action;
   		   		

    	if(pppoeType==IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)
    	{
   			websWrite(wp,"<b>Multiple PPPoE %d Configuration</b>\n",i);
	   	}
	   	else if(pppoeType==IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED)
	   	{
	   		if(i>0) break;
	   		websWrite(wp,"<B>Unnumbered PPPoE Configuration</b>\n");
	   	}
	   	else
    	{
    		if(i>0) break;
    		websWrite(wp,"<b>PPPoE Configuration</b>\n");
    	}

    	websWrite(wp,"<table cellSpacing=1 cellPadding=2 border=0>\n" \
    	"<tr>\n" \
    	"<td bgColor=#aaddff colspan=1>Login ID</td>\n" \
    	"<td bgColor=#ddeeff colspan=3><input type=text name=username%d size=32 maxlength=32 value=\"%s\"></td>\n" \
    	"</tr>\n",i,pRomeCfgParam->pppoeCfgParam[i].username);

   		websWrite(wp,
	    "<tr>\n" \
    	"   <td bgColor=#aaddff colspan=1>Password</td>\n" \
	    "   <td bgColor=#ddeeff colspan=3><input type=password name=password%d size=32 maxlength=32 value=\"%s\"></td>\n" \
	    "</tr>\n"
	    ,i,pRomeCfgParam->pppoeCfgParam[i].password); 

   		websWrite(wp,
	    "<tr>\n" \
    	"   <td bgColor=#aaddff colspan=1>Service Name</td>\n" \
	    "   <td bgColor=#ddeeff colspan=3><input type=text name=serviceName%d size=32 maxlength=32 value=\"%s\"></td>\n" \
	    "</tr>\n"
	    ,i,pRomeCfgParam->pppoeCfgParam[i].serviceName); 

   		websWrite(wp,
	    "<tr>\n" \
    	"   <td bgColor=#aaddff colspan=1>AC Name</td>\n" \
	    "   <td bgColor=#ddeeff colspan=3><input type=text name=acName%d size=32 maxlength=32 value=\"%s\"></td>\n" \
	    "</tr>\n"
	    ,i,pRomeCfgParam->pppoeCfgParam[i].acName); 


	websWrite(wp,
		"<tr>\n" \
		"   <td bgColor=#aaddff colspan=1>MTU</td>\n" \
    		"   <td bgColor=#ddeeff colspan=3><input type=text size=4 maxlength=4 name=mtu%d  value=\"%d\"></td>\n" \
		"</tr>\n",i,pRomeCfgParam->pppoeCfgParam[i].mtu);
	    	    
	    if(pppoeType==IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)
	    {
		uint32 netmask=0;
		int k=0;
		uint8 *netmask_p=(uint8 *)&netmask;	

		websWrite(wp,
		"<tr>\n" \
		"   <td bgColor=#aaddff colspan=1>LAN Type</td>\n" \
    		"   <td bgColor=#ddeeff colspan=3><select name=lanType%d %s onclick=\"javascript:lanTypeChange%d();\"><option value=0 %s>NAPT</option><option value=1 %s>Unnumbered</option></td>\n" \
		"</tr>\n",i,
		pRomeCfgParam->pppoeCfgParam[i].defaultSession==1?"disabled":"",i,
		pRomeCfgParam->pppoeCfgParam[i].lanType==PPPOECFGPARAM_LANTYPE_NAPT?"selected":"",
		pRomeCfgParam->pppoeCfgParam[i].lanType==PPPOECFGPARAM_LANTYPE_UNNUMBERED?"selected":"");

		websWrite(wp,
		"<tr>\n" \
		"   <td bgColor=#aaddff colspan=1>Unnumbered IP Address</td>\n" \
    		"   <td bgColor=#ddeeff colspan=3><input type=text name=unnumberedIpAddr%d1 size=3 maxlength=3 value=%u>.\n" \
    		"   <input type=text name=unnumberedIpAddr%d2 size=3 maxlength=3 value=%u>.\n" \
    		"   <input type=text name=unnumberedIpAddr%d3 size=3 maxlength=3 value=%u>.\n" \
    		"   <input type=text name=unnumberedIpAddr%d4 size=3 maxlength=3 value=%u>\n" \
    		"</td>\n" \
		"</tr>\n",
		i,pRomeCfgParam->pppoeCfgParam[i].unnumberedIpAddr[0],
	    	i,pRomeCfgParam->pppoeCfgParam[i].unnumberedIpAddr[1],
	    	i,pRomeCfgParam->pppoeCfgParam[i].unnumberedIpAddr[2],
	    	i,pRomeCfgParam->pppoeCfgParam[i].unnumberedIpAddr[3]);	

		j=0;			
		netmask_p[0]=pRomeCfgParam->pppoeCfgParam[i].unnumberedIpMask[0];
		netmask_p[1]=pRomeCfgParam->pppoeCfgParam[i].unnumberedIpMask[1];
		netmask_p[2]=pRomeCfgParam->pppoeCfgParam[i].unnumberedIpMask[2];
		netmask_p[3]=pRomeCfgParam->pppoeCfgParam[i].unnumberedIpMask[3];
		if(netmask==0xffffffff) j=32;
		else while(netmask<<j) j++;
			
		websWrite(wp,
    		"<tr>\n" \
	    	"   <td bgColor=#aaddff colspan=1>Unnumbered Network Mask</td>\n" \
    		"   <td bgColor=#ddeeff colspan=3>" \
    		"		<select name=unnumberedIpMask%d>",i);

		for(k=31;k>=0;k--)
		{
			if(k!=0)
			{
				netmask=0xffffffff;
				netmask=(netmask>>(32-k))<<(32-k);
			}
			else
				netmask=0;
			websWrite(wp,
				"		<option value=\"%d\" %s>%u.%u.%u.%u</option>\n"
				,k,(k==j)?"selected":"",netmask_p[0],netmask_p[1],netmask_p[2],netmask_p[3]);
		}

			websWrite(wp,
    		"		</select>" \
	    	"</td></tr>\n");		

	
	    	for(j=0;j<4;j++)
	    	{
	    		websWrite(wp,
	    		"<tr>\n" \
		    	"   <td bgColor=#aaddff colspan=1>Destination Network %d</td>\n" \
    			"   <td bgColor=#ddeeff colspan=3><select name=destnettype%d%d %s>" \
    			"<option value=0 %s>NONE</option>" \
	    		"<option value=1 %s>IP RANGE</option>" \
    			"<option value=2 %s>DOMAIN</option>" \
	    		"<option value=3 %s>TCP PORT</option>" \
    			"<option value=4 %s>UDP PORT</option>" \
    			"<input type=text name=destnet%d%d size=32 maxlength=32 value=\"%s\" %s></td>\n" \
	    		"</tr>\n" ,j+1,i,j+1
	    		,(pRomeCfgParam->pppoeCfgParam[i].defaultSession==1)?"disabled":""
		    	,(pRomeCfgParam->pppoeCfgParam[i].destnetType[j]==0)?"selected":""
		    	,(pRomeCfgParam->pppoeCfgParam[i].destnetType[j]==1)?"selected":""
	    		,(pRomeCfgParam->pppoeCfgParam[i].destnetType[j]==2)?"selected":""
		    	,(pRomeCfgParam->pppoeCfgParam[i].destnetType[j]==3)?"selected":""
		    	,(pRomeCfgParam->pppoeCfgParam[i].destnetType[j]==4)?"selected":""
		    	,i,j+1,pRomeCfgParam->pppoeCfgParam[i].destnet[j]
		    	,(pRomeCfgParam->pppoeCfgParam[i].defaultSession==1)?"disabled":"");
	    	}
   		
		websWrite(wp,
    		"<tr>\n" \
	    	"   <td bgColor=#aaddff>Default</td>\n" \
	    	"   <td bgColor=#ddeeff><input type=checkbox %s disabled><script language=javascript>lanTypeChange%d();</script></td>\n" \
	       "   <td bgColor=#aaddff>Enable</td>\n" \
    		"   <td bgColor=#ddeeff><input type=checkbox name=enable%d value=1 %s></td>\n" \
	    	"</tr>\n",(pRomeCfgParam->pppoeCfgParam[i].defaultSession==1)?"checked":"",i,i,(pRomeCfgParam->pppoeCfgParam[i].enable==1)?"checked":"");
		}
		
		if(pppoeType==IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED)
		{
			uint32 netmask=0;
			int k=0;
			uint8 *netmask_p=(uint8 *)&netmask;			
			
		websWrite(wp,
    		"<tr>\n" \
	    	"   <td bgColor=#aaddff colspan=1>IP Address</td>\n" \
    		"   <td bgColor=#ddeeff colspan=3>" \
    		"		<input type=text name=unnumberedIpAddr1 size=3 maxlength=3 value=\"%d\">.\n" \
    		"		<input type=text name=unnumberedIpAddr2 size=3 maxlength=3 value=\"%d\">.\n" \
    		"		<input type=text name=unnumberedIpAddr3 size=3 maxlength=3 value=\"%d\">.\n" \
    		"		<input type=text name=unnumberedIpAddr4 size=3 maxlength=3 value=\"%d\">\n" \
	    	"</td></tr>\n"
	    	,pRomeCfgParam->pppoeCfgParam[0].unnumberedIpAddr[0]
	    	,pRomeCfgParam->pppoeCfgParam[0].unnumberedIpAddr[1]
	    	,pRomeCfgParam->pppoeCfgParam[0].unnumberedIpAddr[2]
	    	,pRomeCfgParam->pppoeCfgParam[0].unnumberedIpAddr[3]);	

		j=0;			
		netmask_p[0]=pRomeCfgParam->pppoeCfgParam[0].unnumberedIpMask[0];
		netmask_p[1]=pRomeCfgParam->pppoeCfgParam[0].unnumberedIpMask[1];
		netmask_p[2]=pRomeCfgParam->pppoeCfgParam[0].unnumberedIpMask[2];
		netmask_p[3]=pRomeCfgParam->pppoeCfgParam[0].unnumberedIpMask[3];
		if(netmask==0xffffffff) j=32;
		else while(netmask<<j) j++;
			
		websWrite(wp,
    		"<tr>\n" \
	    	"   <td bgColor=#aaddff colspan=1>Network Mask</td>\n" \
    		"   <td bgColor=#ddeeff colspan=3>" \
    		"		<select name=unnumberedIpMask>");

		for(k=31;k>=0;k--)
		{
			if(k!=0)
			{
				netmask=0xffffffff;
				netmask=(netmask>>(32-k))<<(32-k);
			}
			else
				netmask=0;
			websWrite(wp,
				"		<option value=\"%d\" %s>%u.%u.%u.%u</option>\n"
				,k,(k==j)?"selected":"",netmask_p[0],netmask_p[1],netmask_p[2],netmask_p[3]);
		}

			websWrite(wp,
    		"		</select>" \
	    	"</td></tr>\n");

		websWrite(wp,
    		"<tr>\n" \
	    	"   <td bgColor=#aaddff colspan=1>NAPT for LAN hosts</td>\n" \
    		"   <td bgColor=#ddeeff colspan=3>" \
    		"   <input type=checkbox name=\"unnumberedNapt\" value=\"1\" %s>" \
    		"</tr>",(pRomeCfgParam->pppoeCfgParam[i].unnumberedNapt==1)?"checked":"");
	    	    	
		}

	   websWrite(wp,
    	"<tr>\n" \
	    "   <td bgColor=#aaddff>Dial-On-Demand</td>\n" \
    	"   <td bgColor=#ddeeff><input type=checkbox value=1 name=demand%d %s onclick=\"javascript:clickDialOnDemand%d(); \"></td>\n" \
	    "   <td bgColor=#aaddff>Silent Timeout</td>\n" \
    	"   <td bgColor=#ddeeff><input type=text size=8 maxlength=8 name=silentTimeout%d value=\"%d\" onblur=\"javascript:changeSilentTimeout%d();\">seconds</td>\n" \
	    "</tr>\n",i,((pRomeCfgParam->pppoeCfgParam[i].demand)==1)?"checked":"",i,i,(pRomeCfgParam->pppoeCfgParam[i].silentTimeout),i);

			
		action=0; //0:Normal(disconnect), 1:Can't Dial(connecting...,connected), 2:Can't Dial and Hang-up.(config changed)
		if(pRomeCfgParam->pppoeCfgParam[i].dialState!=PPPOECFGPARAM_DIALSTATE_OFF) action=1;
		if(pRomeCfgParam->pppoeCfgParam[i].dialState==PPPOECFGPARAM_DIALSTATE_DIALED_TRYING) action=1;
		if(pppoeType!=pRomeCfgParam->ifCfgParam[0].connType) action=2;
		if(!((pppoeType==IFCFGPARAM_CONNTYPE_PPPOE)||(pppoeType==IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED)||(pppoeType==IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE))) action=2;
		if(configChanged) action=2;
		
		
		getDialStateString(i,str);
		
   		websWrite(wp,
	    "<tr>\n" \
	    "   <td bgColor=#aaddff>Auto Reconnect</td>\n" \
	    "   <td bgColor=#ddeeff><input type=checkbox value=1 name=autoReconnect%d %s onclick=\"javascript:clickAutoReconnect%d();\"></td>\n" \
	    "   <td bgColor=#aaddff>Dial Status</td>\n" \
    	"   <td bgColor=#ddeeff>%s</td>\n" \
	    "</tr>\n",i,(pRomeCfgParam->pppoeCfgParam[i].autoReconnect==1)?"checked":"",i,str);


		websWrite(wp,
    		"<tr>\n" \
	    	"   <td bgColor=#aaddff colspan=1>Performance</td>\n" \
    		"   <td bgColor=#ddeeff colspan=3>" \
    		"   %s routing %s</tr>",(pRomeCfgParam->pppoeCfgParam[i].defaultSession)?"Wirespeed":"Software",(pppoeType==IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED&&pRomeCfgParam->pppoeCfgParam[i].unnumberedNapt==0)?"":"and NAPT");


	if(wizard==0)
	{
   		websWrite(wp,
    		"<tr>\n" \
	    	"   <td align=center colspan=4>\n" \
	    	"       <input type=\"submit\" name=\"submit2\" value=\"Dial\" onClick=\"javascript:changeSessionID(%d);\" %s>\n" \
		"       <input type=\"submit\" name=\"submit2\" value=\"Hang up\" onClick=\"javascript:changeSessionID(%d);\" %s>\n" \
	    	"   </td>\n" \
	    	"</tr>"
		,i,(action>=1)?"disabled":"",i,(action==2)?"disabled":"");
	}

	websWrite(wp,"</table><BR>\n");
    } /* end for i */

   //for(i=0;i<MAX_PPPOE_SESSION;i++)
   //	if(pRomeCfgParam->pppoeCfgParam[i].dialState!=PPPOECFGPARAM_DIALSTATE_OFF) break;

   switch(wizard)
   {
    case IFCFGPARAM_CONNTYPE_PPPOE: //for wizard
    case IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED: //for wizard
    	websWrite(wp,
    	"<input type=\"button\" value=\"< Back\" onclick=\"javascript:history.back();\">&nbsp;&nbsp;&nbsp;&nbsp;\n" \
	"<input type=submit value=\"Save and Restart\" name=\"submitBtn\">&nbsp;&nbsp;&nbsp;&nbsp;\n" \
	"<input type=reset value=\"Reset\">&nbsp;&nbsp;&nbsp;&nbsp;\n" \
	"<input type=\"button\" value=\"Cancel\" onclick=\"javascript:location.replace('status.asp');\">\n");
    	break;    	
   default:
    	websWrite(wp,"<input type=\"button\" name=\"submit2\" value=\"Save All Configuration\" onclick=\"javascript:disconnect_alert();\">" \
    	"&nbsp;&nbsp;&nbsp;&nbsp;<input type=\"submit\" name=\"submit2\" value=\"Refresh\">" \
    	"&nbsp;&nbsp;&nbsp;&nbsp;<input type=\"reset\" value=\"Reset\"></form>");
    	break;    	
   }

}

/************************************************************
 *	
 *	Server Port
 *	
 ************************************************************/ 
void asp_serverPort(int ejid, webs_t wp, int argc, char_t **argv)
{
	uint8 i,j;
	int ms;
	
	if(pRomeCfgParam->natCfgParam.hwaccel == 0)
	{
		websWrite(wp,
		"<B><font color=red>This function must run in \"Hardware Acceleration\" mode, <BR>"\
		"please turn it on, and try again...\n");
		return;
	}

	//header
	websWrite(wp,
	"<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=big5\">\n" \
	"<title>Server Port Configuration</title>\n" \
	"<script language=javascript>\n" \
	"function changeMs(a)\n" \
	"{	fn.ms.value=a; 	fn.submit();  }\n" \
	"function changeRange(op)\n" \
	"{ \n");


	for(i=1;i<=MAX_SERVER_PORT;i++)
	{
		websWrite(wp,
		"	var wps%d=parseInt(spform.wps%d.value);\n" \
		"	var wpf%d=parseInt(spform.wpf%d.value);\n" \
		"	var sp%d=parseInt(spform.sp%d.value);\n",i,i,i,i,i,i);
	}

	for(i=1;i<=MAX_SERVER_PORT;i++)
	{
		websWrite(wp,
		"	if((wps%d>wpf%d)&&(op==3)) { spform.wpf%d.value=wps%d; spform.wps%d.value=wpf%d; spform.spf%d.value=wps%d-wpf%d+sp%d; }\n" \
		"	else {spform.spf%d.value=wpf%d-wps%d+sp%d;} \n",i,i,i,i,i,i,i,i,i,i,i,i,i,i);
	}

	websWrite(wp,
	"} \n" \
	"</script>\n" \
	"</head>\n" \
	"<body bgcolor=\"#ffffff\" text=\"#000000\"><b>Virtual Server</b>\n");

	{
		char *spargv[]={{"ms"}};
		ms=asp_multiSession(wp,1,spargv);
	}

	websWrite(wp,
	"<form action=\"serverp.asp\" name=\"fn\"><input type=hidden name=ms value=0></form>" \
	"<form method=get action=\"/goform/asp_setServerPort\" name=spform onSubmit=\"changeRange(3);\">\n" \
	"<input type=hidden name=ms value=%d>\n" \
	"<table cellSpacing=1 cellPadding=2 border=0>\n" \
	"<tr><td bgColor=#aaddff>WAN Port Range</td><td bgColor=#aaddff>Server IP Address</td><td bgColor=#aaddff>Server Port Range</td><td bgColor=#aaddff>Protocol</td><td bgColor=#aaddff>Enable</td></tr>\n",ms);

	for(i=1;i<=MAX_SERVER_PORT;i++)
	{
		//wan port
		websWrite(wp,"<tr><td bgColor=#ddeeff><input type=text name=wps%u size=5 maxlength=5 value=%u onKeyUp=\"changeRange(1);\">~<input type=text name=wpf%u size=5 maxlength=5 value=%u onKeyUp=\"changeRange(2);\">==></td><td bgColor=#ddeeff>\n",i,pRomeCfgParam->serverpCfgParam[ms][i-1].wanPortStart,i,pRomeCfgParam->serverpCfgParam[ms][i-1].wanPortFinish);

		// server ip
		for(j=1;j<=4;j++)
		{
			websWrite(wp,"<input type=text name=sip%u%u size=3 maxlength=3 value=%u>\n",i,j,pRomeCfgParam->serverpCfgParam[ms][i-1].ip[j-1]);
			if(j<4) websWrite(wp,".");
		}

		//server port
		websWrite(wp,"</td><td bgColor=#ddeeff><input type=text name=sp%u size=5 maxlength=5 value=%u onKeyUp=\"changeRange(3);\">~<input type=text name=spf%u size=5 maxlength=5 value=%u disabled></td>\n",i,pRomeCfgParam->serverpCfgParam[ms][i-1].portStart,i,pRomeCfgParam->serverpCfgParam[ms][i-1].wanPortFinish-pRomeCfgParam->serverpCfgParam[ms][i-1].wanPortStart+pRomeCfgParam->serverpCfgParam[ms][i-1].portStart);

		//protocol
		websWrite(wp,"</td><td bgColor=#ddeeff><select name=proto%u><option value=0 %s>TCP</option><option value=1 %s>UDP</option></td>\n",i,(pRomeCfgParam->serverpCfgParam[ms][i-1].protocol==SERVERPCFGPARAM_PROTOCOL_TCP)?"selected":"",(pRomeCfgParam->serverpCfgParam[ms][i-1].protocol==SERVERPCFGPARAM_PROTOCOL_UDP)?"selected":"");

		//enable
		websWrite(wp,"<td bgColor=#ddeeff><input type=checkbox name=e%u value=1 %s></td>\n",i,(pRomeCfgParam->serverpCfgParam[ms][i-1].enable==1)?"checked":"");

	} //end for

	websWrite(wp,
	"<tr><td colspan=3 align=center><input type=submit value=Apply>&nbsp;&nbsp;&nbsp;&nbsp;<input type=reset value=Reset></td></tr>\n" \
	"</tr></table>");
	websWrite(wp,"<ul><li>Virtual servers using single port number is accelerated by hardware at wirespeed.</ul></body></html>");
	
	return;
}

void asp_setServerPort(webs_t wp, char_t *path, char_t *query)
{
	char tmpstr[32]={0};
	uint8 i,j;
	int	ms=atoi(websGetVar(wp,"ms",""));

	memcpy(ramServerpCfgParam,pRomeCfgParam->serverpCfgParam,sizeof(serverpCfgParam_t)*MAX_SERVER_PORT*MAX_PPPOE_SESSION);
	for(i=0;i<MAX_SERVER_PORT;i++)
	{		
		sprintf(tmpstr,"wps%u",i+1);
		pRomeCfgParam->serverpCfgParam[ms][i].wanPortStart=atoi(websGetVar(wp,tmpstr,""));

		sprintf(tmpstr,"wpf%u",i+1);
		pRomeCfgParam->serverpCfgParam[ms][i].wanPortFinish=atoi(websGetVar(wp,tmpstr,""));
		
		
		for(j=0;j<4;j++)
		{
			sprintf(tmpstr,"sip%u%u",i+1,j+1);
			pRomeCfgParam->serverpCfgParam[ms][i].ip[j]=atoi(websGetVar(wp,tmpstr,""));
		}

		sprintf(tmpstr,"sp%u",i+1);
		pRomeCfgParam->serverpCfgParam[ms][i].portStart=atoi(websGetVar(wp,tmpstr,""));

		sprintf(tmpstr,"proto%u",i+1);
		pRomeCfgParam->serverpCfgParam[ms][i].protocol=atoi(websGetVar(wp,tmpstr,""));
		
		sprintf(tmpstr,"e%u",i+1);
		pRomeCfgParam->serverpCfgParam[ms][i].enable=atoi(websGetVar(wp,tmpstr,""));
		
	} /* end for i */

	serverp_tableDriverAccess(ms); /* return value not important ? */
	
	/* save to cfgmgr */
	cfgmgr_write(CFGMGR_TABID_SERVERP,(void*)pRomeCfgParam->serverpCfgParam,sizeof(struct serverpCfgParam_s)*MAX_SERVER_PORT*MAX_PPPOE_SESSION);
	cfgmgr_task();

	sprintf(tmpstr,"serverp.asp?ms=%d",ms);
	websRedirect(wp, tmpstr);
}


/************************************************************
 *	
 *	Protocol-Based NAT
 *	
 ************************************************************/ 
void asp_pbnat(int ejid, webs_t wp, int argc, char_t **argv)
{
	uint8 i,j;
	int ms;
	

	char *spargv[]={{"ms"}};
	ms=asp_multiSession(wp,1,spargv);

	websWrite(wp,"\n\n\t<form action=\"/goform/asp_setPbnat\" name=\"fm\">\n");
	websWrite(wp,"\t<input type=hidden name=ms value=%d>\n",ms);
	websWrite(wp,"\t<table><tr><th>IP Protocol Number</th><th>Internal IP</th><th>Enabled</th></tr>\n");
	
	for(i=0;i<MAX_PBNAT;i++)
	{
		//protocol
		ipaddr_t IntIp = pRomeCfgParam->pbnatCfgParam[ms][i].IntIp;
		
		websWrite(wp,
			"\t\t<tr>"
			"\t\t<td><input type=input name=protocol_%d size=3 maxlength=3 value='%d'></td>"
			"\t\t<td><input type=input name=IntIp_%d size=15 maxlength=15 value='%d.%d.%d.%d'></td>"
			"\t\t<td><input type=checkbox name=enabled_%d value='1' %s></td>"
			"\t\t</tr>\n",
			i, pRomeCfgParam->pbnatCfgParam[ms][i].protocol,
			i, IntIp>>24, (IntIp&0x00ff0000)>>16, (IntIp&0x0000ff00)>>8,  IntIp&0x000000ff,
			i, pRomeCfgParam->pbnatCfgParam[ms][i].enabled?"checked":"" );
	}


	websWrite(wp,"\t</table>\n");
	websWrite(wp,"\t<input type=submit value='Apply'>\n");
	websWrite(wp,"\t</form>\n");
	
	return;
}

void asp_setPbnat(webs_t wp, char_t *path, char_t *query)
{
	char tmpstr[32]={0};
	uint8 i,j;
	char* pch;
	int	ms;
	ipaddr_t msIp;
	
	ms = atoi(websGetVar(wp,"ms",""));
	msIp = board_getExternalIp( ms );

	/* clear table first */
	pbnat_ipdown( ms, msIp );

	/* then, update configuration according to form request */
	for(i=0;i<MAX_PBNAT;i++)
	{		
		sprintf(tmpstr,"protocol_%u",i);
		pRomeCfgParam->pbnatCfgParam[ms][i].protocol = atoi(websGetVar(wp,tmpstr,""));

		pRomeCfgParam->pbnatCfgParam[ms][i].IntIp = 0;
		sprintf(tmpstr,"IntIp_%u",i);
		for( j = 0, pch = strtok( websGetVar(wp,tmpstr,""), "." );
		     j < 4 && pch;
		     j++, pch = strtok( NULL, "." ) )
		{
			pRomeCfgParam->pbnatCfgParam[ms][i].IntIp |= atoi(pch) << ((3-j)*8);
		}
		
		sprintf(tmpstr,"enabled_%u",i);
		pRomeCfgParam->pbnatCfgParam[ms][i].enabled = atoi(websGetVar(wp,tmpstr,""));
	}

	//serverp_tableDriverAccess(ms); /* return value not important ? */
	
	/* save to cfgmgr */
	cfgmgr_write(CFGMGR_TABID_PBNAT,(void*)pRomeCfgParam->pbnatCfgParam,sizeof(struct pbnatCfgParam_s)*MAX_PBNAT*MAX_PPPOE_SESSION);
	cfgmgr_task();

	/* insert to table again */
	pbnat_ipup( ms, msIp );

	sprintf(tmpstr,"pbnat.asp?ms=%d",ms);
	websRedirect(wp, tmpstr);
}


/************************************************************
 *	
 *	URL Filter
 *	
 ************************************************************/ 
int asp_urlFilter(int ejid, webs_t wp, int argc, char_t **argv)
{
	int i;
	int setted=0;
	int ms=asp_multiSession(wp,argc,argv);
	int8 string[32];

	if(strcmp(argv[0],"ms")!=0)
	{
		websWrite(wp,"<input type=hidden name=ms value=%d>",ms);	
		for(i=0;i<MAX_URL_FILTER;i++)
		{
			sprintf(string, "%u.%u.%u.%u-%u.%u.%u.%u", NIPQUAD(pRomeCfgParam->urlfilterCfgParam[ms][i].ip_start), NIPQUAD(pRomeCfgParam->urlfilterCfgParam[ms][i].ip_end));
			websWrite(wp,"<tr><td bgColor=#ddeeff><input type=text size=32 maxlength=32 name=string%d value=\"%s\"></td>\n",i,pRomeCfgParam->urlfilterCfgParam[ms][i].string);
			websWrite(wp, "<td bgColor=#ddeeff align=center><input type=text size=31 maxlength=31 name=ipRange%d value=\"%s\"></td>\n", i, string);
			websWrite(wp,"<td bgColor=#ddeeff align=center><input type=checkbox value=1 name=enable%d %s></td></tr>\n",i,(pRomeCfgParam->urlfilterCfgParam[ms][i].enable==1)?"checked":"");			
		}
	}
	
	return 0;

}

void asp_setUrlFilter(webs_t wp, char_t *path, char_t *query)
{
	char tmpstr[32]={0};
	int i;
	int ms=atoi(websGetVar(wp,"ms",""));
	
	for(i=0;i<MAX_URL_FILTER;i++)
	{
		//delete old url filter string
		if(pRomeCfgParam->urlfilterCfgParam[ms][i].enable==1)
		{
			//rtl8651a_delURLfilterString(ms,pRomeCfgParam->urlfilterCfgParam[ms][i].string, strlen(pRomeCfgParam->urlfilterCfgParam[ms][i].string));
			rtl8651_delURLFilterRule(ms, pRomeCfgParam->urlfilterCfgParam[ms][i].string, strlen(pRomeCfgParam->urlfilterCfgParam[ms][i].string), 
								pRomeCfgParam->urlfilterCfgParam[ms][i].ip_start, pRomeCfgParam->urlfilterCfgParam[ms][i].ip_end);
		}
		
		//get new url filter string params
		sprintf(tmpstr,"string%u",i);
		memcpy(pRomeCfgParam->urlfilterCfgParam[ms][i].string,websGetVar(wp,tmpstr,""),sizeof(pRomeCfgParam->urlfilterCfgParam[ms][i].string));		
		sprintf(tmpstr,"enable%u",i);
		pRomeCfgParam->urlfilterCfgParam[ms][i].enable=atoi(websGetVar(wp,tmpstr,"")); 
		sprintf(tmpstr, "ipRange%u", i);
		parse_ipRange(websGetVar(wp, tmpstr, ""), &pRomeCfgParam->urlfilterCfgParam[ms][i].ip_start, &pRomeCfgParam->urlfilterCfgParam[ms][i].ip_end);
		
		
		//add new url filter string
		if(pRomeCfgParam->urlfilterCfgParam[ms][i].enable==1)
		{
			//rtl8651a_addURLfilterString(ms,pRomeCfgParam->urlfilterCfgParam[ms][i].string, strlen(pRomeCfgParam->urlfilterCfgParam[ms][i].string));
			rtl8651_addURLFilterRule(ms, pRomeCfgParam->urlfilterCfgParam[ms][i].string, strlen(pRomeCfgParam->urlfilterCfgParam[ms][i].string), 
								pRomeCfgParam->urlfilterCfgParam[ms][i].ip_start, pRomeCfgParam->urlfilterCfgParam[ms][i].ip_end);
		}
		sprintf(tmpstr,"ipRange%u",i);
		parse_ipRange(websGetVar(wp,tmpstr,""),   &pRomeCfgParam->urlfilterCfgParam[ms][i].ip_start, &pRomeCfgParam->urlfilterCfgParam[ms][i].ip_end);
	}
		
	cfgmgr_write(CFGMGR_TABID_URLFILTER,(void*)pRomeCfgParam->urlfilterCfgParam,sizeof(urlfilterCfgParam_t)*MAX_URL_FILTER*MAX_PPPOE_SESSION);
	cfgmgr_task();

	sprintf(tmpstr,"urlfilter.asp?ms=%d",ms);
	websRedirect(wp, tmpstr);	
	
}

/************************************************************
 *	
 *	PING
 *	
 ************************************************************/
static int ping_sec=0; 
int asp_ping(int ejid, webs_t wp, int argc, char_t **argv)
{
	char tempStr[160];
	FILE *f,*f2;
	int finished=0;
	if(strcmp(argv[0],"pingstr")==0)
	{
		websWrite(wp,"%s",websGetVar(wp,"pingstr",""));
		return 0;
	}
	f2=fopen("/var/run/ping.pid","r");
	if(f2!=NULL)
	{
		if(strcmp(argv[0],"refresh")==0)
			{
				ping_sec++;				
				sprintf(tempStr,"<META HTTP-EQUIV=Refresh CONTENT=\"1; URL=ping.asp?pingstr=%s\">",websGetVar(wp,"pingstr",""));
				websWrite(wp,tempStr);
				fclose(f2);
				return 0;				
			}
		if(strcmp(argv[0],"enable")==0)
			{
				websWrite(wp,"disabled");
				fclose(f2);
				return 0;
			}	
		fclose(f2);	
	}
	
	if(strcmp(argv[0],"response")==0)
	{
		f=fopen("/var/run/ping.txt","r");
		if(f!=NULL)
		{				
			while(!feof(f))
			{
				fgets(tempStr,160,f);
				if(memcmp(tempStr,"--END--",7)==0) {ping_sec=0;finished=1;break;}
				if(feof(f)) break;
				websWrite(wp,"%s<BR>",tempStr);
				
			}
			fclose(f);
		}
		else
		{
			if(ping_sec>=20)
			{				
				ping_sec=0;
				websWrite(wp,"Destination Unreachable");
			}
			else if(ping_sec>=19)
			{
				unlink("/var/run/ping.pid");
				websWrite(wp,"Destination Unreachable");
			}
			else if(ping_sec>=1)
			{
				websWrite(wp,"Pinging...");
			}
			else
				websWrite(wp,"&lt;Empty&gt;");
		}
		if(finished==1) unlink("/var/run/ping.txt");
	}
	return 0;
}

void asp_setPing(webs_t wp, char_t *path, char_t *query)
{
	char tempStr[160];
	ping_sec=0;	
	sprintf(tempStr,"ping -f -c 4 %s &",websGetVar(wp,"pingstr",""));
	system(tempStr);
	sleep(1);
	sprintf(tempStr,"ping.asp?pingstr=%s",websGetVar(wp,"pingstr",""));
	websRedirect(wp, tempStr);
}

/************************************************************
 *	
 *	Time Zone
 *	
 ************************************************************/
int asp_TZ(int ejid, webs_t wp, int argc, char_t **argv)
{
	if(atoi(argv[0])==pRomeCfgParam->timeCfgParam.timeZoneIndex) websWrite(wp," selected");
	return 0;
}

int asp_TZ2(int ejid, webs_t wp, int argc, char_t **argv)
{
	if(strcmp(argv[0],"ntpServer1")==0) websWrite(wp,"%s",pRomeCfgParam->timeCfgParam.ntpServer1);
	if(strcmp(argv[0],"ntpServer2")==0) websWrite(wp,"%s",pRomeCfgParam->timeCfgParam.ntpServer2);
	if(strcmp(argv[0],"ntpServer3")==0) websWrite(wp,"%s",pRomeCfgParam->timeCfgParam.ntpServer3);
	if(strcmp(argv[0],"dateTime")==0) 
	{
		clock_t t;
		time(&t);		
		websWrite(wp,"%s (GMT ",ctime(&t));
		if(pRomeCfgParam->timeCfgParam.timeZone[0]=='-')
			websWrite(wp,"+");
		else
			websWrite(wp,"-");
		websWrite(wp,"%s)",pRomeCfgParam->timeCfgParam.timeZone+1);	
		
	}
	return 0;
}

void asp_setTZ(webs_t wp, char_t *path, char_t *query)
{
	char tmp[32]={0};
	if(strcmp(websGetVar(wp,"submit",""),"Save / Time Sync")==0)
	{
		int32 timezone_diff;
		memcpy(tmp,websGetVar(wp,"timeZone",""),3);
		pRomeCfgParam->timeCfgParam.timeZoneIndex=atoi(tmp);
		strcpy(pRomeCfgParam->timeCfgParam.ntpServer1,websGetVar(wp,"ntpServer1",""));
		strcpy(pRomeCfgParam->timeCfgParam.ntpServer2,websGetVar(wp,"ntpServer2",""));
		strcpy(pRomeCfgParam->timeCfgParam.ntpServer3,websGetVar(wp,"ntpServer3",""));
		strcpy(tmp,websGetVar(wp,"timeZone",""));
		strcpy(pRomeCfgParam->timeCfgParam.timeZone,(char *)(tmp+3));
		
		timezone_diff=(tmp[4]-'0')*36000+(tmp[5]-'0')*3600+(tmp[7]-'0')*600+(tmp[8]-'0')*60;
		if(tmp[3]=='+') timezone_diff*=-1;
		rtl8651_setTimeZone(timezone_diff);

		sprintf(tmp,"std%s",pRomeCfgParam->timeCfgParam.timeZone);
		setenv("TZ",tmp, 1);
		
		time_start();
		cfgmgr_write(CFGMGR_TABID_TIME,(void*)&pRomeCfgParam->timeCfgParam,sizeof(timeCfgParam_t));
		cfgmgr_task();
	}
	websRedirect(wp, "mgmt.asp");
}


/************************************************************
 *	
 *	Remote Management
 *	
 ************************************************************/
int asp_remoteMgmt(int ejid, webs_t wp, int argc, char_t **argv)
{
	if(strcmp(argv[0],"remoteMgmtIp")==0)  websWrite(wp,"%u.%u.%u.%u",NIPQUAD(pRomeCfgParam->mgmtCfgParam[0].remoteMgmtIp));
	if(strcmp(argv[0],"remoteMgmtMask")==0)  websWrite(wp,"%u.%u.%u.%u",NIPQUAD(pRomeCfgParam->mgmtCfgParam[0].remoteMgmtMask));
	if(strcmp(argv[0],"remoteMgmtPort")==0)  websWrite(wp,"%d",pRomeCfgParam->mgmtCfgParam[0].remoteMgmtPort);
	if(strcmp(argv[0],"remoteIcmpEnable")==0)  if(pRomeCfgParam->mgmtCfgParam[0].remoteIcmpEnable==1) websWrite(wp,"checked");
	return 0;
}

void asp_setRemoteMgmt(webs_t wp, char_t *path, char_t *query)
{
	uint32 tmp;
	pRomeCfgParam->mgmtCfgParam[0].remoteMgmtPort=atoi(websGetVar(wp,"remoteMgmtPort",""));
	pRomeCfgParam->mgmtCfgParam[0].remoteIcmpEnable=atoi(websGetVar(wp,"remoteIcmpEnable",""));	
	
	parse_ipRange(websGetVar(wp,"remoteMgmtIp",""),&pRomeCfgParam->mgmtCfgParam[0].remoteMgmtIp,&tmp);
	parse_ipRange(websGetVar(wp,"remoteMgmtMask",""),&pRomeCfgParam->mgmtCfgParam[0].remoteMgmtMask,&tmp);	
	remoteMgmt_tableDriverAccess();
	cfgmgr_write(CFGMGR_TABID_MGMT,(void*)pRomeCfgParam->mgmtCfgParam,sizeof(mgmtCfgParam_t)*MAX_MGMT_USER);
	cfgmgr_task();	
	websRedirect(wp, "mgmt.asp");
}

/************************************************************
 *	
 *	User Management
 *	
 ************************************************************/
int asp_user(int ejid, webs_t wp, int argc, char_t **argv)
{
	if(strcmp(argv[0],"account")==0)  websWrite(wp,"%s",pRomeCfgParam->mgmtCfgParam[0].name);
	if(strcmp(argv[0],"password")==0)  websWrite(wp,"%s",pRomeCfgParam->mgmtCfgParam[0].password);
	return 0;
}

void asp_setUser(webs_t wp, char_t *path, char_t *query)
{
				
	strcpy(pRomeCfgParam->mgmtCfgParam[0].name,websGetVar(wp,"account",""));
	strcpy(pRomeCfgParam->mgmtCfgParam[0].password,websGetVar(wp,"password",""));

	time_start();
	cfgmgr_write(CFGMGR_TABID_MGMT,(void*)pRomeCfgParam->mgmtCfgParam,sizeof(mgmtCfgParam_t)*MAX_MGMT_USER);
	cfgmgr_task();
	
	websRedirect(wp, "mgmt.asp");
}

/************************************************************
 *	
 *	Napt Options
 *	
 ************************************************************/
#define NAPT_OPTCNT	4

void asp_setNaptOpt(webs_t wp, char_t *path, char_t *query)
{
	char tmpstr[64];
	uint32 idx;
	char* naptOptItem[NAPT_OPTCNT] = {"UDP_Static_Port_Translation", "TCP_Static_Port_Translation", "TCP_Weak_NAPT","LOOSE_UDP"};
	uint8* romeCfg[NAPT_OPTCNT] =
		{	&pRomeCfgParam->naptCfgParam.udpStaticPortMappingEnable,
			&pRomeCfgParam->naptCfgParam.tcpStaticPortMappingEnable,
			&pRomeCfgParam->naptCfgParam.weakNaptEnable,
			&pRomeCfgParam->naptCfgParam.looseUdpEnable
		};
	
	/* store napt options */
	for (idx = 0 ; idx < NAPT_OPTCNT ; idx ++)
	{
		sprintf(tmpstr, "Enable_%s", naptOptItem[idx]);
		*romeCfg[idx] = atoi(websGetVar(wp,tmpstr,""));
	}

	/* set napt options */
	rtl8651_WeakNaptCtrl(pRomeCfgParam->naptCfgParam.weakNaptEnable);
	rtl8651_TcpNaptStaticPortMappingCtrl(pRomeCfgParam->naptCfgParam.tcpStaticPortMappingEnable);
	rtl8651_UdpNaptStaticPortMappingCtrl(pRomeCfgParam->naptCfgParam.udpStaticPortMappingEnable);
	rtl8651_looseUdpCtrl(pRomeCfgParam->naptCfgParam.looseUdpEnable);
	/* update flash and refresh web page */
	cfgmgr_write(CFGMGR_TABID_NAPT, (void*)&(pRomeCfgParam->naptCfgParam), sizeof(struct naptCfgParam_s));
	cfgmgr_task();
	sprintf(tmpstr, "naptopt.asp");
	websRedirect(wp, tmpstr);
}

int asp_naptOpt(int ejid, webs_t wp, int argc, char_t **argv)
{
	int32 idx;
	char* naptOptName[NAPT_OPTCNT] = {"Default don't masquerade source port number for new UDP flows", "Default don't masquerade source port number for new TCP flows", "Create TCP flows without stateful tracking","Create UDP flows regardless of destination info"};
	char* naptOptItem[NAPT_OPTCNT] = {"UDP_Static_Port_Translation", "TCP_Static_Port_Translation", "TCP_Weak_NAPT","LOOSE_UDP"};
	uint8 romeCfg[NAPT_OPTCNT] =
		{pRomeCfgParam->naptCfgParam.udpStaticPortMappingEnable,
		pRomeCfgParam->naptCfgParam.tcpStaticPortMappingEnable,
		pRomeCfgParam->naptCfgParam.weakNaptEnable,
		pRomeCfgParam->naptCfgParam.looseUdpEnable
	};

	if (strcmp(argv[0], "ListALL") == 0)
	{	/* List setting fields of all options */
		for (idx = 0 ; idx < NAPT_OPTCNT ; idx ++)
		{
			websWrite(wp, "<tr>");
				websWrite(wp, "<td bgColor=#aaddff>%s</td>", naptOptName[idx]);
				websWrite(wp, "<td bgColor=#ddeeff>");
				websWrite(wp, "<input type=checkbox name=\"Enable_%s\" value=1 %s>", naptOptItem[idx], (romeCfg[idx]?"checked":""));
				websWrite(wp, "</td>");
			websWrite(wp, "</tr>");
		}
	}

	return 0;
}

/************************************************************
 *	
 *	DoS
 *	
 ************************************************************/

#define asp_dos_PktInspectDosCheckCnt		11
// for flood check
#define asp_dos_PktCountDosCnt				2
#define asp_dos_PktCountDosCheckCnt		4
/* for port scan check */
#define asp_dos_AdjustableDosCnt			1
#define asp_dos_AdjustableDosLevelCnt		2

int asp_dos(int ejid, webs_t wp, int argc, char_t **argv)
{
	int i, j;
	int ms=asp_multiSession(wp,argc,argv);

	char* PktInspectDosList[]			= {"TcpScan", "TcpSynWithData", "TcpLand", "UdpEchoChargen", "UdpBomb", "UdpLand", "PingOfDeath", "IcmpSmurf", "IcmpLand", "IpSpoof", "TearDrop"};
	char* PktCountDosList[] 			= {"Whole System Flood", "Per-Source IP Flood"};
	char* PktCountDosCheckList[]		= {"SYN", "FIN", "UDP", "ICMP"};
	char* AdjustableList[]				= {"TcpUdpPortScan"};
	char* AdjustableLevelList[]		= {"Low", "High"};

	if (strcmp(argv[0], "ENABLE") == 0)
	{
		if (pRomeCfgParam->dosCfgParam[ms].enable)
			websWrite(wp, "checked");
	}else if (strcmp(argv[0],"MS") == 0)
	{
		websWrite(wp, "%d", ms);
	}else if (strcmp(argv[0], "Ignore_Lan") == 0)
	{
		if (pRomeCfgParam->dosCfgParam[ms].ignoreLanSideCheck)
			websWrite(wp, "checked");
	}else if (strcmp(argv[0],"dosList")==0)
	{
		char* checkStream = "checked";
		char* nullStream = "";
		/* Show dos List */
		
		/* flood check */
		for (i = 0 ; i < asp_dos_PktCountDosCnt ; i ++)
		{
			websWrite(wp,	"<tr>\n" \
							"<td bgColor=#aaddff>%s</td>\n" \
							"<td>\n" \
							"<table width=100%%>\n", PktCountDosList[i]);

			for (j = 0 ; j < asp_dos_PktCountDosCheckCnt ; j ++)
			{
				char** checkPtr;
				uint16 threshold;
				checkPtr = &nullStream;
				switch(j)
				{
					case 0:	/* SYN */
						if (i == 0)
						{
							threshold = pRomeCfgParam->dosCfgParam[ms].syn;
							if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_SYNFLOOD)		checkPtr = &checkStream;
						}
						if (i == 1)
						{
							threshold = pRomeCfgParam->dosCfgParam[ms].perSrc_syn;
							if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_SRC_SYNFLOOD)	checkPtr = &checkStream;
						}
						break;
					case 1:	/* FIN */
						if (i == 0)
						{
							threshold = pRomeCfgParam->dosCfgParam[ms].fin;
							if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_STEALTHFIN)		checkPtr = &checkStream;
						}
						if (i == 1)
						{
							threshold = pRomeCfgParam->dosCfgParam[ms].perSrc_fin;
							if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_SRC_STEALTHFIN)	checkPtr = &checkStream;
						}
						break;
					case 2:	/* UDP */
						if (i == 0)
						{
							threshold = pRomeCfgParam->dosCfgParam[ms].udp;
							if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_UDPFLOOD)		checkPtr = &checkStream;
						}
						if (i == 1)
						{
							threshold = pRomeCfgParam->dosCfgParam[ms].perSrc_udp;
							if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_SRC_UDPFLOOD)	checkPtr = &checkStream;
						}
						break;
					case 3:	/* ICMP */
						if (i == 0)
						{
							threshold = pRomeCfgParam->dosCfgParam[ms].icmp;
							if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_ICMPFLOOD)		checkPtr = &checkStream;
						}
						if (i == 1)
						{
							threshold = pRomeCfgParam->dosCfgParam[ms].perSrc_icmp;
							if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_SRC_ICMPFLOOD)	checkPtr = &checkStream;
						}
						break;
				}
				websWrite(wp,	"<tr>\n"\
								"<td bgColor=#ddeeff><input type=checkbox name=Enable_%s_%d value=1 %s></td>\n"\
								"<td bgColor=#aaddff>%s</td>\n"\
								"<td bgColor=#ddeeff><input type=text name=Count_%s_%d value=%d size=3 maxlength=3 >Packets/Second</td>\n" \
								"</tr>\n", \
								PktCountDosCheckList[j], i, *checkPtr, PktCountDosCheckList[j], PktCountDosCheckList[j], i, threshold);
			}
			
			websWrite(wp,	"</table>\n" \
							"</td>\n" \
							"</tr>\n");
		}
		/* adjustable check */
		for (i = 0 ; i < asp_dos_AdjustableDosCnt ; i ++)
		{
				char** checkPtr;

				checkPtr = &nullStream;
				switch(i)
				{
					case 0:	/* TCP/UDP Port Scan */
						if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_PORTSCAN)
							checkPtr = &checkStream;
						break;
				}		
			websWrite(wp,	"<tr>\n" \
				"<td bgColor=#aaddff>%s</td>\n" \
				"<td>\n" \
				"<table  width=100%%>\n", AdjustableList[i]);

			/* generate check box and Level select icon */
			websWrite(wp, "<tr>\n"\
							"<td bgColor=#ddeeff><input type=checkbox name=Enable_%s value=1 %s></td>\n"\
							"<td bgColor=#aaddff>Level</td>\n"\
							"<td bgColor=#ddeeff><select name=Level_%s>"\
							, AdjustableList[i], *checkPtr, AdjustableList[i]);
			for (j = 0 ; j < asp_dos_AdjustableDosLevelCnt ; j ++)
			{	/* list total Levels */
				websWrite(wp, "<option  value=%d %s>%s", j, ((pRomeCfgParam->dosCfgParam[ms].portScan_level == j)?"selected":""), AdjustableLevelList[j]);
			}
			websWrite(wp, "</select>Sensitivity</td></tr>\n");
			websWrite(wp,	"</table>\n" \
							"</td>\n" \
							"</tr>\n");
		}
		
		/* packet inspect check */
		for (i = 0 ; i < asp_dos_PktInspectDosCheckCnt ; i ++)
		{
			char** checkPtr;
			checkPtr = &nullStream;
			switch(i)
			{
				case 0:	/* tcp scan */
					if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_SCAN)		checkPtr = &checkStream;
					break;
				case 1:	/* syn with data */
					if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_SYNWITHDATA)	checkPtr = &checkStream;
					break;
				case 2:	/* tcp land */
					if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_TCPLAND)		checkPtr = &checkStream;
					break;
				case 3:	/* udp echo chargen */
					if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_ECHOCHARGEN)	checkPtr = &checkStream;
					break;
				case 4:	/* udp bomb */
					if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_UDPBOMB)		checkPtr = &checkStream;
					break;
				case 5:	/* udp land */
					if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_UDPLAND)		checkPtr = &checkStream;
					break;
				case 6:	/* ping of death */
					if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_PINGOFDEATH)	checkPtr = &checkStream;
					break;
				case 7:	/* icmp smurf */
					if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_SMURF)		checkPtr = &checkStream;
					break;
				case 8:	/* icmp land */
					if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_ICMPLAND)	checkPtr = &checkStream;
					break;
				case 9:	/* ip spoof */
					if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_IPSPOOF)		checkPtr = &checkStream;
					break;
				case 10:	/* tear drop */
					if (pRomeCfgParam->dosCfgParam[ms].enableItem&RTL8651_DOS_TEARDROP)	checkPtr = &checkStream;
					break;
			}
			websWrite(wp,	"<tr>\n"  \
							"<td bgColor=#aaddff>%s</td>\n" \
							"<td bgColor=#ddeeff><input type=checkbox name=Enable_%s value=1 %s></td>\n" \
							"</tr>\n" \
							, PktInspectDosList[i], PktInspectDosList[i], *checkPtr);
		}
		/* "select all" button and "clear all" button */
		websWrite(wp,	"<tr>\n" \
						"<td bgColor=#aaddff></td>\n" \
						"<td bgColor=#ddeeff>\n" \
						"<input type=button align=center name=selectall value=\"SelectALL\" onClick=\"dos_SelectAll();\">\n" \
						"<input type=button align=center name=clearall value=\"ClearALL\" onClick=\"dos_ClearAll();\">\n" \
						"</td></tr>\n");

	}else if (strcmp(argv[0], "dosItemAvailable") == 0)
	{	/* make check list available */
		/* flood check */
		for (i = 0 ; i < asp_dos_PktCountDosCnt ; i++)
		for (j = 0 ; j < asp_dos_PktCountDosCheckCnt ; j++)
		{
			websWrite(wp,	"\t\tdos.Enable_%s_%d.disabled = false;\n" \
							"\t\tdos.Count_%s_%d.disabled = false;\n" \
							, PktCountDosCheckList[j], i, PktCountDosCheckList[j], i);
		}
		/* level Adjustable check */
		for (i = 0 ; i < asp_dos_AdjustableDosCnt ; i ++)
		{
			websWrite(wp,	"\t\tdos.Enable_%s.disabled = false;\n" \
							"\t\tdos.Level_%s.disabled = false;\n" \
							, AdjustableList[i], AdjustableList[i]);
		}
		/* packet inspect check */
		for (i = 0 ; i < asp_dos_PktInspectDosCheckCnt ; i ++)
		{
			websWrite(wp,	"\t\tdos.Enable_%s.disabled = false;\n" \
							, PktInspectDosList[i]);
		}
		/* select/clear all button */
		websWrite(wp,		"\t\tdos.selectall.disabled = false;\n" \
							"\t\tdos.clearall.disabled = false;\n");
	}else if (strcmp(argv[0], "dosItemUnavailable") == 0)
	{	/* make check list Unavailable */
		/* flood check */
		for (i = 0 ; i < asp_dos_PktCountDosCnt ; i++)
		for (j = 0 ; j < asp_dos_PktCountDosCheckCnt ; j++)
		{
			websWrite(wp,	"\t\tdos.Enable_%s_%d.disabled = true;\n" \
							"\t\tdos.Count_%s_%d.disabled = true;\n" \
							, PktCountDosCheckList[j], i, PktCountDosCheckList[j], i);
		}
		/* level Adjustable check */
		for (i = 0 ; i < asp_dos_AdjustableDosCnt ; i ++)
		{
			websWrite(wp,	"\t\tdos.Enable_%s.disabled = true;\n" \
							"\t\tdos.Level_%s.disabled = true;\n" \
							, AdjustableList[i], AdjustableList[i]);
		}
		/* packet inspect check */
		for (i = 0 ; i < asp_dos_PktInspectDosCheckCnt ; i ++)
		{
			websWrite(wp,	"\t\tdos.Enable_%s.disabled = true;\n" \
							, PktInspectDosList[i]);
		}
		/* select/clear all button */
		websWrite(wp,		"\t\tdos.selectall.disabled = true;\n" \
							"\t\tdos.clearall.disabled = true;\n");
	}else if (strcmp(argv[0], "ThresHoldCheck") == 0)
	{
		for (i = 0 ; i < asp_dos_PktCountDosCnt ; i++)
		for (j = 0 ; j < asp_dos_PktCountDosCheckCnt ; j++)
		{
			websWrite(wp,	"\t\tif ((dos.Enable_%s_%d.checked == 1) && (dos.Count_%s_%d.value == 0))\n" \
							"\t\t{alert('ThresHold Can NOT be 0.'); return false;}\n" \
							, PktCountDosCheckList[j], i, PktCountDosCheckList[j], i);
		}
	}else if (strcmp(argv[0], "SelectAllItem") == 0)
	{
		/* flood check */
		for (i = 0 ; i < asp_dos_PktCountDosCnt ; i++)
		for (j = 0 ; j < asp_dos_PktCountDosCheckCnt ; j++)
		{
			websWrite(wp,	"\t\tdos.Enable_%s_%d.checked = true;\n" \
							, PktCountDosCheckList[j], i);
		}
		/* level Adjustable check */
		for (i = 0 ; i < asp_dos_AdjustableDosCnt ; i ++)
		{
			websWrite(wp,	"\t\tdos.Enable_%s.checked = true;\n" \
							, AdjustableList[i]);
		}
		/* packet inspect check */
		for (i = 0 ; i < asp_dos_PktInspectDosCheckCnt ; i ++)
		{
			websWrite(wp,	"\t\tdos.Enable_%s.checked = true;\n" \
							, PktInspectDosList[i]);
		}
	}else if (strcmp(argv[0], "ClearAllItem") == 0)
	{
		/* flood check */
		for (i = 0 ; i < asp_dos_PktCountDosCnt ; i++)
		for (j = 0 ; j < asp_dos_PktCountDosCheckCnt ; j++)
		{
			websWrite(wp,	"\t\tdos.Enable_%s_%d.checked = false;\n" \
							, PktCountDosCheckList[j], i);
		}
		/* level Adjustable check */
		for (i = 0 ; i < asp_dos_AdjustableDosCnt ; i ++)
		{
			websWrite(wp,	"\t\tdos.Enable_%s.checked = false;\n" \
							, AdjustableList[i]);
		}
		/* packet inspect check */
		for (i = 0 ; i < asp_dos_PktInspectDosCheckCnt ; i ++)
		{
			websWrite(wp,	"\t\tdos.Enable_%s.checked = false;\n" \
							, PktInspectDosList[i]);
		}
	}
 	return 0;
}

void asp_setDos(webs_t wp, char_t *path, char_t *query)
{
	char tmpstr[64];
	int i,j;
	int ms=atoi(websGetVar(wp,"ms",""));
	char* PktInspectDosList[]			= {"TcpScan", "TcpSynWithData", "TcpLand", "UdpBomb", "UdpLand", "PingOfDeath", "IcmpSmurf", "IcmpLand", "IpSpoof", "TearDrop"};
	char* PktCountDosList[] 			= {"Whole System Flood", "Per-Source IP Flood"};
	char* PktCountDosCheckList[]		= {"SYN", "FIN", "UDP", "ICMP"};
	char* AdjustableList[]				= {"TcpUdpPortScan"};
	char* AdjustableLevelList[]		= {"Low", "High"};

	
	pRomeCfgParam->dosCfgParam[ms].enable = atoi(websGetVar(wp,"enable",""));

	pRomeCfgParam->dosCfgParam[ms].enableItem = 0;	// reset enable Item
	if (atoi(websGetVar(wp, "Enable_SYN_0","")))			{pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_SYNFLOOD;}
	if (atoi(websGetVar(wp, "Enable_FIN_0","")))			{pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_STEALTHFIN;}
	if (atoi(websGetVar(wp, "Enable_UDP_0","")))			{pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_UDPFLOOD;}
	if (atoi(websGetVar(wp, "Enable_ICMP_0","")))		{pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_ICMPFLOOD;}
	if (atoi(websGetVar(wp, "Enable_SYN_1","")))			{pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_SRC_SYNFLOOD;}
	if (atoi(websGetVar(wp, "Enable_FIN_1","")))			{pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_SRC_STEALTHFIN;}
	if (atoi(websGetVar(wp, "Enable_UDP_1","")))			{pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_SRC_UDPFLOOD;}
	if (atoi(websGetVar(wp, "Enable_ICMP_1","")))		{pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_SRC_ICMPFLOOD;}
	if (atoi(websGetVar(wp, "Enable_TcpScan","")))		{pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_SCAN;}
	if (atoi(websGetVar(wp, "Enable_TcpSynWithData","")))	{pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_SYNWITHDATA;}
	if (atoi(websGetVar(wp, "Enable_TcpLand","")))		{pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_TCPLAND;}
	if (atoi(websGetVar(wp, "Enable_UdpEchoChargen",""))){pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_ECHOCHARGEN;}
	if (atoi(websGetVar(wp, "Enable_UdpBomb","")))		{pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_UDPBOMB;}
	if (atoi(websGetVar(wp, "Enable_UdpLand","")))		{pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_UDPLAND;}
	if (atoi(websGetVar(wp, "Enable_PingOfDeath","")))	{pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_PINGOFDEATH;}
	if (atoi(websGetVar(wp, "Enable_IcmpSmurf","")))		{pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_SMURF;}
	if (atoi(websGetVar(wp, "Enable_IcmpLand","")))		{pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_ICMPLAND;}
	if (atoi(websGetVar(wp, "Enable_IpSpoof","")))		{pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_IPSPOOF;}
	if (atoi(websGetVar(wp, "Enable_TearDrop","")))		{pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_TEARDROP;}
	if (atoi(websGetVar(wp, "Enable_TcpUdpPortScan","")))	{pRomeCfgParam->dosCfgParam[ms].enableItem |= RTL8651_DOS_PORTSCAN;}
	

	if (pRomeCfgParam->dosCfgParam[ms].enableItem == 0)
	{
		pRomeCfgParam->dosCfgParam[ms].enable = 0;
	}

	pRomeCfgParam->dosCfgParam[ms].syn=atoi(websGetVar(wp,"Count_SYN_0",""));
	pRomeCfgParam->dosCfgParam[ms].fin=atoi(websGetVar(wp,"Count_FIN_0",""));	
	pRomeCfgParam->dosCfgParam[ms].udp=atoi(websGetVar(wp,"Count_UDP_0",""));
	pRomeCfgParam->dosCfgParam[ms].icmp=atoi(websGetVar(wp,"Count_ICMP_0",""));	
		
	pRomeCfgParam->dosCfgParam[ms].perSrc_syn=atoi(websGetVar(wp,"Count_SYN_1",""));
	pRomeCfgParam->dosCfgParam[ms].perSrc_fin=atoi(websGetVar(wp,"Count_FIN_1",""));
	pRomeCfgParam->dosCfgParam[ms].perSrc_udp=atoi(websGetVar(wp,"Count_UDP_1",""));
	pRomeCfgParam->dosCfgParam[ms].perSrc_icmp=atoi(websGetVar(wp,"Count_ICMP_1",""));

	rtl8651a_setDosThreshold(ms,RTL8651_DOS_SYN_THRESHOLD,pRomeCfgParam->dosCfgParam[ms].syn);
	rtl8651a_setDosThreshold(ms,RTL8651_DOS_FIN_THRESHOLD,pRomeCfgParam->dosCfgParam[ms].fin);
	rtl8651a_setDosThreshold(ms,RTL8651_DOS_UDP_THRESHOLD,pRomeCfgParam->dosCfgParam[ms].udp);
	rtl8651a_setDosThreshold(ms,RTL8651_DOS_ICMP_THRESHOLD,pRomeCfgParam->dosCfgParam[ms].icmp);

	rtl8651a_setPerSrcDosThreshold(ms, RTL8651_DOS_SYN_THRESHOLD, pRomeCfgParam->dosCfgParam[ms].perSrc_syn);
	rtl8651a_setPerSrcDosThreshold(ms, RTL8651_DOS_FIN_THRESHOLD, pRomeCfgParam->dosCfgParam[ms].perSrc_fin);
	rtl8651a_setPerSrcDosThreshold(ms, RTL8651_DOS_UDP_THRESHOLD, pRomeCfgParam->dosCfgParam[ms].perSrc_udp);
	rtl8651a_setPerSrcDosThreshold(ms, RTL8651_DOS_ICMP_THRESHOLD, pRomeCfgParam->dosCfgParam[ms].perSrc_icmp);

	{
		uint32 threshold;

		pRomeCfgParam->dosCfgParam[ms].portScan_level=atoi(websGetVar(wp,"Level_TcpUdpPortScan",""));
		switch (pRomeCfgParam->dosCfgParam[ms].portScan_level)
		{
			case 0:
					threshold = DOS_PORTSCAN_LOW_VAL;
					break;
			case 1:
					threshold = DOS_PORTSCAN_HIGH_VAL;
					break;
			default:
					pRomeCfgParam->dosCfgParam[ms].portScan_level = 0;
					threshold = DOS_PORTSCAN_LOW_VAL;
		}
		rtl8651_dosScanTrackingSetPara(ms, RTL8651_DOS_SCAN_MOINTOR_WIN_DEFAULT, threshold);
	}

	/* ignore dos check if packet from internal vlan */
	{
		pRomeCfgParam->dosCfgParam[ms].ignoreLanSideCheck=atoi(websGetVar(wp, "ignore_lan",""));
		for(i=0;i<MAX_PPPOE_SESSION;i++)
		{
			if (i != ms)
				pRomeCfgParam->dosCfgParam[i].ignoreLanSideCheck = pRomeCfgParam->dosCfgParam[ms].ignoreLanSideCheck;
		}
		if (pRomeCfgParam->dosCfgParam[ms].ignoreLanSideCheck)
		{
			rtl8651_addDosIgnoreType(RTL8651_DOS_IGNORETYPE_FROM_LAN);
		}else
		{
			rtl8651_delDosIgnoreType(RTL8651_DOS_IGNORETYPE_FROM_LAN);
		}
	}

	rtl8651a_setDosStatus(ms, ((pRomeCfgParam->dosCfgParam[ms].enable)?(pRomeCfgParam->dosCfgParam[ms].enableItem):0));
	
	cfgmgr_write(CFGMGR_TABID_DOS,(void*)(pRomeCfgParam->dosCfgParam), sizeof(struct dosCfgParam_s)*MAX_PPPOE_SESSION);
	cfgmgr_task();
	sprintf(tmpstr,"dos.asp?ms=%d",ms);
	websRedirect(wp,tmpstr);
}

int asp_dosProc(int ejid, webs_t wp, int argc, char_t **argv)
{

	int ms=asp_multiSession(wp,argc,argv);
	
	if (strcmp(argv[0],"SIPBLOCK_ENABLE")==0)
 	{
 		if (pRomeCfgParam->dosCfgParam[ms].sipblock_enable ==1) websWrite(wp,"checked");
 	}
	else if(strcmp(argv[0],"MS")==0) websWrite(wp,"%d",ms);
	else if(strcmp(argv[0],"SIPBLOCK_PRISONTIME")==0)
		websWrite(wp,"%d",pRomeCfgParam->dosCfgParam[ms].sipblock_PrisonTime);
 	return 0;
}


void asp_setDosProc(webs_t wp,  char_t *path, char_t *query)
{
	char tmpstr[64];
	int32 idx;
	int ms=atoi(websGetVar(wp,"ms",""));

	/* Check if "Free All Blocked IP" button been clicked or not */
	if (strcmp(websGetVar(wp,"blockAllBlockedIp",""), "FreeALL") == 0)
	{
		rtl8651_dosProc_blockSip_freeAll();	// free all blocked IP
		goto redirect;
	}

	pRomeCfgParam->dosCfgParam[ms].sipblock_enable=atoi(websGetVar(wp,"sipblock_enable",""));

	if (pRomeCfgParam->dosCfgParam[ms].sipblock_enable == 1)
	{	// only modify parameters when source-ip block is enabled
		pRomeCfgParam->dosCfgParam[ms].sipblock_PrisonTime=atoi(websGetVar(wp,"sipblock_prisontime",""));
		for (idx = 0; idx < MAX_PPPOE_SESSION ; idx ++)
		{	// all session share the same Prison Time
			if (idx == ms) continue;
			pRomeCfgParam->dosCfgParam[idx].sipblock_PrisonTime = pRomeCfgParam->dosCfgParam[ms].sipblock_PrisonTime;
		}
		rtl8651_dosProc_blockSip_setPrisonTime(pRomeCfgParam->dosCfgParam[ms].sipblock_PrisonTime);
	}

	rtl8651_dosProc_blockSip_enable(ms, pRomeCfgParam->dosCfgParam[ms].sipblock_enable);

	cfgmgr_write(CFGMGR_TABID_DOS,(void*)(pRomeCfgParam->dosCfgParam), sizeof(struct dosCfgParam_s)*MAX_PPPOE_SESSION);
	cfgmgr_task();
redirect:
	sprintf(tmpstr,"dos.asp?ms=%d",ms);
	websRedirect(wp,tmpstr);

}

/************************************************************
 *	
 *	ALG
 *	
 ************************************************************/
int asp_alg(int ejid, webs_t wp, int argc, char_t **argv)
{

	int algargc=17,i;
	#define ALG_TYPE_CLIENT	 1
	#define ALG_TYPE_CLIENT_SERVER 2	
	#define ALG_TYPE_CLIENT_SERVER_WITH_IP 3
	uint32 algtype[]={ALG_TYPE_CLIENT_SERVER_WITH_IP,ALG_TYPE_CLIENT,ALG_TYPE_CLIENT
	,ALG_TYPE_CLIENT_SERVER_WITH_IP,ALG_TYPE_CLIENT_SERVER_WITH_IP
	,ALG_TYPE_CLIENT_SERVER_WITH_IP,ALG_TYPE_CLIENT_SERVER_WITH_IP,ALG_TYPE_CLIENT_SERVER_WITH_IP
	,ALG_TYPE_CLIENT_SERVER,ALG_TYPE_CLIENT_SERVER,ALG_TYPE_CLIENT_SERVER
	,ALG_TYPE_CLIENT,ALG_TYPE_CLIENT,ALG_TYPE_CLIENT,ALG_TYPE_CLIENT,ALG_TYPE_CLIENT,ALG_TYPE_CLIENT};	
	char *algstr[]={"PPTP","IPSec","L2TP","FTP","Net Meeting","DirectX 7","SIP","ICUII","ICQ / AOL Instant Messenger","Yahoo Messenger","mIRC","VDOlive","Quake","Counter Strike / Half Life","Blizzard Battlenet (StarCraft, DiabloII)","RealAudio","CUseeMe"};
	char *algargv[]={"PPTP","IPSEC","L2TP","FTP","NETMEETING","DX7","SIP","ICUII","ICQ","YAHOOMSG","IRC","VDOLIVE","QUAKE","CS","BATTLENET","REALAUDIO","CUSEEME"};
	uint32 algmask[]={RTL8651_ALG_PPTP,RTL8651_ALG_IPSEC,RTL8651_ALG_L2TP
	,RTL8651_ALG_FTP,RTL8651_ALG_NETMEETING,RTL8651_ALG_DX7,RTL8651_ALG_SIP,RTL8651_ALG_ICUII
	,RTL8651_ALG_ICQ,RTL8651_ALG_YAHOOMSG,RTL8651_ALG_IRC
	,RTL8651_ALG_VDOLIVE,RTL8651_ALG_QUAKE,RTL8651_ALG_CS,RTL8651_ALG_BATTLENET,RTL8651_ALG_REALAUDIO,RTL8651_ALG_CUSEEME};
	uint32 algidx[]={RTL8651_ALG_PPTP_IDX,RTL8651_ALG_IPSEC_IDX,RTL8651_ALG_L2TP_IDX
	,RTL8651_ALG_FTP_IDX,RTL8651_ALG_NETMEETING_IDX,RTL8651_ALG_DX7_IDX,RTL8651_ALG_SIP_IDX,RTL8651_ALG_ICUII_IDX
	,RTL8651_ALG_ICQ_IDX,RTL8651_ALG_YAHOOMSG_IDX,RTL8651_ALG_IRC_IDX
	,RTL8651_ALG_VDOLIVE_IDX,RTL8651_ALG_QUAKE_IDX,RTL8651_ALG_CS_IDX,RTL8651_ALG_BATTLENET_IDX,RTL8651_ALG_REALAUDIO_IDX,RTL8651_ALG_CUSEEME_IDX};	
	int ms=asp_multiSession(wp,argc,argv);

	if(strcmp(argv[0],"web")==0) 
	{		
		uint32 algc=pRomeCfgParam->algCfgParam[ms].algList;
		websWrite(wp,"<input type=hidden name=ms value=%d>\n",ms);
		for(i=0;i<algargc;i++)	
		{
			websWrite(wp,"<tr>\n" \
			"<td bgColor=#ddeeff>%s</td>\n" \
			"<td bgColor=#ddeeff >\n" \
			"<input type=checkbox name=%s value=1 %s>Client",algstr[i],algargv[i],(algc&algmask[i])?"checked":"");

			if(algtype[i]==ALG_TYPE_CLIENT_SERVER)
				websWrite(wp,"/Server ");

			if(algtype[i]==ALG_TYPE_CLIENT_SERVER_WITH_IP)
				{
					websWrite(wp,"/Server(IP:\n" \
					"<input type=text name=%ssip size=15 maxlength=15 value=\"%u.%u.%u.%u\">)\n"
					,algargv[i],NIPQUAD(pRomeCfgParam->algCfgParam[ms].localIp[algidx[i]]));
				}

			websWrite(wp,"</td></tr>\n");
			
		}
	}
	return 0;
}

void asp_setAlg(webs_t wp, char_t *path, char_t *query)
{
	uint32 algc=0;
	int algargc=17,i;
	char tmpstr[32];

	char *algargv[]={"PPTP","IPSEC","L2TP","FTP","NETMEETING","DX7","SIP","ICUII","ICQ","YAHOOMSG","IRC","VDOLIVE","QUAKE","CS","BATTLENET","REALAUDIO","CUSEEME"};
	uint32 algmask[]={RTL8651_ALG_PPTP,RTL8651_ALG_IPSEC,RTL8651_ALG_L2TP
	,RTL8651_ALG_FTP,RTL8651_ALG_NETMEETING,RTL8651_ALG_DX7,RTL8651_ALG_SIP,RTL8651_ALG_ICUII
	,RTL8651_ALG_ICQ,RTL8651_ALG_YAHOOMSG,RTL8651_ALG_IRC
	,RTL8651_ALG_VDOLIVE,RTL8651_ALG_QUAKE,RTL8651_ALG_CS,RTL8651_ALG_BATTLENET,RTL8651_ALG_REALAUDIO,RTL8651_ALG_CUSEEME};
	uint32 algidx[]={RTL8651_ALG_PPTP_IDX,RTL8651_ALG_IPSEC_IDX,RTL8651_ALG_L2TP_IDX
	,RTL8651_ALG_FTP_IDX,RTL8651_ALG_NETMEETING_IDX,RTL8651_ALG_DX7_IDX,RTL8651_ALG_SIP_IDX,RTL8651_ALG_ICUII_IDX
	,RTL8651_ALG_ICQ_IDX,RTL8651_ALG_YAHOOMSG_IDX,RTL8651_ALG_IRC_IDX
	,RTL8651_ALG_VDOLIVE_IDX,RTL8651_ALG_QUAKE_IDX,RTL8651_ALG_CS_IDX,RTL8651_ALG_BATTLENET_IDX,RTL8651_ALG_REALAUDIO_IDX,RTL8651_ALG_CUSEEME_IDX};	
	int ms=atoi(websGetVar(wp,"ms",""));

	for(i=0;i<algargc;i++)
	{
		sprintf(tmpstr,"%s",algargv[i]);
		algc|=(atoi(websGetVar(wp,tmpstr,""))==1)?algmask[i]:0;
	}
	pRomeCfgParam->algCfgParam[ms].algList=algc;

	for(i=0;i<algargc;i++)
	{
		ipaddr_t ip=0,ip2=0;
		sprintf(tmpstr,"%ssip",algargv[i]);
		parse_ipRange(websGetVar(wp,tmpstr,""), &ip, &ip2);
		pRomeCfgParam->algCfgParam[ms].localIp[algidx[i]]=ip;
		//printf("%s ip=%u.%u.%u.%u\n",algargv[i],NIPQUAD(ip));
	}
	
	rtl8651a_setAlgStatus(ms,algc,pRomeCfgParam->algCfgParam[ms].localIp);
	cfgmgr_write(CFGMGR_TABID_ALG,(void*)pRomeCfgParam->algCfgParam,sizeof(algCfgParam_t)*MAX_PPPOE_SESSION);
	cfgmgr_task();

	sprintf(tmpstr,"alg.asp?ms=%d",ms);
	websRedirect(wp, tmpstr);
}
/* end rome_asp.c */



/************************************************************
 *	
 *	Wireless LAN Basic
 *	
 ************************************************************/

int asp_wlanBasic(int ejid, webs_t wp, int argc, char_t **argv)
{
	char temp[8];
	char cmdBuf[80];
	int i;

	//No WLAN card probed
	if(strcmp(argv[0],"exist0")==0){
		websWrite(wp,"%d",totalWlanCards?1:0);
		return 0;
	}
	if(strcmp(argv[0],"exist1")==0){
		websWrite(wp,"%d",totalWlanCards>1?1:0);
		return 0;
	}

	/* read cfg from cfgmgr */

	if (cfgmgr_read(CFGMGR_TABID_WLAN,(void*)(pRomeCfgParam->wlanCfgParam), sizeof(struct wlanCfgParam_s)*MAX_WLAN_CARDS)!=0)
	{
		printf("asp_wlanBasic: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}

	
	for(i=0;i<totalWlanCards;i++){

		sprintf(cmdBuf, "name%d",i);
		if(strcmp(argv[0],cmdBuf)==0){
			websWrite(wp,"%s",pRomeCfgParam->wlanCfgParam[i].aliasName);
			break;
		}
		sprintf(cmdBuf, "ssid%d",i);
		if(strcmp(argv[0],cmdBuf)==0){
			websWrite(wp,"%s",pRomeCfgParam->wlanCfgParam[i].ssid);
			break;
		}
		sprintf(cmdBuf, "enable%d",i);
		if(strcmp(argv[0],cmdBuf)==0){
			websWrite(wp,"%s",pRomeCfgParam->wlanCfgParam[i].enable?"checked":"");
			break;
		}
		sprintf(cmdBuf, "active%d",i);
		if(strcmp(argv[0],cmdBuf)==0){
			if(totalWlanCards){
				websWrite(wp,"%s",pRomeCfgParam->wlanCfgParam[i].enable?"Enabled":"Not Enabled");
			}else
				websWrite(wp,"Not installed");
			break;
		}
		sprintf(cmdBuf,"ch%d_%d",i,pRomeCfgParam->wlanCfgParam[i].channel);
		if(strcmp(argv[0],cmdBuf)==0){
			websWrite(wp,"selected");
			break;
		}
		sprintf(cmdBuf,"channel%d",i,pRomeCfgParam->wlanCfgParam[i].channel);
		if(strcmp(argv[0],cmdBuf)==0){
			websWrite(wp,"%d",1);
			break;
		}		
	}
	return 0;
}


void asp_setWlanBasic(webs_t wp, char_t *path, char_t *query)
{
	char cmdBuf[80];
	int i;

	for(i=0;i<totalWlanCards;i++){
		sprintf(cmdBuf, "name%d",i);
		memcpy(pRomeCfgParam->wlanCfgParam[i].aliasName,websGetVar(wp,cmdBuf,""),strlen(websGetVar(wp,cmdBuf,""))+1);
		sprintf(cmdBuf, "ssid%d",i);
		memcpy(pRomeCfgParam->wlanCfgParam[i].ssid,websGetVar(wp,cmdBuf,""),strlen(websGetVar(wp,cmdBuf,""))+1);
		sprintf(cmdBuf, "enable%d",i);
		pRomeCfgParam->wlanCfgParam[i].enable=atoi(websGetVar(wp,cmdBuf,""));
		sprintf(cmdBuf, "channel%d",i);
		pRomeCfgParam->wlanCfgParam[i].channel=atoi(websGetVar(wp,cmdBuf,""));
	}

	cfgmgr_write(CFGMGR_TABID_WLAN,(void*)pRomeCfgParam->wlanCfgParam,sizeof(wlanCfgParam_t)*MAX_WLAN_CARDS);

	cfgmgr_task();
	websRedirect(wp, "wireless_basic.asp");

	for(i=0;i<totalWlanCards;i++){
		sprintf(cmdBuf, "ifconfig wlan%d down",i); //Bring down the interface first.
		printf("%s\n", cmdBuf);system(cmdBuf);;
		if(pRomeCfgParam->wlanCfgParam[i].enable == 1){
			if(0==wlan_setup(i)){
				sprintf(cmdBuf, "ifconfig wlan%d up",i); //bring up interface.
				printf("%s\n", cmdBuf);system(cmdBuf);;
				wds_up(i);
			}
		}
	}

}

/************************************************************
 *	
 *	Wireless LAN Advance
 *	
 ************************************************************/

int asp_wlanAdvance(int ejid, webs_t wp, int argc, char_t **argv)
{
	char temp[16];
	char cmdBuf[80];
	int i;
	//No WLAN card probed
	if(totalWlanCards==0){
		if(strcmp(argv[0],"alias0")==0){
			websWrite(wp,"Not installed");
			return 0;
		}
		if(strcmp(argv[0],"alias1")==0){
			websWrite(wp,"Not installed");
			return 0;
		}
		return 0;
	}

	//Only one card probed, always disabled second one.
	if(totalWlanCards==1){
		if(strcmp(argv[0],"alias1")==0){
			websWrite(wp,"Not installed");
			return 0;
		}
	}	


		/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_WLAN,(void*)pRomeCfgParam->wlanCfgParam, sizeof(struct wlanCfgParam_s)*MAX_WLAN_CARDS)!=0)
	{
		printf("asp_wlanAdvance: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}

	for(i=0;i<totalWlanCards;i++){		
		
		sprintf(cmdBuf, "alias%d",i);
		if(strcmp(argv[0],cmdBuf)==0){  
			websWrite(wp,"%s",pRomeCfgParam->wlanCfgParam[i].aliasName);
			break;
		}
		sprintf(cmdBuf, "frag%d",i);
		if(strcmp(argv[0],cmdBuf)==0){  
			websWrite(wp,"%d",pRomeCfgParam->wlanCfgParam[i].frag);
			break;
		}
		sprintf(cmdBuf, "rts%d",i);
		if(strcmp(argv[0],cmdBuf)==0){
			websWrite(wp,"%d",pRomeCfgParam->wlanCfgParam[i].rts);
			break;
		}
		sprintf(cmdBuf, "beacon%d",i);
		if(strcmp(argv[0],cmdBuf)==0){
			websWrite(wp,"%d",pRomeCfgParam->wlanCfgParam[i].beacon);
			break;
		}
		sprintf(cmdBuf,"dataRateMode%d_%d",i,pRomeCfgParam->wlanCfgParam[i].dataRateMode);
		if(strcmp(argv[0],cmdBuf)==0){  
			websWrite(wp,"selected");
			break;
		}
		sprintf(cmdBuf,"dataRate%d_%d",i,pRomeCfgParam->wlanCfgParam[i].dataRate);
		if(strcmp(argv[0],cmdBuf)==0){
			websWrite(wp,"selected");
			break;
		}
		sprintf(cmdBuf,"preamble%d_%d",i,pRomeCfgParam->wlanCfgParam[i].preambleType);
		if(strcmp(argv[0],cmdBuf)==0){  
			websWrite(wp,"checked");	
			break;
		}
		sprintf(cmdBuf,"txPower%d_%d",i,pRomeCfgParam->wlanCfgParam[i].txPower);
		if(strcmp(argv[0],cmdBuf)==0){  
			websWrite(wp,"selected");	
			break;
		}		
		sprintf(cmdBuf,"broadcastSSID%d",i);
		if(strcmp(argv[0],cmdBuf)==0){
			websWrite(wp,"%s",pRomeCfgParam->wlanCfgParam[i].broadcastSSID?"checked":"");
			break;
		}
		sprintf(cmdBuf,"iapp%d",i);
		if(strcmp(argv[0],cmdBuf)==0){
			websWrite(wp,"%s",pRomeCfgParam->wlanCfgParam[i].iapp?"checked":"");
			break;
		}
	}
	return 0;

}	
void asp_setWlanAdvance(webs_t wp, char_t *path, char_t *query)
{
	uint16 i,basicRateSet[13]={0,4095,2047,1023,511,255,127,63,31,15,7,3,1};
	uint16 opRateSet[13]={0/*Auto*/,4095,2047,1023,511,255,127,63,31,15,7,3,1};
	int cckTxPower[]={0,3,7,9,11};
	int ofdmTxPower[]={0,5,9,13,17};
	int x;
	char cmdBuf[40]={0};
	for(i=0;i<totalWlanCards;i++){
		sprintf(cmdBuf, "dataRateMode%d",i);
		pRomeCfgParam->wlanCfgParam[i].dataRateMode=
			atoi(websGetVar(wp,cmdBuf,""));

		sprintf(cmdBuf, "dataRate%d",i);
		pRomeCfgParam->wlanCfgParam[i].dataRate=
			atoi(websGetVar(wp,cmdBuf,""));

		sprintf(cmdBuf, "preamble%d",i);
		pRomeCfgParam->wlanCfgParam[i].preambleType=
			atoi(websGetVar(wp,cmdBuf,""));

		sprintf(cmdBuf, "broadcastSSID%d",i);
		pRomeCfgParam->wlanCfgParam[i].broadcastSSID=
			atoi(websGetVar(wp,cmdBuf,""));

		sprintf(cmdBuf, "iapp%d",i);
		pRomeCfgParam->wlanCfgParam[i].iapp=
			atoi(websGetVar(wp,cmdBuf,""));	

		sprintf(cmdBuf, "frag%d",i);
		pRomeCfgParam->wlanCfgParam[i].frag=
			(uint32)atoi(websGetVar(wp,cmdBuf,""));

		if(pRomeCfgParam->wlanCfgParam[i].frag>2346)
			pRomeCfgParam->wlanCfgParam[i].frag=2346;
		else if(pRomeCfgParam->wlanCfgParam[i].frag<256)
			pRomeCfgParam->wlanCfgParam[i].frag=256;

		sprintf(cmdBuf, "rts%d",i);
		pRomeCfgParam->wlanCfgParam[i].rts=
			(uint32)atoi(websGetVar(wp,cmdBuf,""));
		if(pRomeCfgParam->wlanCfgParam[i].rts>2347)
			pRomeCfgParam->wlanCfgParam[i].rts=2347;

		sprintf(cmdBuf, "beacon%d",i);
		pRomeCfgParam->wlanCfgParam[i].beacon=
			(uint32)atoi(websGetVar(wp,cmdBuf,""));
		if(pRomeCfgParam->wlanCfgParam[i].beacon<20)
			pRomeCfgParam->wlanCfgParam[i].beacon=20;			
		else if(pRomeCfgParam->wlanCfgParam[i].beacon>1024)
			pRomeCfgParam->wlanCfgParam[i].beacon=1024;

		sprintf(cmdBuf, "txPower%d",i);
		pRomeCfgParam->wlanCfgParam[i].txPower=
			(uint32)atoi(websGetVar(wp,cmdBuf,""));


		

#ifdef CONFIG_RTL8185
		sprintf(cmdBuf, "ifconfig wlan%d down",i); //Bring down the interface first.
		printf("%s\n", cmdBuf);system(cmdBuf);;

		sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib fragthres=%d", i,pRomeCfgParam->wlanCfgParam[i].frag);
		printf("%s\n", cmdBuf);system(cmdBuf);;
		sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib rtsthres=%d", i,pRomeCfgParam->wlanCfgParam[i].rts);
		printf("%s\n", cmdBuf);system(cmdBuf);;
		sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib bcnint=%d",i, pRomeCfgParam->wlanCfgParam[i].beacon);
		printf("%s\n", cmdBuf);system(cmdBuf);;
		uint16 band[4]={3 /*b+g*/,2/*g only*/,4/*a only */,1/*b only*/};	
		sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib band=%d",i, band[pRomeCfgParam->wlanCfgParam[i].dataRateMode]);
		printf("%s\n", cmdBuf);system(cmdBuf);

		for(x=0;x<14;x++){
			sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib TxPowerCCK=%d,%d", 
				i,x,cckTxPower[pRomeCfgParam->wlanCfgParam[i].txPower]);
			printf("%s\n", cmdBuf);system(cmdBuf);
			sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib TxPowerOFDM=%d,%d", 
				i,x,ofdmTxPower[pRomeCfgParam->wlanCfgParam[i].txPower]);
			printf("%s\n", cmdBuf);system(cmdBuf);
		}


		if(pRomeCfgParam->wlanCfgParam[i].dataRate!=0){
			sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib oprates=%d", i,opRateSet[pRomeCfgParam->wlanCfgParam[i].dataRate]);
			printf("%s\n", cmdBuf);system(cmdBuf);;
			//Don't set basic Rate Set, Let driver take default.
		}
		sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib hiddenAP=%d", i,1-pRomeCfgParam->wlanCfgParam[i].broadcastSSID);
		printf("%s\n", cmdBuf);system(cmdBuf);;
		if(pRomeCfgParam->wlanCfgParam[i].enable == 1){
			sprintf(cmdBuf, "ifconfig wlan%d up promisc",i); //bring up interface.
			printf("%s\n", cmdBuf);system(cmdBuf);;
		}
#endif
	}
		cfgmgr_write(CFGMGR_TABID_WLAN,(void*)pRomeCfgParam->wlanCfgParam,sizeof(wlanCfgParam_t)*MAX_WLAN_CARDS);
		cfgmgr_task();
		websRedirect(wp, "wireless_advance.asp");	
}	

/************************************************************
 *	
 *	Wireless LAN Security
 *	
 ************************************************************/

int asp_wlanSecurity(int ejid, webs_t wp, int argc, char_t **argv)
{
	char temp[16];
	char cmdBuf[40];
	int32 crypt=0, auth=0;
	int i;
	//No WLAN card probed
	if(totalWlanCards==0){
		if(strcmp(argv[0],"alias0")==0){
			websWrite(wp,"Not installed");
			return 0;
		}
		if(strcmp(argv[0],"alias1")==0){
			websWrite(wp,"Not installed");
			return 0;
		}
		return 0;
	}

	//Only one card probed, always disabled second one.
	if(totalWlanCards==1){
		if(strcmp(argv[0],"alias1")==0){
			websWrite(wp,"Not installed");
			return 0;
		}
	}		

	if (cfgmgr_read(CFGMGR_TABID_WLAN,(void*)(pRomeCfgParam->wlanCfgParam), sizeof(struct wlanCfgParam_s)*MAX_WLAN_CARDS)!=0)
	{
		printf("asp_wlanSecurity: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}
	
	for(i=0;i<totalWlanCards;i++){
		/* read cfg from cfgmgr */
		sprintf(cmdBuf, "alias%d",i); 
		if(strcmp(argv[0],cmdBuf)==0){
			websWrite(wp,"%s",pRomeCfgParam->wlanCfgParam[i].aliasName);
			break;
		}
		sprintf(cmdBuf, "cryptType%d",i); 
		if(strcmp(argv[0],cmdBuf)==0){  
			crypt=pRomeCfgParam->wlanCfgParam[i].cryptType;
			websWrite(wp,"%d",crypt);
			break;
		}
		sprintf(cmdBuf, "authType%d",i); 		
		if(strcmp(argv[0],cmdBuf)==0)  {
			auth=pRomeCfgParam->wlanCfgParam[i].authType;
			websWrite(wp,"%d",auth);
			break;
		}

		//Key field both for WEP and WPA
		sprintf(cmdBuf, "key%d",i); 		
		if(strcmp(argv[0],cmdBuf)==0)
		{
			if(pRomeCfgParam->wlanCfgParam[i].cryptType)
				websWrite(wp,"%s",pRomeCfgParam->wlanCfgParam[i].key);
			break;
		}
		sprintf(cmdBuf, "keyType%d",i);
		if(strcmp(argv[0],cmdBuf)==0)
		{
			if(pRomeCfgParam->wlanCfgParam[i].cryptType){
				int32 keyLen=strlen(pRomeCfgParam->wlanCfgParam[i].key);
				if(keyLen<=0){
					websWrite(wp,"(Empty)");
				}else{
					switch((pRomeCfgParam->wlanCfgParam[i].cryptType)){
						case CRYPT_TKIP:
						case CRYPT_AES:
							if(keyLen<8||keyLen>64)
								websWrite(wp,"<font color=red>Key is invalid(%d digits). Should be 64 hex digits or 8~63 char passphrase</font>", keyLen);
							else if(keyLen==64)
								websWrite(wp,"Key accepted(64 hex digits)");
							else
								websWrite(wp,"Passphrase accepted");
							break;
						case CRYPT_WEP64:
							if(keyLen==10)
								websWrite(wp,"Key accepted(10 hex digits)");
							else
								websWrite(wp,"<font color=red>Key is invalid. Should be 10 hex digits</font>");
							break;
						case CRYPT_WEP128:							
							if(keyLen==26)
								websWrite(wp,"Key accepted(26 hex digits)");
							else
								websWrite(wp,"<font color=red>Key is invalid. Should be 26 hex digits</font>");
							break;

					}
				}
			}
			break;
		}
		sprintf(cmdBuf, "port%d",i); 
		if(strcmp(argv[0],cmdBuf)==0) { 
			websWrite(wp,"%d",pRomeCfgParam->wlanCfgParam[i].authRadiusPort);
			break;
		}
		sprintf(cmdBuf, "ipaddr%d",i); 
		if(strcmp(argv[0],cmdBuf)==0){
			websWrite(wp,"%u.%u.%u.%u",NIPQUAD(pRomeCfgParam->wlanCfgParam[i].authRadiusIp));
			break;
		}
		sprintf(cmdBuf, "passwd%d",i); 
		if(strcmp(argv[0],cmdBuf)==0){  
			websWrite(wp,"%s",pRomeCfgParam->wlanCfgParam[i].authRadiusPasswd);
			break;
		}
	}
	if(i==totalWlanCards){
		sprintf(cmdBuf, "cryptType%d",i); 
		if(strcmp(argv[0],cmdBuf)==0){  
			websWrite(wp,"0");
			return 0;
		}
		sprintf(cmdBuf, "authType%d",i); 		
		if(strcmp(argv[0],cmdBuf)==0)  {
			websWrite(wp,"0");
			return 0;
		}
	}	

	return 0;
}	

int32 chkKey(int8 *key, int32 cryptType){
	int i;
	int keylen=strlen(key);
	if(CRYPT_TKIP==cryptType||CRYPT_AES==cryptType){
		//can be passphrase or a 8~64 passphrase
		if(keylen>64||keylen<8)
			return -1;
		if(keylen==64){
			for(i=0;i<64;i++){
				if(isxdigit(key[i]))
					continue;
				else 
					return -1;
			}
			return keylen; //accepted.
		}
		return keylen;//accepted

	}else if(CRYPT_WEP64==cryptType){
		//can only be 10 hex digit
		if(keylen!=10)
			return -1;
		for(i=0;i<10;i++){
			if(isxdigit(key[i]))
				continue;
			else 
				return -1;
		}
		return keylen; //accepted.
	}else if(CRYPT_WEP128==cryptType){
		//can only be 26 hex digit
		if(keylen!=26)
			return -1;
		for(i=0;i<26;i++){
			if(isxdigit(key[i]))
				continue;
			else 
				return -1;
		}
		return keylen; //accepted.
	}
	return -1;
}

void asp_setWlanSecurity(webs_t wp, char_t *path, char_t *query)
{
	char cmdBuf[64]={0};
	int8 KEY[65];
	uint8 wepkeyid=1;
	int32 key_idx;	
    uint32 tempip,i;
	int8 cfgFilePath[]="/var/8185wpa.conf";
	FILE *wpaCfgFile;
	wpaCfgFile=fopen(cfgFilePath,"w+"); //erase old file and create new one.;

	//auth daemon supports only one instance now....:( 
	for(i=0;i<totalWlanCards;i++){	
#ifdef CONFIG_RTL8185
		int32 cryptValue[]={0/*disabled*/,1/*wep64*/,5/*wep128*/,2/*tkip*/,4/*AES*/};
		int cckTxPower[]={0,3,7,9,11};
		int ofdmTxPower[]={0,5,9,13,17};
		uint16 opRateSet[13]={0/*Auto*/,4095,2047,1023,511,255,127,63,31,15,7,3,1};
		int16 authType[5/*encryption selection*/][3/*auth selection*/]={
			/*No encrypt*/{AUTH_NONE,AUTH_8021x,0},
			/*WEP64*/{AUTH_NONE,AUTH_KEY,AUTH_8021x},
			/*WEP128*/{AUTH_NONE,AUTH_KEY,AUTH_8021x},
			/*WPA-TKIP*/{AUTH_NONE,AUTH_8021x,0},
			/*WPA-AES*/{AUTH_NONE,AUTH_8021x,0}
		};
		uint16 band[4]={3 /*b+g*/,2/*g only*/,4/*a only */,1/*b only*/};	
		macaddr_t macStr;
		int32 value, keylen;
#endif
		sprintf(cmdBuf, "authType%d",i); 
		pRomeCfgParam->wlanCfgParam[i].authType=
			atoi(websGetVar(wp,cmdBuf,""));
		sprintf(cmdBuf, "cryptType%d",i); 
		pRomeCfgParam->wlanCfgParam[i].cryptType=
			atoi(websGetVar(wp,cmdBuf,""));

		printf("Crypt: %d, Auth: %d\n", 
			pRomeCfgParam->wlanCfgParam[i].cryptType,
			pRomeCfgParam->wlanCfgParam[i].authType);
			
		sprintf(cmdBuf, "ifconfig wlan%d down", i);
		printf("%s\n", cmdBuf);system(cmdBuf);;

#ifdef CONFIG_RTL8185
		if(wpaCfgFile){
			sprintf(cmdBuf, "ssid = \"%s\"\n",pRomeCfgParam->wlanCfgParam[i].ssid);printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
		}
		/* WLAN encryption setting */

		sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib encmode=%d", i,cryptValue[pRomeCfgParam->wlanCfgParam[i].cryptType]);
		printf("%s\n", cmdBuf);system(cmdBuf);
		switch(pRomeCfgParam->wlanCfgParam[i].cryptType){
			case CRYPT_WEP128:
			case CRYPT_WEP64:	
				sprintf(cmdBuf, "key%d",i); 
				memcpy(KEY, websGetVar(wp,cmdBuf,""),strlen(websGetVar(wp,cmdBuf,""))+1);
				if(-1==chkKey(KEY,pRomeCfgParam->wlanCfgParam[i].cryptType))
					break; //invalid key....don't apply..
				strcpy(pRomeCfgParam->wlanCfgParam[i].key,KEY);
				for(key_idx=0;key_idx<4;key_idx++){
					sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib wepkey%d=%s", i,key_idx+1,pRomeCfgParam->wlanCfgParam[i].key);
					printf("%s\n", cmdBuf);system(cmdBuf);
				}
				sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib wepdkeyid=0",i);
				printf("%s\n", cmdBuf);system(cmdBuf);
				if(wpaCfgFile){
					sprintf(cmdBuf, "encryption = 1\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
					if(CRYPT_WEP64==pRomeCfgParam->wlanCfgParam[i].cryptType){
						sprintf(cmdBuf, "wepKey = 1\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
					}else{
						sprintf(cmdBuf, "wepKey = 2\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
					}
				}

				switch(pRomeCfgParam->wlanCfgParam[i].authType){
					case AUTH_KEY:
						sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib 802_1x=0",i);
						printf("%s\n", cmdBuf);system(cmdBuf);
						sprintf(cmdBuf, "key%d",i); 
						memcpy(KEY, websGetVar(wp,cmdBuf,""),strlen(websGetVar(wp,cmdBuf,""))+1);
						keylen=chkKey(KEY,pRomeCfgParam->wlanCfgParam[i].cryptType);
						if(keylen==-1)
							break; //invalid key....don't apply..
						strcpy(pRomeCfgParam->wlanCfgParam[i].key,KEY);
						value=authType[pRomeCfgParam->wlanCfgParam[i].cryptType][pRomeCfgParam->wlanCfgParam[i].authType];
						sprintf(cmdBuf, "busybox iwpriv  wlan%d set_mib authtype=%d",i,value);	printf("%s\n", cmdBuf);system(cmdBuf);
						break;
					case AUTH_8021x:
						sprintf(cmdBuf, "port%d",i); 
						pRomeCfgParam->wlanCfgParam[i].authRadiusPort=atoi(websGetVar(wp,cmdBuf,""));

						sprintf(cmdBuf, "ipaddr%d",i); 
						parse_ipRange(websGetVar(wp,cmdBuf,""),&pRomeCfgParam->wlanCfgParam[i].authRadiusIp,&tempip);

						sprintf(cmdBuf, "passwd%d",i); 
						memcpy(pRomeCfgParam->wlanCfgParam[i].authRadiusPasswd,websGetVar(wp,cmdBuf,""),strlen(websGetVar(wp,cmdBuf,""))+1);
						sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib 802_1x=1",i);
						printf("%s\n", cmdBuf);system(cmdBuf);

						sprintf(cmdBuf, "enable1x = 1\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);					
						sprintf(cmdBuf, "authentication = 1\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);//use RADIUS
						sprintf(cmdBuf, "rsPort = %d\n",pRomeCfgParam->wlanCfgParam[i].authRadiusPort);printf("%s",cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
						sprintf(cmdBuf, "rsIP = %u.%u.%u.%u\n",NIPQUAD(pRomeCfgParam->wlanCfgParam[i].authRadiusIp));printf("%s",cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
						sprintf(cmdBuf, "rsPassword = \"%s\"\n",pRomeCfgParam->wlanCfgParam[i].authRadiusPasswd);printf("%s",cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
						sprintf(cmdBuf, "rsMaxReq = 3\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
						sprintf(cmdBuf, "rsAWhile = 5\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
						break;
					case AUTH_NONE:
						sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib 802_1x=0",i);
						printf("%s\n", cmdBuf);system(cmdBuf);
						sprintf(cmdBuf, "busybox iwpriv  wlan%d set_mib authtype=0",i,value);	printf("%s\n", cmdBuf);system(cmdBuf);
						break;
				}
				break;
			case CRYPT_TKIP:
			case CRYPT_AES:	
				if(!wpaCfgFile)
					break;
				sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib 802_1x=1",i);
				printf("%s\n", cmdBuf);system(cmdBuf);

				sprintf(cmdBuf, "encryption = 2\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
				if(CRYPT_TKIP==pRomeCfgParam->wlanCfgParam[i].cryptType){
					sprintf(cmdBuf, "unicastCipher = 1\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);					
				}else{
					sprintf(cmdBuf, "unicastCipher = 2\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);					
				}
				sprintf(cmdBuf, "enableMacAuth = 0\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);	
				sprintf(cmdBuf, "supportNonWpaClient = 0\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);	
				sprintf(cmdBuf, "groupRekeyTime = 86400\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
				
				switch(pRomeCfgParam->wlanCfgParam[i].authType){
					case AUTH_8021x://WPA enterprise
						sprintf(cmdBuf, "port%d",i); 
						pRomeCfgParam->wlanCfgParam[i].authRadiusPort=atoi(websGetVar(wp,cmdBuf,""));

						sprintf(cmdBuf, "ipaddr%d",i); 
						parse_ipRange(websGetVar(wp,cmdBuf,""),&pRomeCfgParam->wlanCfgParam[i].authRadiusIp,&tempip);

						sprintf(cmdBuf, "passwd%d",i); 
						memcpy(pRomeCfgParam->wlanCfgParam[i].authRadiusPasswd,websGetVar(wp,cmdBuf,""),strlen(websGetVar(wp,cmdBuf,""))+1);

						sprintf(cmdBuf, "enable1x = 1\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);					
						sprintf(cmdBuf, "authentication = 1\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);//use RADIUS
						sprintf(cmdBuf, "rsPort = %d\n",pRomeCfgParam->wlanCfgParam[i].authRadiusPort);printf("%s",cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
						sprintf(cmdBuf, "rsIP = %u.%u.%u.%u\n",NIPQUAD(pRomeCfgParam->wlanCfgParam[i].authRadiusIp));printf("%s",cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
						sprintf(cmdBuf, "rsPassword = \"%s\"\n",pRomeCfgParam->wlanCfgParam[i].authRadiusPasswd);printf("%s",cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
						sprintf(cmdBuf, "rsMaxReq = 3\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
						sprintf(cmdBuf, "rsAWhile = 5\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
					
						break;
					case AUTH_NONE: //WPA-PSK
						sprintf(cmdBuf, "enable1x = 0\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);					
						sprintf(cmdBuf, "key%d",i); 
						memcpy(KEY, websGetVar(wp,cmdBuf,""),strlen(websGetVar(wp,cmdBuf,""))+1);
						keylen=chkKey(KEY,pRomeCfgParam->wlanCfgParam[i].cryptType);
						if(keylen==-1)
							break; //invalid key....don't apply..
						printf("KEY=%s\n", KEY);
						strcpy(pRomeCfgParam->wlanCfgParam[i].key,KEY);
						sprintf(cmdBuf, "authentication = 2\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
						if(keylen!=64){
							sprintf(cmdBuf, "usePassphrase = 1\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
						}else{
							sprintf(cmdBuf, "usePassphrase = 0\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
						}
						sprintf(cmdBuf, "psk = \"%s\"\n",KEY);printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);							
						sprintf(cmdBuf, "busybox iwpriv  wlan%d set_mib authtype=0",i);	printf("%s\n", cmdBuf);system(cmdBuf);
						break;
				}
				break;
			case CRYPT_NONE:
				//no encryption
				switch(pRomeCfgParam->wlanCfgParam[i].authType){				
					case AUTH_8021x:
						sprintf(cmdBuf, "busybox iwpriv  wlan%d set_mib authtype=0",i,value);	printf("%s\n", cmdBuf);system(cmdBuf);						
						sprintf(cmdBuf, "port%d",i); 
						pRomeCfgParam->wlanCfgParam[i].authRadiusPort=atoi(websGetVar(wp,cmdBuf,""));

						sprintf(cmdBuf, "ipaddr%d",i); 
						parse_ipRange(websGetVar(wp,cmdBuf,""),&pRomeCfgParam->wlanCfgParam[i].authRadiusIp,&tempip);

						sprintf(cmdBuf, "passwd%d",i); 
						memcpy(pRomeCfgParam->wlanCfgParam[i].authRadiusPasswd,websGetVar(wp,cmdBuf,""),strlen(websGetVar(wp,cmdBuf,""))+1);
						sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib 802_1x=1",i);
						printf("%s\n", cmdBuf);system(cmdBuf);

						sprintf(cmdBuf, "enable1x = 1\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);					
						sprintf(cmdBuf, "authentication = 1\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);//use RADIUS
						sprintf(cmdBuf, "rsPort = %d\n",pRomeCfgParam->wlanCfgParam[i].authRadiusPort);printf("%s",cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
						sprintf(cmdBuf, "rsIP = %u.%u.%u.%u\n",NIPQUAD(pRomeCfgParam->wlanCfgParam[i].authRadiusIp));printf("%s",cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
						sprintf(cmdBuf, "rsPassword = \"%s\"\n",pRomeCfgParam->wlanCfgParam[i].authRadiusPasswd);printf("%s",cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
						sprintf(cmdBuf, "rsMaxReq = 3\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
						sprintf(cmdBuf, "rsAWhile = 5\n");printf("%s: %s",cfgFilePath,cmdBuf);fprintf(wpaCfgFile,"%s",cmdBuf);
						break;
					case AUTH_NONE:
						sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib 802_1x=0",i);
						printf("%s\n", cmdBuf);system(cmdBuf);
						sprintf(cmdBuf, "busybox iwpriv  wlan%d set_mib authtype=0",i,value);	printf("%s\n", cmdBuf);system(cmdBuf);
						break;
				}
		}
#endif
	}
	fclose(wpaCfgFile);

	for(i=0;i<totalWlanCards;i++){	
		if(AUTH_8021x==pRomeCfgParam->wlanCfgParam[i].authType||
		pRomeCfgParam->wlanCfgParam[i].cryptType==CRYPT_AES||
		pRomeCfgParam->wlanCfgParam[i].cryptType==CRYPT_TKIP){
			sprintf(cmdBuf, "auth wlan%d eth1 auth %s &", i, cfgFilePath);printf("%s\n", cmdBuf);system(cmdBuf);
		}
		sprintf(cmdBuf, "ifconfig wlan%d up", i);
		printf("%s\n", cmdBuf);system(cmdBuf);;
	}
	
	cfgmgr_write(CFGMGR_TABID_WLAN,(void*)pRomeCfgParam->wlanCfgParam,sizeof(wlanCfgParam_t)*MAX_WLAN_CARDS);
	cfgmgr_task();
	websRedirect(wp, "wireless_security.asp");	
}	

/************************************************************
 *	
 *	Wireless LAN Access Control
 *	
 ************************************************************/

int asp_wlanAccessCtrl(int ejid, webs_t wp, int argc, char_t **argv)
{
	int i,j;
	char cmdBuf[40];
	/* read cfg from cfgmgr */

	if (cfgmgr_read(CFGMGR_TABID_WLAN,(void*)&(pRomeCfgParam->wlanCfgParam), sizeof(struct wlanCfgParam_s)*MAX_WLAN_CARDS)!=0)
	{
		printf("asp_wlanAccessCtrl: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}
	
	for(i=0;i<totalWlanCards;i++){		
		sprintf(cmdBuf, "alias%d",i); 
		if(strcmp(argv[0],cmdBuf)==0){
			websWrite(wp,"(%s)",pRomeCfgParam->wlanCfgParam[i].aliasName);
			break;
		}
		sprintf(cmdBuf, "enable%d_%d",i,pRomeCfgParam->wlanCfgParam[i].acEnable);
		if(strcmp(argv[0],cmdBuf)==0){ 
			websWrite(wp,"selected");
			break;
		}
		sprintf(cmdBuf, "list%d",i);		
		if(strcmp(argv[0],cmdBuf)==0)
		{
			char temp[6]={0}, count=0;

			for(j=0;j<MAX_WLAN_AC;j++)
			{			
				if(memcmp(pRomeCfgParam->wlanCfgParam[i].acMacList[j],temp,6)==0) 
					continue;
				websWrite(wp,
		 		"<tr bgColor=#ddeeff><td>%d</td><td>%02X-%02X-%02X-%02X-%02X-%02X</td><td>%s</td><td><input type=checkbox value=1 name=ac%d_%d></td></tr>\n",
				j,
	 			pRomeCfgParam->wlanCfgParam[i].acMacList[j][0],
	 			pRomeCfgParam->wlanCfgParam[i].acMacList[j][1],
	 			pRomeCfgParam->wlanCfgParam[i].acMacList[j][2],
	 			pRomeCfgParam->wlanCfgParam[i].acMacList[j][3],
	 			pRomeCfgParam->wlanCfgParam[i].acMacList[j][4],
	 			pRomeCfgParam->wlanCfgParam[i].acMacList[j][5], 			
	 			pRomeCfgParam->wlanCfgParam[i].acComment[j],i,j);
				count++;
			} 
			if(count==0)
				websWrite(wp,
		 		"<tr bgColor=#ddeeff><td colspan=4 align=center>--client list is empty now--</td></tr>\n");
			break;
		}	
	}
	return 0;
}	
void asp_setWlanAccessCtrl(webs_t wp, char_t *path, char_t *query)
{
	int w,i, total=-1;
	char zero[6]={0};	
	//rtl8651_tblDrvAclRule_t tempAclRule;
	char cmdBuf[40],cmdBuf1[40];
	/* read cfg from cfgmgr */
	for(w=0;w<totalWlanCards;w++){	
		sprintf(cmdBuf, "deleteSelected%d",w);
		sprintf(cmdBuf1, "deleteAll%d",w);
		if(strcmp(websGetVar(wp,cmdBuf,""),"Delete Selected")==0)
		{
			char str[8];
			for(i=0;i<MAX_WLAN_AC;i++)
			{
				sprintf(str,"ac%d_%d",w,i);
				if(atoi(websGetVar(wp,str,""))==1)
				{
					total=i;
					memset(pRomeCfgParam->wlanCfgParam[w].acMacList[i], 0, 6);
					memset(pRomeCfgParam->wlanCfgParam[w].acComment[i], 0, 32);					
				}else if(total>=0&&memcmp(pRomeCfgParam->wlanCfgParam[w].acMacList[i],zero,6)==0){
					//move last to first
					memcpy(pRomeCfgParam->wlanCfgParam[w].acMacList[total],pRomeCfgParam->wlanCfgParam[w].acMacList[i-1],6);
					total=i;
					break;
				}
			}
		}
		else if(strcmp(websGetVar(wp,cmdBuf1,""),"Delete All")==0)	
		{
			//cleanup all Access control configuration in flash
			memset(pRomeCfgParam->wlanCfgParam[w].acMacList, 0, MAX_WLAN_AC*6);
			memset(pRomeCfgParam->wlanCfgParam[w].acComment, 0, MAX_WLAN_AC*32);
		}
		else
		{
			sprintf(cmdBuf, "enable%d",w);
			pRomeCfgParam->wlanCfgParam[w].acEnable=
				atoi(websGetVar(wp,cmdBuf,""));

			sprintf(cmdBuf, "mac%d",w);
			if(strlen(websGetVar(wp,cmdBuf,""))!=0)
			{
				for(i=0;i<MAX_WLAN_AC;i++)
				{		
					if(memcmp(pRomeCfgParam->wlanCfgParam[w].acMacList[i],zero,6)==0)	
						break;
				}
				if(i==MAX_WLAN_AC) 
					return; // ac list full	
				parse_mac(websGetVar(wp,cmdBuf,""),pRomeCfgParam->wlanCfgParam[w].acMacList[i]);

				sprintf(cmdBuf, "comment%d",w);
				memcpy(pRomeCfgParam->wlanCfgParam[w].acComment[i],websGetVar(wp,cmdBuf,""),strlen(websGetVar(wp,cmdBuf,""))+1);
				i++;
				total=i;
				for(;i<MAX_WLAN_AC;i++){
					memset(pRomeCfgParam->wlanCfgParam[w].acMacList[i],0, 6);
					memset(pRomeCfgParam->wlanCfgParam[w].acComment[i],0, 6);
				}
			}
		}
		
#ifdef CONFIG_RTL8185
		sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib aclmode=%d", w,	pRomeCfgParam->wlanCfgParam[w].acEnable);
		printf("%s\n", cmdBuf);system(cmdBuf);;
		if(total>0){
			sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib aclnum=0", w);
			printf("%s\n", cmdBuf);system(cmdBuf);;
			for(i=0;i<total;i++){
				sprintf(cmdBuf, "busybox iwpriv wlan%d set_mib acladdr=%02x%02x%02x%02x%02x%02x",
					w,pRomeCfgParam->wlanCfgParam[w].acMacList[i][0],
					pRomeCfgParam->wlanCfgParam[w].acMacList[i][1],
					pRomeCfgParam->wlanCfgParam[w].acMacList[i][2],
					pRomeCfgParam->wlanCfgParam[w].acMacList[i][3],
					pRomeCfgParam->wlanCfgParam[w].acMacList[i][4],
					pRomeCfgParam->wlanCfgParam[w].acMacList[i][5]
				);
				printf("%s\n", cmdBuf);system(cmdBuf);;
			}
		}
#endif
	}
	cfgmgr_write(CFGMGR_TABID_WLAN,(void*)pRomeCfgParam->wlanCfgParam,sizeof(wlanCfgParam_t)*MAX_WLAN_CARDS);
	cfgmgr_task();
	websRedirect(wp, "wireless_ac.asp");
}

/************************************************************
 *	
 *	Wireless Distribution System
 *	
 ************************************************************/

int asp_wlanWds(int ejid, webs_t wp, int argc, char_t **argv)
{
	int w;
	char cmdBuf[40];

	if (cfgmgr_read(CFGMGR_TABID_WLAN,(void*)(pRomeCfgParam->wlanCfgParam), sizeof(struct wlanCfgParam_s)*MAX_WLAN_CARDS)!=0)
	{
		printf("asp_wlanWds: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}
	
	for(w=0;w<totalWlanCards;w++){		
		/* read cfg from cfgmgr */
		sprintf(cmdBuf, "alias%d",w); 
		if(strcmp(argv[0],cmdBuf)==0){
			websWrite(wp,"%s",pRomeCfgParam->wlanCfgParam[w].aliasName);
			break;
		}

		sprintf(cmdBuf, "enable%d",w); 
		if(strcmp(argv[0],cmdBuf)==0){
			websWrite(wp,"%s",pRomeCfgParam->wlanCfgParam[w].wdsEnable?"checked":"");
			break;
		}

		sprintf(cmdBuf, "list%d",w); 		
		if(strcmp(argv[0],cmdBuf)==0)
		{
			int i,count=0;
			char temp[6]={0};

			for(i=0;i<MAX_WLAN_WDS;i++)
			{			
				if(memcmp(pRomeCfgParam->wlanCfgParam[w].wdsMacList[i],temp,6)==0) 
					continue;
				websWrite(wp,
		 		"<tr bgColor=#ddeeff><td>%d</td><td>%02X-%02X-%02X-%02X-%02X-%02X</td><td>%s</td><td><input type=checkbox value=1 name=wds%d_%d></td></tr>\n",
				i,
	 			pRomeCfgParam->wlanCfgParam[w].wdsMacList[i][0],
	 			pRomeCfgParam->wlanCfgParam[w].wdsMacList[i][1],
	 			pRomeCfgParam->wlanCfgParam[w].wdsMacList[i][2],
	 			pRomeCfgParam->wlanCfgParam[w].wdsMacList[i][3],
	 			pRomeCfgParam->wlanCfgParam[w].wdsMacList[i][4],
	 			pRomeCfgParam->wlanCfgParam[w].wdsMacList[i][5], 			
	 			pRomeCfgParam->wlanCfgParam[w].wdsComment[i],w,i);
				count++;
			} 
			if(count==0)
				websWrite(wp,
		 		"<tr bgColor=#ddeeff><td colspan=4 align=center>--AP list is empty now--</td></tr>\n");
			break;
		}	
	}		
	return 0;
	
}	
void asp_setWlanWds(webs_t wp, char_t *path, char_t *query)
{
	int i,w;
	char temp[6]={0,0,0,0,0,0};	
	char cmdBuf[64];
	char cmdBuf1[64];
	for(w=0;w<totalWlanCards;w++){		
		int32 wdsCount=pRomeCfgParam->wlanCfgParam[w].wdsEnable;
		sprintf(cmdBuf, "deleteSelected%d",w);
		sprintf(cmdBuf1, "deleteAll%d",w);
		if(strcmp(websGetVar(wp,cmdBuf,""),"Delete Selected")==0)
		{
			int i;
			char str[8];
			for(i=0;i<MAX_WLAN_WDS;i++)
			{
				sprintf(str,"wds%d_%d",w,i);
				if(atoi(websGetVar(wp,str,""))==1)
				{
					memset(pRomeCfgParam->wlanCfgParam[w].wdsMacList[i], 0, 6);
					memset(pRomeCfgParam->wlanCfgParam[w].wdsComment[i], 0, 32);					
					wdsCount--;
				}
			}
		}

		else if(strcmp(websGetVar(wp,cmdBuf1,""),"Delete All")==0)	
		{

			if(memcmp(pRomeCfgParam->wlanCfgParam[w].wdsMacList[i],temp,6)!=0){
				memset(pRomeCfgParam->wlanCfgParam[w].wdsMacList[i], 0, 6);
				memset(pRomeCfgParam->wlanCfgParam[w].wdsComment[i], 0, 32);					
			}
			memset(pRomeCfgParam->wlanCfgParam[w].wdsMacList, 0, MAX_WLAN_WDS*6);
			memset(pRomeCfgParam->wlanCfgParam[w].wdsComment, 0, MAX_WLAN_WDS*32);
			wdsCount=0;
		}

		else
		{
			sprintf(cmdBuf, "enable%d",w);
			pRomeCfgParam->wlanCfgParam[w].wdsEnable=atoi(websGetVar(wp,cmdBuf,""));
			sprintf(cmdBuf, "mac%d",w);
			if(strlen(websGetVar(wp,cmdBuf,""))!=0)
			{
				for(i=0;i<MAX_WLAN_WDS;i++)
				{		
					if(memcmp(pRomeCfgParam->wlanCfgParam[w].wdsMacList[i],temp,6)==0)	
						break;
				}
				if(i==MAX_WLAN_WDS) 
					return; // ac list full	
				parse_mac(websGetVar(wp,cmdBuf,""),pRomeCfgParam->wlanCfgParam[w].wdsMacList[i]);		
				sprintf(cmdBuf, "comment%d",w);
				wdsCount=i+1;
				memcpy(pRomeCfgParam->wlanCfgParam[w].wdsComment[i],websGetVar(wp,cmdBuf,""),strlen(websGetVar(wp,cmdBuf,""))+1);
			}
		}
		pRomeCfgParam->wlanCfgParam[w].wdsEnable=wdsCount;
		cfgmgr_write(CFGMGR_TABID_WLAN,(void*)pRomeCfgParam->wlanCfgParam,sizeof(wlanCfgParam_t)*MAX_WLAN_CARDS);
	}		
	cfgmgr_task();
#ifdef CONFIG_RTL8185
	//must restart WLAN card in 8185 driver...:(
	for(i=0;i<totalWlanCards;i++){
		sprintf(cmdBuf, "ifconfig wlan%d down",i); //Bring down the interface first.
				printf("%s\n", cmdBuf);system(cmdBuf);;
		if(pRomeCfgParam->wlanCfgParam[i].enable == 1){
			if(0==wlan_setup(i)){
				sprintf(cmdBuf, "ifconfig wlan%d up",i); //bring up interface.
				printf("%s\n", cmdBuf);system(cmdBuf);;
				wds_up(i);
			}
		}
	}		
#endif	
	websRedirect(wp, "wireless_wds.asp");
}


/************************************************************
 *	
 *	UDP Blocking
 *	
 ************************************************************/
int asp_udpBlocking(int ejid, webs_t wp, int argc, char_t **argv)
{

	int ms=asp_multiSession(wp,argc,argv);
	
 	if(strcmp(argv[0],"size")==0)
 	{
		int i;
		for(i=3;i<=30;i++)			
	 		websWrite(wp,"<option value=%d%s>%d</option>\n",i,(pRomeCfgParam->udpblockCfgParam[ms].size==i)?" selected":"",i);
	}
 	else if(strcmp(argv[0],"MS")==0)
 	{
 		websWrite(wp,"%d",ms);
 	}
	else if(strcmp(argv[0],"enable")==0)
	{
		if(pRomeCfgParam->udpblockCfgParam[ms].enable==1) websWrite(wp,"checked");
	}
	
	return 0;
}

void asp_setUdpBlocking(webs_t wp, char_t *path, char_t *query)
{
	char		tmpstr[32]={0};	
	int		ms=atoi(websGetVar(wp,"ms",""));

	pRomeCfgParam->udpblockCfgParam[ms].size=atoi(websGetVar(wp,"size",""));

	if(atoi(websGetVar(wp,"enable",""))==1)
	{	
		pRomeCfgParam->udpblockCfgParam[ms].enable=1;
	}	
	else
	{
		pRomeCfgParam->udpblockCfgParam[ms].enable=0;	
	}

	cfgmgr_write(CFGMGR_TABID_UDPBLOCK,(void*)pRomeCfgParam->udpblockCfgParam, sizeof(struct udpblockCfgParam_s)*MAX_PPPOE_SESSION);
	cfgmgr_task();

	sprintf(tmpstr,"udpblocking.asp?ms=%d",ms);	
	udp_tableDriverAccess(ms);
	websRedirect(wp, tmpstr);

}


/************************************************************
 *	
 *	Routing Table
 *	
 ************************************************************/
int asp_routing(int ejid, webs_t wp, int argc, char_t **argv)
{
	int i;
	for(i=0;i<5;i++)
	{

		websWrite(wp,
		"<tr>\n" \
		"<td bgColor=#ddeeff>\n" \
		"		<input type=text name=route%d size=15 maxlength=15 value=\"%u.%u.%u.%u\">\n" \
		"	</td>\n" \
		"	<td bgColor=#ddeeff>\n" \
		"		<input type=text name=mask%d size=15 maxlength=15 value=\"%u.%u.%u.%u\">\n" \
		"	</td>\n" \
		"    <td bgColor=#ddeeff>\n" \
		"		<input type=text name=gateway%d size=15 maxlength=15 value=\"%u.%u.%u.%u\">\n" \
		"	</td>\n" \
		"    <td bgColor=#ddeeff>\n" \
		"		<select name=interface%d><option value=0>------<option value=1 %s>LAN<option value=2 %s>WAN</select>\n" \
		"	</td>\n" \
		"</tr>\n" ,i,NIPQUAD(pRomeCfgParam->routingCfgParam[i].route)
				,i,NIPQUAD(pRomeCfgParam->routingCfgParam[i].mask)
				,i,NIPQUAD(pRomeCfgParam->routingCfgParam[i].gateway)
				,i,(pRomeCfgParam->routingCfgParam[i].interface==1)?"selected":""
				,(pRomeCfgParam->routingCfgParam[i].interface==2)?"selected":"");

	}

	return 0;
}

void asp_setRouting(webs_t wp, char_t *path, char_t *query)
{
	int i,ret;
	char	 tmpstr[32]={0};

	for(i=0;i<MAX_ROUTING;i++)
	{
		if(pRomeCfgParam->routingCfgParam[i].interface!=ROUTING_INTERFACE_NONE)
		{
			ret=rtl8651_delRoute((ipaddr_t)pRomeCfgParam->routingCfgParam[i].route,(ipaddr_t)pRomeCfgParam->routingCfgParam[i].mask);
			if (ret)
			{
				printf("rtl8651_delRoute(default): ret %d\n",ret);
			}
		}
	}
	

	for(i=0;i<MAX_ROUTING;i++)
	{
		ipaddr_t ip=0,ip2=0;

		sprintf(tmpstr,"mask%d",i);
		parse_ipRange(websGetVar(wp,tmpstr,""), &ip, &ip2);
		pRomeCfgParam->routingCfgParam[i].mask=ip;
		
		sprintf(tmpstr,"route%d",i);
		parse_ipRange(websGetVar(wp,tmpstr,""), &ip, &ip2);
		pRomeCfgParam->routingCfgParam[i].route=ip&pRomeCfgParam->routingCfgParam[i].mask;



		sprintf(tmpstr,"gateway%d",i);
		parse_ipRange(websGetVar(wp,tmpstr,""), &ip, &ip2);
		pRomeCfgParam->routingCfgParam[i].gateway=ip;
		
		sprintf(tmpstr,"interface%d",i);
		pRomeCfgParam->routingCfgParam[i].interface=atoi(websGetVar(wp,tmpstr,""));

		if(pRomeCfgParam->routingCfgParam[i].interface!=ROUTING_INTERFACE_NONE)
		{
			char interface[8]={0};
			if(pRomeCfgParam->routingCfgParam[i].interface==ROUTING_INTERFACE_WAN) sprintf(interface,"eth0");
			if(pRomeCfgParam->routingCfgParam[i].interface==ROUTING_INTERFACE_LAN) sprintf(interface,"eth1");
			ret=rtl8651_addRoute((ipaddr_t)pRomeCfgParam->routingCfgParam[i].route,
								  (ipaddr_t)pRomeCfgParam->routingCfgParam[i].mask,
								  (int8*)interface,(ipaddr_t)pRomeCfgParam->routingCfgParam[i].gateway);
			if (ret)
			{
				pRomeCfgParam->routingCfgParam[i].interface=ROUTING_INTERFACE_NONE;
				printf("rtl8651_addRoute %x: ret %d\n",pRomeCfgParam->routingCfgParam[i].route,ret);				
			}
		}		
	}


				
	cfgmgr_write(CFGMGR_TABID_ROUTING,(void*)pRomeCfgParam->routingCfgParam, sizeof(struct routingCfgParam_s)*MAX_ROUTING);
	cfgmgr_task();
	websRedirect(wp, "routing.asp");
	
}



int asp_qos(int ejid, webs_t wp, int argc, char_t **argv)
{
	char *tagName[] = { "ftp_switch", "http_switch", "telnet_switch", "smtp_switch", "pop3_switch", "specific_1", "specific_2", "specific_3" };
	char *labelName[] = { "FTP", "HTTP", "TELNET", "SMTP", "POP3", "port_1", "	port_2", "port_3" };
	char *string_0[] = { "Port 0", "Port 1", "Port 2", "Port 3", "Port 4" };
	char *string_1[] = { "pq_priority_0", "pq_priority_1", "pq_priority_2", "pq_priority_3", "pq_priority_4" };
	char *string_2[] = { "pq_flowctl_0", "pq_flowctl_1", "pq_flowctl_2", "pq_flowctl_3", "pq_flowctl_4" };
	char *string_3[] = { "pq_inrl_0", "pq_inrl_1", "pq_inrl_2", "pq_inrl_3", "pq_inrl_4" };
	char *string_4[] = { "pq_outrl_0", "pq_outrl_1", "pq_outrl_2", "pq_outrl_3", "pq_outrl_4" };
	int index;	

	
	/* Application-Based QoS */
	websWrite(wp,
		"<form method=\"get\" action=\"/goform/asp_setQos\" name=qosform1>\n"\
		"<tr><td><b>Hardware QoS by Application</b></td></tr>\n"\
		"<br>\n"\
		"<td bgColor=#ddeeff><input type=checkbox name=ab_switch %s value=1 onClick=\"itemSwitch1();\">Enable</td>\n"\
		"<table cellSpacing=1 cellPadding=2 border=0>\n"\
		"<tr><td bgColor=#aaddff>  Protocol  </td><td bgColor=#aaddff>  High Priority  </td><td bgColor=#aaddff>  Low Priority  </td></tr>\n",
		((pRomeCfgParam->qosCfgParam[0].qosType&ENABLE_QOS)? "checked": "")
	);
	
	
	for(index=0; index<5; index++) {
		websWrite(wp,
			"<tr>\n"\
			"<td bgColor=#ddeeff>%s</td>\n"\
			"<td bgColor=#ddeeff><input type=radio name=%s value=high %s></td>\n"\
			"<td bgColor=#ddeeff><input type=radio name=%s value=low %s></td>\n"\
			"</tr>\n", 
			labelName[index], 
			tagName[index], ((pRomeCfgParam->qosCfgParam[index].un.policy_based.isHigh==TRUE)? "checked": " "),
			tagName[index], ((pRomeCfgParam->qosCfgParam[index].un.policy_based.isHigh==FALSE)? "checked": " ")
		);
	}

	websWrite(wp, "<tr><td bgColor=#aaddff>  Specific Port#  </td><td bgColor=#aaddff>  High Priority  </td><td bgColor=#aaddff>  Low Priority  </td></tr>\n");

	for(index=5; index<8; index++) {
		websWrite(wp,
			"<tr>\n"\
			"<td bgColor=#ddeeff><input type=text name=%s size = 5 maxlength=5 enable value=%d></td>\n"\
			"<td bgColor=#ddeeff><input type=radio name=%s value=high %s></td>\n"\
			"<td bgColor=#ddeeff><input type=radio name=%s value=low %s></td>\n",
			labelName[index], pRomeCfgParam->qosCfgParam[index].un.policy_based.portNumber,
			tagName[index],   ((pRomeCfgParam->qosCfgParam[index].un.policy_based.isHigh==TRUE)? "checked": " "),
			tagName[index],  ((pRomeCfgParam->qosCfgParam[index].un.policy_based.isHigh==FALSE)? "checked": " ")
		);
	}
	
	websWrite(wp,
		"<tr>\n"\
		"<td colspan=2 align=center>\n"\
		"<input type=submit value=Apply>\n"\
		"<input type=reset value=Reset>\n"\
		"</td>\n"\
	   	"</tr>\n"\
	   	"</table> </form>\n"
	 );



	/* Port-Based QoS */
	websWrite(wp,
		"<form method=\"get\" action=\"/goform/asp_setQos1\" name=qosform2>\n"\
		"<tr><td><b>Hardware QoS by device port number</b></td></tr>\n"\
		"<br>\n"\
		"<td bgColor=#ddeeff><input type=checkbox name=ab_switch %s value=1 onClick=\"itemSwitch2();\">Enable</td>\n"\
		"<table cellSpacing=1 cellPadding=2 border=0>\n"\
		"<tr>\n"\
		"<td bgColor=#aaddff>  Port #  </td>\n"\
		"<td bgColor=#aaddff>  Priority  </td>\n"\
		"<td bgColor=#aaddff>  Flow Control  </td>\n"\
		"<td bgColor=#aaddff>  Incoming Rate Limit </td>\n"\
		"<td bgColor=#aaddff>  Outgoing Rate Limit </td>\n"\
		"</tr>",
		((pRomeCfgParam->qosCfgParam[8].qosType&ENABLE_QOS)? "checked": "")
	);
	
	for(index=8; index<MAX_QOS; index++) {
		websWrite(wp,
			"<tr>\n"\
			"<td bgColor=#ddeeff>%s</td>\n"\
			"<td bgColor=#ddeeff><select name=%s><option %s>High<option %s>Low</select></td>\n"\
			"<td bgColor=#ddeeff><select name=%s><option %s>Enable<option %s>Disable</select></td>\n"\
			"<td bgColor=#ddeeff>\n"\
			"<select name=%s>\n"\
			"<option %s>Full\n"\
			"<option %s>32M<option %s>16M<option %s>8M<option %s>4M<option %s>2M<option %s>1M<option %s>512K<option %s>256K<option %s>128K\n"\
			"</select>\n"\
			"</td>\n"\
			"<td bgColor=#ddeeff>\n"\
			"<select name=%s>\n"\
			"<option %s>Full<option %s>32M<option %s>16M<option %s>8M<option %s>4M<option %s>2M<option %s>1M	<option %s>512K<option %s>256K<option %s>128K\n"\
			"</select>\n"\
			"</td>\n"\
			"</tr>\n",
			string_0[index-8], 
			string_1[index-8], ((pRomeCfgParam->qosCfgParam[index].un.port_based.isHigh==TRUE)? "selected": " "), 
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.isHigh==FALSE)? "selected": " "),
			string_2[index-8], ((pRomeCfgParam->qosCfgParam[index].un.port_based.enableFlowCtrl==TRUE)? "selected": " "),
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.enableFlowCtrl==FALSE)? "selected": " "),
			string_3[index-8], ((pRomeCfgParam->qosCfgParam[index].un.port_based.inRL==RTL8651_BC_FULL)? "selected": " "),
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.inRL==RTL8651_BC_32M)? "selected": " "),
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.inRL==RTL8651_BC_16M)? "selected": " "),
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.inRL==RTL8651_BC_8M)? "selected": " "),
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.inRL==RTL8651_BC_4M)? "selected": " "),
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.inRL==RTL8651_BC_2M)? "selected": " "),
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.inRL==RTL8651_BC_1M)? "selected": " "),
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.inRL==RTL8651_BC_512K)? "selected": " "),
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.inRL==RTL8651_BC_256K)? "selected": " "),
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.inRL==RTL8651_BC_128K)? "selected": " "),
			string_4[index-8],  ((pRomeCfgParam->qosCfgParam[index].un.port_based.outRL==RTL8651_BC_FULL)? "selected": " "),
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.outRL==RTL8651_BC_32M)? "selected": " "),
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.outRL==RTL8651_BC_16M)? "selected": " "),
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.outRL==RTL8651_BC_8M)? "selected": " "),
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.outRL==RTL8651_BC_4M)? "selected": " "),
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.outRL==RTL8651_BC_2M)? "selected": " "),
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.outRL==RTL8651_BC_1M)? "selected": " "),
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.outRL==RTL8651_BC_512K)? "selected": " "),
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.outRL==RTL8651_BC_256K)? "selected": " "),
						      ((pRomeCfgParam->qosCfgParam[index].un.port_based.outRL==RTL8651_BC_128K)? "selected": " ")
		);
	}

	
	websWrite(wp,
		"<tr>\n"\
		"<td colspan=2 align=center>\n"\
		"<input type=submit value=Apply>\n"\
		"<input type=reset value=Reset>\n"\
		"</td>\n"\
	   	"</tr>\n"\
		"</table>\n"\
		"</form>\n"
	);

}

void asp_setQos(webs_t wp, char_t *path, char_t *query)
{
	{
		char *tagName[] = { "ftp_switch", "http_switch", "telnet_switch", "smtp_switch", "pop3_switch", "specific_1", "specific_2", "specific_3" };
		char *spec[] = { "port_1", "port_2", "port_3" };
		rtl8651_tblDrvPolicy_t policy;
		char *str;
		int index;

		str = websGetVar(wp,"ab_switch","");
		if (strcmp(str, "1") == 0) {
			pRomeCfgParam->qosCfgParam[0].qosType |= ENABLE_QOS;
			rtl8651_flushPolicyBasedQoS();
			for(index=0; index<8; index++) {
				str = websGetVar(wp,T(tagName[index])," ");
				if (strcmp(str, "low") == 0)
					pRomeCfgParam->qosCfgParam[index].un.policy_based.isHigh = FALSE;
				else pRomeCfgParam->qosCfgParam[index].un.policy_based.isHigh = TRUE;
				if (index > 4) {
					pRomeCfgParam->qosCfgParam[index].un.policy_based.portNumber = atoi(websGetVar(wp, spec[index-5], " "));
				}
				if (pRomeCfgParam->qosCfgParam[index].un.policy_based.portNumber != 0) {
					bzero(&policy, sizeof(rtl8651_tblDrvPolicy_t));
					policy.proto = RTL8651_POLICY_TCP;
					//Server is in the WAN
					policy.sport_start = policy.sport_end = pRomeCfgParam->qosCfgParam[index].un.policy_based.portNumber;
					rtl8651_addPolicyBasedQoS("eth0", &policy, pRomeCfgParam->qosCfgParam[index].un.policy_based.isHigh);
					//Server is in the LAN
					policy.sport_start = policy.sport_end = 0;
					policy.dport_start = policy.dport_end = pRomeCfgParam->qosCfgParam[index].un.policy_based.portNumber;
					rtl8651_addPolicyBasedQoS("eth0", &policy, pRomeCfgParam->qosCfgParam[index].un.policy_based.isHigh);

					bzero(&policy, sizeof(rtl8651_tblDrvPolicy_t));
					policy.proto = RTL8651_POLICY_TCP;
					//Server is in the WAN
					policy.dport_start = policy.dport_end = pRomeCfgParam->qosCfgParam[index].un.policy_based.portNumber;
					rtl8651_addPolicyBasedQoS("eth1", &policy, pRomeCfgParam->qosCfgParam[index].un.policy_based.isHigh);
					//Server is in the LAN
					policy.dport_start = policy.dport_end = 0;
					policy.sport_start = policy.sport_end = pRomeCfgParam->qosCfgParam[index].un.policy_based.portNumber;
					rtl8651_addPolicyBasedQoS("eth1", &policy, pRomeCfgParam->qosCfgParam[index].un.policy_based.isHigh);

					if (index > 4) {
						bzero(&policy, sizeof(rtl8651_tblDrvPolicy_t));
						policy.proto = RTL8651_POLICY_UDP;
						//Server is in the WAN
						policy.sport_start = policy.sport_end = pRomeCfgParam->qosCfgParam[index].un.policy_based.portNumber;
						rtl8651_addPolicyBasedQoS("eth0", &policy, pRomeCfgParam->qosCfgParam[index].un.policy_based.isHigh);
						//Server is in the LAN
						policy.sport_start = policy.sport_end = 0;
						policy.dport_start = policy.dport_end = pRomeCfgParam->qosCfgParam[index].un.policy_based.portNumber;
						rtl8651_addPolicyBasedQoS("eth0", &policy, pRomeCfgParam->qosCfgParam[index].un.policy_based.isHigh);

						bzero(&policy, sizeof(rtl8651_tblDrvPolicy_t));
						policy.proto = RTL8651_POLICY_UDP;
						//Server is in the WAN
						policy.dport_start = policy.dport_end = pRomeCfgParam->qosCfgParam[index].un.policy_based.portNumber;
						rtl8651_addPolicyBasedQoS("eth1", &policy, pRomeCfgParam->qosCfgParam[index].un.policy_based.isHigh);
						//Server is in the LAN
						policy.dport_start = policy.dport_end = 0;
						policy.sport_start = policy.sport_end = pRomeCfgParam->qosCfgParam[index].un.policy_based.portNumber;
						rtl8651_addPolicyBasedQoS("eth1", &policy, pRomeCfgParam->qosCfgParam[index].un.policy_based.isHigh);


					}
				}
			}
		}
		else {
			pRomeCfgParam->qosCfgParam[0].qosType &= ~ENABLE_QOS;
			rtl8651_flushPolicyBasedQoS();
		}
		
		cfgmgr_write(CFGMGR_TABID_QOS,(void*)pRomeCfgParam->qosCfgParam, sizeof(struct qosCfgParam_s)*MAX_QOS);
		cfgmgr_task();
		websRedirect(wp, "qos.asp");
	}
}



void asp_setQos1(webs_t wp, char_t *path, char_t *query)
{
	char *string_1[] = { "pq_priority_0", "pq_priority_1", "pq_priority_2", "pq_priority_3", "pq_priority_4" };
	char *string_2[] = { "pq_flowctl_0", "pq_flowctl_1", "pq_flowctl_2", "pq_flowctl_3", "pq_flowctl_4" };
	char *string_3[] = { "pq_inrl_0", "pq_inrl_1", "pq_inrl_2", "pq_inrl_3", "pq_inrl_4" };
	char *string_4[] = { "pq_outrl_0", "pq_outrl_1", "pq_outrl_2", "pq_outrl_3", "pq_outrl_4" };
	int index;
	char *str;
	uint32 newInputRate, newOutputRate;

	str = websGetVar(wp,"ab_switch","");
	if (strcmp(str, "1") == 0) {
		pRomeCfgParam->qosCfgParam[8].qosType |= ENABLE_QOS;
		for(index=0; index<5; index++) {
			str = websGetVar(wp,T(string_1[index])," ");
			if (strcmp(str, "Low") == 0) 
				pRomeCfgParam->qosCfgParam[index+8].un.port_based.isHigh = FALSE;
			else pRomeCfgParam->qosCfgParam[index+8].un.port_based.isHigh = TRUE;
			rtl8651_setPortPriorityQueue(index, pRomeCfgParam->qosCfgParam[index+8].un.port_based.isHigh);

			str = websGetVar(wp, T(string_2[index]), " ");
			if (strcmp(str, "Disable") == 0)
				pRomeCfgParam->qosCfgParam[index+8].un.port_based.enableFlowCtrl = FALSE;
			else pRomeCfgParam->qosCfgParam[index+8].un.port_based.enableFlowCtrl = TRUE;
			rtl8651_setFlowControl(index, pRomeCfgParam->qosCfgParam[index+8].un.port_based.enableFlowCtrl);

			str = websGetVar(wp, T(string_3[index]), " ");
			if (strcmp(str, "Full") == 0)
				newInputRate = RTL8651_BC_FULL;
			else if (strcmp(str, "32M") == 0)
				newInputRate= RTL8651_BC_32M;
			else if (strcmp(str, "16M") == 0)
				newInputRate= RTL8651_BC_16M;
			else if (strcmp(str, "8M") == 0)
				newInputRate= RTL8651_BC_8M;
			else if (strcmp(str, "4M") == 0)
				newInputRate= RTL8651_BC_4M;
			else if (strcmp(str, "2M") == 0)
				newInputRate = RTL8651_BC_2M;
			else if (strcmp(str, "1M") == 0)
				newInputRate= RTL8651_BC_1M;
			else if (strcmp(str, "512K") == 0)
				newInputRate = RTL8651_BC_512K;
			else if (strcmp(str, "256K") == 0)
				newInputRate = RTL8651_BC_256K;
			else 	newInputRate = RTL8651_BC_128K;
			if(rtl8651_setEthernetPortBandwidthControl(index, TRUE, newInputRate) == SUCCESS)
				pRomeCfgParam->qosCfgParam[index+8].un.port_based.inRL = newInputRate;

			str = websGetVar(wp, T(string_4[index]), " ");
			if (strcmp(str, "Full") == 0)
				newOutputRate= RTL8651_BC_FULL;
			else if (strcmp(str, "32M") == 0)
				newOutputRate= RTL8651_BC_32M;
			else if (strcmp(str, "16M") == 0)
				newOutputRate= RTL8651_BC_16M;
			else if (strcmp(str, "8M") == 0)
				newOutputRate= RTL8651_BC_8M;
			else if (strcmp(str, "4M") == 0)
				newOutputRate= RTL8651_BC_4M;
			else if (strcmp(str, "2M") == 0)
				newOutputRate= RTL8651_BC_2M;
			else if (strcmp(str, "1M") == 0)
				newOutputRate= RTL8651_BC_1M;
			else if (strcmp(str, "512K") == 0)
				newOutputRate= RTL8651_BC_512K;
			else if (strcmp(str, "256K") == 0)
				newOutputRate= RTL8651_BC_256K;
			else 	newOutputRate = RTL8651_BC_128K;
			if(rtl8651_setEthernetPortBandwidthControl(index, FALSE, newOutputRate) == SUCCESS)
				pRomeCfgParam->qosCfgParam[index+8].un.port_based.outRL = newOutputRate;
		}
	}
	else {
		pRomeCfgParam->qosCfgParam[8].qosType &= ~ENABLE_QOS;

		for(index=0; index<5; index++) {
			rtl8651_setFlowControl(index, FALSE);
			rtl8651_setPortPriorityQueue(index, FALSE);
			rtl8651_setEthernetPortBandwidthControl(index, TRUE, RTL8651_BC_FULL);
			rtl8651_setEthernetPortBandwidthControl(index, FALSE, RTL8651_BC_FULL);
		}
	}
	
	cfgmgr_write(CFGMGR_TABID_QOS,(void*)pRomeCfgParam->qosCfgParam, sizeof(struct qosCfgParam_s)*MAX_QOS);
	cfgmgr_task();
	websRedirect(wp, "qos.asp");
}


int asp_ratio_qos(int ejid, webs_t wp, int argc, char_t **argv)
{
	int i;

	if (strcmp(argv[0], "enable") == 0)
	{
		if (pRomeCfgParam->ratioQosCfgParam.enable)
			websWrite(wp, " checked ");
	} else if (strcmp(argv[0], "upRate") == 0)
	{
		websWrite(wp, "%d", pRomeCfgParam->ratioQosCfgParam.upInfo.maxRate);
	} else if (strcmp(argv[0], "upRateM") == 0)
	{
		if (pRomeCfgParam->ratioQosCfgParam.upInfo.maxRateUnit == 0)
			websWrite(wp, " selected ");
	} else if (strcmp(argv[0], "upRateK") == 0)
	{
		if (pRomeCfgParam->ratioQosCfgParam.upInfo.maxRateUnit == 1)
			websWrite(wp, " selected ");
	} else if (strcmp(argv[0],"list") == 0)
	{	/* list the exist setting of ratio-based qos */
		for (i = 0 ; i < MAX_RATIO_QOS ; i ++)
		{
			if (pRomeCfgParam->ratioQosCfgParam.entry[i].enable)
			{
				int displayPort;
			
				websWrite(wp, "<tr bgColor=#ddeeff>");
				websWrite(wp, "<td>%d</td>", (i+1));
				websWrite(wp, "<td>%s</td>", (pRomeCfgParam->ratioQosCfgParam.entry[i].isSrc?"Src":"Dst"));
				websWrite(wp, "<td>%u.%u.%u.%u</td>", NIPQUAD(pRomeCfgParam->ratioQosCfgParam.entry[i].ip));
				websWrite(wp, "<td>%u.%u.%u.%u</td>", NIPQUAD(pRomeCfgParam->ratioQosCfgParam.entry[i].ipMask));
				switch (pRomeCfgParam->ratioQosCfgParam.entry[i].protoType)
				{
					case 0:
						displayPort = TRUE;
						websWrite(wp, "<td>TCP</td>");
						break;
					case 1:
						displayPort = TRUE;
						websWrite(wp, "<td>UDP</td>");
						break;
					default:
						displayPort = FALSE;
						websWrite(wp, "<td>IP</td>");
				}
				if (displayPort == TRUE)
				{
					websWrite(wp, "<td>%d</td>", pRomeCfgParam->ratioQosCfgParam.entry[i].s_port);
					websWrite(wp, "<td>%d</td>", pRomeCfgParam->ratioQosCfgParam.entry[i].e_port);
				}else
				{
					websWrite(wp, "<td></td>");
					websWrite(wp, "<td></td>");
				}
				websWrite(wp, "<td>%s</td>", (pRomeCfgParam->ratioQosCfgParam.entry[i].isHigh?"Guaranteed":"Spare"));
				websWrite(wp, "<td>%d</td>", pRomeCfgParam->ratioQosCfgParam.entry[i].ratio);
				websWrite(wp, "<td><input type=\"checkbox\" name=\"disable_%d\" value=1></td>", i);
				websWrite(wp, "</tr>");
			}
		}
	}else if (strcmp(argv[0],"list_disab") == 0)
	{
		if (argc < 2)
			return 1;
		for (i = 0 ; i < MAX_RATIO_QOS ; i ++)
		{
			if (pRomeCfgParam->ratioQosCfgParam.entry[i].enable)
				websWrite(wp, "%s.disable_%d.disabled = true\n", argv[1],  i);
		}
	}else if (strcmp(argv[0],"list_enab") == 0)
	{
		if (argc < 2)
			return 1;
		for (i = 0 ; i < MAX_RATIO_QOS ; i ++)
		{
			if (pRomeCfgParam->ratioQosCfgParam.entry[i].enable)
				websWrite(wp, "%s.disable_%d.disabled = false\n", argv[1], i);
		}
	} else if (strcmp(argv[0], "check_new_add_s") == 0)
	{
		if (pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_h ||
			pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_l)
		{
			for ( i = 0 ; i < MAX_RATIO_QOS ; i ++)
			{
				if (!pRomeCfgParam->ratioQosCfgParam.entry[i].enable)
					goto out;/* empty entry found, don't comment new-entry-add html code */
			}
		}
		/* table full, we can not add entry */
		websWrite(wp, "<!--\n");
	} else if (strcmp(argv[0], "check_new_add_e") == 0)
	{
		if (pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_h ||
			pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_l)
		{
			for ( i = 0 ; i < MAX_RATIO_QOS ; i ++)
			{
				if (!pRomeCfgParam->ratioQosCfgParam.entry[i].enable)
					goto out;/* empty entry found, don't comment new-entry-add html code */
			}
		}
		/* table full, we can not add entry */
		websWrite(wp, "-->\n");
	} else if (strcmp(argv[0], "js_check_new_add_s") == 0)
	{
		if (pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_h ||
			pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_l)
		{
			for ( i = 0 ; i < MAX_RATIO_QOS ; i ++)
			{
				if (!pRomeCfgParam->ratioQosCfgParam.entry[i].enable)
					goto out;/* empty entry found, don't comment new-entry-add html code */
			}
		}
		/* table full, we can not add entry */
		websWrite(wp, "/*\n");
	} else if (strcmp(argv[0], "js_check_new_add_e") == 0)
	{
		if (pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_h ||
			pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_l)
		{
			for ( i = 0 ; i < MAX_RATIO_QOS ; i ++)
			{
				if (!pRomeCfgParam->ratioQosCfgParam.entry[i].enable)
					goto out;/* empty entry found, don't comment new-entry-add html code */
			}
		}
		/* table full, we can not add entry */
		websWrite(wp, "*/\n");
	} else if (strcmp(argv[0], "get_ratio_high") == 0)
	{
		websWrite(wp, "%d", pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_h);
	} else if (strcmp(argv[0], "get_ratio_low") == 0)
	{
		websWrite(wp, "%d", pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_l);
	} else if (strcmp(argv[0], "queue_select") == 0)
	{
		if (argc < 2)
			goto out;
		if (pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_h &&
			pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_l)
		{
			websWrite(wp, "<select name=\"queue\" onchange=\"%s();\">", argv[1]);
			websWrite(wp, "<option value=1 selected>Guaranteed");
			websWrite(wp, "<option value=0>Spare");
			websWrite(wp, "</select>");
		}else if (pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_h)
		{
			websWrite(wp, "Guaranteed");
		}else if (pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_l)
		{
			websWrite(wp, "Spare");
		}
	} else if (strcmp(argv[0], "check_queue_s") == 0)
	{
		for ( i = 0 ; i < MAX_RATIO_QOS ; i ++)
		{
			if (!pRomeCfgParam->ratioQosCfgParam.entry[i].enable)
				break;
		}
		if (	(i == MAX_RATIO_QOS) ||
			(!pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_h) ||
			(!pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_l))
		{
			websWrite(wp, "<!--\n");
		}
	} else if (strcmp(argv[0], "check_queue_e") == 0)
	{
		for ( i = 0 ; i < MAX_RATIO_QOS ; i ++)
		{
			if (!pRomeCfgParam->ratioQosCfgParam.entry[i].enable)
				break;
		}
		if (	(i == MAX_RATIO_QOS) ||
			(!pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_h) ||
			(!pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_l))
		{
			websWrite(wp, "-->\n");
		}
	} else if (strcmp(argv[0], "js_check_queue_s") == 0)
	{
		for ( i = 0 ; i < MAX_RATIO_QOS ; i ++)
		{
			if (!pRomeCfgParam->ratioQosCfgParam.entry[i].enable)
				break;
		}
		if (	(i == MAX_RATIO_QOS) ||
			(!pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_h) ||
			(!pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_l))
		{
			websWrite(wp, "/*\n");
		}
	} else if (strcmp(argv[0], "js_check_queue_e") == 0)
	{
		for ( i = 0 ; i < MAX_RATIO_QOS ; i ++)
		{
			if (!pRomeCfgParam->ratioQosCfgParam.entry[i].enable)
				break;
		}
		if (	(i == MAX_RATIO_QOS) ||
			(!pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_h) ||
			(!pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_l))
		{
			websWrite(wp, "*/\n");
		}
	}
out:
	return 0;
}



void rearrange_Ratio_Qos(int32 flushOnly)
{
	int i;

	/* Inite ratio QoS */
	rtl8651_setUpstreamBandwidth("eth0", 0);
	
	if (flushOnly == TRUE)
		return;
	
	/* set upstream bw */
	if (pRomeCfgParam->ratioQosCfgParam.enable == TRUE) {
		rtl8651_setUpstreamBandwidth("eth0",
			(pRomeCfgParam->ratioQosCfgParam.upInfo.maxRateUnit == 0)?
				(pRomeCfgParam->ratioQosCfgParam.upInfo.maxRate * 1000 * 1000):
				(pRomeCfgParam->ratioQosCfgParam.upInfo.maxRate * 1000));
	}
	
	/* set all entries */
	for (i = 0 ; i < MAX_RATIO_QOS ; i ++)
	{
		ratioQosEntry_t* entry;

		entry = &(pRomeCfgParam->ratioQosCfgParam.entry[i]);
		if (entry->enable)
		{
			rtl8651_tblDrvAclRule_t rule;

			memset(&rule, 0, sizeof(rtl8651_tblDrvAclRule_t));
			rule.actionType_ = RTL8651_ACL_DROP;
			switch(entry->protoType) {

			case 0: /* TCP */
				rule.ruleType_	= RTL8651_ACL_TCP;
				if (entry->isSrc == TRUE) {
					rule.tcpSrcPortUB_		= entry->e_port;
					rule.tcpSrcPortLB_		= entry->s_port;
					rule.tcpDstPortUB_		= 65535;
					rule.tcpDstPortLB_		= 0;
				}
				else {
					rule.tcpDstPortUB_		= entry->e_port;
					rule.tcpDstPortLB_		= entry->s_port;
					rule.tcpSrcPortUB_		= 65535;
					rule.tcpSrcPortLB_		= 0;
				}
				goto comm_L3;

			case 1: /* UDP */
				rule.ruleType_	= RTL8651_ACL_UDP;
				if (entry->isSrc == TRUE) {
					rule.udpSrcPortUB_	= entry->e_port;
					rule.udpSrcPortLB_		= entry->s_port;
					rule.udpDstPortUB_	= 65535;
					rule.udpDstPortLB_		= 0;
				}
				else {
					rule.udpDstPortUB_	= entry->e_port;
					rule.udpDstPortLB_		= entry->s_port;
					rule.udpSrcPortUB_	= 65535;
					rule.udpSrcPortLB_		= 0;
				}
				goto comm_L3;

			default: /* IP */
				rule.ruleType_ = RTL8651_ACL_IP;
comm_L3:
				if (entry->isSrc == TRUE) {
					rule.srcIpAddr_		= entry->ip;
					rule.srcIpAddrMask_	= entry->ipMask;
				}
				else {
					rule.dstIpAddr_		= entry->ip;
					rule.dstIpAddrMask_	= entry->ipMask;
				}
			}

			if (entry->isHigh)
			{
				rtl8651_addFlowToUnlimitedQueue("eth1", &rule, entry->ratio);
			}else
			{
				rtl8651_addFlowToLimitedQueue("eth1", &rule, entry->ratio);
			}
		}
	}
}







void asp_setRatio_qos(webs_t wp, char_t *path, char_t *query)
{
	char buff[100];
	int i;
	int reArrange;

	pRomeCfgParam->ratioQosCfgParam.enable = atoi(websGetVar(wp,"enable",""));

	/* modify pRomeCfgParam */
	reArrange = FALSE;
	if (strcmp(websGetVar(wp,"deleteSelected", ""),"Delete Selected") == 0)
	{
		for (i = 0 ; i < MAX_RATIO_QOS ; i ++ )
		{
			if (pRomeCfgParam->ratioQosCfgParam.entry[i].enable)
			{
				sprintf(buff, "disable_%d", i);
				/* check if this entry been deleted or not */
				if (atoi(websGetVar(wp, buff, "")))
				{	/* this entry is deleted */
					reArrange = TRUE;	/* we need to rearrange data */
					if (pRomeCfgParam->ratioQosCfgParam.entry[i].isHigh)
					{
						pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_h += pRomeCfgParam->ratioQosCfgParam.entry[i].ratio;
					}else
					{
						pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_l += pRomeCfgParam->ratioQosCfgParam.entry[i].ratio;
					}
					pRomeCfgParam->ratioQosCfgParam.entry[i].enable = 0;
				}
			}
		}
	}else if (strcmp(websGetVar(wp,"deleteAll", ""), "Delete All") == 0)
	{
		for (i = 0 ; i < MAX_RATIO_QOS ; i ++)
		{
			if (pRomeCfgParam->ratioQosCfgParam.entry[i].enable)
			{
				if (pRomeCfgParam->ratioQosCfgParam.entry[i].isHigh)
					pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_h += pRomeCfgParam->ratioQosCfgParam.entry[i].ratio;
				else
					pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_l += pRomeCfgParam->ratioQosCfgParam.entry[i].ratio;
			}
		}
		memset(pRomeCfgParam->ratioQosCfgParam.entry, 0 , (sizeof(ratioQosEntry_t) * MAX_RATIO_QOS));
	}else if (strcmp(websGetVar(wp,"update", ""),"Apply") == 0)
	{
		pRomeCfgParam->ratioQosCfgParam.upInfo.maxRate = atoi(websGetVar(wp, "uprate", ""));
		pRomeCfgParam->ratioQosCfgParam.upInfo.maxRateUnit = atoi(websGetVar(wp, "uprateUnit", ""));

		if (pRomeCfgParam->ratioQosCfgParam.upInfo.maxRate == 0)
		{
			pRomeCfgParam->ratioQosCfgParam.upInfo.maxRate = 100;
			pRomeCfgParam->ratioQosCfgParam.upInfo.maxRateUnit = 0;
		}else if ((pRomeCfgParam->ratioQosCfgParam.upInfo.maxRateUnit == 1) &&
					(pRomeCfgParam->ratioQosCfgParam.upInfo.maxRate < 128))
		{	/* min check */
			pRomeCfgParam->ratioQosCfgParam.upInfo.maxRate = 128;
		}else if ((pRomeCfgParam->ratioQosCfgParam.upInfo.maxRateUnit == 0) &&
					(pRomeCfgParam->ratioQosCfgParam.upInfo.maxRate > 100))
		{	/* max check */
			pRomeCfgParam->ratioQosCfgParam.upInfo.maxRate = 100;
		}
	}else if (pRomeCfgParam->ratioQosCfgParam.enable)
	{	/* add new */
		ratioQosEntry_t *ptr;
		uint8 IPoct[4];
		uint8 Mskoct[4];
		uint32 ip;
		uint32 mask;
		uint8 qSel, ratio;

		/* check input */
		for (i = 0 ; i < 4 ; i ++)
		{
			sprintf(buff, "ip%d", i);
			IPoct[i] = atoi(websGetVar(wp, buff, ""));
			sprintf(buff, "mask%d", i);
			Mskoct[i] = atoi(websGetVar(wp, buff, ""));

			if ((IPoct[i] > 255 || Mskoct[i] > 255))
				goto out;
		}
		ip = (IPoct[0] * (256 * 256 * 256)) + (IPoct[1] * (256 * 256)) + (IPoct[2] * 256) + IPoct[3];
		mask = (Mskoct[0] * (256 * 256 * 256)) + (Mskoct[1] * (256 * 256)) + (Mskoct[2] * 256) + Mskoct[3];

		ratio = atoi(websGetVar(wp, "ratio", ""));	/* ratio */

		if (ratio == 0)
			goto out;

		if (pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_l &&
			pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_h)
		{
			qSel = atoi(websGetVar(wp,"queue", ""));	/* queue select */
		}else if (pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_h)
		{	/* high only */
			qSel = 1;
		}else
		{	/* low only */
			qSel = 0;
		}
		
		if (qSel)
		{	/* high */
			if (pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_h < ratio)
				goto out;				
		}else
		{	/* low */
			if (pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_l < ratio)
				goto out;	
		}

		ptr = pRomeCfgParam->ratioQosCfgParam.entry;
		for (i = 0 ; i < MAX_RATIO_QOS ; i ++ )
		{
			if (!ptr[i].enable)
			{	/* empty entry found : set entry */
				if (qSel)
				{	/* high */
					ptr[i].isHigh = TRUE;
					ptr[i].ratio = ratio;
					pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_h -= ratio;
				} else
				{	/* low */
					ptr[i].isHigh = FALSE;
					ptr[i].ratio = ratio;
					pRomeCfgParam->ratioQosCfgParam.upInfo.remainingRatio_l -= ratio;
				}
				ptr[i].isSrc		= atoi(websGetVar(wp, "isSrc", ""));
				ptr[i].ip			= ip;
				ptr[i].ipMask		= mask;
				ptr[i].protoType	= atoi(websGetVar(wp, "protoType", ""));
				ptr[i].s_port		= atoi(websGetVar(wp, "s_port", ""));
				ptr[i].e_port		= atoi(websGetVar(wp, "e_port", ""));
				ptr[i].enable		= TRUE;

				reArrange = TRUE;		/* we need to rearrange data */

				goto startReArrange;
			}
		}
	}
startReArrange:
	/* rearrange data if we need */
	if (reArrange == TRUE)
	{
		ratioQosEntry_t *ptr;
		ptr = pRomeCfgParam->ratioQosCfgParam.entry;

		for ( i = 0 ; i < MAX_RATIO_QOS ; i ++ )
		{
			if (!ptr[i].enable)
			{	/* this entry is empty, check non-empty entry behind it and move to this entry */
				int j = i + 1;
				while ((j < MAX_RATIO_QOS) && (!ptr[j].enable))
					j ++;
				if (j == MAX_RATIO_QOS)
					goto ArrangeDone;
				/* non-empty entry found */
				if (!ptr[j].enable)
					printf("Error: ratio QoS reArrange error!\n");
				else
				{	/* mv entry from j to i */
					memcpy(&(ptr[i]), &(ptr[j]), sizeof(ratioQosEntry_t));
					/* clear entry j */
					memset(&(ptr[j]), 0, sizeof(ratioQosEntry_t));
				}
			}
		}
	ArrangeDone:
	}


out:
	/* update tblDrv */
	if (pRomeCfgParam->ratioQosCfgParam.enable == 0)
	{	/* disable Ratio based QoS System */
		rearrange_Ratio_Qos(TRUE);
	}else
	{
		rearrange_Ratio_Qos(FALSE);
	}

	cfgmgr_write(CFGMGR_TABID_RATIOQOS,(void*)&pRomeCfgParam->ratioQosCfgParam, sizeof(ratioQosCfgParam_t));
	cfgmgr_task();
	websRedirect(wp, "ratio_qos.asp");

}

int asp_rateLimit(int ejid, webs_t wp, int argc, char **argv)
{
	uint32 entry;

	websWrite(wp, "<SCRIPT LANGUAGE=\"JavaScript\">\n");
	websWrite(wp, "function rateLimit_FormSwitch() {\n");
	websWrite(wp, "	if (rateLimit_Form.ab_switch.checked == false) { \n");
	for(entry=0; entry<16; entry++) {
		websWrite(wp,
				"rateLimit_Form.srcDst%d.disabled=true;\n"\
				"rateLimit_Form.ip%d.disabled=true;\n"\
				"rateLimit_Form.ipMask%d.disabled=true;\n"\
				"rateLimit_Form.proto%d.disabled=true;\n"\
				"rateLimit_Form.sport%d.disabled=true;\n"\
				"rateLimit_Form.eport%d.disabled=true;\n"\
				"rateLimit_Form.rate%d.disabled= true;\n"\
				"rateLimit_Form.isByteCount%d.disabled=true;\n"\
				"rateLimit_Form.maxRate%d.disabled=true;\n"\
				"rateLimit_Form.ruleAction%d.disabled=true;\n"\
				"rateLimit_Form.apply_entry%d.disabled=true;\n",
				entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry
		);
	}
	websWrite(wp, "} else { \n");
	for(entry=0; entry<16; entry++) {
		websWrite(wp,
				"rateLimit_Form.srcDst%d.disabled=false;\n"\
				"rateLimit_Form.ip%d.disabled=false;\n"\
				"rateLimit_Form.ipMask%d.disabled=false;\n"\
				"rateLimit_Form.proto%d.disabled=false;\n"\
				"rateLimit_Form.sport%d.disabled=false;\n"\
				"rateLimit_Form.eport%d.disabled=false;\n"\
				"rateLimit_Form.rate%d.disabled=false;\n"\
				"rateLimit_Form.isByteCount%d.disabled=false;\n"\
				"rateLimit_Form.maxRate%d.disabled=false;\n"\
				"rateLimit_Form.ruleAction%d.disabled=false;\n"\
				"rateLimit_Form.apply_entry%d.disabled=false; \n",
				entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry
		);
	}
	websWrite(wp, "}\n\n}function onload_func() { rateLimit_FormSwitch(); }\n");
	websWrite(wp, "</SCRIPT>\n"\
				 "</head>\n"\
				 "<body bgcolor=\"#ffffff\" text=\"#000000\" onload=\"onload_func()\">\n"
	);

	
	websWrite(wp,
		"<form method=\"post\" action=\"/goform/asp_setRateLimit\" name=rateLimit_Form>\n"\
		"<tr><td><b>Hardware Rate Policing QoS</b><td bgColor=#ddeeff><input type=checkbox name=ab_switch %s value=1 onClick=\"rateLimit_FormSwitch();\">Enable</td></td></tr>\n"\
		"<br>\n"\
		"<table cellSpacing=1 cellPadding=2 border=0>\n"\
		"<tr><td><b>LAN</b></td></tr>"\
		"<tr><td bgColor=#aaddff>Priority</td>\n"\
		"	<td bgColor=#aaddff>src/dst</td>\n"\
		"	<td bgColor=#aaddff>IP Address</td>\n"\
		"	<td bgColor=#aaddff>IP Mask</td>\n"\
		"	<td bgColor=#aaddff>Protocol</td>\n"\
		"	<td bgColor=#aaddff>Start Port</td>\n"\
		"	<td bgColor=#aaddff>End Port</td>\n"\
		"	<td bgColor=#aaddff>Rate</td>\n"\
		"	<td bgColor=#aaddff>Unit</td>\n"\
		"	<td bgColor=#aaddff>isByteCount</td>\n"\
		"	<td bgColor=#aaddff>Max Rate</td>\n"\
		"	<td bgColor=#aaddff>Unit</td>\n"\
		"	<td bgColor=#aaddff>Drop Log</td>\n"\
		"	<td bgColor=#aaddff>Enable</td>\n"\
		"</tr>\n",
		((pRomeCfgParam->rateLimitCfgParam.enable)? "checked": "")
	);

	for(entry=0; entry<16; entry++) {
		websWrite(wp,
			"<tr><td bgColor=#aaddff>%u</td>\n"\
			"	<td bgColor=#ddeeff align=center><select name=srcDst%d><option %s>src IP<option %s>dst IP</selected></td>\n"\
			"	<td bgColor=#ddeeff align=center><input type=text name=ip%d size=15 value=\"%u.%u.%u.%u\"></td>\n"\
			"	<td bgColor=#ddeeff align=center><input type=text name=ipMask%d size=15 value=\"%u.%u.%u.%u\"></td>\n"\
			"	<td bgColor=#ddeeff align=center><select name=proto%d><option %s>TCP<option %s>UDP<option %s>IP</selected></td>\n"\
			"	<td bgColor=#ddeeff align=center><input type=text name=sport%d size=5 value=%u></td>\n"\
			"	<td bgColor=#ddeeff align=center><input type=text name=eport%d size=5 value=%u></td>\n",
			entry%8, 
			entry, ((pRomeCfgParam->rateLimitCfgParam.entry[entry].isSrcIp==TRUE)? "selected": ""), ((pRomeCfgParam->rateLimitCfgParam.entry[entry].isSrcIp==FALSE)? "selected": ""),
			entry, NIPQUAD(pRomeCfgParam->rateLimitCfgParam.entry[entry].ip) ,
			entry, NIPQUAD(pRomeCfgParam->rateLimitCfgParam.entry[entry].ipMask) ,
			entry, ((pRomeCfgParam->rateLimitCfgParam.entry[entry].protoType==0)? "selected": ""), ((pRomeCfgParam->rateLimitCfgParam.entry[entry].protoType==1)? "selected": ""), ((pRomeCfgParam->rateLimitCfgParam.entry[entry].protoType==2)? "selected": ""),
			entry, pRomeCfgParam->rateLimitCfgParam.entry[entry].s_port,
			entry, pRomeCfgParam->rateLimitCfgParam.entry[entry].e_port
		);
		websWrite(wp, 
			"	<td bgColor=#ddeeff align=center><input type=text name=rate%d size=6 value=%u></td>\n"\
			"	<td bgColor=#ddeeff align=center><select name=rateUnit%d><option %s>Kbps<option %s>Mbps</selected></td>\n"\
			"	<td bgColor=#ddeeff align=center><select name=isByteCount%d><option %s>byteCount<option %s>pktCount</selected></td>\n"\
			"	<td bgColor=#ddeeff align=center><input type=text name=maxRate%d size=6 value=%u></td>\n"\
			"	<td bgColor=#ddeeff align=center><select name=maxRateUnit%d><option %s> Kbps<option %s>Mbps</selected></td>\n"\
			"	<td bgColor=#ddeeff align=center><select name=ruleAction%d><option %s>DropLog<option %s>Drop</selected></td>\n"\
			"	<td bgColor=#ddeeff align=center><input type=checkbox name=apply_entry%d value=1 %s></td>\n"\
			"</tr>\n", 
			entry, pRomeCfgParam->rateLimitCfgParam.entry[entry].rate,
			entry, ((pRomeCfgParam->rateLimitCfgParam.entry[entry].rateUnit==0)? "selected": ""), ((pRomeCfgParam->rateLimitCfgParam.entry[entry].rateUnit==1)? "selected": ""),
			entry, ((pRomeCfgParam->rateLimitCfgParam.entry[entry].isByteCount==TRUE)? "selected": ""), ((pRomeCfgParam->rateLimitCfgParam.entry[entry].isByteCount==FALSE)? "selected": ""),
			entry, pRomeCfgParam->rateLimitCfgParam.entry[entry].maxRate,
			entry, ((pRomeCfgParam->rateLimitCfgParam.entry[entry].maxRateUnit==0)? "selected": ""), ((pRomeCfgParam->rateLimitCfgParam.entry[entry].maxRateUnit==1)? "selected": ""),
			entry, ((pRomeCfgParam->rateLimitCfgParam.entry[entry].isDropLog==TRUE)? "selected": ""), ((pRomeCfgParam->rateLimitCfgParam.entry[entry].isDropLog==FALSE)? "selected": ""),
			entry, ((pRomeCfgParam->rateLimitCfgParam.entry[entry].enable==TRUE)? "checked": "")
		);

		if (entry== 7) {
			websWrite(wp, 
				"<tr><td><b>WAN</b></td></tr>\n"\
				"<tr><td bgColor=#aaddff>Priority</td>\n"\
				"	<td bgColor=#aaddff>src/dst</td>\n"\
				"	<td bgColor=#aaddff>IP Address</td>\n"\
				"	<td bgColor=#aaddff>IP Mask</td>\n"\
				"	<td bgColor=#aaddff>Protocol</td>\n"\
				"	<td bgColor=#aaddff>Start Port</td>\n"\
				"	<td bgColor=#aaddff>End Port</td>\n"\
				"	<td bgColor=#aaddff>Rate</td>\n"\
				"	<td bgColor=#aaddff>Unit</td>\n"\
				"	<td bgColor=#aaddff>isByteCount</td>\n"\
				"	<td bgColor=#aaddff>Max Rate</td>\n"\
				"	<td bgColor=#aaddff>Unit</td>\n"\
				"	<td bgColor=#aaddff>Drop Log</td>\n"\
				"	<td bgColor=#aaddff>Enable</td></tr>\n"
			);		
		}
	}
	
	websWrite(wp,
			"<tr>\n"\
			"<td colspan=2 align=center>\n"\
			"<input type=submit value=Apply>\n"\
			"<input type=reset value=Reset>\n"\
			"</td>\n"\
		   	"</tr>\n"\
			"</table>\n"\
			"</form>\n"
	);
	return 0;
}


int asp_addRateLimitRule(rateLimitEntry_t *rlrule_t, uint32 entry)
{
	int8 *ifName[] = { "eth1", "eth0" };
	rtl8651_tblDrvAclRule_t rule;
	uint32 ifIndex, rate, maxRate;
	

	/* Add a rule to Rome Driver */
	if (rlrule_t->rateUnit == 0)
		rate = (1000 * rlrule_t->rate) >> 3;
	else rate = (1000 * 1000 * rlrule_t->rate) >> 3;
	if (rlrule_t->maxRateUnit == 0)
		maxRate = (1000 * rlrule_t->maxRate) >> 3;
	else maxRate = (1000 * 1000 *rlrule_t->maxRate) >> 3;
	
	ifIndex = entry >> 3;
	rtl8651_addRateLimitGroup(1000+entry, ifName[ifIndex], rate, maxRate, rlrule_t->isByteCount);
	bzero(&rule, sizeof(rtl8651_tblDrvAclRule_t));

	/* Action Type */
	if (rlrule_t->isDropLog == TRUE)
		rule.actionType_ = RTL8651_ACL_DROP_LOG;
	else rule.actionType_ = RTL8651_ACL_DROP;
	
	/* rule Type */
	switch (rlrule_t->protoType) {

	case 0: /* TCP */
		rule.ruleType_	= RTL8651_ACL_TCP;
		if (rlrule_t->isSrcIp == TRUE) {
			rule.tcpSrcPortUB_	= rlrule_t->e_port;
			rule.tcpSrcPortLB_	= rlrule_t->s_port;
			rule.tcpDstPortUB_	= 65535;
			rule.tcpDstPortLB_	= 0;
		}
		else {
			rule.tcpDstPortUB_	= rlrule_t->e_port;
			rule.tcpDstPortLB_	= rlrule_t->s_port;
			rule.tcpSrcPortUB_	= 65535;
			rule.tcpSrcPortLB_	= 0;
		}
		goto comm_L3;

	case 1: /* UDP */
		rule.ruleType_	= RTL8651_ACL_UDP;
		if (rlrule_t->isSrcIp == TRUE) {
			rule.udpSrcPortUB_		= rlrule_t->e_port;
			rule.udpSrcPortLB_		= rlrule_t->s_port;
			rule.udpDstPortUB_		= 65535;
			rule.udpDstPortLB_		= 0;
		}
		else {
			rule.udpDstPortUB_		= rlrule_t->e_port;
			rule.udpDstPortLB_		= rlrule_t->s_port;
			rule.udpSrcPortUB_		= 65535;
			rule.udpSrcPortLB_		= 0;
		}
		goto comm_L3;

	case 2: /* IP */
		rule.ruleType_ = RTL8651_ACL_IP;
comm_L3:
		if (rlrule_t->isSrcIp == TRUE) {
			rule.srcIpAddr_		= rlrule_t->ip;
			rule.srcIpAddrMask_	= rlrule_t->ipMask;
		}
		else {
			rule.dstIpAddr_		= rlrule_t->ip;
			rule.dstIpAddrMask_	= rlrule_t->ipMask;
		}
		break;
	
	default: return 1;
	}
	rtl8651_addRateLimitRule(&rule, 1000+entry);
	return 0;
}



int asp_setRateLimit(webs_t wp, char_t *path, char_t *query)
{
	uint8 buf[20];
	uint32 entry, iph1, iph2, iph3, iph4;
	int8 *str;

	str = websGetVar(wp,"ab_switch","");
	if (strcmp(str, "1") == 0) 
		pRomeCfgParam->rateLimitCfgParam.enable = TRUE;
	else pRomeCfgParam->rateLimitCfgParam.enable = FALSE;
	
	rtl8651_flushRateLimitGroup("eth0");
	rtl8651_flushRateLimitGroup("eth1");

	for(entry=0; entry<16&&pRomeCfgParam->rateLimitCfgParam.enable ==TRUE; entry++) {	
		sprintf(buf, "apply_entry%d", entry);
		pRomeCfgParam->rateLimitCfgParam.entry[entry].enable = atoi(websGetVar(wp, buf," "));
		
		/* Get Destion/Source IP */
		sprintf(buf, "srcDst%d", entry);
		str = websGetVar(wp,T(buf)," ");
		if (strcmp(str, "src IP") == 0)
			pRomeCfgParam->rateLimitCfgParam.entry[entry].isSrcIp = TRUE;
		else pRomeCfgParam->rateLimitCfgParam.entry[entry].isSrcIp = FALSE;

		/* Get IP Address */
		sprintf(buf, "ip%d", entry);
		str = websGetVar(wp,T(buf)," ");
		sscanf(str, "%u.%u.%u.%u", 	&iph1, &iph2, &iph3, &iph4);
 		pRomeCfgParam->rateLimitCfgParam.entry[entry].ip = ((iph1<<24) | iph2<<16 | iph3<<8 | iph4);

 		/* Get IP Mask */
 		sprintf(buf, "ipMask%d", entry);
 		str = websGetVar(wp,T(buf)," ");
		sscanf(str, "%u.%u.%u.%u", 	&iph1, &iph2, &iph3, &iph4);
 		pRomeCfgParam->rateLimitCfgParam.entry[entry].ipMask = ((iph1<<24) | iph2<<16 | iph3<<8 | iph4);

		/* Get Protocol */
		sprintf(buf, "proto%d", entry);
		str = websGetVar(wp,T(buf)," ");
		if (strcmp(str, "TCP") == 0)
			pRomeCfgParam->rateLimitCfgParam.entry[entry].protoType = 0;
		else if (strcmp(str, "UDP") == 0)
			pRomeCfgParam->rateLimitCfgParam.entry[entry].protoType = 1;
		else pRomeCfgParam->rateLimitCfgParam.entry[entry].protoType = 2;
		
		/* Get start Port */
		sprintf(buf, "sport%d", entry);
		pRomeCfgParam->rateLimitCfgParam.entry[entry].s_port = atoi(websGetVar(wp, T(buf), " "));
		/* Get Ending Port */
		sprintf(buf, "eport%d", entry);
		pRomeCfgParam->rateLimitCfgParam.entry[entry].e_port = atoi(websGetVar(wp, T(buf), " "));

		/* Get rate */
		sprintf(buf, "rate%d", entry);
		pRomeCfgParam->rateLimitCfgParam.entry[entry].rate = atoi(websGetVar(wp, T(buf), " "));

		/* Get rate unit */
		sprintf(buf, "rateUnit%d", entry);
		str = websGetVar(wp,T(buf)," ");
		if (strcmp(str, "Kbps") == 0)
			pRomeCfgParam->rateLimitCfgParam.entry[entry].rateUnit = 0;
		else pRomeCfgParam->rateLimitCfgParam.entry[entry].rateUnit = 1;
		

		/* Get ByteCount */
		sprintf(buf, "isByteCount%d", entry);
		str = websGetVar(wp,T(buf)," ");
		if (strcmp(str, "byteCount") == 0)
			pRomeCfgParam->rateLimitCfgParam.entry[entry].isByteCount = TRUE;
		else pRomeCfgParam->rateLimitCfgParam.entry[entry].isByteCount = FALSE;

		/* Get Max Rate */
		sprintf(buf, "maxRate%d", entry);
		pRomeCfgParam->rateLimitCfgParam.entry[entry].maxRate = atoi(websGetVar(wp, T(buf), " "));

		/* Get Max rate unit */
		sprintf(buf, "maxRateUnit%d", entry);
		str = websGetVar(wp,T(buf)," ");
		if (strcmp(str, "Kbps") == 0)
			pRomeCfgParam->rateLimitCfgParam.entry[entry].maxRateUnit= 0;
		else pRomeCfgParam->rateLimitCfgParam.entry[entry].maxRateUnit= 1;

		/* Get Action */
		sprintf(buf, "ruleAction%d", entry);
		str = websGetVar(wp,T(buf)," ");
		if (strcmp(str, "Drop") == 0)
			pRomeCfgParam->rateLimitCfgParam.entry[entry].isDropLog = FALSE;
		else pRomeCfgParam->rateLimitCfgParam.entry[entry].isDropLog = TRUE;

		if (pRomeCfgParam->rateLimitCfgParam.entry[entry].enable == TRUE &&
		    pRomeCfgParam->rateLimitCfgParam.enable == TRUE)
			asp_addRateLimitRule(&pRomeCfgParam->rateLimitCfgParam.entry[entry], entry);		
	}

	cfgmgr_write(CFGMGR_TABID_RATELIMIT,(void*)&pRomeCfgParam->rateLimitCfgParam, sizeof(struct rateLimitCfgParam_s));
	cfgmgr_task();
	websRedirect(wp, "ratelimit.asp");
	return 0;
}








int asp_port(int ejid, webs_t wp, int argc, char_t **argv)
{
	int i, type, enable;
	websWrite(wp,
	"<tr><td bgColor=#aaddff>Port <br>Number</td><td bgColor=#aaddff>Interface</td><td bgColor=#aaddff>On/Off</td></tr>\n");

	for(i=0;i<6;i++)
	{
		type=-1;//Unknown
		enable=0;
		if(pRomeCfgParam->vlanCfgParam[0].enablePorts & (1<<i))
			enable=1; //This port is open
		else if(pRomeCfgParam->vlanCfgParam[1].enablePorts & (1<<i))
			enable=1; //This port is closed
		if(enable){
			if(pRomeCfgParam->vlanCfgParam[0].memberPort & (1<<i))
				type=1; //This port is config as WAN
			else if(pRomeCfgParam->vlanCfgParam[1].memberPort & (1<<i))
				type=0; //This port is config as LAN
		}
		websWrite(wp,
		"<tr>\n" \
		"<td bgColor=#ddeeff>\n" \
		"		%d%s\n" \
		"	</td>\n" \
		"<td bgColor=#ddeeff><select name=interface%d %s onChange=\"selectSwitch();\"><option value=0 %s>LAN<option value=1 %s>WAN</select></td>\n" \
		"<td bgColor=#ddeeff><select name=enable%d  onChange=\"selectSwitch();\"><option value=0 %s>Off<option value=1 %s>ON</select></td>\n" \
		"</td>\n" \
		"</tr>\n" ,i,i==5?"(MII)":"",i,enable?"":"disabled",type==0?"selected":""
				,type==1?"selected":"", i,enable==0?"selected":"",enable==1?"selected":"");
	}

	return 0;
}

void asp_setPort(webs_t wp, char_t *path, char_t *query)
{
	int i,ret;
	char	 tmpstr[32]={0};
	
	for(i=0;i<5;i++)
	{
		sprintf(tmpstr,"enable%d",i);
		if(atoi(websGetVar(wp,tmpstr,""))==0){
			pRomeCfgParam->vlanCfgParam[0].memberPort &=~(1<<i);//clear it
			pRomeCfgParam->vlanCfgParam[1].memberPort &=~(1<<i);//clear it
			pRomeCfgParam->vlanCfgParam[0].enablePorts &=~(1<<i);//clear it
			pRomeCfgParam->vlanCfgParam[1].enablePorts &=~(1<<i);//clear it
		}else{
			pRomeCfgParam->vlanCfgParam[0].enablePorts |=(1<<i);
			pRomeCfgParam->vlanCfgParam[1].enablePorts |=(1<<i);
			sprintf(tmpstr,"interface%d",i);			
			if(atoi(websGetVar(wp,tmpstr,""))){
				//set this port as WAN
				if(pRomeCfgParam->vlanCfgParam[1].memberPort & (1<<i)) //was a LAN port
					pRomeCfgParam->vlanCfgParam[1].memberPort &=~(1<<i);//clear it
				pRomeCfgParam->vlanCfgParam[0].memberPort |=(1<<i);//now config as WAN
			}else{
				//set this port as LAN
				if(pRomeCfgParam->vlanCfgParam[0].memberPort & (1<<i)) //was a WAN port
					pRomeCfgParam->vlanCfgParam[0].memberPort &=~(1<<i);//clear it
				pRomeCfgParam->vlanCfgParam[1].memberPort |=(1<<i);//now config as LAN
			}
		}
	}
	websRedirect(wp, T("lan.asp"));

}


int asp_getLanPortStatus(int ejid, webs_t wp, int argc, char_t **argv){
	int i;
	int up, speed, duplex, range;
	unsigned counters[6];
	for(i=0;i<6;i++)
	{
		if(pRomeCfgParam->vlanCfgParam[1].enablePorts & (1<<i)){
			if(pRomeCfgParam->vlanCfgParam[1].memberPort & (1<<i)){
				if(i==5){
					websWrite(wp,
					"<tr><td bgColor=#aaddff align=center>Port 5(MII)</td><td bgColor=#ddeeff>"\
					"Link is Up</td></tr>"
					);
					continue;
				}
				rtl8651_checkEthernetPortStatus(i,&up, &speed, &duplex, &range);
				if(up){
					websWrite(wp,
					"<tr><td bgColor=#aaddff align=center>Port %d</td><td bgColor=#ddeeff>"\
					"Link is Up. %sMbps, %s duplex"\
					"</td></tr>",  i, speed? "100":"10", duplex?"full-":"half-"
					);
				}else{
					char str[80];
					if(range&0x400){
						range&=0x1ff;
						if(range)
							sprintf(str,"<font color=red>Cable short at %d.%d meters away</font>",range>>1, range&1?5:0);
						else
							sprintf(str, "No cable detected");
					}else if(range&0x200){
						range&=0x1ff;
						if(range)
							sprintf(str,"<font color=red>Cable open %d.%d meters away</font>",range>>1, range&1?5:0);
						else
							sprintf(str, "No cable detected");
					}else
						sprintf(str, "No cable detected");
					websWrite(wp,
					"<tr><td bgColor=#aaddff align=center>Port %d</td><td bgColor=#ddeeff>"\
					"Link is down. %s</td></tr>",  i, str);
				}
			}
		}
	}

	rtl8651_getIntfCounters(0x2, &counters);
	websWrite(wp,"<tr><td bgColor=#aaddff>Received</td><td bgColor=#ddeeff>%d packets, %d bytes</td></tr>",
		counters[1],counters[0]);
	websWrite(wp,"<tr><td bgColor=#aaddff>Transmitted</td><td bgColor=#ddeeff>%d packets, %d bytes</td></tr>",
		counters[3],counters[2]);
	websWrite(wp,"<tr><td bgColor=#aaddff>Dropped</td><td bgColor=#ddeeff>%d packets</td></tr>",
		counters[5]);

	return 0;
}


int asp_getWanPortStatus(int ejid, webs_t wp, int argc, char_t **argv){
	int i;
	int up, speed, duplex, range;
	unsigned counters[6];	
	for(i=0;i<6;i++)
	{
		if(pRomeCfgParam->vlanCfgParam[0].enablePorts & (1<<i)){
			if(pRomeCfgParam->vlanCfgParam[0].memberPort & (1<<i)){
				if(i==5){
					websWrite(wp,
					"<tr><td bgColor=#aaddff align=center>Port 5(MII)</td><td bgColor=#ddeeff>"\
					"Link is Up</td></tr>"
					);
					continue;
				}
				rtl8651_checkEthernetPortStatus(i,&up, &speed, &duplex, &range);
				if(up){
					websWrite(wp,
					"<tr><td bgColor=#aaddff align=center>Port %d</td><td bgColor=#ddeeff>"\
					"Link is Up. %sMbps, %s duplex"\
					"</td></tr>",  i, speed? "100":"10", duplex?"full-":"half-"
					);
				}else{
					char str[32];
					if(range&0x400){
						range&=0x1ff;
						if(range)
							sprintf(str,"Cable short at %d.%d meters away",range>>1, range&1?5:0);
						else
							sprintf(str, "No cable detected");
					}else if(range&0x200){
						range&=0x1ff;
						if(range)
							sprintf(str,"Cable short at %d.%d meters away",range>>1, range&1?5:0);
						else
							sprintf(str, "No cable detected");
					}
					websWrite(wp,
					"<tr><td bgColor=#aaddff align=center>Port %d</td><td bgColor=#ddeeff>"\
					"Link is down. %s</td></tr>",  i, str);
				}
			}
		}
	}
	rtl8651_getIntfCounters(0x1, &counters);	
	websWrite(wp,"<tr><td bgColor=#aaddff>Received</td><td bgColor=#ddeeff>%d packets, %d bytes</td></tr>",
		counters[1],counters[0]);
	websWrite(wp,"<tr><td bgColor=#aaddff>Transmitted</td><td bgColor=#ddeeff>%d packets, %d bytes</td></tr>",
		counters[3],counters[2]);
	websWrite(wp,"<tr><td bgColor=#aaddff>Dropped</td><td bgColor=#ddeeff>%d packets</td></tr>",
		counters[5]);

	return 0;
}


/************************************************************
 *	
 *	Free-style flash editor
 *	
 ************************************************************/ 
int asp_flash(int ejid, webs_t wp, int argc, char_t **argv)
{
	/* null output */
	websWrite(wp,"" );

	return 0;
}

int lengthAfterUrldecode( const char* pstr )
{
	int i, numOfPercent, orgLen;

	orgLen = strlen( pstr );
	numOfPercent = 0;
	for( i = 0; i < orgLen; i++ )
	{
		if ( pstr[i] == '%' ) 
		{
			numOfPercent++;
			i += 2;
		}
	}

	return orgLen - numOfPercent*2;
}

int hex2dec( char ch )
{
	if ( ch >= '0' && ch <= '9' ) return ch - '0';
	if ( ch >= 'a' && ch <= 'f' ) return ch - 'a' + 10;
	if ( ch >= 'A' && ch <= 'F' ) return ch - 'A' + 10;
	return 0;
}

void urldecode( char *dst, char *src, int nSize )  // nSize is the size of *dst.
{
	
	int i, j, orgLen;

	orgLen = strlen( src );
	for( i = 0, j = 0; j < nSize; i++, j++ )
	{
		if ( src[i] == '\0' )
		{
			break;
		}
		else if ( src[i] == '%' )
		{
			dst[j] = hex2dec( src[i+1] ) * 16 + hex2dec( src[i+2] );
			i += 2;
		}
		else
		{
			dst[j] = src[i];
		}
	}
}

void asp_setFlash(webs_t wp, char_t *path, char_t *query)
{
	int startOffset, endOffset;
	int firstBlockOffset, lastBlockOffset;
	unsigned char *pStartOffset = NULL;
	unsigned char *pContent = NULL;
	unsigned char *pContentOrg = NULL;
	unsigned char *pBuffer = NULL;
	int length = 0;
	uint32 lastBlockSize, totalBlockSize;
	int	i;

	pStartOffset = websGetVar(wp,"so","");
	if ( pStartOffset == NULL )
	{
		websWrite( wp, "pStartOffset is NULL" );
		goto out;
	}
	websWrite( wp, "pStartOffset=[%s]<BR>\n", pStartOffset );
	if ( !strncmp( pStartOffset, "0x", 2 ) )
	{
		// Heximal
		char* p = pStartOffset + 2;
		startOffset = 0;

		for( ; *p; p++ )
		{
			startOffset = startOffset * 16 + hex2dec( *p );
		}
	}
	else
	{
		startOffset = atoi( pStartOffset );
	}
	// websWrite( wp, "startOffset=0x%08x<BR>\n", startOffset );

	pContentOrg = websGetVar(wp,"co","");
	if ( pContentOrg == NULL )
	{
		websWrite( wp, "pContentOrg is NULL" );
		goto out;
	}
	// websWrite( wp, "pContentOrg=[%s]<BR>\n", pContentOrg );
	length = lengthAfterUrldecode( pContentOrg );
	endOffset = startOffset + length - 1;
	// websWrite( wp, "length=%d<BR>\n", length );

	pContent = malloc( length );
	if ( pContent == NULL )
	{
		websWrite( wp, "pContent is NULL" );
		goto out;
	}
	
	urldecode( pContent, pContentOrg, length );

	//
	// Content may be across several blocks, so we must find out the first and the last block.
	//
	if ( flashdrv_getBlockSizeAndBlockOffset( startOffset, 0, NULL, &firstBlockOffset, NULL ) != 0 )
	{
		websWrite(wp,"<BR>\nflashdrv_getBlockSizeAndOffsetBase( startOffset=0x%08x ) returns FAILED.\n");
		goto out;
	}
	if ( flashdrv_getBlockSizeAndBlockOffset( endOffset, 0, NULL, &lastBlockOffset, &lastBlockSize ) != 0 )
	{
		websWrite(wp,"<BR>\nflashdrv_getBlockSizeAndOffsetBase( startOffset=0x%08x ) returns FAILED.\n");
		goto out;
	}

	// calculate buffer size
	totalBlockSize = ( lastBlockOffset + lastBlockSize ) - firstBlockOffset;

	{
		// OK
		pBuffer = malloc( totalBlockSize );
		
		// read into buffer
		flashdrv_read( pBuffer, flashdrv_getBlockBase(0)+firstBlockOffset, totalBlockSize );

		// modify buffer
		memcpy( pBuffer + ( startOffset - firstBlockOffset ), pContent, length );

		websWrite( wp, "asp_setFlash(): so=0x%08x co=", flashdrv_getBlockBase(0)+startOffset );
		if ( pContent ) for( i = 0; i < length; i++ ) websWrite( wp, "%02x-", pContent[i] );

		// websWrite( wp, "<BR>\n----------------------------------------------<BR>\n" );
		websWrite( wp, "<BR>\nflashdrv_updateImg(src=0x%08x,dst=0x%08x,size=0x%08x)<BR>\n",(uint32)pBuffer,(uint32)(flashdrv_getBlockBase(0)+firstBlockOffset), totalBlockSize );
		// if ( pBuffer ) for( i = 0; i < totalBlockSize; i++ ) websWrite( wp, "%02x-", pBuffer[i] );
		// websWrite( wp, "<BR>\n----------------------------------------------<BR>\n" );

#ifdef _SUPPORT_LARGE_FLASH_
		/* Load bdinfo first */
		flashdrv_read((void*)&bdinfo,(void*)FLASH_MAP_BOARD_INFO_ADDR,sizeof(bdinfo));
#endif/*_SUPPORT_LARGE_FLASH_*/

		// flush buffer to flash
		if ( flashdrv_updateImg( pBuffer, flashdrv_getBlockBase(0)+firstBlockOffset, totalBlockSize ) == 0 )
		{
			websWrite(wp,"<BR>\nUpdate OK" );
		}
		else
		{
			websWrite(wp,"<BR>\nUpdate failed. flashdrv_updateImg() return FAILED.\n" );
		}
	}

out:
	if ( pContent ) free( pContent );
	if ( pBuffer ) free( pBuffer );
}

int asp_webcam(int ejid, webs_t wp, int argc, char_t **argv)
{
	if(strcmp(argv[0],"exist")==0)
 	{
		FILE *f;
	    f = fopen("/bin/quickcam", "rb");    
	    if(f==NULL)
	    {	
	    	websWrite(wp,"0");
	    }
	    else
	    {
	    	fclose(f);
			websWrite(wp,"1");	    	
	    }
	}
	else if(strcmp(argv[0],"start_server")==0)
 	{
		FILE *f;
	    f = fopen("/var/run/quickcam.pid", "r");    
	    if(f==NULL)
	    {	
	    	system("/bin/quickcam &");
	    	sleep(1);
	    }
	    else
	    {
	    	fclose(f);
	    }	    
	}
	return 0;
}
