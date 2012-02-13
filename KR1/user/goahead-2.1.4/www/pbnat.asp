<html>
<head>
<title>Protocol-Based NAT</title>
<script language=javascript>
function changeMs(a)
{	
	fn.ms.value=a; 	fn.submit();
}
</script>
<style>
TABLE {}
TH { background-color:#aaddff; }
TD { background-color:#ddeeff; }
</style>
</head>
<body bgcolor=#ffffff>
	<B>IP Protocol based NAT</B>
	<form action="pbnat.asp" name="fn"><input type=hidden name=ms value=0></form>
	
	<% asp_pbnat("web"); %>

</body>
</html>
