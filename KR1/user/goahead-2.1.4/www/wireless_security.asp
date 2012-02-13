<html>
<head>
<meta http-equiv="Content-Type" content="text/html">
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Cache-Control" CONTENT="no-cache">
<script language="javascript">
//0
var open = new Array();
var open_len = '2';
open[0]='No authentication';
open[1]='By RADIUS server';

// 1
var wep64 = new Array();
var wep64_len = '3';
wep64[0]='No authentication';
wep64[1]='Shared key';
wep64[2]='By RADIUS server';

//2
var wep128 = new Array();
var wep128_len = '3';
wep128[0]='No authentication';
wep128[1]='Shared key';
wep128[2]='By RADIUS server';

//3
var tkip = new Array();
var tkip_len = '2';
tkip[0]='WPA personal - pre-shared key';
tkip[1]='WPA enterprise - By RADIUS server';
//4
var aes = new Array();
var aes_len = '2';
aes[0]='WPA personal - pre-shared key';
aes[1]='WPA enterprise - By RADIUS server';

function setAuth(cryptIdx, cardIndex) {
   if(cardIndex==0){
	  crypt = document.userform.cryptType0;
	  auth = document.userform.authType0;
	  key = window.defKey0.style;
	  radius = window.radiusSetting0.style;
  }else if(cardIndex==1){
	  crypt = document.userform.cryptType1;
	  auth = document.userform.authType1;
	  key = window.defKey1.style;
	  radius = window.radiusSetting1.style;
  }

  if(cryptIdx == 0) { //No encryption
    auth.options.length = open_len;
    for(j = 0; j < open_len; j++) {
      auth.options[j] = new Option(open[j],j);
    }
    key.display = "none";
  } else if(cryptIdx == 1) { //WEP64
    auth.options.length = wep64_len;
    for(j = 0; j < wep64_len; j++) {
      auth.options[j] = new Option(wep64[j],j);
    }
    key.display = "";
  } else if(cryptIdx == 2) { //WEP128
    auth.options.length = wep64_len;
    for(j = 0; j < wep64_len; j++) {
      auth.options[j] = new Option(wep64[j],j);
    }
    key.display = "";
  } else if(cryptIdx == 3) { //TKIP
    auth.options.length = tkip_len;
    for(j = 0; j < tkip_len; j++) {
      auth.options[j] = new Option(tkip[j],j);
    }
    key.display = "";
  } else if(cryptIdx == 4) { //AES
    auth.options.length = aes_len;
    for(j = 0; j < aes_len; j++) {
      auth.options[j] = new Option(aes[j],j);
    }
    key.display = "";
  }
}



function setKeyRadius(authIdx, cryptIdx, cardIndex) {
  if(cardIndex==0){
	  crypt = document.userform.cryptType0;
	  auth = document.userform.authType0;
	  key = window.defKey0.style;
	  radius = window.radiusSetting0.style;
  }else if(cardIndex==1){
	  crypt = document.userform.cryptType1;
	  auth = document.userform.authType1;
	  key = window.defKey1.style;
	  radius = window.radiusSetting1.style;
  }

  if(cryptIdx == 0) { //No encryption
	key.display = "none";
    if(authIdx==1)
		radius.display = "";
	else
		radius.display = "none";
  } else if(cryptIdx == 1) { //WEP64
    key.display = "";
    radius.display = "";
    if(authIdx!=2)
		radius.display = "none";    
  } else if(cryptIdx == 2) { //WEP128
    key.display = "";
    radius.display = "";
    if(authIdx!=2)
		radius.display = "none";    
  } else if(cryptIdx == 3) { //TKIP
    if(authIdx==1){
    	key.display = "none";
		radius.display = "";
    }else{
		key.display = "";
		radius.display = "none";			
	}
  } else if(cryptIdx == 4) { //AES
    if(authIdx==1){
    	key.display = "none";
		radius.display = "";
    }else{
		key.display = "";
		radius.display = "none";			
	}
  }
}

function loadDefault() {
 var exist0 = <% asp_wlanBasic("exist0"); %>;
 var exist1 = <% asp_wlanBasic("exist1"); %>;
 
  //WLAN card 0
  if(exist0==1){
	userform.cryptType0.disabled=false;
	userform.authType0.disabled=false;
  	
	  model = document.userform.encryption0;
	  auth = document.userform.authType0;
	  crypt = document.userform.cryptType0;
	  keyValue = window.defKey0.style;
	  radius = window.radiusSetting0.style;

	  crypt.selectedIndex = <% asp_wlanSecurity("cryptType0"); %>;
	  setAuth(crypt.selectedIndex,0);  
	  auth.selectedIndex = <% asp_wlanSecurity("authType0"); %>;
	  setKeyRadius(auth.selectedIndex,crypt.selectedIndex, 0);

  }else{
	userform.cryptType0.disabled=true;
	userform.authType0.disabled=true;
  }
  //WLAN card 1
  if(exist1==1){
	userform.cryptType1.disabled=false;
	userform.authType1.disabled=false;
  	
	  model = document.userform.encryption1;
	  auth = document.userform.authType1;
	  crypt = document.userform.cryptType1;
	  keyValue = window.defKey1.style;
	  radius = window.radiusSetting1.style;

	  crypt.selectedIndex = <% asp_wlanSecurity("cryptType1"); %>;
	  setAuth(crypt.selectedIndex,1);  
	  auth.selectedIndex = <% asp_wlanSecurity("authType1"); %>;
	  setKeyRadius(auth.selectedIndex,crypt.selectedIndex, 1);
  }else{
	userform.cryptType1.disabled=true;
	userform.authType1.disabled=true;
  }

}


