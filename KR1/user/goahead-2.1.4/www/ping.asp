<html>
<head>
<meta http-equiv="Content-Type" content="text/html;">
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Cache-Control" CONTENT="no-cache">
<% asp_ping("refresh"); %>
</head>
<body bgcolor="#ffffff" text="#000000">
<form method="post" action="/goform/asp_setPing" name=lanform>
<b>Ping Toolkit</b><BR>
<BR>
<table cellSpacing=1 cellPadding=2 border=0>
    <tr><td  bgColor=#aaddff>IP Address / Host Name</td>
    <td bgColor=#ddeeff>
	<input type=text name=pingstr size=20 maxlength=32 value="<% asp_ping("pingstr"); %>">
	<input type=submit value=Ping <% asp_ping("enable"); %>>
	</td></tr>
	<tr><td bgColor=#aaddff valign=top>Response</td>
    <td bgColor=#ddeeff>
    <% asp_ping("response"); %>
    </td>
    </tr>
</body>
</html>