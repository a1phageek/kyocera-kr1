<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=big5">
<script language=javascript>
function changeMs(a)
{	
	fn.ms.value=a;
	fn.submit();
}

function onLoad_func() {
	dos_enab();
	dosProc_checkEnab();
	dosProc_enab();
}

function dos_check() {
	<% asp_dos("ThresHoldCheck"); %>
	dos.ignore_lan.disabled=false;
	<% asp_dos("dosItemAvailable"); %>
}

function dos_SelectAll() {
	<% asp_dos("SelectAllItem"); %>
}

function dos_ClearAll() {
	<% asp_dos("ClearAllItem"); %>
}

function dosProc_checkEnab() {
	if (dos.enable.checked==true)
	{
		dosProc.sipblock_enable.disabled=false;
		dosProc.sipblock_prisontime.disabled=false;
		dosProc.smt.disabled=false;
		if (dosProc.sipblock_enable.checked==true)
		{
			dosProc.blockAllBlockedIp.disabled=false;
		}else
		{
			dosProc.blockAllBlockedIp.disabled=true;
		}
	}else
	{
		dosProc.sipblock_enable.disabled=true;
		dosProc.sipblock_prisontime.disabled=true;
		dosProc.smt.disabled=true;
		dosProc.blockAllBlockedIp.disabled=true;

		if (dosProc.sipblock_enable.checked==true)
		{
			dosProc.sipblock_enable.checked=false;
			dosProc.submit();
		}
	}
}

function dosProc_enab() {
	if(dosProc.sipblock_enable.checked==true)
	{
		dosProc.sipblock_prisontime.disabled=false;
	}else
	{
		dosProc.sipblock_prisontime.disabled=true;
	}
}

function dos_enab() {
	if (dos.enable.checked==true)
	{
		dos.ignore_lan.disabled=false;
		<% asp_dos("dosItemAvailable"); %>
	}else
	{
		dos.ignore_lan.disabled=true;
		<% asp_dos("dosItemUnavailable"); %>
	}
}

</script>
<title>DoS</title>
</head>
<body bgcolor="#ffffff" text="#000000" onload="onLoad_func()">

<B>DoS Configuration</B><BR><BR>
<% asp_dos("ms"); %>
<form action="dos.asp" name="fn"><input type=hidden name=ms value=0></form>
<form action="/goform/asp_setDos" name="dos" onsubmit="return dos_check()">
<table cellSpacing=1 cellPadding=2 border=0>
<tr>
<td bgColor=#aaddff>DoS Prevention Enable</td>
<td bgColor=#ddeeff><input type=checkbox name=enable onClick="dos_enab();" value=1 <% asp_dos("ENABLE"); %> ></td>
</tr>
<tr>
<td bgColor=#aaddff>Ignore LAN-Side Check</td>
<td bgColor=#ddeeff><input type=checkbox name=ignore_lan value=1 <% asp_dos("Ignore_Lan"); %> ></td>
</tr>
<% asp_dos("dosList"); %>

<tr>
<td colspan=2 align=center>
	<input type=hidden name=ms value=<% asp_dos("MS"); %>>
	<input type=submit name=smt value="Apply">
</td>
</tr>
</table>
</form>

<form action="/goform/asp_setDosProc" name=dosProc>
<BR><B>Source Blocking</B><BR>
<table cellSpacing=1 cellPadding=2 border=0>
	<table>
		<tr>
			<td bgColor=#aaddff>Source IP Blocking Enable</td>
			<td bgColor=#ddeeff><input type=checkbox name=sipblock_enable onClick="dosProc_enab();" value=1 <% asp_dosProc("SIPBLOCK_ENABLE"); %> ></td>
		</tr>
		<tr>
			<td bgColor=#aaddff>Block Time</td>
			<td bgColor=#ddeeff><input type=text name=sipblock_prisontime value=<% asp_dosProc("SIPBLOCK_PRISONTIME"); %> size=3 maxlength=3 >Second</td>
		</tr>
	</table>
	<tr>
		<td colspan=2 align=center>
			<input type=hidden name=ms value=<% asp_dosProc("MS"); %>>
			<input type=submit name=smt value="Apply">
		</td>
	</tr>
	<tr>
		<td colspan=2 align=center>
			<input type=hidden name=ms value=<% asp_dosProc("MS"); %>>
			<input type=submit name=blockAllBlockedIp value="FreeALL">
		</td>
	</tr>
</table>
</form>

</body>
</html>
