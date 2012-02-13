<head>
<meta http-equiv="Content-Type" content="text/html; charset=big5">
<title>Device Port Configuration</title>
</head>
<SCRIPT LANGUAGE="JavaScript">

function selectSwitch()
{
	if (port.enable0.selectedIndex == 0) 
		port.interface0.disabled=true;
	else
		port.interface0.disabled=false;
	if (port.enable1.selectedIndex == 0) 
		port.interface1.disabled=true;
	else
		port.interface1.disabled=false;
	if (port.enable2.selectedIndex == 0) 
		port.interface2.disabled=true;
	else
		port.interface2.disabled=false;
	if (port.enable3.selectedIndex == 0) 
		port.interface3.disabled=true;
	else
		port.interface3.disabled=false;
	if (port.enable4.selectedIndex == 0) 
		port.interface4.disabled=true;
	else
		port.interface4.disabled=false;
	if (port.enable5.selectedIndex == 0) 
		port.interface5.disabled=true;
	else
		port.interface5.disabled=false;
	//Must prevent all ports be turned off. Keep at least one Port ON and let it be LAN.
	if(port.enable4.selectedIndex+port.enable3.selectedIndex+port.enable2.selectedIndex+
		port.enable1.selectedIndex+port.enable0.selectedIndex==0){
		port.enable0.selectedIndex=1;//turn on this default port.
		port.interface0.selectedIndex=0;//set as LAN so user can access gateway.
	}
	if(port.interface4.selectedIndex+port.interface3.selectedIndex+port.interface2.selectedIndex+
		port.interface1.selectedIndex+port.interface0.selectedIndex==5){
		port.interface0.selectedIndex=1;//turn on this default port.
		port.interface0.selectedIndex=0;//set as LAN so user can access gateway.
	}	
	printNatThroughput();
}


function printNatThroughput(){
	var lan=0, wan=0;
	if(port.enable0.selectedIndex==1){
		if(port.interface0.selectedIndex==0) 	lan+=200;
		else 	wan+=200;
	}
	if(port.enable1.selectedIndex==1){
		if(port.interface1.selectedIndex==0) 	lan+=200;
		else 	wan+=200;
	}
	if(port.enable2.selectedIndex==1){
		if(port.interface2.selectedIndex==0) 	lan+=200;
		else 	wan+=200;
	}
	if(port.enable3.selectedIndex==1){
		if(port.interface3.selectedIndex==0) 	lan+=200;
		else 	wan+=200;
	}
	if(port.enable4.selectedIndex==1){
		if(port.interface4.selectedIndex==0) 	lan+=200;
		else 	wan+=200;
	}
	if(port.enable5.selectedIndex==1){
		if(port.interface5.selectedIndex==0) 	lan+=200;
		else 	wan+=200;
	}
	if(lan<wan)
		str = lan.toString();
	else
		str=wan.toString();
	port.nat.value=str;
}

</SCRIPT>


<body bgcolor="#ffffff" text="#000000" onLoad="selectSwitch()">
<style>#cntdwn{ border-color: #eeffdd; background-color: white; border-width: 0px;font-size: 12pt;color: blue;text-align:right; font-weight:bold; }
</style>
<b>Device Port Config</b><BR>
<BR>
<table border=0><tr><td>
<table cellSpacing=1 cellPadding=2 border=0>
	<form method="post" action="/goform/asp_setPort" name="port">
<%
asp_port();
%>
	<tr><td colspan=3 align=center bgColor=#eeffdd>Max Routing/NAT full-dulplex switching throughput is
	<INPUT TYPE=text NAME=nat size=4 id=cntdwn>Mbps</td></tr>
	<tr>	
		<td colspan=3 align=center><input type=submit value="Next >">&nbsp;&nbsp;<input type=reset value=Reset></td>
	</tr>

	</form>
	
</table>
</td><td>
<ul>
<li>Port Number: Index of port selected. Indexing is from left to right (or from top to bottom) The leftmost (or top) port is Port 0.
<li>Interface
<ul>
	<li>LAN: Set as Home/SOHO network port. 
	<li>WAN: Connects to ISP/Internet.
</ul>
<li>ON: Port is enabled. 
<li>Off: Port is disabled. Interface setting ignored. No packet is accepted/forwarded on this port
</ul>
If all five ports from Port0~4 are Off or set to WAN, Port0 would be forced turned ON and set to LAN interface.<br>
</td>
</tr></table>
</body>
</html>
