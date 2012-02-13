<html>
<head>
<SCRIPT LANGUAGE="JavaScript">

function Disab() {

	if(natform.natToggle.checked==false)
	{
		natform.hwaccel.visabled=true;
		natform.ipsecPassthru.disabled=true;
		natform.pptpPassthru.disabled=true;
		natform.l2tpPassthru.disabled=true;
	}
	else
	{
		natform.hwaccel.visabled=false;
		natform.ipsecPassthru.disabled=true;
		natform.pptpPassthru.disabled=true;
		natform.l2tpPassthru.disabled=true;
	}

}

</SCRIPT>

<meta http-equiv="Content-Type" content="text/html; charset=big5">
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Cache-Control" CONTENT="no-cache">
<title>NAT Configuration</title>

</head>
<body bgcolor="#ffffff" text="#000000">

<b>NAT Options</b>
<table cellSpacing=1 cellPadding=2 border=0>
	<form method="post" action="/goform/asp_setNat" name=natform>
	<input type=hidden name="dummy" value=maskAddr4>
	<tr><td bgColor=#aaddff><input type=checkbox name=natToggle checked disabled onClick="Disab();">Enable</td></tr>
    	<tr><td bgColor=#ddeeff><input type=checkbox name=hwaccel <% asp_flashGetString("hwacc_check"); %>>L4 Switching</td></tr>
	<tr><td bgColor=#ddeeff><input type=checkbox name=ipsecPassthru disabled>IPSec Pass-Through</td></tr>
    	<tr><td bgColor=#ddeeff><input type=checkbox name=pptpPassthru disabled>PPTP Pass-Through</td></tr>
    	<tr><td bgColor=#ddeeff><input type=checkbox name=l2tpPassthru disabled >L2TP Pass-Through</td></tr>
    	<tr><td align=center><input type=submit value="Save">&nbsp;&nbsp;<input type=reset value=Reset></td></tr>
    </form>
</table>
<SCRIPT language="JavaScript">
   Disab();
</SCRIPT> 
</body>
</html>
