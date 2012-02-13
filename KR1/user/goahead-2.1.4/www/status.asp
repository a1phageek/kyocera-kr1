<html>
<head>
<meta http-equiv="Content-Type" content="text/html">
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Cache-Control" CONTENT="no-cache">
<title>RTL8650 Demo System Status</title>
</head>
<body bgcolor="#ffffff" text="#000000">
<table cellSpacing=1 cellPadding=2 border=0>
	<tr><td colspan=2><b>System</b></td></tr>
	<tr><td bgColor=#aaddff>Manufacturer</td><td bgColor=#ddeeff>Realtek Semiconductor Corporation</td></tr>
	<tr><td bgColor=#aaddff>Product Model</td><td bgColor=#ddeeff>RTL8650/RTL8651</td></tr>
	<tr><td bgColor=#aaddff>Firmware Version</td><td bgColor=#ddeeff><% asp_configChanged("CONFIG_CHANGE_FW_VER"); %><% asp_flashGetString("fwVer");%></td></tr>
	<tr><td bgColor=#aaddff>Firmware Date</td><td bgColor=#ddeeff><% asp_configChanged("CONFIG_CHANGE_FW_VER"); %><% asp_flashGetString("fwDate");%></td></tr>
	<tr><td bgColor=#aaddff>Loader Version</td><td bgColor=#ddeeff><% asp_configChanged("CONFIG_CHANGE_LD_VER"); %><%asp_flashGetString("ldVer");%></td></tr>
	<tr><td bgColor=#aaddff>Rome Driver Version</td><td bgColor=#ddeeff><% asp_configChanged("CONFIG_CHANGE_LD_VER"); %><%asp_flashGetString("romeDrvVer");%></td></tr>
	<tr><td bgColor=#aaddff>NAT</td><td bgColor=#ddeeff><% asp_configChanged("CONFIG_CHANGE_NAT"); %><%asp_flashGetString("nat");%></td></tr>
	<tr><td bgColor=#aaddff>L4 Switching</td><td bgColor=#ddeeff><% asp_configChanged("CONFIG_CHANGE_HW_ACCEL"); %><%asp_flashGetString("hwAcc");%></td></tr>
	<tr><td colspan=2>&nbsp;</td></tr>
	<tr><td colspan=2><b>LAN</b></td></tr>
	<tr><td bgColor=#aaddff>IP Address</td><td bgColor=#ddeeff><% asp_configChanged("CONFIG_CHANGE_LAN_IP"); %><% asp_flashGetCfgParam("lan11"); %></td></tr>
	<tr><td bgColor=#aaddff>Subnet Mask</td><td bgColor=#ddeeff><% asp_configChanged("CONFIG_CHANGE_LAN_MASK"); %><% asp_flashGetCfgParam("lan12"); %></td></tr>
	<tr><td bgColor=#aaddff>MAC Address</td><td bgColor=#ddeeff><% asp_configChanged("CONFIG_CHANGE_LAN_MAC"); %><% asp_flashGetString("lanMac");%></td></tr>
	<tr><td bgColor=#aaddff>DHCP Server</td><td bgColor=#ddeeff><% asp_configChanged("CONFIG_CHANGE_DHCPS"); %><%asp_flashGetString("dhcpsToggle");%></td></tr>
	<% asp_getLanPortStatus();%>	
	<tr><td colspan=2>&nbsp;</td></tr>
	<tr><td colspan=2><b>WAN</b></td></tr>
	<tr><td bgColor=#aaddff>Connection Method</td><td bgColor=#ddeeff><% asp_configChanged("CONFIG_CHANGE_CONN_TYPE"); %><%asp_flashGetString("wanIpType");%></td></tr>
	<tr><td bgColor=#aaddff>IP Address</td><td bgColor=#ddeeff><% asp_configChanged("CONFIG_CHANGE_WAN_IP"); %><% asp_getWanAddress("wanip"); %></td></tr>
	<tr><td bgColor=#aaddff>Subnet Mask</td><td bgColor=#ddeeff><% asp_configChanged("CONFIG_CHANGE_WAN_MASK"); %><% asp_getWanAddress("wanmask"); %></td></tr>
	<tr><td bgColor=#aaddff>Default Gateway</td><td bgColor=#ddeeff><% asp_configChanged("CONFIG_CHANGE_WAN_GW"); %><% asp_getWanAddress("gateway"); %></td></tr>
	<tr><td bgColor=#aaddff>DNS IP Address</td><td bgColor=#ddeeff><% asp_configChanged("CONFIG_CHANGE_WAN_DNS"); %><% asp_getWanAddress("dns");%></td></tr>
	<tr><td bgColor=#aaddff>MAC Address</td><td bgColor=#ddeeff><% asp_configChanged("CONFIG_CHANGE_WAN_MAC"); %><%asp_flashGetString("wanMac");%></td></tr>
	<% asp_getWanPortStatus();%>	
	
	<tr><td colspan=2>&nbsp;</td></tr>
	<tr><td colspan=2><b>WLAN1 - <%asp_wlanBasic("name0");%></b></td></tr>
	<tr><td bgColor=#aaddff>Status</td><td bgColor=#ddeeff><%asp_wlanBasic("active0");%></td></tr>
	<tr><td bgColor=#aaddff>SSID</td><td bgColor=#ddeeff><% asp_wlanBasic("ssid0"); %></td></tr>
	<tr><td bgColor=#aaddff>Channel</td><td bgColor=#ddeeff><% asp_wlanBasic("channel0"); %></td></tr>

	<tr><td colspan=2>&nbsp;</td></tr>
	<tr><td colspan=2><b>WLAN2 -<%asp_wlanBasic("name1");%></b></td></tr>
	<tr><td bgColor=#aaddff>Status</td><td bgColor=#ddeeff><%asp_wlanBasic("active1");%></td></tr>
	<tr><td bgColor=#aaddff>SSID</td><td bgColor=#ddeeff><% asp_wlanBasic("ssid1"); %></td></tr>
	<tr><td bgColor=#aaddff>Channel</td><td bgColor=#ddeeff><% asp_wlanBasic("channel"); %></td></tr>
	
	<% asp_statusExtended(); %>
</table>
<% asp_configChanged("CONFIG_CHANGE_MESSAGE"); %>
<BR><BR>
<img src="ws_button3.gif">
</body>
</html>
