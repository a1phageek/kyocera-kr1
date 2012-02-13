<html>
<head>
<meta http-equiv="Content-Type" content="text/html;">
<title>WAN Interface Configuration</title>
<% asp_l2tpWizard("autoRefresh"); %>
<script language="javascript">
function clickAutoReconnect()
{
  if(fn.autoReconnect.checked==1)
  {
	if((fn.silentTimeout.value!=0)&&(fn.dialOnDemand.checked==0))
	{
		alert('If you want to turn auto reconnect on, you have to set silent timeout to zero second.');
		fn.autoReconnect.checked=0;		
	}
  }
  else
  {
	fn.dialOnDemand.checked=0;
  }
}

function clickDialOnDemand()
{
	if(fn.dialOnDemand.checked==1) fn.autoReconnect.checked=1;
	else clickAutoReconnect();	
}

function changeSilentTimeout()
{
	if((fn.silentTimeout.value!=0)&&(fn.autoReconnect.checked==1)&&(fn.dialOnDemand.checked==0))
	{
		alert('Silent timeout can\'t run with auto reconnect on.');
		fn.autoReconnect.checked=0;
	}
}
</script>
</head>
<body bgcolor="#ffffff" text="#000000">

<form method="get" action="/goform/asp_setL2tpWizard" name="fn">
<b>L2TP WAN Interface Configuration</b>
<table cellSpacing=1 cellPadding=2 border=0>
	
    <tr><td bgColor=#aaddff>IP Address</td>
    <td bgColor=#ddeeff colspan=3>
	<input type=text name=wanip_1 size=3 maxlength=3 value=<% asp_l2tpWizard("ip1"); %>>.
	<input type=text name=wanip_2 size=3 maxlength=3 value=<% asp_l2tpWizard("ip2"); %>>.
	<input type=text name=wanip_3 size=3 maxlength=3 value=<% asp_l2tpWizard("ip3"); %>>.
	<input type=text name=wanip_4 size=3 maxlength=3 value=<% asp_l2tpWizard("ip4"); %>>
	</td>
	</tr>
    <tr><td bgColor=#aaddff>Subnet Mask</td>
    <td bgColor=#ddeeff colspan=3>
	<input type=text name=wansm_1 size=3 maxlength=3 value=<% asp_l2tpWizard("sm1"); %>>.
	<input type=text name=wansm_2 size=3 maxlength=3 value=<% asp_l2tpWizard("sm2"); %>>.
	<input type=text name=wansm_3 size=3 maxlength=3 value=<% asp_l2tpWizard("sm3"); %>>.
	<input type=text name=wansm_4 size=3 maxlength=3 value=<% asp_l2tpWizard("sm4"); %>>
	</td>
	</tr>
    <tr><td bgColor=#aaddff>L2TP Server IP Address</td>
    <td bgColor=#ddeeff  colspan=3>
	<input type=text name=wansv_1 size=3 maxlength=3 value=<% asp_l2tpWizard("sv1"); %>>.
	<input type=text name=wansv_2 size=3 maxlength=3 value=<% asp_l2tpWizard("sv2"); %>>.
	<input type=text name=wansv_3 size=3 maxlength=3 value=<% asp_l2tpWizard("sv3"); %>>.
	<input type=text name=wansv_4 size=3 maxlength=3 value=<% asp_l2tpWizard("sv4"); %>>
	</td>
	</tr>
	<tr>
	<td bgColor=#aaddff>Login ID</td>
	<td bgColor=#ddeeff  colspan=3><input type=text name=username size=32 maxlength=32 value="<% asp_l2tpWizard("username"); %>"></td>
	</tr>
	<tr>
	<td bgColor=#aaddff>Password</td>
	<td bgColor=#ddeeff  colspan=3><input type=password name=password size=32 maxlength=32 value="<% asp_l2tpWizard("password"); %>"></td>
	</tr>	


    <tr>
    <td bgColor=#aaddff>MTU</td>
    <td bgColor=#ddeeff>
	<input type=text name=wanmtu size=4 maxlength=4 value=<% asp_l2tpWizard("mtu"); %>>
	</td>
    <td bgColor=#aaddff>Silent Timeout</td>
    <td bgColor=#ddeeff>
	<input type=text name=silentTimeout size=8 maxlength=8 value=<% asp_l2tpWizard("silentTimeout"); %> onblur="javascript:changeSilentTimeout();">
	</td>
	</tr>

	<tr>
    <td bgColor=#aaddff>Dial-On-Demand</td>
    <td bgColor=#ddeeff>
	<input type=checkbox name=dialOnDemand value=1 <% asp_l2tpWizard("dialOnDemand"); %> onclick="javascript:clickDialOnDemand();">
	</td>
    <td bgColor=#aaddff>Auto Reconnect</td>
    <td bgColor=#ddeeff>
	<input type=checkbox name=autoReconnect value=1 <% asp_l2tpWizard("autoReconnect"); %> onclick="javascript:clickAutoReconnect();">
	</td>
	</tr>	
	<tr>
	<td bgColor=#aaddff>Dial Status</td>
	<td bgColor=#ddeeff colspan=3><% asp_l2tpWizard("dialStatus"); %></td>
	</tR>
	
    <tr>
    	<td colspan=4 align=center>
			<% asp_l2tpWizard("submit"); %>
    	</td>
    </tr>

	</form>
	
</table>
<% asp_configChanged("CONFIG_CHANGE_MESSAGE"); %>
</body>
</html>
