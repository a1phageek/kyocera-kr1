<html>
<head>
<meta http-equiv="Content-Type" content="text/html">
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Cache-Control" CONTENT="no-cache">
<title>WLAN Advance Settings</title>
</head>
<SCRIPT LANGUAGE="JavaScript">
function wlan_adv_switch()
{
  var exist0 = <% asp_wlanBasic("exist0"); %>;
  var exist1  = <% asp_wlanBasic("exist1"); %>;
  if(exist0==0){
	userform.frag0.disabled=true;
	userform.rts0.disabled=true;	
	userform.beacon0.disabled=true;
	userform.dataRateMode0.disabled=true;
	userform.dataRate0.disabled=true;
	userform.preamble0.disabled=true;
	userform.broadcastSSID0.disabled=true;	
	userform.txPower0.disabled=true;		
	userform.iapp0.disabled=true;			
  }else{
	userform.frag0.disabled=false;
	userform.rts0.disabled=false;	
	userform.beacon0.disabled=false;
	userform.dataRateMode0.disabled=false;
	userform.dataRate0.disabled=false;
	userform.preamble0.disabled=false;
	userform.broadcastSSID0.disabled=false;	
	userform.txPower0.disabled=false;		
	userform.iapp0.disabled=false;
  }
  if(exist1==0){
	userform.frag1.disabled=true;
	userform.rts1.disabled=true;	
	userform.beacon1.disabled=true;
	userform.dataRateMode1.disabled=true;
	userform.dataRate1.disabled=true;
	userform.preamble1.disabled=true;
	userform.broadcastSSID1.disabled=true;	
	userform.txPower1.disabled=true;		
	userform.iapp1.disabled=true;			
  }else{
	userform.frag1.disabled=false;
	userform.rts1.disabled=false;	
	userform.beacon1.disabled=false;
	userform.dataRateMode1.disabled=false;
	userform.dataRate1.disabled=false;
	userform.preamble1.disabled=false;
	userform.broadcastSSID1.disabled=false;	
	userform.txPower1.disabled=false;		
	userform.iapp1.disabled=false;
  }
}
</script>
<body bgcolor="#ffffff" onload="wlan_adv_switch()">
<form method="get" action="/goform/asp_setWlanAdvance" name="userform">
<B>Wireless Advance Settings</b><BR>
<BR>
<table cellSpacing=1 cellPadding=2 border=0>

  <tr><td bgColor=#ddffaa>WLAN Card 1 - <% asp_wlanAdvance("alias0"); %></td></tr>
  <tr><td bgColor=#aaddff>Fragment Threshold</td>
      <td bgColor=#ddeeff><input type="text" name="frag0" size=4 maxlength=4 value="<% asp_wlanAdvance("frag0"); %>">(256-2346)</td>
  </tr>

  <tr><td bgColor=#aaddff>RTS Threshold</td>
      <td bgColor=#ddeeff><input type="text" name="rts0" size=4 maxlength=4 value="<% asp_wlanAdvance("rts0"); %>">(0-2347)</td>
  </tr>
  
   <tr><td bgColor=#aaddff>Beacon Interval</td>
      <td bgColor=#ddeeff><input type="text" name="beacon0" size=4 maxlength=4 value="<% asp_wlanAdvance("beacon0"); %>">(20-1024 ms)</td>
  </tr> 

  <tr><td bgColor=#aaddff>Data Rate Mode</td>
      <td bgColor=#ddeeff><select name="dataRateMode0">
		<option value=0 <% asp_wlanAdvance("dataRateMode0_0"); %>>802.11b+g</option>
		<option value=1 <% asp_wlanAdvance("dataRateMode0_1"); %>>802.11g</option>
		<option value=2 <% asp_wlanAdvance("dataRateMode0_2"); %>>802.11a only</option>
		<option value=3 <% asp_wlanAdvance("dataRateMode0_3"); %>>802.11b only</option>
      </td>
  </tr> 
  
  <tr><td bgColor=#aaddff>Max Data Rate</td>
      <td bgColor=#ddeeff><select name="dataRate0">
		<option value=0 <% asp_wlanAdvance("dataRate0_0"); %>>Auto</option>
		<option value=1 <% asp_wlanAdvance("dataRate0_1"); %>>54M</option>
		<option value=2 <% asp_wlanAdvance("dataRate0_2"); %>>48M</option>		
		<option value=3 <% asp_wlanAdvance("dataRate0_3"); %>>36M</option>		
		<option value=4 <% asp_wlanAdvance("dataRate0_4"); %>>24M</option>		
		<option value=5 <% asp_wlanAdvance("dataRate0_5"); %>>18M</option>		
		<option value=6 <% asp_wlanAdvance("dataRate0_6"); %>>12M</option>
		<option value=7 <% asp_wlanAdvance("dataRate0_7"); %>>9M</option>		
		<option value=8 <% asp_wlanAdvance("dataRate0_8"); %>>6M</option>		
		<option value=9 <% asp_wlanAdvance("dataRate0_9"); %>>802.11b-11M</option>		
		<option value=10 <% asp_wlanAdvance("dataRate0_10"); %>>802.11b-5.5M</option>
		<option value=11 <% asp_wlanAdvance("dataRate0_11"); %>>802.11b-2M</option>
		<option value=12 <% asp_wlanAdvance("dataRate0_12"); %>>802.11b-1M</option>
      </td>
  </tr> 

  <tr><td bgColor=#aaddff>Preamble Type</td>
      <td bgColor=#ddeeff>
      <input type="radio" name="preamble0" value=0  <% asp_wlanAdvance("preamble0_0"); %>>Long Preamble &nbsp;&nbsp;
      <input type="radio" name="preamble0" value=1  <% asp_wlanAdvance("preamble0_1"); %>>Short Preamble &nbsp;&nbsp;      
      </td>
  </tr>
  
  <tr><td bgColor=#aaddff>Broadcast SSID</td>
      <td bgColor=#ddeeff><input type="checkbox" name="broadcastSSID0" value=1  <% asp_wlanAdvance("broadcastSSID0"); %>>Enable</td>      
  </tr>    

  <tr><td bgColor=#aaddff>Transmit Power</td>
      <td bgColor=#ddeeff><select name="txPower0">
		<option value=0 <% asp_wlanAdvance("txPower0_0"); %>>Disabled</option>
		<option value=1 <% asp_wlanAdvance("txPower0_1"); %>>Weak</option>
		<option value=2 <% asp_wlanAdvance("txPower0_2"); %>>Average</option>		
		<option value=3 <% asp_wlanAdvance("txPower0_3"); %>>Strong</option>
		<option value=4 <% asp_wlanAdvance("txPower0_4"); %>>Maximum</option>

      </td>
  </tr> 

  <tr><td bgColor=#aaddff>IAPP:</td>
      <td bgColor=#ddeeff><input type="checkbox" name="iapp0" value=1  <% asp_wlanAdvance("iapp0"); %>>Enable</td>      
  </tr>  
