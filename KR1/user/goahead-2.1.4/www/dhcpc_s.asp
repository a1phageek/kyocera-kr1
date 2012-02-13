<html>
<head>
<meta http-equiv="Content-Type" content="text/html">
<!--                                                           
<script language="JavaScript">


function Renewtoggle()
{
	var counter;
	
	if(frm1.dhcpcToggle.checked==false)
	{
		document.all("spanMenu").style.visibility = 'hidden';
	}	
	else
	{
		document.all("spanMenu").style.visibility = 'visible';
	}
}
</script>
-->
<title>DHCP Client Configuration</title>
</head>
<body bgcolor="#ffffff" text="#000000">
<form method="get" action="/goform/asp_setDhcpClient" name="frm1">
<b>DHCP Client</b>
<table cellSpacing=1 cellPadding=2 border=0>
    <tr><td bgColor=#aaddff>Enable</td><td bgColor=#ddeeff>
    <input type="checkbox" name="dhcpcToggle"         ></td></tr>
    <tr>
    	<td bgColor=#aaddff>Clone MAC</td> 
    	<td bgColor=#ddeeff>
    		<input type=checkbox name=cmacToggle vcmacToggle>Use&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    		<input type=text name=cmac1 size=2 maxlength=2 value=vcmac1>.
    		<input type=text name=cmac2 size=2 maxlength=2 value=vcmac2>.
    		<input type=text name=cmac3 size=2 maxlength=2 value=vcmac3>.
    		<input type=text name=cmac4 size=2 maxlength=2 value=vcmac4>.
    		<input type=text name=cmac5 size=2 maxlength=2 value=vcmac5>.
    		<input type=text name=cmac6 size=2 maxlength=2 value=vcmac6>
    	</td>
    </tr>
	<tr><td bgColor=#aaddff>IP Address</td><td bgColor=#ddeeff>IpAddr              </td></tr>
	<tr><td bgColor=#aaddff>Subnet Mask</td><td bgColor=#ddeeff>MaskAddr            </td></tr>
	<tr><td bgColor=#aaddff>Default Gateway</td><td bgColor=#ddeeff>GwAddr              </td></tr>
	<tr><td bgColor=#aaddff>DNS</td><td bgColor=#ddeeff>DnsAddr             </td></tr>
	<tr><td colspan=2 align=center><input type=submit name="submitBtn" value="Save">&nbsp;&nbsp;<!--<span id="spanMenu" style="visibility:hidden"><input type=submit value="Renew" name="submitBtn" renew_off>-->&nbsp;&nbsp;</span><input type=reset value="Reset"></td></tr>
</table>
</form>
<!--
<script language="JavaScript">
Renewtoggle();
</script>
-->
</body>
</html>
