<html><head><meta http-equiv="Content-Type" content="text/html; charset=big5">
<title>Special Application Configuration</title></head>
<script language=javascript>
	function changeMs(a)
	{
		fn.ms.value=a;
		fn.submit();
	}
<% asp_specialApplication("JS"); %>	
<body bgcolor="#ffffff" text="#000000"><b>Special Application</b>
<% asp_specialApplication("ms"); %>
<form action="specialap.asp" name="fn"><input type=hidden name=ms value=0></form>
<form method=get action="/goform/asp_setSpecialApplication" name="formname">
<% asp_specialApplication("content"); %>
<tr><td colspan=8 align=center>
<input type=submit value=Apply>&nbsp;&nbsp;&nbsp;&nbsp;
<input type=reset value=Reset></td></tr>
</table></form></body></html>


