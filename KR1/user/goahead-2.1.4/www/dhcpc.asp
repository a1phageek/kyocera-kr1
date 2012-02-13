<html>
<head>
<meta http-equiv="Content-Type" content="text/html">
<title>DHCP Client Configuration</title>
</head>
<body bgcolor="#ffffff" text="#000000">
<form method="get" action="/goform/asp_setDhcpClient" name=frm>
<b>DHCP Client</b>
<table cellSpacing=1 cellPadding=2 border=0>
    <tr>
    	<td bgColor=#aaddff>Clone MAC</td> 
    	<td bgColor=#ddeeff>
    		<input type=checkbox name=cmacToggle <%asp_flashGetCloneMac("cmacValid");%> >Use&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    		<input type=text name=cmac1 size=2 maxlength=2 value=<%asp_flashGetCloneMac("cmac1");%> >.
    		<input type=text name=cmac2 size=2 maxlength=2 value=<%asp_flashGetCloneMac("cmac2");%> >.
    		<input type=text name=cmac3 size=2 maxlength=2 value=<%asp_flashGetCloneMac("cmac3");%>>.
    		<input type=text name=cmac4 size=2 maxlength=2 value=<%asp_flashGetCloneMac("cmac4");%>>.
    		<input type=text name=cmac5 size=2 maxlength=2 value=<%asp_flashGetCloneMac("cmac5");%>>.
    		<input type=text name=cmac6 size=2 maxlength=2 value=<%asp_flashGetCloneMac("cmac6");%>>
    		<input type=hidden name=saverenew value="save">
    	</td>
    </tr>
	<tr><td colspan=2 align=center>
	<input type="button" value="< Back" onclick="javascript:history.back();">&nbsp;&nbsp;&nbsp;&nbsp;
	<input type=submit value="Save and Restart" name="submitBtn">&nbsp;&nbsp;&nbsp;&nbsp;
	<input type=reset value="Reset">&nbsp;&nbsp;&nbsp;&nbsp;
	<input type="button" value="Cancel" onclick="javascript:location.replace('status.asp');">
	</td></tr>
</table>
</form>
</body>
</html>
