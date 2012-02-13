<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=big5">
<title>Routing Configuration</title>
</head>
<body bgcolor="#ffffff" text="#000000">

<b>Hardware Static Route</b><BR>
<BR>
<table cellSpacing=1 cellPadding=2 border=0>

	<form method="post" action="/goform/asp_setRouting">
	
	<tr><td bgColor=#aaddff>Route</td><td bgColor=#aaddff>Route Mask</td><td bgColor=#aaddff>Next Hop IP</td><td bgColor=#aaddff>Interface</td></tr>
		
<%
asp_routing();
%>

	<tr>	
		<td colspan=4 align=center><input type=submit value=Save>&nbsp;&nbsp;<input type=reset value=Reset></td>
	</tr>

	</form>
	
</table>
<ul>
<li>Given "Next Hop IP" must be in the same subnet as specified "Interface" to be accepted.
</ul>
</body>
</html>