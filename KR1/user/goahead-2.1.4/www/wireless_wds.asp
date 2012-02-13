<html>
<head>
<meta http-equiv="Content-Type" content="text/html">
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Cache-Control" CONTENT="no-cache">
<title>WDS</title>
</head>
<body bgcolor=#ffffffff>
<form method="get" action="/goform/asp_setWlanWds">
<B>Wireless Distribution System Setting (under construction)</b><BR>
<BR>

<input type="checkbox" name="enable0" value=1 <% asp_wlanWds("enable0"); %>>
Enable WDS on WLAN card 1(<% asp_wlanAccessCtrl("alias0"); %>)<BR>
<table cellSpacing=1 cellPadding=2 border=0  bgColor=white>
	<tr><td bgColor=#aaddff>No.</td><td bgColor=#aaddff>AP MAC Address</td><td bgColor=#aaddff>Comment</td><td bgColor=#aaddff>Select</td></tr>
	<% asp_wlanWds("list0"); %>
	<tr bgColor=#ddeeff><td>New</td>
		<td><input type="text" name="mac0" size=17 maxlength=17></td>
		<td><input type="text" name="comment0" size=31 maxlength=31></td>
		<td></td>
	</tr>
</table>
<input type="submit" name="submit" value="Apply">
<input type="submit" name="deleteSelected0" value="Delete Selected">
<input type="submit" name="deleteAll0" value="Delete All">
<input type="reset" value="Reset" name="reset0">
<p>
<p>
<p>
<input type="checkbox" name="enable1" value=1 <% asp_wlanWds("enable1"); %>>
Enable WDS on WLAN card 2 (<% asp_wlanAccessCtrl("alias1"); %>)<BR>
<table cellSpacing=1 cellPadding=2 border=0  bgColor=white>
	<tr><td bgColor=#aaddff>No.</td><td bgColor=#aaddff>AP MAC Address</td><td bgColor=#aaddff>Comment</td><td bgColor=#aaddff>Select</td></tr>
	<% asp_wlanWds("list1"); %>
	<tr bgColor=#ddeeff><td>New</td>
		<td><input type="text" name="mac1" size=17 maxlength=17></td>
		<td><input type="text" name="comment1" size=31 maxlength=31></td>
		<td></td>
	</tr>
</table>
<input type="submit" name="submit" value="Apply">
<input type="submit" name="deleteSelected1" value="Delete Selected">
<input type="submit" name="deleteAll1" value="Delete All">
<input type="reset" value="Reset" name="reset1">

</form>
</body>
</html>
