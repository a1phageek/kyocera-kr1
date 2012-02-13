<html>
<head>
<title>Flash</title>
</head>
<body>
<form action="/goform/asp_setFlash" name="fm" method=POST>
<% asp_flash("web"); %>
<TABLE BORDER=0 CELLPADDING=2 CELLSPACING=2>
	<TR BGCOLOR=LIGHTBLUE>
		<TD> Start Offset: </TD>
		<TD> <INPUT TYPE=TEXT NAME=so VALUE=""> </TD>
		<TD> Offset to flashbase, example: 0x4000 </TD>
	</TR>
	<TR BGCOLOR=LIGHTBLUE>
		<TD> Content to update: </TD>
		<TD> <TEXTAREA NAME=co COLS=48 ROWS=8></TEXTAREA> </TD>
		<TD> Used to update flash, example: %00%00%10%11%12%00) </TD>
	</TR>
	<TR BGCOLOR=LIGHTBLUE>
		<TD> &nbsp; </TD>
		<TD> <INPUT TYPE=SUBMIT VALUE="UPDATE"> </TD>
		<TD> &nbsp; </TD>
	</TR>
</TABLE>
</form>
</body>
</html>