</table>

<p>

<table cellSpacing=1 cellPadding=2 border=0>

  <tr><td bgColor=#ddffaa>WLAN Card 2 - <% asp_wlanAdvance("alias1"); %></td></tr>
  <tr><td bgColor=#aaddff>Fragment Threshold</td>
      <td bgColor=#ddeeff><input type="text" name="frag1" size=4 maxlength=4 value="<% asp_wlanAdvance("frag1"); %>">(256-2346)</td>
  </tr>

  <tr><td bgColor=#aaddff>RTS Threshold</td>
      <td bgColor=#ddeeff><input type="text" name="rts1" size=4 maxlength=4 value="<% asp_wlanAdvance("rts1"); %>">(0-2347)</td>
  </tr>
  
   <tr><td bgColor=#aaddff>Beacon Interval</td>
      <td bgColor=#ddeeff><input type="text" name="beacon1" size=4 maxlength=4 value="<% asp_wlanAdvance("beacon1"); %>">(20-1024 ms)</td>
  </tr> 

  <tr><td bgColor=#aaddff>Data Rate Mode</td>
      <td bgColor=#ddeeff><select name="dataRateMode1">
		<option value=0 <% asp_wlanAdvance("dataRateMode1_0"); %>>802.11b+g</option>
		<option value=1 <% asp_wlanAdvance("dataRateMode1_1"); %>>802.11g</option>
		<option value=2 <% asp_wlanAdvance("dataRateMode1_2"); %>>802.11a only</option>
		<option value=3 <% asp_wlanAdvance("dataRateMode1_3"); %>>802.11b only</option>
      </td>
  </tr> 
  
  <tr><td bgColor=#aaddff>Max Data Rate</td>
      <td bgColor=#ddeeff><select name="dataRate1">
		<option value=0 <% asp_wlanAdvance("dataRate1_0"); %>>Auto</option>
		<option value=1 <% asp_wlanAdvance("dataRate1_1"); %>>54M</option>
		<option value=2 <% asp_wlanAdvance("dataRate1_2"); %>>48M</option>		
		<option value=3 <% asp_wlanAdvance("dataRate1_3"); %>>36M</option>		
		<option value=4 <% asp_wlanAdvance("dataRate1_4"); %>>24M</option>		
		<option value=5 <% asp_wlanAdvance("dataRate1_5"); %>>18M</option>		
		<option value=6 <% asp_wlanAdvance("dataRate1_6"); %>>12M</option>
		<option value=7 <% asp_wlanAdvance("dataRate1_7"); %>>9M</option>		
		<option value=8 <% asp_wlanAdvance("dataRate1_8"); %>>6M</option>		
		<option value=9 <% asp_wlanAdvance("dataRate1_9"); %>>802.11b-11M</option>		
		<option value=10 <% asp_wlanAdvance("dataRate1_10"); %>>802.11b-5.5M</option>
		<option value=11 <% asp_wlanAdvance("dataRate1_11"); %>>802.11b-2M</option>
		<option value=12 <% asp_wlanAdvance("dataRate1_12"); %>>802.11b-1M</option>
      </td>
  </tr> 

  <tr><td bgColor=#aaddff>Preamble Type</td>
      <td bgColor=#ddeeff>
      <input type="radio" name="preamble1" value=0  <% asp_wlanAdvance("preamble1_0"); %>>Long Preamble &nbsp;&nbsp;
      <input type="radio" name="preamble1" value=1  <% asp_wlanAdvance("preamble1_1"); %>>Short Preamble &nbsp;&nbsp;      
      </td>
  </tr>
  
  <tr><td bgColor=#aaddff>Broadcast SSID</td>
      <td bgColor=#ddeeff><input type="checkbox" name="broadcastSSID1" value=1  <% asp_wlanAdvance("broadcastSSID1"); %>>Enable</td>      
  </tr>    

  <tr><td bgColor=#aaddff>Transmit Power</td>
      <td bgColor=#ddeeff><select name="txPower1">
		<option value=0 <% asp_wlanAdvance("txPower1_0"); %>>Disabled</option>
		<option value=1 <% asp_wlanAdvance("txPower1_1"); %>>Weak</option>
		<option value=2 <% asp_wlanAdvance("txPower1_2"); %>>Average</option>		
		<option value=3 <% asp_wlanAdvance("txPower1_3"); %>>Strong</option>
		<option value=4 <% asp_wlanAdvance("txPower1_4"); %>>Maximum</option>

      </td>
  </tr> 



  <tr><td bgColor=#aaddff>IAPP:</td>
      <td bgColor=#ddeeff><input type="checkbox" name="iapp1" value=1  <% asp_wlanAdvance("iapp1"); %>>Enable</td>      
  </tr>  

 <tr><Td colspan=2 align=center>
     <input type="submit" name="save" value="Apply">&nbsp;&nbsp;&nbsp;&nbsp;
     <input type="reset" value="Reset" name="reset">
  </tr>
</table>



</form>
</body>
</html>
