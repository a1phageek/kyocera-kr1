<html>
<head>
<meta http-equiv="Content-Type" content="text/html;">
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Cache-Control" CONTENT="no-cache">
<script language="javascript">
    var a=0,b=0,c=0,d=0,e=0;
   
    function menu()
    {
    	if(a==0)
    	{
    		document.all("spanMenu").style.display = '';
    		a=1;
    		if(b==1) document.all("spanMenu2").style.display = '';
    	}
    	else
    	{
    		document.all("spanMenu").style.display = 'none';	    		
		document.all("spanMenu2").style.display = 'none';	
		a=0;
    	}
    }

    function menu2()
    {
    	if(b==0)
    	{
    		document.all("spanMenu2").style.display = '';
    		b=1;
    	}
    	else
    	{
    		document.all("spanMenu2").style.display = 'none';	
    		b=0;
    	}
    }    
    function menu4()
    {
    	if(d==0)
    	{
    		document.all("spanMenu4").style.display = '';
    		d=1;
    	}
    	else
    	{
    		document.all("spanMenu4").style.display = 'none';	
    		d=0;
    	}
    }    
    function menu5()
    {
    	if(b==0)
    	{
    		document.all("spanMenu5").style.display = '';
    		b=1;
    	}
    	else
    	{
    		document.all("spanMenu5").style.display = 'none';	
    		b=0;
    	}
    }
    function menu3()
    {
		var exist0 = <% asp_wlanBasic("exist0"); %>;
		var exist1 = <% asp_wlanBasic("exist1"); %>;
		var total = exist0+exist1;

		if(c==0)
		{
			if(total==0){
				document.all("spanMenu3").style.display = 'none';
				document.all("spanMenu3_1").style.display = '';
			}else{
				document.all("spanMenu3_1").style.display = 'none';	
	    		document.all("spanMenu3").style.display = '';
			}
			c=1;
		}
		else
		{
			c=0;
			document.all("spanMenu3_1").style.display = 'none';	
    		document.all("spanMenu3").style.display = 'none';
		}
    }    

    function menu6()
    {
		var exist = <% asp_webcam("exist"); %>;
		if(e==0)
		{
			if(exist==0){
				document.all("spanMenu6").style.display = 'none';
				document.all("spanMenu6_1").style.display = '';
			}else{
				document.all("spanMenu6").style.display = '';
				document.all("spanMenu6_1").style.display = 'none';			   		
			}
			e=1;
		}
		else
		{
			e=0;
			document.all("spanMenu6").style.display = 'none';
			document.all("spanMenu6_1").style.display = 'none';	
		}
	}

