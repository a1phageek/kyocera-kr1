<html>
<head>
<meta http-equiv="Content-Type" content="text/html">
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Cache-Control" CONTENT="no-cache">
<script language=javascript>
function changeMs(a)
{	
	fn.ms.value=a;
	fn.submit();
}
</script>
<title>DMZ Host</title>
</head>
<body bgcolor="#ffffff" text="#000000">
<B>DMZ Configuration</B><BR><BR>
<% asp_dmz("ms"); %>
<form action="dmz.asp" name="fn"><input type=hidden name=ms value=0></form>
<form method="get" action="/goform/asp_setDmz">
<input type=hidden name=ms value=<% asp_dmz("MS"); %>>
<table cellSpacing=1 cellPadding=2 border=0>
<tr>
    	<td bgColor=#aaddff>DMZ Host</td> 
    	<td bgColor=#ddeeff>
    		<input type=text name=ip1 size=3 maxlength=3 value="<% asp_dmz("dmzIp","1"); %>">.
    		<input type=text name=ip2 size=3 maxlength=3 value="<% asp_dmz("dmzIp","2"); %>">.
    		<input type=text name=ip3 size=3 maxlength=3 value="<% asp_dmz("dmzIp","3"); %>">.
    		<input type=text name=ip4 size=3 maxlength=3 value="<% asp_dmz("dmzIp","4"); %>">
    		&nbsp;&nbsp;&nbsp;
    		<input type=checkbox name="enable" value=1 <% asp_dmz("enable"); %>>Enable
    	</td>
</tr>
<tr>
	<td colspan=2 align=center>
		<input type=submit value="Apply">
	</td>
</tr>
</table>

</form>
</body>
</html>
