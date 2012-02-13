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
<title>URL Filter</title>
</head>
<body bgcolor="#ffffff" text="#000000">

<B>URL Filter Configuration</B><BR><BR>
<% asp_urlFilter("ms"); %>
<form action="urlfilter.asp" name="fn"><input type=hidden name=ms value=0></form>
<form method="get" action="/goform/asp_setUrlFilter">
<table cellSpacing=1 cellPadding=2 border=0>
<tr>
    	<td bgColor=#aaddff>URL string pattern to be blocked</td><td bgColor=#aaddff>Source IP range</td><td bgColor=#aaddff>Enable</td>
</tr>
    	<% asp_urlFilter(); %>
<tr>
	<td colspan=2 align=center>
		<input type=submit value="Apply">
	</td>
</tr>
</table>
<ul><li>Any web access traffic(ie. destined to any TCP port) is inspected by hardware at wirespeed.
</ul>
</form>
</body>
</html>