</script>
</head>
<body background="menu_bg.gif" text="#000000">
<style type="text/css">
A:link    {  font-family: "Arial Narrow"; font-size: 13pt; text-decoration: none; color: #0840a0}
A:visited {  font-family: "Arial Narrow"; font-size: 13pt; text-decoration: none; color: #0840a0}
A:hover   {  font-family: "Arial Narrow"; font-size: 13pt; text-decoration: underline; color: red}
A:active {  color: #ff0000}
</style>
<table CELLSPACING=1 CELLPADDING=1>
<!--<tr><td><font face="Arial Narrow" size="+2"><B>Main Menu</font></td></tr>-->
<tr><td align=left><img src="realtek.gif"></td></tr>
<tr><td><a href="status.asp" target="show_area"><B>Status</a></td></tr>
<tr><td><a href="port.asp" target="show_area"><B>Setup Wizard</a></td></tr>
<tr><td><span id="spanHead5" onclick="menu5()">
		<a href="#"><B>LAN Services</a><BR>
		</span>
		<div id="spanMenu5" STYLE="display:none">
			<table CELLSPACING=0 CELLPADDING=0>
			<tr><td width=10></td><td><a href="dhcpd_adv.asp" target="show_area"><span style="font-size: 12pt;">DHCP Server</span></a></td></tr>
			<tr><td width=10></td><td><a href="upnp.asp" target="show_area"><span style="font-size: 12pt;">UPnP</span></a></td></tr>
			</table>
</td></tr>
<tr><td><span id="spanHead" onclick="menu()">
		<a href="#"><B>Dialed-up WAN</a><BR>
		</span>
		<div id="spanMenu" STYLE="display:none">
			<table CELLSPACING=0 CELLPADDING=0>
			<tr><td width=10></td><td><a href="pppoe.asp" target="show_area"><span style="font-size: 12pt;">PPPoE</span></a></td></tr>
			<tr><td width=10></td><td><a href="wanpptp.asp" target="show_area"><span style="font-size: 12pt;">PPTP</span></a></td></tr>
			<tr><td width=10></td><td><a href="wanl2tp.asp" target="show_area"><span style="font-size: 12pt;">L2TP</span></a></td></tr>
			<tr><td width=10></td><td><a href="wandhcpl2tp.asp" target="show_area"><span style="font-size: 12pt;">L2TP+DHCP</span></a></td></tr>
			</table>
</td></tr>
<tr><td><span id="spanHead2" onclick="menu3()">
		<a href="#"><B>Wireless LAN</a><BR>
		</span>
		<div id="spanMenu3" STYLE="display:none">
			<table CELLSPACING=0 CELLPADDING=0>
			<tr><td width=10></td><td><a href="wireless_basic.asp" target="show_area"><span style="font-size: 12pt;">Basic Setting</span></a></td></tr>
			<tr><td width=10></td><td><a href="wireless_advance.asp" target="show_area"><span style="font-size: 12pt;">Advance Setting</span></a></td></tr>
			<tr><td width=10></td><td><a href="wireless_security.asp" target="show_area"><span style="font-size: 12pt;">Security</span></a></td></tr>
			<tr><td width=10></td><td><a href="wireless_ac.asp" target="show_area"><span style="font-size: 12pt;">Access Control</span></a></td></tr>
			<tr><td width=10></td><td><a href="wireless_wds.asp" target="show_area"><span style="font-size: 12pt;">WDS</span></a></td></tr>
			</table>
		</div>
		<div id="spanMenu3_1" STYLE="display:none">
			<table CELLSPACING=0 CELLPADDING=0>
			<tr><td width=10></td><td><span style="font-size: 10pt;">No WLAN card (RTL8185?) detected</span></td></tr>
			</table>
		</div>
</td></tr>

<tr><td><span id="spanHead6" onclick="menu6()">
		<a href="#"><B>Home Monitor</a><BR>
		</span>
		<div id="spanMenu6" STYLE="display:none">
			<table CELLSPACING=0 CELLPADDING=0>
			<tr><td width=10></td><td><a href="webcam.asp" target="show_area"><span style="font-size: 12pt;">Web Camera</span></a></td></tr>
			</table>
		</div>
		<div id="spanMenu6_1" STYLE="display:none">
			<table CELLSPACING=0 CELLPADDING=0>
			<tr><td width=10></td><td><span style="font-size: 10pt;">No web camera detected</span></td></tr>
			</table>
		</div>
</td></tr>


<tr><td><span id="spanHead4" onclick="menu4()">
		<a href="#"><B>Gateway Features</a><BR>
		</span>
		<div id="spanMenu4" STYLE="display:none">
			<table CELLSPACING=0 CELLPADDING=0>
			<tr><td width=10></td><td><a href="acl.asp" target="show_area"><span style="font-size: 12pt;">Access Control List*</span></a></td></tr>
			<tr><td width=10></td><td><a href="serverp.asp" target="show_area"><span style="font-size: 12pt;">Virtual Server*</span></a></td></tr>
			<tr><td width=10></td><td><a href="urlfilter.asp" target="show_area"><span style="font-size: 12pt;">URL Filter*</span></a></td></tr>
			<tr><td width=10></td><td><a href="qos.asp" target="show_area"><span style="font-size: 12pt;">Port-based QoS*</span></a></td></tr>
			<tr><td width=10></td><td><a href="ratelimit.asp" target="show_area"><span style="font-size: 12pt;">Rate Policing QoS*</span></a></td></tr>
			<tr><td width=10></td><td><a href="ratio_qos.asp" target="show_area"><span style="font-size: 12pt;">Guaranteed/Spare Class QoS*</span></a></td></tr>
			<tr><td width=10></td><td><a href="routing.asp" target="show_area"><span style="font-size: 12pt;">Static Route*</span></a></div></td></tr>
			<tr><td width=10></td><td><a href="ddns.asp" target="show_area"><span style="font-size: 12pt;">Dynamic DNS</span></a></td></tr>
			<tr><td width=10></td><td><a href="specialap.asp" target="show_area"><span style="font-size: 12pt;">Special Application</span></a></td></tr>
			<tr><td width=10></td><td><a href="alg.asp" target="show_area"><span style="font-size: 12pt;">ALG</span></a></td></tr>
			<tr><td width=10></td><td><a href="dmz.asp" target="show_area"><span style="font-size: 12pt;">DMZ Host*</span></a></td></tr>
			<tr><td width=10></td><td><a href="dos.asp" target="show_area"><span style="font-size: 12pt;">Anti-DoS</span></a></td></tr>
			<tr><td width=10></td><td><a href="udpblocking.asp" target="show_area"><span style="font-size: 12pt;">UDP Blocking</span></a></td></tr>
			<tr><td width=10></td><td><a href="pbnat.asp" target="show_area"><span style="font-size: 12pt;">Protocol-based NAT </span></a></td></tr>
			<tr><td width=10></td><td><a href="ping.asp" target="show_area"><span style="font-size: 12pt;">Ping toolkit</span></a></td></tr>
			<tr><td width=10></td><td><a href="naptopt.asp" target="show_area"><span style="font-size: 12pt;">NAPT options*</span></a></td></tr>
			<tr><td colspan=2</td><span style="font-size: 10pt;">*:Hardware accelereted</span></tr>
			</table>
</td></tr>
<tr><td><a href="mgmt.asp" target="show_area"><B>Management</a></td></tr>
<tr><td>
	<span id="spanHead2" onclick="menu2()">
	<a href="#"><B>Event Log</a></span>
	<div id="spanMenu2" STYLE="display:none">
	<table CELLSPACING=0 CELLPADDING=0>
	<tr><td width=10></td><td><a href="eventlog.asp?action=system" target="show_area"><span style="font-size: 12pt;">System Log</span></a></td></tr>
	<tr><td width=10></td><td><a href="eventlog.asp?action=acl" target="show_area"><span style="font-size: 12pt;">ACL Log</span></a></td></tr>
	<tr><td width=10></td><td><a href="eventlog.asp?action=urlfilter" target="show_area"><span style="font-size: 12pt;">URL Filter Log</span></a></td></tr>
	<tr><td width=10></td><td><a href="eventlog.asp?action=dos" target="show_area"><span style="font-size: 12pt;">DoS Log</span></a></td></tr>
	<tr><td width=10></td><td><a href="eventlog.asp?action=new_napt" target="show_area"><span style="font-size: 12pt;">New Connection Log</span></a></td></tr>
	</table>
	</div>
</td></tr>
</body>
</html>
