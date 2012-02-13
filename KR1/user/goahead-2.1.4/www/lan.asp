<html>
<head>
<meta http-equiv="Content-Type" content="text/html;">
<title>LAN Interface Configuration</title>
<SCRIPT LANGUAGE="JavaScript">

function Disab() {

	if(lanform.dhcps_switch.checked==false)
	{
		lanform.dhcps_ippool_start.disabled=true;
		lanform.dhcps_ippool_end.disabled=true;
	}
	else
	{
		lanform.dhcps_ippool_start.disabled=false;
		lanform.dhcps_ippool_end.disabled=false;
	}
}

function ChangeIP() {
	lanform.dhcps11.value=lanform.lanip_1.value;
	lanform.dhcps12.value=lanform.lanip_2.value;
	lanform.dhcps13.value=lanform.lanip_3.value;
	lanform.dhcps21.value=lanform.lanip_1.value;
	lanform.dhcps22.value=lanform.lanip_2.value;
	lanform.dhcps23.value=lanform.lanip_3.value;
}

function changeStartEnd() {
	var seterr=0;
	var mask=parseInt(lanform.lansm_4.value)&0xff;
	var ip=parseInt(lanform.lanip_4.value)&0xff;
	var ipmin=(mask&ip);
	var ipmax=(mask&ip)|((~mask)&0xff);
	var startip=parseInt(lanform.dhcps_ippool_start.value);
	var endip=parseInt(lanform.dhcps_ippool_end.value);
	
	if(startip > endip)
	{
		lanform.dhcps_ippool_start.value = endip;
		lanform.dhcps_ippool_end.value = startip;
	}
	
	
	if(lanform.dhcps_switch.checked==true)
	{
		if((startip<ipmin)||(startip>ipmax)) seterr=1;
		if((endip<ipmin)||(endip>ipmax)) seterr=1;
		
		if(seterr==0)
		{
			lanform.submit();
		}
		else
		{
			alert('DHCP server IP pool doesn\'t match with subnet mask (allow range: '+ipmin+'-'+ipmax+').');
		}
	}
	else
	{
		lanform.submit();
	}
}
</SCRIPT>
</head>
<body bgcolor="#ffffff" text="#000000">

<form method="post" action="/goform/asp_setLan" name=lanform>

<b>LAN Interface Configuration</b>
<table cellSpacing=1 cellPadding=2 border=0>

    <tr><td  bgColor=#aaddff>IP Address</td>
    <td bgColor=#ddeeff>
	<input type=text name=lanip_1 size=3 maxlength=3 onKeyUp="ChangeIP();" value=<% asp_flashGetIpElement("lan111"); %>  >.
	<input type=text name=lanip_2 size=3 maxlength=3 onKeyUp="ChangeIP();" value= <% asp_flashGetIpElement("lan112"); %> >.
	<input type=text name=lanip_3 size=3 maxlength=3 onKeyUp="ChangeIP();" value=<% asp_flashGetIpElement("lan113"); %> >.
	<input type=text name=lanip_4 size=3 maxlength=3 value= <% asp_flashGetIpElement("lan114");%> >
	</td></tr>
    <tr><td bgColor=#aaddff>Subnet Mask</td>
    <td bgColor=#ddeeff>
	<input type=text name=lansm_1 size=3 maxlength=3 value=<% asp_flashGetIpElement("mask111");%> >.
	<input type=text name=lansm_2 size=3 maxlength=3 value=<% asp_flashGetIpElement("mask112");%> >.
	<input type=text name=lansm_3 size=3 maxlength=3 value=<% asp_flashGetIpElement("mask113");%> >.
	<input type=text name=lansm_4 size=3 maxlength=3 value=<% asp_flashGetIpElement("mask114");%>>

	<tr><td colspan=2></td></tr>
	
	<tr>
		<td bgColor=#aaddff>DHCP Server Status</td>
		<td bgColor=#ddeeff><input type=checkbox name=dhcps_switch  onClick="Disab();" <% asp_flashGetString("dhcps_check");%>>Enable</td>
	</tr>
	
    <tr>
    
    	<td bgColor=#aaddff>DHCP Server IP Pool Start IP</td>
    	<td bgColor=#ddeeff>
			<input type=text name=dhcps11 size=3 disabled>.
			<input type=text name=dhcps12 size=3 disabled>.
			<input type=text name=dhcps13 size=3 disabled>.
			<input type=text name=dhcps_ippool_start size=3 maxlength=3 value=<% asp_flashGetString("dhcpsS");%> >
		</td>
	</tr>
	
	<tr>
		<td bgColor=#aaddff>DHCP Server IP Pool End IP</td>	
    	<td bgColor=#ddeeff>
			<input type=text name=dhcps21 size=3 disabled>.
			<input type=text name=dhcps22 size=3 disabled>.
			<input type=text name=dhcps23 size=3 disabled>.
			<input type=text name=dhcps_ippool_end size=3 maxlength=3 value=<% asp_flashGetString("dhcpsE");%>   >
		</td>
	</tr>
	
    <tr>
    	<td colspan=2 align=center>
    		<input type="button" value="< Back" onclick="javascript:history.back();">
    		&nbsp;&nbsp;&nbsp;&nbsp;    		
    		<input type="button" value="Next >" onclick="changeStartEnd();">
    		&nbsp;&nbsp;&nbsp;&nbsp;
    		<input type="reset" value="Reset">
    		&nbsp;&nbsp;&nbsp;&nbsp;
    		<input type="button" value="Cancel" onclick="javascript:location.replace('status.asp');">
    		
    	</td>
    </tr>
    
	</form>

</table>
<SCRIPT language="JavaScript">
   Disab();
   ChangeIP();
</SCRIPT> 
</body>
</html>
