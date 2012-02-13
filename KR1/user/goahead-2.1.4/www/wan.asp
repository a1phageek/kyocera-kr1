<html>
<head>
<meta http-equiv="Content-Type" content="text/html">
<title>WAN Type Selection</title>
</head>

<body bgcolor="#ffffff" text="#000000">

<b>WAN Type Selection</b>
<table cellSpacing=1 cellPadding=2 border=0>
	<form method="post" action="/goform/asp_wanIp" name=wanform>
    <tr>
    	<td bgColor=#aaddff>Type</td>    
    	<td bgColor=#ddeeff>Selection</td>
	<td bgColor=#eeddff>Hardware Feature</td>
	</tr>


    <tr>
    	<td bgColor=#aaddff>Static IP</td>    
    	<td bgColor=#ddeeff><input type=radio name="wantype" value="0" <% asp_flashGetString("wanstatic");%> ></td>
	<td bgColor=#eeddff>Wirespeed</td>
	</tr>
    <tr>
    	<td bgColor=#aaddff>DHCP</td>    	
    	<td bgColor=#ddeeff><input type=radio name="wantype" value="2" <%asp_flashGetString("wandhcp");%>></td>
	<td bgColor=#eeddff>Wirespeed</td>
	</tr>	
    <tr>
    	<td bgColor=#aaddff>PPPoE</td>    	
    	<td bgColor=#ddeeff><input type=radio name="wantype" value="1" <%asp_flashGetString("wanpppoe");%>></td>
	<td bgColor=#eeddff>1st session wirespeed</td>
	</tr>	
   
	<%asp_flashGetString("wanunnumbered");%>

	<%asp_flashGetString("pptpl2tp");%>

	<%asp_flashGetString("dhcpl2tp");%>

	<tr>
    	<td colspan=2 align=center>
    		
    		<input type="button" value="< Back" onclick="javascript:history.back();">
    		&nbsp;&nbsp;&nbsp;&nbsp;
    		<input type="submit" value="Next >">
    		&nbsp;&nbsp;&nbsp;&nbsp;
    		<input type="reset" value="Reset">
    		&nbsp;&nbsp;&nbsp;&nbsp;
    		<input type="button" value="Cancel" onclick="javascript:location.replace('status.asp');">
    	</td>
	</tr>
	
	</form>
	
</table>

</body>
</html>
