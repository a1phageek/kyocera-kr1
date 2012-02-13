<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=big5">
<title>UPnP</title>
</head>
<body bgcolor="#ffffff" text="#000000">
<form action="/goform/asp_setUpnp">
<B>UPnP Configuration</B><BR><BR>
<table cellSpacing=1 cellPadding=2 border=0>
<tr>
<td bgColor=#aaddff>UPnP Service</td>
<td bgColor=#ddeeff>
<input type=checkbox name=enable value=1 <% asp_upnp(); %> >Enable
</td>
</tr>
<tr>
<td colspan=2 align=center>
	<input type=submit name=smt value="Apply">
</td>
</tr>
</table>
</form>
</body>
</html>
