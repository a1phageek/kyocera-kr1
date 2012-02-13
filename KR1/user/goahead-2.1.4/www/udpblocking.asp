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
<title>UDP Blocking</title>
</head>
<body bgcolor="#ffffff" text="#000000">
<B>UDP Blocking Configuration</B><BR><BR>
<% asp_udpBlocking("ms"); %>
<form action="udpblocking.asp" name="fn"><input type=hidden name=ms value=0></form>
<form method="get" action="/goform/asp_setUdpBlocking">
<input type=hidden name=ms value=<% asp_udpBlocking("MS"); %>>
<table cellSpacing=1 cellPadding=2 border=0>
<tr>
    	<td bgColor=#aaddff>UDP Blocking</td> 
    	<td bgColor=#ddeeff>
		<input type=checkbox name="enable" value=1 <% asp_udpBlocking("enable"); %>>Enable
	</td>
</tr>

<tr>
    	<td bgColor=#aaddff>Maximum UDP Size</td> 
    	<td bgColor=#ddeeff>
    		<select name="size">    		
    		<% asp_udpBlocking("size"); %>">
    		</select>Kbytes
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
