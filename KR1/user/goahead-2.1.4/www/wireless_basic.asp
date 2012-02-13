<html>
<head>
<meta http-equiv="Content-Type" content="text/html">
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Cache-Control" CONTENT="no-cache">
<title>WLAN Basic Settings</title>
</head>
<SCRIPT LANGUAGE="JavaScript">
function onload_func()
{
  var exist0 = <% asp_wlanBasic("exist0"); %>;
  var exist1  = <% asp_wlanBasic("exist1"); %>;
	if(exist0==0)
		userform.enable0.disabled=true;
	else
		userform.enable0.disabled=false;
	if(userform.enable0.checked == false){
		userform.name0.disabled=true;
		userform.ssid0.disabled=true;
		userform.channel0.disabled=true;
	}else{
		userform.name0.disabled=false;
		userform.ssid0.disabled=false;
		userform.channel0.disabled=false;
	}

	if(exist1==0)
		userform.enable1.disabled=true;
	else
		userform.enable1.disabled=false;
	if(userform.enable1.checked == false){
		userform.name1.disabled=true;
		userform.ssid1.disabled=true;
		userform.channel1.disabled=true;
	}else{
		userform.name1.disabled=false;
		userform.ssid1.disabled=false;
		userform.channel1.disabled=false;
	}
}
</script>
<body bgcolor="#ffffff" onload="onload_func()">
<form method="get" action="/goform/asp_setWlanBasic" name="userform">
<B>Wireless Basic Settings</b><BR>
<BR>
Wireless Card 1 <input type="checkbox" name="enable0" value=1 onClick="onload_func();" <% asp_wlanBasic("enable0"); %>>Enable<p>
<table cellSpacing=1 cellPadding=2 border=0>
  <tr><td bgColor=#aaddff>Alias Name</td>
      <td bgColor=#ddeeff><input type="text" name="name0" size=32 maxlength=32 value="<% asp_wlanBasic("name0"); %>"></td>      
  </tr>
  
  <tr><td bgColor=#aaddff>SSID</td>
      <td bgColor=#ddeeff><input type="text" name="ssid0" size=16 maxlength=16 value="<% asp_wlanBasic("ssid0"); %>"></td>
  </tr>  

  <tr><td bgColor=#aaddff>Channel Number</td>
      <td bgColor=#ddeeff><select name="channel0">
		<option value=0 <% asp_wlanBasic("ch0_0"); %>>Auto</option>
		<option value=1 <% asp_wlanBasic("ch0_1"); %>>1</option>
		<option value=2 <% asp_wlanBasic("ch0_2"); %>>2</option>
		<option value=3 <% asp_wlanBasic("ch0_3"); %>>3</option>
		<option value=4 <% asp_wlanBasic("ch0_4"); %>>4</option>
		<option value=5 <% asp_wlanBasic("ch0_5"); %>>5</option>
		<option value=6 <% asp_wlanBasic("ch0_6"); %>>6</option>
		<option value=7 <% asp_wlanBasic("ch0_7"); %>>7</option>
		<option value=8 <% asp_wlanBasic("ch0_8"); %>>8</option>
		<option value=9 <% asp_wlanBasic("ch0_9"); %>>9</option>
		<option value=10 <% asp_wlanBasic("ch0_10"); %>>10</option>
		<option value=11 <% asp_wlanBasic("ch0_11"); %>>11</option>
      </td>
  </tr> 
 <tr><td bgColor=#aaddff>Associated Clients</td>
      <td bgColor=#ddeeff></td>
  </tr>
</table>

<BR>
Wireless Card 2<input type="checkbox" name="enable1" value=1 onClick="onload_func();" <% asp_wlanBasic("enable1"); %>>Enable<p>
<table cellSpacing=1 cellPadding=2 border=0 >
  <tr><td bgColor=#aaddff>Alias Name</td>
      <td bgColor=#ddeeff><input type="text" name="name1" size=32 maxlength=32 value="<% asp_wlanBasic("name1"); %>"></td>      
  </tr>
  
  <tr><td bgColor=#aaddff>SSID</td>
      <td bgColor=#ddeeff><input type="text" name="ssid1" size=16 maxlength=16 value="<% asp_wlanBasic("ssid1"); %>"></td>
  </tr>  

  <tr><td bgColor=#aaddff>Channel Number</td>
      <td bgColor=#ddeeff><select name="channel1">
		<option value=0 <% asp_wlanBasic("ch1_0"); %>>Auto</option>
		<option value=1 <% asp_wlanBasic("ch1_1"); %>>1</option>
		<option value=2 <% asp_wlanBasic("ch1_2"); %>>2</option>
		<option value=3 <% asp_wlanBasic("ch1_3"); %>>3</option>
		<option value=4 <% asp_wlanBasic("ch1_4"); %>>4</option>
		<option value=5 <% asp_wlanBasic("ch1_5"); %>>5</option>
		<option value=6 <% asp_wlanBasic("ch1_6"); %>>6</option>
		<option value=7 <% asp_wlanBasic("ch1_7"); %>>7</option>
		<option value=8 <% asp_wlanBasic("ch1_8"); %>>8</option>
		<option value=9 <% asp_wlanBasic("ch1_9"); %>>9</option>
		<option value=10 <% asp_wlanBasic("ch1_10"); %>>10</option>
		<option value=11 <% asp_wlanBasic("ch1_11"); %>>11</option>
      </td>
  </tr> 
 <tr><td bgColor=#aaddff>Associated Clients</td>
      <td bgColor=#ddeeff></td>
  </tr>
 <tr><Td colspan=2 align=center>
     <input type="submit" name="save" value="Apply">&nbsp;&nbsp;&nbsp;&nbsp;
     <input type="reset" value="Reset" name="reset">
  </tr>
</table>

























</form>
</body>
</html>
