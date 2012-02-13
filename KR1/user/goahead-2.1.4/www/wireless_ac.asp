<html>
<head>
<meta http-equiv="Content-Type" content="text/html">
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Cache-Control" CONTENT="no-cache">
<title>Wireless Access Control</title>
</head>
<body bgcolor=#ffffffff>
<form method="get" action="/goform/asp_setWlanAccessCtrl">
<B>Wireless Access Control (under construction)</b><BR>
<p>

<% asp_wlanAccessCtrl("alias0"); %>: <select name="enable0">
<option value=0 <% asp_wlanAccessCtrl("enable0_0"); %>>Accept any client</option>
<option value=1 <% asp_wlanAccessCtrl("enable0_1"); %>>Accept clients in the list and reject all others</option>
<option value=2 <% asp_wlanAccessCtrl("enable0_2"); %>>Reject clients in the list and accept all others</option>
<BR>
<table cellSpacing=1 cellPadding=2 border=0  bgColor=white>
	<tr><td bgColor=#aaddff>No.</td><td bgColor=#aaddff>MAC Address</td><td bgColor=#aaddff>Comment</td><td bgColor=#aaddff>Select</td></tr>
	<% asp_wlanAccessCtrl("list0"); %>
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
<% asp_wlanAccessCtrl("alias1"); %>: <select name="enable1">
<option value=0 <% asp_wlanAccessCtrl("enable1_0"); %>>Accept any client</option>
<option value=1 <% asp_wlanAccessCtrl("enable1_1"); %>>Accept clients in the list and reject all others</option>
<option value=2 <% asp_wlanAccessCtrl("enable1_2"); %>>Reject clients in the list and accept all others</option>
<BR>
<table cellSpacing=1 cellPadding=2 border=0  bgColor=white>
	<tr><td bgColor=#aaddff>No.</td><td bgColor=#aaddff>MAC Address</td><td bgColor=#aaddff>Comment</td><td bgColor=#aaddff>Select</td></tr>
	<% asp_wlanAccessCtrl("list1"); %>
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
