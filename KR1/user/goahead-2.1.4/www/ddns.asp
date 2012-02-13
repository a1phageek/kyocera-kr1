<html>
<head>
<meta http-equiv="Content-Type" content="text/html">
<title>Dynamic DNS</title>
</head>

<body bgcolor="#ffffff" text="#000000">
<form method="post" action="/goform/asp_setDdns" name=wanform>
<b>Dynamic DNS</b>
<BR>
<BR>
Register a new account in <a href="http://www.noip.com" target="_blank">http://www.noip.com</a>.<BR>
<table cellSpacing=1 cellPadding=2 border=0>

<tr><td bgColor=#aaddff>Enable</td>
    <td bgColor=#ddeeff>
	<input type=checkbox name=enable value="1" <% asp_ddns("enable"); %>>
	</td>
</tr>

<tr><td bgColor=#aaddff>Username</td>
    <td bgColor=#ddeeff>
	<input type=text name=username size=16 maxlength=31 value="<% asp_ddns("username"); %>">
	</td>
</tr>
<tr><td bgColor=#aaddff>Password</td>
    <td bgColor=#ddeeff>
	<input type=password name=password size=16 maxlength=31 value="<% asp_ddns("password"); %>">
	</td>
</tr>
<tr><td bgColor=#aaddff>Hosts</td>
    <td bgColor=#ddeeff>
	<% asp_ddns("hosts"); %>
	</td>
</tr>

<tr><td colspan=2 align=center>
	<input type="submit" value="Apply"> 
	</td>
</tr>	

</table>
</body>
</html>