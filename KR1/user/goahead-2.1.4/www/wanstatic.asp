<html>
<head>
<meta http-equiv="Content-Type" content="text/html;">
<title>WAN Interface Configuration</title>
</head>
<body bgcolor="#ffffff" text="#000000">

<form method="get" action="/goform/asp_setStaticWizard">
<b>WAN Interface Configuration</b>
<table cellSpacing=1 cellPadding=2 border=0>

	
	
    <tr><td bgColor=#aaddff>IP Address</td>
    <td bgColor=#ddeeff>
	<input type=text name=wanip_1 size=3 maxlength=3 value=<% asp_flashGetIpElement("wan111");%> >.
	<input type=text name=wanip_2 size=3 maxlength=3 value=<% asp_flashGetIpElement("wan112");%> >.
	<input type=text name=wanip_3 size=3 maxlength=3 value=<% asp_flashGetIpElement("wan113");%> >.
	<input type=text name=wanip_4 size=3 maxlength=3 value=<% asp_flashGetIpElement("wan114");%> >
	</td>
	</tr>
    <tr><td bgColor=#aaddff>Subnet Mask</td>
    <td bgColor=#ddeeff>
	<input type=text name=wansm_1 size=3 maxlength=3 value=<% asp_flashGetIpElement("mask211");%> >.
	<input type=text name=wansm_2 size=3 maxlength=3 value=<% asp_flashGetIpElement("mask212");%> >.
	<input type=text name=wansm_3 size=3 maxlength=3 value=<% asp_flashGetIpElement("mask213");%> >.
	<input type=text name=wansm_4 size=3 maxlength=3 value=<% asp_flashGetIpElement("mask214");%> >
	</td>
	</tr>
    <tr><td bgColor=#aaddff>Default Gateway</td>
    <td bgColor=#ddeeff>
	<input type=text name=wangw_1 size=3 maxlength=3 value=<% asp_flashGetIpElement("g1");%>>.
	<input type=text name=wangw_2 size=3 maxlength=3 value=<% asp_flashGetIpElement("g2");%>>.
	<input type=text name=wangw_3 size=3 maxlength=3 value=<% asp_flashGetIpElement("g3");%>>.
	<input type=text name=wangw_4 size=3 maxlength=3 value=<% asp_flashGetIpElement("g4");%>>
	</td>
	</tr>
    <tr><td bgColor=#aaddff>DNS</td>
    <td bgColor=#ddeeff>
	<input type=text name=wandns_1 size=3 maxlength=3 value=<% asp_flashGetIpElement("dn1"); %> >.
	<input type=text name=wandns_2 size=3 maxlength=3 value=<% asp_flashGetIpElement("dn2"); %> >.
	<input type=text name=wandns_3 size=3 maxlength=3 value=<% asp_flashGetIpElement("dn3");%> >.
	<input type=text name=wandns_4 size=3 maxlength=3 value=<% asp_flashGetIpElement("dn4");%> >
	</td>
	
    <tr>
    	<td colspan=2 align=center>
    		<input type="button" value="< Back" onclick="javascript:history.back();">
    		&nbsp;&nbsp;&nbsp;&nbsp;
    		<input type="submit" value="Save and Restart">    		
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