</script>
<title>Wireless Security Setup</title>
</head>
<body bgcolor="#ffffff">

<B>Wireless Security Setup (under construction)</b><BR>
<BR>
<form method="get" action="/goform/asp_setWlanSecurity" name="userform">
  <table cellSpacing=1 cellPadding=2 border=0>
  <tr><td bgcolor=#ddffaa>WLAN Card 1: <% asp_wlanSecurity("alias0"); %></td></tr>
   <tr><td bgColor=#aaddff>Data encryption</td>
   <td bgColor=#ddeeff>
        <select name="cryptType0" onChange="setAuth(this.selectedIndex, 0);">
   	<option value=0>No encryption</option>
   	<option value=1>WEP - 64bits</option>
   	<option value=2>WEP - 128bits</option>
  	<option value=3>WPA - TKIP</option>
        <option value=4>WPA - AES</option>
        </select>
    </td>
   </tr><tr>
   <td bgColor=#aaddff>Authentication</td>
   <td bgColor=#ddeeff>
   <select name="authType0" onChange="setKeyRadius(this.selectedIndex,crypt.selectedIndex, 0);">
	<option selected>No authentication</option></select>
   </td>
   </tr>

<tr id="defKey0" STYLE="display:none;"><td bgColor=#aaddff>Key setting</td>
 	<td bgColor=#ddeeff><input type="password" name="key0" size=32 maxlength=64 value="<% asp_wlanSecurity("key0"); %>"><br>
 	<% asp_wlanSecurity("keyType0"); %></td>
</tr>
<tr></tr>
<tr id="radiusSetting0" STYLE="display:none;"><td bgColor=#aaddff>RADIUS Setting</td>
 	<td bgColor=#ddeeff>
 	<table>
 	<tr><Td>Port:</td><td><input type="text" name="port0" size=5 maxlength=5 value="<% asp_wlanSecurity("port0"); %>"></td></tr>
 	<tr><td>IP Address:</td><td><input type="text" name="ipaddr0" size=15 maxlength=15 value="<% asp_wlanSecurity("ipaddr0"); %>"></td></tr>
 	<tr><td>Password:</td><td><input type="password" name="passwd0" size=15 maxlength=16  value="<% asp_wlanSecurity("passwd0"); %>"></td></tr>
 	</table>
 	</td>      
</tr>
</table>


  <table cellSpacing=1 cellPadding=2 border=0>
  <tr><td bgcolor=#ddffaa>WLAN Card 2:<% asp_wlanSecurity("alias1"); %></td></tr>
   <tr><td bgColor=#aaddff>Data encryption</td>
   <td bgColor=#ddeeff>
        <select name="cryptType1" onChange="setAuth(this.selectedIndex, 1);">
   	<option value=0>No encryption</option>
   	<option value=1>WEP - 64bits</option>
   	<option value=2>WEP - 128bits</option>
  	<option value=3>WPA - TKIP</option>
        <option value=4>WPA - AES</option>
        </select>
    </td>
   </tr><tr>
   <td bgColor=#aaddff>Authentication</td>
   <td bgColor=#ddeeff>
   <select name="authType1" onChange="setKeyRadius(this.selectedIndex,crypt.selectedIndex, 0);">
	<option selected>No authentication</option></select>
   </td>
   </tr>

<tr id="defKey1" STYLE="display:none;"><td bgColor=#aaddff>Key setting</td>
 	<td bgColor=#ddeeff><input type="text" name="key1" size=32 maxlength=32 value="<% asp_wlanSecurity("key1"); %>"><% asp_wlanSecurity("keyType1"); %></td>      
</tr>
<tr></tr>
<tr id="radiusSetting1" STYLE="display:none;"><td bgColor=#aaddff>RADIUS Setting</td>
 	<td bgColor=#ddeeff>
 	<table>
 	<tr><Td>Port:</td><td><input type="text" name="port1" size=5 maxlength=5 value="<% asp_wlanSecurity("port1"); %>"></td></tr>
 	<tr><td>IP Address:</td><td><input type="text" name="ipaddr1" size=15 maxlength=15 value="<% asp_wlanSecurity("ipaddr1"); %>"></td></tr>
 	<tr><td>Password:</td><td><input type="password" name="passwd1" size=15 maxlength=16  value="<% asp_wlanSecurity("passwd1"); %>"></td></tr>
 	</table>
 	</td>      
</tr>

 <tr><Td colspan=2 align=center>
     <input type="submit" name="save" value="Apply">&nbsp;&nbsp;&nbsp;&nbsp;
     <input type="reset" value="Reset" name="reset">
  </tr>

</table>
	<script language=javascript>
	loadDefault();
	</script>


</form>
</body>
</html>
