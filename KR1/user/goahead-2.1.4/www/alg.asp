<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=big5">

<script language=javascript>
function changeMs(a)
{	
	fn.ms.value=a;
	fn.submit();
}
</script>
<title>ALG</title>
</head>
<body bgcolor="#ffffff" text="#000000">
<B>ALG Configuration</B><BR><BR>
<% asp_alg("ms"); %>
<form action="alg.asp" name="fn"><input type=hidden name=ms value=0></form>
<form action="/goform/asp_setAlg" name="fm">
<table cellSpacing=1 cellPadding=2 border=0>
<tr>
<td bgColor=#aaddff>ALG</td><td bgColor=#aaddff>Enable</td>
</tr>

<% asp_alg("web"); %>

<tr>
<td colspan=2 align=center>
	<input type=submit name=smt value="Apply">
</td>
</tr>

</table>
</form>
</body>
</html>
