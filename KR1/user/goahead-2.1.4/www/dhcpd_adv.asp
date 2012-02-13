<html>
<head>
<meta http-equiv="Content-Type" content="text/html">
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Cache-Control" CONTENT="no-cache">
<title>DHCP Server Configuration</title>
<SCRIPT LANGUAGE="JavaScript">

function Disab() {

	if(lanform.dhcps_switch.checked==false)
	{
		lanform.dhcps_ippool_start.disabled=true;
		lanform.dhcps_ippool_end.disabled=true;

		lanform.mac_1_1.disabled=true;
		lanform.mac_1_2.disabled=true;
		lanform.mac_1_3.disabled=true;
		lanform.mac_1_4.disabled=true;
		lanform.mac_1_5.disabled=true;
		lanform.mac_1_6.disabled=true;
		lanform.dip_1_1.disabled=true;
		lanform.dip_1_2.disabled=true;
		lanform.dip_1_3.disabled=true;
		lanform.dip_1_4.disabled=true;
		lanform.mac_2_1.disabled=true;
		lanform.mac_2_2.disabled=true;
		lanform.mac_2_3.disabled=true;
		lanform.mac_2_4.disabled=true;
		lanform.mac_2_5.disabled=true;
		lanform.mac_2_6.disabled=true;
		lanform.dip_2_1.disabled=true;
		lanform.dip_2_2.disabled=true;
		lanform.dip_2_3.disabled=true;
		lanform.dip_2_4.disabled=true;
	}
	else
	{
		lanform.dhcps_ippool_start.disabled=false;
		lanform.dhcps_ippool_end.disabled=false;

		lanform.mac_1_1.disabled=false;
		lanform.mac_1_2.disabled=false;
		lanform.mac_1_3.disabled=false;
		lanform.mac_1_4.disabled=false;
		lanform.mac_1_5.disabled=false;
		lanform.mac_1_6.disabled=false;
		lanform.dip_1_1.disabled=false;
		lanform.dip_1_2.disabled=false;
		lanform.dip_1_3.disabled=false;
		lanform.dip_1_4.disabled=false;
		lanform.mac_2_1.disabled=false;
		lanform.mac_2_2.disabled=false;
		lanform.mac_2_3.disabled=false;
		lanform.mac_2_4.disabled=false;
		lanform.mac_2_5.disabled=false;
		lanform.mac_2_6.disabled=false;
		lanform.dip_2_1.disabled=false;
		lanform.dip_2_2.disabled=false;
		lanform.dip_2_3.disabled=false;
		lanform.dip_2_4.disabled=false;
	}
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

<form method="get" action="/goform/asp_dhcpServerAdvance" name=lanform>
	<input type=hidden name=lansm_4  value=<% asp_flashGetIpElement("mask114");%>>
	<input type=hidden name=lanip_4 value= <% asp_flashGetIpElement("lan114");%> >
<b>DHCP Server Configuration</b>
<table cellSpacing=1 cellPadding=2 border=0>

	
	<tr>
	<td bgColor=#aaddff>DHCP Server Status</td>
	<td bgColor=#ddeeff><input type=checkbox name=dhcps_switch  onClick="Disab();" <%asp_flashGetString("dhcps_check");%>>Enable</td>
	</tr>
	
   	 <tr>
    	<td bgColor=#aaddff>DHCP Server IP Pool Start IP</td>
    	<td bgColor=#ddeeff>
	<input type=text name=lanip_1 size=3 maxlength=3 disabled  value=<% asp_flashGetIpElement("lan111"); %>  >.
	<input type=text name=lanip_2 size=3 maxlength=3  disabled value= <% asp_flashGetIpElement("lan112"); %> >.
	<input type=text name=lanip_3 size=3 maxlength=3  disabled value=<% asp_flashGetIpElement("lan113"); %> >.
	<input type=text name=dhcps_ippool_start size=3 maxlength=3 value=<% asp_flashGetString("dhcpsS");%> >
	</td>
	</tr>
	<tr>
	<td bgColor=#aaddff>DHCP Server IP Pool End IP</td>	
    	<td bgColor=#ddeeff>
	<input type=text name=lanip_1 size=3 maxlength=3 disabled  value=<% asp_flashGetIpElement("lan111"); %>  >.
	<input type=text name=lanip_2 size=3 maxlength=3  disabled value= <% asp_flashGetIpElement("lan112"); %> >.
	<input type=text name=lanip_3 size=3 maxlength=3   disabled  value=<% asp_flashGetIpElement("lan113"); %> >.
	<input type=text name=dhcps_ippool_end size=3 maxlength=3 value=<%asp_flashGetString("dhcpsE");%> >
		</td>
	</tr>
</table>
<p>
<b>Static DHCP leases</b>
<p>
<table cellSpacing=1 cellPadding=2 border=0>
	<tr><td bgColor=#aaddff>No.</td><td bgColor=#aaddff>Hardware Address</td><td bgColor=#aaddff>Assigned IP Address</td></tr>
	<tr>
		<td bgColor=#ddeeff>1</td>
		<td bgColor=#ddeeff>
			<input type=text name=mac_1_1 size=2 maxlength=2 enable value=<% asp_flashGetString("dhcpmac_1_1"); %>>:
			<input type=text name=mac_1_2 size=2 maxlength=2 enable value=<% asp_flashGetString("dhcpmac_1_2"); %>>:
			<input type=text name=mac_1_3 size=2 maxlength=2 enable value=<% asp_flashGetString("dhcpmac_1_3"); %>>:
			<input type=text name=mac_1_4 size=2 maxlength=2 enable value=<% asp_flashGetString("dhcpmac_1_4"); %>>:
			<input type=text name=mac_1_5 size=2 maxlength=2 enable value=<% asp_flashGetString("dhcpmac_1_5"); %>>:
			<input type=text name=mac_1_6 size=2 maxlength=2 enable value=<% asp_flashGetString("dhcpmac_1_6"); %>>
		</td>
		<td bgColor=#ddeeff>
			<input type=text name=dip_1_1 size=3 maxlength=3 enable value=<% asp_flashGetString("dhcpip_1_1"); %>>.
			<input type=text name=dip_1_2 size=3 maxlength=3 enable value=<% asp_flashGetString("dhcpip_1_2"); %>>.
			<input type=text name=dip_1_3 size=3 maxlength=3 enable value=<% asp_flashGetString("dhcpip_1_3"); %>>.
			<input type=text name=dip_1_4 size=3 maxlength=3 enable value=<% asp_flashGetString("dhcpip_1_4"); %>>
		</td>
	</tr>
	<tr>
		<td bgColor=#ddeeff>2</td>
		<td bgColor=#ddeeff>
			<input type=text name=mac_2_1 size=2 maxlength=2 enable value=<% asp_flashGetString("dhcpmac_2_1"); %>>:
			<input type=text name=mac_2_2 size=2 maxlength=2 enable value=<% asp_flashGetString("dhcpmac_2_2"); %>>:
			<input type=text name=mac_2_3 size=2 maxlength=2 enable value=<% asp_flashGetString("dhcpmac_2_3"); %>>:
			<input type=text name=mac_2_4 size=2 maxlength=2 enable value=<% asp_flashGetString("dhcpmac_2_4"); %>>:
			<input type=text name=mac_2_5 size=2 maxlength=2 enable value=<% asp_flashGetString("dhcpmac_2_5"); %>>:
			<input type=text name=mac_2_6 size=2 maxlength=2 enable value=<% asp_flashGetString("dhcpmac_2_6"); %>>
		</td>
		<td bgColor=#ddeeff>
			<input type=text name=dip_2_1 size=3 maxlength=3 enable value=<% asp_flashGetString("dhcpip_2_1"); %>>.
			<input type=text name=dip_2_2 size=3 maxlength=3 enable value=<% asp_flashGetString("dhcpip_2_2"); %>>.
			<input type=text name=dip_2_3 size=3 maxlength=3 enable value=<% asp_flashGetString("dhcpip_2_3"); %>>.
			<input type=text name=dip_2_4 size=3 maxlength=3 enable value=<% asp_flashGetString("dhcpip_2_4"); %>>
		</td>
	</tr>
	

    <tr>
    	<td colspan=3 align=center>
    		<input type="button" value="Save" onclick="changeStartEnd();">    		
    		&nbsp;&nbsp;&nbsp;&nbsp;    	
    		<input type="reset" value="Reset">	
    		
    	</td>
    </tr>
      
	<% asp_dhcpServerLeaseList(); %>
</table>
	</form>

</table>
<SCRIPT language="JavaScript">

</SCRIPT> 
</body>
</html>
