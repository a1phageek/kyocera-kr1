/*
* ----------------------------------------------------------------
* Copyright c                  Realtek Semiconductor Corporation, 2002  
* All rights reserved.
* 
* $Header: /home/cvsroot/uClinux-dist/user/goahead-2.1.4/LINUX/board.c,v 1.258 2004/08/13 06:00:18 cfliu Exp $
*
* Abstract: Board specific definitions. Probably this file will be removed, 
*           and a data structure or script file stored in flash is used instead.
*
* $Author: cfliu $ 
*
* $Log: board.c,v $
* Revision 1.262  2004/08/20 12:12:47  cfliu
* +: Add Loose UDP ctrl item
* +: Rewrite WLAN WDS ctrl code
*
* Revision 1.261  2004/08/18 05:39:59  chenyl
* +: napt special option web-based setting
*
* Revision 1.260  2004/08/17 12:30:01  chenyl
* *: add port config to multicast default upstream setting API
*
* Revision 1.259  2004/08/17 09:25:54  tony
* *: add define CONFIG_RTL8185 to fix compiler warning.
*
* Revision 1.258  2004/08/13 06:00:18  cfliu
* +: Rewrite Wireless webpages for RTL8185 11a/b/g card
*
* Revision 1.257  2004/08/11 07:28:22  tony
* +: new features: support quickcam web camera.
*
* Revision 1.256  2004/08/04 14:48:56  yjlou
* *: merge rtl_bdinfo.h into flashdrv.h
*
* Revision 1.255  2004/07/29 11:46:05  chhuang
* *: bug fixed: use rtl8651_setNetMtu() instead of rtl8651_setPppoeMtu()
*
* Revision 1.254  2004/07/27 10:50:07  cfliu
* -: Remove RTL865x external loopback port related code for PPTP/L2TP/WLAN
*
* Revision 1.253  2004/07/27 07:23:52  chenyl
* +: DoS ignore type setting
*
* Revision 1.252  2004/07/26 05:46:11  chhuang
* *: Even if multiple session, only one MAC address is used in WAN
*
* Revision 1.251  2004/07/23 09:28:00  cfliu
* +: Add WLAN access ctrl code
*
* Revision 1.250  2004/07/15 11:16:39  tony
* +: add remote management DONT_TIMEOUT action in PPTP/L2TP mode.
*
* Revision 1.249  2004/07/14 13:55:59  chenyl
* +: web page for MN queue
*
* Revision 1.248  2004/07/13 10:23:30  tony
* *: fix bug when dhcpl2tp redial, dhcpc will call many times.
*
* Revision 1.247  2004/07/13 09:05:41  tony
* +: New Features: add new WAN type DHCPL2TP, support redial many times.
*
* Revision 1.246  2004/07/13 07:12:34  cfliu
* Add write kernel memory function
*
* Revision 1.245  2004/07/12 11:21:16  cfliu
* +: Update WLAN setting parameters
*
* Revision 1.244  2004/07/12 07:35:32  chhuang
* *: modify rate limit web page
*
* Revision 1.243  2004/07/12 07:25:16  tony
* *: make dhcp get gateway ip to be a l2tp server ip in dhcpl2tp mode.
*
* Revision 1.242  2004/07/12 07:03:47  tony
* +: add wan type dhcpl2tp.
*
* Revision 1.241  2004/07/12 06:12:05  cfliu
* no message
*
* Revision 1.240  2004/07/12 05:56:11  cfliu
* *: correct OFDM set error
*
* Revision 1.239  2004/07/12 04:25:36  chenyl
* *: extend existing port scan mechanism
*
* Revision 1.238  2004/07/08 10:51:41  tony
* *: change ipUp routine to new arch.
*
* Revision 1.237  2004/07/07 07:15:42  chenyl
* *: bug fix : bandwidth control sweep index fail
*
* Revision 1.236  2004/07/07 05:12:08  chhuang
* +: add a new WAN type. Not complete yet!!
*
* Revision 1.235  2004/07/07 05:04:45  tony
* *: disable pppd second parameter.
*
* Revision 1.234  2004/07/07 02:00:53  tony
* *: upnp_start will call upnp_stop to kill old upnp process.
*
* Revision 1.233  2004/07/06 06:19:09  chhuang
* +: add rate limit
*
* Revision 1.232  2004/07/05 11:58:44  tony
* *: modify UI control for UPNPD.
*
* Revision 1.231  2004/07/02 12:33:22  tony
* *: support for new IGD(UPNP).
*
* Revision 1.230  2004/06/30 05:16:11  tony
* *: support PPPoE/PPTP/L2TP for MMU mode.
*
* Revision 1.229  2004/06/29 16:16:06  cfliu
* +: define USERAPP_NOMMU only when MMU is not enabled
*
* Revision 1.228  2004/06/29 10:57:37  cfliu
* *: Update shared PCI mode check function using RTL865XBv2's SharedPCI bit
*
* Revision 1.227  2004/06/29 09:39:27  cfliu
* +:Add generic kernel memory read function based on /proc filesystem
*
* Revision 1.226  2004/06/29 07:35:42  chenyl
* +: igmp-proxy group filter
*
* Revision 1.225  2004/06/17 07:51:26  tony
* +: add call setNetMtu in PPTP/L2TP mode.
*
* Revision 1.224  2004/06/15 10:08:46  tony
* *: using share memory instead of direct access in syslog.
*
* Revision 1.223  2004/06/15 05:36:07  rupert
* *: fix do_exec
*
* Revision 1.222  2004/06/14 13:48:17  rupert
* +: Add work properly with MMU kernel
*
* Revision 1.221  2004/06/14 06:15:24  chenyl
* +: USERAPP_NOMMU for upnp application
* +: _UPNP_MM_ for upnp memory management
*
* Revision 1.220  2004/06/11 09:20:18  tony
* +: using share memory instead of direct access for pRomeCfgParam.
*
* Revision 1.219  2004/06/11 00:45:02  cfliu
* +: Add port ON/Off webpage
*
* Revision 1.218  2004/06/10 14:35:11  cfliu
* +: Add port config webpage
*
* Revision 1.217  2004/06/10 10:34:52  tony
* +: add PPTP dial status: disconnecting, make redial function correct.
*
* Revision 1.216  2004/06/09 12:32:16  tony
* +: add PPTP/L2TP UI page.(Dial and Hang_Up).
*
* Revision 1.215  2004/06/08 10:53:57  cfliu
* +: Add WLAN dual mode webpages. Not yet completed...
*
* Revision 1.214  2004/06/08 02:40:26  chenyl
* *: related modification for dns-proxy implementation change
*
* Revision 1.213  2004/06/07 08:01:00  tony
* *: fix dial-on-demand can't re-dial problem.
*
* Revision 1.212  2004/06/07 07:23:12  tony
* *** empty log message ***
*
* Revision 1.211  2004/06/04 08:08:30  chhuang
* ip unnumbered configuration: rtl8651_specifyNetworkIntfLinkLayerType() then
* rtl8651_addPppoeSession()
*
* Revision 1.210  2004/06/02 12:51:46  tony
* make PPPoE/L2TP is able to run multi-times in single process.
* and fix pppoe can't run dial-on-demand problem.
*
* Revision 1.209  2004/05/31 14:39:06  jzchen
* Change the counter setting to configured accompany with VLAN setting
*
* Revision 1.208  2004/05/28 09:49:16  yjlou
* +: support Protocol-Based NAT
*
* Revision 1.207  2004/05/27 05:17:59  cfliu
* Default set WLAN enable to 0
*
* Revision 1.206  2004/05/27 05:12:36  tony
* fix multiple pppoe dial problem.
*
* Revision 1.205  2004/05/27 05:07:45  chhuang
* change the order of rtl8651_specifyNetworkIntfLinkLayerType() and rtl8651_addPppoeSession()
*
* Revision 1.204  2004/05/26 10:11:55  tony
* make pppoe daemon always run at the same process.
*
* Revision 1.203  2004/05/22 03:21:57  chenyl
* * bug fix of web page setting/initial setting
*
* Revision 1.202  2004/05/21 12:08:50  chenyl
* *: TCP/UDP/ICMP spoof -> check for (sip) == (dip)
* *: modify DOS prevention webpage
*
* Revision 1.201  2004/05/20 09:57:00  tony
* fix bug: make l2tpd more stable.
*
* Revision 1.200  2004/05/20 08:43:55  chhuang
* add Web Page for QoS
*
* Revision 1.199  2004/05/20 07:10:11  tony
* fix bug: success for compiler in non pptp/l2tp mode.
*
* Revision 1.198  2004/05/20 03:21:37  tony
* New Features: add dial-on-demand and auto-reconnect for PPTP/L2TP.
*
* Revision 1.197  2004/05/19 08:38:54  orlando
* checkin diag led and init button and bi color led realted code
*
* Revision 1.196  2004/05/17 12:16:35  cfliu
* Add access concentrator's name for PPPoE.
*
* Revision 1.195  2004/05/17 07:22:16  tony
* fix pppoe silent timeout bug and make PPTP/L2TP support silent timeout.
*
* Revision 1.194  2004/05/12 08:09:56  chhuang
* +: dhcp static binding
*
* Revision 1.193  2004/05/12 07:20:15  chenyl
* +: source ip blocking
* *: modify dos mechanism
*
* Revision 1.192  2004/05/12 05:15:05  tony
* support PPTP/L2TP in RTL865XB.
*
* Revision 1.191  2004/05/10 05:49:20  chhuang
* add src ip blocking for url-filtering
*
* Revision 1.190  2004/05/05 09:39:37  tony
* open dns relay when pptp/l2tp mode
*
* Revision 1.189  2004/05/05 08:27:07  tony
* new features: add remote management spec
*
* Revision 1.188  2004/05/03 15:02:51  cfliu
* Add 8650B extension port support.
* Revise all WLAN/extport related code.
*
* Revision 1.187  2004/04/30 13:00:32  chenyl
* *: remove CONFIG_RTL865X_MULTICAST and always include multicast/igmp function
*
* Revision 1.186  2004/04/29 05:41:54  chhuang
* add new API rtl8651_addNaptMappingExt()
*
* Revision 1.185  2004/04/27 01:50:51  tony
* add l2tp length bit
*
* Revision 1.184  2004/04/19 13:45:18  tony
* add resetPptpProprity and resetL2tpProprity IOCTL.
*
* Revision 1.183  2004/04/16 07:34:49  cfliu
* Reorg loopback port setting code.
*
* Revision 1.182  2004/04/14 08:43:56  tony
* make L2TP support auto reconnect.
*
* Revision 1.181  2004/04/13 08:15:53  tony
* fix bug: make PPTP/L2TP run stably.
*
* Revision 1.180  2004/04/09 02:26:12  tony
* remove rtl8651_specifyNetworkIntfLinkLayerType_1 IOCTL.
*
* Revision 1.179  2004/04/08 13:18:12  tony
* add PPTP/L2TP routine for MII lookback port.
*
* Revision 1.178  2004/04/08 12:29:05  cfliu
* Add WDS support on extension port API
* Add some debug mesg
*
* Revision 1.177  2004/03/31 01:59:36  tony
* add L2TP wan type UI pages.
*
* Revision 1.176  2004/03/19 09:51:18  tony
* make the 'lan permit' acl is able to select by user.
*
* Revision 1.175  2004/03/17 03:04:36  tony
* fix bug: dhcp server started when dhcps is disable in UI.
*
* Revision 1.174  2004/03/12 08:31:56  tony
* when Server Port add failed, disable it in UI.
*
*
* Revision 1.172  2004/03/10 11:41:40  cfliu
* Move layer2 related functions and code to separate header file & src file.
*
* Revision 1.171  2004/03/09 08:58:02  cfliu
* add MII port as one of the default LAN port
*
* Revision 1.170  2004/03/05 12:26:14  tony
* fix bug: when in DMZ mode, trap port 23 to protocol stack.
*
* Revision 1.169  2004/03/04 08:51:38  tony
* fix bug: must asign dial seesion ID, when call rtl8651_addRoute in pppoe mode.
*
* Revision 1.168  2004/03/03 03:43:31  tony
* add static routing table in turnkey.
*
* Revision 1.167  2004/02/23 15:02:23  cfliu
* Add WLAN support
*
* Revision 1.166  2004/02/18 09:27:56  tony
* add telnetd on turnkey.
*
* Revision 1.165  2004/02/13 08:26:09  rupert
* Fix lanType parameter when PPPoE Down
*
* Revision 1.164  2004/02/11 06:24:21  tony
* let ACL can drop LAN users's packets which forward to Internet.
* but, all the LAN users's packets to gateway must pass through.
*
* Revision 1.163  2004/02/11 03:05:39  orlando
* add dial-on-demand support for multiple pppoe's unnumbered case
*
* Revision 1.162  2004/02/10 12:48:52  orlando
* check in multiple/unnumbered ip code
*
* Revision 1.161  2004/02/09 09:18:00  tony
* add function: user is able to add protocol stack Server used TCP ports by
* rtl8651_addProtoStackServerUsedTcpPort(incoming_src_port); and flush the
* table by rtl8651_flushProtoStackServerUsedTcpPort();
*
* Revision 1.160  2004/02/09 07:17:36  tony
* avoid setting gateway's IP address in ACL(rtl8651a_addAclRule).
*
* Revision 1.159  2004/02/05 12:06:21  tony
* fix acl bug: set flagMask to 0 when set type to IP.
*
* Revision 1.158  2004/02/05 07:11:16  tony
* add acl filter field: IP.
*
* Revision 1.157  2004/02/04 06:23:49  rupert
* Add UDP Block Support
*
* Revision 1.156  2004/02/03 08:14:34  tony
* add UDP Blocking web UI configuration.
*
* Revision 1.155  2004/02/03 07:31:45  orlando
* checkin bug fix regarding multiple pppoe/napt's patch CVS: ----------------------------------------------------------------------
*
* Revision 1.154  2004/01/30 12:03:05  tony
* make Special Application is able to support multiple session.
*
* Revision 1.153  2004/01/30 06:52:54  tony
* fix bug: serverpTableDriverAccessCalledFromBoot is not able to check correctly.
*
* Revision 1.152  2004/01/29 14:15:49  rupert
* fix dmz wan ip
*
* Revision 1.151  2004/01/29 12:17:08  rupert
* Add Multiple rtl8651a_setWanStatus() support
*
* Revision 1.150  2004/01/29 12:15:35  orlando
* for acl_tableDriverAccess()
*
* Revision 1.149  2004/01/29 11:42:07  rupert
* Add Multiple DMZ support
*
* Revision 1.148  2004/01/29 10:56:08  orlando
* turnon dos related settings
*
* Revision 1.147  2004/01/29 09:46:33  tony
* modify serverp_tableDriverAccess(), make it support multiple session.
* add protocol type in server port UI.
*
* Revision 1.146  2004/01/29 01:40:41  orlando
* checkin acl's dsid support, not the final version
*
* Revision 1.145  2004/01/27 08:34:13  tony
* fix bug: low NAPT throughput when special application type is BOTH.
*
* Revision 1.144  2004/01/16 12:16:46  tony
* modify ALG cfg params and web UI,
* rearrange rtl8651a_setAlgStatus() must called after rtl8651_addIpIntf()
*
* Revision 1.143  2004/01/15 13:46:30  tony
* move rtl8651a_setAlgStatus(), it must call after IP Up and rtl8651_addIpIntf().
*
* Revision 1.142  2004/01/14 03:37:18  tony
* Support single session in rtl8651_setAlgStatus (for old user)
*
* Revision 1.141  2004/01/14 02:42:46  hiwu
* add PPTP configuration
*
* Revision 1.140  2004/01/12 09:48:31  orlando
* add debug msg for rtl8651_setPppoeDefaultDialSessionId()
*
* Revision 1.139  2004/01/09 08:26:52  orlando
* checkin acl related api change
*
* Revision 1.138  2004/01/09 08:03:54  tony
* make the code architecture of ALG is able to support multiple dial session.
*
* Revision 1.137  2004/01/09 06:12:01  orlando
* checkin serverport api change
*
* Revision 1.136  2004/01/09 05:03:59  orlando
* checkin dmz api change
*
* Revision 1.135  2004/01/09 04:27:16  orlando
* tune module init sequence in sysInit()
*
* Revision 1.134  2004/01/08 13:24:59  orlando
* url/log/dos related change
*
* Revision 1.133  2004/01/08 12:13:44  tony
* add Port Range into Server Port.
* support Server Port for multiple session UI.
*
* Revision 1.132  2004/01/08 07:23:20  tony
* support multiple session UI for ACL, URL Filter, DoS log.
*
* Revision 1.131  2004/01/07 10:59:56  tony
* Support multiple session UI plugin for ACL.
*
* Revision 1.130  2004/01/07 09:10:04  tony
* add PPTP Client UI in Config Wizard.
*
* Revision 1.129  2004/01/07 07:36:36  tony
* Support multiple session UI plugin for ALG.
*
* Revision 1.128  2004/01/07 05:38:30  orlando
* logging feature sercomm phase2 spec 1st milestone checkin
*
* Revision 1.127  2004/01/07 03:43:53  orlando
* checkin sercomm phase2 spec 1st milestone api
*
* Revision 1.126  2004/01/06 13:50:25  tony
* Support multiple session UI plugin for the following functions:
* DMZ, URL Filter, Dos Prevention
*
* Revision 1.125  2004/01/02 01:15:55  orlando
* to support global ip before pppoe dial-up, add ip interface at wanlan_init(),
* and skip addIpIntf()/delIpIntf() at ipUpEvent/ipDownEvent.
*
* Revision 1.124  2003/12/31 05:53:00  orlando
* to support global ip subnet in lan before pppoe dial-up using
* "ifconfig eth1:1 <addr>", also call "rtl8651_addIpIntf()"
* in wanlan_init().
*
* Revision 1.123  2003/12/19 04:33:01  tony
* add Wireless Lan config pages: Basic Setting, Advance Setting, Security, Access Control, WDS
*
* Revision 1.122  2003/12/18 12:45:05  rupert
* fix demand route
*
* Revision 1.121  2003/12/17 11:50:06  rupert
* change policy route from ip net to ip range
*
* Revision 1.120  2003/12/17 08:29:08  tony
* add "parse_ipRange()" fucntion.
*
* Revision 1.119  2003/12/17 03:07:52  rupert
* fix idle timeout
*
* Revision 1.118  2003/12/12 10:44:14  orlando
* clear setPppoeMtu() code before ip-up events
*
* Revision 1.117  2003/12/12 02:43:03  tony
* add rtl8651_setMtu() function.
*
* Revision 1.116  2003/12/12 01:34:54  tony
* add NAPT toggle in Unnumbered PPPoE.
*
* Revision 1.115  2003/12/10 03:08:58  tony
* make logging function support time zone in kernel space.
*
* Revision 1.114  2003/12/09 13:44:34  tony
* add ACL,DoS,URL Filter logging function in kernel space.
*
* Revision 1.113  2003/12/09 03:05:44  tony
* add a param in rtl8651_setPppoeMtu function.
*
* Revision 1.112  2003/12/02 12:30:45  tony
* the common.h is replace by rtl8651_dos.h and rtl8651_alg_init.h
*
* Revision 1.111  2003/12/02 10:24:47  tony
* Add Routine: support DoS is able to set threshold by user(Webs GUI).
*
* Revision 1.110  2003/12/02 08:30:11  tony
* remove AOE2 from Special Application.(move to ALG: DirectX7)
*
* Revision 1.109  2003/12/01 12:35:52  tony
* make ALG is able to be configured by users(Web GUI).
*
* Revision 1.108  2003/11/28 07:27:53  tony
* modify AOE's trigger port type of default value to BOTH.
*
* Revision 1.107  2003/11/28 01:49:06  rupert
* FIX MULTIPLE HANGUP
*
* Revision 1.106  2003/11/27 12:15:54  tony
* remove mIRC from specail application default value.(use mIRC by ALG now)
*
* Revision 1.105  2003/11/27 03:26:57  tony
* fix bug:if default session is changed, old route table will can't delete correctly.
* must del old route before setting new default session.
*
* Revision 1.104  2003/11/24 02:21:01  rupert
* only multiple pppoe have defaultSession
*
* Revision 1.103  2003/11/21 03:29:23  tony
* fix bug: when pppoe default session not at first session, pppoe will don't have default route.
*
* Revision 1.102  2003/11/21 01:56:48  tony
* add function: user is able to add protocol stack used UDP ports by
* rtl8651_addProtoStackUsedUdpPort(incoming_src_port);
*
* Revision 1.101  2003/11/20 12:19:29  rupert
* Demand Route
*
* Revision 1.100  2003/11/19 03:52:09  tony
* modify unnumbered pppoe netmask gui.
*
* Revision 1.99  2003/11/19 02:40:15  rupert
* fix pppd_start_dial
*
* Revision 1.98  2003/11/18 13:25:39  rupert
* add dial demand
*
* Revision 1.97  2003/11/18 09:04:47  tony
* add routine: support mtu configure by user in pppoe.
*
* Revision 1.96  2003/11/14 09:13:36  tony
* modify multiple pppoe web page
*
* Revision 1.95  2003/11/13 12:52:06  tony
* add MTU and MRU field in pppoe.
*
* Revision 1.94  2003/11/13 12:31:28  tony
* small change
*
* Revision 1.93  2003/11/12 05:37:48  tony
* fix bug: 1.pppd can't be killed when connect failed.
* fix bug: 2.pppoe's service name can't use 'space' char.
*
* Revision 1.92  2003/11/10 13:49:36  tony
* fix multi-pppoe bugs:
* 1:dialState set to zero when boot.
* 2:disable Dial-on-Demand and Silent Timeout.
* 3:must hang up all the connection before save configuration.
* 4:only default session dial-on when boot.
*
* Revision 1.91  2003/11/10 11:40:58  orlando
* add pppd option "<local IP address>:<remote IP address>" for
* pppoe unnumbered case
*
* Revision 1.90  2003/11/10 07:43:34  rupert
* correct dport start end
*
* Revision 1.89  2003/11/07 11:05:59  rupert
* for multiple pppoe
*
* Revision 1.88  2003/11/07 09:16:26  rupert
* update multiple pppoe
*
* Revision 1.87  2003/11/07 06:31:22  tony
* add type PPPOECFGPARAM_DESTNETTYPE_NONE in MultiPPPoE dest network.
*
* Revision 1.86  2003/11/07 05:34:36  tony
* modify YMSG ALG, it'll not have to cooperate with trigger port.
*
* Revision 1.85  2003/11/06 02:25:08  tony
* add field in multi-pppoe cfg.
*
* Revision 1.84  2003/11/04 09:30:56  tony
* modfiy special-application list in board.c
*
* Revision 1.83  2003/11/04 01:56:34  orlando
* modify acl_tableDriverAccess() to set a aclCfgParam[i].port==0 ->
* tcpDst(Src)PortLB=0,tcpDst(Src)UB=0xffff. also modify ingress rule
* to set on 'eth1'(LAN) and egress rule to set on 'eth0'(WAN)
*
* Revision 1.82  2003/10/30 12:23:01  orlando
* set event log to be default "off"
*
* Revision 1.81  2003/10/29 10:20:41  tony
* modify acl policy list.
*
* Revision 1.80  2003/10/28 08:33:45  orlando
* rtl8651_setAsicNaptAutoAddDelete() obsoleted by rtl8651_enableNaptAutoAdd()
*
* Revision 1.79  2003/10/27 09:39:11  tony
* fix bug: clear acl rules with eth1
*
* Revision 1.78  2003/10/24 10:25:58  tony
* add DoS attack interactive webpage,
* FwdEngine is able to get WAN status by rtl8651_wanStatus(0:disconnect,1:connect)
*
* Revision 1.77  2003/10/21 08:07:02  tony
* add DoS attack log routine
*
* Revision 1.76  2003/10/17 07:17:54  tony
* *** empty log message ***
*
* Revision 1.75  2003/10/17 06:37:37  tony
* bug fix: factory_deafult() get error factory params.
*
* Revision 1.74  2003/10/17 03:41:47  tony
* add klogd to log_init
*
* Revision 1.73  2003/10/16 09:06:58  orlando
* add association between pppoe table entry (pppObjId) and pppx by
* parameter parsing argv[9]
*
* Revision 1.72  2003/10/16 03:11:32  orlando
* tune multiple pppoe association of hw table entry and ppp-x
*
* Revision 1.71  2003/10/15 12:12:08  orlando
* add pppoeCfgParam[].pppx to keep track of hw ppp obj id (0,1,2,3,...)
* in association with linux ppp dial up interface (ppp0,ppp1,ppp2,ppp3,...)
*
* Revision 1.70  2003/10/15 08:04:47  tony
* move Trigger Port range checking routine from FastAlias to NaptAlias
*
* Revision 1.69  2003/10/15 06:55:44  orlando
* add chap support
*
* Revision 1.68  2003/10/14 13:13:00  tony
* remove untested special application.
*
* Revision 1.67  2003/10/14 08:15:01  tony
* add user account management routine
*
* Revision 1.66  2003/10/14 02:28:30  tony
* add trigger port routine
*
* Revision 1.65  2003/10/09 13:23:44  tony
* TRIGGER_PORT: add function rtl8651_flushTriggerPortRules,rtl8651_addTriggerPortRule
*
* Revision 1.64  2003/10/07 06:18:17  rupert
* servie name
*
* Revision 1.63  2003/10/07 03:47:17  tony
* revise bug: multi pppoe start by pppoeCfgParam[].enable
*
* Revision 1.62  2003/10/07 03:45:36  tony
* revise bug: multi pppoe start by pppoeCfgParam[].enable
*
* Revision 1.61  2003/10/07 03:25:36  rupert
* change udhcpd server DNS options to gateway LAN Address
*
* Revision 1.60  2003/10/07 02:25:22  tony
* *** empty log message ***
*
* Revision 1.59  2003/10/06 13:17:38  rupert
* add user managment of webs interface
*
* Revision 1.58  2003/10/06 09:05:18  rupert
* Running the  dns proxy when  default session of multiple PPPoE starts
*
* Revision 1.57  2003/10/06 07:03:17  rupert
* remove sleep  when dhcps start
*
* Revision 1.56  2003/10/06 02:22:17  orlando
* add/delete a network route to linux protocol stack upon ip-up/down-event for
* unnumbered pppoe case (global network assigned by ISP in LAN)
*
* Revision 1.55  2003/10/03 12:27:35  tony
* add NTP time sync routine in management web page.
*
* Revision 1.54  2003/10/03 03:17:34  rupert
* fix dns null problem
*
* Revision 1.53  2003/10/03 01:26:42  tony
* add ServiceName field in pppoeCfgParam_t.
* add dynamic check in PPPoE/Unnumbered PPPoE/Multiple PPPoE web page.
*
* Revision 1.52  2003/10/02 10:50:17  orlando
* add manualHangup and whichPppObjId fields in pppoeCfgParam_t for auto
* reconnect implementation
*
* Revision 1.51  2003/10/02 07:53:13  orlando
* add pppoeManualHangupFlag[] for recording manual hangup events!!
*
* Revision 1.50  2003/10/02 06:34:40  orlando
* ppd_start() changed to add "defaultroute" or "nodefaultroute" in ppp option file, so linux routing table will have one default route only in multiple pppd case
*
* Revision 1.49  2003/10/02 04:58:29  rupert
* add dns proxy/cache relay
*
* Revision 1.48  2003/10/01 13:51:51  tony
* remove all unused variable.
*
* Revision 1.47  2003/10/01 13:40:39  tony
* solve all the compiler warnning message in tblDrv_mapping.c
*
* Revision 1.46  2003/10/01 13:25:13  tony
* change default connection type to static
*
* Revision 1.45  2003/10/01 12:27:20  orlando
* delete /var/run/pppd[id].pid file in board_ipDownEventTableDriverAccess(). asp_pppoe will check its existence, so as to decide if dial button is enabled.
*
* Revision 1.44  2003/10/01 10:51:38  rupert
* add dhcpc client status
*
* Revision 1.43  2003/10/01 10:31:47  tony
* solve all the compiler warnning messages in the board.c
*
* Revision 1.42  2003/10/01 09:41:32  orlando
* check in dial-on-demand and silent timeout related code
*
* Revision 1.41  2003/10/01 08:36:39  tony
* modify DHCPS start sequence.
*
* Revision 1.40  2003/10/01 07:43:17  tony
* kill upnp/ddns daemon before service start. in ddns_start(), upnp_start()
*
* Revision 1.39  2003/10/01 07:01:50  tony
* arrange boot start code
*
* Revision 1.38  2003/10/01 06:10:05  orlando
* sync changes
*
* Revision 1.37  2003/10/01 05:57:31  tony
* add URL Filter routine
*
* Revision 1.36  2003/10/01 05:43:24  orlando
* modify acl to act on "eth0" instead of "vl0"
*
* Revision 1.35  2003/10/01 05:17:09  orlando
* fine tune dmz_tableDriverAccess()/serverp_tableDriverAccess()/acl_tableDriverAcess() to choose the correct wan ip to config to rtl8651
*
* Revision 1.34  2003/10/01 03:03:10  orlando
* sync with cvs before changing multiple pppoe/default implementaiton
*
* Revision 1.33  2003/09/30 12:19:49  tony
* arrange board_ipUpEventTableDriverAccess(), let upnp,ddns,dmz be able to start after dhcpc/pppoe started.
*
* Revision 1.32  2003/09/30 09:46:38  tony
* add dmz_tableDriverAccess()
*
* Revision 1.31  2003/09/30 08:56:29  tony
* remove newServerpCfgParam[] from flash, rename ram PPPoeCfg to ramPppoeCfgParam
*
* Revision 1.30  2003/09/30 08:25:56  orlando
* rtl8651_setDefaultAcl(ifName,defAction) requires different parameter than before
*
* Revision 1.29  2003/09/30 08:00:49  orlando
* trim acl_tableDriverAccess(), and call acl_tableDriverAccess() in acl_init only in STATIC case
*
* Revision 1.28  2003/09/30 07:08:49  orlando
* clear ifCfgParam[0].ipAddr/ipMask/gwAddr/dnsPrimaryAddr/dnsSecondaryAddr to 0 for dhcp client wan config case, dhcp client will fill in correct values later
*
* Revision 1.27  2003/09/30 06:13:28  orlando
* check in after new table driver checkin
*
* Revision 1.26  2003/09/30 03:51:09  orlando
* sync changes
*
* Revision 1.25  2003/09/29 13:28:19  rupert
* add primary and secondary fields of ifCfgParam_s
*
* Revision 1.24  2003/09/29 12:58:34  tony
* add DDNS start/stop routine.
*
* Revision 1.23  2003/09/29 08:57:32  tony
* add routine: when pppoe config changed, the dial function will be disabled.
* add routine: dhcps is able to start/stop server on runtime.
*
* Revision 1.22  2003/09/29 07:20:37  orlando
* modify dmz_init() and add dmz related code in board_ipUpEventTableDriverAccess()
*
* Revision 1.21  2003/09/29 06:30:15  orlando
* kill -9 changed kill -15
*
* Revision 1.20  2003/09/29 05:07:22  orlando
* add board_ipUp/DownEventTableDriverAccess()
*
* Revision 1.19  2003/09/26 10:23:52  tony
* add PPPoE dial,hang-up routine
*
* Revision 1.18  2003/09/26 07:19:13  orlando
* set pppoeCfgParam[].dialState to ..._TRYING right before calling /bin/pppd
*
* Revision 1.17  2003/09/26 02:09:48  tony
* add/del UPnP default route in upnp_init() and asp_setUpnp()
*
* Revision 1.16  2003/09/26 01:53:40  orlando
* pppoeCfgParam adds more fields for control, tune code to adapt the changes
*
* Revision 1.15  2003/09/25 10:44:50  tony
* small change
*
* Revision 1.14  2003/09/25 10:14:50  tony
* modify pppoe webpage, support unnumbered IP, PPPoE, Multiple PPPoE.
*
* Revision 1.13  2003/09/25 06:41:40  rupert
* add routing function of kernel
*
* Revision 1.12  2003/09/25 03:37:36  orlando
* integration...
*
* Revision 1.11  2003/09/25 02:15:32  orlando
* Big Change
*
* Revision 1.8  2003/09/23 11:44:33  tony
* add UPNP daemon start/stop routine.
*
* Revision 1.7  2003/09/23 03:47:29  tony
* add ddnsCfgParam,ddnsDefaultFactory,ddns_init
*
* Revision 1.6  2003/09/22 08:01:45  tony
* add UPnP Web Configuration Function Routine
*
* Revision 1.5  2003/09/22 06:33:37  tony
* add syslogd process start/stop by CGI.
* add eventlog download/clear routine.
*
* Revision 1.4  2003/09/19 06:06:51  tony
* *** empty log message ***
*
* Revision 1.3  2003/09/19 01:43:50  tony
* add dmz routine
*
* Revision 1.2  2003/09/18 02:05:45  tony
* modify pppoeCfgParam to array
*
* Revision 1.1.1.1  2003/08/27 06:20:15  rupert
* uclinux initial
*
* Revision 1.1.1.1  2003/08/27 03:08:53  rupert
*  initial version 
*
* Revision 1.47  2003/07/04 10:02:26  orlando
* adjust rtl8651_addRoute() and rtl8651_addArp() sequence
* in if_init(). don't call them if l2 switching mode.
*
* Revision 1.46  2003/07/04 08:37:53  orlando
* adjust code layout.
*
* Revision 1.45  2003/07/04 08:32:45  orlando
* add debug compile switch for rtl8651_addRoute().
*
* Revision 1.44  2003/07/04 04:52:44  kckao
* Added for PPPoE HW acceleration
*
* Revision 1.43  2003/07/04 03:23:30  orlando
* add ASIC default route to wan's default gateway
* during if_init() is processing ifCfgParam[0].
*
* Revision 1.42  2003/07/03 12:15:59  tony
* fix some warning messages.
*
* Revision 1.41  2003/07/03 08:09:47  orlando
* if dhcp client clone mac is used, call rtl8651_assignVlanMacAddress()
* to change asic wan vlan mac, no matter if hw acceleration
* is used or not.
*
* Revision 1.40  2003/07/03 06:27:20  orlando
* serverp_tableDriverAccess() use inet_addr() to replace inet_aton().
*
* Revision 1.39  2003/07/03 05:59:46  tony
* add configChanged global param.
*
* Revision 1.38  2003/07/03 01:58:37  orlando
* acl table driver access should use "vl0" to set asic, even if pppoe is
* enabled.
*
* Revision 1.37  2003/07/02 10:26:25  orlando
* 1. arp has to be added after ip interface created.
* 2. assignVlanMac has to be done before network interface
*     created above the vlan. so clone mac has to be assigned
*     in vlan_init().
*
* Revision 1.36  2003/07/02 08:06:50  orlando
* call rtl8651_assignVlanMacAddress() to assign clone mac to wan
* interface if dhcp client is started on wan port and clone mac
* is enabled.
*
* Revision 1.35  2003/07/01 13:59:40  orlando
* modify mismatchDrop() logic to reflect correct default permit or drop.
*
* Revision 1.34  2003/07/01 12:58:48  orlando
* 1. replace rtlHwEnabled by natCfgParam.hwaccel.
* 2. call different table driver acl API depending on natCfgParam.hwaccel.
*
* Revision 1.33  2003/07/01 10:25:14  orlando
* call table driver server port and acl API in board.c
*
* Revision 1.32  2003/07/01 05:59:18  elvis
* remote L3_ACCELERATION and L4_ACCELERATION
*
* Revision 1.31  2003/07/01 03:09:06  tony
* add aclGlobalCfgParam structure.
* modify aclCfgParam and serverpCfgParam structure.
*
* Revision 1.30  2003/06/30 13:27:56  tony
* add a field in server_port table
*
* Revision 1.29  2003/06/30 07:46:12  tony
* add ACL and Server_Port structure
*
* Revision 1.28  2003/06/27 13:53:18  orlando
* remove dhcpcCfgParam.valid field, use ifCfgParam[0].connType field
* modify rtConfigHook() for cloneMac:
* use dhcpcCfgParam.cloneMac if ifCfgParam[0].connType is
* DHCPC and dhcpcCfgParam.cmacValid is 1 during boottime.
*
* Revision 1.27  2003/06/27 05:34:54  orlando
* modify pppoe_init() and dns_init().
*
* Revision 1.26  2003/06/26 03:22:29  tony
* remove some element in dhcpc data structure.
*
* Revision 1.25  2003/06/26 03:21:10  orlando
* modify dns_init().
*
* Revision 1.24  2003/06/25 08:59:53  orlando
* change default setting of static route to be "null".
*
* Revision 1.23  2003/06/25 07:08:40  orlando
* change factory default settings.
*
* Revision 1.22  2003/06/23 10:57:23  elvis
* change include path of  rtl_types.h
*
* Revision 1.21  2003/06/20 12:59:55  tony
* add dhcp client
*
* Revision 1.20  2003/06/17 10:50:00  elvis
* regular update
*
* Revision 1.19  2003/06/17 06:59:46  orlando
* trying to avoid word aligned load/store.
*
* Revision 1.18  2003/06/16 08:08:30  tony
* add dhcps & dns function
*
* Revision 1.17  2003/06/11 11:39:29  tony
* add WAN IP connection Type (static, PPPoE, DHCP)
*
* Revision 1.16  2003/06/06 12:23:13  orlando
* modify userDb default values.
*
* Revision 1.15  2003/06/06 11:56:57  orlando
* add pppoe cfg.
*
* Revision 1.14  2003/06/06 06:31:45  idayang
* add mgmt table in cfgmgr.
*
* Revision 1.13  2003/06/03 10:56:47  orlando
* add nat table in cfgmgr.
*
* Revision 1.12  2003/06/02 01:50:14  elvis
* regular update
*
* Revision 1.11  2003/05/19 09:40:08  elvis
* 1. bug-fix: parameters to rtl8651_addXXX
* 2. add show_curCfg()
*
* Revision 1.10  2003/05/19 07:35:22  orlando
* change factory default ip addresses.
*
* Revision 1.9  2003/05/16 06:18:16  elvis
* no message
*
* Revision 1.8  2003/05/12 10:55:46  orlando
* adjuct header file structure.
*
* Revision 1.7  2003/05/09 13:27:34  orlando
* tune output message.
*
* Revision 1.6  2003/05/09 12:24:08  kckao
* add rtConfigHook() and move upwan() and uplan() to this file
*
* Revision 1.5  2003/05/02 09:18:13  orlando
* no message
*
* Revision 1.4  2003/05/02 08:51:34  orlando
* merge cfgmgr with old board.c/board.h.
*
* Revision 1.3  2003/04/08 15:29:53  elvis
* add interface ,route and static arp configurations
*
* Revision 1.2  2003/04/04 02:31:17  elvis
* Define member ports by symbols
*
* Revision 1.1  2003/04/03 01:52:42  danwu
* init
*
* ---------------------------------------------------------------
*/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <wait.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/wireless.h>	//Added by Murphy Wu For ATH_SET
//#include <net/if.h>		//Marked by Murphy Wu For ATH_SET
#include <net/if_arp.h>
#include <linux/if_ether.h>
#include <net/route.h>
#include <re865x.h>
#include <unistd.h> /* for sleep */
#include "board.h"
#include "flashdrv.h"
#include <rtl8651_dos.h> /* for DoS define */
#include <rtl8651_multicast.h>
#include <linux/config.h>

/*#include <rt_stack/h/sys/porting.h> */
#include "rtl_cfgmgr.h"
#include "rtl_board.h" /* exported function prototypes */
#include <rtl8651_tblDrv.h>
#include <rtl8651_alg_init.h>
#ifndef __uClinux__
/*  mmap header */
#include <fcntl.h>
#include <stdio.h>

#include <sys/mman.h>


#endif
#include "cfg.h"	//Added by Nick HO For CFG_TLV
#define IF_NAMESIZE     16	//Added by Murphy Wu For ATH_SET

// Dino Chang 2005/03/07
#include <rtl8651_alg_init.h>
// Dino
/* ==============================================================
	Gateway Setting Related data:

		1. Rome driver setting declarations
		2. Default Values
     ============================================================== */

//romeCfgParam_t romeCfgParam;
//romeCfgParam_t * pRomeCfgParam = &romeCfgParam;
extern romeCfgParam_t *pRomeCfgParam;
ifCfgParam_t			ramLanCfgParam;
ifCfgParam_t			ramWanCfgParam;
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
pptpCfgParam_t			ramPptpCfgParam;
l2tpCfgParam_t			ramL2tpCfgParam;
//dhcpL2tpCfgParam_t		ramDhcpL2tpCfgParam;
#endif
pppoeCfgParam_t			ramPppoeCfgParam[MAX_PPPOE_SESSION];
dhcpsCfgParam_t			ramDhcpsCfgParam;
dhcpcCfgParam_t			ramDhcpcCfgParam;
serverpCfgParam_t		ramServerpCfgParam[MAX_PPPOE_SESSION][MAX_SERVER_PORT];
urlfilterCfgParam_t		ramUrlfilterCfgParam[MAX_PPPOE_SESSION][MAX_URL_FILTER];
int 	totalWlanCards;
#ifdef CONFIG_RTL8185
	int8 cfgFilePath[]="/var/8185wpa.conf";
#endif

#ifdef CONFIG_RTL865XB_3G
#define CONFIG_RTL865XB_3G_VLAN 8 // current solution use eth0 as bridge, so set ppp0 to be the same vlan ID as eth0. 10
#endif


#ifdef CFG_TLV
vlanCfgParam_t vlanCfgParamDefault[3] = 
{
    {8, (PHY4),(PHY4),      1480, 1, {{0x00, 0x00, 0xda, 0x11, 0x22, 0x58}}, 0}, 
    {9, (PHY0|PHY1|PHY2|PHY3|PHYMII),(PHY0|PHY1|PHY2|PHY3|PHYMII), 1480, 1, {{0x00, 0x00, 0xda, 0x11, 0x22, 0x68}}, 0}, 
    {0, 0x00,0x00,             0,    0, {{0,0,0,0,0,0}}, 0}
};

ifCfgParam_t ifCfgParamDefault[3] =
{	
	{0, {0,0,0,0} ,{0,0,0,0},{0,0,0,0} ,{0,0,0,0}, {0,0,0,0},0, 1, IFCFGPARAM_CONNTYPE_3G, {0,0,0,0,0,0}, 0}, 
	{1, {192,168,0,1},{255,255,255,0},{192,168,0,1},{168,95,1,1}, {168,95,1,1},0, 1, 0,{0,0,0,0,0,0},0},
	{0, {0,0,0,0} ,{0,0,0,0},{0,0,0,0} ,{0,0,0,0},{0,0,0,0} , 0, 0, 0, {0,0,0,0,0,0}, 0}
};

routeCfgParam_t routeCfgParamDefault[] =
{
	{0, {0,0,0,0},     {0,0,0,0},       {0,0,0,0},       0, 0}
};



arpCfgParam_t arpCfgParamDefault[] =
{
	/* Add static arp entrie for the hosts on the LAN interface, e.g. your PC */
	{0, {0,0,0,0},      0,         0,{{0,0,0,0,0,0}}, 0}
};


natCfgParam_t natCfgParamDefault =
{
	1, /* enable */
	1, /* hw accel */
	1, 	/* ipsec passthru */
	1, 	/* pptp passthru */
	0,  /* no l2tp passthru */
	0,	/* no mac filter -- Min Lee*/
	0	/* field_end */
};

pppoeCfgParam_t pppoeCfgParamDefault[MAX_PPPOE_SESSION] =
{
	{
	1, /* disabled/enabled */
	1, /* default session or not */
	0, /* is pppoe unnumbered ? */
	1, /* auto reconnect */	
	0, /* dial on demand */
	0, /* dial state */
	0, /* session id */
	0, /* silent time out */
	{172,21,1,254} ,{255,255,255,0},{172,21,1,250} ,{168,95,1,1},
	{192,168,1,1}, /* local host ip */
	"\0", /* user name */
	"\0", /* password */
	"\0", /* ac name */
	"\0", /* service name */
	{"\0","\0","\0","\0"}, /* destination network */
	{PPPOECFGPARAM_DESTNETTYPE_NONE,	PPPOECFGPARAM_DESTNETTYPE_NONE,	PPPOECFGPARAM_DESTNETTYPE_NONE,	PPPOECFGPARAM_DESTNETTYPE_NONE},
	{0,0,0,0},  /* unnumbered ip */
	{255,255,255,254},   /* unnumbered network mask */
	{0,0,0,0,0,0}, /* server mac */
	0xff, /* pppx */
	1, /* Unnumbered PPPoE NAPT */
	0, /* whichPppObjId */
	0,  /* manualHangup */
	1492, /* MTU */
	PPPOECFGPARAM_LANTYPE_NAPT
	},

	{
	1, /* disabled/enabled */
	0, /* default session or not */
	0, /* is pppoe unnumbered ? */
	1, /* auto reconnect */
	1, /* dial on demand */	
	0, /* dial state */
	0, /* session id */
	0, /* silent time out */
	
	{172,22,1,254} ,{255,255,255,0},{172,22,1,250} ,{168,95,1,1},
	{192,168,1,1}, /* local host ip */
	"\0", /* user name */
	"\0", /* password */
	"\0", /* AC name */
	"\0", /* service name */
	{"\0","\0","\0","\0"}, /* destination network */
	{PPPOECFGPARAM_DESTNETTYPE_NONE,	PPPOECFGPARAM_DESTNETTYPE_NONE,	PPPOECFGPARAM_DESTNETTYPE_NONE,	PPPOECFGPARAM_DESTNETTYPE_NONE},
	{0,0,0,0},  /* unnumbered ip */
	{255,255,255,254},  /* unnumbered network mask */
	{0,0,0,0,0,0}, /* server mac */
	0xff, /* pppx */
	0, /* rsv1 */
	0, /* whichPppObjId */
	0,  /* manualHangup */
	1492, /* MTU */
	PPPOECFGPARAM_LANTYPE_NAPT,
	0
	}
};


dhcpsCfgParam_t dhcpsCfgParamDefault =
{
	1, /* DHCP Server Status, 0:disable 1:enable */
	100, /* DHCP Server IP Pool Start IP */
	199, /* DHCP Server IP Pool End IP */
	"", /* domain name */	
	86400,	  /* dhcp lease time */ 
	0,	 
	{0, 0, 0, 0, 0, 0, 0, 0},
	{{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
	0,	//ConnType
	"", 	//user name
	"",        //password
	"",      //server name
	"#777",         //dial_number
	0,		//language type
	0
};

dhcpcCfgParam_t dhcpcCfgParamDefault =
{
	0,		        /* cmac valid */	
	{0,0,0,0,0,0},	/* clone mac address */
	0,				/* host name -- Min Lee */
	0
};

pppCfgParam_t pppCfgParamDefault =
{
	"", 	//user name
	"",        //password
	"",      //server name
	"#777",         //dial_number
	PPPCFGPARAM_LANGTYPE_ENGLISH,	
	0
};  //peter and howard

dnsCfgParam_t dnsCfgParamDefault =
{
	1, /* DNS Server Status, 0:disable 1:enable */
	{168,95,1,1},
	{0,0,0,0},
	0	
};

mgmtCfgParam_t mgmtCfgParamDefault[MAX_MGMT_USER] =
{
	{"admin", "\0",1,0,0,0,8080,1,1,0},
	{"user",  "\0",0,0,0,0,   0,0,0,0}
};

aclGlobalCfgParam_t aclGlobalCfgParamDefault =
{
	ACLGLOBALCFGPARAM_ALLOWALLBUTACL,	// equivalent to "drop" rule
	1,
	0
};

// Dino Chang modify, 2004/11/08
aclCfgParam_t aclCfgParamDefault[MAX_PPPOE_SESSION][MAX_ACL] =
{
    {{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0, 0},
    {{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0, 0},
   	{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0, 0},
   	{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0, 0},
   	{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0, 0},
   	{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0, 0},
   	{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0, 0},
   	{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0, 0}},

    {{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0, 0},
    {{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0, 0},
   	{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0, 0},
   	{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0, 0},
   	{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0, 0},
   	{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0, 0},
   	{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0, 0},
   	{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0, 0}}

};


serverpCfgParam_t serverpCfgParamDefault[MAX_PPPOE_SESSION][MAX_SERVER_PORT] =
{
   {{"Virtual Server FTP",{0,0,0,0},21,21,21,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0, 0},
	{"Virtual Server HTTP",{0,0,0,0},80,80,80,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0, 0},
	{"Virtual Server HTTPS",{0,0,0,0},443,443,443,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0, 0},
	{"Virtual Server DNS",{0,0,0,0},53,53,53,SERVERPCFGPARAM_PROTOCOL_UDP,0,0,0,0,0,0, 0},
	{"Virtual Server SMTP",{0,0,0,0},25,25,25,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0, 0},
	{"Virtual Server POP3",{0,0,0,0},110,110,110,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0, 0},
	{"Virtual Server Telnet",{0,0,0,0},23,23,23,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0, 0},
	{"IPSec",{0,0,0,0},500,500,500,SERVERPCFGPARAM_PROTOCOL_UDP,0,0,0,0,0,0, 0},
	{"PPTP",{0,0,0,0},1723,1723,1723,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0, 0},
	{"NetMeeting",{0,0,0,0},1720,1720,1720,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0}},
	
   {{"Virtual Server FTP",{0,0,0,0},21,21,21,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0, 0},
	{"Virtual Server HTTP",{0,0,0,0},80,80,80,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0, 0},
	{"Virtual Server HTTPS",{0,0,0,0},443,443,443,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0, 0},
	{"Virtual Server DNS",{0,0,0,0},53,53,53,SERVERPCFGPARAM_PROTOCOL_UDP,0,0,0,0,0,0, 0},
	{"Virtual Server SMTP",{0,0,0,0},25,25,25,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0, 0},
	{"Virtual Server POP3",{0,0,0,0},110,110,110,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0, 0},
	{"Virtual Server Telnet",{0,0,0,0},23,23,23,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0, 0},
	{"IPSec",{0,0,0,0},500,500,500,SERVERPCFGPARAM_PROTOCOL_UDP,0,0,0,0,0,0, 0},
	{"PPTP",{0,0,0,0},1723,1723,1723,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0, 0},
	{"NetMeeting",{0,0,0,0},1720,1720,1720,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0, 0}}
};

specialapCfgParam_t specialapCfgParamDefault[MAX_PPPOE_SESSION][MAX_SPECIAL_AP] =
{
   {{"Battle.net",SPECIALACPCFGPARAM_PROTOCOL_BOTH,"6112",SPECIALACPCFGPARAM_PROTOCOL_BOTH,6112,6112,0, 0},
	{"Dialpad",SPECIALACPCFGPARAM_PROTOCOL_BOTH,"51200-51210",SPECIALACPCFGPARAM_PROTOCOL_BOTH,7175,7175,0, 0},
	{"ICU II",SPECIALACPCFGPARAM_PROTOCOL_BOTH,"2000-2085",SPECIALACPCFGPARAM_PROTOCOL_BOTH,2019,2019,0, 0},
	{"MSN Gaming Zone",SPECIALACPCFGPARAM_PROTOCOL_TCP,"28800-29000",SPECIALACPCFGPARAM_PROTOCOL_TCP,6667,6667,0, 0},
	{"PC-to-Phone",SPECIALACPCFGPARAM_PROTOCOL_TCP,"12120-12122",SPECIALACPCFGPARAM_PROTOCOL_TCP,12053,12053,0, 0},
	{"Quick Time 4",SPECIALACPCFGPARAM_PROTOCOL_UDP,"6970-6999",SPECIALACPCFGPARAM_PROTOCOL_TCP,554,554,0, 0},
	{"",SPECIALACPCFGPARAM_PROTOCOL_TCP,"",SPECIALACPCFGPARAM_PROTOCOL_TCP,0,0,0,{0,0,0}},
	{"",SPECIALACPCFGPARAM_PROTOCOL_TCP,"",SPECIALACPCFGPARAM_PROTOCOL_TCP,0,0,0,{0,0,0}}},

   {{"Battle.net",SPECIALACPCFGPARAM_PROTOCOL_BOTH,"6112",SPECIALACPCFGPARAM_PROTOCOL_BOTH,6112,6112,0, 0},
	{"Dialpad",SPECIALACPCFGPARAM_PROTOCOL_BOTH,"51200-51210",SPECIALACPCFGPARAM_PROTOCOL_BOTH,7175,7175,0, 0},
	{"ICU II",SPECIALACPCFGPARAM_PROTOCOL_BOTH,"2000-2085",SPECIALACPCFGPARAM_PROTOCOL_BOTH,2019,2019,0, 0},
	{"MSN Gaming Zone",SPECIALACPCFGPARAM_PROTOCOL_TCP,"28800-29000",SPECIALACPCFGPARAM_PROTOCOL_TCP,6667,6667,0, 0},
	{"PC-to-Phone",SPECIALACPCFGPARAM_PROTOCOL_TCP,"12120-12122",SPECIALACPCFGPARAM_PROTOCOL_TCP,12053,12053,0, 0},
	{"Quick Time 4",SPECIALACPCFGPARAM_PROTOCOL_UDP,"6970-6999",SPECIALACPCFGPARAM_PROTOCOL_TCP,554,554,0, 0},
	{"",SPECIALACPCFGPARAM_PROTOCOL_TCP,"",SPECIALACPCFGPARAM_PROTOCOL_TCP,0,0,0, 0},
	{"",SPECIALACPCFGPARAM_PROTOCOL_TCP,"",SPECIALACPCFGPARAM_PROTOCOL_TCP,0,0,0, 0}}
	
//    {"ICUII v5.9",SPECIALACPCFGPARAM_PROTOCOL_BOTH,"2000-2003",SPECIALACPCFGPARAM_PROTOCOL_TCP,23562,23566,0,{0,0,0}},	
//    {"AOE II",SPECIALACPCFGPARAM_PROTOCOL_BOTH,"2300-2400",SPECIALACPCFGPARAM_PROTOCOL_BOTH,47624,47624,0,{0,0,0}},
//	{"ICQ 2000b~2003b",SPECIALACPCFGPARAM_PROTOCOL_TCP,"24500-24505",SPECIALACPCFGPARAM_PROTOCOL_TCP,5190,5190,0,{0,0,0}}, // do it with ALG
//	{"Yahoo MSG v5.6",SPECIALACPCFGPARAM_PROTOCOL_BOTH,"5000-5001,5055,5002",SPECIALACPCFGPARAM_PROTOCOL_TCP,5050,5050,0,{0,0,0}}, // do it with ALG
//	{"PPTP Server",SPECIALACPCFGPARAM_PROTOCOL_TCP,"1723",SPECIALACPCFGPARAM_PROTOCOL_TCP,1723,1723,0,{0,0,0}}, //do it with ALG
//	{"IPSEC",SPECIALACPCFGPARAM_PROTOCOL_UDP,"62514",SPECIALACPCFGPARAM_PROTOCOL_UDP,62514,62514,0,{0,0,0}}, // do it with ALG
//	{"mIRC v6.2",SPECIALACPCFGPARAM_PROTOCOL_TCP,"1024-5000",SPECIALACPCFGPARAM_PROTOCOL_TCP,6660,6669,0,{0,0,0}},
//	{"Dialpad",SPECIALACPCFGPARAM_PROTOCOL_UDP,"51200-51201",SPECIALACPCFGPARAM_PROTOCOL_UDP,51200,51201,0,{0,0,0}},
//	{"Paltalk",SPECIALACPCFGPARAM_PROTOCOL_UDP,"2090-2091",SPECIALACPCFGPARAM_PROTOCOL_UDP,2090,2091,0,{0,0,0}},
//	{"Starcraft",SPECIALACPCFGPARAM_PROTOCOL_UDP,"6112",SPECIALACPCFGPARAM_PROTOCOL_TCP,6112,6112,0,{0,0,0}},
};

tbldrvCfgParam_t tbldrvCfgParamDefault =
{
	1,    // auto add on
	1,    // auto delete on
	60,   // icmp timeout
	300,  // udp timeout
	3600, // tcp long timeout
	900,  // tcp med timeout
	20,    // tcp fast timeout
	0
};

dmzCfgParam_t dmzCfgParamDefault[MAX_PPPOE_SESSION] =
{
	//enable, ipaddress
	{0, 0},
	{0, 0},
	0
};

logCfgParam_t logCfgParamDefault[MAX_PPPOE_SESSION] =
{
	{0},	//disable all modules
	0 	//disable all modules
};

upnpCfgParam_t upnpCfgParamDefault =
{
	1,	// enable
	0
};

ddnsCfgParam_t ddnsCfgParamDefault =
{
	0,	// enable
	"\0",
	"\0",
	"\0",  //Added by Nick HO
	0,      //Added by Nick HO
	0
};

urlfilterCfgParam_t urlfilterCfgParamDefault[MAX_PPPOE_SESSION][MAX_URL_FILTER] =
{
	{{0,	"\0", 0, 0, 0 },{0,	"\0", 0, 0, 0 },{0,	"\0", 0, 0, 0 },{0,	"\0", 0, 0, 0 },{0,	"\0", 0, 0, 0 },{0,	"\0", 0, 0, 0 },{0,	"\0", 0, 0 , 0},{0,	"\0", 0, 0, 0 }},
	{{0,	"\0", 0, 0, 0 }, {0,	"\0", 0, 0, 0 },{0,	"\0", 0, 0, 0 },{0,	"\0", 0, 0, 0 },{0,	"\0", 0, 0, 0 },{0,	"\0", 0, 0, 0 },{0,	"\0", 0, 0, 0 },{0,	"\0", 0, 0, 0 }}	
};

macfilterCfgParam_t macfilterCfgParamDefault[MAX_PPPOE_SESSION][MAX_URL_FILTER] =
{
	{{0, "\0", 0, 0},{0, "\0", 0, 0},{0, "\0", 0, 0},{0, "\0", 0, 0},{0, "\0", 0, 0},{0, "\0", 0, 0},{0, "\0", 0, 0},{0, "\0", 0, 0}},
	{{0, "\0", 0, 0},{0, "\0", 0, 0},{0, "\0", 0, 0},{0, "\0", 0, 0},{0, "\0", 0, 0},{0, "\0", 0, 0},{0, "\0", 0, 0},{0, "\0", 0, 0}}
};

// Dino Chang 2005/02/15
tmssCfgParam_t tmssCfgParamDefault =
{
        0, // tmss_enable
        2, // show_web_time
        0, // auto_update
        2, // check_update_time
        0, // license_enable
        0, // pc_enable
        0, // pc_control
        {0}, // parent_pass
        {0, 0, 0, {0, 0, 0, 0, 0}, 0}, // general
        0, // pc_user_cnt
        3600, // user_idle_timeout
        {{0, 0, 1, {{0}, {0}, 0, {0, 0, 0, 0, 0}, 0}, 0},
         {0, 0, 1, {{0}, {0}, 0, {0, 0, 0, 0, 0}, 0}, 0},
         {0, 0, 1, {{0}, {0}, 0, {0, 0, 0, 0, 0}, 0}, 0},
         {0, 0, 1, {{0}, {0}, 0, {0, 0, 0, 0, 0}, 0}, 0},
         {0, 0, 1, {{0}, {0}, 0, {0, 0, 0, 0, 0}, 0}, 0},
         {0, 0, 1, {{0}, {0}, 0, {0, 0, 0, 0, 0}, 0}, 0},
         {0, 0, 1, {{0}, {0}, 0, {0, 0, 0, 0, 0}, 0}, 0},
         {0, 0, 1, {{0}, {0}, 0, {0, 0, 0, 0, 0}, 0}, 0},
         {0, 0, 1, {{0}, {0}, 0, {0, 0, 0, 0, 0}, 0}, 0}}, // pc_user
        0
};

#if 0
tmssCfgParam_t tmssCfgParamDefault =
{
	1, 2, 0, 2, 0, 0, 0, 0, 0, 0, 3600, 0, 0
};
#endif
// Dino

// Dino Chang 2004/12/30
tmssCInfoCfgParam_t tmssCInfoCfgParamDefault[PC_USER_LEN_S] = {
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0, 0},
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0, 0},
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0, 0},
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0, 0},
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0, 0},
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0, 0},
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0, 0},
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0, 0},
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0, 0},
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0, 0}
};
// Dino

timeCfgParam_t timeCfgParamDefault =
{
	0,
	5,
	"-08:00",
	"clock.stdtime.gov.tw",
	"time-b.nist.gov",
	"time.nist.gov",
	1,		//month
	1,		//day
	0,		//hour
	0,		//minute
	2004,	//year
	0
};

naptCfgParam_t naptCfgParamDefault =
{
	/* disabled, disabled, disabled */
	0,
	0,
	0,
	0,
	0
};

dosCfgParam_t dosCfgParamDefault[MAX_PPPOE_SESSION] =
{
	/* Disabled, Disabled, Don't ignore, SYN,FIN,UDP,ICMP,SRC_SYN,SRC_FIN,SRC_UDP,SRC_ICMP, SIPBLOCK_ENABLE, SIPBLOCK_PRISONTIME, portScan Level  */
	{0,0,0,50,50,50,50,30,30,30,30,0,120,0, 0},
	{0,0,0,50,50,50,50,30,30,30,30,0,120,0, 0}
};

algCfgParam_t algCfgParamDefault[MAX_PPPOE_SESSION] =
{
	{	0xffffffef,	//enable all ALG, except NetMeeting
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // ALG Server IP List
		0
	},

	{	0xffffffef,	//enable all ALG, except NetMeeting
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // ALG Server IP List
		0
	}

};


wlanCfgParam_t wlanCfgParamDefault[MAX_WLAN_CARDS] =
{{
	aliasName:	"WLAN1",
	ssid:		"KR1",
	enable:		1,
	ap_mode:	0,		// Min Lee
	channel:	6,
        dataRateMode:   WLAN_DATA_RATE_ABG,
        rsv:		0,
	dataRate:		WLAN_DATA_RATE_AUTO,
	frag:			2346,
	rts:			2346,
	beacon:			100,
	preambleType:	WLAN_PREAMBLE_TYPE_LONG ,
	broadcastSSID:	0,
	txPower:		19,
	iapp:			1,

	cryptType:		CRYPT_NONE,
	authType:		AUTH_NONE,
	
	authRadiusPort:	1812,
	authRadiusIp:	0,
	authRadiusPasswd:"\0",
	
	
	acMacList:		{0},
	acComment:		{0},

	wdsMacList:		{0},
	wdsComment:		{0},
	rsv2:			{0},
	acEnable:		0,
	wdsEnable:		0,	
	key:			"\0",
	auto_channel:   1,		// 0:disable, 1:enable
	wep_type:		0,		// 0:disable, 1:enable
	wep_key_type:	0,		// 0:Hex, 1:ASCII
	wep_key_len:	1,		// 1:64Bits, 2:128Bits
	wep_def_key:	0,		// 0:key1, 1:key2, 2:key3, 3:key4
	key64:			{0},
	key128:			{0},
	passphrase:		"\0",
	turbo:			0, //howard 
	dtim:			3,
	rad_srv:		{{0, 1812, "\0", 0}, {0, 1812, "\0", 0}},
	field_end:		0
	
},
{
	aliasName:	"WLAN2",
	ssid:		"Realtek AP2", 
	enable:		0,
	ap_mode:	0,
	channel:	1,
    dataRateMode:   WLAN_DATA_RATE_ABG,
    rsv:			0,
	dataRate:		WLAN_DATA_RATE_AUTO,
	frag:			2346,
	rts:			2347,
	beacon:			100,
	preambleType:	WLAN_PREAMBLE_TYPE_LONG ,
	broadcastSSID:	0,
	txPower:		4,
	iapp:			1,
	cryptType:		CRYPT_NONE,
	authType:		AUTH_NONE,
	
	authRadiusPort:	1812,
	authRadiusIp:	0,
	authRadiusPasswd:"\0",


	acMacList:		{0},
	acComment:		{0},

	wdsMacList:		{0},
	wdsComment:		{0},	

	acEnable:		0,	
	wdsEnable:		0,
	rsv2:			{0},
	key:			"\0",
	auto_channel:   0,		// 0:disable, 1:enable
	wep_type:		0,		// 0:disable, 1:enable
	wep_key_type:	0,		// 0:Hex, 1:ASCII
	wep_key_len:	1,		// 1:64Bits, 2:128Bits
	wep_def_key:	0,		// 0:key1, 1:key2, 2:key3, 3:key4
	key64:			{0},
	key128:			{0},
	passphrase:		"\0",
	turbo:			0,
	dtim:			3,
	rad_srv:		{{0, 1812, "\0"}, {0, 1812, "\0"}},
	field_end:		0
}};

#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
pptpCfgParam_t pptpCfgParamDefault =
{
	{192,168,0,100},		//ip address
	{255,255,255,0},	//ipmask
	{192,168,0,2},		//pptp server ip address
	"\0",				//username
	"\0",				//password
	1452,			//mtu
	0,				// manualHangup
	1,				// autoReconnect
	0,				// demand
	PPTPCFGPARAM_DIALSTATE_OFF,				// dialState
	0,
	0				// silentTimeout
};

l2tpCfgParam_t l2tpCfgParamDefault =
{
	{192,168,0,100},		//ip address
	{255,255,255,0},	//ipmask
	{192,168,0,2},		//pptp server ip address
	"\0",				//username
	"\0",				//password
	1452,			//mtu
	0,				// manualHangup
	1,				// autoReconnect
	0,				// demand
	L2TPCFGPARAM_DIALSTATE_OFF,				// dialState
	0,				// silentTimeout
	0
};

#if 0
dhcpL2tpCfgParam_t dhcpL2tpCfgParamDefault = 
{
	{192,168,0,2},		//pptp server ip address
	"\0",				//username
	"\0",				//password
	1452,			//mtu
	0,				// manualHangup
	1,				// autoReconnect
	0,				// demand
	L2TPCFGPARAM_DIALSTATE_OFF,				// dialState
	0				// silentTimeout
};
#endif


#endif

udpblockCfgParam_t udpblockCfgParamDefault[MAX_PPPOE_SESSION] =
{
	//enable, size
		{0,30},
		{0,30},
		0
};


routingCfgParam_t routingCfgParamDefault[MAX_ROUTING] =
{
	// route,mask,gateway,interface
		{0,0xffffffff,0,0, 0},
		{0,0xffffffff,0,0, 0},
		{0,0xffffffff,0,0, 0},
		{0,0xffffffff,0,0, 0},
		{0,0xffffffff,0,0, 0}		
};


qosCfgParam_t qosCfgParamDefault[MAX_QOS] = 
{
	//Policy-Based QoS
	{ POLICY_BASED_QOS, 	{  20, FALSE }, 0 },
	{ POLICY_BASED_QOS, 	{  80, FALSE }, 0 },
	{ POLICY_BASED_QOS, 	{  23, FALSE }, 0 },
	{ POLICY_BASED_QOS,	{  25, FALSE }, 0 },
	{ POLICY_BASED_QOS,	{110, FALSE }, 0 },
	{ POLICY_BASED_QOS,	{    0,  FALSE }, 0 },
	{ POLICY_BASED_QOS,	{    0,  FALSE }, 0 },
	{ POLICY_BASED_QOS,	{    0,  FALSE }, 0 },

	//Port-Based QoS
	{ PORT_BASED_QOS,	{    0,  FALSE, FALSE, RTL8651_BC_FULL, RTL8651_BC_FULL }, 0 },
	{ PORT_BASED_QOS,	{    1,  FALSE, FALSE, RTL8651_BC_FULL, RTL8651_BC_FULL }, 0 },
	{ PORT_BASED_QOS,	{    2,  FALSE, FALSE, RTL8651_BC_FULL, RTL8651_BC_FULL }, 0 },
	{ PORT_BASED_QOS,	{    3,  FALSE, FALSE, RTL8651_BC_FULL, RTL8651_BC_FULL }, 0 },
	{ PORT_BASED_QOS,	{    4,  FALSE, FALSE, RTL8651_BC_FULL, RTL8651_BC_FULL }, 0 }
};


rateLimitCfgParam_t rateLimitCfgParamDefault = 
{
	0,
	{
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE, 0 },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE, 0 },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE, 0 },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE, 0 },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE, 0 },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE, 0 },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE, 0 },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE, 0 },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE, 0 },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE, 0 },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE, 0 },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE, 0 },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE, 0 },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE, 0 },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE, 0 },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE, 0 },
	},
	0
};

ratioQosCfgParam_t ratioQosCfgParamDefault =
{
	0,
	{	/* max Rate, Rate Unit, remaining Ratio */
		100, 0, 100, 100	/* total 100 Mbps, remaining 100%, 100% */
	},
	{	/* enable, isSrc, ip, ipMask, protoType, s_port, e_port, queue, ratio */
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0, 0},
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0, 0},
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0, 0},
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0, 0},
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0, 0},
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0, 0},
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0, 0},
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0, 0},
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0, 0},
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0, 0}
		/* MAX_RATIO_QOS is 10 */
	},
	0
};



pbnatCfgParam_t pbnatCfgParamDefault[MAX_PPPOE_SESSION][MAX_PBNAT] =
{
	/* Session 0 */
	{ 0, 0, 0x00000000, 0 },
	{ 0, 0, 0x00000000, 0 },
	{ 0, 0, 0x00000000, 0 },
	{ 0, 0, 0x00000000, 0 },

	/* Session 1 */
	{ 0, 0, 0x00000000, 0 },
	{ 0, 0, 0x00000000, 0 },
	{ 0, 0, 0x00000000, 0 },
	{ 0, 0, 0x00000000, 0 }
};

#else //==== Non CFG_TLV
vlanCfgParam_t vlanCfgParamDefault[3] = 
{
    {8, (PHY4),(PHY4),      1480, 1, {{0x00, 0x00, 0xda, 0x11, 0x22, 0x58}}, {0,0}}, 
    {9, (PHY0|PHY1|PHY2|PHY3|PHYMII),(PHY0|PHY1|PHY2|PHY3|PHYMII), 1480, 1, {{0x00, 0x00, 0xda, 0x11, 0x22, 0x68}}, {0,0}}, 
    {0, 0x00,0x00,             0,    0, {{0,0,0,0,0,0}},                        {0,0}}
};

ifCfgParam_t ifCfgParamDefault[3] =
{
	{0, {0,0,0,0} ,{255,255,255,0},{0,0,0,0} ,{168,95,1,1}, {0,0,0,0},0, 1, IFCFGPARAM_CONNTYPE_3G, 0},
	{1, {192,168,0,1},{255,255,255,0},{192,168,0,1},{168,95,1,1}, {168,95,1,1},0, 1, 0, 0},
	{0, {0,0,0,0}      ,{0,0,0,0}      ,{0,0,0,0}      ,{0,0,0,0}  ,{0,0,0,0} , 0, 0, 0, 0}
};

routeCfgParam_t routeCfgParamDefault[] =
{
	{0, {0,0,0,0},     {0,0,0,0},       {0,0,0,0},       0}
};

#if 0
	{0, {172,20,1,255},0xffffffff,1,{0xff,0xff,0xff,0xff,0xff,0xff},{0,0}}, /* Local broadcast */
	/* Add static arp entries for the hosts on the WAN interface */
	{1, {192,168,0,255},0xffffffff,1,{0xff,0xff,0xff,0xff,0xff,0xff},{0,0}}, /* Local broadcast */// Modified by Nick Wang, 11/30/2004
#endif

arpCfgParam_t arpCfgParamDefault[] =
{
	/* Add static arp entrie for the hosts on the LAN interface, e.g. your PC */
	{0, {0,0,0,0},      0,         0,{{0,0,0,0,0,0}},                  {0,0}}
};


natCfgParam_t natCfgParamDefault =
{
	1, /* enable */
	1, /* hw accel */
	1, 	/* ipsec passthru */
	1, 	/* pptp passthru */
	0,  /* no l2tp passthru */
	0,	/* no mac filter -- Min Lee*/
	{0,0,0}	/* rsv */
};

pppoeCfgParam_t pppoeCfgParamDefault[MAX_PPPOE_SESSION] =
{
	{
	1, /* disabled/enabled */
	1, /* default session or not */
	0, /* is pppoe unnumbered ? */
	1, /* auto reconnect */	
	0, /* dial on demand */
	0, /* dial state */
	0, /* session id */
	0, /* silent time out */
	{172,21,1,254} ,{255,255,255,0},{172,21,1,250} ,{168,95,1,1},
	{192,168,1,1}, /* local host ip */
	"\0", /* user name */
	"\0", /* password */
	"\0", /* ac name */
	"\0", /* service name */
	{"\0","\0","\0","\0"}, /* destination network */
	{PPPOECFGPARAM_DESTNETTYPE_NONE,	PPPOECFGPARAM_DESTNETTYPE_NONE,	PPPOECFGPARAM_DESTNETTYPE_NONE,	PPPOECFGPARAM_DESTNETTYPE_NONE},
	{0,0,0,0},  /* unnumbered ip */
	{255,255,255,254},   /* unnumbered network mask */
	{0,0,0,0,0,0}, /* server mac */
	0xff, /* pppx */
	1, /* Unnumbered PPPoE NAPT */
	0, /* whichPppObjId */
	0,  /* manualHangup */
	1492, /* MTU */
	PPPOECFGPARAM_LANTYPE_NAPT
	},

	{
	1, /* disabled/enabled */
	0, /* default session or not */
	0, /* is pppoe unnumbered ? */
	1, /* auto reconnect */
	1, /* dial on demand */	
	0, /* dial state */
	0, /* session id */
	0, /* silent time out */
	
	{172,22,1,254} ,{255,255,255,0},{172,22,1,250} ,{168,95,1,1},
	{192,168,1,1}, /* local host ip */
	"\0", /* user name */
	"\0", /* password */
	"\0", /* AC name */
	"\0", /* service name */
	{"\0","\0","\0","\0"}, /* destination network */
	{PPPOECFGPARAM_DESTNETTYPE_NONE,	PPPOECFGPARAM_DESTNETTYPE_NONE,	PPPOECFGPARAM_DESTNETTYPE_NONE,	PPPOECFGPARAM_DESTNETTYPE_NONE},
	{0,0,0,0},  /* unnumbered ip */
	{255,255,255,254},  /* unnumbered network mask */
	{0,0,0,0,0,0}, /* server mac */
	0xff, /* pppx */
	0, /* rsv1 */
	0, /* whichPppObjId */
	0,  /* manualHangup */
	1492, /* MTU */
	PPPOECFGPARAM_LANTYPE_NAPT
	}
};


dhcpsCfgParam_t dhcpsCfgParamDefault =
{
	1, /* DHCP Server Status, 0:disable 1:enable */
	100, /* DHCP Server IP Pool Start IP */
	199, /* DHCP Server IP Pool End IP */
	"", /* domain name */	
	86400,	  /* dhcp lease time */ 
	0, 
	{0, 0, 0, 0, 0, 0, 0, 0},
	{{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
	0,	//ConnType
	"KR1", 	//user name
	"kr1",        //password
	"verizion",      //server name
	"#777",         //dial_number
	0		//language type	
};

dhcpcCfgParam_t dhcpcCfgParamDefault =
{
	0,		        /* cmac valid */	
	{0,0,0,0,0,0},	/* clone mac address */
	0				/* host name -- Min Lee */
};

dnsCfgParam_t dnsCfgParamDefault =
{
	1, /* DNS Server Status, 0:disable 1:enable */
	{168,95,1,1},
	{0,0,0,0}	
};

mgmtCfgParam_t mgmtCfgParamDefault[MAX_MGMT_USER] =
{
	{"admin", "\0",1,0,0,0,8080,1,1,0},
	{"user",  "\0",0,0,0,0,   0,0,0,0}
};

aclGlobalCfgParam_t aclGlobalCfgParamDefault =
{
	ACLGLOBALCFGPARAM_ALLOWALLBUTACL,	// equivalent to "drop" rule
	1,
	{0,0}
};

// Dino Chang modify, 2004/11/08
aclCfgParam_t aclCfgParamDefault[MAX_PPPOE_SESSION][MAX_ACL] =
{
        {{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0},
        {{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0},
   	{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0},
   	{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0},
   	{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0},
   	{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0},
   	{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0},
   	{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0}},

        {{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0},
        {{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0},
   		{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0},
   		{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0},
   		{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0},
   		{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0},
   		{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0},
   		{{0,0,0,0},0,0,0,ACLCFGPARAM_PROTOCOL_TCP,{0,0,0},"",{0,0,0,0},{0,0,0,0},{0,0,0,0},0,0,0,0,0,0,0,0,0, 0}}

};
// Dino Chang

serverpCfgParam_t serverpCfgParamDefault[MAX_PPPOE_SESSION][MAX_SERVER_PORT] =
{
   {{"Virtual Server FTP",{0,0,0,0},21,21,21,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0},
	{"Virtual Server HTTP",{0,0,0,0},80,80,80,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0},
	{"Virtual Server HTTPS",{0,0,0,0},443,443,443,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0},
	{"Virtual Server DNS",{0,0,0,0},53,53,53,SERVERPCFGPARAM_PROTOCOL_UDP,0,0,0,0,0,0},
	{"Virtual Server SMTP",{0,0,0,0},25,25,25,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0},
	{"Virtual Server POP3",{0,0,0,0},110,110,110,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0},
	{"Virtual Server Telnet",{0,0,0,0},23,23,23,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0},
	{"IPSec",{0,0,0,0},500,500,500,SERVERPCFGPARAM_PROTOCOL_UDP,0,0,0,0,0,0},
	{"PPTP",{0,0,0,0},1723,1723,1723,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0},
	{"NetMeeting",{0,0,0,0},1720,1720,1720,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0}},
	
   {{"Virtual Server FTP",{0,0,0,0},21,21,21,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0},
	{"Virtual Server HTTP",{0,0,0,0},80,80,80,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0},
	{"Virtual Server HTTPS",{0,0,0,0},443,443,443,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0},
	{"Virtual Server DNS",{0,0,0,0},53,53,53,SERVERPCFGPARAM_PROTOCOL_UDP,0,0,0,0,0,0},
	{"Virtual Server SMTP",{0,0,0,0},25,25,25,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0},
	{"Virtual Server POP3",{0,0,0,0},110,110,110,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0},
	{"Virtual Server Telnet",{0,0,0,0},23,23,23,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0},
	{"IPSec",{0,0,0,0},500,500,500,SERVERPCFGPARAM_PROTOCOL_UDP,0,0,0,0,0,0},
	{"PPTP",{0,0,0,0},1723,1723,1723,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0},
	{"NetMeeting",{0,0,0,0},1720,1720,1720,SERVERPCFGPARAM_PROTOCOL_TCP,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0},
	{"",{0,0,0,0},0,0,0,0,0,0,0,0,0,0}}
};

specialapCfgParam_t specialapCfgParamDefault[MAX_PPPOE_SESSION][MAX_SPECIAL_AP] =
{
   {{"Battle.net",SPECIALACPCFGPARAM_PROTOCOL_BOTH,"6112",SPECIALACPCFGPARAM_PROTOCOL_BOTH,6112,6112,0,{0,0,0}},
	{"Dialpad",SPECIALACPCFGPARAM_PROTOCOL_BOTH,"51200-51210",SPECIALACPCFGPARAM_PROTOCOL_BOTH,7175,7175,0,{0,0,0}},
	{"ICU II",SPECIALACPCFGPARAM_PROTOCOL_BOTH,"2000-2085",SPECIALACPCFGPARAM_PROTOCOL_BOTH,2019,2019,0,{0,0,0}},
	{"MSN Gaming Zone",SPECIALACPCFGPARAM_PROTOCOL_TCP,"28800-29000",SPECIALACPCFGPARAM_PROTOCOL_TCP,6667,6667,0,{0,0,0}},
	{"PC-to-Phone",SPECIALACPCFGPARAM_PROTOCOL_TCP,"12120-12122",SPECIALACPCFGPARAM_PROTOCOL_TCP,12053,12053,0,{0,0,0}},
	{"Quick Time 4",SPECIALACPCFGPARAM_PROTOCOL_UDP,"6970-6999",SPECIALACPCFGPARAM_PROTOCOL_TCP,554,554,0,{0,0,0}},
	{"",SPECIALACPCFGPARAM_PROTOCOL_TCP,"",SPECIALACPCFGPARAM_PROTOCOL_TCP,0,0,0,{0,0,0}},
	{"",SPECIALACPCFGPARAM_PROTOCOL_TCP,"",SPECIALACPCFGPARAM_PROTOCOL_TCP,0,0,0,{0,0,0}}},

   {{"Battle.net",SPECIALACPCFGPARAM_PROTOCOL_BOTH,"6112",SPECIALACPCFGPARAM_PROTOCOL_BOTH,6112,6112,0,{0,0,0}},
	{"Dialpad",SPECIALACPCFGPARAM_PROTOCOL_BOTH,"51200-51210",SPECIALACPCFGPARAM_PROTOCOL_BOTH,7175,7175,0,{0,0,0}},
	{"ICU II",SPECIALACPCFGPARAM_PROTOCOL_BOTH,"2000-2085",SPECIALACPCFGPARAM_PROTOCOL_BOTH,2019,2019,0,{0,0,0}},
	{"MSN Gaming Zone",SPECIALACPCFGPARAM_PROTOCOL_TCP,"28800-29000",SPECIALACPCFGPARAM_PROTOCOL_TCP,6667,6667,0,{0,0,0}},
	{"PC-to-Phone",SPECIALACPCFGPARAM_PROTOCOL_TCP,"12120-12122",SPECIALACPCFGPARAM_PROTOCOL_TCP,12053,12053,0,{0,0,0}},
	{"Quick Time 4",SPECIALACPCFGPARAM_PROTOCOL_UDP,"6970-6999",SPECIALACPCFGPARAM_PROTOCOL_TCP,554,554,0,{0,0,0}},
	{"",SPECIALACPCFGPARAM_PROTOCOL_TCP,"",SPECIALACPCFGPARAM_PROTOCOL_TCP,0,0,0,{0,0,0}},
	{"",SPECIALACPCFGPARAM_PROTOCOL_TCP,"",SPECIALACPCFGPARAM_PROTOCOL_TCP,0,0,0,{0,0,0}}}
	
//    {"ICUII v5.9",SPECIALACPCFGPARAM_PROTOCOL_BOTH,"2000-2003",SPECIALACPCFGPARAM_PROTOCOL_TCP,23562,23566,0,{0,0,0}},	
//    {"AOE II",SPECIALACPCFGPARAM_PROTOCOL_BOTH,"2300-2400",SPECIALACPCFGPARAM_PROTOCOL_BOTH,47624,47624,0,{0,0,0}},
//	{"ICQ 2000b~2003b",SPECIALACPCFGPARAM_PROTOCOL_TCP,"24500-24505",SPECIALACPCFGPARAM_PROTOCOL_TCP,5190,5190,0,{0,0,0}}, // do it with ALG
//	{"Yahoo MSG v5.6",SPECIALACPCFGPARAM_PROTOCOL_BOTH,"5000-5001,5055,5002",SPECIALACPCFGPARAM_PROTOCOL_TCP,5050,5050,0,{0,0,0}}, // do it with ALG
//	{"PPTP Server",SPECIALACPCFGPARAM_PROTOCOL_TCP,"1723",SPECIALACPCFGPARAM_PROTOCOL_TCP,1723,1723,0,{0,0,0}}, //do it with ALG
//	{"IPSEC",SPECIALACPCFGPARAM_PROTOCOL_UDP,"62514",SPECIALACPCFGPARAM_PROTOCOL_UDP,62514,62514,0,{0,0,0}}, // do it with ALG
//	{"mIRC v6.2",SPECIALACPCFGPARAM_PROTOCOL_TCP,"1024-5000",SPECIALACPCFGPARAM_PROTOCOL_TCP,6660,6669,0,{0,0,0}},
//	{"Dialpad",SPECIALACPCFGPARAM_PROTOCOL_UDP,"51200-51201",SPECIALACPCFGPARAM_PROTOCOL_UDP,51200,51201,0,{0,0,0}},
//	{"Paltalk",SPECIALACPCFGPARAM_PROTOCOL_UDP,"2090-2091",SPECIALACPCFGPARAM_PROTOCOL_UDP,2090,2091,0,{0,0,0}},
//	{"Starcraft",SPECIALACPCFGPARAM_PROTOCOL_UDP,"6112",SPECIALACPCFGPARAM_PROTOCOL_TCP,6112,6112,0,{0,0,0}},
};

tbldrvCfgParam_t tbldrvCfgParamDefault =
{
	1,    // auto add on
	1,    // auto delete on
	60,   // icmp timeout
	300,  // udp timeout
	3600, // tcp long timeout
	900,  // tcp med timeout
	20    // tcp fast timeout
};

dmzCfgParam_t dmzCfgParamDefault[MAX_PPPOE_SESSION] =
{
	//enable, ipaddress
	{0, 0},
	{0, 0}	
};

logCfgParam_t logCfgParamDefault[MAX_PPPOE_SESSION] =
{
	{0},	//disable all modules
	{0} 	//disable all modules
};

upnpCfgParam_t upnpCfgParamDefault =
{
	1	// enable
};

ddnsCfgParam_t ddnsCfgParamDefault =
{
	0,	// enable
	"\0",
	"\0",
	"\0",  //Added by Nick HO
	0      //Added by Nick HO
};

urlfilterCfgParam_t urlfilterCfgParamDefault[MAX_PPPOE_SESSION][MAX_URL_FILTER] =
{
	{{0,	"\0", 0, 0 },{0,	"\0", 0, 0 },{0,	"\0", 0, 0 },{0,	"\0", 0, 0 },{0,	"\0", 0, 0 },{0,	"\0", 0, 0 },{0,	"\0", 0, 0 },{0,	"\0", 0, 0 }},
	{{0,	"\0", 0, 0 }, {0,	"\0" },{0,	"\0" },{0,	"\0" },{0,	"\0" },{0,	"\0" },{0,	"\0" },{0,	"\0" }}	
};

macfilterCfgParam_t macfilterCfgParamDefault[MAX_PPPOE_SESSION][MAX_URL_FILTER] =
{
	{{0, "\0", 0},{0, "\0", 0},{0, "\0", 0},{0, "\0", 0},{0, "\0", 0},{0, "\0", 0},{0, "\0", 0},{0, "\0", 0}},
	{{0, "\0", 0},{0, "\0", 0},{0, "\0", 0},{0, "\0", 0},{0, "\0", 0},{0, "\0", 0},{0, "\0", 0},{0, "\0", 0}}
};

// Dino Chang 2005/02/15

tmssCfgParam_t tmssCfgParamDefault =
{
        1, // tmss_enable
        2, // show_web_time
        0, // auto_update
        2, // check_update_time
        0, // license_enable
        0, // pc_enable
        0, // pc_control
        {0}, // parent_pass
        {0, 0, 0, {0, 0, 0, 0}}, // general
        0, // pc_user_cnt
        3600, // user_idle_timeout
        {{0, 0, 1, {{0}, {0}, 0, {0, 0, 0, 0}}},
         {0, 0, 1, {{0}, {0}, 0, {0, 0, 0, 0}}},
         {0, 0, 1, {{0}, {0}, 0, {0, 0, 0, 0}}},
         {0, 0, 1, {{0}, {0}, 0, {0, 0, 0, 0}}},
         {0, 0, 1, {{0}, {0}, 0, {0, 0, 0, 0}}},
         {0, 0, 1, {{0}, {0}, 0, {0, 0, 0, 0}}},
         {0, 0, 1, {{0}, {0}, 0, {0, 0, 0, 0}}},
         {0, 0, 1, {{0}, {0}, 0, {0, 0, 0, 0}}},
         {0, 0, 1, {{0}, {0}, 0, {0, 0, 0, 0}}}} // pc_user
};

// Dino Chang 2004/12/30
tmssCInfoCfgParam_t tmssCInfoCfgParamDefault[PC_USER_LEN_S] = {
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0},
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0},
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0},
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0},
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0},
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0},
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0},
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0},
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0},
        {{0, 0, 0, 0, 0, 0}, 0, 0, 0}
};
// Dino

timeCfgParam_t timeCfgParamDefault =
{
	0,
	5,
	"-08:00",
	"clock.stdtime.gov.tw",
	"time-b.nist.gov",
	"time.nist.gov",
	1,		//month
	1,		//day
	0,		//hour
	0,		//minute
	2004	//year
};

naptCfgParam_t naptCfgParamDefault =
{
	/* disabled, disabled, disabled */
	0,
	0,
	0,
	0
};

dosCfgParam_t dosCfgParamDefault[MAX_PPPOE_SESSION] =
{
	/* Disabled, Disabled, Don't ignore, SYN,FIN,UDP,ICMP,SRC_SYN,SRC_FIN,SRC_UDP,SRC_ICMP, SIPBLOCK_ENABLE, SIPBLOCK_PRISONTIME, portScan Level  */
	{0,0,0,50,50,50,50,30,30,30,30,0,120, 0},
	{0,0,0,50,50,50,50,30,30,30,30,0,120, 0}
};

algCfgParam_t algCfgParamDefault[MAX_PPPOE_SESSION] =
{
	{	0xffffffec,	//enable all ALG, except NetMeeting
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} // ALG Server IP List
	},

	{	0xffffffec,	//enable all ALG, except NetMeeting
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} // ALG Server IP List
	}

};


wlanCfgParam_t wlanCfgParamDefault[MAX_WLAN_CARDS] =
{{
	aliasName:	"WLAN1",
	ssid:		"default",
	enable:		1,
	ap_mode:	0,		// Min Lee
	channel:	6,
    dataRateMode:   WLAN_DATA_RATE_ABG,
    rsv:		0,
	dataRate:		WLAN_DATA_RATE_AUTO,
	frag:			2346,
	rts:			2346,
	beacon:			100,
	preambleType:	WLAN_PREAMBLE_TYPE_LONG ,
	broadcastSSID:	0,
	txPower:		17,
	iapp:			1,

	cryptType:		CRYPT_NONE,
	authType:		AUTH_NONE,
	
	authRadiusPort:	1812,
	authRadiusIp:	0,
	authRadiusPasswd:"\0",
	
	
	acMacList:		{0},
	acComment:		{0},

	wdsMacList:		{0},
	wdsComment:		{0},
	rsv2:			{0},
	acEnable:		0,
	wdsEnable:		0,	
	key:			"\0",
	auto_channel:   0,		// 0:disable, 1:enable
	wep_type:		0,		// 0:disable, 1:enable
	wep_key_type:	0,		// 0:Hex, 1:ASCII
	wep_key_len:	1,		// 1:64Bits, 2:128Bits
	wep_def_key:	0,		// 0:key1, 1:key2, 2:key3, 3:key4
	key64:			{0},
	key128:			{0},
	passphrase:		"\0",
	turbo:			0,
	dtim:			3,
	rad_srv:			{{0, 1812, "\0"}, {0, 1812, "\0"}}
	
},
{
	aliasName:	"WLAN2",
	ssid:		"Realtek AP2", 
	enable:		0,
	ap_mode:	0,
	channel:	1,
    dataRateMode:   WLAN_DATA_RATE_ABG,
    rsv:			0,
	dataRate:		WLAN_DATA_RATE_AUTO,
	frag:			2346,
	rts:			2347,
	beacon:			100,
	preambleType:	WLAN_PREAMBLE_TYPE_LONG ,
	broadcastSSID:	0,
	txPower:		4,
	iapp:			1,
	cryptType:		CRYPT_NONE,
	authType:		AUTH_NONE,
	
	authRadiusPort:	1812,
	authRadiusIp:	0,
	authRadiusPasswd:"\0",


	acMacList:		{0},
	acComment:		{0},

	wdsMacList:		{0},
	wdsComment:		{0},	

	acEnable:		0,	
	wdsEnable:		0,
	rsv2:			{0},
	key:			"\0",
	auto_channel:   0,		// 0:disable, 1:enable
	wep_type:		0,		// 0:disable, 1:enable
	wep_key_type:	0,		// 0:Hex, 1:ASCII
	wep_key_len:	1,		// 1:64Bits, 2:128Bits
	wep_def_key:	0,		// 0:key1, 1:key2, 2:key3, 3:key4
	key64:			{0},
	key128:			{0},
	passphrase:		"\0",
	turbo:			0,
	dtim:			3,
	rad_srv:			{{0, 1812, "\0"}, {0, 1812, "\0"}}
}};

#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
pptpCfgParam_t pptpCfgParamDefault =
{
	{192,168,0,100},		//ip address
	{255,255,255,0},	//ipmask
	{192,168,0,2},		//pptp server ip address
	"\0",				//username
	"\0",				//password
	1452,			//mtu
	0,				// manualHangup
	1,				// autoReconnect
	0,				// demand
	PPTPCFGPARAM_DIALSTATE_OFF,				// dialState
	0				// silentTimeout
};

l2tpCfgParam_t l2tpCfgParamDefault =
{
	{192,168,0,100},		//ip address
	{255,255,255,0},	//ipmask
	{192,168,0,2},		//pptp server ip address
	"\0",				//username
	"\0",				//password
	1452,			//mtu
	0,				// manualHangup
	1,				// autoReconnect
	0,				// demand
	L2TPCFGPARAM_DIALSTATE_OFF,				// dialState
	0				// silentTimeout
};

#if 0
dhcpL2tpCfgParam_t dhcpL2tpCfgParamDefault = 
{
	{192,168,0,2},		//pptp server ip address
	"\0",				//username
	"\0",				//password
	1452,			//mtu
	0,				// manualHangup
	1,				// autoReconnect
	0,				// demand
	L2TPCFGPARAM_DIALSTATE_OFF,				// dialState
	0				// silentTimeout
};
#endif


#endif

udpblockCfgParam_t udpblockCfgParamDefault[MAX_PPPOE_SESSION] =
{
	//enable, size
		{0,30},
		{0,30}
};


routingCfgParam_t routingCfgParamDefault[MAX_ROUTING] =
{
	// route,mask,gateway,interface
		{0,0xffffffff,0,0},
		{0,0xffffffff,0,0},
		{0,0xffffffff,0,0},
		{0,0xffffffff,0,0},
		{0,0xffffffff,0,0}		
};


qosCfgParam_t qosCfgParamDefault[MAX_QOS] = 
{
	//Policy-Based QoS
	{ POLICY_BASED_QOS, 	{  20, FALSE } },
	{ POLICY_BASED_QOS, 	{  80, FALSE } },
	{ POLICY_BASED_QOS, 	{  23, FALSE } },
	{ POLICY_BASED_QOS,	{  25, FALSE } },
	{ POLICY_BASED_QOS,	{110, FALSE } },
	{ POLICY_BASED_QOS,	{    0,  FALSE } },
	{ POLICY_BASED_QOS,	{    0,  FALSE } },
	{ POLICY_BASED_QOS,	{    0,  FALSE } },

	//Port-Based QoS
	{ PORT_BASED_QOS,	{    0,  FALSE, FALSE, RTL8651_BC_FULL, RTL8651_BC_FULL } },
	{ PORT_BASED_QOS,	{    1,  FALSE, FALSE, RTL8651_BC_FULL, RTL8651_BC_FULL } },
	{ PORT_BASED_QOS,	{    2,  FALSE, FALSE, RTL8651_BC_FULL, RTL8651_BC_FULL } },
	{ PORT_BASED_QOS,	{    3,  FALSE, FALSE, RTL8651_BC_FULL, RTL8651_BC_FULL } },
	{ PORT_BASED_QOS,	{    4,  FALSE, FALSE, RTL8651_BC_FULL, RTL8651_BC_FULL } }
};


rateLimitCfgParam_t rateLimitCfgParamDefault = 
{
	0,
	{
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE },
		{ 0, 0, 0xffffff00, 0, 65535, 0, FALSE, 0, 0, 0, 0, FALSE, FALSE }
	}
};

ratioQosCfgParam_t ratioQosCfgParamDefault =
{
	0,
	{	/* max Rate, Rate Unit, remaining Ratio */
		100, 0, 100, 100	/* total 100 Mbps, remaining 100%, 100% */
	},
	{	/* enable, isSrc, ip, ipMask, protoType, s_port, e_port, queue, ratio */
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0},
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0},
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0},
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0},
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0},
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0},
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0},
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0},
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0},
		{ 0, 0, 0, 0xffffffff, 0, 0, 0xffff, 0, 0}
		/* MAX_RATIO_QOS is 10 */
	}
};



pbnatCfgParam_t pbnatCfgParamDefault[MAX_PPPOE_SESSION][MAX_PBNAT] =
{
	/* Session 0 */
	{ 0, 0, 0x00000000 },
	{ 0, 0, 0x00000000 },
	{ 0, 0, 0x00000000 },
	{ 0, 0, 0x00000000 },

	/* Session 1 */
	{ 0, 0, 0x00000000 },
	{ 0, 0, 0x00000000 },
	{ 0, 0, 0x00000000 },
	{ 0, 0, 0x00000000 }
};

#endif //=== CFG_TLV

/* ============================================
	internal function declarations
     ============================================ */
extern int re865xIoctl(char *name,uint32 arg0, uint32 arg1, uint32 arg2, uint32 arg3);

// Dino Chang 2005/01/18
extern void check_tmss_lic(int sig);
// Dino

// local function declare
int INET_setroute(int action, int options, char **args);
int setInterfaceIp(char *name,int ip, int netmask);
//uint32 getRomeCfgParamPtr(romeCfgParam_t ** ptr);
int do_exec(char *name, char *identity);
int do_kill(char *filename);
int upInterface(char *name);
void pppd_start(uint32 identity);
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
void pptp_start(uint32 identity);
void l2tp_start(uint32 identity);
#endif
void runDNSProxy();

uint32 configChanged = 0;
static rtl8651_tblDrvAclRule_t tempAclRule;

/* local function prototype declaration */
int32 pppd_stop(uint32 pppObjId);


void LogDebugInfo(FILE *fp)
{
		
		printf("ifCfgParam[0].ipAddr: %u.%u.%u.%u\n"
			,pRomeCfgParam->ifCfgParam[0].ipAddr[0]
			,pRomeCfgParam->ifCfgParam[0].ipAddr[1]
			,pRomeCfgParam->ifCfgParam[0].ipAddr[2]
			,pRomeCfgParam->ifCfgParam[0].ipAddr[3]);

		fprintf(fp,"ifCfgParam[0].ipAddr: %u.%u.%u.%u\n"
			,pRomeCfgParam->ifCfgParam[0].ipAddr[0]
			,pRomeCfgParam->ifCfgParam[0].ipAddr[1]
			,pRomeCfgParam->ifCfgParam[0].ipAddr[2]
			,pRomeCfgParam->ifCfgParam[0].ipAddr[3]);

		printf("ifCfgParam[0].ipMask: %u.%u.%u.%u\n"
			,pRomeCfgParam->ifCfgParam[0].ipMask[0]
			,pRomeCfgParam->ifCfgParam[0].ipMask[1]
			,pRomeCfgParam->ifCfgParam[0].ipMask[2]
			,pRomeCfgParam->ifCfgParam[0].ipMask[3]);

		fprintf(fp,"ifCfgParam[0].ipMask: %u.%u.%u.%u\n"
			,pRomeCfgParam->ifCfgParam[0].ipMask[0]
			,pRomeCfgParam->ifCfgParam[0].ipMask[1]
			,pRomeCfgParam->ifCfgParam[0].ipMask[2]
			,pRomeCfgParam->ifCfgParam[0].ipMask[3]);

		printf("ifCfgParam[0].gwAddr: %u.%u.%u.%u\n"
			,pRomeCfgParam->ifCfgParam[0].gwAddr[0]
			,pRomeCfgParam->ifCfgParam[0].gwAddr[1]
			,pRomeCfgParam->ifCfgParam[0].gwAddr[2]
			,pRomeCfgParam->ifCfgParam[0].gwAddr[3]);

		fprintf(fp,"ifCfgParam[0].gwAddr: %u.%u.%u.%u\n"
			,pRomeCfgParam->ifCfgParam[0].gwAddr[0]
			,pRomeCfgParam->ifCfgParam[0].gwAddr[1]
			,pRomeCfgParam->ifCfgParam[0].gwAddr[2]
			,pRomeCfgParam->ifCfgParam[0].gwAddr[3]);

		printf("ifCfgParam[0].dnsPrimaryAddr: %u.%u.%u.%u\n"
			,pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[0]
			,pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[1]
			,pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[2]
			,pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[3]);

		fprintf(fp,"ifCfgParam[0].dnsPrimaryAddr: %u.%u.%u.%u\n"
			,pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[0]
			,pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[1]
			,pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[2]
			,pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[3]);

	}

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

/*
 * Routine Name: vlanCfgFactoryDefaultFunction
 * Description: route cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void vlanCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(vlanCfgParam_t)*VLAN_LOAD_COUNT;
	vlanCfgParam_t tmpTab[VLAN_LOAD_COUNT];
	memset((void*)tmpTab, 0, mySize);
	memcpy((void*)tmpTab, (void*)vlanCfgParamDefault, sizeof(vlanCfgParamDefault));
	memcpy((void*)pBuf, (void*)tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end vlanCfgFactoryDefaultFunction */    


#ifdef CONFIG_RTL865X

int ReadPhysicalMemory32(unsigned int addr, unsigned int *value){
	char str[32];
	int ret=-1;
    sprintf(str,"0x%08x",addr);
	FILE *f=fopen("/proc/rtl865x/reg32","w+");
	if(f){
	    fputs(str,f);
		fgets(str,sizeof(str),f);
		*value=(unsigned) strtoul(str,NULL,0);
		fclose(f);
		ret=0;
	}
	return ret;
}

int WritePhysicalMemory32(unsigned int addr, unsigned int value){
	char str[32];
	int ret=-1;
    sprintf(str,"0x%08x",addr);
	FILE *f=fopen("/proc/rtl865x/reg32","w+");
	if(f){
	    fputs(str,f);
	    sprintf(str,"0x%08x",value);
	    fputs(str,f);
		fclose(f);
		ret=0;
	}
	return ret;
}

int _chip_is_shared_pci_mode(){
	unsigned int value;
	if(ReadPhysicalMemory32(0xbc805030,&value))
		printf("Can't read kernel memory via /proc filesystem. \n");
	if((value&0x80000000)==0){
		#ifdef CONFIG_PCI
			goto maybe8650B_208;
		#else
			return 0;
		#endif
	}
maybe8650B_208:	
	if(ReadPhysicalMemory32(0xbc805104,&value))
		printf("Can't read kernel memory via /proc filesystem. \n");
	value&=0x07f0ffff;
	if(0x05005788==value){
		//chip is 8650B 208 package with PCI bonding and PCI kernel option is enabled.
		return 1;
	}
	return 0;
}
#endif

/*
	memory dump utility
*/
void memDump (void *start, uint32 size, int8 * strHeader){
  int32 row, column, index, index2, max;
  uint8 *buf, ascii[17];
  int8 empty = ' ';

  if(!start ||(size==0))
  	return;
  buf = (uint8 *) start;

  /*
     16 bytes per line
   */
  if (strHeader)
    printf ("%s", strHeader);
  column = size % 16;
  row = (size / 16) + 1;
  for (index = 0; index < row; index++, buf += 16) {
      memset (ascii, 0, 17);
      printf ("\n%08x ", (memaddr) buf);

      max = (index == row - 1) ? column : 16;

      //Hex
      for (index2 = 0; index2 < max; index2++){
	  if (index2 == 8)
	    printf ("  ");
	  printf ("%02x ", (uint8) buf[index2]);
	  ascii[index2] = ((uint8) buf[index2] < 32) ? empty : buf[index2];
	}

      if (max != 16){
	  if (max < 8)
	    printf ("  ");
	  for (index2 = 16 - max; index2 > 0; index2--)
	    printf ("   ");
	}

      //ASCII
      printf ("  %s", ascii);
    }
  printf ("\n");
  return;
}

/* ============================================
	Semaphore Control in board.c
     ============================================ */
uint32 rtl8651_sem_init(uint8 val)
{
	uint32 i;

	if (pRomeCfgParam == NULL) {
		printf("FAILED : pRomeCfgParam is NULL\n");
		return 1;
	}

	/* set default value to ALL semaphores */
	for ( i = 0 ; i < MAX_USER_SEMAPHORE ; i ++ ) {
		pRomeCfgParam->semaphore[i] = val;
	}

	return 0;
}

void rtl8651_sem_wait(uint32 sem_type)
{
	if (sem_type >= MAX_USER_SEMAPHORE)
		return;	/* type does not exist */

	while (pRomeCfgParam->semaphore[sem_type] == 0)
		usleep(200);
	pRomeCfgParam->semaphore[sem_type] --;
}

void rtl8651_sem_post(uint32 sem_type)
{
	if (sem_type >= MAX_USER_SEMAPHORE)
		return;	/* type does not exist */

	pRomeCfgParam->semaphore[sem_type] ++;
}

/*
 * Routine Name: vlan_init
 * Description: configure vlan and create interface
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 vlan_init()
{
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_VLAN,(void*)pRomeCfgParam->vlanCfgParam, sizeof(pRomeCfgParam->vlanCfgParam))!=0)
	{
		printf("vlan_init: call cfgmgr_read fail\n");		
		/* take proper actions */
		return 1;
	}
	return 0;
}

/*
 * Routine Name: ifCfgFactoryDefaultFunction
 * Description: if cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void ifCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(ifCfgParam_t)*MAX_IP_INTERFACE;
	ifCfgParam_t tmpTab[MAX_IP_INTERFACE];
	
	memset((void*)tmpTab,0,sizeof(tmpTab));
	memcpy((void*)tmpTab,(void*)ifCfgParamDefault,sizeof(ifCfgParamDefault));
	memcpy((void*)pBuf, (void*)tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end ifCfgFactoryDefaultFunction */    

/*
 * Routine Name: wanlan_int
 * Description: configure wan/lan interface
 * Input:
 *   none
 * Output:
 *   none
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 wanlan_init(void)
{
	char           *argv[8];
	unsigned char  macStr[17];
	//char           *cp;
	uint32         i=0;
	uint32	lanMemberPorts=pRomeCfgParam->vlanCfgParam[1].memberPort;
	uint32 	wanMemberPorts = pRomeCfgParam->vlanCfgParam[0].memberPort;
	int32          ret=FAILED;
	FILE *f;
	
        // Temp solution, need to be verified later
        pRomeCfgParam->ifCfgParam[0].connType=IFCFGPARAM_CONNTYPE_3G;
	/* init wan part I */
	
	re865xIoctl("eth0",ADD_IF,0,0,0);
	
	if (pRomeCfgParam->ifCfgParam[0].connType  == IFCFGPARAM_CONNTYPE_DHCPC&& (pRomeCfgParam->dhcpcCfgParam.cmacValid == 1))
		memcpy((char*)&macStr,(char*)&(pRomeCfgParam->dhcpcCfgParam.cloneMac),6);
	else
	// Dino Chang 2005/02/16
	{
	// Dino
		bdinfo_getMac((macaddr_t*)macStr);
	// Dino Chang 2005/02/16
		// WAN mac will add 1
		macStr[5] = (macStr[5] + 1) & 0xff;
		if(macStr[5] == 0) { // 0xff, 4 + 1
			macStr[4] = (macStr[4] + 1) & 0xff;
			if(macStr[4] == 0) { // 0xff, 3 + 1
				macStr[3] = (macStr[3] + 1) & 0xff;
			}
		}
	}

	wanMemberPorts |= (1<<CONFIG_RTL865XB_3G_PORT);

	// Dino
	re865xIoctl("eth0",ADD_VLAN,pRomeCfgParam->vlanCfgParam[0].vid,wanMemberPorts,(uint32)&macStr);

	rtl8651_cfgCounter(0x1, wanMemberPorts);//Configure VLAN 8 member port using counter 1 to monitor
	/* init lan */
	sleep(1);
	bdinfo_getMac((macaddr_t*)macStr);

	
	#ifdef CONFIG_RTL865XB_3G
	{
		FILE *f;
		f=fopen("/var/bdinfo_mac.txt","w");
		fprintf(f,"%u\n%u\n%u\n%u\n%u\n%u\n",macStr[0],macStr[1],macStr[2],macStr[3],macStr[4],macStr[5]);
		fclose(f);
	}
	#endif

	//Reserve MAX_PPPOE_SESSION MACs...
	// Dino Chang 2004/12/16
	//macStr[5] = macStr[5]+1; //MAX_PPPOE_SESSION; /* Even if multiple PPPoE session, only one MAC address is used in WAN */
	// Dino

#ifdef CONFIG_RTL865XB_EXP_WLAN
	//printf("WLAN: Adding extport 6 (0x40)\n");
	lanMemberPorts|= 0x40; //add first extension port(used by WLAN) to LAN 
#endif
	
	lanMemberPorts&= ~0x20; //remove MII port 
	
	re865xIoctl("eth1",ADD_VLAN,pRomeCfgParam->vlanCfgParam[1].vid,lanMemberPorts,(uint32)&macStr);
	rtl8651_cfgCounter(0x2, lanMemberPorts);//Configure VLAN 9 member port using counter 2 to monitor

	setInterfaceIp("lo",0x7f000001,0xff000000);	
	re865xIoctl("eth1",SET_VLAN_TYPE,LL_VLAN,0,0);
	// Dino Chang 2004/11/30
	// we can specify network mask, not always 255.255.255.0
	re865xIoctl("eth1",SET_VLAN_IP,*(uint32*)(&(pRomeCfgParam->ifCfgParam[1].ipAddr)), *(uint32*)(&(pRomeCfgParam->ifCfgParam[1].ipMask)),0);
	//re865xIoctl("eth1",SET_VLAN_IP,*(uint32*)(&(pRomeCfgParam->ifCfgParam[1].ipAddr)),0xffffff00,0);
	// Dino Chang
	re865xIoctl("eth1",SET_NET_PARA,0,0,0);
	// Dino Chang 2004/11/30
	// we can specify network mask, not always 255.255.255.0
	setInterfaceIp("eth1",*(uint32*)(&(pRomeCfgParam->ifCfgParam[1].ipAddr)), *(uint32*)(&(pRomeCfgParam->ifCfgParam[1].ipMask)));
	//setInterfaceIp("eth1",*(uint32*)(&(pRomeCfgParam->ifCfgParam[1].ipAddr)),0xffffff00);
	// Dino Chang
	
	/* turn off auto learn due to url filtering 'forward' logging needs */
	rtl8651_enableNaptAutoAdd(0);

	DEBUG_P("mac address %02X:%02X:%02X:%02X:%02X:%02X\n",macStr[0],macStr[1],macStr[2],macStr[3],macStr[4],macStr[5]);

	DEBUG_P("wan address %x netmask %x\n",*(uint32*)&(pRomeCfgParam->ifCfgParam[0].ipAddr),*(uint32*)&(pRomeCfgParam->ifCfgParam[0].ipMask));

	DEBUG_P("lan address %x netmask %x\n",*(uint32*)&(pRomeCfgParam->ifCfgParam[1].ipAddr),*(uint32*)&(pRomeCfgParam->ifCfgParam[1].ipMask));

#if 0
	macStr[5] +=1;
	re865xIoctl("eth2",ADD_VLAN,0xa,1<<CONFIG_RTL865X_LOOPBACK_PORTNUMBER,(uint32)&macStr);	
	re865xIoctl("eth2",SET_VLAN_TYPE,LL_VLAN,0,0);
	DEBUG_P("pptp/WAN created.\n");
#endif

	/* set protocol stack used udp ports in DMZ (software path) */
/*
	rtl8651_flushProtoStackUsedUdpPorts();	
	rtl8651_addProtoStackUsedUdpPort(53);
	rtl8651_flushProtoStackServerUsedTcpPorts();		
	rtl8651_addProtoStackServerUsedTcpPort(80);
*/
	//start telnetd
	#if 0
	system("/bin/telnetd &");
	rtl8651_addProtoStackServerUsedTcpPort(23);	
	#endif

	/* init wan part II */
	if (pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_STATIC) 
	{
		uint32 netmask=0;
		rtl8651_setMtu(1500);	
		netmask|=(pRomeCfgParam->ifCfgParam[0].ipMask[0])<<24;
		netmask|=(pRomeCfgParam->ifCfgParam[0].ipMask[1])<<16;
		netmask|=(pRomeCfgParam->ifCfgParam[0].ipMask[2])<<8;
		netmask|=pRomeCfgParam->ifCfgParam[0].ipMask[3];
		
		//printf("cfg wan to static ...\n");
		re865xIoctl("eth0",SET_VLAN_TYPE,LL_VLAN,0,0);
		re865xIoctl("eth0",SET_VLAN_IP,*(uint32*)(&(pRomeCfgParam->ifCfgParam[0].ipAddr)),netmask,0);
		#if defined(CONFIG_RTL865X_DIAG_LED)
		{
			ipaddr_t gwWanIp,gwWanIpmask;		
			ipaddr_t gwLanIp,gwLanIpmask;
			memcpy((void*)&gwWanIp,pRomeCfgParam->ifCfgParam[0].ipAddr,4);
			memcpy((void*)&gwWanIpmask,pRomeCfgParam->ifCfgParam[0].ipMask,4);			
			memcpy((void*)&gwLanIp,pRomeCfgParam->ifCfgParam[1].ipAddr,4);
			memcpy((void*)&gwLanIpmask,pRomeCfgParam->ifCfgParam[1].ipMask,4);
			if ((gwWanIp&gwWanIpmask)==(gwLanIp&gwLanIpmask))
				re865xIoctl("eth0",RTL8651_IOCTL_DIAG_LED,5,0,0);
		}
		#endif /* CONFIG_RTL865X_DIAG_LED */
		re865xIoctl("eth0",ADD_GW,*(uint32*)(&(pRomeCfgParam->ifCfgParam[0].gwAddr)),netmask,0);
		re865xIoctl("eth0",SET_EXT_INTERFACE,*(uint32*)(&(pRomeCfgParam->ifCfgParam[0].ipAddr)),0,0);
		setInterfaceIp("eth0",*(uint32*)(&(pRomeCfgParam->ifCfgParam[0].ipAddr)),netmask);
		argv[0]="default";
		
		sprintf(macStr,"%u.%u.%u.%u",
			pRomeCfgParam->ifCfgParam[0].gwAddr[0],
			pRomeCfgParam->ifCfgParam[0].gwAddr[1],
			pRomeCfgParam->ifCfgParam[0].gwAddr[2],
			pRomeCfgParam->ifCfgParam[0].gwAddr[3]);
		argv[1]="gw";
		argv[2]=(char*)&macStr;
		argv[3]=NULL;
		INET_setroute(RTACTION_ADD, 0, argv);
		 runDNSProxy();
	} /* end ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_STATIC */	
	else if (pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_DHCPC)
	{
		printf("cfg wan to dhcp client ...\n");
		rtl8651_setMtu(1500);		
		/* clear ifCfgParam[0].ipAddr[] etc, dhcp client will fill these values later */
		memset((void*)pRomeCfgParam->ifCfgParam[0].ipAddr,0,4);
		memset((void*)pRomeCfgParam->ifCfgParam[0].ipMask,0,4);
		memset((void*)pRomeCfgParam->ifCfgParam[0].gwAddr,0,4);
		memset((void*)pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,0,4);
		memset((void*)pRomeCfgParam->ifCfgParam[0].dnsSecondaryAddr,0,4);
		setInterfaceIp("eth0",0,0);		
		re865xIoctl("eth0",SET_VLAN_TYPE,LL_VLAN,0,0);	
		rtl8651_addExtNetworkInterface((int8*)"eth0");
		udhcpc_start();
	}
	else if (pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_PPPOE)
	{
		//printf("cfg wan to single PPPoE ...\n");

		/* linux if up */
		upInterface("eth0");
				
		rtl8651_addNetworkIntf("eth0");
    		//rtl8651_setAsicNaptAutoAddDelete(FALSE, 1);
    		rtl8651_enableNaptAutoAdd(0);
    		/* note: for ip unnumbered case, reference  pppObjId=0 only
    		 *       ppp obj id equal to session number (0,1,2,3,...)
    		 */
	    	
		ret=rtl8651_specifyNetworkIntfLinkLayerType("eth0",RTL8651_LL_PPPOE,pRomeCfgParam->vlanCfgParam[0].vid);
		if (ret)
			printf("rtl8651_specifyNetworkIntfLinkLayerType: ret %d\n",ret);
		//for (i=0;i<MAX_PPPOE_SESSION;i++)
	    	for (i=0;i<1;i++)
			rtl8651_addPppoeSession(i,pRomeCfgParam->vlanCfgParam[0].vid);

		/* make this network interface for napt */
		//rtl8651_addExtNetworkInterface(int8 * ifName);
		ret=rtl8651_addExtNetworkInterface((int8*)"eth0");
		if (ret)
			printf("rtl8651_addExtNetworkInterface: ret %d\n",ret);


		ret=rtl8651_addRoute((ipaddr_t)0,(ipaddr_t)0,(int8*)"eth0",0);
		if (ret)
		{
			printf("rtl8651_addRoute: ret %d\n",ret);
		}		

		#if 1
		pppd_start(0); // start pppd on pppObjId 0
		#else
		for (i=0;i<1;i++)
		{
			char identity[4];
			char papStr[20];
			char pppOptionStr[20];

			#if 0
			/* pppoeCfgParam[].enable only used for multiple pppoe */
			if (pRomeCfgParam->pppoeCfgParam[i].enable==0)
			{
				printf("pppoe session #%u disabled ... \n", i);
				continue;
			}
			else
				printf("pppoe session #%u enabled ... \n",i);
			#endif
				
			tmp = malloc(160);
			f1 = fopen("/etc/ppp.options","r");
			
			sprintf(papStr,"/var/ppp/pap%u",i);
			f2 = fopen(papStr,"w+");
			sprintf(tmp,"%s * %s" ,pRomeCfgParam->pppoeCfgParam[i].username,pRomeCfgParam->pppoeCfgParam[i].password);
			fputs(tmp,f2);
			fclose(f2);
			
			sprintf(pppOptionStr,"/var/ppp/options%u",i);
			f2 = fopen(pppOptionStr,"w+");
			while (!feof(f1))
			{
				fgets(tmp,160,f1);
				if (!memcmp(tmp,"user",4))
					sprintf(tmp,"user %s\n",pRomeCfgParam->pppoeCfgParam[i].username);
				fputs(tmp,f2);
			}			
			fclose(f1);
			
			/* render idle option (auto disconnect) for pppd's reference */
			fprintf(f2,"idle %u\n",pRomeCfgParam->pppoeCfgParam[i].silentTimeout);			
			
			/* render dial-on-demand option for pppd's reference */
/*
			if (pRomeCfgParam->pppoeCfgParam[i].demand==1)
				fprintf(f2,"demand 1\n");
*/
			fclose(f2);
			free(tmp);
			pRomeCfgParam->pppoeCfgParam[i].dialState=PPPOECFGPARAM_DIALSTATE_DIALED_TRYING;
			sprintf(identity,"%u",i);
			do_exec("/bin/pppd",identity);
		} /* end for i */
		#endif
		
	} /* ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_PPPOE */
	else if (pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)
	{
		uint32 pppObjId=0;
		
		//printf("cfg wan to multiple PPPoE ...\n");

		/* linux if up */
		upInterface("eth0");
				
		rtl8651_addNetworkIntf("eth0");
    		//rtl8651_setAsicNaptAutoAddDelete(FALSE, 1);
    		rtl8651_enableNaptAutoAdd(0);

    		ret=rtl8651_specifyNetworkIntfLinkLayerType("eth0",RTL8651_LL_PPPOE,pRomeCfgParam->vlanCfgParam[0].vid);
		if (ret)
			printf("rtl8651_specifyNetworkIntfLinkLayerType: ret %d\n",ret);
			
	    	/* note: for ip unnumbered case, reference  pppObjId=0 only
	    	 *       ppp obj id equal to session number (0,1,2,3,...)
	    	 */
	    	for (i=0;i<MAX_PPPOE_SESSION;i++){
			rtl8651_addPppoeSession(i,pRomeCfgParam->vlanCfgParam[0].vid);
			if (pRomeCfgParam->pppoeCfgParam[i].defaultSession==1){
				ret=rtl8651_setPppoeDefaultDialSessionId(i);
				if (ret)
					printf("rtl8651_setPppoeDefaultDialSessionId: ret %d\n",ret);
				else
					printf("rtl8651_setPppoeDefaultDialSessionId(%d): ret %d\n",i,ret);
			}
			if(pRomeCfgParam->pppoeCfgParam[i].lanType==PPPOECFGPARAM_LANTYPE_UNNUMBERED)
				pppObjId=i;
		}

		/*make this network interface  for NAPT*/
		//rtl8651_addExtNetworkInterface(int8 * ifName);
		ret=rtl8651_addExtNetworkInterface((int8*)"eth0");
		if (ret)
			printf("rtl8651_addExtNetworkInterface: ret %d\n",ret);

		/* assign the pre-assigned global ip to gw */
		#if 1
		if(pRomeCfgParam->pppoeCfgParam[pppObjId].lanType==PPPOECFGPARAM_LANTYPE_UNNUMBERED)
		{		
			ipaddr_t ipStub=0x0b010101,ipMaskStub=0xfffffffd;
			//memcpy(pRomeCfgParam->pppoeCfgParam[pppObjId].unnumberedIpAddr,&ipStub,4);
			//memcpy(pRomeCfgParam->pppoeCfgParam[pppObjId].unnumberedIpMask,&ipMaskStub,4);
			memcpy((void*)&ipStub,pRomeCfgParam->pppoeCfgParam[pppObjId].unnumberedIpAddr,4);
			memcpy((void*)&ipMaskStub,pRomeCfgParam->pppoeCfgParam[pppObjId].unnumberedIpMask,4);
			//printf("rtl8651_addIpIntf(eth1): ip(0x%x) ipMask(0x%0x)\n",ipStub,ipMaskStub);
			ret=rtl8651_addIpIntf("eth1",(ipaddr_t)ipStub,(ipaddr_t)ipMaskStub);
			if (ret)
			{
				printf("rtl8651_addIpIntf: ret %d\n",ret);
			}
			/* initialize user protocol stack: set inside
			 * global ip address to LAN
			 * uClinux "ifconfig" command:
			 *    add alias:ifconfig eth1:1 10.1.1.2 netmask 255.255.255.0
			 *    del alias:ifconfig eth1:1 0.0.0.0		 
			 */
			{
				int 	ipValue;
				char	*av[5];
				
				char	pname[]="/bin/ifconfig";
				//static char ip_string[MAX_IP_STRING_LEN];
				//static char ipmask_string[MAX_IP_STRING_LEN];
				char	*ip_string;
				char	*ipmask_string;
				ip_string     = malloc(MAX_IP_STRING_LEN);
				ipmask_string = malloc(MAX_IP_STRING_LEN);
				ipValue=(int)ipStub;
				sprintf(ip_string,"%u.%u.%u.%u",(ipValue>>24)&0xff, (ipValue>>16)&0xff, (ipValue>>8)&0xff, ipValue&0xff);
				ipValue=(int)ipMaskStub;
				sprintf(ipmask_string,"%u.%u.%u.%u",(ipValue>>24)&0xff, (ipValue>>16)&0xff, (ipValue>>8)&0xff, ipValue&0xff);
				
				av[0]=pname;
				av[1]="eth1:1";
				av[2]=ip_string;
				av[3]="netmask";
				av[4]=ipmask_string;
				av[5]=NULL;
				runProgram(pname,av);
				free(ip_string);
				free(ipmask_string);
			}			
		}		
		#endif


		ret=rtl8651_addRoute((ipaddr_t)0,(ipaddr_t)0,(int8*)"eth0",0);
		if (ret)
		{
			printf("rtl8651_addRoute: ret %d\n",ret);
		}		

		for (i=0;i<MAX_PPPOE_SESSION;i++)
		{
			/* orlando remark 2003/10/16:
			 * only dial default session upon boot
			 * user has to manual dial additional sessions
			 */
			pRomeCfgParam->pppoeCfgParam[i].dialState=0;			 
			if (pRomeCfgParam->pppoeCfgParam[i].defaultSession == 1)
			{
				pppd_start(i); // start pppd on pppObjId x
			}
		}		
	} /* ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE */
	else if (pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED)
	{
		ipaddr_t ipStub;
		ipaddr_t ipMaskStub;
		//printf("cfg wan to unnumbered PPPoE ...\n");

		/* linux if up */
		upInterface("eth0");

		rtl8651_addNetworkIntf("eth0");
    		//rtl8651_setAsicNaptAutoAddDelete(FALSE, 1);
    		rtl8651_enableNaptAutoAdd(0);

		if (ret)
			printf("rtl8651_specifyNetworkIntfLinkLayerType: ret %d\n",ret);

		ret=rtl8651_specifyNetworkIntfLinkLayerType("eth0",RTL8651_LL_PPPOE,pRomeCfgParam->vlanCfgParam[0].vid);
		
	    	/* note: for ip unnumbered case, reference  pppObjId=0 only
	    	 *       ppp obj id equal to session number (0,1,2,3,...)
	    	 */
	    	for (i=0;i<1;i++)
			rtl8651_addPppoeSession(i,pRomeCfgParam->vlanCfgParam[0].vid);
		
		

		/* make this network interface for napt */
		//rtl8651_addExtNetworkInterface(int8 * ifName);
		ret=rtl8651_addExtNetworkInterface((int8*)"eth0");
		if (ret)
			printf("rtl8651_addExtNetworkInterface: ret %d\n",ret);

		/* assumption: ifCfgParam[0] is for wan interface */
		memcpy((void*)&ipMaskStub,pRomeCfgParam->pppoeCfgParam[0].unnumberedIpMask,4);
		ret=rtl8651_addIpUnnumbered("eth0","eth1", (ipaddr_t)ipMaskStub);
		if (ret)
			printf("rtl8651_addIpUnnumbered: ret %d\n",ret);		

		/* assign the pre-assigned global ip to gw */
		#if 1
		memcpy((void*)&ipStub,pRomeCfgParam->pppoeCfgParam[0].unnumberedIpAddr,4);
		//printf("rtl8651_addIpIntf(eth1): ip(0x%x) ipMask(0x%0x)\n",ipStub,ipMaskStub);
		ret=rtl8651_addIpIntf("eth1",(ipaddr_t)ipStub,(ipaddr_t)ipMaskStub);
		if (ret)
		{
			printf("rtl8651_addIpIntf: ret %d\n",ret);
		}
		#endif
		
		/* initialize user protocol stack: set inside
		 * global ip address to LAN
		 * uClinux "ifconfig" command:
		 *    add alias:ifconfig eth1:1 10.1.1.2 netmask 255.255.255.0
		 *    del alias:ifconfig eth1:1 0.0.0.0		 
		 */
		{
			int 	ipValue;
			char	*av[5];
			char	*ip_string;
			char	*ipmask_string;
			char	pname[]="/bin/ifconfig";

			ip_string     = malloc(MAX_IP_STRING_LEN);
			ipmask_string = malloc(MAX_IP_STRING_LEN);
			ipValue=(int)ipStub;
			sprintf(ip_string,"%u.%u.%u.%u",(ipValue>>24)&0xff, (ipValue>>16)&0xff, (ipValue>>8)&0xff, ipValue&0xff);
			ipValue=(int)ipMaskStub;
			sprintf(ipmask_string,"%u.%u.%u.%u",(ipValue>>24)&0xff, (ipValue>>16)&0xff, (ipValue>>8)&0xff, ipValue&0xff);
			
			av[0]=pname;
			av[1]="eth1:1";
			av[2]=ip_string;
			av[3]="netmask";
			av[4]=ipmask_string;
			av[5]=NULL;
			runProgram(pname,av);
			free(ip_string);
			free(ipmask_string);
		}


		ret=rtl8651_addRoute((ipaddr_t)0,(ipaddr_t)0,(int8*)"eth0",0);
		if (ret)
		{
			printf("rtl8651_addRoute: ret %d\n",ret);
		}
		
		#if 1
		pppd_start(0); // start pppd on pppObjId x
		#else
		for (i=0;i<MAX_PPPOE_SESSION;i++)
		//for (i=0;i<1;i++)
		{
			char identity[4];
			char papStr[20];
			char pppOptionStr[20];

			#if 0
			/* pppoeCfgParam[].enable only used for multiple pppoe */
			if (pRomeCfgParam->pppoeCfgParam[i].enable==0)
			{
				printf("pppoe session #%u disabled ... \n", i);
				continue;
			}
			else
				printf("pppoe session #%u enabled ... \n",i);
			#endif
						
			tmp = malloc(160);
			f1 = fopen("/etc/ppp.options","r");
			
			sprintf(papStr,"/var/ppp/pap%u",i);
			f2 = fopen(papStr,"w+");
			sprintf(tmp,"%s * %s" ,pRomeCfgParam->pppoeCfgParam[i].username,pRomeCfgParam->pppoeCfgParam[i].password);
			fputs(tmp,f2);
			fclose(f2);

			sprintf(pppOptionStr,"/var/ppp/options%u",i);
			f2 = fopen(pppOptionStr,"w+");
			while (!feof(f1))
			{
				fgets(tmp,160,f1);
				if (!memcmp(tmp,"user",4))
					sprintf(tmp,"user %s\n",pRomeCfgParam->pppoeCfgParam[i].username);
				fputs(tmp,f2);
			}
			
			/* put requested ip in the option file
			 * option format: <local IP address>:<remote IP address>
			 */
			fprintf(f2,"%u.%u.%u.%u:0.0.0.0\n",
				pRomeCfgParam->pppoeCfgParam[i].unnumberedIpAddr[0],
				pRomeCfgParam->pppoeCfgParam[i].unnumberedIpAddr[1],
				pRomeCfgParam->pppoeCfgParam[i].unnumberedIpAddr[2],
				pRomeCfgParam->pppoeCfgParam[i].unnumberedIpAddr[3]);
			
			fclose(f1);
			fclose(f2);
			free(tmp);
			pRomeCfgParam->pppoeCfgParam[i].dialState=PPPOECFGPARAM_DIALSTATE_DIALED_TRYING;
			sprintf(identity,"%u",i);
			//do_exec("/bin/pppd",identity);
		} /* end for i */
		#endif
	} /* ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED */


#ifdef CONFIG_RTL865XB_3G
	else if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_3G) 
	{
		int vlanID_3G=CONFIG_RTL865XB_3G_VLAN;
		re865xIoctl("ppp0",ADD_IF,0,0,0);
		rtl8651_setMtu(1500);	

		
		bdinfo_getMac((macaddr_t*)macStr);
		
#if 1 // 3G case


#if 0
		re865xIoctl("eth0",ADD_VLAN,vlanID_3G,(1<<CONFIG_RTL865XB_3G_PORT),(uint32)&macStr);
		ret=rtl8651_addNetworkIntf("ppp0");	
		if (ret)	printf("rtl8651_addNetworkIntf: ret %d\n",ret);		
		ret=rtl8651_specifyNetworkIntfLinkLayerType("ppp0",LL_VLAN,vlanID_3G);
		if (ret)	printf("rtl8651_specifyNetworkIntfLinkLayerType: ret %d\n",ret);
		rtl8651_enableNaptAutoAdd(0);
		ret=rtl8651_addExtNetworkInterface((int8*)"ppp0");
		if (ret)	printf("rtl8651_addExtNetworkInterface: ret %d\n",ret);

#else
upInterface("eth0");
		ret=rtl8651_addNetworkIntf("eth0");
		if (ret)	printf("rtl8651_addNetworkIntf: ret %d\n",ret);		
		ret=rtl8651_specifyNetworkIntfLinkLayerType("eth0",LL_VLAN,vlanID_3G);
		if (ret)	printf("rtl8651_specifyNetworkIntfLinkLayerType: ret %d\n",ret);
		rtl8651_enableNaptAutoAdd(0);
		ret=rtl8651_addExtNetworkInterface((int8*)"eth0");
		if (ret)	printf("rtl8651_addExtNetworkInterface: ret %d\n",ret);
#endif

#else // Cardbus USB v1.1 throughput test case
{

		int32  ret;
		ipaddr_t myIp,myIpMask,myGw,myDns;
		ether_addr_t myMac;


		// ETH2 MAC address, must make a real one
		myMac.octet[0]=0;	
		myMac.octet[1]=0x10;	
		myMac.octet[2]=0x60;	
		myMac.octet[3]=0xde;	
		myMac.octet[4]=0xb8;	
		myMac.octet[5]=0x7c;	

		myIp=0xc0a89a77;
		myIpMask=0xffffff00;
		myGw=0xc0a89afe;
		myDns=0xa85f0101;
		
		re865xIoctl("eth0",ADD_VLAN,vlanID_3G,(1<<CONFIG_RTL865XB_3G_PORT),(uint32)&myMac);
		ret=rtl8651_addNetworkIntf("eth2");	
		if (ret)	printf("rtl8651_addNetworkIntf: ret %d\n",ret);		
		ret=rtl8651_specifyNetworkIntfLinkLayerType("eth2",LL_VLAN,vlanID_3G);
		if (ret)	printf("rtl8651_specifyNetworkIntfLinkLayerType: ret %d\n",ret);
		rtl8651_enableNaptAutoAdd(0);
		ret=rtl8651_addExtNetworkInterface((int8*)"eth2");
		if (ret)	printf("rtl8651_addExtNetworkInterface: ret %d\n",ret);

		system("ifconfig eth2 up");
		system("ifconfig eth2 192.168.154.119");
                
		ret=rtl8651_addIpIntf("eth2",(ipaddr_t)myIp,(ipaddr_t)myIpMask);
		if (ret)
		{
			printf("rtl8651_addIpIntf: ret %d\n",ret);
		}



		// Gateway MAC address, must make a real one
		myMac.octet[0]=0;	
		myMac.octet[1]=0x0;	
		myMac.octet[2]=0xe8;	
		myMac.octet[3]=0x66;	
		myMac.octet[4]=0x46;	
		myMac.octet[5]=0x85;	

		ret=rtl8651_addArp(myGw,&myMac,"eth2",CONFIG_RTL865XB_3G_PORT);
		if(ret)
		{
			printf("rtl8651_addArp: ret %d\n",ret);
		}
	
		
		
		ret=rtl8651_addRoute((ipaddr_t)0,(ipaddr_t)0,(int8*)"eth2",myGw);
		if (ret)	
		{
			printf("rtl8651_addRoute: ret %d\n",ret);
		}
				
		ret=rtl8651_addNaptMapping((ipaddr_t)myIp);
		if (ret)
		{
			printf("rtl8651_addNaptMapping(%u.%u.%u.%u): ret %d\n",NIPQUAD(myIp),ret);
		}
}
#endif
	} /* end ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_3G */	
#endif
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
	else if (pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_PPTP) 
	{
		uint32 netmask=0;		
		uint8 strBuffer[128];
		netmask|=(pRomeCfgParam->pptpCfgParam.ipMask[0])<<24;
		netmask|=(pRomeCfgParam->pptpCfgParam.ipMask[1])<<16;
		netmask|=(pRomeCfgParam->pptpCfgParam.ipMask[2])<<8;
		netmask|=pRomeCfgParam->pptpCfgParam.ipMask[3];
		
		//printf("cfg wan to pptp ...\n");	

		//specifyNetworkIntfLinkLayerType
		//re865xIoctl("eth0",SET_VLAN_TYPE,LL_PPTP,0,0); //for PPPoE only
		rtl8651_addNetworkIntf("eth0");	
		//rtl8651_setNetMtu("eth0",pRomeCfgParam->pptpCfgParam.mtu);	//must call after addNetworkIntf ,2004/6/17
		rtl8651_specifyNetworkIntfLinkLayerType("eth0",LL_PPTP,0x08);

		/* linux if up */
		upInterface("eth0");
			
    	//rtl8651_setAsicNaptAutoAddDelete(FALSE, 1);
    	rtl8651_enableNaptAutoAdd(0);

		//add ip interface
		re865xIoctl("eth0",SET_VLAN_IP,*(uint32*)(&(pRomeCfgParam->pptpCfgParam.ipAddr)),*(uint32*)(&(pRomeCfgParam->pptpCfgParam.ipMask)),0);		
		#if 0 
		ret=rtl8651_specifyNetworkIntfLinkLayerType("eth0",RTL8651_LL_PPTP,pRomeCfgParam->vlanCfgParam[0].vid);
		if (ret)
			printf("rtl8651_specifyNetworkIntfLinkLayerType: ret %d\n",ret);
		#endif

		ret=rtl8651_addExtNetworkInterface((int8*)"eth0");
		if (ret)
			printf("rtl8651_addExtNetworkInterface: ret %d\n",ret);
	
		sprintf(strBuffer,"/sbin/ifconfig eth0 %d.%d.%d.%d netmask %d.%d.%d.%d",
			pRomeCfgParam->pptpCfgParam.ipAddr[0],
			pRomeCfgParam->pptpCfgParam.ipAddr[1],
			pRomeCfgParam->pptpCfgParam.ipAddr[2],
			pRomeCfgParam->pptpCfgParam.ipAddr[3],
			pRomeCfgParam->pptpCfgParam.ipMask[0],
			pRomeCfgParam->pptpCfgParam.ipMask[1],
			pRomeCfgParam->pptpCfgParam.ipMask[2],
			pRomeCfgParam->pptpCfgParam.ipMask[3]);

		system(strBuffer);

		ret=rtl8651_addSession(RTL8651_PPTP, 1, "eth0", 0x08); 
		if (ret)
		{
			printf("rtl8651_addSession: ret %d\n",ret);
		}
		
		ret=rtl8651_addRoute((ipaddr_t)0,(ipaddr_t)0,(int8*)"eth0",0);
		if (ret)
		{
			printf("rtl8651_addRoute: ret %d\n",ret);
		}

		pptp_start(0);				
		
	} /* end ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_PPTP */	
	
else if (pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_L2TP) 
	{
		uint32 netmask=0;		
		uint8 strBuffer[128];

		netmask|=(pRomeCfgParam->l2tpCfgParam.ipMask[0])<<24;
		netmask|=(pRomeCfgParam->l2tpCfgParam.ipMask[1])<<16;
		netmask|=(pRomeCfgParam->l2tpCfgParam.ipMask[2])<<8;
		netmask|=pRomeCfgParam->l2tpCfgParam.ipMask[3];
		
		//printf("cfg wan to l2tp ...\n");

#if 1 //l2tp by MII lookback path
		rtl8651_addNetworkIntf("eth0");	
		//rtl8651_setNetMtu("eth0",pRomeCfgParam->l2tpCfgParam.mtu);	//must call after addNetworkIntf ,2004/6/17
		rtl8651_specifyNetworkIntfLinkLayerType("eth0",LL_L2TP,0x08);

		//rtl8651_addIpIntf
		re865xIoctl("eth0",SET_VLAN_IP,*(uint32*)(&(pRomeCfgParam->l2tpCfgParam.ipAddr)),*(uint32*)(&(pRomeCfgParam->l2tpCfgParam.ipMask)),0);
#else // pure software path	
		re865xIoctl("eth0",SET_VLAN_TYPE,LL_VLAN,0,0);
		re865xIoctl("eth0",SET_VLAN_IP,*(uint32*)(&(pRomeCfgParam->l2tpCfgParam.ipAddr)),netmask,0);
		setInterfaceIp("eth0",*(uint32*)(&(pRomeCfgParam->l2tpCfgParam.ipAddr)),netmask);
#endif		

		/* linux if up */
		upInterface("eth0");
				
    	rtl8651_enableNaptAutoAdd(0);

		ret=rtl8651_addExtNetworkInterface((int8*)"eth0");
		if (ret)
			printf("rtl8651_addExtNetworkInterface: ret %d\n",ret);    	
		
		sprintf(strBuffer,"/sbin/ifconfig eth0 %d.%d.%d.%d netmask %d.%d.%d.%d",
			pRomeCfgParam->l2tpCfgParam.ipAddr[0],
			pRomeCfgParam->l2tpCfgParam.ipAddr[1],
			pRomeCfgParam->l2tpCfgParam.ipAddr[2],
			pRomeCfgParam->l2tpCfgParam.ipAddr[3],			
			pRomeCfgParam->l2tpCfgParam.ipMask[0],
			pRomeCfgParam->l2tpCfgParam.ipMask[1],
			pRomeCfgParam->l2tpCfgParam.ipMask[2],
			pRomeCfgParam->l2tpCfgParam.ipMask[3]);
		
		system(strBuffer);

		ret=rtl8651_addSession(RTL8651_L2TP, 1, "eth0", 0x08); 
		if (ret)
		{
			printf("rtl8651_addSession: ret %d\n",ret);
		}
		
		ret=rtl8651_addRoute((ipaddr_t)0,(ipaddr_t)0,(int8*)"eth0",0);
		if (ret)
		{
			printf("rtl8651_addRoute: ret %d\n",ret);
		}
		
		l2tp_start(0);
		
	} /* end ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_L2TP */
	else if (pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_DHCPL2TP) {

		printf("cfg: DHCP+L2TP Wan Type ...\n");
		pRomeCfgParam->l2tpCfgParam.dialState=L2TPCFGPARAM_DIALSTATE_DIALED_DHCP_DISCOVER;


				
		/* clear ifCfgParam[0].ipAddr[] etc, dhcp client will fill these values later */
		memset((void*)pRomeCfgParam->ifCfgParam[0].ipAddr,0,4);
		memset((void*)pRomeCfgParam->ifCfgParam[0].ipMask,0,4);
		memset((void*)pRomeCfgParam->ifCfgParam[0].gwAddr,0,4);
		memset((void*)pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,0,4);
		memset((void*)pRomeCfgParam->ifCfgParam[0].dnsSecondaryAddr,0,4);

		setInterfaceIp("eth0",0,0);		
//		re865xIoctl("eth0",SET_VLAN_TYPE,LL_VLAN,0,0);	
//		rtl8651_specifyNetworkIntfLinkLayerType("eth0",LL_L2TP,0x08);
			
   		rtl8651_enableNaptAutoAdd(0);    		
		
		udhcpc_start();
	}
	
#endif // CONFIG_RTL865X(B)_PPTPL2TP

	// set default Upstream of IGMP system
	rtl8651_multicastSetUpStream("eth0", 0);
	rtl8651_EnablIpMulticast(pRomeCfgParam->mgmtCfgParam[0].multicastEnable);

	return 0;
} /* end wanlan_init */

/*
 * Routine Name: routeCfgFactoryDefaultFunction
 * Description: route cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void routeCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(routeCfgParam_t)*MAX_ROUTE;
	routeCfgParam_t tmpTab[MAX_ROUTE];
	
	memset((void*)tmpTab, 0, mySize);
	memcpy((void*)tmpTab, (void*)routeCfgParamDefault, sizeof(routeCfgParamDefault));
	memcpy((void*)pBuf, (void*)tmpTab, mySize);
	*pBufSize = mySize;

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));

	return;
} /* end routeCfgFactoryDefaultFunction */


/*
 * Routine Name: staticroute_init
 * Description: configure route
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 staticroute_init(void)
{
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_ROUTE,(void*)pRomeCfgParam->routeCfgParam, sizeof(pRomeCfgParam->routeCfgParam))!=0)
	{
		printf("route_init: call cfgmgr_read fail\n");		
		/* take proper actions */
		return NOT_OK;
	}

	/* configure interface address and route */
	{
		routeCfgParam_t *pRouteCfg;
		int8 ifName[RTL8651_IPINTF_NAME_LEN];
		uint32 i=0;

		i=0;

		for(pRouteCfg=&(pRomeCfgParam->routeCfgParam[i]); (pRouteCfg->valid == 1)&&(i<MAX_ROUTE); )
		{
			char gateway_str[18]={0};
			char route_str[18]={0};
			
			sprintf(ifName, "vl%d", pRouteCfg->if_unit);
			{
				sprintf(gateway_str,"%u.%u.%u.%u",pRouteCfg->ipGateway[0],pRouteCfg->ipGateway[1],pRouteCfg->ipGateway[2],pRouteCfg->ipGateway[3]);
				sprintf(route_str,"%u.%u.%u.%u",pRouteCfg->ipAddr[0] & pRouteCfg->ipMask[0],
												pRouteCfg->ipAddr[1] & pRouteCfg->ipMask[1],
												pRouteCfg->ipAddr[2] & pRouteCfg->ipMask[2],
												pRouteCfg->ipAddr[3] & pRouteCfg->ipMask[3]);
			}
			if (pRomeCfgParam->natCfgParam.hwaccel) {
				#if 0
				ret=rtl8651_addRoute(*((ipaddr_t *)(pRouteCfg->ipAddr)), *((ipaddr_t *)(pRouteCfg->ipMask)), ifName, *((ipaddr_t *)(pRouteCfg->ipGateway)));
				if(ret!=SUCCESS)
					printf("staticroute_init: rtl8651_addRoute failed ret:%d\n",ret);
				else
					printf("staticroute_init: rtl8651_addRoute(%s) ok\n",route_str);
				#endif
			}
			i++;
			pRouteCfg = &pRomeCfgParam->routeCfgParam[i];
		} /* end for */
	}
	return OK;
} /* end staticroute_init */


/*
 * Routine Name: arpCfgFactoryDefaultFunction
 * Description: arp cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void arpCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(arpCfgParam_t)*MAX_STATIC_ARP;
	arpCfgParam_t tmpTab[MAX_STATIC_ARP];
	
	memset((void*)tmpTab, 0, mySize);
	memcpy((void*)tmpTab, (void*)arpCfgParamDefault, sizeof(arpCfgParamDefault));
	memcpy((void*)pBuf, (void*)tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end arpCfgFactoryDefaultFunction */

/*
 * Routine Name: arp_init
 * Description: configure static arp entries
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 arp_init(void)
{
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_ARP,(void*)pRomeCfgParam->arpCfgParam, sizeof(pRomeCfgParam->arpCfgParam))!=0)
	{
		printf("arp_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}

	if (pRomeCfgParam->natCfgParam.hwaccel)	/* configure static arp entries */
	{
		struct arpCfgParam_s *pArpCfg;
		int8 ifName[RTL8651_IPINTF_NAME_LEN];
		uint32 i=0;
		
		for(pArpCfg = &(pRomeCfgParam->arpCfgParam[i]); (pArpCfg->valid==1)&&(i<MAX_STATIC_ARP); ){
			sprintf(ifName, "vl%d", pArpCfg->if_unit);			
			//rtl8651_addArp(inet_addr(pArpCfg->ipAddr), &(pArpCfg->mac), ifName, pArpCfg->port);
			//rtl8651_addArp(*(ipaddr_t*)pArpCfg->ipAddr, &(pArpCfg->mac), ifName, pArpCfg->port);
			i++;
			pArpCfg = &(pRomeCfgParam->arpCfgParam[i]);
		}
	}

	return 0;	
} /* end arp_init */


/*
 * Routine Name: natCfgFactoryDefaultFunction
 * Description: nat cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void natCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(pRomeCfgParam->natCfgParam);
	natCfgParam_t tmpTab;
	
	memset((void*)&tmpTab, 0, mySize);
	memcpy((void*)&tmpTab, (void*)&(natCfgParamDefault), sizeof(natCfgParamDefault));
	memcpy((void*)pBuf, (void*)&tmpTab, mySize);
	
	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end natCfgFactoryDefaultFunction */

/*
 * Routine Name: nat_init
 * Description: configure nat cfg param entry
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 nat_init(void)
{
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_NAT,(void*)&(pRomeCfgParam->natCfgParam), sizeof(pRomeCfgParam->natCfgParam))!=0)
	{
		printf("nat_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}
	return 0;	
} /* end nat_init */
/* userDb_init()
*/

uint32 userDb_init(void)
{
	if (cfgmgr_read(CFGMGR_TABID_MGMT,(void*)&(pRomeCfgParam->mgmtCfgParam), sizeof(struct mgmtCfgParam_s)*MAX_MGMT_USER)!=0)
	{
		printf("userDb_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}
	return 0;	


}

/*
 * Routine Name: pppoeCfgFactoryDefaultFunction
 * Description: pppoe cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void pppoeCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(pppoeCfgParam_t)*MAX_PPPOE_SESSION;
	pppoeCfgParam_t tmpTab[MAX_PPPOE_SESSION];
	
	memset((void*)tmpTab, 0, mySize);
	memcpy((void*)tmpTab, (void*)pppoeCfgParamDefault, sizeof(pppoeCfgParamDefault));
	memcpy((void*)pBuf, (void*)tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */	
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end pppoeCfgFactoryDefaultFunction */

/*
 * Routine Name: pppoe_init
 * Description: configure pppoe cfg param entry
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
#if 0
/* obsolete by wan flat init in sysInit 2003/09/24 */
uint32 pppoe_init(void)
{
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_PPPOE,(void*)pRomeCfgParam->pppoeCfgParam, sizeof(pppoeCfgParam_t)*MAX_PPPOE_SESSION)!=0)
	{
		printf("pppoe_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}

	memcpy((void*)ramPppoeCfgParam,(void*)pRomeCfgParam->pppoeCfgParam,sizeof(pppoeCfgParam_t)*MAX_PPPOE_SESSION));
	
	return 0;	
} /* end pppoe_init */
#endif

/*
 * Routine Name: dhcpsCfgFactoryDefaultFunction
 * Description: dhcps cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void dhcpsCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(dhcpsCfgParam_t);
	dhcpsCfgParam_t tmpTab;
	
	memset((void*)&tmpTab, 0, mySize);
	memcpy((void*)&tmpTab, (void*)&dhcpsCfgParamDefault, sizeof(dhcpsCfgParamDefault));
	memcpy((void*)pBuf, (void*)&tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	//printf("\n size of dhcpsCfgParam_t: %d", sizeof(dhcpsCfgParam_t));
	//printf("\n size of dhcpsCfgParamDefault: %d", sizeof(dhcpsCfgParamDefault));
	//printf("\ntmpTab.username_2g: %s", tmpTab.username_2g);
	//printf("\ntmpTab.password_2g: %s", tmpTab.password_2g);
	//printf("\ntmpTab.server_2g: %s", tmpTab.server_2g);
	//printf("\ntmpTab.dial_number_2g: %s", tmpTab.dial_number_2g);
	
	//printf("\ncfg.device.username_2g: %s", cfg.device.username_2g);
	//printf("\ncfg.device.password_2g: %s", cfg.device.password_2g);
	//printf("\ncfg.device.server_2g: %s", cfg.device.server_2g);
	//printf("\ncfg.device.dial_number_2g: %s", cfg.device.dial_number_2g);
	
	
	
	return;
} /* end dhcpsCfgFactoryDefaultFunction */


void dhcps_start(void)
{
	FILE *f1,*f2;
	unsigned char tmp[160];
	char *cp;
	int32 i;

	if (pRomeCfgParam->dhcpsCfgParam.enable == 0)
	{
		do_kill("/var/run/udhcpd.pid");
		return;
	}

	/* Don't need sleep because running dns proxy/cache */
	f1 = fopen("/etc/udhcpd.conf","r");
	if (!f1)
	{
		printf("fopen /etc/udhcpd.conf failed\n");
		while(1);
	}	
	f2 = fopen("/var/udhcpd.conf","w+");
	if (!f2)
	{
		printf("fopen /var/udhcpd.conf failed\n");
		while(1);
	}
	
	while (!feof(f1))
	{
		fgets(tmp,160,f1);
		if (!memcmp(tmp,"startip",7))
			sprintf(tmp,"start	%u.%u.%u.%u\n",
				pRomeCfgParam->ifCfgParam[1].ipAddr[0],
				pRomeCfgParam->ifCfgParam[1].ipAddr[1],
				pRomeCfgParam->ifCfgParam[1].ipAddr[2],
				pRomeCfgParam->dhcpsCfgParam.startIp);
					
		else if (!memcmp(tmp,"endip",5))
			sprintf(tmp,"end	%u.%u.%u.%u\n",
				pRomeCfgParam->ifCfgParam[1].ipAddr[0],
				pRomeCfgParam->ifCfgParam[1].ipAddr[1],
				pRomeCfgParam->ifCfgParam[1].ipAddr[2],
				pRomeCfgParam->dhcpsCfgParam.endIp);

		// Dino Chang 2005/01/02, modify for DNS relay
		else if (!memcmp(tmp,"dnsip",5)) {
			if(pRomeCfgParam->dnsCfgParam.enable == 0) {
// Dino Chang 2005/04/27
// Add Secondary DNS
				uint32 dnsServer[2] = {0};
				dnsServer[0] = ((uint32)(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[0]) << 24) + ((uint32)(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[1]) << 16) + ((uint32)(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[2]) << 8) + (uint32)(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[3]);
				dnsServer[1] = ((uint32)(pRomeCfgParam->ifCfgParam[0].dnsSecondaryAddr[0]) << 24) + ((uint32)(pRomeCfgParam->ifCfgParam[0].dnsSecondaryAddr[1]) << 16) + ((uint32)(pRomeCfgParam->ifCfgParam[0].dnsSecondaryAddr[2]) << 8) + (uint32)(pRomeCfgParam->ifCfgParam[0].dnsSecondaryAddr[3]);

				if(dnsServer[0] && dnsServer[1]) {
					sprintf(tmp,"option dns %u.%u.%u.%u\noption dns %u.%u.%u.%u\n",
						pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[0],
						pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[1],
						pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[2],
						pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[3],
						pRomeCfgParam->ifCfgParam[0].dnsSecondaryAddr[0],
						pRomeCfgParam->ifCfgParam[0].dnsSecondaryAddr[1],
						pRomeCfgParam->ifCfgParam[0].dnsSecondaryAddr[2],
						pRomeCfgParam->ifCfgParam[0].dnsSecondaryAddr[3]);
				} else if(dnsServer[0]) { // Primary DNS exist
					sprintf(tmp,"option dns  %u.%u.%u.%u\n",
						pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[0],
						pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[1],
						pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[2],
						pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[3]);
				} else if(dnsServer[1]) { // Secondary DNS exist
					sprintf(tmp,"option dns  %u.%u.%u.%u\n",
						pRomeCfgParam->ifCfgParam[0].dnsSecondaryAddr[0],
						pRomeCfgParam->ifCfgParam[0].dnsSecondaryAddr[1],
						pRomeCfgParam->ifCfgParam[0].dnsSecondaryAddr[2],
						pRomeCfgParam->ifCfgParam[0].dnsSecondaryAddr[3]);
				} else { // No DNS, use router as DNS
					sprintf(tmp,"option dns  %u.%u.%u.%u\n",
						pRomeCfgParam->ifCfgParam[1].ipAddr[0],
						pRomeCfgParam->ifCfgParam[1].ipAddr[1],
						pRomeCfgParam->ifCfgParam[1].ipAddr[2],
						pRomeCfgParam->ifCfgParam[1].ipAddr[3]);
				}
#if 0
				if((pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[0] == 0) &&
				   (pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[1] == 0) &&
				   (pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[2] == 0) &&
				   (pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[3] == 0)) {
					sprintf(tmp,"opt dns  %u.%u.%u.%u\n",
						pRomeCfgParam->ifCfgParam[1].ipAddr[0],
						pRomeCfgParam->ifCfgParam[1].ipAddr[1],
						pRomeCfgParam->ifCfgParam[1].ipAddr[2],
						pRomeCfgParam->ifCfgParam[1].ipAddr[3]);
				} else {
					sprintf(tmp,"opt dns  %u.%u.%u.%u\n",
						pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[0],
						pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[1],
						pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[2],
						pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[3]);
				}
#endif
// Dino Chang 2005/04/27
			} else { // use router as DNS server
				sprintf(tmp,"opt dns  %u.%u.%u.%u\n",
					pRomeCfgParam->ifCfgParam[1].ipAddr[0],
					pRomeCfgParam->ifCfgParam[1].ipAddr[1],
					pRomeCfgParam->ifCfgParam[1].ipAddr[2],
					pRomeCfgParam->ifCfgParam[1].ipAddr[3]);
			}
		}
#if 0
		if (!memcmp(tmp,"dnsip",5))
			sprintf(tmp,"opt dns  %u.%u.%u.%u\n",
				pRomeCfgParam->ifCfgParam[1].ipAddr[0],
				pRomeCfgParam->ifCfgParam[1].ipAddr[1],
				pRomeCfgParam->ifCfgParam[1].ipAddr[2],
				pRomeCfgParam->ifCfgParam[1].ipAddr[3]);
#endif
		// Dino
		else if (!memcmp(tmp,"routerip",8))
			sprintf(tmp,"option router  %u.%u.%u.%u\n",
				pRomeCfgParam->ifCfgParam[1].ipAddr[0],
				pRomeCfgParam->ifCfgParam[1].ipAddr[1],
				pRomeCfgParam->ifCfgParam[1].ipAddr[2],
				pRomeCfgParam->ifCfgParam[1].ipAddr[3]);
		// Dino Chang 2004/11/30
		// network mask
		else if(!memcmp(tmp, "subnet", 6))
			sprintf(tmp, "option subnet %u.%u.%u.%u\n",
				pRomeCfgParam->ifCfgParam[1].ipMask[0],
				pRomeCfgParam->ifCfgParam[1].ipMask[1],
				pRomeCfgParam->ifCfgParam[1].ipMask[2],
				pRomeCfgParam->ifCfgParam[1].ipMask[3]);
		// Dino
		// Dino Chang 2004/12/21
		// domain
		// Dino Chang 2005/04/27
		// fix domain doesn't work bug
		else if(!memcmp(tmp, "domain", 6)) {
			if(pRomeCfgParam->dhcpsCfgParam.domain_name[0]) {
				sprintf(tmp, "option domain %s\n", pRomeCfgParam->dhcpsCfgParam.domain_name);
			} else {
				sprintf(tmp, "\n");
			}
		}
#if 0
		if(!memcmp(tmp, "domain", 7))
			sprintf(tmp, "domain %s\n",
				pRomeCfgParam->dhcpsCfgParam.domain_name);
#endif
		// Dino Chang 2005/04/27
		// lease time
		else if(!memcmp(tmp, "lease", 5))
			sprintf(tmp, "option lease %ld\n",
				pRomeCfgParam->dhcpsCfgParam.dhcp_lease);
		// Dino
		fputs(tmp,f2);
	}

	for(i=0; i<DHCPS_HWARR_NO; i++) {
		// Dino Chang 2005/04/27
		if(pRomeCfgParam->dhcpsCfgParam.manualIp[i] == 0)
			continue;
		// Dino 2005/04/27
		sprintf(tmp, "\nmanual %02X:%02X:%02X:%02X:%02X:%02X  %u.%u.%u.%u\n", 
				pRomeCfgParam->dhcpsCfgParam.hardwareAddr[i][0], pRomeCfgParam->dhcpsCfgParam.hardwareAddr[i][1],
				pRomeCfgParam->dhcpsCfgParam.hardwareAddr[i][2], pRomeCfgParam->dhcpsCfgParam.hardwareAddr[i][3],
				pRomeCfgParam->dhcpsCfgParam.hardwareAddr[i][4], pRomeCfgParam->dhcpsCfgParam.hardwareAddr[i][5],
				pRomeCfgParam->dhcpsCfgParam.manualIp[i]>>24, (pRomeCfgParam->dhcpsCfgParam.manualIp[i]&0x00ff0000)>>16,
				(pRomeCfgParam->dhcpsCfgParam.manualIp[i]&0x0000ff00)>>8, pRomeCfgParam->dhcpsCfgParam.manualIp[i]&0xff);
		fputs(tmp,f2);
	}

	fclose(f1);

// Dino Chang 2005/01/02
#if 0
	f1 = fopen("/var/resolv.conf","r");
	if (f1)
	{
		//while ((!feof(f1))) 
		{
			memset(tmp,0,160);
			fgets(tmp,160,f1);
			/*for (i=0;i<160;i++)
			{
				if (memcmp(*tmp," ",1))
					tmp++;
				if (*tmp==0)break;
			}*/
			cp = strchr(tmp,' ');
			if (cp)
			fprintf(f2,"opt dns %s\n",cp);
		}
		fclose(f1);// modified by Nick/Dino, 12/27/2004
	}
#endif
	fclose(f2);

	// Dino Chang 2005/01/02, modify for DNS relay
	send_signal("/var/run/udhcpd.pid", SIGUSR1);
	sleep(1);
	do_kill("/var/run/udhcpd.pid");
	// Dino
	do_exec("/bin/udhcpd",NULL);	
	sleep(1);
}

/*
 * Routine Name: dhcps_init
 * Description: configure dhcps cfg param entry
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 dhcps_init(void)
{
	FILE *f;
	int i;

	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_DHCPS,(void*)&(pRomeCfgParam->dhcpsCfgParam), sizeof(dhcpsCfgParam_t))!=0)
	{
		printf("dhcps_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}

	memcpy((void*)&ramDhcpsCfgParam,&(pRomeCfgParam->dhcpsCfgParam),sizeof(dhcpsCfgParam_t));

	for (i = 0; i < MAX_STATIC_DHCP_ITEM; i++){
		cfg.device.static_ip[i] = ramDhcpsCfgParam.manualIp[i];
		memcpy(cfg.device.static_mac[i], ramDhcpsCfgParam.hardwareAddr[i], 6);
		if (ramDhcpsCfgParam.manualIp[i] != 0){
			f=fopen("/var/dhcpd_client_static", "a+");
			//fprintf(f, "%02x %02x %02x %02x %02x %02x %02x\n", 
			fprintf(f, "%02x\n", ramDhcpsCfgParam.manualIp[i]);
			fprintf(f, "%02x%02x%02x%02x\n", 
				ramDhcpsCfgParam.hardwareAddr[i][0],
				ramDhcpsCfgParam.hardwareAddr[i][1],
				ramDhcpsCfgParam.hardwareAddr[i][2],
				ramDhcpsCfgParam.hardwareAddr[i][3]);
			fprintf(f, "%02x%02x0000\n", 
				ramDhcpsCfgParam.hardwareAddr[i][4],
				ramDhcpsCfgParam.hardwareAddr[i][5]);
		
			fclose(f);
		}
	}
	
	
	return 0;	
} /* end dhcps_init */

/*
 * Routine Name: dhcpcCfgFactoryDefaultFunction
 * Description: dhcpc cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void dhcpcCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(dhcpcCfgParam_t);
	dhcpcCfgParam_t tmpTab;
	
	memset((void*)&tmpTab, 0, mySize);
	memcpy((void*)&tmpTab, (void*)&dhcpcCfgParamDefault, sizeof(dhcpcCfgParamDefault));
	memcpy((void*)pBuf, (void*)&tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end dhcpcCfgFactoryDefaultFunction */

/*
 * Routine Name: dhcpc_init
 * Description: configure dhcpc cfg param entry
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
#if 0
/* obsoleted by flat wan init in sysInit 2003/09/24 orlando */
uint32 dhcpc_init(void)
{	
	/* read cfg from cfgmgr
	 */
	if (cfgmgr_read(CFGMGR_TABID_DHCPC,(void*)&(pRomeCfgParam->dhcpcCfgParam), sizeof(dhcpcCfgParam_t))!=0)
	{
		printf("dhcpc_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}
	
	memcpy((void*)&(ramDhcpcCfgParam),(void*)&(pRomeCfgParam->dhcpcCfgParam),sizeof(dhcpcCfgParam_t));
	
	return 0;	
} /* end dhcpc_init */
#endif

//peter and howard
/*
 * Routine Name: pppCfgFactoryDefaultFunction
 * Description: ppp cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void pppCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(pppCfgParam_t);
	pppCfgParam_t tmpTab;
	
	memset((void*)&tmpTab, 0, mySize);
	memcpy((void*)&tmpTab, (void*)&pppCfgParamDefault, sizeof(pppCfgParamDefault));
	memcpy((void*)pBuf, (void*)&tmpTab, mySize);
	
	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end dnsCfgFactoryDefaultFunction */

/*
 * Routine Name: ppp_init
 * Description: create /var/resolv.conf based on xxxCfgParam[].dnsPrimaryAddr
 *              this file is to be referenced by nameserver
 *              note that dhcpc and pppd will overwrite this file upon ip-up.
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */

// 2006-01-25: Peter modify  for 3G(KR1)
uint32 ppp_chap_secrets(void)
{
	char user[32], secrets[32], server[32], dial_no[32];
	FILE *f;

	memset(user, 0, 32);
	memset(secrets, 0, 32);
	memset(server, 0, 32);
	memset(dial_no, 0, 32);

	if(strlen(pRomeCfgParam->pppCfgParam.username)==0)
		strncpy(user, "*", 1);
	else
		strncpy(user, pRomeCfgParam->pppCfgParam.username, 32);

	if(strlen(pRomeCfgParam->pppCfgParam.password)==0)
		strncpy(secrets, "*", 1);
	else
		strncpy(secrets, pRomeCfgParam->pppCfgParam.password, 32);

	if(strlen(pRomeCfgParam->pppCfgParam.server)==0)
		strncpy(server, "*", 1);
	else
		strncpy(server, pRomeCfgParam->pppCfgParam.server, 32);

	if(strlen(pRomeCfgParam->pppCfgParam.dial_number)==0)
		strncpy(dial_no, "#777", 4);
	else
		strncpy(dial_no, pRomeCfgParam->pppCfgParam.dial_number, 32);

	f = fopen("/var/ppp/chap-secrets", "w");
	
	fprintf(f, "# Secrets for authentication using CHAP\n");
	fprintf(f, "# dial_number : %s\n", dial_no);
	fprintf(f, "# client  server     secrets    IP\n");
	fprintf(f, " %s \t %s \t %s \t *\n", user, server, secrets);    
       	
	fclose(f);

}

uint32 ppp_pap_secrets(void)
{
	char user[32], secrets[32], server[32], dial_no[32];
	FILE *f;

	memset(user, 0, 32);
	memset(secrets, 0, 32);
	memset(server, 0, 32);
	memset(dial_no, 0, 32);

	if(strlen(pRomeCfgParam->pppCfgParam.username)==0)
		strncpy(user, "*", 1);
	else
		strncpy(user, pRomeCfgParam->pppCfgParam.username, 32);

	if(strlen(pRomeCfgParam->pppCfgParam.password)==0)
		strncpy(secrets, "*", 1);
	else
		strncpy(secrets, pRomeCfgParam->pppCfgParam.password, 32);

	if(strlen(pRomeCfgParam->pppCfgParam.server)==0)
		strncpy(server, "*", 1);
	else
		strncpy(server, pRomeCfgParam->pppCfgParam.server, 32);

	if(strlen(pRomeCfgParam->pppCfgParam.dial_number)==0)
		strncpy(dial_no, "#777", 4);
	else
		strncpy(dial_no, pRomeCfgParam->pppCfgParam.dial_number, 32);

	f = fopen("/var/ppp/pap-secrets", "w");
	
	fprintf(f, "# Secrets for authentication using CHAP\n");
	fprintf(f, "# dial_number : %s\n", dial_no);
	fprintf(f, "# client  server     secrets    IP\n");
	fprintf(f, " %s \t %s \t %s \t *\n", user, server, secrets);    
       	
	fclose(f);

}

uint32 ppp_init(void)
{
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_PPP,(void*)&(pRomeCfgParam->pppCfgParam), sizeof(pRomeCfgParam->pppCfgParam))!=0)
	{
		printf("ppp_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}
	
	ppp_pap_secrets();
	ppp_chap_secrets();
/*
     FILE *f;

        f = fopen("/var/ppp/chap-secrets", "w+");
        fprintf(f, "# dial_number : %s\n", pRomeCfgParam->pppCfgParam.dial_number);
        fprintf(f, " %s \t %s \t %s \n", pRomeCfgParam->pppCfgParam.username,
                                         pRomeCfgParam->pppCfgParam.password,
                                         pRomeCfgParam->pppCfgParam.server);
		fclose(f);
*/
	return 0;	
} /* end ppp_init */
//peter and howard
// 2006-01-25: Peter modify  for 3G(KR1) end

/*
 * Routine Name: dnsCfgFactoryDefaultFunction
 * Description: dns cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void dnsCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(dnsCfgParam_t);
	dnsCfgParam_t tmpTab;
	
	memset((void*)&tmpTab, 0, mySize);
	memcpy((void*)&tmpTab, (void*)&dnsCfgParamDefault, sizeof(dnsCfgParamDefault));
	memcpy((void*)pBuf, (void*)&tmpTab, mySize);
	
	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end dnsCfgFactoryDefaultFunction */

/*
 * Routine Name: dns_init
 * Description: create /var/resolv.conf based on xxxCfgParam[].dnsPrimaryAddr
 *              this file is to be referenced by nameserver
 *              note that dhcpc and pppd will overwrite this file upon ip-up.
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 dns_init(void)
{
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_DNS,(void*)&(pRomeCfgParam->dnsCfgParam), sizeof(pRomeCfgParam->dnsCfgParam))!=0)
	{
		printf("dns_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}
	
	/* PPPoE has to have a callback mechanism when ppp state reaches "NETWORK"
	   reflect the new dns given by pppoe service provider
	   current implementation try to init it during bootup to wan's static dns.
	   it's easier for demonstraton purpose. but need to change it when
	   the callback mechanism is ready!! orlando 2003/06/26
	 */
	if ( (pRomeCfgParam->dnsCfgParam.enable == 1)
	     && 
	     (   pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_STATIC
	      || pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_PPPOE
	      || pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE
	      || pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED	      
	      || pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_DHCPC
	     )
	   )
	{
		char primary[16]={0};
		FILE *f;
		sprintf(primary,"%d.%d.%d.%d",pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[0],
									  pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[1],
									  pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[2],
									  pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[3] );
		//printf("dns_init: starting dns proxy at %s ...\n",primary);
		
		// set resolv.conf
		f=fopen("/var/resolv.conf","w+");
		fprintf(f,"nameserver %s\n",primary);
		fclose(f);
	}

	return 0;	
} /* end dns_init */


void aclCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(aclCfgParam_t)*MAX_ACL*MAX_PPPOE_SESSION;
	aclCfgParam_t tmpTab[MAX_PPPOE_SESSION][MAX_ACL];
	
	memset((void*)tmpTab, 0, mySize);
	memcpy((void*)tmpTab, (void*)aclCfgParamDefault, sizeof(struct aclCfgParam_s)*MAX_ACL*MAX_PPPOE_SESSION);
	memcpy((void*)pBuf, (void*)tmpTab, mySize);
	
	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end aclCfgFactoryDefaultFunction */

void aclGlobalCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(aclGlobalCfgParam_t);
	aclGlobalCfgParam_t tmpTab;
	
	memset((void*)&tmpTab, 0, mySize);
	memcpy((void*)&tmpTab, (void*)&aclGlobalCfgParamDefault, sizeof(aclGlobalCfgParamDefault));
	memcpy((void*)pBuf, (void*)&tmpTab, mySize);
	
	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end aclCfgFactoryDefaultFunction */


/*
 * Routine Name: acl_tableDriverAccess
 * Description: access table driver acl APIs
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 acl_tableDriverAccess(void)
{
	int32 ret;
	int8 ifStr[20];
	int8 ifWanStr[20];
	int8 ifLanStr[20];
	uint32 i,j,k;
	uint32 loopUpperBound=0;

	memset(ifStr,0,20);	
	sprintf(ifStr,"eth0");
	memset(ifWanStr,0,20);	
	sprintf(ifWanStr,"eth0");
	memset(ifLanStr,0,20);	
	sprintf(ifLanStr,"eth1");

	if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)
		loopUpperBound = MAX_PPPOE_SESSION;
	else
		loopUpperBound = 1;
	
	/* flush all wan interface's egress/ingress rule */
	for(i=0;i<loopUpperBound;i++){
		if (pRomeCfgParam->natCfgParam.hwaccel==1)
		{
			ret = rtl8651a_flushAclRule(i,ifWanStr,0);
			if (ret)
			{
				printf("acl_tableDriverAccess: rtl8651_flushAclRule(if:%s) ret:%d\n",ifWanStr,ret);
				return NOT_OK;		
			}
		
			ret = rtl8651a_flushAclRule(i,ifWanStr,1);		
			if (ret)
			{
				printf("acl_tableDriverAccess: rtl8651_flushAclRule(if:%s) ret:%d\n",ifWanStr,ret);
				return NOT_OK;		
			}

			ret = rtl8651a_flushAclRule(i,ifLanStr,0);
			if (ret)
			{
				printf("acl_tableDriverAccess: rtl8651_flushAclRule(if:%s) ret:%d\n",ifLanStr,ret);
				return NOT_OK;		
			}
		
			ret = rtl8651a_flushAclRule(i,ifLanStr,1);		
			if (ret)
			{
				printf("acl_tableDriverAccess: rtl8651_flushAclRule(if:%s) ret:%d\n",ifLanStr,ret);
				return NOT_OK;		
			}
		}
	}

	// Add MAC filter configuration in the beginning of all rules.
	if(pRomeCfgParam->natCfgParam.mac_filter)
	{
		int mac_filter_counter  = 0;
		rtl8651_tblDrvAclRule_t samplelRule;
		int8 ifLanStr[20] = {0};
		int32 ret;
		uint8 macNull[6]={0,0,0,0,0,0};
		uint8 macFFFF[6]={0xff,0xff,0xff,0xff,0xff,0xff};

		for (i = 0; i < FILTER_ENTRY_NUM; i++)
		{
			if (memcmp(pRomeCfgParam->macfilterCfgParam[0][i].mac, macNull, 6))		
				mac_filter_counter++;
		}
		
		for(i = 0; i < mac_filter_counter; i++)
		{
			sprintf(ifLanStr,"eth1");
			samplelRule.matchType_ = RTL8651_ACL_GENERIC;        
			samplelRule.ruleType_ = RTL8651_ACL_MAC;
				
			if(pRomeCfgParam->macfilterCfgParam[0][i].allow == 0)
				samplelRule.actionType_ = RTL8651_ACL_DROP; // Deny MAC list (Negative List)
			else if(pRomeCfgParam->macfilterCfgParam[0][i].allow == 1)
				samplelRule.actionType_ = RTL8651_ACL_PERMIT; // Allow MAC list (Positive List)		
			else
			{
				 printf("Error!!!!!!!!\n");
				 return 0;
			}
			
			memcpy(&samplelRule.srcMac_, pRomeCfgParam->macfilterCfgParam[0][i].mac,6); 
			memcpy(&samplelRule.srcMacMask_, macFFFF,6);  
			
			memcpy(&samplelRule.dstMac_, macNull,6);
			memcpy(&samplelRule.dstMacMask_, macNull,6);
			samplelRule.typeLen_ = 0; 
			samplelRule.typeLenMask_ = 0;  
			ret = rtl8651a_addAclRule(0, ifLanStr, 1 ,(rtl8651_tblDrvAclRule_t *)&samplelRule);
			if (ret!=SUCCESS)
				printf("%s: %d: rtl8651a_addAclRule() ret:%d\n",__FUNCTION__, __LINE__, ret);
		}
		
		// add DROP for all for positive list
		if(pRomeCfgParam->macfilterCfgParam[0][0].allow == 1)
    		{
			sprintf(ifLanStr,"eth1");
			samplelRule.matchType_ = RTL8651_ACL_GENERIC;        
			samplelRule.ruleType_ = RTL8651_ACL_MAC;
				
			samplelRule.actionType_ = RTL8651_ACL_DROP; // Deny all others MAC
			
			memcpy(&samplelRule.srcMac_, macNull,6); 
			memcpy(&samplelRule.srcMacMask_, macNull,6);  
			
			memcpy(&samplelRule.dstMac_, macNull,6);
			memcpy(&samplelRule.dstMacMask_, macNull,6);
			samplelRule.typeLen_ = 0; 
			samplelRule.typeLenMask_ = 0;  
			ret = rtl8651a_addAclRule(0, ifLanStr, 1 ,(rtl8651_tblDrvAclRule_t *)&samplelRule);
			if (ret!=SUCCESS)
				printf("%s: %d: add DROP for all for positive list: rtl8651a_addAclRule() ret:%d\n",__FUNCTION__, __LINE__, ret);
    		}
		
	}

	if(pRomeCfgParam->aclGlobalCfgParam.lanPermit==1)
	{
		memset((void*)&tempAclRule,0,sizeof(rtl8651_tblDrvAclRule_t));	
		{
			uint32 tempip,mask;

			memcpy(&tempip,pRomeCfgParam->ifCfgParam[1].ipAddr,4);		
			memcpy(&mask,pRomeCfgParam->ifCfgParam[1].ipMask,4);
			tempip&=mask;
			memcpy(&(tempAclRule.dstIpAddr_),&tempip,4);
			memcpy(&(tempAclRule.dstIpAddrMask_),pRomeCfgParam->ifCfgParam[1].ipMask,4);
		}
		tempAclRule.ruleType_=RTL8651_ACL_IP;
		tempAclRule.actionType_=RTL8651_ACL_PERMIT;
		ret = rtl8651a_addAclRule(0,ifLanStr,1,(rtl8651_tblDrvAclRule_t* )&tempAclRule);		
	}
	
	
	for(i=0;i<loopUpperBound;i++){
		if (pRomeCfgParam->pppoeCfgParam[i].dialState == PPPOECFGPARAM_DIALSTATE_DIALED_SUCCESS &&
		(!pRomeCfgParam->pppoeCfgParam[i].defaultSession) && pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED)
		{
			if(pRomeCfgParam->pppoeCfgParam[i].lanType==PPPOECFGPARAM_LANTYPE_NAPT)
			{
				memset((void*)&tempAclRule,0,sizeof(rtl8651_tblDrvAclRule_t));
				tempAclRule.dstIpAddrMask_=0xffffffff;
				memcpy(&(tempAclRule.dstIpAddr_),pRomeCfgParam->pppoeCfgParam[i].ipAddr,4);
				tempAclRule.ruleType_=RTL8651_ACL_IP;
				tempAclRule.actionType_=RTL8651_ACL_CPU;
			}else{
				ipaddr_t mySubnet;
				memset((void*)&tempAclRule,0,sizeof(rtl8651_tblDrvAclRule_t));
				memcpy(&(tempAclRule.dstIpAddrMask_),pRomeCfgParam->pppoeCfgParam[i].unnumberedIpMask,4);
				memcpy(&(tempAclRule.dstIpAddr_),pRomeCfgParam->pppoeCfgParam[i].unnumberedIpAddr,4);
				mySubnet = (tempAclRule.dstIpAddr_ & tempAclRule.dstIpAddrMask_);
				tempAclRule.dstIpAddr_ = mySubnet;
				tempAclRule.ruleType_=RTL8651_ACL_IP;
				tempAclRule.actionType_=RTL8651_ACL_CPU;
			}
			ret = rtl8651a_addAclRule(i,"eth0",1,(rtl8651_tblDrvAclRule_t* )&tempAclRule);
			if (ret!=SUCCESS)
			{
				printf("rtl8651a_addAclRule() ret:%d\n",ret);				
			}
		}
	}



	// flush all trigger port entry in fwdEngine
	rtl8651_flushTriggerPortRules();
			
	// add trigger port	
	for(k=0;k<loopUpperBound;k++)
	{	
		for(i=0;i<MAX_SPECIAL_AP;i++)
		{		
			if((pRomeCfgParam->specialapCfgParam[k][i].outStart!=0)&&(pRomeCfgParam->specialapCfgParam[k][i].enable==1))
			{
				char tempStr1[10]={0},tempStr2[10]={0};
				int idx=0,shift=0;
			
				// add trigger port entry to fwdEngine
				for(j=0;j<SPECIAL_AP_MAX_IN_RANGE;j++)
				{
					if(((pRomeCfgParam->specialapCfgParam[k][i].inRange[j]>='0')&&(pRomeCfgParam->specialapCfgParam[k][i].inRange[j]<='9')))
					{
						if(shift>=9) continue;
						if(idx==0)
							tempStr1[shift++]=pRomeCfgParam->specialapCfgParam[k][i].inRange[j];
						else
							tempStr2[shift++]=pRomeCfgParam->specialapCfgParam[k][i].inRange[j];
						
					}
					else if((pRomeCfgParam->specialapCfgParam[k][i].inRange[j]==',')||
							(pRomeCfgParam->specialapCfgParam[k][i].inRange[j]=='-')||
							(pRomeCfgParam->specialapCfgParam[k][i].inRange[j]==0))
					{
						if(idx==0)
							tempStr1[shift]=0;
						else
							tempStr2[shift]=0;
					
						shift=0;
						if((pRomeCfgParam->specialapCfgParam[k][i].inRange[j]==',')||
							(pRomeCfgParam->specialapCfgParam[k][i].inRange[j]==0))
						{
							uint16 inStart,inFinish;
							inStart=atoi(tempStr1);
							if(idx==0)
								inFinish=inStart;
							else
								inFinish=atoi(tempStr2);
							idx=0;
						
							//make inFinish always bigger than inStart
							if(inStart>inFinish)
							{
								uint16 temp;
								temp=inFinish;
								inFinish=inStart;
								inStart=temp;
							}
							
							if(!((inStart==0)||(inFinish==0)))
							{
								rtl8651_addTriggerPortRule(
								k, //dsid
								pRomeCfgParam->specialapCfgParam[k][i].inType,inStart,inFinish,
								pRomeCfgParam->specialapCfgParam[k][i].outType,
								pRomeCfgParam->specialapCfgParam[k][i].outStart,
								pRomeCfgParam->specialapCfgParam[k][i].outFinish,0);
								//printf("inRange=%d-%d\n",inStart,inFinish);
							}
						}
						else
						{
							idx++;	
						}
						if(pRomeCfgParam->specialapCfgParam[k][i].inRange[j]==0)
							break;
					}				
				}
#if 0 //disable trap to cpu , when turn auto-learn off.
				memset((void*)&tempAclRule,0,sizeof(rtl8651_tblDrvAclRule_t));
				//trap all IP address
				tempAclRule.dstIpAddrMask_=0;
				
				if (pRomeCfgParam->specialapCfgParam[i].outType == ACLCFGPARAM_PROTOCOL_TCP)
				{
					tempAclRule.ruleType_=RTL8651_ACL_TCP;
				
					tempAclRule.tcpDstPortLB_=pRomeCfgParam->specialapCfgParam[i].outStart;
					tempAclRule.tcpDstPortUB_=pRomeCfgParam->specialapCfgParam[i].outFinish;
					tempAclRule.tcpSrcPortLB_=0;
					tempAclRule.tcpSrcPortUB_=0xffff;
					
					tempAclRule.tcpFlagMask_=0;
					tempAclRule.tcpFlag_=0;
					tempAclRule.tcpURG_=0;
					tempAclRule.tcpACK_=0;
					tempAclRule.tcpPSH_=0;
					tempAclRule.tcpRST_=0;
					tempAclRule.tcpSYN_=1;
					tempAclRule.tcpFIN_=0;
				}
				else if (pRomeCfgParam->specialapCfgParam[i].outType == ACLCFGPARAM_PROTOCOL_UDP)
				{
					tempAclRule.ruleType_=RTL8651_ACL_UDP;
					tempAclRule.udpDstPortLB_=pRomeCfgParam->specialapCfgParam[i].outStart;
					tempAclRule.udpDstPortUB_=pRomeCfgParam->specialapCfgParam[i].outFinish;
					tempAclRule.udpSrcPortLB_=0;
					tempAclRule.udpSrcPortUB_=0xffff;	
				}
						
				//trap packets to CPU 
				tempAclRule.actionType_=RTL8651_ACL_CPU;

				if (pRomeCfgParam->natCfgParam.hwaccel==1)
				{
					//add egress rule
					ret = rtl8651a_addAclRule(i,"eth1",TRUE,(rtl8651_tblDrvAclRule_t *)&tempAclRule);
					if (ret!=SUCCESS)
					{
						printf("acl_tableDriverAcess Trigger_Port: rtl8651_addAclRule() ret:%d\n",ret);
						return NOT_OK;
					}
					else
						printf("acl_tableDriverAcess Trigger_Port: rtl8651_addAclRule(if:eth1) ret:%d\n",ret);
				}			
#endif
			}
		} //end special_application
	}
	


	/* set default permit rule
	 * the next loop ensures that rtl8651_setDefaultAcl() is called
	 * once and only once.
	 */
	for(j=0;j<loopUpperBound;j++){
		for (i=0; i<MAX_ACL; i++){
			// Dino Chang modify, 2004/11/08
#if 0
			if ((pRomeCfgParam->aclCfgParam[j][i].enable == 0) ||
                            (check_schedule_day_time(pRomeCfgParam->aclCfgParam[j][i].apptime, pRomeCfgParam->aclCfgParam[j][i].start_day, pRomeCfgParam->aclCfgParam[j][i].end_day, pRomeCfgParam->aclCfgParam[j][i].start_time, pRomeCfgParam->aclCfgParam[j][i].end_time) == 0))
			//if (pRomeCfgParam->aclCfgParam[j][i].enable == 0)
				continue;
#endif
			// Dino Chang
			if (pRomeCfgParam->natCfgParam.hwaccel==1){
				int8 defAction;
				if (pRomeCfgParam->aclGlobalCfgParam.policy==ACLGLOBALCFGPARAM_DENYALLBUTACL)
					defAction = RTL8651_ACL_DROP;
				else if (pRomeCfgParam->aclGlobalCfgParam.policy==ACLGLOBALCFGPARAM_ALLOWALLBUTACL)
					defAction = RTL8651_ACL_PERMIT;
				else if (pRomeCfgParam->aclGlobalCfgParam.policy==ACLGLOBALCFGPARAM_ALLOWALLBUTACL_LOG)
					defAction = RTL8651_ACL_PERMIT;

				ret = rtl8651a_setDefaultAcl(j,"eth0", defAction);
				if (ret)
				{
					printf("acl_tableDriverAcess: rtl8651_setDefaultAcl(eth0) ret:%d\n",ret);
					return NOT_OK;
				}

				ret = rtl8651a_setDefaultAcl(j,"eth1", defAction);
				if (ret)
				{
					printf("acl_tableDriverAcess: rtl8651_setDefaultAcl(eth1) ret:%d\n",ret);
					return NOT_OK;
				}
				break;
			}
		} /* end for i */
	} /* end for j */
	
	/* scan aclCfgParam[] to set rule to asic one by one */	
	for (j=0;j<loopUpperBound;j++){
		for (i=0; i<MAX_ACL; i++){		
			// Dino Chang add, 2004/11/08
			int is_time = 0;
			// Dino Chang
			if (pRomeCfgParam->aclCfgParam[j][i].enable != 1)
				continue;		
			// Dino Chang add, 2004/11/08
			is_time = check_schedule_day_time(pRomeCfgParam->aclCfgParam[j][i].apptime, pRomeCfgParam->aclCfgParam[j][i].start_day, pRomeCfgParam->aclCfgParam[j][i].end_day, pRomeCfgParam->aclCfgParam[j][i].start_time, pRomeCfgParam->aclCfgParam[j][i].end_time);
                        if(is_time == 0) {
                                // if rule is not at time & is deny, continue
                                // or we have to set drop rule
                                if(pRomeCfgParam->aclCfgParam[j][i].allow == 1) continue;
                        }
			// Dino Chang
			memset((void*)&tempAclRule,0,sizeof(rtl8651_tblDrvAclRule_t));
		
			/* prepare acl rule data structure */
// Dino Chang, 2004/11/08
                        // Source IP Address
                        memcpy(&(tempAclRule.srcIpAddrStart_), pRomeCfgParam->aclCfgParam[j][i].ip, 4);
                        memcpy(&(tempAclRule.srcIpAddrEnd_), pRomeCfgParam->aclCfgParam[j][i].src_end_ip, 4);

                        // Destination IP Address
                        memcpy(&(tempAclRule.dstIpAddrStart_), pRomeCfgParam->aclCfgParam[j][i].dst_start_ip, 4);
                        memcpy(&(tempAclRule.dstIpAddrEnd_), pRomeCfgParam->aclCfgParam[j][i].dst_end_ip, 4);

                        // action type
                        if(pRomeCfgParam->aclCfgParam[j][i].allow == 0) { // allow
                                if(is_time == 1)
                                        tempAclRule.actionType_ = RTL8651_ACL_PERMIT;
                                else
                                        tempAclRule.actionType_ = RTL8651_ACL_DROP;
                        } else {
                                tempAclRule.actionType_ = RTL8651_ACL_DROP;
                        }

                        // which interface we want to set rule
                        if(pRomeCfgParam->aclCfgParam[j][i].src_iface == 0) {
                                sprintf(ifStr, ifLanStr);
                        } else {
                                sprintf(ifStr, ifWanStr);
                        }
#if 0
			if (   (pRomeCfgParam->aclCfgParam[j][i].direction == ACLCFGPARAM_DIRECTION_INGRESS_DEST)
			    || (pRomeCfgParam->aclCfgParam[j][i].direction == ACLCFGPARAM_DIRECTION_EGRESS_DEST )
			   )
			{			
				memcpy(&(tempAclRule.dstIpAddr_),pRomeCfgParam->aclCfgParam[j][i].ip,4);
				if (   pRomeCfgParam->aclCfgParam[j][i].ip[0]==0
			    	&& pRomeCfgParam->aclCfgParam[j][i].ip[1]==0
				    && pRomeCfgParam->aclCfgParam[j][i].ip[2]==0
				    && pRomeCfgParam->aclCfgParam[j][i].ip[3]==0)
					tempAclRule.dstIpAddrMask_=0;
				else
					tempAclRule.dstIpAddrMask_=0xffffffff;
			}
			else
			{
				memcpy(&(tempAclRule.srcIpAddr_),pRomeCfgParam->aclCfgParam[j][i].ip,4);
				if (   pRomeCfgParam->aclCfgParam[j][i].ip[0]==0
				    && pRomeCfgParam->aclCfgParam[j][i].ip[1]==0
				    && pRomeCfgParam->aclCfgParam[j][i].ip[2]==0
				    && pRomeCfgParam->aclCfgParam[j][i].ip[3]==0)
					tempAclRule.srcIpAddrMask_=0;
				else
					tempAclRule.srcIpAddrMask_=0xffffffff;
			}
#endif
// Dino Chang
			
			tempAclRule.tos_=0;
			tempAclRule.tosMask_=0;
		
			tempAclRule.ipFlag_=0;
			tempAclRule.ipMF_=0;
			tempAclRule.ipDF_=0;
			//tempAclRule.ipRSV_=0;
				
// Dino Chang 2004/11/08
			if ((pRomeCfgParam->aclCfgParam[j][i].type == ACLCFGPARAM_PROTOCOL_TCP) || (pRomeCfgParam->aclCfgParam[j][i].type == ACLCFGPARAM_PROTOCOL_ANY))
			//if (pRomeCfgParam->aclCfgParam[j][i].type == ACLCFGPARAM_PROTOCOL_TCP)
// Dino Chang
			{
				tempAclRule.ruleType_=RTL8651_ACL_TCP;
				tempAclRule.tcpFlagMask_=0;
// Dino Chang, 2004/11/08
                                if(pRomeCfgParam->aclCfgParam[j][i].port == 0) {
                                        tempAclRule.tcpDstPortLB_ = 0;
                                        tempAclRule.tcpDstPortUB_ = 0xffff;
                                } else {
                                        tempAclRule.tcpDstPortLB_ = pRomeCfgParam->aclCfgParam[j][i].port;
                                        tempAclRule.tcpDstPortUB_ = pRomeCfgParam->aclCfgParam[j][i].end_port;
                                }

                                tempAclRule.tcpSrcPortLB_ = 0;
                                tempAclRule.tcpSrcPortUB_ = 0xffff;
#if 0
				if (  (pRomeCfgParam->aclCfgParam[j][i].direction==ACLCFGPARAM_DIRECTION_INGRESS_DEST)
				    ||(pRomeCfgParam->aclCfgParam[j][i].direction==ACLCFGPARAM_DIRECTION_EGRESS_DEST )
				   )
				{
					if (pRomeCfgParam->aclCfgParam[j][i].port==0){
						tempAclRule.tcpDstPortLB_=0;
						tempAclRule.tcpDstPortUB_=0xffff;
					}else{
						tempAclRule.tcpDstPortLB_=pRomeCfgParam->aclCfgParam[j][i].port;
						tempAclRule.tcpDstPortUB_=pRomeCfgParam->aclCfgParam[j][i].port;
					}
					tempAclRule.tcpSrcPortLB_=0;
					tempAclRule.tcpSrcPortUB_=0xffff;
				}
				else
				{
					if (pRomeCfgParam->aclCfgParam[j][i].port==0){
						tempAclRule.tcpSrcPortLB_=0;
						tempAclRule.tcpSrcPortUB_=0xffff;
					}else{
						tempAclRule.tcpSrcPortLB_=pRomeCfgParam->aclCfgParam[j][i].port;
						tempAclRule.tcpSrcPortUB_=pRomeCfgParam->aclCfgParam[j][i].port;
					}
					tempAclRule.tcpDstPortLB_=0;
					tempAclRule.tcpDstPortUB_=0xffff;
				}
#endif
// Dino Chang
				tempAclRule.tcpFlag_=0;
				tempAclRule.tcpURG_=0;
				tempAclRule.tcpACK_=0;
				tempAclRule.tcpPSH_=0;
				tempAclRule.tcpRST_=0;
				tempAclRule.tcpSYN_=0;
				tempAclRule.tcpFIN_=0;

// Dino Chang, 2004/11/08
                                if (pRomeCfgParam->natCfgParam.hwaccel == 1) {
                                        ret = rtl8651a_addAclRuleExt(j, ifStr, 1, (rtl8651_tblDrvAclRule_t *)&tempAclRule);
                                        if (ret != SUCCESS) {
                                                printf("acl_tableDriverAcess: rtl8651a_addAclRuleExt() for TCP ret:%d\n",ret);
                                                pRomeCfgParam->aclCfgParam[j][i].enable=0;
                                                return NOT_OK;
                                        } else
                                                printf("acl_tableDriverAcess: rtl8651a_addAclRule(if:%s) for TCP ret:%d\n",ifStr,ret);
			}
// Dino
			}
// Dino Chang, 2004/11/08
			if ((pRomeCfgParam->aclCfgParam[j][i].type == ACLCFGPARAM_PROTOCOL_UDP) || (pRomeCfgParam->aclCfgParam[j][i].type == ACLCFGPARAM_PROTOCOL_ANY))
			//else if (pRomeCfgParam->aclCfgParam[j][i].type == ACLCFGPARAM_PROTOCOL_UDP)
// Dino Chang
			{
				tempAclRule.ruleType_=RTL8651_ACL_UDP;
// Dino Chang 2004/11/08
                                if(pRomeCfgParam->aclCfgParam[j][i].port == 0) {
                                        tempAclRule.udpDstPortLB_ = 0;
                                        tempAclRule.udpDstPortUB_ = 0xffff;
                                } else {
                                        tempAclRule.udpDstPortLB_ = pRomeCfgParam->aclCfgParam[j][i].port;
                                        tempAclRule.udpDstPortUB_ = pRomeCfgParam->aclCfgParam[j][i].end_port;
                                }
                                tempAclRule.udpSrcPortLB_ = 0;
                                tempAclRule.udpSrcPortUB_ = 0xffff;

                                if (pRomeCfgParam->natCfgParam.hwaccel == 1) {
                                        ret = rtl8651a_addAclRuleExt(j, ifStr, 1, (rtl8651_tblDrvAclRule_t *)&tempAclRule);
                                        if (ret != SUCCESS) {
                                                printf("acl_tableDriverAcess: rtl8651a_addAclRuleExt() for UDP ret:%d\n",ret);
                                                pRomeCfgParam->aclCfgParam[j][i].enable=0;
                                                return NOT_OK;
                                        } else
                                                printf("acl_tableDriverAcess: rtl8651a_addAclRule(if:%s) for UDP ret:%d\n",ifStr,ret);
                                }
#if 0
				if (  (pRomeCfgParam->aclCfgParam[j][i].direction==ACLCFGPARAM_DIRECTION_INGRESS_DEST)
				    ||(pRomeCfgParam->aclCfgParam[j][i].direction==ACLCFGPARAM_DIRECTION_EGRESS_DEST )
				   )
				{
					if (pRomeCfgParam->aclCfgParam[j][i].port==0){
						tempAclRule.udpDstPortLB_=0;
						tempAclRule.udpDstPortUB_=0xffff;
					}else{
						tempAclRule.udpDstPortLB_=pRomeCfgParam->aclCfgParam[j][i].port;
						tempAclRule.udpDstPortUB_=pRomeCfgParam->aclCfgParam[j][i].port;
					}
					tempAclRule.udpSrcPortLB_=0;
					tempAclRule.udpSrcPortUB_=0xffff;
				}
				else
				{
					if (pRomeCfgParam->aclCfgParam[j][i].port==0){
						tempAclRule.udpSrcPortLB_=0;
						tempAclRule.udpSrcPortUB_=0xffff;
					}else{
						tempAclRule.udpSrcPortLB_=pRomeCfgParam->aclCfgParam[j][i].port;
						tempAclRule.udpSrcPortUB_=pRomeCfgParam->aclCfgParam[j][i].port;
					}
					tempAclRule.udpDstPortLB_=0;
					tempAclRule.udpDstPortUB_=0xffff;
				}
#endif
// Dino Chang
			}
// Dino Chang 2004/11/08
			if (pRomeCfgParam->aclCfgParam[j][i].type == ACLCFGPARAM_PROTOCOL_IP)
			//else if (pRomeCfgParam->aclCfgParam[j][i].type == ACLCFGPARAM_PROTOCOL_IP)
// Dino
			{
				tempAclRule.ruleType_=RTL8651_ACL_IP;
				tempAclRule.tcpFlagMask_=0;
#if 0				
				if (  (pRomeCfgParam->aclCfgParam[j][i].direction==ACLCFGPARAM_DIRECTION_INGRESS_DEST)
				    ||(pRomeCfgParam->aclCfgParam[j][i].direction==ACLCFGPARAM_DIRECTION_EGRESS_DEST )
				   )
				{
				
					tempAclRule.tcpDstPortLB_=0;
					tempAclRule.tcpDstPortUB_=0xffff;				
					tempAclRule.tcpSrcPortLB_=0;
					tempAclRule.tcpSrcPortUB_=0xffff;
				}
				else
				{				
					tempAclRule.tcpSrcPortLB_=0;
					tempAclRule.tcpSrcPortUB_=0xffff;				
					tempAclRule.tcpDstPortLB_=0;
					tempAclRule.tcpDstPortUB_=0xffff;
				}
#endif
				tempAclRule.ipFlag_=0;
				tempAclRule.ipFlagMask_=0;

// Dino Chang 2004/11/08
                                if (pRomeCfgParam->natCfgParam.hwaccel == 1) {
                                        ret = rtl8651a_addAclRuleExt(j, ifStr, 1, (rtl8651_tblDrvAclRule_t *)&tempAclRule);
                                        if (ret != SUCCESS) {
                                                printf("acl_tableDriverAcess: rtl8651a_addAclRuleExt() for IP ret:%d\n",ret);
                                                pRomeCfgParam->aclCfgParam[j][i].enable=0;
                                                return NOT_OK;
                                        } else
                                                printf("acl_tableDriverAcess: rtl8651a_addAclRule(if:%s) for IP ret:%d\n",ifStr,ret);
                                }
// Dino
			}			
// Dino Chang 2004/11/08
                        if ((pRomeCfgParam->aclCfgParam[j][i].type == ACLCFGPARAM_PROTOCOL_ICMP) || (pRomeCfgParam->aclCfgParam[j][i].type == ACLCFGPARAM_PROTOCOL_ANY)) 
                        {
                        	// Forced Protocol ANY blocked by IP
                                if(pRomeCfgParam->aclCfgParam[j][i].type == ACLCFGPARAM_PROTOCOL_ANY)
                                {
                                	tempAclRule.ruleType_=RTL8651_ACL_IP;
					tempAclRule.tcpFlagMask_=0;
					tempAclRule.ipFlag_=0;
					tempAclRule.ipFlagMask_=0;
                                	
                                	if (pRomeCfgParam->natCfgParam.hwaccel == 1) {
                                	        ret = rtl8651a_addAclRuleExt(j, ifStr, 1, (rtl8651_tblDrvAclRule_t *)&tempAclRule);
                                	        if (ret != SUCCESS) {
                                	                printf("acl_tableDriverAcess: rtl8651a_addAclRuleExt() for IP ret:%d\n",ret);
                                	                pRomeCfgParam->aclCfgParam[j][i].enable=0;
                                	                return NOT_OK;
                                	        } else
                                	                printf("acl_tableDriverAcess: rtl8651a_addAclRule(if:%s) for IP ret:%d\n",ifStr,ret);
                                	}	
                                }
                                else
                                {
                                	if(pRomeCfgParam->aclCfgParam[j][i].src_iface == -1) 
                                	{
                                		// Set LAN first
                                		sprintf(ifStr, ifLanStr);
                                		tempAclRule.ruleType_ = RTL8651_ACL_ICMP;
                                		tempAclRule.icmpType_ = pRomeCfgParam->aclCfgParam[j][i].port;
                                		tempAclRule.icmpTypeMask_ = 255;
                                		tempAclRule.icmpCodeMask_ = 255;
                                		if (pRomeCfgParam->natCfgParam.hwaccel == 1) 
                                		{
                                	        	ret = rtl8651a_addAclRuleExt(j, ifStr, 1, (rtl8651_tblDrvAclRule_t *)&tempAclRule);
                                	        	if (ret != SUCCESS) 
                                	        	{
                                	                	printf("acl_tableDriverAcess: rtl8651a_addAclRuleExt() for ICMP ret:%d\n",ret);
                                	                	pRomeCfgParam->aclCfgParam[j][i].enable=0;
                                	                	return NOT_OK;
                                	        	} else
                                	                	printf("acl_tableDriverAcess: rtl8651a_addAclRule(if:%s) for ICMP ret:%d\n",ifStr,ret);
                               		 	}
                        		}
                                	else
                                	{
                                		tempAclRule.ruleType_ = RTL8651_ACL_ICMP;
                                		tempAclRule.icmpType_ = pRomeCfgParam->aclCfgParam[j][i].port;
                                		tempAclRule.icmpTypeMask_ = 255;
                                		tempAclRule.icmpCodeMask_ = 255;
                                		if (pRomeCfgParam->natCfgParam.hwaccel == 1) {
                                	        	ret = rtl8651a_addAclRuleExt(j, ifStr, 1, (rtl8651_tblDrvAclRule_t *)&tempAclRule);
                                	        	if (ret != SUCCESS) {
                                	                	printf("acl_tableDriverAcess: rtl8651a_addAclRuleExt() for ICMP ret:%d\n",ret);
                                	                	pRomeCfgParam->aclCfgParam[j][i].enable=0;
                                	                	return NOT_OK;
                                	        	} else
                                	                	printf("acl_tableDriverAcess: rtl8651a_addAclRule(if:%s) for ICMP ret:%d\n",ifStr,ret);
                                		}
                        		}
                        	}
			}			
					
#if 0
			if (pRomeCfgParam->aclGlobalCfgParam.policy == ACLGLOBALCFGPARAM_ALLOWALLBUTACL)
				tempAclRule.actionType_=RTL8651_ACL_DROP;
			else if (pRomeCfgParam->aclGlobalCfgParam.policy == ACLGLOBALCFGPARAM_DENYALLBUTACL)
				tempAclRule.actionType_=RTL8651_ACL_PERMIT;
			else if(pRomeCfgParam->aclGlobalCfgParam.policy == ACLGLOBALCFGPARAM_ALLOWALLBUTACL_LOG)
				tempAclRule.actionType_=RTL8651_ACL_DROP_LOG;			

			if (  (pRomeCfgParam->aclCfgParam[j][i].direction==ACLCFGPARAM_DIRECTION_INGRESS_DEST)
				||(pRomeCfgParam->aclCfgParam[j][i].direction==ACLCFGPARAM_DIRECTION_INGRESS_SRC )
			   )
				sprintf(ifStr,ifLanStr);
			else
				sprintf(ifStr,ifWanStr);
			
			if (pRomeCfgParam->natCfgParam.hwaccel==1)
			{
				ret = rtl8651a_addAclRule(j, ifStr,
										  (pRomeCfgParam->aclCfgParam[j][i].direction==ACLCFGPARAM_DIRECTION_INGRESS_DEST)
										||(pRomeCfgParam->aclCfgParam[j][i].direction==ACLCFGPARAM_DIRECTION_INGRESS_SRC ),
										(rtl8651_tblDrvAclRule_t *)&tempAclRule);
				if (ret!=SUCCESS)
				{
					printf("acl_tableDriverAcess: rtl8651a_addAclRule() ret:%d\n",ret);
					pRomeCfgParam->aclCfgParam[j][i].enable=0;
					return NOT_OK;
				}
				else
					printf("acl_tableDriverAcess: rtl8651a_addAclRule(if:%s) ret:%d\n",ifStr,ret);
			}
#endif
// Dino Chang
		
		} /* end for i */
	} /* end for j */
	
	return OK;	
} /* end acl_tableDriverAccess */


/*
 * Routine Name: acl_init
 * Description: configure acl cfg param entry
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 acl_init(void)
{	
	if (cfgmgr_read(CFGMGR_TABID_ACL_GLOBAL,(void*)&(pRomeCfgParam->aclGlobalCfgParam), sizeof(pRomeCfgParam->aclGlobalCfgParam))!=0)
	{
		printf("acl_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return NOT_OK;
	}
	
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_ACL,(void*)pRomeCfgParam->aclCfgParam, sizeof(struct aclCfgParam_s)*MAX_ACL*MAX_PPPOE_SESSION)!=0)
	{
		printf("acl_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return NOT_OK;
	}
		
	return OK;	
} /* end acl_init */

#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
void pptpCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(struct pptpCfgParam_s);
	pptpCfgParam_t tmpTab;
	
	memset((void*)&tmpTab, 0, mySize);
	memcpy((void*)&tmpTab, (void*)&pptpCfgParamDefault, sizeof(struct pptpCfgParam_s));
	memcpy((void*)pBuf, (void*)&tmpTab, mySize);
	
	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end pptpCfgFactoryDefaultFunction */

void l2tpCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(struct l2tpCfgParam_s);
	l2tpCfgParam_t tmpTab;
	
	memset((void*)&tmpTab, 0, mySize);
	memcpy((void*)&tmpTab, (void*)&l2tpCfgParamDefault, sizeof(struct l2tpCfgParam_s));
	memcpy((void*)pBuf, (void*)&tmpTab, mySize);
	
	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end l2tpCfgFactoryDefaultFunction */


#if 0
void dhcpL2tpCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(struct dhcpL2tpCfgParam_s);
	l2tpCfgParam_t tmpTab;
	
	memset((void*)&tmpTab, 0, mySize);
	memcpy((void*)&tmpTab, (void*)&dhcpL2tpCfgParamDefault, sizeof(struct dhcpL2tpCfgParam_s));
	memcpy((void*)pBuf, (void*)&tmpTab, mySize);
	
	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} 
#endif



uint32 pptp_init(void)
{	
	if (cfgmgr_read(CFGMGR_TABID_PPTP,(void*)&(pRomeCfgParam->pptpCfgParam), sizeof(struct pptpCfgParam_s))!=0)
	{
		printf("pptp_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return NOT_OK;
	}
	
	return OK;	
} /* end pptp_init */

uint32 l2tp_init(void)
{	
	if (cfgmgr_read(CFGMGR_TABID_L2TP,(void*)&(pRomeCfgParam->l2tpCfgParam), sizeof(struct l2tpCfgParam_s))!=0)
	{
		printf("l2tp_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return NOT_OK;
	}	
	return OK;	
} /* end l2tp_init */


#if 0
uint32 dhcpL2tp_init(void)
{	
	if (cfgmgr_read(CFGMGR_TABID_DHCPL2TP,(void*)&(pRomeCfgParam->dhcpL2tpCfgParam), sizeof(struct dhcpL2tpCfgParam_s))!=0)
	{
		printf("dhcp_l2tp_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return NOT_OK;
	}	
	return OK;	
} 
#endif




#endif //CONFIG_RTL865X(B)_PPTPL2TP

void serverpCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(struct serverpCfgParam_s)*MAX_SERVER_PORT*MAX_PPPOE_SESSION;
	serverpCfgParam_t tmpTab[MAX_PPPOE_SESSION][MAX_SERVER_PORT];
	
	memset((void*)tmpTab, 0, mySize);
	memcpy((void*)tmpTab, (void*)serverpCfgParamDefault, sizeof(struct serverpCfgParam_s)*MAX_SERVER_PORT*MAX_PPPOE_SESSION);
	memcpy((void*)pBuf, (void*)tmpTab, mySize);
	
	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end serverpCfgFactoryDefaultFunction */

// Dino Chang 2005/03/09
#define IP_ADDR(w,x,y,z)    ((unsigned long)(((unsigned long)w << 24) | \
                                             ((unsigned long)x << 16) | \
                                             ((unsigned long)y << 8) | \ 
                                             (unsigned long)z))
// Dino

// Dino Chang 2005/03/07
int vs_to_alg(serverpCfgParam_t *serverp, int action) {

        if((serverp->protocol == SPECIALACPCFGPARAM_PROTOCOL_TCP) &&
           (serverp->portStart == 1723) &&
           (serverp->wanPortStart == 1723)) { // PPTP
                if(action == 0) // delete
                        pRomeCfgParam->algCfgParam[0].algList &= (~RTL8651_ALG_PPTP);
                else if(action == 1) // add
                        pRomeCfgParam->algCfgParam[0].algList |= RTL8651_ALG_PPTP;
		// Dino Chang 2005/03/09
		pRomeCfgParam->algCfgParam[0].localIp[RTL8651_ALG_PPTP_IDX] = IP_ADDR(serverp->ip[0], serverp->ip[1], serverp->ip[2], serverp->ip[3]);
                //pRomeCfgParam->algCfgParam[0].localIp[RTL8651_ALG_PPTP_IDX] = serverp->ip;
		// Dino
                rtl8651a_setAlgStatus(0, pRomeCfgParam->algCfgParam[0].algList, pRomeCfgParam->algCfgParam[0].localIp);
                return SUCCESS;
        } else if((serverp->protocol == SPECIALACPCFGPARAM_PROTOCOL_UDP) &&
                  (serverp->portStart == 500) &&
                  (serverp->wanPortStart == 500)) { // IPSec
                if(action == 0) // delete
                        pRomeCfgParam->algCfgParam[0].algList &= (~RTL8651_ALG_IPSEC);
                else if(action == 1) // add
                        pRomeCfgParam->algCfgParam[0].algList |= RTL8651_ALG_IPSEC;
		// Dino Chang 2005/03/09
		pRomeCfgParam->algCfgParam[0].localIp[RTL8651_ALG_IPSEC_IDX] = IP_ADDR(serverp->ip[0], serverp->ip[1], serverp->ip[2], serverp->ip[3]);
                //pRomeCfgParam->algCfgParam[0].localIp[RTL8651_ALG_IPSEC_IDX] = serverp->ip;
		// Dino
                rtl8651a_setAlgStatus(0, pRomeCfgParam->algCfgParam[0].algList, pRomeCfgParam->algCfgParam[0].localIp);
                return SUCCESS;
        } else if((serverp->protocol == SPECIALACPCFGPARAM_PROTOCOL_TCP) &&
                  (serverp->portStart == 1720) &&
                  (serverp->wanPortStart == 1720)) { // NetMeeting
                if(action == 0) // delete
                        pRomeCfgParam->algCfgParam[0].algList &= (~RTL8651_ALG_NETMEETING);
                else if(action == 1) // add
                        pRomeCfgParam->algCfgParam[0].algList |= RTL8651_ALG_NETMEETING;
		// Dino Chang 2005/03/09
		pRomeCfgParam->algCfgParam[0].localIp[RTL8651_ALG_NETMEETING_IDX] = IP_ADDR(serverp->ip[0], serverp->ip[1], serverp->ip[2], serverp->ip[3]);
                //pRomeCfgParam->algCfgParam[0].localIp[RTL8651_ALG_NETMEETING_IDX] = serverp->ip;
		// Dino
                rtl8651a_setAlgStatus(0, pRomeCfgParam->algCfgParam[0].algList, pRomeCfgParam->algCfgParam[0].localIp);
                return SUCCESS;
        }
        return FAILURE;
}
// Dino

/*
 * Routine Name: serverp_tableDriverAccess
 * Description: access table driver server port APIs
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 serverp_tableDriverAccess(uint32 dsid)
{
	int32 ret;
	ipaddr_t wanIp;
	ipaddr_t tempIpAddr;
	int i;

	// Dino Chang 2005/03/07
	unsigned char alg_pptp = 0, alg_ipsec = 0;
	// Dino

	if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)
	{
		memcpy((void*)&wanIp,pRomeCfgParam->pppoeCfgParam[dsid].ipAddr,4);			
	}
	else
	{
		memcpy((void*)&wanIp,(void*)(pRomeCfgParam->ifCfgParam[0].ipAddr),4);		
	}

	if(wanIp==0)
	{
		printf("serverp_tableDriverAccess: no valid wan ip!\n");
		return NOT_OK;
	}

				
		/* for loop to delete all "enabled" entry in serverpCfgParam[] */	
		for (i=0;i<MAX_SERVER_PORT;i++)
		{	
			// Dino Chang 2004/11/08
			//if ((ramServerpCfgParam[dsid][i].enable == 0) || (check_schedule_day_time(ramServerpCfgParam[dsid][i].apptime, ramServerpCfgParam[dsid][i].start_day, ramServerpCfgParam[dsid][i].end_day, ramServerpCfgParam[dsid][i].start_time, ramServerpCfgParam[dsid][i].end_time) == 0))
			if (ramServerpCfgParam[dsid][i].enable == 0)
			// Dino Chang
				continue;

			// Dino Chang 2005/03/07
			if(vs_to_alg(&ramServerpCfgParam[dsid][i], 0) == SUCCESS) // delete alg
				continue;
			// Dino

			memcpy((void*)&tempIpAddr,(void*)(ramServerpCfgParam[dsid][i].ip),4);

			if(ramServerpCfgParam[dsid][i].wanPortStart==ramServerpCfgParam[dsid][i].wanPortFinish)
			{			
// Dino Chang 2004/12/21
#if 0
				ret = rtl8651a_delNaptServerPortMapping(
					dsid,
					(ramServerpCfgParam[dsid][i].protocol==SERVERPCFGPARAM_PROTOCOL_TCP),
					(ipaddr_t) wanIp,                     /* current wan ip */
					(uint16)   ramServerpCfgParam[dsid][i].wanPortStart,
					(ipaddr_t) tempIpAddr,
					(uint16)   ramServerpCfgParam[dsid][i].portStart);

				if (ret != SUCCESS)
				{					
					printf("serverp_tableDriverAccess: rtl8651_delNaptServerPortMapping() ret:%d\n",ret);
					//return NOT_OK;
				}				
#else
				if((ramServerpCfgParam[dsid][i].protocol == SERVERPCFGPARAM_PROTOCOL_TCP) || (ramServerpCfgParam[dsid][i].protocol == SERVERPCFGPARAM_PROTOCOL_BOTH)) {
					ret = rtl8651a_delNaptServerPortMapping(
						dsid,
						1,
						(ipaddr_t) wanIp,                     /* current wan ip */
						(uint16)   ramServerpCfgParam[dsid][i].wanPortStart,
						(ipaddr_t) tempIpAddr,
						(uint16)   ramServerpCfgParam[dsid][i].portStart);

					if (ret != SUCCESS)
					{					
						printf("serverp_tableDriverAccess: rtl8651_delNaptServerPortMapping() ret:%d\n",ret);
						//return NOT_OK;
					}
				}

				if((ramServerpCfgParam[dsid][i].protocol == SERVERPCFGPARAM_PROTOCOL_UDP) || (ramServerpCfgParam[dsid][i].protocol == SERVERPCFGPARAM_PROTOCOL_BOTH)) {
					ret = rtl8651a_delNaptServerPortMapping(
						dsid,
						0,
						(ipaddr_t) wanIp,                     /* current wan ip */
						(uint16)   ramServerpCfgParam[dsid][i].wanPortStart,
						(ipaddr_t) tempIpAddr,
						(uint16)   ramServerpCfgParam[dsid][i].portStart);

					if (ret != SUCCESS)
					{					
						printf("serverp_tableDriverAccess: rtl8651_delNaptServerPortMapping() ret:%d\n",ret);
						//return NOT_OK;
					}
				}
#endif
// Dino		
			}
			else
			{
// Dino Chang 2004/12/21
#if 0
				ret = rtl8651a_delNaptServerPortRange(
					dsid,
					(ramServerpCfgParam[dsid][i].protocol==SERVERPCFGPARAM_PROTOCOL_TCP),
					(ipaddr_t)wanIp,                     /* current wan ip */
					(uint16)ramServerpCfgParam[dsid][i].wanPortStart,
					(ipaddr_t)tempIpAddr,
					(uint16)ramServerpCfgParam[dsid][i].portStart,
					(uint16)ramServerpCfgParam[dsid][i].wanPortFinish-ramServerpCfgParam[dsid][i].wanPortStart+1);

				if (ret != SUCCESS)
				{
					printf("serverp_tableDriverAccess: rtl8651_delNaptServerPortRange() ret:%d\n",ret);
					//return NOT_OK;
				}				
#else
				if((ramServerpCfgParam[dsid][i].protocol == SERVERPCFGPARAM_PROTOCOL_TCP) || (ramServerpCfgParam[dsid][i].protocol == SERVERPCFGPARAM_PROTOCOL_BOTH)) {
					ret = rtl8651a_delNaptServerPortRange(
						dsid,
						1,
						(ipaddr_t)wanIp,                     /* current wan ip */
						(uint16)ramServerpCfgParam[dsid][i].wanPortStart,
						(ipaddr_t)tempIpAddr,
						(uint16)ramServerpCfgParam[dsid][i].portStart,
						(uint16)ramServerpCfgParam[dsid][i].wanPortFinish-ramServerpCfgParam[dsid][i].wanPortStart+1);

					if (ret != SUCCESS)
					{
						printf("serverp_tableDriverAccess: rtl8651_delNaptServerPortRange() ret:%d\n",ret);
						//return NOT_OK;
					}
				}
			
				if((ramServerpCfgParam[dsid][i].protocol == SERVERPCFGPARAM_PROTOCOL_UDP) || (ramServerpCfgParam[dsid][i].protocol == SERVERPCFGPARAM_PROTOCOL_BOTH)) {
					ret = rtl8651a_delNaptServerPortRange(
						dsid,
						0,
						(ipaddr_t)wanIp,                     /* current wan ip */
						(uint16)ramServerpCfgParam[dsid][i].wanPortStart,
						(ipaddr_t)tempIpAddr,
						(uint16)ramServerpCfgParam[dsid][i].portStart,
						(uint16)ramServerpCfgParam[dsid][i].wanPortFinish-ramServerpCfgParam[dsid][i].wanPortStart+1);

					if (ret != SUCCESS)
					{
						printf("serverp_tableDriverAccess: rtl8651_delNaptServerPortRange() ret:%d\n",ret);
						//return NOT_OK;
					}
				}
#endif
				
			}
			
			
		} /* end for */
	
	
	/* for loop to add all "enabled" entry in newServerpCfgParam[] */
	for (i=0;i<MAX_SERVER_PORT;i++)
	{
                // Dino Chang 2005/03/07
                // record if there are PPTP and IPSec virtual server in lists
                if((pRomeCfgParam->serverpCfgParam[dsid][i].protocol == SPECIALACPCFGPARAM_PROTOCOL_TCP) &&
                   (pRomeCfgParam->serverpCfgParam[dsid][i].portStart == 1723) &&
                   (pRomeCfgParam->serverpCfgParam[dsid][i].wanPortStart == 1723) &&
                   (pRomeCfgParam->serverpCfgParam[dsid][i].enable == 1)) { // PPTP
                        alg_pptp = 1;
                } else if((pRomeCfgParam->serverpCfgParam[dsid][i].protocol == SPECIALACPCFGPARAM_PROTOCOL_UDP) &&
                   (pRomeCfgParam->serverpCfgParam[dsid][i].portStart == 500) &&
                   (pRomeCfgParam->serverpCfgParam[dsid][i].wanPortStart == 500)
 &&
                   (pRomeCfgParam->serverpCfgParam[dsid][i].enable == 1)) { // IPSec
                        alg_ipsec = 1;
                }
                // Dino
		
		// Dino Chang, 2004/11/08
		if ((pRomeCfgParam->serverpCfgParam[dsid][i].enable == 0) ||
                    (check_schedule_day_time(pRomeCfgParam->serverpCfgParam[dsid][i].apptime, pRomeCfgParam->serverpCfgParam[dsid][i].start_day, pRomeCfgParam->serverpCfgParam[dsid][i].end_day, pRomeCfgParam->serverpCfgParam[dsid][i].start_time, pRomeCfgParam->serverpCfgParam[dsid][i].end_time) == 0))
		//if (pRomeCfgParam->serverpCfgParam[dsid][i].enable == 0)
		// Dino Chang
			continue;
			
		/* enable/create some serverp entry */
		if ( (    (pRomeCfgParam->serverpCfgParam[dsid][i].ip[0] == 0)
			   && (pRomeCfgParam->serverpCfgParam[dsid][i].ip[1] == 0)
			   && (pRomeCfgParam->serverpCfgParam[dsid][i].ip[2] == 0)
			   && (pRomeCfgParam->serverpCfgParam[dsid][i].ip[3] == 0)
			  )
			 || (pRomeCfgParam->serverpCfgParam[dsid][i].portStart == 0)
			 || (pRomeCfgParam->serverpCfgParam[dsid][i].wanPortStart == 0)
			)
		{
			/* clear this invalid rule */
			memset(&(pRomeCfgParam->serverpCfgParam[dsid][i]),0,sizeof(serverpCfgParam_t));
		}
		else
		{

			// Dino Chang 2005/03/07
			if(vs_to_alg(&pRomeCfgParam->serverpCfgParam[dsid][i], 1) == SUCCESS) // add
                                continue;
			// Dino
			memcpy((void*)&tempIpAddr,(void*)(pRomeCfgParam->serverpCfgParam[dsid][i].ip),4);

			
			
			if(pRomeCfgParam->serverpCfgParam[dsid][i].wanPortStart==pRomeCfgParam->serverpCfgParam[dsid][i].wanPortFinish)
			{	
// Dino Chang 2004/12/21
#if 0	
				ret = rtl8651a_addNaptServerPortMapping(
					dsid,
					(pRomeCfgParam->serverpCfgParam[dsid][i].protocol==SERVERPCFGPARAM_PROTOCOL_TCP),
					(ipaddr_t) wanIp,               /* current wan ip */
					(uint16)   pRomeCfgParam->serverpCfgParam[dsid][i].wanPortStart,
					(ipaddr_t) tempIpAddr,
					(uint16)   pRomeCfgParam->serverpCfgParam[dsid][i].portStart);

				if (ret != SUCCESS)
				{
					ramServerpCfgParam[dsid][i].enable = 0;
					pRomeCfgParam->serverpCfgParam[dsid][i].enable=0;				
					printf("serverp_tableDriverAccess: rtl8651_addNaptServerPortMapping(wanip:%0x,wanport:%u,localip:%0x,localport:%u) ret:%d\n",wanIp,pRomeCfgParam->serverpCfgParam[dsid][i].wanPortStart,tempIpAddr,pRomeCfgParam->serverpCfgParam[dsid][i].portStart,ret);
					return NOT_OK;
				}				
#else
				if((pRomeCfgParam->serverpCfgParam[dsid][i].protocol==SERVERPCFGPARAM_PROTOCOL_TCP) || (pRomeCfgParam->serverpCfgParam[dsid][i].protocol==SERVERPCFGPARAM_PROTOCOL_BOTH)) {
					ret = rtl8651a_addNaptServerPortMapping(
						dsid,
						1,
						(ipaddr_t) wanIp,               /* current wan ip */
						(uint16)   pRomeCfgParam->serverpCfgParam[dsid][i].wanPortStart,
						(ipaddr_t) tempIpAddr,
						(uint16)   pRomeCfgParam->serverpCfgParam[dsid][i].portStart);

					if (ret != SUCCESS)
					{
						ramServerpCfgParam[dsid][i].enable = 0;
						pRomeCfgParam->serverpCfgParam[dsid][i].enable=0;				
						printf("serverp_tableDriverAccess: rtl8651_addNaptServerPortMapping(wanip:%0x,wanport:%u,localip:%0x,localport:%u) ret:%d\n",wanIp,pRomeCfgParam->serverpCfgParam[dsid][i].wanPortStart,tempIpAddr,pRomeCfgParam->serverpCfgParam[dsid][i].portStart,ret);
						return NOT_OK;
					}
				}

				if((pRomeCfgParam->serverpCfgParam[dsid][i].protocol == SERVERPCFGPARAM_PROTOCOL_UDP) || (pRomeCfgParam->serverpCfgParam[dsid][i].protocol == SERVERPCFGPARAM_PROTOCOL_BOTH)) {
					ret = rtl8651a_addNaptServerPortMapping(
						dsid,
						0,
						(ipaddr_t) wanIp,               /* current wan ip */
						(uint16)   pRomeCfgParam->serverpCfgParam[dsid][i].wanPortStart,
						(ipaddr_t) tempIpAddr,
						(uint16)   pRomeCfgParam->serverpCfgParam[dsid][i].portStart);

					if (ret != SUCCESS)
					{
						ramServerpCfgParam[dsid][i].enable = 0;
						pRomeCfgParam->serverpCfgParam[dsid][i].enable=0;				
						printf("serverp_tableDriverAccess: rtl8651_addNaptServerPortMapping(wanip:%0x,wanport:%u,localip:%0x,localport:%u) ret:%d\n",wanIp,pRomeCfgParam->serverpCfgParam[dsid][i].wanPortStart,tempIpAddr,pRomeCfgParam->serverpCfgParam[dsid][i].portStart,ret);
						return NOT_OK;
					}
				}
#endif
// Dino
			}
			else
			{

// Dino Chang 2004/12/21
#if 0
				ret = rtl8651a_addNaptServerPortRange(
					dsid,
					(pRomeCfgParam->serverpCfgParam[dsid][i].protocol==SERVERPCFGPARAM_PROTOCOL_TCP),
					(ipaddr_t) wanIp,               /* current wan ip */
					(uint16)   pRomeCfgParam->serverpCfgParam[dsid][i].wanPortStart,
					(ipaddr_t) tempIpAddr,
					(uint16)   pRomeCfgParam->serverpCfgParam[dsid][i].portStart, 
					(uint16) pRomeCfgParam->serverpCfgParam[dsid][i].wanPortFinish-pRomeCfgParam->serverpCfgParam[dsid][i].wanPortStart+1);		

				if (ret != SUCCESS)
				{
					ramServerpCfgParam[dsid][i].enable = 0;
					pRomeCfgParam->serverpCfgParam[dsid][i].enable=0;				
					printf("serverp_tableDriverAccess: rtl8651_addNaptServerPortRange(wanip:%0x,wanport:%u,localip:%0x,localport:%u) ret:%d\n",wanIp,pRomeCfgParam->serverpCfgParam[dsid][i].wanPortStart,tempIpAddr,pRomeCfgParam->serverpCfgParam[dsid][i].portStart,ret);
					return NOT_OK;
				}				
#else
				if((pRomeCfgParam->serverpCfgParam[dsid][i].protocol==SERVERPCFGPARAM_PROTOCOL_TCP) || (pRomeCfgParam->serverpCfgParam[dsid][i].protocol == SERVERPCFGPARAM_PROTOCOL_BOTH)) {
					ret = rtl8651a_addNaptServerPortRange(
						dsid,
						1,
						(ipaddr_t) wanIp,               /* current wan ip */
						(uint16)   pRomeCfgParam->serverpCfgParam[dsid][i].wanPortStart,
						(ipaddr_t) tempIpAddr,
						(uint16)   pRomeCfgParam->serverpCfgParam[dsid][i].portStart, 
						(uint16) pRomeCfgParam->serverpCfgParam[dsid][i].wanPortFinish-pRomeCfgParam->serverpCfgParam[dsid][i].wanPortStart+1);		

					if (ret != SUCCESS)
					{
						ramServerpCfgParam[dsid][i].enable = 0;
						pRomeCfgParam->serverpCfgParam[dsid][i].enable=0;				
						printf("serverp_tableDriverAccess: rtl8651_addNaptServerPortRange(wanip:%0x,wanport:%u,localip:%0x,localport:%u) ret:%d\n",wanIp,pRomeCfgParam->serverpCfgParam[dsid][i].wanPortStart,tempIpAddr,pRomeCfgParam->serverpCfgParam[dsid][i].portStart,ret);
						return NOT_OK;
					}
				}

				if((pRomeCfgParam->serverpCfgParam[dsid][i].protocol == SERVERPCFGPARAM_PROTOCOL_UDP) || (pRomeCfgParam->serverpCfgParam[dsid][i].protocol == SERVERPCFGPARAM_PROTOCOL_BOTH)) {
					ret = rtl8651a_addNaptServerPortRange(
						dsid,
						0,
						(ipaddr_t) wanIp,               /* current wan ip */
						(uint16)   pRomeCfgParam->serverpCfgParam[dsid][i].wanPortStart,
						(ipaddr_t) tempIpAddr,
						(uint16)   pRomeCfgParam->serverpCfgParam[dsid][i].portStart, 
						(uint16) pRomeCfgParam->serverpCfgParam[dsid][i].wanPortFinish-pRomeCfgParam->serverpCfgParam[dsid][i].wanPortStart+1);		

					if (ret != SUCCESS)
					{
						ramServerpCfgParam[dsid][i].enable = 0;
						pRomeCfgParam->serverpCfgParam[dsid][i].enable=0;				
						printf("serverp_tableDriverAccess: rtl8651_addNaptServerPortRange(wanip:%0x,wanport:%u,localip:%0x,localport:%u) ret:%d\n",wanIp,pRomeCfgParam->serverpCfgParam[dsid][i].wanPortStart,tempIpAddr,pRomeCfgParam->serverpCfgParam[dsid][i].portStart,ret);
						return NOT_OK;
					}
				}
#endif
// Dino				
			}
			
		} /* end if */
	} /* end for */

        // Dino Chang 2005/03/07
        // move alg init to here
        if(alg_pptp == 0) {
                if(pRomeCfgParam->natCfgParam.pptpPassthru) {
                        pRomeCfgParam->algCfgParam[0].algList |= RTL8651_ALG_PPTP;
                } else {
                        pRomeCfgParam->algCfgParam[0].algList &= ~RTL8651_ALG_PPTP;
                }
                pRomeCfgParam->algCfgParam[0].localIp[RTL8651_ALG_PPTP_IDX] = 0;
                rtl8651a_setAlgStatus(0, pRomeCfgParam->algCfgParam[0].algList, pRomeCfgParam->algCfgParam[0].localIp);
        } 
	if(alg_ipsec == 0) {
                if(pRomeCfgParam->natCfgParam.ipsecPassthru) {
                        pRomeCfgParam->algCfgParam[0].algList |= RTL8651_ALG_IPSEC;
                } else {
                        pRomeCfgParam->algCfgParam[0].algList &= ~RTL8651_ALG_IPSEC;
                }
                pRomeCfgParam->algCfgParam[0].localIp[RTL8651_ALG_IPSEC_IDX] = 0;
                rtl8651a_setAlgStatus(0, pRomeCfgParam->algCfgParam[0].algList, pRomeCfgParam->algCfgParam[0].localIp);
        }
        // Dino

	return OK;
} /* end serverp_tableDriverAccess */

/*
 * Routine Name: serverp_init
 * Description: configure serverp cfg param entry
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 serverp_init(void)
{	


	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_SERVERP,(void*)pRomeCfgParam->serverpCfgParam, sizeof(serverpCfgParam_t)*MAX_SERVER_PORT*MAX_PPPOE_SESSION)!=0)
	{
		printf("serverp_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}		

	/* skip all delete when boot from scratch, since asic are all EMPTY */
	memcpy(ramServerpCfgParam,pRomeCfgParam->serverpCfgParam,sizeof(serverpCfgParam_t)*MAX_SERVER_PORT*MAX_PPPOE_SESSION);

	return 0;	
} /* end serverp_init */


/*
 * Routine Name: dmzCfgFactoryDefaultFunction
 * Description: dmz cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void dmzCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(struct dmzCfgParam_s)*MAX_PPPOE_SESSION;
	dmzCfgParam_t tmpTab[MAX_PPPOE_SESSION];
	
	memset((void*)tmpTab, 0, mySize);
	memcpy((void*)tmpTab, (void*)dmzCfgParamDefault, mySize);
	memcpy((void*)pBuf, (void*)tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end dmzCfgFactoryDefaultFunction */

/*
 * Routine Name: dmz_tableDriverAccess
 * Description: access table driver dmz APIs
 * Input:  char *ip , pointer to a ip address(char[4]).
 *   none
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 dmz_tableDriverAccess(int pppObjId)
{
	/* config DMZ host*/
	int32    ret;
	ipaddr_t wanIp=0;
	
		
	if (pRomeCfgParam->dmzCfgParam[pppObjId].enable==0)
		return OK; // do nothing and return success if not dmz host not enabled

	if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)
	{
		memcpy((void*)&wanIp,pRomeCfgParam->pppoeCfgParam[pppObjId].ipAddr,4);		
	}
	else
	{
		memcpy((void*)&wanIp,(void*)(pRomeCfgParam->ifCfgParam[0].ipAddr),4);
	}
	if(wanIp==0)
	{
		printf("dmz_tableDriverAccess: no valid wan ip!\n");
		return NOT_OK;
	}
	/* access table driver now */
	ret=rtl8651a_addDmzHost(pppObjId,(ipaddr_t)wanIp, (ipaddr_t)pRomeCfgParam->dmzCfgParam[pppObjId].dmzIp);
	if(ret)
	{
		printf("dmz_tableDriverAccess: rtl8651_addDmzHost() ret:%d\n",ret);
		return NOT_OK;
	}
	return OK;
} /* dmz_tableDriverAccess */



uint32 udp_tableDriverAccess(int pppObjId)
{
	/* config DMZ host*/
	int32    ret;
	uint32  len;

	if (pRomeCfgParam->udpblockCfgParam[pppObjId].enable)
		len=	pRomeCfgParam->udpblockCfgParam[pppObjId].size*1000;
	else
		len=0xffff;
		
	/* access table driver now */
	ret=rtl8651a_setUdpSizeThreshValue(pppObjId,len);
	if(ret)
	{
		printf(" ret:udp_tableDriverAccess() : rtl8651a_setUdpSizeThreshValue %d\n",ret);
		return NOT_OK;
	}
	return OK;
} /* udp_tableDriverAccess */

uint32 remoteMgmt_tableDriverAccess(void)
{
	uint32 ret;
	rtl8651_flushProtoStackActions();

	//rtl8651_addProtoStackActions(0,0,PS_ACTION_TO_PS,PS_ACTION_SERVER,IPPROTO_TCP,23,23);	
	rtl8651_addProtoStackActions(0,0,PS_ACTION_TO_PS,PS_ACTION_SERVER,IPPROTO_TCP,8888,8888);	
	//rtl8651_addProtoStackActions(0,0,PS_ACTION_TO_PS,PS_ACTION_CLIENT,IPPROTO_UDP,53,53);
	//disable L2TP,PPTP idle timeout
	rtl8651_addProtoStackActions(0,0,PS_ACTION_DONT_TIMEOUT,PS_ACTION_CLIENT,IPPROTO_UDP,1701,1701);
	rtl8651_addProtoStackActions(0,0,PS_ACTION_DONT_TIMEOUT,PS_ACTION_CLIENT,IPPROTO_TCP,1723,1723);	
	
#if 0	
	if(ret)
	{
		printf(" ret:remoteMgmt_tableDriverAccess() : rtl8651_addProtoStackActions %d\n",ret);
		return NOT_OK;
	}	
#endif	

	if(pRomeCfgParam->mgmtCfgParam[0].remoteMgmtEnable){	//savior 20041214
	ret=rtl8651_addProtoStackActions(pRomeCfgParam->mgmtCfgParam[0].remoteMgmtIp,
									pRomeCfgParam->mgmtCfgParam[0].remoteMgmtMask,
									PS_ACTION_TO_PS,PS_ACTION_SERVER,IPPROTO_TCP,
									pRomeCfgParam->mgmtCfgParam[0].remoteMgmtPort,80);
	}
	// Dino Chang 2005/01/02
	// to prevent WAN side can access web console
	else {
		ret=rtl8651_addProtoStackActions(0, 0, PS_ACTION_DROP, 0, IPPROTO_TCP, 80, 80);
	}
	// Dino

	if(ret)
	{
		printf(" ret:remoteMgmt_tableDriverAccess() : rtl8651_addProtoStackActions %d\n",ret);
		return NOT_OK;
	}	

	
	if(pRomeCfgParam->mgmtCfgParam[0].remoteIcmpEnable)
		ret=rtl8651_addProtoStackActions(0,0,PS_ACTION_TO_PS,0,IPPROTO_ICMP,0,0);
	else
		ret=rtl8651_addProtoStackActions(0,0,PS_ACTION_DROP,0,IPPROTO_ICMP,0,0);
	if(ret)
	{
		printf(" ret:remoteMgmt_tableDriverAccess() : rtl8651_addProtoStackActions %d\n",ret);
		return NOT_OK;
	}	
		
	return OK;
} /* remoteMgmt_tableDriverAccess */


/*
 * Routine Name: dmz_init
 * Description: configure dmz cfg param entry
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 dmz_init(void)
{	
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_DMZ,(void*)(pRomeCfgParam->dmzCfgParam), sizeof(struct dmzCfgParam_s)*MAX_PPPOE_SESSION)!=0)
	{
		printf("dmz_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}

	return 0;	
} /* end dmz_init */

/*
 * Routine Name: udpblockCfgFactoryDefaultFunction
 * Description: udpblock cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void udpblockCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(struct udpblockCfgParam_s)*MAX_PPPOE_SESSION;
	udpblockCfgParam_t tmpTab[MAX_PPPOE_SESSION];
	
	memset((void*)tmpTab, 0, mySize);
	memcpy((void*)tmpTab, (void*)udpblockCfgParamDefault, mySize);
	memcpy((void*)pBuf, (void*)tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end udpblockCfgFactoryDefaultFunction */

/*
 * Routine Name: udpblock_init
 * Description: configure udpblock cfg param entry
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 udpblock_init(void)
{	
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_UDPBLOCK,(void*)(pRomeCfgParam->udpblockCfgParam), sizeof(struct udpblockCfgParam_s)*MAX_PPPOE_SESSION)!=0)
	{
		printf("udpblock_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}

	return 0;	
} /* end udpblock_init */


/*
 * Routine Name: routingCfgFactoryDefaultFunction
 * Description: routing cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void routingCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(struct routingCfgParam_s)*MAX_ROUTING;
	routingCfgParam_t tmpTab[MAX_ROUTING];
	
	memset((void*)tmpTab, 0, mySize);
	memcpy((void*)tmpTab, (void*)routingCfgParamDefault, mySize);
	memcpy((void*)pBuf, (void*)tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end routingCfgFactoryDefaultFunction */

/*
 * Routine Name: routing_init
 * Description: configure routing cfg param entry
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 routing_init(void)
{	
	int i,ret;
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_ROUTING,(void*)(pRomeCfgParam->routingCfgParam), sizeof(struct routingCfgParam_s)*MAX_ROUTING)!=0)
	{
		printf("routing_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}

	for(i=0;i<MAX_ROUTING;i++)
	{
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
	return 0;	
} /* end routing_init */


/*
 * Routine Name: logCfgFactoryDefaultFunction
 * Description: log cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void logCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(struct logCfgParam_s)*MAX_PPPOE_SESSION;
	logCfgParam_t tmpTab[MAX_PPPOE_SESSION];
	
	memset((void*)tmpTab, 0, mySize);
	memcpy((void*)tmpTab, (void*)logCfgParamDefault, mySize);
	memcpy((void*)pBuf, (void*)tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end logCfgFactoryDefaultFunction */

/*
 * Routine Name: log_init
 * Description: configure log cfg param entry
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 log_init(void)
{	
	int i;
	
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_LOG,(void*)(pRomeCfgParam->logCfgParam), sizeof(struct logCfgParam_s)*MAX_PPPOE_SESSION)!=0)
	{
		printf("log_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}

	system("touch /var/syslog");

	if(pRomeCfgParam->logCfgParam[0].module&RTL8651_LOGMODULE_SYS)
	{
		rtl8651a_enableLogging(0,RTL8651_LOGMODULE_SYS,1);
		system("/bin/syslogd &");
		system("/bin/klogd &");		
	}	
	
	for(i=0;i<MAX_PPPOE_SESSION;i++)
	{
		if(pRomeCfgParam->logCfgParam[i].module&RTL8651_LOGMODULE_DOS)
			rtl8651a_enableLogging(i,RTL8651_LOGMODULE_DOS,1);
		if(pRomeCfgParam->logCfgParam[i].module&RTL8651_LOGMODULE_ACL)
			rtl8651a_enableLogging(i,RTL8651_LOGMODULE_ACL, 1);
		if(pRomeCfgParam->logCfgParam[i].module&RTL8651_LOGMODULE_URL)
			rtl8651a_enableLogging(i,RTL8651_LOGMODULE_URL, 1);
		if(pRomeCfgParam->logCfgParam[i].module&RTL8651_LOGMODULE_NEWFLOW)
			rtl8651a_enableLogging(i,RTL8651_LOGMODULE_NEWFLOW, 1);
	}

	for(i=0;i<MAX_PPPOE_SESSION;i++)
	{
		if(pRomeCfgParam->logCfgParam[i].module&RTL8651_LOGMODULE_DOS)
			rtl8651a_enableLogging(i,RTL8651_LOGMODULE_DOS,1);
		if(pRomeCfgParam->logCfgParam[i].module&RTL8651_LOGMODULE_ACL)
			rtl8651a_enableLogging(i,RTL8651_LOGMODULE_ACL, 1);
		if(pRomeCfgParam->logCfgParam[i].module&RTL8651_LOGMODULE_URL)
			rtl8651a_enableLogging(i,RTL8651_LOGMODULE_URL, 1);
		if(pRomeCfgParam->logCfgParam[i].module&RTL8651_LOGMODULE_NEWFLOW)
			rtl8651a_enableLogging(i,RTL8651_LOGMODULE_NEWFLOW, 1);
	}

	return 0;	
} /* end log_init */

/*
 * Routine Name: upnpCfgFactoryDefaultFunction
 * Description: upnp cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void upnpCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(struct upnpCfgParam_s);
	upnpCfgParam_t tmpTab;
	
	memset((void*)&tmpTab, 0, mySize);
	memcpy((void*)&tmpTab, (void*)&upnpCfgParamDefault, mySize);
	memcpy((void*)pBuf, (void*)&tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end upnpCfgFactoryDefaultFunction */

#ifdef __uClinux__
#define USERAPP_NOMMU
#endif


#if 0 //for PSEUDO_ICSD
#ifdef USERAPP_NOMMU
uint32 upnp_start(void)
{
	if(pRomeCfgParam->upnpCfgParam.enable == 1)
	{
		// start upnp service
		char temp[2][30] = {0};
		char *argv[6] = {0};
		char interface[10] = {0};
		int pid = 0;
		FILE *f = NULL;
		FILE *f_arg = NULL;

		// Route related setting		
		argv[0]="-net";
		argv[1]="239.0.0.0";
		argv[2]="netmask";
		argv[3]="255.0.0.0";
		argv[4]="eth1";
		argv[5]=NULL;

		// upnp daemon related setting

		/*
			Current argument for UPnP daemon
		*/
		switch(pRomeCfgParam->ifCfgParam[0].connType)
		{
			case IFCFGPARAM_CONNTYPE_STATIC:
			case IFCFGPARAM_CONNTYPE_DHCPC:
				sprintf(interface,"eth0");
				break;
			case IFCFGPARAM_CONNTYPE_PPPOE:
			case IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED:
				sprintf(interface,"ppp0");
				break;
			case IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE:					
				sprintf(interface,"ppp%d",pRomeCfgParam->pppoeCfgParam[pRomeCfgParam->pppoeCfgParam[0].whichPppObjId].pppx);
				break;
		}
		sprintf(&temp[0][0], "eth1 %s", interface);

		f_arg = fopen("/var/run/pseudoicsd.arg", "r");
		if (f_arg)
		{
			char ptr[20];
			char* cp;

			bzero(ptr, sizeof(ptr));
			cp = &temp[1][0];
			while (fscanf(f_arg, "%s", ptr) != EOF)
			{
				if (cp == &temp[1][0])
				{	// first argument
					cp += sprintf(cp, "%s", ptr);
				}else
				{
					cp += sprintf(cp, " %s", ptr);
				}
			}

			if (strncmp(&temp[0][0], &temp[1][0], strlen(&temp[0][0])) == 0)
			{	// we don't change configuration file
				fclose(f_arg);
				goto start_upnp;
			}
			// configuration change: modify configuration file
			fclose(f_arg);
		}
		// re-open argument file to write
		f_arg = fopen("/var/run/pseudoicsd.arg", "w");
		if (!f_arg)
		{
			printf("Error: cannot set argument of uPnP daemon!\n");
			goto errout;
		}
		fprintf(f_arg, "%s\n", &temp[0][0]);
		fclose(f_arg);

start_upnp:

		f = fopen("/var/run/pseudoicsd.pid", "r");
		if (f)
		{
			fscanf(f, "%d", &pid);
			if (kill(pid, SIGHUP))
			{	// file not exist
				fclose(f);
				unlink("/var/run/pseudoicsd.pid");
				goto create_upnp;
			}
			fclose(f);	// reset successful: return
			goto out;
		}
create_upnp:
		{
			char command[20] = {0};
			sprintf(command, "/bin/upnp &");
			system(command);
		}

out:
		// create route for upnp
		INET_setroute(RTACTION_ADD, 0, argv);
		// set multicast filter to upnp
		rtl8651_multicastAddFilter(ntohl(inet_addr("239.255.255.250")), MCAST_FILTER_ACT_MIRROR);
		return 0;
errout:
		// remove route for upnp
		INET_setroute(RTACTION_DEL, 0, argv);
		rtl8651_multicastDelFilter(ntohl(inet_addr("239.255.255.250")), MCAST_FILTER_ACT_MIRROR);
		return 1;
	}
	return 1;
}
#else /* USERAPP_NOMMU */
uint32 upnp_start(void)
{
	if(pRomeCfgParam->upnpCfgParam.enable==1)
	{
		// start upnp service
		char temp[30];
		char *argv[6];
		char interface[10]={0};
		int pid=0;
		FILE *f;
		
		argv[0]="-net";
		argv[1]="239.0.0.0";
		argv[2]="netmask";
		argv[3]="255.0.0.0";
		argv[4]="eth1";
		argv[5]=NULL;
		
		//kill old process		
		f=fopen("/var/run/pseudoicsd.pid","r");
		if(f!=NULL)
		{
			// del upnp route
			INET_setroute(RTACTION_DEL, 0, argv);			
			fscanf(f,"%d",&pid);
			kill(pid,SIGTERM);
			remove("/var/run/pseudoicsd.pid");
			fclose(f);
		}
		
		switch(pRomeCfgParam->ifCfgParam[0].connType)
		{
			case IFCFGPARAM_CONNTYPE_STATIC:
			case IFCFGPARAM_CONNTYPE_DHCPC:
				sprintf(interface,"eth0");
				break;
			case IFCFGPARAM_CONNTYPE_PPPOE:
			case IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED:
				sprintf(interface,"ppp0");
				break;
			case IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE:					
				sprintf(interface,"ppp%d",pRomeCfgParam->pppoeCfgParam[pRomeCfgParam->pppoeCfgParam[0].whichPppObjId].pppx);
				break;
		}
				
		INET_setroute(RTACTION_ADD, 0, argv);
		rtl8651_multicastAddFilter(ntohl(inet_addr("239.255.255.250")), MCAST_FILTER_ACT_MIRROR);
		sprintf(temp,"/bin/upnp eth1 %s &",interface);		
		system(temp);
	}
	else
	{
		rtl8651_multicastDelFilter(ntohl(inet_addr("239.255.255.250")), MCAST_FILTER_ACT_MIRROR);
		return 1;
	}
	return 0;		
}
void upnp_stop(void)
{
		char *argv[6];
		argv[0]="-net";
		argv[1]="239.0.0.0";
		argv[2]="netmask";
		argv[3]="255.0.0.0";
		argv[4]="eth1";
		argv[5]=NULL;
		
		//kill old process		
		send_signal("/var/run/pseudoicsd.pid",SIGTERM);
		unlink("/var/run/pseudoicsd.pid");
		// del upnp route
		INET_setroute(RTACTION_DEL, 0, argv);		
}


#endif /* USERAPP_NOMMU */

#else // for LINUX-IGD
void upnp_stop(void)
{
		char *argv[6];
		argv[0]="-net";
		argv[1]="239.0.0.0";
		argv[2]="netmask";
		argv[3]="255.0.0.0";
		argv[4]="eth1";
		argv[5]=NULL;
		
		//kill old process		
		send_signal("/var/run/linuxigd.pid",SIGTERM);
		unlink("/var/run/linuxigd.pid");
		// del upnp route
		INET_setroute(RTACTION_DEL, 0, argv);		
		sleep(1);
}

uint32 upnp_start(void)
{
	if(pRomeCfgParam->upnpCfgParam.enable==1)
	{
		//close old upnp daemon.
		upnp_stop();
	
		// start upnp service
		char temp[30];
		char *argv[6];
		char interface[10]={0};
		int pid=0;
		FILE *f, *f2;
		
		argv[0]="-net";
		argv[1]="239.0.0.0";
		argv[2]="netmask";
		argv[3]="255.0.0.0";
		argv[4]="eth1";
		argv[5]=NULL;

                f=fopen("/var/gatedesc.xml","w");
                f2=fopen("/etc/linuxigd/gatedesc.xml.org","r");
                if(f2!=NULL)
                {
                        char tempStr[160]={0};

                        while(1)
                        {
                                fgets(tempStr,160,f2);
                                if(feof(f2)) break;
                                if(strstr(tempStr,"<url>")!=NULL)
                                        fprintf(f,"<url>http://%d.%d.%d.%d/ligd.gif</url>\n"
                                        ,pRomeCfgParam->ifCfgParam[1].ipAddr[0]
                                        ,pRomeCfgParam->ifCfgParam[1].ipAddr[1]
                                        ,pRomeCfgParam->ifCfgParam[1].ipAddr[2]
                                        ,pRomeCfgParam->ifCfgParam[1].ipAddr[3]);
                                else if(strstr(tempStr, "<presentationURL>") != NULL)
                                        fprintf(f,"<presentationURL>http://%d.%d.%d.%d:80</presentationURL>\n"
                                        ,pRomeCfgParam->ifCfgParam[1].ipAddr[0]
                                        ,pRomeCfgParam->ifCfgParam[1].ipAddr[1]
                                        ,pRomeCfgParam->ifCfgParam[1].ipAddr[2]
                                        ,pRomeCfgParam->ifCfgParam[1].ipAddr[3]);
				else
                                        fputs(tempStr,f);
                        }
                        fclose(f2);
                }
                fclose(f);
	
		switch(pRomeCfgParam->ifCfgParam[0].connType)
		{
			case IFCFGPARAM_CONNTYPE_STATIC:
			case IFCFGPARAM_CONNTYPE_DHCPC:
				sprintf(interface,"eth0");
				break;
			case IFCFGPARAM_CONNTYPE_PPPOE:
			case IFCFGPARAM_CONNTYPE_PPTP:
			case IFCFGPARAM_CONNTYPE_L2TP:				
			case IFCFGPARAM_CONNTYPE_DHCPL2TP:								
			case IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED:
			case IFCFGPARAM_CONNTYPE_3G:
				sprintf(interface,"ppp0");
				break;
			case IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE:					
				sprintf(interface,"ppp%d",pRomeCfgParam->pppoeCfgParam[pRomeCfgParam->pppoeCfgParam[0].whichPppObjId].pppx);
				break;
		}
				
		INET_setroute(RTACTION_ADD, 0, argv);
		rtl8651_multicastAddFilter(ntohl(inet_addr("239.255.255.250")), MCAST_FILTER_ACT_MIRROR);
		sprintf(temp,"/bin/upnpd %s eth1 &",interface);		
		system(temp);
	}
	else
	{
		rtl8651_multicastDelFilter(ntohl(inet_addr("239.255.255.250")), MCAST_FILTER_ACT_MIRROR);
		return 1;
	}
	return 0;		
}
#endif

void udhcpc_start(void)
{
	send_signal("/var/run/udhcpc.pid",9);
	do_exec("/bin/udhcpc",NULL);
}

void udhcpc_renew(void)
{
	FILE *f;

	f = fopen("/var/run/udhcpc.pid", "r");
	if (f)
		send_signal("/var/run/udhcpc.pid", SIGUSR1);
	else
		udhcpc_start();
}

/*
 * Routine Name: upnp_init
 * Description: configure upnp cfg param entry
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 upnp_init(void)
{	
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_UPNP,(void*)&(pRomeCfgParam->upnpCfgParam), sizeof(struct upnpCfgParam_s))!=0)
	{
		printf("upnp_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}

	return 0;	
} /* end upnp_init */

/*
 * Routine Name: urlfilterCfgFactoryDefaultFunction
 * Description: urlfilter cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void urlfilterCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(struct urlfilterCfgParam_s)*MAX_URL_FILTER*MAX_PPPOE_SESSION;
	urlfilterCfgParam_t tmpTab[MAX_PPPOE_SESSION][MAX_URL_FILTER];
	
	memset((void*)tmpTab, 0, mySize);
	memcpy((void*)tmpTab, (void*)urlfilterCfgParamDefault, mySize);
	memcpy((void*)pBuf, (void*)tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end urlfilterCfgFactoryDefaultFunction */

/* Mac filter Default Function -- Min Lee */
void macfilterCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(struct macfilterCfgParam_s)*MAX_URL_FILTER*MAX_PPPOE_SESSION;
	macfilterCfgParam_t tmpTab[MAX_PPPOE_SESSION][MAX_URL_FILTER];
	
	memset((void*)tmpTab, 0, mySize);
	memcpy((void*)tmpTab, (void*)macfilterCfgParamDefault, mySize);
	memcpy((void*)pBuf, (void*)tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end macfilterCfgFactoryDefaultFunction */

/* Tmss Default Function -- Min Lee */
void tmssCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(struct tmssCfgParam_s);
	tmssCfgParam_t tmpTab;
	
	memset((void*)&tmpTab, 0, mySize);
	memcpy((void*)&tmpTab, (void*)&tmssCfgParamDefault, mySize);
	memcpy((void*)pBuf, (void*)&tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end macfilterCfgFactoryDefaultFunction */

// Dino Chang 2004/12/30
void tmssCInfoCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
        uint32 mySize = sizeof(struct tmssCInfoCfgParam_s)*PC_USER_LEN_S;
        tmssCInfoCfgParam_t tmpTab[PC_USER_LEN_S];
                                                                                        memset((void*)tmpTab, 0, mySize);
        memcpy((void*)tmpTab, (void*)tmssCInfoCfgParamDefault, mySize);
        memcpy((void*)pBuf, (void*)tmpTab, mySize);                             
        /* patch for avoiding word-aligned load/store */
        /* *pBufSize = mySize; */
        memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));

        return;
} /* end tmssCInfoCfgFactoryDefaultFunction */
// Dino

/*
 * Routine Name: urlfilter_init
 * Description: configure urlfilter cfg param entry
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 urlfilter_init(void)
{		
	int i;
	unsigned char mac_mask[6] = {255,255,255,255,255,255};
	
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_URLFILTER,(void*)(pRomeCfgParam->urlfilterCfgParam), sizeof(struct urlfilterCfgParam_s)*MAX_PPPOE_SESSION*MAX_URL_FILTER)!=0)
	{
		printf("urlfilter_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}
	
	for(i=0;i<MAX_URL_FILTER;i++)
	{
		if(pRomeCfgParam->urlfilterCfgParam[0][i].enable==1)
		{
			rtl8651_addURLFilterRule(0,pRomeCfgParam->urlfilterCfgParam[0][i].string, strlen(pRomeCfgParam->urlfilterCfgParam[0][i].string), pRomeCfgParam->urlfilterCfgParam[0][i].ip_start, pRomeCfgParam->urlfilterCfgParam[0][i].ip_end);
		}
	}
	
	/* read Mac Filter cfg from cfgmgr -- Min Lee*/
	if (cfgmgr_read(CFGMGR_TABID_MACFILTER,(void*)(pRomeCfgParam->macfilterCfgParam), sizeof(struct macfilterCfgParam_s)*MAX_PPPOE_SESSION*MAX_URL_FILTER)!=0)
	{
		printf("macfilter_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}
	
	if (pRomeCfgParam->natCfgParam.mac_filter){
		rtl8651_flushMacAccessControl();
		rtl8651_enableMacAccessControl(pRomeCfgParam->natCfgParam.mac_filter);

		// Dino Chang 2005/01/10
		if(pRomeCfgParam->macfilterCfgParam[0][0].allow)
			rtl8651_setMacAccessControlDefaultPermit(0); // set default to deny
		else
			rtl8651_setMacAccessControlDefaultPermit(1); // set default to allow
		// Dino
		
		for(i = 0; i < MAX_URL_FILTER; i++){			
			// origional via SourceMACControl API
			rtl8651_addMacAccessControl(pRomeCfgParam->macfilterCfgParam[0][i].mac, mac_mask, pRomeCfgParam->macfilterCfgParam[0][i].allow);	
			
			// Special process for MAC filter via IP_ACL.
			if (pRomeCfgParam->natCfgParam.mac_filter)
			{
				rtl8651_tblDrvAclRule_t samplelRule;
				int8 ifLanStr[20] = {0};
				int32 ret;
				uint8 macNull[6]={0,0,0,0,0,0};
				uint8 macFFFF[6]={0xff,0xff,0xff,0xff,0xff,0xff};
				
				sprintf(ifLanStr,"eth1");
				samplelRule.matchType_ = RTL8651_ACL_GENERIC;        //<- 最新版本 Rome Driver 要加這個，所以我加了讓他過
				samplelRule.ruleType_ = RTL8651_ACL_MAC;
				samplelRule.actionType_ = RTL8651_ACL_DROP;
				
				memcpy(&samplelRule.srcMac_, pRomeCfgParam->macfilterCfgParam[0][i].mac,6); // don’t care 
				memcpy(&samplelRule.srcMacMask_, macFFFF,6);  
				
				memcpy(&samplelRule.dstMac_, macNull,6);
				memcpy(&samplelRule.dstMacMask_, macNull,6);
				samplelRule.typeLen_ = 0; 
				samplelRule.typeLenMask_ = 0;  
				
				printf("if = %s, mac = %x\n", ifLanStr, (pRomeCfgParam->macfilterCfgParam[0][i].mac[5]));
							        // ingress rule 
				ret = rtl8651a_addAclRule(0, ifLanStr, 1 ,(rtl8651_tblDrvAclRule_t *)&samplelRule);
				printf("ret = %d\n", ret);
				
				if (ret!=SUCCESS)
					printf("rtl8651a_addAclRule() ret:%d\n",ret);
			}
		}
	}
	
	return 0;	
} /* end urlfilter_init */



/*
 * Routine Name: timeCfgFactoryDefaultFunction
 * Description: time cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void timeCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(struct timeCfgParam_s);
	timeCfgParam_t tmpTab;
	
	memset((void*)&tmpTab, 0, mySize);
	memcpy((void*)&tmpTab, (void*)&timeCfgParamDefault, mySize);
	memcpy((void*)pBuf, (void*)&tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end timeCfgFactoryDefaultFunction */

uint32 zone_init(void)
{
	char tmp[16];
	FILE *f;
	int32 timezone_diff;
	sprintf(tmp,"std%s",pRomeCfgParam->timeCfgParam.timeZone);
	// Dino Chang 2005/03/07
	if(tmp[3] == '+')
		memcpy(&tmp[3], "-", 1);
	else if(tmp[3] == '-')
		memcpy(&tmp[3], "+", 1);
	// Dino
	setenv("TZ",tmp, 1);
	timezone_diff=(tmp[4]-'0')*36000+(tmp[5]-'0')*3600+(tmp[7]-'0')*600+(tmp[8]-'0')*60;
	if(tmp[3]=='+') timezone_diff*=-1;
	rtl8651_setTimeZone(timezone_diff);
	
	f=fopen("/var/TZ","w");
	fprintf(f,"%s\n",tmp);
	fclose(f);
}

uint32 time_start(void)
{
	char tempStr[56];

	// Dino Chang 2005/01/07
	send_signal("/var/run/sched.pid", SIGUSR1); // tell schedule daemon to start check	
	// Dino
	if(pRomeCfgParam->timeCfgParam.time_type == 0) {
	sprintf(tempStr,"/bin/rdate -s %s &",pRomeCfgParam->timeCfgParam.ntpServer1);
	system(tempStr);
	sprintf(tempStr,"/bin/rdate -s %s &",pRomeCfgParam->timeCfgParam.ntpServer2);
	system(tempStr);
	sprintf(tempStr,"/bin/rdate -s %s &",pRomeCfgParam->timeCfgParam.ntpServer3);
	system(tempStr);
	} else {
		sprintf(tempStr, "date %02d%02d%02d%02d%d",
			pRomeCfgParam->timeCfgParam.months,
			pRomeCfgParam->timeCfgParam.days,
			pRomeCfgParam->timeCfgParam.hours,
			pRomeCfgParam->timeCfgParam.minutes,
			pRomeCfgParam->timeCfgParam.years);
			system(tempStr);
			
		printf("date %02d%02d%02d%02d%d",//test savior 20041216
			pRomeCfgParam->timeCfgParam.months,
			pRomeCfgParam->timeCfgParam.days,
			pRomeCfgParam->timeCfgParam.hours,
			pRomeCfgParam->timeCfgParam.minutes,
			pRomeCfgParam->timeCfgParam.years);

		// Dino Chang 2005/01/08
		send_signal("/var/run/sched.pid", SIGHUP); // reset all rule
		// Dino
	}
}

/*
 * Routine Name: time_init
 * Description: configure time cfg param entry
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 time_init(void)
{		
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_TIME,(void*)&pRomeCfgParam->timeCfgParam, sizeof(struct timeCfgParam_s))!=0)
	{
		printf("time_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}
	return 0;	
} /* end time_init */

/*
 * Routine Name: naptCfgFactoryDefaultFunction
 * Description: napt cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void naptCfgFactoryDefaultFunction(void *pBuf, uint32 * pbufSize)
{
	uint32 mySize = sizeof(struct naptCfgParam_s);
	naptCfgParam_t tmpTab;
	
	memset((void*)&tmpTab, 0, mySize);
	memcpy((void*)&tmpTab, (void*)&naptCfgParamDefault, mySize);
	memcpy((void*)pBuf, (void*)&tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pbufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end naptCfgFactoryDefaultFunction */

/*
 * Routine Name: napt_init
 * Description: configure napt cfg param entry
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 napt_init(void)
{
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_NAPT, (void*)(&pRomeCfgParam->naptCfgParam), sizeof(struct naptCfgParam_s)) != 0)
	{
		printf("napt_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}
	/* initial parameters */
	rtl8651_WeakNaptCtrl(pRomeCfgParam->naptCfgParam.weakNaptEnable);
	rtl8651_looseUdpCtrl(pRomeCfgParam->naptCfgParam.looseUdpEnable);
	rtl8651_TcpNaptStaticPortMappingCtrl(pRomeCfgParam->naptCfgParam.tcpStaticPortMappingEnable);
	rtl8651_UdpNaptStaticPortMappingCtrl(pRomeCfgParam->naptCfgParam.udpStaticPortMappingEnable);

	return 0;
}	/* end napt_init */


/*
 * Routine Name: dosCfgFactoryDefaultFunction
 * Description: dos cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void dosCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(struct dosCfgParam_s)*MAX_PPPOE_SESSION;
	dosCfgParam_t tmpTab[MAX_PPPOE_SESSION];
	
	memset((void*)tmpTab, 0, mySize);
	memcpy((void*)tmpTab, (void*)dosCfgParamDefault, mySize);
	memcpy((void*)pBuf, (void*)tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end dosCfgFactoryDefaultFunction */


/*
 * Routine Name: dos_init
 * Description: configure dos cfg param entry
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 dos_init(void)
{
	uint32 i;
	uint32 pScanThreshold;
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_DOS,(void*)(pRomeCfgParam->dosCfgParam), sizeof(struct dosCfgParam_s)*MAX_PPPOE_SESSION)!=0)
	{
		printf("dos_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}
	for(i=0;i<MAX_PPPOE_SESSION;i++){
		rtl8651a_setDosThreshold(i,RTL8651_DOS_SYN_THRESHOLD,pRomeCfgParam->dosCfgParam[i].syn);
		rtl8651a_setDosThreshold(i,RTL8651_DOS_FIN_THRESHOLD,pRomeCfgParam->dosCfgParam[i].fin);
		rtl8651a_setDosThreshold(i,RTL8651_DOS_UDP_THRESHOLD,pRomeCfgParam->dosCfgParam[i].udp);
		rtl8651a_setDosThreshold(i,RTL8651_DOS_ICMP_THRESHOLD,pRomeCfgParam->dosCfgParam[i].icmp);

		rtl8651a_setPerSrcDosThreshold(i, RTL8651_DOS_SYN_THRESHOLD, pRomeCfgParam->dosCfgParam[i].perSrc_syn);
		rtl8651a_setPerSrcDosThreshold(i, RTL8651_DOS_FIN_THRESHOLD, pRomeCfgParam->dosCfgParam[i].perSrc_fin);
		rtl8651a_setPerSrcDosThreshold(i, RTL8651_DOS_UDP_THRESHOLD, pRomeCfgParam->dosCfgParam[i].perSrc_udp);
		rtl8651a_setPerSrcDosThreshold(i, RTL8651_DOS_ICMP_THRESHOLD, pRomeCfgParam->dosCfgParam[i].perSrc_icmp);

		switch (pRomeCfgParam->dosCfgParam[i].portScan_level)
		{
			case 0:	/* Low */
				pScanThreshold = DOS_PORTSCAN_LOW_VAL;
				break;
			case 1:	/* high */
				pScanThreshold = DOS_PORTSCAN_HIGH_VAL;
				break;
			default:
				pScanThreshold = DOS_PORTSCAN_LOW_VAL;
		}
		rtl8651_dosScanTrackingSetPara(i, RTL8651_DOS_SCAN_MOINTOR_WIN_DEFAULT, pScanThreshold);

		if (pRomeCfgParam->dosCfgParam[i].ignoreLanSideCheck)
		{
			rtl8651_addDosIgnoreType(RTL8651_DOS_IGNORETYPE_FROM_LAN);
		}else
		{
			rtl8651_delDosIgnoreType(RTL8651_DOS_IGNORETYPE_FROM_LAN);
		}

		rtl8651_dosProc_blockSip_setPrisonTime(pRomeCfgParam->dosCfgParam[i].sipblock_PrisonTime);
		rtl8651_dosProc_blockSip_enable(i, pRomeCfgParam->dosCfgParam[i].sipblock_enable);
	}

	return 0;	
} /* end dos_init */


/*
 * Routine Name: algCfgFactoryDefaultFunction
 * Description: alg cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void algCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(struct algCfgParam_s)*MAX_PPPOE_SESSION;
	algCfgParam_t tmpTab[MAX_PPPOE_SESSION];
	
	memset((void*)tmpTab, 0, mySize);
	memcpy((void*)tmpTab, (void*)algCfgParamDefault, mySize);
	memcpy((void*)pBuf, (void*)tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end algCfgFactoryDefaultFunction */


/*
 * Routine Name: alg_init
 * Description: configure alg cfg param entry
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 alg_init(void)
{		
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_ALG,(void*)(pRomeCfgParam->algCfgParam), sizeof(struct algCfgParam_s)*MAX_PPPOE_SESSION)!=0)
	{
		printf("alg_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}
	return 0;	
} /* end alg_init */


/*
 * Routine Name: wlanCfgFactoryDefaultFunction
 * Description: wlan cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void wlanCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(struct wlanCfgParam_s)*MAX_WLAN_CARDS;
	wlanCfgParam_t tmpTab[MAX_WLAN_CARDS];
	
	memset((void*)tmpTab, 0, mySize);
	memcpy((void*)tmpTab, (void*)wlanCfgParamDefault, mySize);
	memcpy((void*)pBuf, (void*)tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	return;
} /* end wlanCfgFactoryDefaultFunction */

int32 wds_up(int w){
	int32 i;
	char cmdBuf[64];
	if(pRomeCfgParam->wlanCfgParam[w].enable){
		//Bring up WDS interfaces.
		printf("total %d WDS links\n",pRomeCfgParam->wlanCfgParam[w].wdsEnable);
		for(i=0;i<(pRomeCfgParam->wlanCfgParam[w].wdsEnable);i++){
			sprintf(cmdBuf, "ifconfig wlan%dwds%d up", w,i);
			printf("%s\n", cmdBuf);system(cmdBuf);
		}
	}
}

int setessid(char *ssid)
{
	int s;
	struct iwreq	wrq;
	char		essid[IW_ESSID_MAX_SIZE + 1];

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0){
		printf("Error on open socket!!!\n");
		return -1;
	}

	strncpy(wrq.ifr_name, "ath0", IF_NAMESIZE);
	wrq.u.essid.flags = 1;
	strcpy(essid, ssid);
	wrq.u.essid.pointer = (caddr_t) essid;
	wrq.u.essid.length = strlen(essid) + 1;
	if (ioctl(s, SIOCSIWESSID, &wrq) < 0){
                printf("Error on set ESSID!!!\n");
                close(s);
                return -1;
	}
	close(s);
	return 0;
}

// Dino Chang 2005/04/01
int setKey(int wep_type, int authType, char *wepkey, int index, int keylen)
//int setKey(int wep_type, int authType, char *wepkey, int index)
// Dino
{
	// Dino Chang 2005/04/01
	int s;
	//int s, keylen;
	// Dino
	struct iwreq	wrq;
	unsigned char	key[IW_ENCODING_TOKEN_MAX];

        s = socket(AF_INET, SOCK_DGRAM, 0);
        if (s < 0){
                printf("Error on open socket!!!\n");
                return -1;
        }

	// Dino Chang 2005/04/01
	memset(&key, '\0', IW_ENCODING_TOKEN_MAX);
	// Dino

	strncpy(wrq.ifr_name, "ath0", IF_NAMESIZE);
	wrq.u.data.pointer = (caddr_t) NULL;
	wrq.u.data.flags = 0;
	wrq.u.data.length = 0;

	if (wep_type){
// Dino Chang 2005/04/01
#if 0
		keylen = strlen(wepkey);
		if (keylen > IW_ENCODING_TOKEN_MAX)
		  keylen = IW_ENCODING_TOKEN_MAX;
		strncpy(key, wepkey, keylen);
#endif
		memcpy(key, wepkey, keylen);
// Dino

		if (keylen > 0){
		  wrq.u.data.length = keylen;
		  wrq.u.data.pointer = (caddr_t) key;
		}

		wrq.u.encoding.flags |= index;
	}else
		wrq.u.data.flags |= IW_ENCODE_DISABLED;

	if (authType == 0)
		wrq.u.data.flags |= IW_ENCODE_OPEN;
	else if (authType == 1)
		wrq.u.data.flags |= IW_ENCODE_RESTRICTED;

	if(ioctl(s, SIOCSIWENCODE, &wrq) < 0){
                printf("Error on set KEY!!!\n");
                close(s);
                return -1;
	}
	close(s);
	return 0;
}

int32 wlan_setup(int w){
	uint8 wepkeyid=1;
	char temp[6]={0};	
	char mask[6]={0xff,0xff,0xff,0xff,0xff,0xff};	
	char cmdBuf[64]={0};
	int32 key_idx, setkey_idx;
	int i=0,value;
	
	int32 cryptValue[]={0/*disabled*/,1/*wep64*/,5/*wep128*/,2/*tkip*/,4/*AES*/};
	int cckTxPower[]={0,3,7,9,11};
	int ofdmTxPower[]={0,3,6,9,17};//,17};
	uint16 opRateSet[13]={0/*Auto*/,4095,2047,1023,511,255,127,63,31,15,7,3,1};
	int16 authType[5/*encryption selection*/][3/*auth selection*/]={
		/*No encrypt*/{AUTH_NONE,AUTH_8021x,0},
		/*WEP64*/{AUTH_NONE,AUTH_KEY,AUTH_8021x},
		/*WEP128*/{AUTH_NONE,AUTH_KEY,AUTH_8021x},
		/*WPA-TKIP*/{AUTH_NONE,AUTH_8021x,0},
		/*WPA-AES*/{AUTH_NONE,AUTH_8021x,0}

	};
	uint16 band[4]={0 /*a+b+g*/,3/*g only*/,1/*a only */,2/*b only*/};	
	unsigned char   key[IW_ENCODING_TOKEN_MAX];
	
	if (cfgmgr_read(CFGMGR_TABID_WLAN,(void*)pRomeCfgParam->wlanCfgParam, sizeof(struct wlanCfgParam_s)*MAX_WLAN_CARDS)!=0)
	{
		printf("wlan_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return -1;
	}
	
#if 1 /* Ed Lai 05/10/30 , move for Channel list */
	printf("******* wlanconfig *******\n");
	sprintf(cmdBuf, "wlanconfig ath%d create wlandev wifi%d wlanmode ap",w,w); 
	printf("%s\n", cmdBuf);system(cmdBuf);
#endif

	// TESTING new PCB board
	if(pRomeCfgParam->wlanCfgParam[w].enable == 0)
	{
		printf("wlan_init: wlan%d not enabled\n",w);
		
		// Dim WLAN LED
		system("gpio 1 1 9 0");
		return -1; /* Ed Lai, stopping setup */
	}	

	printf("Setup WLAN device %d...\n",w);
       
	// Enable WLAN LED blinking
	system("gpio 2 1 9 7");
	
	// no need
	//sprintf(cmdBuf, "iwconfig ath%d txpower auto", w);  //enable power control
	//printf("%s\n", cmdBuf);system(cmdBuf);
	
	// should be check later, currently left it as default
	//sprintf(cmdBuf, "iwconfig ath%d txpower %d", w, pRomeCfgParam->wlanCfgParam[w].txPower);
	//printf("%s\n", cmdBuf);system(cmdBuf);

	setessid(pRomeCfgParam->wlanCfgParam[w].ssid);

	if(pRomeCfgParam->wlanCfgParam[w].beacon != 100) // default value = 100
	{
    		sprintf(cmdBuf, "iwpriv ath%d bintval %d", w,pRomeCfgParam->wlanCfgParam[w].beacon);
        	printf("%s\n", cmdBuf);system(cmdBuf);
	}
	
	if(pRomeCfgParam->wlanCfgParam[w].dtim != 3) // default value = 3
	{
    		sprintf(cmdBuf, "iwpriv ath%d dtim_period %d", w,pRomeCfgParam->wlanCfgParam[w].dtim);
        	printf("%s\n", cmdBuf);system(cmdBuf);
	}
	
    	sprintf(cmdBuf, "iwpriv ath%d hide_ssid %d", w,pRomeCfgParam->wlanCfgParam[w].broadcastSSID);
    	printf("%s\n", cmdBuf);system(cmdBuf);

	if(pRomeCfgParam->wlanCfgParam[w].frag != 2346) // default = 2346
	{
		sprintf(cmdBuf, "iwconfig ath%d frag %d", w,pRomeCfgParam->wlanCfgParam[w].frag);
		printf("%s\n", cmdBuf);system(cmdBuf);
	}
	
	if(pRomeCfgParam->wlanCfgParam[w].rts != 2346) // default value = 2346
	{
		// no need
		//sprintf(cmdBuf, "iwconfig ath%d rts fixed", w);
		//printf("%s\n", cmdBuf);system(cmdBuf);
		
		sprintf(cmdBuf, "iwconfig ath%d rts %d", w,pRomeCfgParam->wlanCfgParam[w].rts);
		printf("%s\n", cmdBuf);system(cmdBuf);
	}
	
	if (pRomeCfgParam->wlanCfgParam[w].dataRateMode == WLAN_DATA_RATE_G) // mix mode no need to set
	{  /*normal 11g(G Only)*/
		sprintf(cmdBuf, "iwpriv ath%d mode 3", w); 
		printf("%s\n", cmdBuf);system(cmdBuf);
		sprintf(cmdBuf, "iwpriv ath%d pureg 1", w); 
		printf("%s\n", cmdBuf);system(cmdBuf);
		
	}

	if (pRomeCfgParam->wlanCfgParam[w].auto_channel == 1)
		sprintf(cmdBuf, "iwconfig ath%d channel 0", w);
	else
		sprintf(cmdBuf, "iwconfig ath%d channel %d", w,pRomeCfgParam->wlanCfgParam[w].channel);
	printf("%s\n", cmdBuf);system(cmdBuf);

	
        if(pRomeCfgParam->wlanCfgParam[w].dataRate != 0) // no need to set auto rate 
        {
		if(pRomeCfgParam->wlanCfgParam[w].dataRate == 11)
			sprintf(cmdBuf, "iwconfig ath%d rate 5.5M", w);
		else
                	sprintf(cmdBuf, "iwconfig ath%d rate %dM", w, pRomeCfgParam->wlanCfgParam[w].dataRate / 2);         //Don't set basic Rate Set, Let driver take default.
		printf("%s\n", cmdBuf);system(cmdBuf);
	}
        
	if (pRomeCfgParam->wlanCfgParam[w].wep_type)
	{  //wep enable
		int keylen = 0;
		memset(&key, '\0', IW_ENCODING_TOKEN_MAX);
		//Set Active key first!
		setkey_idx = pRomeCfgParam->wlanCfgParam[w].wep_def_key;
		if (pRomeCfgParam->wlanCfgParam[w].wep_key_len==1) //64bit
		{
			keylen = 5;
			memcpy(key, pRomeCfgParam->wlanCfgParam[w].key64[setkey_idx], keylen);
		}
		else if (pRomeCfgParam->wlanCfgParam[w].wep_key_len==2)  //128bit
		{
			keylen = 13;
			memcpy(key, pRomeCfgParam->wlanCfgParam[w].key128[setkey_idx], keylen);
		}
		setKey(pRomeCfgParam->wlanCfgParam[w].wep_type, pRomeCfgParam->wlanCfgParam[w].authType, key, setkey_idx+1, keylen);
	}  //end of wep enable
	else  //wep disable
		setKey(pRomeCfgParam->wlanCfgParam[w].wep_type, pRomeCfgParam->wlanCfgParam[w].authType, NULL, -1, 0);
	
	if (pRomeCfgParam->wlanCfgParam[w].authType == 0)
	{  //shared key
		sprintf(cmdBuf, "iwpriv ath%d authmode 1", w);
		printf("%s\n", cmdBuf);system(cmdBuf);
		// set privacy for WEP
		if(pRomeCfgParam->wlanCfgParam[w].wep_type) {
			sprintf(cmdBuf, "iwpriv ath%d privacy 1", w);
			printf("%s\n", cmdBuf); system(cmdBuf);
		}
	}
	else
	if (pRomeCfgParam->wlanCfgParam[w].authType == 1)
	{  //shared key
		sprintf(cmdBuf, "iwpriv ath%d authmode 2", w);
		printf("%s\n", cmdBuf);system(cmdBuf);
	}
	
	return 0;
}

// Dino Chang 2005/02/25
void write_wpa_psk_config(void) {
	FILE *fp = NULL;

	if((fp = fopen("/var/hostapd.conf", "w+")) != NULL) {
		fprintf(fp, "interface=ath0\n");
		fprintf(fp, "driver=madwifi\n");
#if 0
		fprintf(fp, "logger_syslog=0\n");
		fprintf(fp, "logger_syslog_level=0\n");
		fprintf(fp, "logger_stdout=0\n");
		fprintf(fp, "logger_stdout_level=0\n");
#endif
		fprintf(fp, "debug=0\n");
		fprintf(fp, "dump_file=/var/tmp/hostapd.dump\n");
		fprintf(fp, "ctrl_interface=/var/run/hostapd\n");
		fprintf(fp, "ctrl_interface_group=root\n");
		fprintf(fp, "macaddr_acl=0\n");
		fprintf(fp, "auth_algs=3\n");
		fprintf(fp, "own_ip_addr=%u.%u.%u.%u\n", pRomeCfgParam->ifCfgParam[1].ipAddr[0], pRomeCfgParam->ifCfgParam[1].ipAddr[1], pRomeCfgParam->ifCfgParam[1].ipAddr[2], pRomeCfgParam->ifCfgParam[1].ipAddr[3]);
		fprintf(fp, "ssid=%s\n", pRomeCfgParam->wlanCfgParam[0].ssid);
		fprintf(fp, "eap_message=hello\n");
		fprintf(fp, "eapol_key_index_workaround=0\n");
		fprintf(fp, "wpa=1\n");
		// Dino Chang 2005/04/12
		// for WPA-PSK 64 hex characters
		if(strlen(pRomeCfgParam->wlanCfgParam[0].passphrase) == 64)
			fprintf(fp, "wpa_psk=%s\n", pRomeCfgParam->wlanCfgParam[0].passphrase);
		else
		// Dino
			fprintf(fp, "wpa_passphrase=%s\n", pRomeCfgParam->wlanCfgParam[0].passphrase);
		fprintf(fp, "wpa_pairwise=TKIP\n");
		fprintf(fp, "wpa_key_mgmt=WPA-PSK\n");
		fclose(fp);

		// bring up hostapd daemon
		system("/bin/hostapd -dd /etc/hostapd.conf&");
	}
}
// Dino

// Dino Chang 2005/03/15
void write_wpa_config(void) {
	FILE *fp = NULL;

	if((fp = fopen("/var/hostapd.conf", "w+")) != NULL) {
		fprintf(fp, "interface=ath0\n");
		fprintf(fp, "driver=madwifi\n");
#if 0
		fprintf(fp, "logger_syslog=0\n");
		fprintf(fp, "logger_syslog_level=0\n");
		fprintf(fp, "logger_stdout=0\n");
		fprintf(fp, "logger_stdout_level=0\n");
#endif
		fprintf(fp, "debug=0\n");
		fprintf(fp, "dump_file=/var/tmp/hostapd.dump\n");
		fprintf(fp, "ctrl_interface=/var/run/hostapd\n");
		fprintf(fp, "ctrl_interface_group=root\n");
		fprintf(fp, "macaddr_acl=0\n");
		fprintf(fp, "auth_algs=3\n");
		fprintf(fp, "own_ip_addr=%u.%u.%u.%u\n", pRomeCfgParam->ifCfgParam[1].ipAddr[0], pRomeCfgParam->ifCfgParam[1].ipAddr[1], pRomeCfgParam->ifCfgParam[1].ipAddr[2], pRomeCfgParam->ifCfgParam[1].ipAddr[3]);
		fprintf(fp, "ssid=%s\n", pRomeCfgParam->wlanCfgParam[0].ssid);
		fprintf(fp, "eap_message=hello\n");
		fprintf(fp, "eapol_key_index_workaround=0\n");
		fprintf(fp, "ieee8021x=1\n");
		// primary radius server
		fprintf(fp, "auth_server_addr=%u.%u.%u.%u\n", NIPQUAD(pRomeCfgParam->wlanCfgParam[0].rad_srv[0].ip));
		fprintf(fp, "auth_server_port=%u\n", pRomeCfgParam->wlanCfgParam[0].rad_srv[0].port);
		fprintf(fp, "auth_server_shared_secret=%s\n", pRomeCfgParam->wlanCfgParam[0].rad_srv[0].secret);
                // secondary radius server
		if(pRomeCfgParam->wlanCfgParam[0].rad_srv[1].ip != 0) {
			fprintf(fp, "auth_server_addr=%u.%u.%u.%u\n", NIPQUAD(pRomeCfgParam->wlanCfgParam[0].rad_srv[1].ip));
			fprintf(fp, "auth_server_port=%u\n", pRomeCfgParam->wlanCfgParam[0].rad_srv[1].port);
			fprintf(fp, "auth_server_shared_secret=%s\n", pRomeCfgParam->wlanCfgParam[0].rad_srv[1].secret);
                }
		fprintf(fp, "wpa=1\n");
		fprintf(fp, "wpa_pairwise=TKIP\n");
		fprintf(fp, "wpa_key_mgmt=WPA-EAP\n");
		fclose(fp);

		// bring up hostapd daemon
		system("/bin/hostapd /etc/hostapd.conf&");
	}
}
// Dino

/*
 * Routine Name: wlan_init
 * Description: configure wlan cfg param entry
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 wlan_init(void)
{		
	/* read cfg from cfgmgr */
	int w;
	char cmdBuf[80];
	FILE *f;
	totalWlanCards=0;
#ifdef CONFIG_RTL8185
	char name[32];
	//See how many cards probed by system
	for(w=0;w<MAX_WLAN_CARDS;w++){
		sprintf(name,"/proc/ath%d/mib_all",w);
		f=fopen(name,"r");
		if(!f){
			printf("Can't open file proc-ath0-mib_all !\n");
			break;
		}
		totalWlanCards++;
		fclose(f);
	}
#endif
	totalWlanCards=1;
	printf("Total %d wlan cards\n", totalWlanCards);
	for(w=0;w<totalWlanCards;w++){
		if(0==wlan_setup(w)){
			switch(pRomeCfgParam->wlanCfgParam[w].authType) {
				case 0: // Open
				case 1: // Shared
				default:
					{
						printf("Open ath%d...\n",w);
						sprintf(cmdBuf, "ifconfig ath%d up",w); //bring up interface.
						printf("%s\n", cmdBuf);system(cmdBuf);
					}
					break;
				case 2: // WPA
					write_wpa_config();
					break;
				case 3: // WPA-PSK
					write_wpa_psk_config();
					break;
			}
#if 0
			printf("Open ath%d...\n",w);
			sprintf(cmdBuf, "ifconfig ath%d up",w); //bring up interface.
			printf("%s\n", cmdBuf);system(cmdBuf);
#endif
			//wds_up(w);  //marked by Murphy--we don't implement WDS
		}else
			printf("WLAN setup failed!!!\n");
	}
	return 0;	
} /* end wlan_init */

/*
 * Routine Name: ddnsCfgFactoryDefaultFunction
 * Description: ddns cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */

void ddnsCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(struct ddnsCfgParam_s);
	ddnsCfgParam_t tmpTab;
	
	memset((void*)&tmpTab, 0, mySize);
	memcpy((void*)&tmpTab, (void*)&ddnsCfgParamDefault, mySize);
	memcpy((void*)pBuf, (void*)&tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end ddnsCfgFactoryDefaultFunction */

void ddns_start(void)
{
	int pid;
	char type[32];
	FILE *f;

	f = fopen("/var/run/ez-ipupdate.pid", "r");
	if(f != NULL){
		fscanf(f, "%d", &pid);
		fclose(f);
		printf("[Penguin: board.c] ddns_start: ez-ipupdate's pid=%d\n", pid);
		sprintf(type, "kill -9 %d", pid);
		printf("[Penguin: board.c] ddns_start: command=%s\n", type);
		if(pid > 0)
			system(type);
	}

	switch(pRomeCfgParam->ddnsCfgParam.ddns_type) 
	{
	case 0:	
		sprintf(type, "dyndns");
		break;
	case 1:
		sprintf(type, "easydns");
		break;
	case 2:
		sprintf(type, "no-ip2");
		break;
	}

	f = fopen("/var/ddns.conf", "w");
	if(f != NULL){
		fprintf(f, "#!//bin//ez-ipupdate -c\n");
		fprintf(f, "service-type=%s\n", type);
		fprintf(f, "user=%s:%s\n", pRomeCfgParam->ddnsCfgParam.username, pRomeCfgParam->ddnsCfgParam.password);
		fprintf(f, "host=%s\n", pRomeCfgParam->ddnsCfgParam.Rhostname);
		fprintf(f, "interface=ppp0\n");
		fprintf(f, "max-interval=2073600\n");
		fprintf(f, "cache-file=/var/tmp/ez-ipupdate.cache\n");
		fprintf(f, "daemon\n");
		fclose(f);
	}
	system("/bin/ez-ipupdate -d -c /var/ddns.conf");

}

#if 0
/*
void ddns_start(void)
{
	char str[160]={0};
	char interface[10]={0};
	int cnt=0;
	FILE *f,*f2;
		
	if(pRomeCfgParam->ddnsCfgParam.enable==1)
	{
		//del old process
		remove("/var/run/no-ip2.conf");
		do_kill("/var/run/no-ip2.pid");	
	
		switch(pRomeCfgParam->ifCfgParam[0].connType)
		{
			case IFCFGPARAM_CONNTYPE_STATIC:
			case IFCFGPARAM_CONNTYPE_DHCPC:
				sprintf(interface,"eth0");
				break;
			case IFCFGPARAM_CONNTYPE_PPPOE:
			case IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED:
			case IFCFGPARAM_CONNTYPE_PPTP:
			case IFCFGPARAM_CONNTYPE_L2TP:
			case IFCFGPARAM_CONNTYPE_3G:
				sprintf(interface,"ppp0");
				break;
			case IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE:
				sprintf(interface,"ppp%d",pRomeCfgParam->pppoeCfgParam[pRomeCfgParam->pppoeCfgParam[0].whichPppObjId].pppx);
				break;
		}
		
		f2=fopen("/var/run/no-ip2.pid","r");
		if(f2!=NULL) { fclose(f2); return; } // process is running...
	
		remove("/var/run/no-ip2.conf");
		//sprintf(str,"noip2 -C -U 30 -u \"%s\" -p \"%s\" -I %s &",pRomeCfgParam->ddnsCfgParam.username,pRomeCfgParam->ddnsCfgParam.password,interface); //Marked by Nick HO
		sprintf(str,"noip2 -C -U 30 -u \"%s\" -p \"%s\" -I %s -R \"%s\" -T %u &",pRomeCfgParam->ddnsCfgParam.username,pRomeCfgParam->ddnsCfgParam.password,interface, pRomeCfgParam->ddnsCfgParam.Rhostname, pRomeCfgParam->ddnsCfgParam.ddns_type); //Added by Nick HO
		
		system(str);	
		while(1)
		{
			sleep(1);
			f=fopen("/var/run/no-ip2.conf","r");
			if(f!=NULL) break;
			cnt++;
			if(cnt>5) break;
		}
		if(f!=NULL) fclose(f);
		if(cnt>5) return; // configure file can't be created.		
		system("/bin/noip2 &");	
	}
}
*/
#endif

/*
 * Routine Name: ddns_init
 * Description: configure ddns cfg param entry
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 ddns_init(void)
{	
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_DDNS,(void*)&(pRomeCfgParam->ddnsCfgParam), sizeof(struct ddnsCfgParam_s))!=0)
	{
		printf("ddns_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}

	return 0;	
} /* end ddns_init */



void qosCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(struct qosCfgParam_s)*MAX_QOS;
	qosCfgParam_t tmpTab[MAX_QOS];
	
	memset((void*)&tmpTab, 0, mySize);
	memcpy((void*)&tmpTab, (void*)&qosCfgParamDefault, mySize);
	memcpy((void*)pBuf, (void*)&tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
}


void rateLimitCfgFactoryDefaultFunction(void *pBuf, uint32 *pBufSize)
{
	uint32 mySize = sizeof(struct rateLimitCfgParam_s);
	rateLimitCfgParam_t tmpTab;

	memset((void *)&tmpTab, 0, mySize);
	memcpy((void *)&tmpTab, (void *)&rateLimitCfgParamDefault, mySize);
	memcpy((void *)pBuf, (void *)&tmpTab, mySize);
	memcpy((void *)pBufSize, (void *)&mySize, sizeof(mySize));
	return;
}


uint32 rateLimit_init(void)
{
	int32 retval;
	uint32 entry;
	uint8 *ifname[] = { "eth1", "eth0" };
	rtl8651_tblDrvAclRule_t rule;
	
	
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_RATELIMIT,(void*)&(pRomeCfgParam->rateLimitCfgParam), sizeof(struct rateLimitCfgParam_s))!=0) 	{
		printf("rateLimit_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}

	for(entry=0; entry<8; entry++) {
		if (pRomeCfgParam->rateLimitCfgParam.entry[entry].enable == FALSE) {
			bzero(&pRomeCfgParam->rateLimitCfgParam.entry[entry], sizeof(rateLimitEntry_t));
			pRomeCfgParam->rateLimitCfgParam.entry[entry].ipMask = 0xffffffff;
			continue;
		}
		if (pRomeCfgParam->rateLimitCfgParam.enable == FALSE)
			continue;
		asp_addRateLimitRule(&pRomeCfgParam->rateLimitCfgParam.entry[entry], entry);
	}
	return 0;
}


void ratioQoSCfgFactoryDefaultFunction(void *pBuf, uint32 *pBufSize)
{
	uint32 mySize = sizeof(ratioQosCfgParam_t);
	ratioQosCfgParam_t tmpTab;

	memset((void *)&tmpTab, 0, mySize);
	memcpy((void *)&tmpTab, (void *)&ratioQosCfgParamDefault, mySize);
	memcpy((void *)pBuf, (void *)&tmpTab, mySize);
	memcpy((void *)pBufSize, (void *)&mySize, sizeof(mySize));
	return;
}

uint32 ratioQos_init(void)
{	
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_RATIOQOS,(void*)&(pRomeCfgParam->ratioQosCfgParam), sizeof(struct ratioQosCfgParam_s))!=0)
	{
		printf("ratioQos_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}
	rearrange_Ratio_Qos(FALSE);
	return 0;
}


/*
 *  This function will return the external IP address of the specific session
 *    no matter the WAN type is static or not, and no matter the WAN type is PPPoE or not.
 */
ipaddr_t board_getExternalIp( int32 ms )
{
	ipaddr_t msIp;

	msIp = NULL;

	switch(pRomeCfgParam->ifCfgParam[0].connType)
	{
		case IFCFGPARAM_CONNTYPE_STATIC:/* fall through */
		case IFCFGPARAM_CONNTYPE_DHCPC: /* fall through */
		case IFCFGPARAM_CONNTYPE_PPTP:  /* fall through */
		case IFCFGPARAM_CONNTYPE_L2TP:  /* fall through */
		case IFCFGPARAM_CONNTYPE_PPPOE: /* fall through */
		case IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED:
			msIp = pRomeCfgParam->ifCfgParam[0].ipAddr[0] << 24 |
		           pRomeCfgParam->ifCfgParam[0].ipAddr[1] << 16 |
		           pRomeCfgParam->ifCfgParam[0].ipAddr[2] << 8 |
		           pRomeCfgParam->ifCfgParam[0].ipAddr[3];
			break;
		case IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE:
			if ( pRomeCfgParam->pppoeCfgParam[ms].dialState == PPPOECFGPARAM_DIALSTATE_DIALED_SUCCESS )
			{
				msIp = pRomeCfgParam->pppoeCfgParam[ms].ipAddr[0] << 24 |
			           pRomeCfgParam->pppoeCfgParam[ms].ipAddr[1] << 16 |
			           pRomeCfgParam->pppoeCfgParam[ms].ipAddr[2] << 8 |
			           pRomeCfgParam->pppoeCfgParam[ms].ipAddr[3];
			}
			break;
		default:
			break;
	}

	return msIp;
}


/*
 *	Protocol-Based NAT
 */
void pbnatCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	pbnatCfgParam_t tmpTab[MAX_PPPOE_SESSION][MAX_PBNAT];
	uint32 mySize = sizeof(tmpTab);
	
	memset((void*)&tmpTab, 0, mySize);
	memcpy((void*)&tmpTab, (void*)&pbnatCfgParamDefault, mySize);
	memcpy((void*)pBuf, (void*)&tmpTab, mySize);

	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
}


int32 pbnat_ipup( int32 ms, ipaddr_t MyIp )
{
	int j,ret;

	if ( MyIp == NULL ) return SUCCESS; /* This session does NOT holds an IP address */
	
	for( j = 0; j < MAX_PBNAT; j++ )
	{
		if ( pRomeCfgParam->pbnatCfgParam[ms][j].enabled )
		{
			ret = rtl8651_addProtocolBasedNAT( pRomeCfgParam->pbnatCfgParam[ms][j].protocol,
			                                   MyIp,
		                                       pRomeCfgParam->pbnatCfgParam[ms][j].IntIp );
			if ( ret != SUCCESS )
			{
				printf( "rtl8651_addProtocolBasedNAT(protocol:%d,ExtIp:%08x,IntIp:%08x) error: %d, but never mind.\n",
					pRomeCfgParam->pbnatCfgParam[ms][j].protocol,
					MyIp,
		    		pRomeCfgParam->pbnatCfgParam[ms][j].IntIp,
		    		ret );
			}
		}
	}

	return SUCCESS;
}


int32 pbnat_ipdown( int32 ms, ipaddr_t MyIp )
{
	int j,ret;

	if ( MyIp == NULL ) return SUCCESS; /* This session does NOT holds an IP address */
	
	for( j = 0; j < MAX_PBNAT; j++ )
	{
		if ( pRomeCfgParam->pbnatCfgParam[ms][j].enabled ) // no matter enabled or not, delete anyway.
		{
			ret = rtl8651_delProtocolBasedNAT( pRomeCfgParam->pbnatCfgParam[ms][j].protocol,
			                                   MyIp,
		                                       pRomeCfgParam->pbnatCfgParam[ms][j].IntIp );
			if ( ret != SUCCESS )
			{
				printf( "rtl8651_delProtocolBasedNAT(protocol:%d,ExtIp:%08x,IntIp:%08x) error: %d, but never mind.\n",
					pRomeCfgParam->pbnatCfgParam[ms][j].protocol,
					MyIp,
		    		pRomeCfgParam->pbnatCfgParam[ms][j].IntIp,
		    		ret );
			}
		}
	}

	return SUCCESS;
}


uint32 pbnat_init()
{
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_PBNAT,(void*)&(pRomeCfgParam->pbnatCfgParam), sizeof(struct pbnatCfgParam_s)*MAX_PBNAT*MAX_PPPOE_SESSION)!=0)
	{
		printf("pbnat_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}

	if ( pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_STATIC )
	{
		pbnat_ipup( 0, pRomeCfgParam->ifCfgParam[0].ipAddr[0] << 24 |
		               pRomeCfgParam->ifCfgParam[0].ipAddr[1] << 16 |
		               pRomeCfgParam->ifCfgParam[0].ipAddr[2] << 8 |
		               pRomeCfgParam->ifCfgParam[0].ipAddr[3] );
	}

	return 0;
}


uint32 qos_init(void)
{	
	rtl8651_tblDrvPolicy_t policy;
	int32 index;
	
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_QOS,(void*)&(pRomeCfgParam->qosCfgParam), sizeof(struct qosCfgParam_s)*MAX_QOS)!=0)
	{
		printf("qos_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return 1;
	}

	if (pRomeCfgParam->qosCfgParam[0].qosType & ENABLE_QOS) {
		for (index=0; index<8; index++) {
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

	
	if (pRomeCfgParam->qosCfgParam[8].qosType & ENABLE_QOS) {
		for(index=0; index<RTL8651_PORT_NUMBER; index++) {
			rtl8651_setFlowControl(index, pRomeCfgParam->qosCfgParam[index+8].un.port_based.enableFlowCtrl);
			rtl8651_setPortPriorityQueue(index, pRomeCfgParam->qosCfgParam[index+8].un.port_based.isHigh);
			rtl8651_setEthernetPortBandwidthControl(index, TRUE, pRomeCfgParam->qosCfgParam[index+8].un.port_based.inRL);
			rtl8651_setEthernetPortBandwidthControl(index, FALSE, pRomeCfgParam->qosCfgParam[index+8].un.port_based.outRL);
		}
	}
	else {
		for(index=0; index<RTL8651_PORT_NUMBER; index++) {
			rtl8651_setFlowControl(index, TRUE);
			rtl8651_setPortPriorityQueue(index, FALSE);
			rtl8651_setEthernetPortBandwidthControl(index, TRUE, RTL8651_BC_FULL);
			rtl8651_setEthernetPortBandwidthControl(index, FALSE, RTL8651_BC_FULL);
		}
	}
	
	return 0;	
} 



/*
 * Routine Name: mgmtCfgFactoryDefaultFunction
 * Description: mgmt cfg callback function (to be called by cfgmgr when
 *              factory default reset is needed)
 * Input:  
 *   pBuf       buffer pointer (prepared by caller)
 *   pBufSize   uint32 pointer (prepared by caller)
 * Output:
 *   none
 * Return:
 *   none
 * ThreadSafe: don't know yet
 */
void mgmtCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(mgmtCfgParam_t)*MAX_MGMT_USER;
	mgmtCfgParam_t tmpTab[MAX_MGMT_USER];
	
	memset((void*)tmpTab, 0, mySize);
	memcpy((void*)tmpTab, (void*)mgmtCfgParamDefault, sizeof(mgmtCfgParamDefault));
	memcpy((void*)pBuf, (void*)tmpTab, mySize);
	
	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end mgmtCfgFactoryDefaultFunction */

int32 bdinfo_getMac(macaddr_t *mac_P)
{
	flashdrv_read((void*)mac_P,(void*)FLASH_MAP_BOARD_INFO_ADDR,sizeof(macaddr_t));
    return 0;
} /* end bdinfo_getMac */

void specialapCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(struct specialapCfgParam_s)*MAX_SPECIAL_AP*MAX_PPPOE_SESSION;
	specialapCfgParam_t tmpTab[MAX_PPPOE_SESSION][MAX_SPECIAL_AP];
	
	memset((void*)tmpTab, 0, mySize);
	memcpy((void*)tmpTab, (void*)specialapCfgParamDefault, mySize);
	memcpy((void*)pBuf, (void*)tmpTab, mySize);
	
	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));

	return;
} /* end specialapCfgFactoryDefaultFunction */

/*
 * Routine Name: specialap_init
 * Description: configure specialap cfg param entry
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 specialap_init(void)
{	
	/* read cfg from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_SPECIALAP,(void*)pRomeCfgParam->specialapCfgParam, sizeof(struct specialapCfgParam_s)*MAX_SPECIAL_AP*MAX_PPPOE_SESSION)!=0)
	{
		printf("specialap_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return NOT_OK;
	}
	return OK;
} /* end specialap_init */

void tbldrvCfgFactoryDefaultFunction(void * pBuf, uint32 * pBufSize)
{
	uint32 mySize = sizeof(tbldrvCfgParam_t);
	tbldrvCfgParam_t tmpTab;
	
	memset((void*)&tmpTab, 0, mySize);
	memcpy((void*)&tmpTab, (void*)&tbldrvCfgParamDefault, sizeof(tbldrvCfgParam_t));
	memcpy((void*)pBuf, (void*)&tmpTab, mySize);
	
	/* patch for avoiding word-aligned load/store */
	/* *pBufSize = mySize; */
	memcpy((void*)pBufSize, (void*)&mySize, sizeof(mySize));
	
	return;
} /* end tbldrvCfgFactoryDefaultFunction */


/*
 * rtl8651 demo system configuration initialization entry point
 * called by main.c of go-ahead web server	
 */
#define SIGNAL(s, handler)	do { \
	sa.sa_handler = handler; \
	sigaction(s, &sa, NULL);  \
    } while (0)


void pppd_start_dial(int sig)
{
	char idStr[4];
	int identity;
	char tmp[20];
	FILE *f2;
	identity=sig-40;
	sprintf(idStr,"%u",identity);
	fprintf(stderr,"receive signal %d identity %d\n",sig,identity);
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)	
	if(identity==4) // PPTP
	{	
		char tmp[32];
		rtl8651_setNetMtu("eth0",pRomeCfgParam->pptpCfgParam.mtu);
		sprintf(tmp,"/var/run/pptp.pid");		

#ifdef __uClinux__ 
#else //MMU
		send_signal(tmp,9);
		unlink(tmp);
		unlink("/var/run/pppd0.pid");
#endif

#ifdef __uClinux__ //non-MMU
		f2=fopen(tmp,"r");
		if(f2)
		{		
			fclose(f2);
			send_signal("/var/run/pptp.pid",SIGUSR2);
		}		
		else
#endif			
		{
			sprintf(tmp,"/bin/pptp %u.%u.%u.%u &"
			,pRomeCfgParam->pptpCfgParam.svAddr[0]
			,pRomeCfgParam->pptpCfgParam.svAddr[1]
			,pRomeCfgParam->pptpCfgParam.svAddr[2]
			,pRomeCfgParam->pptpCfgParam.svAddr[3]);
			system(tmp);
		}

	}
	else if(identity==5)
	{
		char tempStr[32];
		rtl8651_setNetMtu("eth0",pRomeCfgParam->l2tpCfgParam.mtu);
		sprintf(tempStr,"/var/run/l2tpd.pid");		
#ifdef __uClinux__ 
#else // MMU
		send_signal(tempStr,9);
		unlink(tempStr);
		unlink("/var/run/pppd0.pid");
#endif

#ifdef __uClinux__ // non-MMU
		f2=fopen(tempStr,"r");
		if(f2)
		{
			fclose(f2);
			send_signal("/var/run/l2tpd.pid",SIGUSR2);		
		}
		else
#endif			
		{		
			sprintf(tempStr,"/bin/l2tpd &");
			system(tempStr);
		}
	}
	else
	{
#endif
		sprintf(tmp,"/var/run/pppd%u.pid",identity);

#ifdef __uClinux__ 
#else//MMU
		send_signal(tmp,9);
		unlink(tmp);
#endif

#ifdef __uClinux__
		f2=fopen(tmp,"r");
		if(f2)
		{
			fclose(f2);
			send_signal(tmp,16); //restart pppd
		}
		else // run first time	
#endif		
		{
			do_exec("/bin/pppd",idStr);
		}
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)	
	}
#endif

}


#if defined(CONFIG_RTL865X_INIT_BUTTON)
void call_factory_default(int sig)
{
	cfgmgr_factoryDefault(1);
} /* end call_factory_ default */
#endif /* CONFIG_RTL865X_INIT_BUTTON */


// call back function for GPIO restore to default
void call_default_gpio(int sig)
{
	cfgmgr_factoryDefault();
	re865xIoctl("eth0",RTL8651_IOCTL_REBOOT,0,0,0);
}

// call back function for WCN while USB disk inserted
void call_wcn_update(int sig)
{
	cfgmgr_write(CFGMGR_TABID_WLAN,(void*)pRomeCfgParam->wlanCfgParam,sizeof(wlanCfgParam_t)*MAX_WLAN_CARDS);
	cfgmgr_task();
	re865xIoctl("eth0",RTL8651_IOCTL_REBOOT,0,0,0);
}

uint32 sysInit(void)
{
	FILE *f1;
    	FILE *pidfile;
        struct sigaction sa;
        sigset_t mask;
	int i;
	
	/* cfgmgr module init */
	printf("System initializing...\n");
		    
	// wriet webs pid to file, added by Nick Wang. 11/09/2004
    	if ((pidfile = fopen("/var/run/webs.pid", "w")) != NULL)
    	{
                fprintf(pidfile, "%d\n", getpid());
                (void) fclose(pidfile);
    	}
	// add end
	
	fflush(0);
	
	
	// Set Switch LED mode
	rtl8651_setsinglecolor5ledmode();

	/* ===================================
		1. Init cfgmgr modules
	=================================== */
	#if defined(CONFIG_RTL865X_DIAG_LED)
	if (cfgmgr_init()!=OK)
		//re865xIoctl("eth0",SET_VLAN_IP,*(uint32*)(&(pRomeCfgParam->pptpCfgParam.ipAddr)),netmask,0);
		//re865xIoctl(char * name, uint32 arg0, uint32 arg1, uint32 arg2, uint32 arg3);
		re865xIoctl("eth0",RTL8651_IOCTL_DIAG_LED,2,0,0);
	#else
	cfgmgr_init(); //before any flash read/write	
	#endif /* CONFIG_RTL865X_DIAG_LED */
	/* ===================================
		2. Init timezone
	=================================== */
	time_init();
	zone_init();
	/* ===================================
		3. Init variables for NAT
	=================================== */
	nat_init();
	/* ===================================
		4. Init signals
	=================================== */
    	sigemptyset(&mask);
    	sigaddset(&mask, 40);
    	sigaddset(&mask, 41);
    	sigaddset(&mask, 42);
    	sigaddset(&mask, 43);

    	sigaddset(&mask, 44); //for pptp
    	sigaddset(&mask, 45); //for l2tp
	#if defined(CONFIG_RTL865X_INIT_BUTTON)
	sigaddset(&mask, 50);
	#endif /* CONFIG_RTL865X_INIT_BUTTON */

	// GST GPIO sig handlers
	sigaddset(&mask, 51);
	// add end

	// WCN Check handlers
	sigaddset(&mask, 52);
	// add end

	// Dino Chang 2005/01/18, add for TMSS write license to flash
	sigaddset(&mask, 53);
	// Dino
	
	sa.sa_mask = mask;
	sa.sa_flags = 0;
	re865xIoctl("eth0",ADD_SIGNAL_RECEIVE_PROCESS,getpid(),0,0);
    	SIGNAL(40, pppd_start_dial);	
    	SIGNAL(41, pppd_start_dial);	
    	SIGNAL(42, pppd_start_dial);	
    	SIGNAL(43, pppd_start_dial);
    	SIGNAL(44, pppd_start_dial);
    	SIGNAL(45, pppd_start_dial);    	

	#if defined(CONFIG_RTL865X_INIT_BUTTON)
	SIGNAL(50, call_factory_default);
	#endif /* CONFIG_RTL865X_INIT_BUTTON */

	// added by Nick Wang, 11/09/2004
	SIGNAL(51, call_default_gpio);
	// add end

	// added by Nick Wang, 11/16/2004
	SIGNAL(52, call_wcn_update);
	// add end

	// Dino Chang 2005/01/18
	SIGNAL(53, check_tmss_lic);
	// Dino
	/* ===================================
		5. Init variables for VLAN
	=================================== */
	
	/* read vlanCfgParam from cfgmgr */
	vlan_init();

	/* call cfgmgr to read xxxCfgParam structures from nv-ram
	 * wan interface's init is flat in this sysInit()
	 */
	/* ===================================
		6. Init variables for Interface
	=================================== */
	/* read ifCfgParam from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_IF,(void*)pRomeCfgParam->ifCfgParam, sizeof(pRomeCfgParam->ifCfgParam))!=0)
	{
		printf("sysInit: cfgmgr read ifCfgParam fail\n");
		/* take proper actions */
		return 1;
	}
	memcpy((void*)&ramWanCfgParam, &(pRomeCfgParam->ifCfgParam[0]),sizeof(ifCfgParam_t));
	memcpy((void*)&ramLanCfgParam, &(pRomeCfgParam->ifCfgParam[1]),sizeof(ifCfgParam_t));
	
	/* ===================================
		7. Init variables for DHCP-Client
	=================================== */
	/* read dhcpcCfgParam from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_DHCPC,(void*)&(pRomeCfgParam->dhcpcCfgParam), sizeof(dhcpcCfgParam_t))!=0)
	{
		printf("sysInit: cfgmgr read dhcpcCfgParam fail\n");
		/* take proper actions */
		return 1;
	}
	
	memcpy((void*)&(ramDhcpcCfgParam),(void*)&(pRomeCfgParam->dhcpcCfgParam),sizeof(dhcpcCfgParam_t));
	
	/* check if dhcp client uses clone mac */
	#if 0
	/* rupert used another method to check if clone mac is used in wanlan_init() */
	for(i=0; i<VLAN_LOAD_COUNT; i++)
	{
		if(pRomeCfgParam->vlanCfgParam[i].valid)
		{
			if (   i==0 /* vlanCfgParam[0] is for external vlan(wan) */
			    && (pRomeCfgParam->ifCfgParam[i].connType  == IFCFGPARAM_CONNTYPE_DHCPC)
			    && (pRomeCfgParam->dhcpcCfgParam.cmacValid == 1)
			   )
			{				
				if (    pRomeCfgParam->dhcpcCfgParam.cloneMac[0]==0
				     && pRomeCfgParam->dhcpcCfgParam.cloneMac[1]==0
				     && pRomeCfgParam->dhcpcCfgParam.cloneMac[2]==0
				     && pRomeCfgParam->dhcpcCfgParam.cloneMac[3]==0
				     && pRomeCfgParam->dhcpcCfgParam.cloneMac[4]==0
				     && pRomeCfgParam->dhcpcCfgParam.cloneMac[5]==0 )
				{
					/* error-protection: in case err cfg cannot boot */
					pRomeCfgParam->dhcpcCfgParam.cloneMac[0]=0x00;
					pRomeCfgParam->dhcpcCfgParam.cloneMac[1]=0x00;
					pRomeCfgParam->dhcpcCfgParam.cloneMac[2]=0xdd;
					pRomeCfgParam->dhcpcCfgParam.cloneMac[3]=0xee;
					pRomeCfgParam->dhcpcCfgParam.cloneMac[4]=0xff;
					pRomeCfgParam->dhcpcCfgParam.cloneMac[5]=0x10;
				}				
				
				memcpy(mac,pRomeCfgParam->dhcpcCfgParam.cloneMac,6);
			}
			else
			{
				bdinfo_getMac((macaddr_t*)mac);
				mac[5]+=i;
			}			
			memcpy(&(pRomeCfgParam->vlanCfgParam[i].gmac), mac, 6);		
		} /* end if */		
	} /* end for i */	
	memcpy(&(pRomeCfgParam->vlanCfgParam[i].gmac), mac, 6);
	#endif
	/* ===================================
		8. Init variables for PPPoE
	=================================== */
	/* read pppoeCfgParam from cfgmgr */
	if (cfgmgr_read(CFGMGR_TABID_PPPOE,(void*)pRomeCfgParam->pppoeCfgParam, sizeof(pppoeCfgParam_t)*MAX_PPPOE_SESSION)!=0)
	{
		printf("sysInit: cfgmgr read ppoeCfgParam fail\n");
		/* take proper actions */
		return 1;
	}

	/* ===================================
		9. Init PPTP/L2TP services
	=================================== */
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
	pptp_init();// read pptp configuration
	//printf("l2tp, ");	
	l2tp_init();// read l2tp configuration
	pRomeCfgParam->pptpCfgParam.dialState=PPTPCFGPARAM_DIALSTATE_OFF;	
	pRomeCfgParam->l2tpCfgParam.dialState=L2TPCFGPARAM_DIALSTATE_OFF;
#endif //CONFIG_RTL865X(B)_PPTPL2TP
	// clear dialState when boot
	for(i=0;i<MAX_PPPOE_SESSION;i++)
		pRomeCfgParam->pppoeCfgParam[i].dialState=PPPOECFGPARAM_DIALSTATE_OFF;	
	
	
	memcpy((void*)ramPppoeCfgParam,(void*)pRomeCfgParam->pppoeCfgParam,sizeof(pppoeCfgParam_t)*MAX_PPPOE_SESSION);

	/* ===================================
		10. Init WAN/LAN settings and configurations
	=================================== */
	wanlan_init(); //NOTICE: rome driver access is okay after this line

	/* ===================================
		11. Init RomeDriver Services
	=================================== */
	log_init();	
	userDb_init();
	
	/* read from flash, these components are wan-independent */
	// removed by Nick Wang, No needed in KR1 project and add new method for MAC filter.
	//urlfilter_init(); // read flash and setTableDriver	
	//##################################################################
	if(pRomeCfgParam->natCfgParam.mac_filter)
	{
		int mac_filter_counter  = 0;
		rtl8651_tblDrvAclRule_t samplelRule;
		int8 ifLanStr[20] = {0};
		int32 ret;
		uint8 macNull[6]={0,0,0,0,0,0};
		uint8 macFFFF[6]={0xff,0xff,0xff,0xff,0xff,0xff};

		for (i = 0; i < FILTER_ENTRY_NUM; i++)
		{
			if (memcmp(pRomeCfgParam->macfilterCfgParam[0][i].mac, macNull, 6))		
				mac_filter_counter++;
		}
		
		for(i = 0; i < mac_filter_counter; i++)
		{
			sprintf(ifLanStr,"eth1");
			samplelRule.matchType_ = RTL8651_ACL_GENERIC;        
			samplelRule.ruleType_ = RTL8651_ACL_MAC;
				
			if(pRomeCfgParam->macfilterCfgParam[0][i].allow == 0)
				samplelRule.actionType_ = RTL8651_ACL_DROP; // Deny MAC list (Negative List)
			else if(pRomeCfgParam->macfilterCfgParam[0][i].allow == 1)
				samplelRule.actionType_ = RTL8651_ACL_PERMIT; // Allow MAC list (Positive List)		
			else
			{
				 printf("Error!!!!!!!!\n");
				 return 0;
			}
			
			memcpy(&samplelRule.srcMac_, pRomeCfgParam->macfilterCfgParam[0][i].mac,6); 
			memcpy(&samplelRule.srcMacMask_, macFFFF,6);  
			
			memcpy(&samplelRule.dstMac_, macNull,6);
			memcpy(&samplelRule.dstMacMask_, macNull,6);
			samplelRule.typeLen_ = 0; 
			samplelRule.typeLenMask_ = 0;  
			ret = rtl8651a_addAclRule(0, ifLanStr, 1 ,(rtl8651_tblDrvAclRule_t *)&samplelRule);
			if (ret!=SUCCESS)
				printf("%s: %d: rtl8651a_addAclRule() ret:%d\n",__FUNCTION__, __LINE__, ret);
		}
		
		// add DROP for all for positive list
		if(pRomeCfgParam->macfilterCfgParam[0][0].allow == 1)
    		{
			sprintf(ifLanStr,"eth1");
			samplelRule.matchType_ = RTL8651_ACL_GENERIC;        
			samplelRule.ruleType_ = RTL8651_ACL_MAC;
				
			samplelRule.actionType_ = RTL8651_ACL_DROP; // Deny all others MAC
			
			memcpy(&samplelRule.srcMac_, macNull,6); 
			memcpy(&samplelRule.srcMacMask_, macNull,6);  
			
			memcpy(&samplelRule.dstMac_, macNull,6);
			memcpy(&samplelRule.dstMacMask_, macNull,6);
			samplelRule.typeLen_ = 0; 
			samplelRule.typeLenMask_ = 0;  
			ret = rtl8651a_addAclRule(0, ifLanStr, 1 ,(rtl8651_tblDrvAclRule_t *)&samplelRule);
			if (ret!=SUCCESS)
				printf("%s: %d: add DROP for all for positive list: rtl8651a_addAclRule() ret:%d\n",__FUNCTION__, __LINE__, ret);
    		}
		
	}
	//##################################################################

        dns_init();
	dos_init();
	alg_init();
	dhcps_init();	
	
	/* read from flash, these components are wan-dependent */
	specialap_init(); //must before acl_tableDriverAccess()
	acl_init();  //must before acl_tableDriverAccess()
	serverp_init();		
	dmz_init();
	udpblock_init();
	upnp_init();	
	ddns_init();
	routing_init();
	qos_init();
	pbnat_init();
	rateLimit_init();
	ratioQos_init();
	napt_init();

        ppp_init();//peter and howard


	/* ===================================
		12. Init Wireless LAN

		NOTE:
			(tony: wlan must put here, else other init will have problem.)
	=================================== */
	//tony: wlan must put here, else other init will have problem.
	wlan_init();
	
	/* add by Min Lee */
	if (cfgmgr_read(CFGMGR_TABID_WLAN,(void*)pRomeCfgParam->wlanCfgParam, sizeof(struct wlanCfgParam_s)*MAX_WLAN_CARDS)!=0)
	{
		printf("wlan_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return -1;
	}

	if (cfgmgr_read(CFGMGR_TABID_TMSS,(void*)(&pRomeCfgParam->tmssCfgParam), sizeof(struct tmssCfgParam_s))!=0)
	{
		printf("tmss_init: call cfgmgr_read fail\n");
		/* take proper actions */
		return -1;
	}

        // Dino Chang 2004/12/30
        if (cfgmgr_read(CFGMGR_TABID_TMSS_CLIENTINFO,(void*)(&pRomeCfgParam->tmssCInfoCfgParam), sizeof(struct tmssCInfoCfgParam_s) * PC_USER_LEN_S)!=0)
        {
                printf("tmss_init: call cfgmgr_read fail\n");
                /* take proper actions */
                return -1;
        }
        // Dino
							
#ifdef __mips__
		re865xIoctl("eth0",GET_MAC,(unsigned int)&pRomeCfgParam->ifCfgParam[0].mac,0,0);
#endif
	/* the end of add by Min Lee */
	
	/* ===================================
		13.	Init configurations and related services
			if WAN-side is the STATIC IP Mode
	=================================== */
	
	// STATIC MODE
	if(pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_STATIC)
	{
		/* for wan cfg other than STATIC, network interface will be created
		 * after wan ip is assigned, so acl rule can only be created by that
		 * time
		 */
		time_start();		
		acl_tableDriverAccess();
		serverp_tableDriverAccess(0);
		dmz_tableDriverAccess(0);
		udp_tableDriverAccess(0);
		upnp_start();
		ddns_start(); //must after dns_init();
		rtl8651a_setWanStatus(0,TRUE);
		rtl8651a_setDosStatus(0, ((pRomeCfgParam->dosCfgParam[0].enable)?(pRomeCfgParam->dosCfgParam[0].enableItem):0));
		// Dino Chang 2005/03/07
		// more to serverp_tableDriverAccess()
		//rtl8651a_setAlgStatus(0,pRomeCfgParam->algCfgParam[0].algList,pRomeCfgParam->algCfgParam[0].localIp);
		// Dino
	}
	else // only enable UPNP Daemon without WAN IP
		upnp_start();

	/* ===================================
		14.	Init DHCP Server
	=================================== */
	if(pRomeCfgParam->dhcpsCfgParam.enable==1) dhcps_start();

	/* ===================================
		15.	Init Remote-management functions
	=================================== */
	remoteMgmt_tableDriverAccess();	
         
	// Dino Chang 2005/01/15
	// Fixed when WAN is fixed IP, we can't connect to internet through 80 port
	// Dino Chang 2004/12/08
	// Start Trend Micro Security Services daemon
#if 0
	if(pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_STATIC)
	{
		// Dino Chang 2004/12/28
		//do_wait_exec("/bin/tmss/tmHostent", NULL);
		// Dino
		// Dino Chang 2005/01/11
		//if(pRomeCfgParam->tmssCfgParam.tmss_enable) {
		if(pRomeCfgParam->tmssCfgParam.tmss_enable || pRomeCfgParam->tmssCfgParam.pc_enable) {
			// Dino Chang 2005/01/15
			restart_tmss_daemon(1);
			// Dino
		}
   	}
	   // Dino
	// Dino
#endif

	/* ===================================
		16.	Init semaphores
	=================================== */
	//rtl8651_sem_init(1);	/* default : no counting semaphore */
	
	return 0;
} /* end sysInit */

/*
 * re865x Ioctl (to call kernel functions including rtl8651 table driver APIs
 */
// Dino Chang, 2004/11/08, move this function call to tblDrv_mapping.c
#if 0
int re865xIoctl(char *name,uint32 arg0, uint32 arg1, uint32 arg2, uint32 arg3)
{
	uint32 args[4];
	struct ifreq ifr;
	int sockfd;

	args[0] = arg0;
	args[1] = arg1;
	args[2] = arg2;
	args[3] = arg3;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		DEBUG_P("fatal error socket\n");
		return -3;
	}
	strcpy((char*)&ifr.ifr_name, name);
	((uint32 *)(&ifr.ifr_data))[0] = (uint32)args;
	if (ioctl(sockfd, SIOCDEVPRIVATE, &ifr)<0)
	{
		perror("device ioctl:");
		DEBUG_P("\nerror %s line %d\n",__FUNCTION__,__LINE__);
		close(sockfd);
		return -1;
	}
	close(sockfd);
	return 0;
} /* end re865xIoctl */
#endif
// Dino Chang
/*
		run command with argument
 
*/
int runProgram(char *name,char *argv[])
{
	int pid;
	char *env[2];	
	pid =vfork();
	if (pid==0)
	{
		pid=vfork();
		if (pid>0) _exit(0); // second parent; grandparent still alive 
		env[0]="PATH=/bin:/sbin";
		env[1]=NULL;
		execve(name,argv,env);
		_exit(0);
	}
	else
		waitpid(pid,NULL,0);
	return 0;

}

// Dino Chang 2004/12/28 Add for TMSS
int do_wait_exec(char *program, char *argument[]) {
	pid_t pid;
	int status;
	char *env[2];

	switch(pid = fork()) {
		case -1:
			return 0;
		case 0:
			env[0] = "PATH=/bin:/sbin";
			env[1] = NULL;
			execve(program, argument, env);
			_exit(0);
		break;
	}
	waitpid(pid, &status, 0);
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
		return 0;
	}
	return 1;
}
// Dino
int do_exec(char *name, char *identity)
{
	int pid,pid1;
	
	pid =vfork();
	if (pid==0)
	{
		char *av[3];
		char timezone[20];
		char *env[3];
		char tempStr[20]={0};
		FILE *f;
		f=fopen("/etc/TZ","r");
		if(f!=NULL)
		{
			fgets(tempStr,20,f);
			fclose(f);
		}

		pid1=vfork();
		if (pid1>0) _exit(0); // second parent; grandparent still alive 
		
		env[0]="PATH=/bin:/sbin";
		strcpy(timezone,"TZ=std+00:00:00");
		env[1]=timezone;
		env[2]=NULL;
		//memcpy(env[1],tempStr,12);
		av[0]=name;
		av[1]=identity;
		//sprintf(tempStr,"%x",(uint32)pRomeCfgParam);
		av[2]=NULL;
		//av[3]=NULL;
		setsid();
		execve(name,av,env);
		_exit(0);
	}
	else
		waitpid(pid,NULL,0);
		
	//st = WEXITSTATUS(stat);
	return 0;
} /* end do_exec */

int do_kill(char *filename)
{
	int pid=0;
	FILE *f;
	f=fopen(filename,"r");
	if(f!=NULL)
	{
		fscanf(f,"%d",&pid);

		kill(pid,SIGTERM);
		kill(pid,SIGQUIT);
		sleep(1);
		kill(pid,SIGKILL);
		fclose(f);
		remove(filename);
		return 0;
	}
	return -1;	
}



#define IFREQ_OFFSETOF(x)  offsetof(struct ifreq, x)
int setInterfaceIp(char *name,int ip, int netmask)
{
	struct ifreq ifr;
	unsigned char Str[20];
	struct sockaddr_in sai;
	unsigned char	*v=(char*)&ip;
	int net_fd;

	if ((net_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		DEBUG_P("fatal error socket\n");
	}
	strcpy((char*)&ifr.ifr_name, name);
	sprintf(Str,"%u.%u.%u.%u",v[0],v[1],v[2],v[3]);
	sai.sin_family = AF_INET;
	sai.sin_addr.s_addr = INADDR_ANY;
	sai.sin_port = 0;
	if(inet_aton(Str, &sai.sin_addr) == 0) 
	{
		DEBUG_P("not dotted\n");
	}
	memcpy((char*)&ifr+IFREQ_OFFSETOF(ifr_addr),(char*)&sai,
		       sizeof(struct sockaddr));
	if (ioctl(net_fd, SIOCSIFADDR, &ifr) < 0) 
		DEBUG_P("error ioctl\n");

	v=(char*)&netmask;
	sprintf(Str,"%u.%u.%u.%u",v[0],v[1],v[2],v[3]);
	if(inet_aton(Str, &sai.sin_addr) == 0) 
	{
		DEBUG_P("not dotted\n");
	}
	memcpy((char*)&ifr+IFREQ_OFFSETOF(ifr_netmask),(char*)&sai,
		       sizeof(struct sockaddr));
	if (ioctl(net_fd, SIOCSIFNETMASK, &ifr) < 0) 
		DEBUG_P("error2 ioctl\n");


	if (ioctl(net_fd, SIOCGIFFLAGS, &ifr) < 0) 
		DEBUG_P("SIOCGIFFLAGS"); 
	ifr.ifr_flags=IFF_UP|IFF_RUNNING|IFF_MULTICAST;
	if (ioctl(net_fd, SIOCSIFFLAGS, &ifr) < 0) 
		DEBUG_P("SIOCSIFFLAGS"); 
	close(net_fd);
	return 0;
}	

int upInterface(char *name)
{
	struct ifreq ifr;
	//struct sockaddr_in sai;
	int net_fd;
	if ((net_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		DEBUG_P("fatal error socket\n");
	}
	strcpy((char*)&ifr.ifr_name, name);
	if (ioctl(net_fd, SIOCGIFFLAGS, &ifr) < 0) 
		DEBUG_P("SIOCGIFFLAGS"); 
	ifr.ifr_flags=IFF_UP|IFF_RUNNING|IFF_MULTICAST;
	if (ioctl(net_fd, SIOCSIFFLAGS, &ifr) < 0) 
		DEBUG_P("SIOCSIFFLAGS"); 
	close(net_fd);
	return 0;
}	

uint32 board_checkSubnetOverlapping(ipaddr_t ipAddr, ipaddr_t ipMask)
{
	#define WAN_INTERFACE_NUM  0
	
	int i;
	uint8 gipAddr[4], gnetMask[4];

	printf("=> Check subnet overlapping .... ");

	gipAddr[0] = (uint8)((uint32)ipAddr >> 24);
	gipAddr[1] = (uint8)(((uint32)ipAddr & 0x00ff0000) >> 16);
	gipAddr[2] = (uint8)(((uint32)ipAddr & 0x0000ff00) >> 8);
	gipAddr[3] = (uint8)(((uint32)ipAddr & 0x000000ff));

	gnetMask[0] = (uint8)((uint32)ipMask >> 24);
	gnetMask[1] = (uint8)(((uint32)ipMask & 0x00ff0000) >> 16);
	gnetMask[2] = (uint8)(((uint32)ipMask & 0x0000ff00) >> 8);
	gnetMask[3] = (uint8)((uint32)ipMask & 0x000000ff);

 
	for ( i = 0 ; i < MAX_IP_INTERFACE ; i ++ )
	{
		int arrIdx, j;
		uint8 bmask;
		if ( i == WAN_INTERFACE_NUM )	/* same as WAN */
			continue;
		if ( pRomeCfgParam->ifCfgParam[i].valid )
		{
			for (arrIdx = 0; arrIdx < 4 ; arrIdx ++)
			{
				for (j = 0 ; j < 8 ; j ++)
				{
					uint8 ip[2], msk[2];

					bmask = 1<<j;	/* bitmask in current Array entry */

					ip[0] = pRomeCfgParam->ifCfgParam[i].ipAddr[arrIdx] & bmask;
					msk[0] = pRomeCfgParam->ifCfgParam[i].ipMask[arrIdx] & bmask;
					ip[1] = gipAddr[arrIdx] & bmask;
					msk[1] = gnetMask[arrIdx] & bmask;

					if (msk[0] && msk[1] && (ip[0] != ip[1]))
					{
						goto check_next;	/* Subnet is different ! : legal subnet */
					}
				}
			}
			/* subset overlap ! : subnet is illegal ! */
			printf("OverLap! FAIL!\n");
			return 1;	/* overlap found */
		}
check_next:
		continue;
	}
	printf("No Overlap!\n");
	return 0; /* no overlap */
}

void dhcpcDeconfig()
{
	printf("dhcpc client deconfig\n");
	board_ipDownEventTableDriverAccess();
	setInterfaceIp("eth0",0,0);
}

/*
	DHCP Client RENEW
*/
int dhcpcIPRenew()
{
	return 0;
}
int dhcpcIPBound()
{
	FILE *fp;
	int i,dns_idx;
	char	*dns_environment_str,*dns_token,*router_string;
	int 	ipAddr=0,netMask=0,gwAddr=0,ramDNSAddr[2];
	char *args[6];	
	/*************************************************************************************************/
	/******************************** Check the correctness of IP and Subnet ********************************/
#if 0
	if (board_checkSubnetOverlapping(ipAddr, netMask) > 0)
		goto errout;
#endif
 	/*************************************************************************************************/

	/* Check OK: init variables and call ip-down */
	board_ipDownEventTableDriverAccess();
#if 0	
	if (getRomeCfgParamPtr(&pRomeCfgParam))
	{
		return 1; /* fail */
	}
#endif

	fp = fopen("/var/resolv.conf","w+");
	dns_environment_str=getenv("dns");
	dns_token=malloc(MAX_IP_STRING_LEN);
	dns_idx=0;
	if (dns_environment_str)
	while(*(char*)dns_environment_str)
	{
		i=0;
		while (*(char*)dns_environment_str )
		{
			dns_token[i]=*(char*)dns_environment_str;
			i++;
			dns_environment_str++;
			if (*(char*)dns_environment_str==' ')
			{
				dns_environment_str++;
				break;
			}
		}
		
		dns_token[i]='\0';
		if (dns_idx>=2)
			break;
		ramDNSAddr[dns_idx++]=inet_addr(dns_token);
		fprintf(fp,"nameserver %s\n",dns_token);
	}
	
	free(dns_token);
	fprintf(fp,"\n");
	fclose(fp);
	if (getenv("ip"))
	{
		printf("ip  %s\n",getenv("ip"));
		ipAddr=inet_addr(getenv("ip"));
	}
	if (getenv("router"))
	{
		printf("router %s\n",getenv("router"));
		gwAddr=inet_addr(getenv("router"));
	}
	if (getenv("subnet"))
	{
		printf("subnet %s\n",getenv("subnet"));
		netMask=inet_addr(getenv("subnet"));
	}
	setInterfaceIp("eth0",ipAddr, netMask);


	args[0]="default";
	args[1]="gw";
	router_string=malloc(MAX_IP_STRING_LEN);
	sprintf(router_string,"%u.%u.%u.%u",(gwAddr>>24)&0xff,
					(gwAddr>>16)&0xff,
					(gwAddr>>8)&0xff,
					(gwAddr)&0xff);
	args[2]=router_string;
	args[3]=NULL;

	INET_setroute(RTACTION_ADD, 0, args);
	free(router_string);	
	memcpy((void*)&pRomeCfgParam->ifCfgParam[0].ipAddr,(void*)&ipAddr,sizeof(ipAddr));
	memcpy((void*)&pRomeCfgParam->ifCfgParam[0].gwAddr,(void*)&gwAddr,sizeof(gwAddr));
	memcpy((void*)&pRomeCfgParam->ifCfgParam[0].ipMask,(void*)&netMask,sizeof(netMask));
	memcpy((void*)&pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,(void*)&ramDNSAddr[0],sizeof(int));
	memcpy((void*)&pRomeCfgParam->ifCfgParam[0].dnsSecondaryAddr,(void*)&ramDNSAddr[1],sizeof(int));	
	board_ipUpEventTableDriverAccess();
	return 0;

errout:
	udhcpc_renew();	
	return 0;
} /* end dhcpIpBound */



/* 
 * set add route for linux kernel function
 */
static int INET_resolve(char *name, struct sockaddr *sa)
{
	struct sockaddr_in *s_in = (struct sockaddr_in *)sa;

	s_in->sin_family = AF_INET;
	s_in->sin_port = 0;

	/* Default is special, meaning 0.0.0.0. */
	if (strcmp(name, "default")==0) {
		s_in->sin_addr.s_addr = INADDR_ANY;
		return 1;
	}
	/* Look to see if it's a dotted quad. */
	if (inet_aton(name, &s_in->sin_addr)) {
		return 0;
	}
	/* guess not.. */
	return -1;
}


int INET_setroute(int action, int options, char **args)
{
	struct rtentry rt;
	char target[64], gateway[64] = "NONE", netmask[64] = "default";
	int xflag, isnet;
	int skfd;

	xflag = 0;

	if (strcmp(*args, "-net")==0) {
		xflag = 1;
		args++;
	} else if (strcmp(*args, "-host")==0) {
		xflag = 2;
		args++;
	}
	strncpy(target, *args++, (sizeof target));
	fprintf(stderr,"target %s\n",target);
	/* Clean out the RTREQ structure. */
	memset((char *) &rt, 0, sizeof(struct rtentry));


	if ((isnet = INET_resolve(target, &rt.rt_dst)) < 0) {
		printf("can't resolve %s", target);
		return -1;   /* XXX change to E_something */
	}

	switch (xflag) {
		case 1:
			isnet = 1;
			break;

		case 2:
			isnet = 0;
			break;

		default:
			break;
	}

	/* Fill in the other fields. */
	rt.rt_flags = (RTF_UP | RTF_HOST);
	if (isnet)
		rt.rt_flags &= ~RTF_HOST;

	while (*args) {
		if (strcmp(*args, "metric")==0) {
			int metric;

			args++;
			metric = atoi(*args);
#if HAVE_NEW_ADDRT
			rt.rt_metric = metric + 1;
#else
			ENOSUPP("inet_setroute", "NEW_ADDRT (metric)");  /* XXX Fixme */
#endif
			args++;
			continue;
		}

		if (strcmp(*args, "netmask")==0) {
			struct sockaddr mask;

			args++;
			strncpy(netmask, *args, (sizeof netmask));
			if ((isnet = INET_resolve(netmask, &mask)) < 0) {
				error_msg(_("can't resolve netmask %s"), netmask);
				return E_LOOKUP;
			}
			rt.rt_genmask = full_mask(mask);
			args++;
			continue;
		}

		if (strcmp(*args, "gw")==0 || strcmp(*args, "gateway")==0) {
			args++;
			strncpy(gateway, *args, (sizeof gateway));
			if ((isnet = INET_resolve(gateway, &rt.rt_gateway)) < 0) {
				error_msg(_("can't resolve gw %s"), gateway);
				return E_LOOKUP;
			}
			if (isnet) {
				error_msg(
					_("%s: cannot use a NETWORK as gateway!"),
					gateway);
				return E_OPTERR;
			}
			rt.rt_flags |= RTF_GATEWAY;
			args++;
			continue;
		}

		if (strcmp(*args, "mss")==0) {
			args++;
			rt.rt_flags |= RTF_MSS;
			rt.rt_mss = atoi(*args);
			args++;
			if (rt.rt_mss < 64 || rt.rt_mss > 32768) {
				error_msg(_("Invalid MSS."));
				return E_OPTERR;
			}
			continue;
		}

		if (strcmp(*args, "window")==0) {
			args++;
			rt.rt_flags |= RTF_WINDOW;
			rt.rt_window = atoi(*args);
			args++;
			if (rt.rt_window < 128) {
				error_msg(_("Invalid window."));
				return E_OPTERR;
			}
			continue;
		}

		if (strcmp(*args, "irtt")==0) {
			args++;
			args++;
#if HAVE_RTF_IRTT
			rt.rt_flags |= RTF_IRTT;
			rt.rt_irtt = atoi(*(args - 1));
			rt.rt_irtt *= (HZ / 100);       /* FIXME */
#if 0                           /* FIXME: do we need to check anything of this? */
			if (rt.rt_irtt < 1 || rt.rt_irtt > (120 * HZ)) {
				error_msg(_("Invalid initial rtt."));
				return E_OPTERR;
			}
#endif
#else
			ENOSUPP("inet_setroute", "RTF_IRTT"); /* XXX Fixme */
#endif
			continue;
		}

		if (strcmp(*args, "reject")==0) {
			args++;
#if HAVE_RTF_REJECT
			rt.rt_flags |= RTF_REJECT;
#else
			ENOSUPP("inet_setroute", "RTF_REJECT"); /* XXX Fixme */
#endif
			continue;
		}
		if (strcmp(*args, "mod")==0) {
			args++;
			rt.rt_flags |= RTF_MODIFIED;
			continue;
		}
		if (strcmp(*args, "dyn")==0) {
			args++;
			rt.rt_flags |= RTF_DYNAMIC;
			continue;
		}
		if (strcmp(*args, "reinstate")==0) {
			args++;
			rt.rt_flags |= RTF_REINSTATE;
			continue;
		}
		if (strcmp(*args, "device")==0 || strcmp(*args, "dev")==0) {
			args++;
			rt.rt_dev = *args++;
			continue;
		}
		/* nothing matches */
		if (!rt.rt_dev) {
			rt.rt_dev = *args++;
		} else {
		}
	}

#if HAVE_RTF_REJECT
	if ((rt.rt_flags & RTF_REJECT) && !rt.rt_dev)
		rt.rt_dev = "lo";
#endif

	/* sanity checks.. */
	if (mask_in_addr(rt)) {
		unsigned long mask = mask_in_addr(rt);
		mask = ~ntohl(mask);
		if ((rt.rt_flags & RTF_HOST) && mask != 0xffffffff) {
			error_msg(
				_("netmask %.8x doesn't make sense with host route"),
				(unsigned int)mask);
			return E_OPTERR;
		}
		if (mask & (mask + 1)) {
			error_msg(_("bogus netmask %s"), netmask);
			return E_OPTERR;
		}
		mask = ((struct sockaddr_in *) &rt.rt_dst)->sin_addr.s_addr;
		if (mask & ~mask_in_addr(rt)) {
			error_msg(_("netmask doesn't match route address"));
			return E_OPTERR;
		}
	}
	/* Fill out netmask if still unset */
	if ((action == RTACTION_ADD) && rt.rt_flags & RTF_HOST)
		mask_in_addr(rt) = 0xffffffff;

	/* Create a socket to the INET kernel. */
	if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return E_SOCK;
	}
	/* Tell the kernel to accept this route. */
	if (action == RTACTION_DEL) {
		if (ioctl(skfd, SIOCDELRT, &rt) < 0) {
			perror("SIOCDELRT");
			close(skfd);
			return E_SOCK;
		}
	} else {
		if (ioctl(skfd, SIOCADDRT, &rt) < 0) {
			perror("SIOCADDRT");
			close(skfd);
			return E_SOCK;
		}
	}

	/* Close the socket. */
	(void) close(skfd);
	return EXIT_SUCCESS;
}




void board_ipUp_dhcp(FILE *fp,ipaddr_t *myIp,ipaddr_t *myIpMask,ipaddr_t *myGw,ipaddr_t *myDns)
{
		int32  ret;
		memcpy((void*)myIp,pRomeCfgParam->ifCfgParam[0].ipAddr,4);
		memcpy((void*)myIpMask,pRomeCfgParam->ifCfgParam[0].ipMask,4);
		memcpy((void*)myGw,pRomeCfgParam->ifCfgParam[0].gwAddr,4);
		memcpy((void*)myDns,pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,4);

		
		ret=rtl8651_addIpIntf("eth0",(ipaddr_t)*myIp,(ipaddr_t)*myIpMask);
		if (ret)
		{
			printf("rtl8651_addIpIntf: ret %d\n",ret);
			fprintf(fp,"rtl8651_addIpIntf: ret %d\n",ret);
		}

		/* set default gateway ip */
		ret=rtl8651_addRoute((ipaddr_t)0,(ipaddr_t)0,(int8*)"eth0",(ipaddr_t)*myGw);
		if (ret)
		{
			printf("rtl8651_addRoute: ret %d\n",ret);
			fprintf(fp,"rtl8651_addRoute(default): ret %d\n",ret);
		}
	
		/* how to determine who is napt? who is nat? */
		ret=rtl8651_addNaptMapping((ipaddr_t)*myIp);
		if (ret)
		{
			printf("rtl8651_addNaptMapping: ret %d\n",ret);
			fprintf(fp,"rtl8651_addNaptMapping: ret %d\n",ret);
		}	

}

void board_ipUp_pppoe(FILE *fp,ipaddr_t *myIp,ipaddr_t *myIpMask,ipaddr_t *myGw,ipaddr_t *myDns)
{
		int32  ret;
		uint16 mySessionId;
		ether_addr_t myMac;
		memcpy((void*)myIp,pRomeCfgParam->ifCfgParam[0].ipAddr,4);
		memcpy((void*)myIpMask,pRomeCfgParam->ifCfgParam[0].ipMask,4);
		memcpy((void*)myGw,pRomeCfgParam->ifCfgParam[0].gwAddr,4);
		memcpy((void*)myDns,pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,4);
		memcpy((void*)&myMac,pRomeCfgParam->pppoeCfgParam[0].svrMac,6);

		mySessionId = (uint16)pRomeCfgParam->pppoeCfgParam[0].sessionId;
		pRomeCfgParam->pppoeCfgParam[0].manualHangup=0; /* clear manual hangup flag */
		
		rtl8651_flushDemandRoute(0);
		{ 
                        // Added By Nick Wang, 12/27/2004
                        /* which port is wan port? */
			uint32 	wanMemberPorts = pRomeCfgParam->vlanCfgParam[0].memberPort;
			uint32  wanPortNum=0;
			fprintf(fp,"wanMemberPorts is 0x%x\n",wanMemberPorts);
			for(wanPortNum=0;wanPortNum<RTL8651_PORT_NUMBER;wanPortNum++){
				if (wanMemberPorts & (1<<wanPortNum)){
					break;
				}
			}
			fprintf(fp,"wan port number is %d\n",wanPortNum);
			ret=rtl8651_setPppoeSessionProperty((uint32)0,(uint16)mySessionId,(ether_addr_t*)&myMac,(uint32)wanPortNum);
		        if (ret)
		        {
			        printf("rtl8651_setPppoeSessionProperty: ret %d\n",ret);
			        fprintf(fp,"rtl8651_setPppoeSessionProperty: ret %d\n",ret);
		        }
                        // Added End
		}
                ret=rtl8651_addIpIntf("eth0",(ipaddr_t)*myIp,(ipaddr_t)0xffffffff);
		if (ret)
		{
			printf("rtl8651_addIpIntf: ret %d\n",ret);
			fprintf(fp,"rtl8651_addIpIntf: ret %d\n",ret);
		}

		ret=rtl8651_bindPppoeSession((ipaddr_t)*myIp,0); /* single pppoe always uses pppObjId=0 */
		if (ret)
		{
			printf("rtl8651_bindPppoeSession: ret %d\n",ret);
			fprintf(fp,"rtl8651_bindPppoeSession: ret %d\n",ret);
		}

		/* how to determine who is napt? who is nat? */
		ret=rtl8651_addNaptMapping((ipaddr_t)*myIp);
		if (ret)
		{
			printf("rtl8651_addNaptMapping: ret %d\n",ret);
			fprintf(fp,"rtl8651_addNaptMapping: ret %d\n",ret);
		}

		/* set time out option */
		if (pRomeCfgParam->pppoeCfgParam[0].silentTimeout>0)
		{
			uint32 sec = pRomeCfgParam->pppoeCfgParam[0].silentTimeout;
			ret=re865xIoctl("eth0",SET_PPPD_IDLE_TIME,0,TRUE,sec);
			if (ret)
			{
				printf("rtl8651_setPppoeSessionHangUp: ret %d\n",ret);
				fprintf(fp,"rtl8651_setPppoeSessionHangUp: ret %d\n",ret);
			}
		}
		
		/* sync what i have got back to romeCfgParam
		 * for single pppoe, the assigned ip/ipMask/gw/dns is saved in ifCfgParam[0].ipAddr/ipMask/gwAddr/dnsAddr
		 */
		pRomeCfgParam->pppoeCfgParam[0].dialState = PPPOECFGPARAM_DIALSTATE_DIALED_SUCCESS;

		//rtl8651_setPppoeMtu(0,pRomeCfgParam->pppoeCfgParam[0].mtu);
		rtl8651_setNetMtu("eth0", pRomeCfgParam->pppoeCfgParam[0].mtu);


}

void board_ipUp_multiplePppoe(FILE *fp,ipaddr_t *myIp,ipaddr_t *myIpMask,ipaddr_t *myGw,ipaddr_t *myDns,uint32 *pppObjId)
{
		int32 i,ret;
		uint16 mySessionId;
		uint32 myLanType=0;
		pppoeCfg_t	*pppoeCfgPtr;
		int32 ip_start,ip_end;
		uint16 port_start,port_end;
		rtl8651_tblDrvPolicyRoute_t tmpRt;
		ether_addr_t myMac;
		uint32 wanPortNum=0; // Added By Nick Wang, 12/27/2004
	
		if(pRomeCfgParam->pppoeCfgParam[*pppObjId].lanType==PPPOECFGPARAM_LANTYPE_NAPT){
			memcpy((void*)myIp,pRomeCfgParam->pppoeCfgParam[*pppObjId].ipAddr,4);
			memcpy((void*)myIpMask,pRomeCfgParam->pppoeCfgParam[*pppObjId].ipMask,4);
			memcpy((void*)myGw,pRomeCfgParam->pppoeCfgParam[*pppObjId].gwAddr,4);
			memcpy((void*)myDns,pRomeCfgParam->pppoeCfgParam[*pppObjId].dnsAddr,4);
			memcpy((void*)&myMac,pRomeCfgParam->pppoeCfgParam[*pppObjId].svrMac,6);
			memcpy((void*)&myLanType,&(pRomeCfgParam->pppoeCfgParam[*pppObjId].lanType),4);
			
			mySessionId = (uint16)pRomeCfgParam->pppoeCfgParam[*pppObjId].sessionId;		
			pRomeCfgParam->pppoeCfgParam[*pppObjId].manualHangup=0; /* clear manual hangup flag */
			pppoeCfgPtr=malloc(sizeof(*pppoeCfgPtr));
			pppoeCfgPtr->defaultSession=pRomeCfgParam->pppoeCfgParam[*pppObjId].defaultSession;
			pppoeCfgPtr->sessionId=mySessionId;
			memcpy(pppoeCfgPtr->svrMac,(void*)&myMac,6);
			memcpy(pppoeCfgPtr->ipAddr,(void*)myIp,4);
			memcpy(pppoeCfgPtr->ipMask,(void*)myIpMask,4);
			memcpy(pppoeCfgPtr->gwAddr,(void*)myGw,4);
			memcpy(&(pppoeCfgPtr->lanType),(void*)&myLanType,4);
			
			{ /* which port is wan port? */
				uint32 	wanMemberPorts = pRomeCfgParam->vlanCfgParam[0].memberPort;
				//uint32  wanPortNum=0;
				//fprintf(fp,"wanMemberPorts is 0x%x\n",wanMemberPorts);
				for(wanPortNum=0;wanPortNum<RTL8651_PORT_NUMBER;wanPortNum++){
					if (wanMemberPorts & (1<<wanPortNum)){
						break;
					}
				}
				fprintf(fp,"wan port number is %d\n",wanPortNum);
			}
			re865xIoctl("eth0",PPPOEUP_CFG,(uint32)pppoeCfgPtr,*pppObjId,wanPortNum/*0*/);
			free(pppoeCfgPtr);

			/* how to determine who is napt? who is nat? */
			if (pRomeCfgParam->pppoeCfgParam[*pppObjId].defaultSession != 1)
			{
				ret=rtl8651_addRoute((ipaddr_t)*myIp,(ipaddr_t)0xffffffff,
					(int8*)"eth0",(ipaddr_t)*pppObjId);
				if (ret)
				{
					printf("rtl8651_addRoute %x: ret %d\n",*myIp,ret);
					fprintf(fp,"rtl8651_addRoute %x: ret %d\n",*myIp,ret);
				}
				//chhuang:
				rtl8651_addNaptMappingExt(*myIp);
				tmpRt.type=STATIC_POLICY_ROUTE|TRIGGER_DSTIP;
				tmpRt.ip_alias=*myIp;
				tmpRt.ip_start=*myGw;
				tmpRt.ip_end=*myGw;
				rtl8651_addPolicyRoute(&tmpRt);
				for (i=0;i<4;i++)
				{
					if (pRomeCfgParam->pppoeCfgParam[*pppObjId].destnetType[i] == PPPOECFGPARAM_DESTNETTYPE_IP)
					{
						if (!pRomeCfgParam->pppoeCfgParam[*pppObjId].destnet[i]) continue;

						parse_ipRange(pRomeCfgParam->pppoeCfgParam[*pppObjId].destnet[i],&ip_start,&ip_end);
						tmpRt.type=STATIC_POLICY_ROUTE|TRIGGER_DSTIP;
						tmpRt.ip_alias=*myIp;
						tmpRt.ip_start=ip_start;
						tmpRt.ip_end=ip_end;
						rtl8651_addPolicyRoute(&tmpRt);
					}
					else if (pRomeCfgParam->pppoeCfgParam[*pppObjId].destnetType[i] ==
						 PPPOECFGPARAM_DESTNETTYPE_TCPPORT)
					{
						parse_portRange( pRomeCfgParam->pppoeCfgParam[*pppObjId].destnet[i],
							&port_start ,&port_end);
						tmpRt.type=STATIC_POLICY_ROUTE|TRIGGER_DSTTCPPORT;
						tmpRt.ip_alias=*myIp;
						tmpRt.trigger.dport.start=port_start;
						tmpRt.trigger.dport.end=port_end;
						rtl8651_addPolicyRoute(&tmpRt);
					}
					else if (pRomeCfgParam->pppoeCfgParam[*pppObjId].destnetType[i] ==
						 PPPOECFGPARAM_DESTNETTYPE_UDPPORT)
					{
						parse_portRange( pRomeCfgParam->pppoeCfgParam[*pppObjId].destnet[i],
							&port_start ,&port_end);
						tmpRt.type=STATIC_POLICY_ROUTE|TRIGGER_DSTUDPPORT;
						tmpRt.ip_alias=*myIp;
						tmpRt.trigger.dport.start=port_start;
						tmpRt.trigger.dport.end=port_end;
						rtl8651_addPolicyRoute(&tmpRt);
					}
				}
				
				pRomeCfgParam->pppoeCfgParam[*pppObjId].dialState = PPPOECFGPARAM_DIALSTATE_DIALED_SUCCESS;
				//acl_tableDriverAccess(); /* set acl, if any */

				#if 0
				/* multiple pppoe = single napt + multiple nat solution */
				ipaddr_t localHostIp;
				memcpy((void*)&localHostIp,pRomeCfgParam->pppoeCfgParam[pppObjId].localHostIp,4);
				ret=rtl8651_addNatMapping((ipaddr_t)*myIp,(ipaddr_t)localHostIp);
				if (ret)
				{
					printf("rtl8651_addNatMapping: ret %d\n",ret);
					fprintf(fp,"rtl8651_addNatMapping: ret %d\n",ret);
				}
				#else
				/* multiple pppoe = multiple napt solution */
				#endif
			}
			
			rtl8651_flushDemandRoute(*pppObjId);
			if (pRomeCfgParam->pppoeCfgParam[*pppObjId].silentTimeout>0)
			{
				uint32 sec = pRomeCfgParam->pppoeCfgParam[*pppObjId].silentTimeout;
				if (pRomeCfgParam->pppoeCfgParam[*pppObjId].defaultSession)
					ret=re865xIoctl("eth0",SET_PPPD_IDLE_TIME,*pppObjId,TRUE,sec);
				else
					ret=re865xIoctl("eth0",SET_SOFTPPPD_IDLE_TIME,*pppObjId,sec,0);
				if (ret)
				{
					printf("rtl8651_setPppoeSessionHangUp: ret %d\n",ret);
					fprintf(fp,"rtl8651_setPppoeSessionHangUp: ret %d\n",ret);
				}
			}

			pRomeCfgParam->pppoeCfgParam[*pppObjId].dialState = PPPOECFGPARAM_DIALSTATE_DIALED_SUCCESS;

			rtl8651_setPppoeMtu(*pppObjId,pRomeCfgParam->pppoeCfgParam[*pppObjId].mtu);
			
		}else {
			/* pRomeCfgParam->pppoeCfgParam[pppObjId].lanType==PPPOECFGPARAM_LANTYPE_UNNUMBERED */
			
			memcpy((void*)myIp,pRomeCfgParam->pppoeCfgParam[*pppObjId].unnumberedIpAddr,4);
			memcpy((void*)myIpMask,pRomeCfgParam->pppoeCfgParam[*pppObjId].unnumberedIpMask,4);
			memcpy((void*)myGw,pRomeCfgParam->pppoeCfgParam[*pppObjId].gwAddr,4);
			memcpy((void*)myDns,pRomeCfgParam->pppoeCfgParam[*pppObjId].dnsAddr,4);
			memcpy((void*)&myMac,pRomeCfgParam->pppoeCfgParam[*pppObjId].svrMac,6);
			memcpy((void*)&myLanType,&(pRomeCfgParam->pppoeCfgParam[*pppObjId].lanType),4);
			mySessionId = (uint16)pRomeCfgParam->pppoeCfgParam[*pppObjId].sessionId;		
			pRomeCfgParam->pppoeCfgParam[*pppObjId].manualHangup=0; /* clear manual hangup flag */
			pppoeCfgPtr=malloc(sizeof(*pppoeCfgPtr));
			pppoeCfgPtr->defaultSession=pRomeCfgParam->pppoeCfgParam[*pppObjId].defaultSession;
			pppoeCfgPtr->sessionId=mySessionId;
			memcpy(pppoeCfgPtr->svrMac,(void*)&myMac,6);
			memcpy(pppoeCfgPtr->ipAddr,(void*)myIp,4);
			memcpy(pppoeCfgPtr->ipMask,(void*)myIpMask,4);
			memcpy(pppoeCfgPtr->gwAddr,(void*)myGw,4);
			memcpy(&(pppoeCfgPtr->lanType),(void*)&myLanType,4);

                        // Added By Nick Wang, 12/27/2004
			{ /* which port is wan port? */
				uint32 	wanMemberPorts = pRomeCfgParam->vlanCfgParam[0].memberPort;
				//uint32  wanPortNum=0;
				//fprintf(fp,"wanMemberPorts is 0x%x\n",wanMemberPorts);
				for(wanPortNum=0;wanPortNum<RTL8651_PORT_NUMBER;wanPortNum++){
					if (wanMemberPorts & (1<<wanPortNum)){
						break;
					}
				}
				fprintf(fp,"wan port number is %d\n",wanPortNum);
			}
                        // Added End
			re865xIoctl("eth0",PPPOEUP_CFG,(uint32)pppoeCfgPtr,(uint32)*pppObjId,wanPortNum/*0*/);
			free(pppoeCfgPtr);

		
			/* how to determine who is napt? who is nat? */
			if (pRomeCfgParam->pppoeCfgParam[*pppObjId].defaultSession == 0){
				ipaddr_t srcSubnet;
				uint32 stub1,stub2;
				#if 0
				ret=rtl8651_addRoute((ipaddr_t)*myIp,(ipaddr_t)*myIpMask,(int8*)"eth0",(ipaddr_t)*pppObjId);
				#else
				/* bug fix for multiple pppoe's unnumbered case, 2004/10/04 */
				ret=rtl8651_addRoute((ipaddr_t)*myIp,(ipaddr_t)0xffffffff,(int8*)"eth0",(ipaddr_t)*pppObjId);
				#endif
				if (ret)
				{
					printf("rtl8651_addRoute %x: ret %d\n",*myIp,ret);
					fprintf(fp,"rtl8651_addRoute %x: ret %d\n",*myIp,ret);
				}
				
				memcpy(&stub1,pRomeCfgParam->pppoeCfgParam[*pppObjId].unnumberedIpAddr,4);
				memcpy(&stub2,pRomeCfgParam->pppoeCfgParam[*pppObjId].unnumberedIpMask,4);
				
				srcSubnet = (ipaddr_t)(stub1 & stub2);/* get subnet value */
				tmpRt.type=STATIC_POLICY_ROUTE|TRIGGER_SRCGLOBALIP;
				tmpRt.ip_alias=*myIp;
				tmpRt.ip_start=srcSubnet+1;
				tmpRt.ip_end=srcSubnet+(~stub2);
				if ((ret=rtl8651_addPolicyRoute(&tmpRt)) == SUCCESS) {
					{
					uint8 * pUint8=&(tmpRt.ip_start);
					printf("rtl8651_addPolicyRoute->ip_start: %u.%u.%u.%u\n",pUint8[0],pUint8[1],pUint8[2],pUint8[3]);
					fprintf(fp,"rtl8651_addPolicyRoute->ip_start: %u.%u.%u.%u\n",pUint8[0],pUint8[1],pUint8[2],pUint8[3]);
					}
					
					{
						uint8 * pUint8=&(tmpRt.ip_end);
						printf("rtl8651_addPolicyRoute->ip_end  : %u.%u.%u.%u\n",pUint8[0],pUint8[1],pUint8[2],pUint8[3]);
						fprintf(fp,"rtl8651_addPolicyRoute->ip_end  : %u.%u.%u.%u\n",pUint8[0],pUint8[1],pUint8[2],pUint8[3]);
					}
				}
				else {
					printf("rtl8651_addPolicyRoute() fail, retval=%d\n", ret);
					fprintf(fp, "rtl8651_addPolicyRoute() fail, retval=%d\n", ret);
				}
			}
				
			pRomeCfgParam->pppoeCfgParam[*pppObjId].dialState = PPPOECFGPARAM_DIALSTATE_DIALED_SUCCESS;
			rtl8651_flushDemandRoute(*pppObjId);
			if (pRomeCfgParam->pppoeCfgParam[*pppObjId].silentTimeout>0)
			{
				uint32 sec = pRomeCfgParam->pppoeCfgParam[*pppObjId].silentTimeout;
				if (pRomeCfgParam->pppoeCfgParam[*pppObjId].defaultSession)
					ret=re865xIoctl("eth0",SET_PPPD_IDLE_TIME,*pppObjId,TRUE,sec);
				else
					ret=re865xIoctl("eth0",SET_SOFTPPPD_IDLE_TIME,*pppObjId,sec,0);
				if (ret)
				{
					printf("rtl8651_setPppoeSessionHangUp: ret %d\n",ret);
					fprintf(fp,"rtl8651_setPppoeSessionHangUp: ret %d\n",ret);
				}
			}
			pRomeCfgParam->pppoeCfgParam[*pppObjId].dialState = PPPOECFGPARAM_DIALSTATE_DIALED_SUCCESS;
			rtl8651_setPppoeMtu(*pppObjId,pRomeCfgParam->pppoeCfgParam[*pppObjId].mtu);
			acl_tableDriverAccess(); /* set acl, if any */
		}

}

void board_ipUp_unnumberedPppoe(FILE *fp,ipaddr_t *myIp,ipaddr_t *myIpMask,ipaddr_t *myGw,ipaddr_t *myDns)
{
		int32  ret;
		uint16   mySessionId;
		ipaddr_t myIp2;
		ether_addr_t myMac;
		rtl8651_tblDrvPolicyRoute_t tmpRt;

		
		memcpy((void*)myIp,pRomeCfgParam->ifCfgParam[0].ipAddr,4);
		memcpy((void*)myIpMask,pRomeCfgParam->ifCfgParam[0].ipMask,4);
		memcpy((void*)myGw,pRomeCfgParam->ifCfgParam[0].gwAddr,4);
		memcpy((void*)myDns,pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,4);
		memcpy((void*)&myMac,pRomeCfgParam->pppoeCfgParam[0].svrMac,6);

		mySessionId = (uint16)pRomeCfgParam->pppoeCfgParam[0].sessionId;
		pRomeCfgParam->pppoeCfgParam[0].manualHangup=0; /* clear manual hangup flag */
		
                #if 0
		/* to fix the wan port not auto detect problem */
		ret=rtl8651_setPppoeSessionProperty((uint32)0,(uint16)mySessionId,(ether_addr_t*)&myMac,(uint32)0);
		if (ret)
		{
			printf("rtl8651_setPppoeSessionProperty: ret %d\n",ret);
			fprintf(fp,"rtl8651_setPppoeSessionProperty: ret %d\n",ret);
		}
		#else
		{ /* which port is wan port? */
			uint32 	wanMemberPorts = pRomeCfgParam->vlanCfgParam[0].memberPort;
			uint32  wanPortNum=0;
			fprintf(fp,"wanMemberPorts is 0x%x\n",wanMemberPorts);
			for(wanPortNum=0;wanPortNum<RTL8651_PORT_NUMBER;wanPortNum++){
				if (wanMemberPorts & (1<<wanPortNum)){
					break;
				}
			}
			fprintf(fp,"wan port number is %d\n",wanPortNum);
			ret=rtl8651_setPppoeSessionProperty((uint32)0,(uint16)mySessionId,(ether_addr_t*)&myMac,(uint32)wanPortNum);
			if (ret)
			{
				printf("rtl8651_setPppoeSessionProperty: ret %d\n",ret);
				fprintf(fp,"rtl8651_setPppoeSessionProperty: ret %d\n",ret);
			}
		}		
		#endif

		memcpy((void*)myIpMask,pRomeCfgParam->pppoeCfgParam[0].unnumberedIpMask,4);
		memcpy((void*)&myIp2,pRomeCfgParam->pppoeCfgParam[0].unnumberedIpAddr,4);
		if (*myIp!=myIp2)
		{
			ret=rtl8651_delIpIntf("eth1",(ipaddr_t)myIp2,(ipaddr_t)*myIpMask);
			if (ret)
			{
				printf("rtl8651_delIpIntf: ret %d\n",ret);
				fprintf(fp,"rtl8651_delIpIntf: ret %d\n",ret);
			}
			ret=rtl8651_addIpIntf("eth1",(ipaddr_t)*myIp,(ipaddr_t)*myIpMask);
			if (ret)
			{
				printf("rtl8651_addIpIntf: ret %d\n",ret);
				fprintf(fp,"rtl8651_addIpIntf: ret %d\n",ret);
			}
		}else{
			#if 0
			/* don't know which one is better yet?? 2003/12/26 orlando
			 *  1. add again and let it fail
			 *  2. don't add again
			 * 2003/12/31: add again seems to be correct. but
			 * needs more testing to back me up!
			 */
			ret=rtl8651_addIpIntf("eth1",(ipaddr_t)*myIp,(ipaddr_t)*myIpMask);
			if (ret)
			{
				printf("rtl8651_addIpIntf: ret %d\n",ret);
				fprintf(fp,"rtl8651_addIpIntf: ret %d\n",ret);
			}
			#endif
		}
		
		/* clear ip alias of protocol stack's lan interface */
		{
			char	*av[5];
			char	pname[]="/bin/ifconfig";
			av[0]=pname;
			av[1]="eth1:1";
			av[2]="0.0.0.0";
			//av[3]="netmask";
			//av[4]=ipmask_string;
			av[3]=NULL;
			runProgram(pname,av);
		}

		ret=rtl8651_setLanSideExternalIpInterface("eth1",(ipaddr_t)*myIp,(ipaddr_t)*myIpMask,1/*true*/);
		if (ret)
		{
			printf("rtl8651_setLanSideExternalIpInterface: ret %d\n",ret);
			fprintf(fp,"rtl8651_setLanSideExternalIpInterface: ret %d\n",ret);
		}

		ret=rtl8651_bindPppoeSession((ipaddr_t)*myIp,0); /* unnumberd pppoe always uses pppoeCfgParam[0] */
		if (ret)
		{
			printf("rtl8651_bindPppoeSession: ret %d\n",ret);
			fprintf(fp,"rtl8651_bindPppoeSession: ret %d\n",ret);
		}

		if (pRomeCfgParam->pppoeCfgParam[0].silentTimeout>0)
		{
			uint32 sec = pRomeCfgParam->pppoeCfgParam[0].silentTimeout;
			ret=re865xIoctl("eth0",SET_PPPD_IDLE_TIME,0,TRUE,sec);
			if (ret)
			{
				printf("rtl8651_setPppoeSessionHangUp: ret %d\n",ret);
				fprintf(fp,"rtl8651_setPppoeSessionHangUp: ret %d\n",ret);
			}
		}

#if 0		
		ret=rtl8651_addRoute((ipaddr_t)0,(ipaddr_t)0,(int8*)"eth0",(ipaddr_t)0);
		if (ret)
		{
			printf("rtl8651_addRoute(default): ret %d\n",ret);
			fprintf(fp,"rtl8651_addRoute(default): ret %d\n",ret);
		}
#endif		

		if(pRomeCfgParam->pppoeCfgParam[0].unnumberedNapt==1)
		{
			ret=rtl8651_addNaptMapping((ipaddr_t)*myIp);
			if (ret)
			{
				printf("rtl8651_addNaptMapping: ret %d\n",ret);
				fprintf(fp,"rtl8651_addNaptMapping: ret %d\n",ret);
			}
		}

		/* 2005.3.7 */
		/* When we dial up and get IP address successfully, we should add a policy route for outbound packets. */
		/* If we don't add this policy route, packets trapped to CPU cannot find the route to WAN. */
		/* See function '_rtl8651_l34PacketProcess()' for reference. */
		tmpRt.type=STATIC_POLICY_ROUTE|TRIGGER_DSTIP;
		tmpRt.ip_alias=*myIp;
		tmpRt.ip_start=0;
		tmpRt.ip_end=0xffffffff;
		ret = rtl8651_addPolicyRoute(&tmpRt);
		if(ret != SUCCESS) {
			printf("addPolicyRoute failed -- ret: %d\n", ret);
			return NOT_OK;
		}


		/* add global assigned network route to user protocol stack */
		{
			char *argv[6];
			uint8 networkRoute[4];
			uint8 netStr[20];
			uint8 netmaskStr[20];
			uint32 i;
		
			memcpy((void*)networkRoute,(void*)pRomeCfgParam->ifCfgParam[0].ipAddr,4);
			for (i=0;i<4;i++)
				networkRoute[i] &= pRomeCfgParam->ifCfgParam[0].ipMask[i];
				
			argv[0]="-net";
			
			//argv[1]="239.0.0.0"; // pRomeCfgParam->pppoeCfgParam[0].ipAddr
			sprintf(netStr,"%u.%u.%u.%u",
				networkRoute[0],
				networkRoute[1],
				networkRoute[2],
				networkRoute[3]);
			argv[1]=netStr;
			
			argv[2]="netmask";
			
			//argv[3]="255.0.0.0"; // pRomeCfgParam->pppoeCfgParam[0].ipMask
			sprintf(netmaskStr,"%u.%u.%u.%u",
				pRomeCfgParam->ifCfgParam[0].ipMask[0],
				pRomeCfgParam->ifCfgParam[0].ipMask[1],
				pRomeCfgParam->ifCfgParam[0].ipMask[2],
				pRomeCfgParam->ifCfgParam[0].ipMask[3]);
			argv[3]=netmaskStr;
			
			argv[4]="eth1";
			argv[5]=NULL;
								
			INET_setroute(RTACTION_ADD, 0, argv);
		} /* end block add user protocol stack network route */
			
		/* now update cfg values back to romeCfgParam
		 * for single pppoe, the assigned ip/ipMask/gw/dns is saved in ifCfgParam[0].ipAddr/ipMask/gwAddr/dnsAddr
		 */
		pRomeCfgParam->pppoeCfgParam[0].dialState = PPPOECFGPARAM_DIALSTATE_DIALED_SUCCESS;

		rtl8651_setPppoeMtu(0,pRomeCfgParam->pppoeCfgParam[0].mtu);

}
#ifdef CONFIG_RTL865XB_3G
void board_ipUp_3g(FILE *fp,ipaddr_t *myIp,ipaddr_t *myIpMask,ipaddr_t *myGw,ipaddr_t *myDns,unsigned char *bdinfo_mac)
{
		int32  ret;
		int i;
		char tempStr[64]={0};
		ether_addr_t myMac;


#if 1
		
		memcpy(myMac.octet,bdinfo_mac,6);

		// eth0's mac = bdinfo's mac + 1
		myMac.octet[5]++;

		memcpy((void*)myIp,pRomeCfgParam->ifCfgParam[0].ipAddr,4);
		//memcpy((void*)myIpMask,pRomeCfgParam->ifCfgParam[0].ipMask,4);
		//memcpy((void*)myGw,pRomeCfgParam->ifCfgParam[0].gwAddr,4);
		//memcpy((void*)myDns,pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,4);		

		sprintf(tempStr,"ifconfig eth0 %u.%u.%u.%u",pRomeCfgParam->ifCfgParam[0].ipAddr[0],pRomeCfgParam->ifCfgParam[0].ipAddr[1]
											   ,pRomeCfgParam->ifCfgParam[0].ipAddr[2],pRomeCfgParam->ifCfgParam[0].ipAddr[3]);
		system(tempStr);
		fprintf(fp,"%s\n",tempStr);

		system("route del default");
		system("ifconfig eth0 netmask 255.255.255.248");
		
		// Need to be removed
		//send_signal("/var/run/priority.pid",9);

		// spoof ip mask for romeDriver( 255.255.255.255-->255.255.255.248)
		*myIpMask=0xfffffff8;

		ret=rtl8651_addIpIntf("eth0",(ipaddr_t)*myIp,(ipaddr_t)*myIpMask);
		if (ret)
		{
			printf("rtl8651_addIpIntf: ret %d\n",ret);
			fprintf(fp,"rtl8651_addIpIntf: ret %d\n",ret);
		}

		// mac for ppp0 recv from eth0 to wan. (src mac = dest mac )
		// 0: for network, 7: for broadcast
		for(i=1;i<7;i++)
		{
			if((((pRomeCfgParam->ifCfgParam[0].ipAddr[3]&0xf8)+i)!=pRomeCfgParam->ifCfgParam[0].ipAddr[3]))
			{
				sprintf(tempStr,"arp -i eth0 -s %u.%u.%u.%u %02x:%02x:%02x:%02x:%02x:%02x"
					,pRomeCfgParam->ifCfgParam[0].ipAddr[0],pRomeCfgParam->ifCfgParam[0].ipAddr[1]
					,pRomeCfgParam->ifCfgParam[0].ipAddr[2],((pRomeCfgParam->ifCfgParam[0].ipAddr[3]&0xf8)+i)
					,myMac.octet[0],myMac.octet[1],myMac.octet[2],myMac.octet[3],myMac.octet[4],myMac.octet[5]);		
				system(tempStr);
				fprintf(fp,"%s\n",tempStr);
		}
		}
		
		for(i=1;i<7;i++)
		{

			if((((pRomeCfgParam->ifCfgParam[0].ipAddr[3]&0xf8)+i)!=pRomeCfgParam->ifCfgParam[0].ipAddr[3]))
			{
				unsigned char temp;

				temp= pRomeCfgParam->ifCfgParam[0].ipAddr[3];
				pRomeCfgParam->ifCfgParam[0].ipAddr[3]=((pRomeCfgParam->ifCfgParam[0].ipAddr[3]&0xf8)+i);
				// add default route for Protocol-Stack
				sprintf(tempStr,"route add default gw %u.%u.%u.%u eth0",pRomeCfgParam->ifCfgParam[0].ipAddr[0],pRomeCfgParam->ifCfgParam[0].ipAddr[1]
						 									   	 ,pRomeCfgParam->ifCfgParam[0].ipAddr[2],((pRomeCfgParam->ifCfgParam[0].ipAddr[3]&0xf8)+i));
				system(tempStr);

				memcpy((void*)myIp,pRomeCfgParam->ifCfgParam[0].ipAddr,4);
				
				// add default route for RomeDrvier (myIp = virtual gateway's IP)
				ret=rtl8651_addRoute((ipaddr_t)0,(ipaddr_t)0,(int8*)"eth0",*myIp);
		if (ret)	
		{
			printf("rtl8651_addRoute: ret %d\n",ret);
			fprintf(fp,"rtl8651_addRoute: ret %d\n",ret);			
		}

				// Register Virtual Gateway's ARP from RomeDriver (Virtual Gateway's MAC = eth0's MAC + 1 = bdinfo_mac + 2)
				myMac.octet[5]++;
				ret=rtl8651_addArp(*myIp,&myMac,"eth0",CONFIG_RTL865XB_3G_PORT);
				if(ret)
				{
					printf("rtl8651_addArp: ret %d\n",ret);
					fprintf(fp,"rtl8651_addArp: ret %d\n",ret);			
				}

				// recover myIp to real WAN ip (eth0/ppp0)
				pRomeCfgParam->ifCfgParam[0].ipAddr[3]=temp;
				memcpy((void*)myIp,pRomeCfgParam->ifCfgParam[0].ipAddr,4);

				break;
			}
		}
		

		// add wan-side NAPT ip address (eth0/ppp0)
		ret=rtl8651_addNaptMapping((ipaddr_t)*myIp);
		if (ret)
		{
			printf("rtl8651_addNaptMapping(%u.%u.%u.%u): ret %d\n",NIPQUAD(*myIp),ret);
			fprintf(fp,"rtl8651_addNaptMapping(%u.%u.%u.%u): ret %d\n",NIPQUAD(*myIp),ret);
		}

#endif


		fprintf(fp,"3g ip-up success!\n");

}
#endif
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
void board_ipUp_pptp(FILE *fp,ipaddr_t *myIp,ipaddr_t *myIpMask,ipaddr_t *myGw,ipaddr_t *myDns)
{
		int32  ret;
		pRomeCfgParam->pptpCfgParam.dialState=PPTPCFGPARAM_DIALSTATE_DIALED_SUCCESS;		
		memcpy((void*)myIp,pRomeCfgParam->ifCfgParam[0].ipAddr,4);
		memcpy((void*)myIpMask,pRomeCfgParam->ifCfgParam[0].ipMask,4);
		memcpy((void*)myGw,pRomeCfgParam->ifCfgParam[0].gwAddr,4);
		memcpy((void*)myDns,pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,4);		

		rtl8651_flushDemandRoute(4); //for pptp

		ret=rtl8651_addIpIntf("eth0",(ipaddr_t)*myIp,(ipaddr_t)*myIpMask);
		//fprintf(fp,"rtl8651_addIpIntf: ip=%x mask=%x\n",myIp,myIpMask);
		if (ret)
		{
			printf("rtl8651_addIpIntf: ret %d\n",ret);
			fprintf(fp,"rtl8651_addIpIntf: ret %d\n",ret);
		}
		ret = rtl8651_setPptpProperty(1, 0, 0, *(uint32*)(&(pRomeCfgParam->pptpCfgParam.svAddr)));
		if (ret)
		{
			printf("rtl8651_setPptpProperty: ret %d\n",ret);
			fprintf(fp,"rtl8651_setPptpProperty: ret %d\n",ret);
		}
		
		ret=rtl8651_bindSession(*myIp, 1); // use myIp to separate sessions.
		if (ret)
		{
			printf("rtl8651_bindSession: ret %d\n",ret);
			fprintf(fp,"rtl8651_bindSession: ret %d\n",ret);
		}
	
		// ret=rtl8651_addNaptMapping((ipaddr_t)myIp);
				
		ret=rtl8651_addNaptMapping((ipaddr_t)*myIp);
		if (ret)
		{
			printf("rtl8651_addNaptMapping(%u.%u.%u.%u): ret %d\n",NIPQUAD(*myIp),ret);
			fprintf(fp,"rtl8651_addNaptMapping(%u.%u.%u.%u): ret %d\n",NIPQUAD(*myIp),ret);
		}
		
		/* set time out option */
		if (pRomeCfgParam->pptpCfgParam.silentTimeout>0)
		{		
			ret=re865xIoctl("eth0",SET_PPPD_IDLE_TIME,0,TRUE,pRomeCfgParam->pptpCfgParam.silentTimeout);
			if (ret)
			{
				printf("rtl8651_setPppoeSessionHangUp: ret %d\n",ret);
				fprintf(fp,"rtl8651_setPppoeSessionHangUp: ret %d\n",ret);
			}
		}
		
}

void board_ipUp_l2tp(FILE *fp,ipaddr_t *myIp,ipaddr_t *myIpMask,ipaddr_t *myGw,ipaddr_t *myDns)
{
		int32  ret;
		pRomeCfgParam->l2tpCfgParam.dialState=L2TPCFGPARAM_DIALSTATE_DIALED_SUCCESS;
		
		memcpy((void*)myIp,pRomeCfgParam->ifCfgParam[0].ipAddr,4);
		memcpy((void*)myIpMask,pRomeCfgParam->ifCfgParam[0].ipMask,4);
		memcpy((void*)myGw,pRomeCfgParam->ifCfgParam[0].gwAddr,4);
		memcpy((void*)myDns,pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,4);

		rtl8651_flushDemandRoute(5); //for l2tp

		ret=rtl8651_addRoute((ipaddr_t)0,(ipaddr_t)0,(int8*)"eth0",0);
		if (ret)
		{
			printf("rtl8651_addRoute: ret %d\n",ret);
			//fprintf(fp,"rtl8651_addRoute: ret %d\n",ret);
		}		
		
		ret=rtl8651_addIpIntf("eth0",(ipaddr_t)*myIp,(ipaddr_t)*myIpMask);
		if (ret)
		{
			printf("rtl8651_addIpIntf:ip=%u.%u.%u.%u:mask=%u.%u.%u.%u: ret %d\n",NIPQUAD(myIp),NIPQUAD(myIpMask),ret);
			fprintf(fp,"rtl8651_addIpIntf:ip=%u.%u.%u.%u:mask=%u.%u.%u.%u: ret %d\n",NIPQUAD(myIp),NIPQUAD(myIpMask),ret);
		}
		
		ret = rtl8651_setL2tpProperty(1, 0, 0, *(uint32*)(&(pRomeCfgParam->l2tpCfgParam.svAddr)));
		if (ret)
		{
			printf("rtl8651_setL2tpProperty: ret %d\n",ret);
			fprintf(fp,"rtl8651_setL2tpProperty: ret %d\n",ret);
		}
		
		ret=rtl8651_bindSession(*myIp, 1); // use myIp to separate sessions.
		if (ret)
		{
			printf("rtl8651_bindSession: ret %d\n",ret);
			fprintf(fp,"rtl8651_bindSession: ret %d\n",ret);
		}
	
		ret=rtl8651_addNaptMapping((ipaddr_t)*myIp);
		if (ret)
		{
			printf("rtl8651_addNaptMapping(%u.%u.%u.%u): ret %d\n",NIPQUAD(*myIp),ret);
			fprintf(fp,"rtl8651_addNaptMapping(%u.%u.%u.%u): ret %d\n",NIPQUAD(*myIp),ret);
		}

		/* set time out option */
		if (pRomeCfgParam->l2tpCfgParam.silentTimeout>0)
		{		
			ret=re865xIoctl("eth0",SET_PPPD_IDLE_TIME,0,TRUE,pRomeCfgParam->l2tpCfgParam.silentTimeout);
			if (ret)
			{
				printf("rtl8651_setPppoeSessionHangUp: ret %d\n",ret);
				fprintf(fp,"rtl8651_setPppoeSessionHangUp: ret %d\n",ret);
			}
		}	

}
int32 board_ipUp_dhcpL2tp_init(FILE *fp,ipaddr_t *myIp,ipaddr_t *myIpMask,ipaddr_t *myGw,ipaddr_t *myDns)
{
		int8 strBuffer[128];			
		uint32 netmask=0;	
		int ret;			
		#if 0
				pRomeCfgParam->ifCfgParam[0].gwAddr[0]=192;
				pRomeCfgParam->ifCfgParam[0].gwAddr[1]=168;
				pRomeCfgParam->ifCfgParam[0].gwAddr[2]=154;
				pRomeCfgParam->ifCfgParam[0].gwAddr[3]=240;
				pRomeCfgParam->ifCfgParam[0].ipMask[0]=255;
				pRomeCfgParam->ifCfgParam[0].ipMask[1]=255;
				pRomeCfgParam->ifCfgParam[0].ipMask[2]=255;
				pRomeCfgParam->ifCfgParam[0].ipMask[3]=0;
		#endif

				if(memcmp(pRomeCfgParam->ifCfgParam[0].ipMask,&netmask,4)==0)
				{
					printf("the DHCP server may be BOOTP Server!\n");
					return -1;
				}
		
				memcpy((void*)myIp,pRomeCfgParam->ifCfgParam[0].ipAddr,4);
				memcpy((void*)myIpMask,pRomeCfgParam->ifCfgParam[0].ipMask,4);
				memcpy((void*)myGw,pRomeCfgParam->ifCfgParam[0].gwAddr,4);				
				memcpy((void*)myDns,pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,4);
				
				memcpy(pRomeCfgParam->l2tpCfgParam.ipAddr, (void*)myIp, 4);
				memcpy(pRomeCfgParam->l2tpCfgParam.ipMask, (void*)myIpMask, 4);
				memcpy(pRomeCfgParam->l2tpCfgParam.svAddr, (void*)myGw, 4);

				netmask|=(pRomeCfgParam->l2tpCfgParam.ipMask[0])<<24;
				netmask|=(pRomeCfgParam->l2tpCfgParam.ipMask[1])<<16;
				netmask|=(pRomeCfgParam->l2tpCfgParam.ipMask[2])<<8;
				netmask|=pRomeCfgParam->l2tpCfgParam.ipMask[3];

				ret=rtl8651_addNetworkIntf("eth0");	
				if(ret)
				{
					printf("rtl8651_addNetworkIntf: ret %d\n",ret);
//					fprintf(fp,"rtl8651_addNetworkIntf: ret %d\n",ret);				
				}

				ret=rtl8651_specifyNetworkIntfLinkLayerType("eth0",LL_L2TP,0x08);
				if (ret)
				{
					printf("rtl8651_specifyNetworkIntfLinkLayerType: ret %d\n",ret);
					fprintf(fp,"rtl8651_specifyNetworkIntfLinkLayerType: ret %d\n",ret);
				}		
		
				//addIpInterface				
				re865xIoctl("eth0",SET_VLAN_IP,*(uint32*)(&(pRomeCfgParam->l2tpCfgParam.ipAddr)),*(uint32*)(&(pRomeCfgParam->l2tpCfgParam.ipMask)),0);

				/* linux if up */
				upInterface("eth0");

				ret=rtl8651_addExtNetworkInterface((int8*)"eth0");	
				if(ret)					
				{
					printf("rtl8651_addExtNetworkInterface: ret %d\n",ret);
					fprintf(fp,"rtl8651_addExtNetworkInterface: ret %d\n",ret);				
				}

				ret=rtl8651_addSession(RTL8651_L2TP, 1, "eth0", 0x08); 
				if (ret)
				{
					printf("rtl8651_addSession: ret %d\n",ret);
					fprintf(fp,"rtl8651_addSession: ret %d\n",ret);
				}				
				
				ret=rtl8651_addRoute((ipaddr_t)0,(ipaddr_t)0,(int8*)"eth0",0);
				if (ret)
				{
					printf("rtl8651_addRoute: ret %d\n",ret);
					fprintf(fp,"rtl8651_addRoute: ret %d\n",ret);
				}				

				sprintf(strBuffer,"/sbin/ifconfig eth0 %d.%d.%d.%d netmask %d.%d.%d.%d",
					pRomeCfgParam->ifCfgParam[0].ipAddr[0],
					pRomeCfgParam->ifCfgParam[0].ipAddr[1],
					pRomeCfgParam->ifCfgParam[0].ipAddr[2],
					pRomeCfgParam->ifCfgParam[0].ipAddr[3],			
					pRomeCfgParam->ifCfgParam[0].ipMask[0],
					pRomeCfgParam->ifCfgParam[0].ipMask[1],
					pRomeCfgParam->ifCfgParam[0].ipMask[2],
					pRomeCfgParam->ifCfgParam[0].ipMask[3]
				);
				system(strBuffer);
			
				printf("DHCP Finish! IP:%u.%u.%u.%u Server IP:%u.%u.%u.%u\n",NIPQUAD(*myIp),NIPQUAD(*myGw));
				fprintf(fp, "DHCP Finish! IP:%u.%u.%u.%u Server IP:%u.%u.%u.%u\n",NIPQUAD(*myIp),NIPQUAD(*myGw));
				pRomeCfgParam->l2tpCfgParam.dialState=L2TPCFGPARAM_DIALSTATE_OFF;

				return 0;				
}

void board_ipUp_dhcpL2tp(FILE *fp,ipaddr_t *myIp,ipaddr_t *myIpMask,ipaddr_t *myGw,ipaddr_t *myDns)
{
		int32  ret;
		if (pRomeCfgParam->l2tpCfgParam.dialState==L2TPCFGPARAM_DIALSTATE_DIALED_DHCP_DISCOVER)
			{
				fprintf(fp, "DHCP OK......\n");
				ret=board_ipUp_dhcpL2tp_init(fp,myIp,myIpMask,myGw,myDns);
				if(ret) //dhcp client not get IP info.
				{
					udhcpc_start();
					return;
				}
				else
				{					
					pRomeCfgParam->l2tpCfgParam.dialState=L2TPCFGPARAM_DIALSTATE_OFF;				
					l2tp_start(0);
				}
			}
			else 
			{
				fprintf(fp, "L2TP OK......\n");
				board_ipUp_l2tp(fp,myIp,myIpMask,myGw,myDns);
			}	
}
#endif

/*
 * Routine Name: board_ipUpEventTableDriverAccess
 * Description: when a wan interface got new ipAddr/ipMask/gwAddr/dnsAddr
 *              call this function to access rtl865x table driver APIs to
 *              configure rtl865x
 *              scenarios: dhcp client ip up event, pppoe ip up event
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 board_ipUpEventTableDriverAccess(void)
{
	FILE   *fp;
	char   tempStr[30];
	int32  ret;
	unsigned char mac[6];
	uint32 pppObjId=0;
	ipaddr_t myIp;
	ipaddr_t myIpMask;
	ipaddr_t myGw;
	ipaddr_t myDns;
	//rtl8651_sem_wait(SEMAPHORE_IPEVENT);

	switch(pRomeCfgParam->ifCfgParam[0].connType)
	{
	case IFCFGPARAM_CONNTYPE_DHCPC:
		sprintf(tempStr,"/var/dhcpup.txt");
		break;
	case IFCFGPARAM_CONNTYPE_PPPOE:
		sprintf(tempStr,"/var/pppoeup.txt");
		break;		
	case IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED:
		sprintf(tempStr,"/var/unnumup.txt");
		break;
	case IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE:
		pppObjId = pRomeCfgParam->pppoeCfgParam[0].whichPppObjId;
		sprintf(tempStr,"/var/pppoeup%u.txt",pppObjId);
		break;
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)	
	case IFCFGPARAM_CONNTYPE_PPTP:
		sprintf(tempStr,"/var/pptpup.txt");
		break;
	case IFCFGPARAM_CONNTYPE_L2TP:
		sprintf(tempStr,"/var/l2tpup.txt");
		break;
	case IFCFGPARAM_CONNTYPE_DHCPL2TP:
		sprintf(tempStr, "/var/dhcpl2tpup.txt");
		break;		
		
#endif	//CONFIG_RTL865X(B)_PPTPL2TP	
#ifdef CONFIG_RTL865XB_3G
	case IFCFGPARAM_CONNTYPE_3G:
		sprintf(tempStr, "/var/3gup.txt");
		break;	
#endif
	default:
		sprintf(tempStr,"/var/unkownup.txt");
	}

	fp = fopen("/var/bdinfo_mac.txt","rb");
	if(fp!=NULL)
	{
		int c;
		fscanf(fp,"%u\n",&c); mac[0]=c&0xff;
		fscanf(fp,"%u\n",&c); mac[1]=c&0xff;
		fscanf(fp,"%u\n",&c); mac[2]=c&0xff;
		fscanf(fp,"%u\n",&c); mac[3]=c&0xff;
		fscanf(fp,"%u\n",&c); mac[4]=c&0xff;
		fscanf(fp,"%u\n",&c); mac[5]=c&0xff;
		fclose(fp);
	}

	
	fp = fopen(tempStr,"r");
	if(fp==NULL)
		fp=fopen(tempStr,"w");
	fclose(fp);
	fp = fopen(tempStr,"a");
	fprintf(fp,"board_ipUpEventTableDriverAccess ...\n");	

	switch (pRomeCfgParam->ifCfgParam[0].connType)
	{
		case IFCFGPARAM_CONNTYPE_DHCPC:
		 	
			fprintf(fp,"connType: dhcpc\n");
			break;

		case IFCFGPARAM_CONNTYPE_PPPOE:

		 	

			fprintf(fp,"connType: pppoe\n");
			fprintf(fp,"servermac:%x:%x:%x:%x:%x:%x\n",
				pRomeCfgParam->pppoeCfgParam[0].svrMac[0],
				pRomeCfgParam->pppoeCfgParam[0].svrMac[1],
				pRomeCfgParam->pppoeCfgParam[0].svrMac[2],
				pRomeCfgParam->pppoeCfgParam[0].svrMac[3],
				pRomeCfgParam->pppoeCfgParam[0].svrMac[4],
				pRomeCfgParam->pppoeCfgParam[0].svrMac[5]);

			break;

		case IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED:
		 	
			fprintf(fp,"connType: unnumbered pppoe\n");
			fprintf(fp,"servermac:%x:%x:%x:%x:%x:%x\n",
				pRomeCfgParam->pppoeCfgParam[0].svrMac[0],
				pRomeCfgParam->pppoeCfgParam[0].svrMac[1],
				pRomeCfgParam->pppoeCfgParam[0].svrMac[2],
				pRomeCfgParam->pppoeCfgParam[0].svrMac[3],
				pRomeCfgParam->pppoeCfgParam[0].svrMac[4],
				pRomeCfgParam->pppoeCfgParam[0].svrMac[5]);
			break;

		case IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE:
			fprintf(fp,"connType: multiple pppoe\n");
			fprintf(fp,"pppObjId: %u\n",pppObjId);
			fprintf(fp,"pppx: %u\n",pRomeCfgParam->pppoeCfgParam[pppObjId].pppx);
			fprintf(fp,"servermac:%x:%x:%x:%x:%x:%x\n",
				pRomeCfgParam->pppoeCfgParam[pppObjId].svrMac[0],
				pRomeCfgParam->pppoeCfgParam[pppObjId].svrMac[1],
				pRomeCfgParam->pppoeCfgParam[pppObjId].svrMac[2],
				pRomeCfgParam->pppoeCfgParam[pppObjId].svrMac[3],
				pRomeCfgParam->pppoeCfgParam[pppObjId].svrMac[4],
				pRomeCfgParam->pppoeCfgParam[pppObjId].svrMac[5]);
			memcpy(pRomeCfgParam->ifCfgParam[0].ipAddr,pRomeCfgParam->pppoeCfgParam[pppObjId].ipAddr,  4);
			memcpy(pRomeCfgParam->ifCfgParam[0].ipMask,pRomeCfgParam->pppoeCfgParam[pppObjId].ipMask,  4);
			memcpy(pRomeCfgParam->ifCfgParam[0].gwAddr,pRomeCfgParam->pppoeCfgParam[pppObjId].gwAddr,  4);
			memcpy(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,pRomeCfgParam->pppoeCfgParam[pppObjId].dnsAddr,4);
			break;
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
		case IFCFGPARAM_CONNTYPE_PPTP:		 	
			fprintf(fp,"connType: pptp\n");
			break;
		case IFCFGPARAM_CONNTYPE_L2TP:
			fprintf(fp,"connType: l2tp\n");
			break;
		case IFCFGPARAM_CONNTYPE_DHCPL2TP:
			fprintf(fp, "connType: DHCP + L2TP\n");
			break;
#endif //CONFIG_RTL865X(B)_PPTPL2TP
#ifdef CONFIG_RTL865XB_3G
		case IFCFGPARAM_CONNTYPE_3G:		 	
			fprintf(fp,"connType: 3G\n");
			break;
#endif
		default:
			break;
	}

	LogDebugInfo(fp);

	
#if 0
	fprintf(fp,"ipAddr:%u.%u.%u.%u\n",
		pRomeCfgParam->ifCfgParam[0].ipAddr[0],
		pRomeCfgParam->ifCfgParam[0].ipAddr[1],
		pRomeCfgParam->ifCfgParam[0].ipAddr[2],
		pRomeCfgParam->ifCfgParam[0].ipAddr[3]);

	fprintf(fp,"ipMask:%u.%u.%u.%u\n",
		pRomeCfgParam->ifCfgParam[0].ipMask[0],
		pRomeCfgParam->ifCfgParam[0].ipMask[1],
		pRomeCfgParam->ifCfgParam[0].ipMask[2],
		pRomeCfgParam->ifCfgParam[0].ipMask[3]);

	fprintf(fp,"gwAddr:%u.%u.%u.%u\n",
		pRomeCfgParam->ifCfgParam[0].gwAddr[0],
		pRomeCfgParam->ifCfgParam[0].gwAddr[1],
		pRomeCfgParam->ifCfgParam[0].gwAddr[2],
		pRomeCfgParam->ifCfgParam[0].gwAddr[3]);

	fprintf(fp,"dnsPrimaryAddr:%u.%u.%u.%u\n",
		pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[0],
		pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[1],
		pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[2],
		pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr[3]);
#endif

	#if defined(CONFIG_RTL865X_DIAG_LED)
	{
		ipaddr_t gwLanIp,gwLanIpmask;
		memcpy((void*)&gwLanIp,pRomeCfgParam->ifCfgParam[1].ipAddr,4);
		memcpy((void*)&gwLanIpmask,pRomeCfgParam->ifCfgParam[1].ipMask,4);
		if ((myIp&myIpMask)==(gwLanIp&gwLanIpmask))
		re865xIoctl("eth0",RTL8651_IOCTL_DIAG_LED,5,0,0);
	}
	#endif


	switch (pRomeCfgParam->ifCfgParam[0].connType)
	{

		case IFCFGPARAM_CONNTYPE_DHCPC:
			board_ipUp_dhcp(fp,&myIp,&myIpMask,&myGw,&myDns);
			break;		
		case IFCFGPARAM_CONNTYPE_PPPOE:
			board_ipUp_pppoe(fp,&myIp,&myIpMask,&myGw,&myDns);
			break;					
		case IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE:
			board_ipUp_multiplePppoe(fp,&myIp,&myIpMask,&myGw,&myDns,&pppObjId);
			break;					
		case IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED:
			board_ipUp_unnumberedPppoe(fp,&myIp,&myIpMask,&myGw,&myDns);
			break;
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)		
		case IFCFGPARAM_CONNTYPE_PPTP:
			board_ipUp_pptp(fp,&myIp,&myIpMask,&myGw,&myDns);
			break;
		case IFCFGPARAM_CONNTYPE_L2TP:
			board_ipUp_l2tp(fp,&myIp,&myIpMask,&myGw,&myDns);
			break;		
		case IFCFGPARAM_CONNTYPE_DHCPL2TP:
			board_ipUp_dhcpL2tp(fp,&myIp,&myIpMask,&myGw,&myDns);
			break;		
#endif // CONFIG_RTL865X(B)_PPTPL2TP
#ifdef CONFIG_RTL865XB_3G
		case IFCFGPARAM_CONNTYPE_3G:
			board_ipUp_3g(fp,&myIp,&myIpMask,&myGw,&myDns,mac);
			break;		
#endif
		default:
		/* don't know what to do now */
			printf("board_ipUpEventTableDriverAccess: unknown ifCfgParam[0].connType(%u)\n",pRomeCfgParam->ifCfgParam[0].connType);
			break;
	}/* switch(pRomeCfgParam->ifCfgParam[0].connType) */



	switch(pRomeCfgParam->ifCfgParam[0].connType)
	{
		case IFCFGPARAM_CONNTYPE_STATIC:
			/* do nothing */
			break;
		case IFCFGPARAM_CONNTYPE_DHCPC: /* fall through */
		case IFCFGPARAM_CONNTYPE_PPTP:  /* fall through */
		case IFCFGPARAM_CONNTYPE_L2TP:  /* fall through */
		case IFCFGPARAM_CONNTYPE_PPPOE: /* fall through */
		case IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED:
		case IFCFGPARAM_CONNTYPE_3G: // Add for 3G case.
//		case IFCFGPARAM_CONNTYPE_DHCPL2TP:
			// Dino Chang 2005/03/07
			// move to serverp_tableDriverAccess
			//rtl8651a_setAlgStatus(0,pRomeCfgParam->algCfgParam[0].algList,pRomeCfgParam->algCfgParam[0].localIp);
			// Dino
			serverp_tableDriverAccess(0);
			dmz_tableDriverAccess(0);
			udp_tableDriverAccess(0);
			rtl8651a_setWanStatus(0,TRUE);
			rtl8651a_setDosStatus(0,((pRomeCfgParam->dosCfgParam[0].enable)?(pRomeCfgParam->dosCfgParam[0].enableItem):0));			
			pbnat_ipup( 0, myIp ); /* Protocol-Based NAT */
			break;
		case IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE:
			rtl8651a_setAlgStatus(pppObjId,pRomeCfgParam->algCfgParam[pppObjId].algList,pRomeCfgParam->algCfgParam[pppObjId].localIp);
			serverp_tableDriverAccess(pppObjId); 
			dmz_tableDriverAccess(pppObjId); /* config dmz host with newly-got ip */
			acl_tableDriverAccess(); /* set acl, if any */			
			rtl8651a_setDosStatus(pppObjId,((pRomeCfgParam->dosCfgParam[pppObjId].enable)?(pRomeCfgParam->dosCfgParam[pppObjId].enableItem):0));
			rtl8651a_setWanStatus(pppObjId,TRUE);			
			pbnat_ipup( pppObjId, myIp ); /* Protocol-Based NAT */
			break;
		
		default:
			break;
	}
	acl_tableDriverAccess();

	// conntion type not multiple pppoe or multiple pppoe's default session.
	if((pRomeCfgParam->ifCfgParam[0].connType!=IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)||
		((pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)&&
		(pRomeCfgParam->pppoeCfgParam[pppObjId].defaultSession == 1)))
	{
		FILE *f;
		f=fopen("/var/run/rdate.run","r");
		if(f==NULL)
		{
			time_start();			
			f=fopen("/var/run/rdate.run","w");
		}
		fclose(f);

		upnp_start();
		ddns_start();
		runDNSProxy();
	}

	fclose(fp);
	dhcps_start();

#ifdef CONFIG_RTL865XB_3G
		//if(pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_3G)
		//{
			//sleep(5);
			//rtl8651_softfwd(1); // temp solution to forced in pure software forward, and been update by ROME 3.6
		//}
#endif	

	return 0;
} /* end board_ipUpEventTableDriverAccess */

/*
 * Routine Name: board_ipDownEventTableDriverAccess
 * Description: when a wan interface got new ipAddr/ipMask/gwAddr/dnsAddr
 *              call this function to access rtl865x table driver APIs to
 *              configure rtl865x
 *              scenarios: dhcp client ip up event, pppoe ip up event
 * Input:  
 *   none
 * Output:
 *   none
 * Return:
 *   OK      if success
 *   NOT_OK  if fail
 * ThreadSafe: don't know yet
 */
uint32 board_ipDownEventTableDriverAccess(void)
{
	FILE   *fp;
	char   tempStr[30];
	int32  i,ret;
	uint32 pppObjId;
	ipaddr_t myIp;
	ipaddr_t myIpMask;
//	romeCfgParam_t * pRomeCfgParam;
	pppoeCfg_t	pppoeCfgPtr;

	//rtl8651_sem_wait(SEMAPHORE_IPEVENT);
#if 0		
	if (getRomeCfgParamPtr(&pRomeCfgParam))
	{
		printf("read pRomeCfgParam failed\n");
		return 1; /* fail */
	}
#endif	

	pppObjId = (uint32)pRomeCfgParam->pppoeCfgParam[0].whichPppObjId;
			
	switch(pRomeCfgParam->ifCfgParam[0].connType)
	{
		case IFCFGPARAM_CONNTYPE_DHCPC:
			sprintf(tempStr,"/var/dhcpdown.txt");
			break;
		case IFCFGPARAM_CONNTYPE_PPPOE:
			sprintf(tempStr,"/var/pppoedown.txt");
			break;
		case IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED:			
			sprintf(tempStr,"/var/unnumdown.txt");
			break;
		case IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE:
			pppObjId = pRomeCfgParam->pppoeCfgParam[0].whichPppObjId;
			sprintf(tempStr,"/var/pppoedown%u.txt",pppObjId);
			break;
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
		case IFCFGPARAM_CONNTYPE_PPTP:
			sprintf(tempStr,"/var/pptpdown.txt");
			break;
		case IFCFGPARAM_CONNTYPE_L2TP:		
			sprintf(tempStr,"/var/l2tpdown.txt");
			break;
		case IFCFGPARAM_CONNTYPE_DHCPL2TP:		
			sprintf(tempStr,"/var/dhcpl2tpdown.txt");			
			break;
#endif			
		default:
			sprintf(tempStr,"/var/unkowndown.txt");
			break;
	}

	fp = fopen(tempStr,"r");
	if(fp==NULL)
		fp=fopen(tempStr,"w");
	fclose(fp);
	fp = fopen(tempStr,"a");
	fprintf(fp,"board_ipDownEventTableDriverAccess ...\n");

	//fp = stderr;

	if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_DHCPC)
	{
		/* dhcp client renew case */
		rtl8651a_setWanStatus(0,FALSE);		
		pRomeCfgParam->pppoeCfgParam[0].dialState = PPPOECFGPARAM_DIALSTATE_OFF;

		LogDebugInfo(fp);
		
		memcpy((void*)&myIp,pRomeCfgParam->ifCfgParam[0].ipAddr,4);
		memcpy((void*)&myIpMask,pRomeCfgParam->ifCfgParam[0].ipMask,4);

		ret=rtl8651_delNaptMapping(myIp);
		if (ret)
		{
			printf("rtl8651_delNaptMapping: ret %d\n",ret);
			fprintf(fp,"rtl8651_delNaptMapping: ret %d\n",ret);
		}

		//rtl8651_delRoute(ipaddr_t ipAddr, ipaddr_t ipMask);
		ret=rtl8651_delRoute((ipaddr_t)0,(ipaddr_t)0);
		if (ret)
		{
			printf("rtl8651_delRoute(default): ret %d\n",ret);
			fprintf(fp,"rtl8651_delRoute(default): ret %d\n",ret);
		}

		ret=rtl8651_delIpIntf("eth0",myIp,myIpMask);
		if (ret)
		{
			printf("rtl8651_delIpIntf: ret %d\n",ret);
			fprintf(fp,"rtl8651_delIpIntf: ret %d\n",ret);
		}
		
		i=0;
		memcpy(pRomeCfgParam->ifCfgParam[0].ipAddr,&i,4);
		memcpy(pRomeCfgParam->ifCfgParam[0].ipMask,&i,4);
		memcpy(pRomeCfgParam->ifCfgParam[0].gwAddr,&i,4);
		memcpy(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,&i,4);
	}	
#ifdef CONFIG_RTL865XB_3G
	else if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_3G) 
	{
		rtl8651a_setWanStatus(0, FALSE);	
	
		LogDebugInfo(fp);

//		memset(pRomeCfgParam->ifCfgParam[0].ipAddr, 0, 4);
//		memset(pRomeCfgParam->ifCfgParam[0].ipMask, 0, 4);
//		memset(pRomeCfgParam->ifCfgParam[0].gwAddr, 0, 4);
			
//		printf("ipAddr=%02x\n", pRomeCfgParam->ifCfgParam[0].ipAddr);
//		printf("ipMask=%02x\n", pRomeCfgParam->ifCfgParam[0].ipMask);
		memcpy((void*)&myIp,pRomeCfgParam->ifCfgParam[0].ipAddr,4);
		memcpy((void*)&myIpMask,pRomeCfgParam->ifCfgParam[0].ipMask,4);
		ret=rtl8651_delNaptMapping(myIp);
		printf("rtl8651_delNaptMapping: ret %d\n",ret);
		if (ret)
		{
			printf("rtl8651_delNaptMapping: ret %d\n",ret);
			fprintf(fp,"rtl8651_delNaptMapping: ret %d\n",ret);
	}	

/*Peter*/	ret=rtl8651_delIpIntf("ppp0", myIp, myIpMask);
		if (ret)
		{
			printf("rtl8651_delIpIntf: ret=%d\n",ret);
			fprintf(fp,"rtl8651_delIpIntf: ret=%d\n",ret);
		}
		i=0;
		memcpy(pRomeCfgParam->ifCfgParam[0].ipAddr, &i, 4);
		memcpy(pRomeCfgParam->ifCfgParam[0].ipMask, &i, 4);
		memcpy(pRomeCfgParam->ifCfgParam[0].gwAddr, &i, 4);
		memcpy(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr, &i, 4);
		memcpy(pRomeCfgParam->ifCfgParam[0].dnsSecondaryAddr, &i, 4);  // 2005-10-07: Peter add for 3G 

/*Peter*/
	}	
#endif
	else if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_PPPOE)
	{

		/* single pppoe hangup case */
		rtl8651a_setWanStatus(0,FALSE);		
		pRomeCfgParam->pppoeCfgParam[0].dialState = PPPOECFGPARAM_DIALSTATE_OFF;

		LogDebugInfo(fp);
	
		memcpy((void*)&myIp,pRomeCfgParam->ifCfgParam[0].ipAddr,4);
		memcpy((void*)&myIpMask,pRomeCfgParam->ifCfgParam[0].ipMask,4);

		ret=rtl8651_delNaptMapping(myIp);
		if (ret)
		{
			printf("rtl8651_delNaptMapping: ret %d\n",ret);
			fprintf(fp,"rtl8651_delNaptMapping: ret %d\n",ret);
		}

		//ret=rtl8651_resetPppoeSessionProperty((uint32)pppObjId);
		ret=rtl8651_resetPppoeSessionProperty((uint32)0);
		if (ret)
		{
			printf("rtl8651_resetPppoeSessionProperty: ret %d\n",ret);
			fprintf(fp,"rtl8651_resetPppoeSessionProperty: ret %d\n",ret);
		}

		//rtl8651_delRoute(ipaddr_t ipAddr, ipaddr_t ipMask);
		#if 0
		ret=rtl8651_delRoute((ipaddr_t)0,(ipaddr_t)0);
		if (ret)
		{
			printf("rtl8651_delRoute(default): ret %d\n",ret);
			fprintf(fp,"rtl8651_delRoute(default): ret %d\n",ret);
		}
		#endif

		ret=rtl8651_delIpIntf("eth0",myIp,myIpMask);
		if (ret)
		{
			printf("rtl8651_delIpIntf: ret %d\n",ret);
			fprintf(fp,"rtl8651_delIpIntf: ret %d\n",ret);
		}

		i=0;
		memcpy(pRomeCfgParam->ifCfgParam[0].ipAddr,&i,4);
		memcpy(pRomeCfgParam->ifCfgParam[0].ipMask,&i,4);
		memcpy(pRomeCfgParam->ifCfgParam[0].gwAddr,&i,4);
		memcpy(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,&i,4);
		
		/* have to delete the pppdx.pid file, asp_pppoe will check it */		
		//unlink("/var/run/pppd0.pid");
		
		//fprintf(fp,"pppoeManualHangupFlag[0]:%u\n",pppoeManualHangupFlag[0]);
		ret=re865xIoctl("eth0",SET_PPPD_IDLE_TIME,0,FALSE,0);
		if(ret)
		{
			fprintf(fp,"disable auto hang up failed.\n");
		}
		
		/* check autoReconnect to see if a pppd_start() is needed */
		if (   (pRomeCfgParam->pppoeCfgParam[0].manualHangup==0)
		    && (pRomeCfgParam->pppoeCfgParam[0].autoReconnect==1)
		   )
		{			
			fprintf(fp,"do pppd_start() due to autoreconnect\n");
			pppd_start(0);
		}
	}
	else if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)
	{
		/* multiple pppoe hangup case */
		
		rtl8651a_setWanStatus(pppObjId,FALSE);
		pRomeCfgParam->pppoeCfgParam[pppObjId].dialState = PPPOECFGPARAM_DIALSTATE_OFF;
		
		printf("pppoeCfgParam[%u].ipAddr: %u.%u.%u.%u\n"
			,pppObjId
			,pRomeCfgParam->pppoeCfgParam[pppObjId].ipAddr[0]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].ipAddr[1]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].ipAddr[2]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].ipAddr[3]);

		fprintf(fp,"pppoeCfgParam[%u].ipAddr: %u.%u.%u.%u\n"
			,pppObjId			
			,pRomeCfgParam->pppoeCfgParam[pppObjId].ipAddr[0]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].ipAddr[1]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].ipAddr[2]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].ipAddr[3]);

		printf("pppoeCfgParam[%u].ipMask: %u.%u.%u.%u\n"
			,pppObjId
			,pRomeCfgParam->pppoeCfgParam[pppObjId].ipMask[0]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].ipMask[1]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].ipMask[2]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].ipMask[3]);

		fprintf(fp,"pppoeCfgParam[%u].ipMask: %u.%u.%u.%u\n"
			,pppObjId
			,pRomeCfgParam->pppoeCfgParam[pppObjId].ipMask[0]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].ipMask[1]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].ipMask[2]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].ipMask[3]);

		printf("pppoeCfgParam[%u].gwAddr: %u.%u.%u.%u\n"
			,pppObjId
			,pRomeCfgParam->pppoeCfgParam[pppObjId].gwAddr[0]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].gwAddr[1]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].gwAddr[2]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].gwAddr[3]);

		fprintf(fp,"pppoeCfgParam[%u].gwAddr: %u.%u.%u.%u\n"
			,pppObjId
			,pRomeCfgParam->pppoeCfgParam[pppObjId].gwAddr[0]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].gwAddr[1]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].gwAddr[2]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].gwAddr[3]);

		printf("pppoeCfgParam[%u].dnsAddr: %u.%u.%u.%u\n"
			,pppObjId
			,pRomeCfgParam->pppoeCfgParam[pppObjId].dnsAddr[0]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].dnsAddr[1]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].dnsAddr[2]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].dnsAddr[3]);

		fprintf(fp,"pppoeCfgParam[%u].dnsAddr: %u.%u.%u.%u\n"
			,pppObjId
			,pRomeCfgParam->pppoeCfgParam[pppObjId].dnsAddr[0]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].dnsAddr[1]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].dnsAddr[2]
			,pRomeCfgParam->pppoeCfgParam[pppObjId].dnsAddr[3]);
				
		memcpy((void*)&myIp,pRomeCfgParam->pppoeCfgParam[pppObjId].ipAddr,4);
		memcpy((void*)&myIpMask,pRomeCfgParam->pppoeCfgParam[pppObjId].ipMask,4);
		
		pppoeCfgPtr.defaultSession=pRomeCfgParam->pppoeCfgParam[pppObjId].defaultSession;	
		memcpy(&pppoeCfgPtr.ipAddr,(void*)&myIp,4);
		memcpy(&pppoeCfgPtr.ipMask,(void*)&myIpMask,4);
		memcpy(&pppoeCfgPtr.gwAddr,(void*)&pRomeCfgParam->pppoeCfgParam[pppObjId].gwAddr,4);
		memcpy(&(pppoeCfgPtr.lanType),&(pRomeCfgParam->pppoeCfgParam[pppObjId].lanType),4);
		re865xIoctl("eth0",PPPOEDOWN_CFG,(uint32)&pppoeCfgPtr,pppObjId,0);

		ret=0;
		if (pRomeCfgParam->pppoeCfgParam[pppObjId].defaultSession==1)
		{
		//	ret=rtl8651_delNaptMapping(myIp);
			if (ret)
			{
				printf("rtl8651_delNaptMapping: ret %d\n",ret);
				fprintf(fp,"rtl8651_delNaptMapping: ret %d\n",ret);
			}
		}
		else
		{

//			ipaddr_t localHostIp;
//			memcpy((void*)&localHostIp,pRomeCfgParam->pppoeCfgParam[pppObjId].localHostIp,4);
	//		ret=rtl8651_delNaptMapping(myIp);
	//		ret=rtl8651_delRoute((ipaddr_t)myIp,(ipaddr_t)0xffffffff);
	//		rtl8651_flushPolicyRoute(myIp);
			if (ret)
			{
				printf("rtl8651_delRoute: ret %d\n",ret);
				fprintf(fp,"rtl8651_delRoute: ret %d\n",ret);
			}

#if 0
			ret=rtl8651_delNatMapping(myIp,localHostIp);
			if (ret)
			{
				printf("rtl8651_delNatMapping: ret %d\n",ret);
				fprintf(fp,"rtl8651_delNatMapping: ret %d\n",ret);
			}
#endif
		}

	//	ret=rtl8651_resetPppoeSessionProperty((uint32)pppObjId);
		if (ret)
		{
			printf("rtl8651_resetPppoeSessionProperty: ret %d\n",ret);
			fprintf(fp,"rtl8651_resetPppoeSessionProperty: ret %d\n",ret);
		}

		if (pRomeCfgParam->pppoeCfgParam[pppObjId].defaultSession==1)
		{
			//rtl8651_delRoute(ipaddr_t ipAddr, ipaddr_t ipMask);
			#if 0			
			ret=rtl8651_delRoute((ipaddr_t)0,(ipaddr_t)0);
			if (ret)
			{
				printf("rtl8651_delRoute(default): ret %d\n",ret);
				fprintf(fp,"rtl8651_delRoute(default): ret %d\n",ret);
			}
			#endif
		}

	//	ret=rtl8651_delIpIntf("eth0",myIp,myIpMask);
		if (ret)
		{
			printf("rtl8651_delIpIntf: ret %d\n",ret);
			fprintf(fp,"rtl8651_delIpIntf: ret %d\n",ret);
		}

		#if 0
		/* pick another up session and set default route to it */
		{ /* emily */
			for (i=0; i<MAX_PPPOE_SESSION;i++){
				if (pRomeCfgParam->pppoeCfgParam[i].dialState == PPPOECFGPARAM_DIALSTATE_DIALED_SUCCESS){
					ret=rtl8651_addRoute((ipaddr_t)0,(ipaddr_t)0,"eth0",0);
					if (ret){
						printf("rtl8651_addRoute(default): ret %d\n",ret);
						fprintf(fp,"rtl8651_addRoute(default): ret %d\n",ret);
					}
				}
			}
		}
		#endif
		
		i=0;			
		memcpy(pRomeCfgParam->pppoeCfgParam[pppObjId].ipAddr,&i,4);
		memcpy(pRomeCfgParam->pppoeCfgParam[pppObjId].ipMask,&i,4);
		memcpy(pRomeCfgParam->pppoeCfgParam[pppObjId].gwAddr,&i,4);
		memcpy(pRomeCfgParam->pppoeCfgParam[pppObjId].dnsAddr,&i,4);

		{
			//uint8 fname[20];
			uint32 whichPppObjId=pRomeCfgParam->pppoeCfgParam[0].whichPppObjId;
			//sprintf(fname,"/var/run/pppd%u.pid",whichPppObjId);
			//unlink(fname);
			
			ret=re865xIoctl("eth0",SET_PPPD_IDLE_TIME,whichPppObjId,FALSE,0);
			if(ret)
			{
				fprintf(fp,"disable auto hang up session%d failed.\n",whichPppObjId);
			}
		
			/* check autoReconnect to see if a pppd_start() is needed */
			if (   (pRomeCfgParam->pppoeCfgParam[whichPppObjId].manualHangup==0)
			    && (pRomeCfgParam->pppoeCfgParam[whichPppObjId].autoReconnect==1)
			   )
				pppd_start(whichPppObjId);
		}
	}		
	else if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED)
	{
		/* ip unnumbered pppoe hangup case */
		rtl8651a_setWanStatus(0,FALSE);
		pRomeCfgParam->pppoeCfgParam[0].dialState = PPPOECFGPARAM_DIALSTATE_OFF;
		
		LogDebugInfo(fp);
		
		memcpy((void*)&myIp,pRomeCfgParam->ifCfgParam[0].ipAddr,4);
		memcpy((void*)&myIpMask,pRomeCfgParam->ifCfgParam[0].ipMask,4);

		ret=rtl8651_delNaptMapping(myIp);
		if (ret)
		{
			printf("rtl8651_delNaptMapping: ret %d\n",ret);
			fprintf(fp,"rtl8651_delNaptMapping: ret %d\n",ret);
		}

		//ret=rtl8651_resetPppoeSessionProperty((uint32)pppObjId);
		ret=rtl8651_resetPppoeSessionProperty((uint32)0);
		if (ret)
		{
			printf("rtl8651_resetPppoeSessionProperty: ret %d\n",ret);
			fprintf(fp,"rtl8651_resetPppoeSessionProperty: ret %d\n",ret);
		}

		memcpy((void*)&myIpMask,pRomeCfgParam->pppoeCfgParam[0].unnumberedIpMask,4);
		ret=rtl8651_setLanSideExternalIpInterface("eth1",myIp,(ipaddr_t)myIpMask,0/*isExternal:false*/);
		if (ret)
		{
			printf("rtl8651_setLanSideExternalIpInterface: ret %d\n",ret);
			fprintf(fp,"rtl8651_setLanSideExternalIpInterface: ret %d\n",ret);
		}


		/*   2005.3.8
		  *   Delete the host route which is corresponding to the originally dialed-up PPPoE session.
		  */
		ret = rtl8651_delRoute(myIp, 0xffffffff);
		if (ret) {
			printf("rtl8651_delRoute(host route): ret %d\n", ret);
			fprintf(fp, "rtl8651_delRoute(host route): ret %d\n", ret);
		}

		
		//rtl8651_delRoute(ipaddr_t ipAddr, ipaddr_t ipMask);
		#if 0
		ret=rtl8651_delRoute((ipaddr_t)0,(ipaddr_t)0);
		if (ret)
		{
			printf("rtl8651_delRoute(default): ret %d\n",ret);
			fprintf(fp,"rtl8651_delRoute(default): ret %d\n",ret);
		}
		#endif

		#if 0
		ret=rtl8651_delIpIntf("eth1",myIp,myIpMask);
		if (ret)
		{
			printf("rtl8651_delIpIntf: ret %d\n",ret);
			fprintf(fp,"rtl8651_delIpIntf: ret %d\n",ret);
		}
		#endif

		/* Delete the policy route which is added in board_ipUp_unnumberedPppoe(). */
		ret = rtl8651_flushPolicyRoute(myIp);
		if(ret != SUCCESS) {
			printf("flushPolicyRoute failed -- ret: %d\n", ret);
			return NOT_OK;
		}




		
		/* set global ip address to lan side as ip alias */
		{
			int 	ipValue;
			char	*av[5];
			char	*ip_string;
			char	*ipmask_string;
			char	pname[]="/bin/ifconfig";

			ip_string     = malloc(MAX_IP_STRING_LEN);
			ipmask_string = malloc(MAX_IP_STRING_LEN);
			memcpy(&ipValue,pRomeCfgParam->pppoeCfgParam[0].unnumberedIpAddr,4);
			sprintf(ip_string,"%u.%u.%u.%u",(ipValue>>24)&0xff, (ipValue>>16)&0xff, (ipValue>>8)&0xff, ipValue&0xff);
			memcpy(&ipValue,pRomeCfgParam->pppoeCfgParam[0].unnumberedIpMask,4);
			sprintf(ipmask_string,"%u.%u.%u.%u",(ipValue>>24)&0xff, (ipValue>>16)&0xff, (ipValue>>8)&0xff, ipValue&0xff);
			
			av[0]=pname;
			av[1]="eth1:1";
			av[2]=ip_string;
			av[3]="netmask";
			av[4]=ipmask_string;
			av[5]=NULL;
			runProgram(pname,av);
			free(ip_string);
			free(ipmask_string);
		}

		/* delete global assigned network route to user protocol stack */
		{
			char *argv[6];
			uint8 networkRoute[4];
			uint8 netStr[20];
			uint8 netmaskStr[20];
		
			memcpy((void*)networkRoute,(void*)pRomeCfgParam->ifCfgParam[0].ipAddr,4);
			for (i=0;i<4;i++)
				networkRoute[i] &= pRomeCfgParam->ifCfgParam[0].ipMask[i];
				
			argv[0]="-net";
			
			sprintf(netStr,"%u.%u.%u.%u",
				networkRoute[0],
				networkRoute[1],
				networkRoute[2],
				networkRoute[3]);
			argv[1]=netStr;
			
			argv[2]="netmask";
			
			sprintf(netmaskStr,"%u.%u.%u.%u",
				pRomeCfgParam->ifCfgParam[0].ipMask[0],
				pRomeCfgParam->ifCfgParam[0].ipMask[1],
				pRomeCfgParam->ifCfgParam[0].ipMask[2],
				pRomeCfgParam->ifCfgParam[0].ipMask[3]);
			argv[3]=netmaskStr;
			
			argv[4]="eth1";
			argv[5]=NULL;
								
			INET_setroute(RTACTION_DEL, 0, argv);
		} /* end block delete user protocol stack network route */

		i=0;
		memcpy(pRomeCfgParam->ifCfgParam[0].ipAddr,&i,4);
		memcpy(pRomeCfgParam->ifCfgParam[0].ipMask,&i,4);
		memcpy(pRomeCfgParam->ifCfgParam[0].gwAddr,&i,4);
		memcpy(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,&i,4);


		/* have to delete the pppdx.pid file, asp_pppoe will check it */		
		//unlink("/var/run/pppd0.pid");

		//disable idle timeout
		ret=re865xIoctl("eth0",SET_PPPD_IDLE_TIME,0,FALSE,0);
		if(ret)
		{
			fprintf(fp,"disable auto hang up failed.\n");
		}		

		/* check autoReconnect to see if a pppd_start() is needed */
		if (   (pRomeCfgParam->pppoeCfgParam[0].manualHangup==0)
		    && (pRomeCfgParam->pppoeCfgParam[0].autoReconnect==1)
		   )
			pppd_start(0);
		
	}
#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
	else if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_PPTP)
	{
		/* single pppoe hangup case */
		rtl8651a_setWanStatus(0,FALSE);		
		pRomeCfgParam->pptpCfgParam.dialState = PPTPCFGPARAM_DIALSTATE_DISCONNECTING;

		LogDebugInfo(fp);
		
		memcpy((void*)&myIp,pRomeCfgParam->ifCfgParam[0].ipAddr,4);
		memcpy((void*)&myIpMask,pRomeCfgParam->ifCfgParam[0].ipMask,4);

		ret=rtl8651_resetPptpProperty(1);
		if (ret)
		{
			printf("rtl8651_resetPptpProperty: ret %d\n",ret);
			fprintf(fp,"rtl8651_resetPptpProperty: ret %d\n",ret);
		}

		system("/sbin/ifconfig ppp0 down");

		i=0;
		memcpy(pRomeCfgParam->ifCfgParam[0].ipAddr,&i,4);
		memcpy(pRomeCfgParam->ifCfgParam[0].ipMask,&i,4);
		memcpy(pRomeCfgParam->ifCfgParam[0].gwAddr,&i,4);
		memcpy(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,&i,4);
		memcpy(pRomeCfgParam->ifCfgParam[0].dnsSecondaryAddr,&i,4);
		
		/* have to delete the pppdx.pid file, asp_pppoe will check it */		
		//unlink("/var/run/pppd0.pid");
		
		//fprintf(fp,"pppoeManualHangupFlag[0]:%u\n",pppoeManualHangupFlag[0]);
		ret=rtl8651_delNaptMapping(myIp);		
		if (ret)
		{
			printf("rtl8651_delNaptMapping: ret %d\n",ret);
			fprintf(fp,"rtl8651_delNaptMapping: ret %d\n",ret);
		}

	
		ret=rtl8651_delIpIntf("eth0",myIp,myIpMask);		
		if (ret)
		{
			printf("rtl8651_delIpIntf: ret %d\n",ret);
			fprintf(fp,"rtl8651_delIpIntf: ret %d\n",ret);
		}
		
		ret=re865xIoctl("eth0",SET_PPPD_IDLE_TIME,0,FALSE,0);
		if(ret)
		{
			fprintf(fp,"disable auto hang up failed.\n");
		}	
		
		/* check autoReconnect to see if a pppd_start() is needed */
		if (   (pRomeCfgParam->pptpCfgParam.manualHangup==0)
		    && (pRomeCfgParam->pptpCfgParam.autoReconnect==1)
		   )
		{			
			fprintf(fp,"do pppd_start() due to autoreconnect\n");
			pptp_start(0);			
		}
	}	
	else if ((pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_L2TP)||
		    (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_DHCPL2TP))
	{	
		if ((pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_DHCPL2TP)&&
			(pRomeCfgParam->l2tpCfgParam.dialState==L2TPCFGPARAM_DIALSTATE_DIALED_DHCP_DISCOVER))
			{
				fprintf(fp,"DHCP IP-Down\n");
				fclose(fp);
				//pRomeCfgParam->l2tpCfgParam.dialState=L2TPCFGPARAM_DIALSTATE_OFF;
				//rtl8651_sem_post(SEMAPHORE_IPEVENT);
				return 0;
			}

		fprintf(fp,"L2TP IP-Down\n");
		
		pRomeCfgParam->l2tpCfgParam.dialState=L2TPCFGPARAM_DIALSTATE_DIALED_DHCP_DISCOVER;

		rtl8651a_setWanStatus(0,FALSE);
		//pRomeCfgParam->l2tpCfgParam.dialState = L2TPCFGPARAM_DIALSTATE_OFF;

		LogDebugInfo(fp);

		memcpy((void*)&myIp,pRomeCfgParam->ifCfgParam[0].ipAddr,4);
		memcpy((void*)&myIpMask,pRomeCfgParam->ifCfgParam[0].ipMask,4);

		ret=rtl8651_resetL2tpProperty(1);
		if (ret)
		{
			printf("rtl8651_resetL2tpProperty: ret %d\n",ret);
			fprintf(fp,"rtl8651_resetL2tpProperty: ret %d\n",ret);
		}



		
		/* have to delete the pppdx.pid file, asp_pppoe will check it */		
		
		//fprintf(fp,"pppoeManualHangupFlag[0]:%u\n",pppoeManualHangupFlag[0]);

#if 0
		/* rtl8651_delRoute() rtl8651_delNaptMapping() rtl8651_delIpIntf() by DEL_VLAN_IP */
		re865xIoctl("eth0",DEL_VLAN_IP,0,0,0);
#else
		ret=rtl8651_delNaptMapping(myIp);
		if (ret)
		{
			printf("rtl8651_delNaptMapping: ret %d\n",ret);
			fprintf(fp,"rtl8651_delNaptMapping: ret %d\n",ret);
		}

		if (pRomeCfgParam->ifCfgParam[0].connType == IFCFGPARAM_CONNTYPE_DHCPL2TP)
		{		
			ipaddr_t myGw=0,myDns=0;

			ret=rtl8651_delRoute((ipaddr_t)0,(ipaddr_t)0);
			if(ret)
			{
				printf("rtl8651_delRoute: ret %d\n",ret);
				fprintf(fp,"rtl8651_delRoute: ret %d\n",ret);		
			}

			ret=rtl8651_delExtNetworkInterface((int8*)"eth0");
			if (ret)
			{
				printf("rtl8651_delExtNetworkInterface: ret %d\n",ret);
				fprintf(fp,"rtl8651_delExtNetworkInterface: ret %d\n",ret);
			}
			
			ret=rtl8651_delSession(1); 
			if (ret)
			{
				printf("rtl8651_delSession: ret %d\n",ret);
				fprintf(fp,"rtl8651_delSession: ret %d\n",ret);
			}

			ret=rtl8651_delIpIntf("eth0",*(uint32*)(&(pRomeCfgParam->l2tpCfgParam.ipAddr)),*(uint32*)(&(pRomeCfgParam->l2tpCfgParam.ipMask)));
			if (ret)
			{
				printf("rtl8651_delIpIntf: ret %d\n",ret);
				fprintf(fp,"rtl8651_delIpIntf: %x %x ret %d\n",*(uint32*)(&(pRomeCfgParam->l2tpCfgParam.ipAddr)),*(uint32*)(&(pRomeCfgParam->l2tpCfgParam.ipMask)),ret);
			}
		
			ret=rtl8651_delIpIntf("eth0", myIp, myIpMask);
			if (ret)
			{
				printf("rtl8651_delIpIntf: ret %d\n",ret);
				fprintf(fp,"rtl8651_delIpIntf: ret %d\n",ret);			
			}

			ret=rtl8651_removeNetworkIntfLinkLayerType("eth0");
			if (ret)
			{
				printf("rtl8651_removeNetworkIntfLinkLayerType: ret %d\n",ret);
				fprintf(fp,"rtl8651_removeNetworkIntfLinkLayerType: ret %d\n",ret);
			}

			ret=rtl8651_delNetworkIntf("eth0");	
			if(ret)
			{
				printf("rtl8651_delNetworkIntf: ret %d\n",ret);
				fprintf(fp,"rtl8651_delNetworkIntf: ret %d\n",ret);				
			}		
		

			pRomeCfgParam->l2tpCfgParam.dialState=L2TPCFGPARAM_DIALSTATE_DIALED_DHCP_DISCOVER;

			setInterfaceIp("eth0",0,0);
		}
		else
		{
			ret=rtl8651_delIpIntf("eth0",myIp,myIpMask);
			if (ret)
			{
				printf("rtl8651_delIpIntf: ret %d\n",ret);
				fprintf(fp,"rtl8651_delIpIntf: ret %d\n",ret);
			}
		}
	
		system("/sbin/ifconfig ppp0 down");

		i=0;
		memcpy(pRomeCfgParam->ifCfgParam[0].ipAddr,&i,4);
		memcpy(pRomeCfgParam->ifCfgParam[0].ipMask,&i,4);
		memcpy(pRomeCfgParam->ifCfgParam[0].gwAddr,&i,4);
		memcpy(pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,&i,4);

		unlink("/var/run/pppd0.pid");
		unlink("/var/run/l2tpd.pid");
		
		
#endif		

		//disable idle timeout
		ret=re865xIoctl("eth0",SET_PPPD_IDLE_TIME,0,FALSE,0);
		if(ret)
		{
			fprintf(fp,"disable auto hang up failed.\n");
		}	
		
		/* check autoReconnect to see if a pppd_start() is needed */
		if (   (pRomeCfgParam->l2tpCfgParam.manualHangup==0)
		    && (pRomeCfgParam->l2tpCfgParam.autoReconnect==1)
		   )
		{			
			fprintf(fp,"do l2tp_start() due to autoreconnect\n");
			l2tp_start(0);
		}
	}

#endif	
	/////////////////////
	/* end if pRomeCfgParam->ifCfgParam[0].connType */

	
	/*
	 *   Protocol-Based NAT
	 */
	{
		switch(pRomeCfgParam->ifCfgParam[0].connType)
		{
			case IFCFGPARAM_CONNTYPE_STATIC:
				/* do nothing */
				break;
			case IFCFGPARAM_CONNTYPE_DHCPC: /* fall through */
			case IFCFGPARAM_CONNTYPE_PPTP:  /* fall through */
			case IFCFGPARAM_CONNTYPE_L2TP:  /* fall through */
			case IFCFGPARAM_CONNTYPE_PPPOE: /* fall through */
			case IFCFGPARAM_CONNTYPE_DHCPL2TP: /* fall through */
			case IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED:
				pbnat_ipdown( 0, myIp );
				break;
			case IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE:
				pbnat_ipdown( pppObjId, myIp );
				break;
			default:
				break;
		}
	
	} /* end of pbnat */

	
	

	fclose(fp);
	upnp_stop();

	// Dino Chang 2004/12/08
	// Stop Trend Micro Security Services daemon
	{
		// Dino Chang 2005/01/15
		if(pRomeCfgParam->tmssCfgParam.tmss_enable || pRomeCfgParam->tmssCfgParam.pc_enable) {
			restart_tmss_daemon(0);
		}
		// Dino
	}
	// Dino

	return 0;
} /* end board_ipDownEventTableDriverAccess(void) */

#if 0
uint32 getRomeCfgParamPtr(romeCfgParam_t ** ptr)
{
	uint32 i;
	FILE * fp;
	fp = fopen("/var/romecfg.txt","r");
	if (!fp)
		return 1;
	fscanf(fp,"%x",&i);
	*ptr = (romeCfgParam_t *)i;
	return 0;
}
#endif

void AddDemandRoute(int pppObjId,int defaultSession)
{
	rtl8651_tblDrvDemandRoute_t tmpRt;
	int32  i;
	uint32 ip_start,ip_end;
	uint16 port_start,port_end;
	
	rtl8651_flushDemandRoute(pppObjId);
	
	if (defaultSession){
		tmpRt.type=TRIGGER_DSTIP;
		tmpRt.ip_start=0;
		tmpRt.ip_end=0xffffffff;
		rtl8651_addDemandRoute(&tmpRt,pppObjId,0);
		return;
	}

	if (pRomeCfgParam->pppoeCfgParam[pppObjId].lanType == PPPOECFGPARAM_LANTYPE_UNNUMBERED){
		ipaddr_t stub1,stub2;
		uint32 srcSubnet;
		memcpy(&stub1,pRomeCfgParam->pppoeCfgParam[pppObjId].unnumberedIpAddr,4);
		memcpy(&stub2,pRomeCfgParam->pppoeCfgParam[pppObjId].unnumberedIpMask,4);
		srcSubnet = (ipaddr_t)(stub1 & stub2);/* get subnet value */
		tmpRt.type=STATIC_POLICY_ROUTE|TRIGGER_SRCIP;
		tmpRt.ip_start=srcSubnet+1;
		tmpRt.ip_end=srcSubnet+(~stub2);
		rtl8651_addDemandRoute(&tmpRt,pppObjId,0);
	}
	
	for (i=0;i<4;i++){
		if (pRomeCfgParam->pppoeCfgParam[pppObjId].destnetType[i] == PPPOECFGPARAM_DESTNETTYPE_IP){
			parse_ipRange(pRomeCfgParam->pppoeCfgParam[pppObjId].destnet[i],&ip_start,&ip_end);
			tmpRt.type=TRIGGER_DSTIP;
			tmpRt.ip_start=ip_start;
			tmpRt.ip_end=ip_end;
			rtl8651_addDemandRoute(&tmpRt,pppObjId,0);
		} else if (pRomeCfgParam->pppoeCfgParam[pppObjId].destnetType[i] == PPPOECFGPARAM_DESTNETTYPE_TCPPORT){
			parse_portRange(pRomeCfgParam->pppoeCfgParam[pppObjId].destnet[i], &port_start ,&port_end);
					tmpRt.type=TRIGGER_DSTTCPPORT;
					tmpRt.trigger.dport.start=port_start;
					tmpRt.trigger.dport.end=port_end;
					rtl8651_addDemandRoute(&tmpRt,pppObjId,0);
		} else if (pRomeCfgParam->pppoeCfgParam[pppObjId].destnetType[i] ==	PPPOECFGPARAM_DESTNETTYPE_UDPPORT){
			parse_portRange( pRomeCfgParam->pppoeCfgParam[pppObjId].destnet[i], &port_start ,&port_end);
			tmpRt.type=TRIGGER_DSTUDPPORT;
			tmpRt.trigger.dport.start=port_start;
			tmpRt.trigger.dport.end=port_end;
			rtl8651_addDemandRoute(&tmpRt,pppObjId,0);
		}
	}

}

/*
 * start pppd on specified pppObjId (0,1,...,MAX_PPPOE_SESSION-1)
 */
void pppd_start(uint32 identity)
{
	FILE * f1, * f2;
	//romeCfgParam_t * pRomeCfgParam;
	char    idStr[4];
	char    papStr[20];
	char    pppOptionStr[20];
	char    chapStr[20];
	uint8   tmp[160];

#ifdef __uClinux__ 
#else //MMU
	sprintf(tmp,"/var/run/pppd%d.pid",identity);
	send_signal(tmp,9);
	unlink(tmp);
#endif

#if 0
	if (getRomeCfgParamPtr(&pRomeCfgParam))
	{
		printf("pppd_start: getRomeCfgParamPtr failed\n");
		return;
	}
#endif

	if (pRomeCfgParam->pppoeCfgParam[identity].enable==0)
	{
		printf("pppoe session #%u disabled ... \n", identity);
		return;
	}

	f1 = fopen("/etc/ppp.options","r");
	if (!f1)
	{
		printf("pppd_start: fopen /etc/ppp.options failed\n");
		return;
	}
		
	sprintf(papStr,"/var/ppp/pap%u",identity);
	f2 = fopen(papStr,"w+");
	if (!f2)
	{
		printf("pppd_start: fopen %s failed\n",papStr);
		return;
	}
	
	sprintf(tmp,"%s * %s" ,pRomeCfgParam->pppoeCfgParam[identity].username,pRomeCfgParam->pppoeCfgParam[identity].password);
	fputs(tmp,f2);
	fclose(f2);
	
	sprintf(chapStr,"/var/ppp/chap%u",identity);
	f2 = fopen(chapStr,"w+");
	if (!f2)
	{
		printf("pppd_start: fopen %s failed\n",chapStr);
		return;
	}
	sprintf(tmp,"%s * %s" ,pRomeCfgParam->pppoeCfgParam[identity].username,pRomeCfgParam->pppoeCfgParam[identity].password);
	fputs(tmp,f2);
	fclose(f2);
	
		
	sprintf(pppOptionStr,"/var/ppp/options%u",identity);
	f2 = fopen(pppOptionStr,"w+");
	while (!feof(f1))
	{
		fgets(tmp,160,f1);

		if (!memcmp(tmp,"mtu",3))
			sprintf(tmp,"mtu %d mru %d\n",pRomeCfgParam->pppoeCfgParam[identity].mtu,pRomeCfgParam->pppoeCfgParam[identity].mtu);
		
		if (!memcmp(tmp,"user",4))
			sprintf(tmp,"user %s\n",pRomeCfgParam->pppoeCfgParam[identity].username);

		fputs(tmp,f2);
	}			
	fclose(f1);


	/* put requested ip in the option file
	 * option format: <local IP address>:<remote IP address>
	 */
	if (   pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_PPPOE_UNNUMBERED
		|| (    pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE
		     && pRomeCfgParam->pppoeCfgParam[identity].lanType==PPPOECFGPARAM_LANTYPE_UNNUMBERED
		   ) )
		fprintf(f2,"%u.%u.%u.%u:0.0.0.0\n",
			pRomeCfgParam->pppoeCfgParam[identity].unnumberedIpAddr[0],
			pRomeCfgParam->pppoeCfgParam[identity].unnumberedIpAddr[1],
			pRomeCfgParam->pppoeCfgParam[identity].unnumberedIpAddr[2],
			pRomeCfgParam->pppoeCfgParam[identity].unnumberedIpAddr[3]);
	
	/* generate idle option (auto disconnect) for pppd's reference */
	fprintf(f2,"idle %u\n",pRomeCfgParam->pppoeCfgParam[identity].silentTimeout);
	
	/* generate dial-on-demand option for pppd's reference */
	if (pRomeCfgParam->pppoeCfgParam[identity].serviceName[0])
		fprintf(f2,"service_name \"%s\"\n",pRomeCfgParam->pppoeCfgParam[identity].serviceName);

	/* generate dial-on-demand option for pppd's reference */
	if (pRomeCfgParam->pppoeCfgParam[identity].acName[0])
		fprintf(f2,"ac_name \"%s\"\n",pRomeCfgParam->pppoeCfgParam[identity].acName);

	/* generate nodefaultroute option for pppd's reference */
	if ((pRomeCfgParam->pppoeCfgParam[identity].defaultSession!=1)&&(pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE))
		fprintf(f2,"nodefaultroute\n");
	else
		fprintf(f2,"defaultroute\n");

	// Dino Chang 2005/03/03
	if (pRomeCfgParam->pppoeCfgParam[identity].autoReconnect != 1)
		fprintf(f2,"nopersist\n");
	else
		fprintf(f2,"persist\n");
	// Dino
		
	fclose(f2);

	sprintf(idStr,"%u",identity);
	if (pRomeCfgParam->pppoeCfgParam[identity].demand==1)
	{   
		pRomeCfgParam->pppoeCfgParam[identity].dialState=PPPOECFGPARAM_DIALSTATE_DIALED_WAITING;

		if (pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_PPPOE_MULTIPLE_PPPOE)
			AddDemandRoute(identity,pRomeCfgParam->pppoeCfgParam[identity].defaultSession);
		else
			AddDemandRoute(identity,1);
		return ;
	}
	// Dino Chang 2005/03/03
	else if (pRomeCfgParam->pppoeCfgParam[identity].autoReconnect==1) {
	// Dino

		pRomeCfgParam->pppoeCfgParam[identity].dialState=PPPOECFGPARAM_DIALSTATE_DIALED_TRYING;

#ifdef __uClinux__ // non-MMU
		sprintf(tmp,"/var/run/pppd%u.pid",identity);
		f2=fopen(tmp,"r");
		if(f2)
		{
			fclose(f2);
			send_signal(tmp,16); //restart pppd
		}
		else // run first time	
#endif	
		{
			do_exec("/bin/pppd",idStr);
		}
	// Dino Chang 2005/03/03
	}
	// Dino

} /* end pppd_start */


/*
 * called by asp_setpppoe() only now. the table driver cannot call this
 * function directly, so cannot install this function as a callback in
 * rtl8651_setPppoeSessionHangUp().
 * note: pppd_stop can only be called by user thru asp engine, so there
 *       manual hangup must be true !!
 */
int32 pppd_stop(uint32 pppObjId)
{
	uint8 tempStr[20];
	pppoeCfg_t	pppoeCfgPtr;
	
	/* set flag to record the Mannual hangup event */	
	pRomeCfgParam->pppoeCfgParam[pppObjId].manualHangup = 1;
	sprintf(tempStr,"/var/run/pppd%u.pid",pppObjId);
	pRomeCfgParam->pppoeCfgParam[pppObjId].dialState=PPPOECFGPARAM_DIALSTATE_OFF;	
	
	pppoeCfgPtr.defaultSession=pRomeCfgParam->pppoeCfgParam[pppObjId].defaultSession;	
	pppoeCfgPtr.sessionId=pRomeCfgParam->pppoeCfgParam[pppObjId].sessionId;
	memcpy((char*)&pppoeCfgPtr.ipAddr,(char*)&pRomeCfgParam->pppoeCfgParam[pppObjId].ipAddr,4);
	memcpy((char*)&pppoeCfgPtr.ipMask,(char*)&pRomeCfgParam->pppoeCfgParam[pppObjId].ipMask,4);
	memcpy(&(pppoeCfgPtr.lanType),&(pRomeCfgParam->pppoeCfgParam[pppObjId].lanType),4);
	re865xIoctl("eth0",PPPOEDOWN_CFG,(uint32)&pppoeCfgPtr,pppObjId,0);
	
	//do_kill(tempStr);
	send_signal(tempStr,1); //Hang Up

	return 0;
} /* end pppd_stop */

#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)
/*
 * start pptp 
 */
void pptp_start(uint32 identity)
{
	FILE * f1, * f2;
	//romeCfgParam_t * pRomeCfgParam;
	char    idStr[4];
	char    papStr[20];
	char    pppOptionStr[20];
	char    chapStr[20];
	uint8   tmp[160];

#ifdef __uClinux__ 
#else //MMU
//	sprintf(tmp,"/var/run/pptp.pid");
//	send_signal(tmp,9);
//	unlink(tmp);
//	unlink("/var/run/pppd0.pid");
	send_signal("/var/run/pptp.pid", 9);
	send_signal("/var/run/pppd0.pid", 15);
	unlink("/var/run/pptp.pid");
	unlink("/var/run/pppd0.pid");
#endif	

#if 0
	if (getRomeCfgParamPtr(&pRomeCfgParam))
	{
		printf("pppd_start: getRomeCfgParamPtr failed\n");
		return;
	}
#endif
	
	f1 = fopen("/etc/pptp.options","r");
	if (!f1)
	{
		printf("pppd_start: fopen /etc/pptp.options failed\n");
		return;
	}
		
	sprintf(papStr,"/var/ppp/pap%u",identity);
	f2 = fopen(papStr,"w+");
	if (!f2)
	{
		printf("pppd_start: fopen %s failed\n",papStr);
		return;
	}
	
	sprintf(tmp,"%s * %s\n" ,pRomeCfgParam->pptpCfgParam.username,pRomeCfgParam->pptpCfgParam.password);
	fputs(tmp,f2);
	fclose(f2);
	
	sprintf(chapStr,"/var/ppp/chap%u",identity);
	f2 = fopen(chapStr,"w+");
	if (!f2)
	{
		printf("pppd_start: fopen %s failed\n",chapStr);
		return;
	}
	sprintf(tmp,"NTDOMAIN %s %s\n" ,pRomeCfgParam->pptpCfgParam.username,pRomeCfgParam->pptpCfgParam.password);
	fputs(tmp,f2);
	sprintf(tmp,"%s NTDOMAIN %s\n" ,pRomeCfgParam->pptpCfgParam.username,pRomeCfgParam->pptpCfgParam.password);
	fputs(tmp,f2);	
	fclose(f2);
			
	sprintf(pppOptionStr,"/var/ppp/options%u",identity);
	f2 = fopen(pppOptionStr,"w+");
	while (!feof(f1))
	{
		fgets(tmp,160,f1);
		fputs(tmp,f2);
	}			
	fclose(f1);
	
	sprintf(tmp,"mtu %d mru %d\n",pRomeCfgParam->pptpCfgParam.mtu,pRomeCfgParam->pptpCfgParam.mtu);
	fputs(tmp,f2);	
	sprintf(tmp,"name %s\n",pRomeCfgParam->pptpCfgParam.username);	
	fputs(tmp,f2);
	
	fputs("remotename NTDOMAIN\n",f2);
	
	/* generate idle option (auto disconnect) for pppd's reference */
	fprintf(f2,"idle %u\n",pRomeCfgParam->pptpCfgParam.silentTimeout);

	fprintf(f2,"defaultroute\n");

	// Dino Chang 2005/03/03
	if (pRomeCfgParam->pptpCfgParam.autoReconnect != 1)
		fprintf(f2,"nopersist\n");
	else
		fprintf(f2,"persist\n");
	// Dino
		
	fclose(f2);

	rtl8651_setNetMtu("eth0",pRomeCfgParam->pptpCfgParam.mtu);

	if (pRomeCfgParam->pptpCfgParam.demand==1)
	{
		pRomeCfgParam->pptpCfgParam.dialState=PPPOECFGPARAM_DIALSTATE_DIALED_WAITING;
		printf("Add Demand Route\n");
		AddDemandRoute(4,1); // 4: used for signal 44.		
	}
	else
	// Dino Chang 2005/03/03
	if (pRomeCfgParam->pptpCfgParam.autoReconnect==1)
	// Dino
	{
		pRomeCfgParam->pptpCfgParam.dialState=PPPOECFGPARAM_DIALSTATE_DIALED_TRYING;

		//sprintf(tempStr,"/bin/pptp %u.%u.%u.%u file /var/ppp/options0 &"
#ifdef __uClinux__		
		f2=fopen("/var/run/pptp.pid","r");
		if(f2)
		{		
			fclose(f2);
			send_signal("/var/run/pptp.pid",SIGUSR2);
		}
		else
#endif			
		{
			sprintf(tmp,"/bin/pptp %u.%u.%u.%u &"
			,pRomeCfgParam->pptpCfgParam.svAddr[0]
			,pRomeCfgParam->pptpCfgParam.svAddr[1]
			,pRomeCfgParam->pptpCfgParam.svAddr[2]
			,pRomeCfgParam->pptpCfgParam.svAddr[3]);
			system(tmp);
		}

		
	}
} /* end pptp_start */


/*
 * start pptp 
 */
void l2tp_start(uint32 identity)
{
	FILE * f1, * f2;
	//romeCfgParam_t * pRomeCfgParam;
	char    idStr[4];
	char    papStr[20];
	char    pppOptionStr[20];
	char    chapStr[20];
	char	tmp[160];


	if((pRomeCfgParam->ifCfgParam[0].connType==IFCFGPARAM_CONNTYPE_DHCPL2TP)&&
		(pRomeCfgParam->l2tpCfgParam.dialState==L2TPCFGPARAM_DIALSTATE_DIALED_DHCP_DISCOVER))
	{
		udhcpc_start();	
		return;
	}
	
#ifdef __uClinux__ 
#else //MMU
	sprintf(tmp,"/var/run/l2tpd.pid");
	send_signal(tmp,9);
	unlink(tmp);
	unlink("/var/run/pppd0.pid");
#endif	

#if 0
	if (getRomeCfgParamPtr(&pRomeCfgParam))
	{
		printf("pppd_start: getRomeCfgParamPtr failed\n");
		return;
	}
#endif
	
	f1 = fopen("/etc/l2tp.options","r");
	if (!f1)
	{
		printf("pppd_start: fopen /etc/l2tp.options failed\n");
		return;
	}
		
	sprintf(papStr,"/var/ppp/pap%u",identity);
	f2 = fopen(papStr,"w+");
	if (!f2)
	{
		printf("pppd_start: fopen %s failed\n",papStr);
		return;
	}
	
	sprintf(tmp,"%s * %s\n" ,pRomeCfgParam->l2tpCfgParam.username,pRomeCfgParam->l2tpCfgParam.password);
	fputs(tmp,f2);
	fclose(f2);
	
	sprintf(chapStr,"/var/ppp/chap%u",identity);
	f2 = fopen(chapStr,"w+");
	if (!f2)
	{
		printf("pppd_start: fopen %s failed\n",chapStr);
		return;
	}
	sprintf(tmp,"NTDOMAIN %s %s\n" ,pRomeCfgParam->l2tpCfgParam.username,pRomeCfgParam->l2tpCfgParam.password);
	fputs(tmp,f2);
	sprintf(tmp,"%s NTDOMAIN %s\n" ,pRomeCfgParam->l2tpCfgParam.username,pRomeCfgParam->l2tpCfgParam.password);
	fputs(tmp,f2);	
	fclose(f2);
			
	sprintf(pppOptionStr,"/var/ppp/options%u",identity);
	f2 = fopen(pppOptionStr,"w+");
	while (!feof(f1))
	{
		fgets(tmp,160,f1);
		fputs(tmp,f2);
	}			
	fclose(f1);
	
	sprintf(tmp,"mtu %d mru %d\n",pRomeCfgParam->l2tpCfgParam.mtu,pRomeCfgParam->l2tpCfgParam.mtu);
	fputs(tmp,f2);	
	sprintf(tmp,"name %s\n",pRomeCfgParam->l2tpCfgParam.username);	
	fputs(tmp,f2);
	
	fputs("remotename NTDOMAIN\n",f2);
	
	/* generate idle option (auto disconnect) for pppd's reference */
	fprintf(f2,"idle %u\n",pRomeCfgParam->l2tpCfgParam.silentTimeout);

	fprintf(f2,"defaultroute\n");
		
	// Dino Chang 2005/03/03
	if (pRomeCfgParam->l2tpCfgParam.autoReconnect != 1)
		fprintf(f2,"nopersist\n");
	else
		fprintf(f2,"persist\n");
	// Dino

	fclose(f2);

	// L2TP configuration file
	f2 = fopen("/var/l2tpd.conf","w+");
	fprintf(f2,"[lac test]\n");
	fprintf(f2,"lns=%u.%u.%u.%u\n",pRomeCfgParam->l2tpCfgParam.svAddr[0],pRomeCfgParam->l2tpCfgParam.svAddr[1],pRomeCfgParam->l2tpCfgParam.svAddr[2],pRomeCfgParam->l2tpCfgParam.svAddr[3]);
	fprintf(f2,"pppoptfile = /etc/config/l2tpd-pppd.conf\n");
	fprintf(f2,"length bit = yes\n");	
	fprintf(f2,"redial = no\n");
//	fprintf(f2,"redial timeout = 15\n");
	fprintf(f2,"autodial = yes\n");
	fclose(f2);

	// L2TP PPPD conifguration file
	f2 = fopen("/var/l2tpd-pppd.conf","w+");
	fprintf(f2,"user %s\n",pRomeCfgParam->l2tpCfgParam.username);
	fprintf(f2,"asyncmap 0\n");
	fprintf(f2,"noauth\n");
	fprintf(f2,"defaultroute\n");
	// Dino Chang 2005/03/03
	if (pRomeCfgParam->l2tpCfgParam.autoReconnect != 1)
		fprintf(f2,"nopersist\n");
	else
		fprintf(f2,"persist\n");
	// Dino
	fprintf(f2,"mru %d\nmtu %d\n",pRomeCfgParam->l2tpCfgParam.mtu,pRomeCfgParam->l2tpCfgParam.mtu);
	fclose(f2);	

	rtl8651_setNetMtu("eth0",pRomeCfgParam->l2tpCfgParam.mtu);

	if (pRomeCfgParam->l2tpCfgParam.demand==1)
	{
		pRomeCfgParam->l2tpCfgParam.dialState=PPPOECFGPARAM_DIALSTATE_DIALED_WAITING;
		AddDemandRoute(5,1); //used for signal 45
	}
	else
	// Dino Chang 2005/03/03
	if (pRomeCfgParam->l2tpCfgParam.autoReconnect==1)
	// Dino
	{
		pRomeCfgParam->l2tpCfgParam.dialState=PPPOECFGPARAM_DIALSTATE_DIALED_TRYING;
#ifdef __uClinux__
		f2=fopen("/var/run/l2tpd.pid","r");
		if(f2)
		{		
			fclose(f2);
			send_signal("/var/run/l2tpd.pid",SIGUSR2);
		}		
		else
#endif			
		{
			sprintf(tmp,"/bin/l2tpd &");		
			system(tmp);
		}
		
	}
} /* end pptp_start */
#endif


/* DNS Proxy*/
void runDNSProxy()
{
	char		dns_ip_string_1[MAX_IP_STRING_LEN];
	char		dns_ip_string_2[MAX_IP_STRING_LEN];
//	char		local_ip_string[MAX_IP_STRING_LEN];
	char	pname[]="/bin/dnrd";
	int		ipAddr = 0;
	int		boundIP = 0;
	FILE *	f = NULL;
	int		pid = 0;

	// fill server list config file
	f = fopen("/var/run/dnrd.serv", "w");
	if (f == NULL)
	{
		printf("Can NOT set server list of DNRD daemon, exit.\n");
		return;
	}

	bzero(dns_ip_string_1, MAX_IP_STRING_LEN);
	bzero(dns_ip_string_2, MAX_IP_STRING_LEN);
//	bzero(local_ip_string, MAX_IP_STRING_LEN);
	
	memcpy((void*)&boundIP,(void*)&pRomeCfgParam->ifCfgParam[1].ipAddr,
			sizeof(boundIP));

	memcpy((void*)&ipAddr,(void*)&pRomeCfgParam->ifCfgParam[0].dnsPrimaryAddr,
			sizeof(ipAddr));
	sprintf(dns_ip_string_1,"%u.%u.%u.%u",(ipAddr>>24)&0xff, (ipAddr>>16)&0xff, 
			(ipAddr>>8)&0xff, ipAddr&0xff);

	memcpy((void*)&ipAddr,(void*)&pRomeCfgParam->ifCfgParam[0].dnsSecondaryAddr,
			sizeof(ipAddr));
	sprintf(dns_ip_string_2,"%u.%u.%u.%u",(ipAddr>>24)&0xff, (ipAddr>>16)&0xff, 
			(ipAddr>>8)&0xff, ipAddr&0xff);

//	sprintf(local_ip_string,"%u.%u.%u.%u",(boundIP>>24)&0xff,
//				  (boundIP>>16)&0xff,(boundIP>>8)&0xff,
//				  boundIP&0xff);


	fprintf(f,"%s\n", dns_ip_string_1);
	fprintf(f,"%s\n", dns_ip_string_2);
	fclose(f);

	f = fopen("/var/run/dnrd.pid","r");
	if (!f)
	{
		goto forkNew;
	}

	fscanf(f, "%d", &pid);

// Dino Chang 2005/02/02
#if 0
#ifndef __uClinux__
	if (kill(pid, 9))
#else
	if (kill(pid, SIGHUP))
#endif		
#endif
// Dino
// Dino Chang 2005/04/28
#if 0
	if(kill(pid, SIGTERM))
	{
		// process not exist
		fclose(f);
		unlink("/var/run/dnrd.pid");
		goto forkNew;
	}else
	{	// hangup success
		fclose(f);
		return;
	}
#endif
	kill(pid, SIGTERM);
	fclose(f);
// Dino


forkNew:
	{
		char tmp[32];
		sprintf(tmp,"%s &", pname);
		system(tmp);
	}
	return;
}
char *mgmtUserName()
{

	return (char*)&pRomeCfgParam->mgmtCfgParam[0].name;
}
char *mgmtPassword()
{
	return (char*)&pRomeCfgParam->mgmtCfgParam[0].password;
}
#ifndef __uClinux__
int  rtl865x_mmap(int base,int length) // for flash , register mapping
{
	int sg_fd;
	int new_base;
	if ((sg_fd = open("/dev/mem", O_RDWR)) < 0) 
	       printf("open fail\n");
    	new_base = mmap((void*)0x0, length, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, sg_fd, base);
	close(sg_fd);
	if (new_base==MAP_FAILED)
	{
		fprintf(stderr,"MAP FAIL\n");
		return 0;	
	}
	return new_base;
}

int  rtl865x_kmmap(int base,int length) //for kmalloc mapping
{
	int sg_fd;
	int new_base;
	if ((sg_fd = open("/dev/node1", O_RDWR)) < 0) 
	       printf("open fail\n");
    	new_base = mmap((void*)0x0, length, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, sg_fd, base);
	close(sg_fd);
	if (new_base==MAP_FAILED)
	{
		fprintf(stderr,"MAP FAIL\n");
		return 0;	
	}
	return new_base;
}

#endif
/* end board.c */
