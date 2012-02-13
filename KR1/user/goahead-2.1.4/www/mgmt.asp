<html>
<head>
<meta http-equiv="Content-Type" content="text/html">
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Cache-Control" CONTENT="no-cache">
<script language="javascript">
<!--
function set_defaults()
{

	var cf = document.funcfrm;
	msg = "\nAll config parameters will be set to their default values. \n\nClick OK to continue or Cancel to abort";
	if (confirm(msg))
	{
 		alert('Please restart system to make factory default parameters take effect!');
  		cf.submit();
	}
}

function restart()
{

	var cf = document.funcfrm2;
	msg = "\nWould you want to restart system?\n\nClick OK to continue or Cancel to abort\n";
	if (confirm(msg))
	{
  		cf.submit();
	}
}

function checkPasswd()
{
	if(userform.password.value!=userform.password2.value)
		{
			alert('Two passwords don\'t match!');
			return false;
		}
	return true;	
}

-->
</script>
<title>Management</title>
</head>
<body bgcolor="#ffffff" text="#000000">

<form method="get" action="/goform/asp_setRemoteMgmt" name="userform">
<B>Remote Management</b><BR>
<BR>
<table cellSpacing=1 cellPadding=2 border=0>
<tr><td bgColor=#aaddff>Remote Management IP</td><td bgColor=#ddeeff><input type=text name=remoteMgmtIp size=15 maxlength=15 value="<% asp_remoteMgmt("remoteMgmtIp"); %>"></td></tr>
<tr><td bgColor=#aaddff>Remote Management Netmask</td><td bgColor=#ddeeff><input type=text name=remoteMgmtMask size=15 maxlength=15 value="<% asp_remoteMgmt("remoteMgmtMask"); %>"></td></tr>
<tr><td bgColor=#aaddff>Remote Management Port</td><td bgColor=#ddeeff><input type=text name=remoteMgmtPort size=5 maxlength=5 value="<% asp_remoteMgmt("remoteMgmtPort"); %>"></td></tr>
<tr><td bgColor=#aaddff>Ping from WAN side</td><td bgColor=#ddeeff><input type=checkbox name=remoteIcmpEnable value=1 <% asp_remoteMgmt("remoteIcmpEnable"); %>>Enable</td></tr>
<tr><td colspan=2><input type=submit name="submit" value="Save"></td></tr>
</table>
</form>

<form method="get" action="/goform/asp_setUser" name="userform" onsubmit="return checkPasswd();">
<B>User Management</b><BR>
<BR>
<table cellSpacing=1 cellPadding=2 border=0>
<tr><td bgColor=#aaddff>Account</td><td bgColor=#ddeeff><input type=text name=account value="<% asp_user("account"); %>"</td></tr>
<tr><td bgColor=#aaddff>Password</td><td bgColor=#ddeeff><input type=password name=password value="<% asp_user("password"); %>"</td></tr>
<tr><td bgColor=#aaddff>Retype your Password</td><td bgColor=#ddeeff><input type=password name=password2 value="<% asp_user("password"); %>"</td></tr>
<tr><td colspan=2><input type=submit name="submit" value="Save"></td></tr>
</table>
</form>

<form method="get" action="/goform/asp_setTZ">
<B>Time Zone Management</b><BR>
<BR>

<table cellSpacing=1 cellPadding=2 border=0>
<tr>
	<td bgColor=#aaddff>Time Zone</td>
	<td bgColor=#ddeeff>

<select name="timeZone">
<option value="001-00:00"<% asp_TZ(1); %>>Africa/Abidjan
<option value="002-00:00"<% asp_TZ(2); %>>Africa/Accra
<option value="003-03:00"<% asp_TZ(3); %>>Africa/Addis_Ababa
<option value="004-01:00"<% asp_TZ(4); %>>Africa/Algiers
<option value="005-03:00"<% asp_TZ(5); %>>Africa/Asmera
<option value="006-00:00"<% asp_TZ(6); %>>Africa/Bamako
<option value="007-01:00"<% asp_TZ(7); %>>Africa/Bangui
<option value="008-00:00"<% asp_TZ(8); %>>Africa/Banjul
<option value="009-00:00"<% asp_TZ(9); %>>Africa/Bissau
<option value="010-02:00"<% asp_TZ(10); %>>Africa/Blantyre
<option value="011-01:00"<% asp_TZ(11); %>>Africa/Brazzaville
<option value="012-02:00"<% asp_TZ(12); %>>Africa/Bujumbura
<option value="013-02:00"<% asp_TZ(13); %>>Africa/Cairo
<option value="014-00:00"<% asp_TZ(14); %>>Africa/Casablanca
<option value="015-02:00"<% asp_TZ(15); %>>Africa/Ceuta
<option value="016-00:00"<% asp_TZ(16); %>>Africa/Conakry
<option value="017-00:00"<% asp_TZ(17); %>>Africa/Dakar
<option value="018-03:00"<% asp_TZ(18); %>>Africa/Dar_es_Salaam
<option value="019-03:00"<% asp_TZ(19); %>>Africa/Djibouti
<option value="020-01:00"<% asp_TZ(20); %>>Africa/Douala
<option value="021-00:00"<% asp_TZ(21); %>>Africa/El_Aaiun
<option value="022-00:00"<% asp_TZ(22); %>>Africa/Freetown
<option value="023-02:00"<% asp_TZ(23); %>>Africa/Gaborone
<option value="024-02:00"<% asp_TZ(24); %>>Africa/Harare
<option value="025-02:00"<% asp_TZ(25); %>>Africa/Johannesburg
<option value="026-03:00"<% asp_TZ(26); %>>Africa/Kampala
<option value="027-03:00"<% asp_TZ(27); %>>Africa/Khartoum
<option value="028-02:00"<% asp_TZ(28); %>>Africa/Kigali
<option value="029-01:00"<% asp_TZ(29); %>>Africa/Kinshasa
<option value="030-01:00"<% asp_TZ(30); %>>Africa/Lagos
<option value="031-01:00"<% asp_TZ(31); %>>Africa/Libreville
<option value="032-00:00"<% asp_TZ(32); %>>Africa/Lome
<option value="033-01:00"<% asp_TZ(33); %>>Africa/Luanda
<option value="034-02:00"<% asp_TZ(34); %>>Africa/Lubumbashi
<option value="035-02:00"<% asp_TZ(35); %>>Africa/Lusaka
<option value="036-01:00"<% asp_TZ(36); %>>Africa/Malabo
<option value="037-02:00"<% asp_TZ(37); %>>Africa/Maputo
<option value="038-02:00"<% asp_TZ(38); %>>Africa/Maseru
<option value="039-02:00"<% asp_TZ(39); %>>Africa/Mbabane
<option value="040-03:00"<% asp_TZ(40); %>>Africa/Mogadishu
<option value="041-00:00"<% asp_TZ(41); %>>Africa/Monrovia
<option value="042-03:00"<% asp_TZ(42); %>>Africa/Nairobi
<option value="043-01:00"<% asp_TZ(43); %>>Africa/Ndjamena
<option value="044-01:00"<% asp_TZ(44); %>>Africa/Niamey
<option value="045-00:00"<% asp_TZ(45); %>>Africa/Nouakchott
<option value="046-00:00"<% asp_TZ(46); %>>Africa/Ouagadougou
<option value="047-01:00"<% asp_TZ(47); %>>Africa/Porto-Novo
<option value="048-00:00"<% asp_TZ(48); %>>Africa/Sao_Tome
<option value="049-00:00"<% asp_TZ(49); %>>Africa/Timbuktu
<option value="050-02:00"<% asp_TZ(50); %>>Africa/Tripoli
<option value="051-01:00"<% asp_TZ(51); %>>Africa/Tunis
<option value="052-02:00"<% asp_TZ(52); %>>Africa/Windhoek
<option value="053+09:00"<% asp_TZ(53); %>>America/Adak
<option value="054+08:00"<% asp_TZ(54); %>>America/Anchorage
<option value="055+04:00"<% asp_TZ(55); %>>America/Anguilla
<option value="056+04:00"<% asp_TZ(56); %>>America/Antigua
<option value="057+03:00"<% asp_TZ(57); %>>America/Araguaina
<option value="058+04:00"<% asp_TZ(58); %>>America/Aruba
<option value="059+03:00"<% asp_TZ(59); %>>America/Asuncion
<option value="060+09:00"<% asp_TZ(60); %>>America/Atka
<option value="061+04:00"<% asp_TZ(61); %>>America/Barbados
<option value="062+03:00"<% asp_TZ(62); %>>America/Belem
<option value="063+06:00"<% asp_TZ(63); %>>America/Belize
<option value="064+04:00"<% asp_TZ(64); %>>America/Boa_Vista
<option value="065+05:00"<% asp_TZ(65); %>>America/Bogota
<option value="066+06:00"<% asp_TZ(66); %>>America/Boise
<option value="067+03:00"<% asp_TZ(67); %>>America/Buenos_Aires
<option value="068+06:00"<% asp_TZ(68); %>>America/Cambridge_Bay
<option value="069+05:00"<% asp_TZ(69); %>>America/Cancun
<option value="070+04:00"<% asp_TZ(70); %>>America/Caracas
<option value="071+03:00"<% asp_TZ(71); %>>America/Catamarca
<option value="072+03:00"<% asp_TZ(72); %>>America/Cayenne
<option value="073+05:00"<% asp_TZ(73); %>>America/Cayman
<option value="074+05:00"<% asp_TZ(74); %>>America/Chicago
<option value="075+06:00"<% asp_TZ(75); %>>America/Chihuahua
<option value="076+03:00"<% asp_TZ(76); %>>America/Cordoba
<option value="077+06:00"<% asp_TZ(77); %>>America/Costa_Rica
<option value="078+04:00"<% asp_TZ(78); %>>America/Cuiaba
<option value="079+04:00"<% asp_TZ(79); %>>America/Curacao
<option value="080-00:00"<% asp_TZ(80); %>>America/Danmarkshavn
<option value="081+07:00"<% asp_TZ(81); %>>America/Dawson
<option value="082+07:00"<% asp_TZ(82); %>>America/Dawson_Creek
<option value="083+06:00"<% asp_TZ(83); %>>America/Denver
<option value="084+04:00"<% asp_TZ(84); %>>America/Detroit
<option value="085+04:00"<% asp_TZ(85); %>>America/Dominica
<option value="086+06:00"<% asp_TZ(86); %>>America/Edmonton
<option value="087+05:00"<% asp_TZ(87); %>>America/Eirunepe
<option value="088+06:00"<% asp_TZ(88); %>>America/El_Salvador
<option value="089+07:00"<% asp_TZ(89); %>>America/Ensenada
<option value="090+05:00"<% asp_TZ(90); %>>America/Fort_Wayne
<option value="091+03:00"<% asp_TZ(91); %>>America/Fortaleza
<option value="092+03:00"<% asp_TZ(92); %>>America/Glace_Bay
<option value="093+02:00"<% asp_TZ(93); %>>America/Godthab
<option value="094+03:00"<% asp_TZ(94); %>>America/Goose_Bay
<option value="095+04:00"<% asp_TZ(95); %>>America/Grand_Turk
<option value="096+04:00"<% asp_TZ(96); %>>America/Grenada
<option value="097+04:00"<% asp_TZ(97); %>>America/Guadeloupe
<option value="098+06:00"<% asp_TZ(98); %>>America/Guatemala
<option value="099+05:00"<% asp_TZ(99); %>>America/Guayaquil
<option value="100+04:00"<% asp_TZ(100); %>>America/Guyana
<option value="101+03:00"<% asp_TZ(101); %>>America/Halifax
<option value="102+04:00"<% asp_TZ(102); %>>America/Havana
<option value="103+07:00"<% asp_TZ(103); %>>America/Hermosillo
<option value="104+05:00"<% asp_TZ(104); %>>America/Indiana/Indianapolis
<option value="105+05:00"<% asp_TZ(105); %>>America/Indiana/Knox
<option value="106+05:00"<% asp_TZ(106); %>>America/Indiana/Marengo
<option value="107+05:00"<% asp_TZ(107); %>>America/Indiana/Vevay
<option value="108+05:00"<% asp_TZ(108); %>>America/Indianapolis
<option value="109+06:00"<% asp_TZ(109); %>>America/Inuvik
<option value="110+04:00"<% asp_TZ(110); %>>America/Iqaluit
<option value="111+05:00"<% asp_TZ(111); %>>America/Jamaica
<option value="112+03:00"<% asp_TZ(112); %>>America/Jujuy
<option value="113+08:00"<% asp_TZ(113); %>>America/Juneau
<option value="114+04:00"<% asp_TZ(114); %>>America/Kentucky/Louisville
<option value="115+04:00"<% asp_TZ(115); %>>America/Kentucky/Monticello
<option value="116+05:00"<% asp_TZ(116); %>>America/Knox_IN
<option value="117+04:00"<% asp_TZ(117); %>>America/La_Paz
<option value="118+05:00"<% asp_TZ(118); %>>America/Lima
<option value="119+07:00"<% asp_TZ(119); %>>America/Los_Angeles
<option value="120+04:00"<% asp_TZ(120); %>>America/Louisville
<option value="121+03:00"<% asp_TZ(121); %>>America/Maceio
<option value="122+06:00"<% asp_TZ(122); %>>America/Managua
<option value="123+04:00"<% asp_TZ(123); %>>America/Manaus
<option value="124+04:00"<% asp_TZ(124); %>>America/Martinique
<option value="125+06:00"<% asp_TZ(125); %>>America/Mazatlan
<option value="126+03:00"<% asp_TZ(126); %>>America/Mendoza
<option value="127+05:00"<% asp_TZ(127); %>>America/Menominee
<option value="128+05:00"<% asp_TZ(128); %>>America/Merida
<option value="129+05:00"<% asp_TZ(129); %>>America/Mexico_City
<option value="130+02:00"<% asp_TZ(130); %>>America/Miquelon
<option value="131+05:00"<% asp_TZ(131); %>>America/Monterrey
<option value="132+03:00"<% asp_TZ(132); %>>America/Montevideo
<option value="133+04:00"<% asp_TZ(133); %>>America/Montreal
<option value="134+04:00"<% asp_TZ(134); %>>America/Montserrat
<option value="135+04:00"<% asp_TZ(135); %>>America/Nassau
<option value="136+04:00"<% asp_TZ(136); %>>America/New_York
<option value="137+04:00"<% asp_TZ(137); %>>America/Nipigon
<option value="138+08:00"<% asp_TZ(138); %>>America/Nome
<option value="139+02:00"<% asp_TZ(139); %>>America/Noronha
<option value="140+05:00"<% asp_TZ(140); %>>America/North_Dakota/Center
<option value="141+05:00"<% asp_TZ(141); %>>America/Panama
<option value="142+04:00"<% asp_TZ(142); %>>America/Pangnirtung
<option value="143+03:00"<% asp_TZ(143); %>>America/Paramaribo
<option value="144+07:00"<% asp_TZ(144); %>>America/Phoenix
<option value="145+05:00"<% asp_TZ(145); %>>America/Port-au-Prince
<option value="146+04:00"<% asp_TZ(146); %>>America/Port_of_Spain
<option value="147+05:00"<% asp_TZ(147); %>>America/Porto_Acre
<option value="148+04:00"<% asp_TZ(148); %>>America/Porto_Velho
<option value="149+04:00"<% asp_TZ(149); %>>America/Puerto_Rico
<option value="150+05:00"<% asp_TZ(150); %>>America/Rainy_River
<option value="151+05:00"<% asp_TZ(151); %>>America/Rankin_Inlet
<option value="152+03:00"<% asp_TZ(152); %>>America/Recife
<option value="153+06:00"<% asp_TZ(153); %>>America/Regina
<option value="154+05:00"<% asp_TZ(154); %>>America/Rio_Branco
<option value="155+03:00"<% asp_TZ(155); %>>America/Rosario
<option value="156+04:00"<% asp_TZ(156); %>>America/Santiago
<option value="157+04:00"<% asp_TZ(157); %>>America/Santo_Domingo
<option value="158+03:00"<% asp_TZ(158); %>>America/Sao_Paulo
<option value="159-00:00"<% asp_TZ(159); %>>America/Scoresbysund
<option value="160+06:00"<% asp_TZ(160); %>>America/Shiprock
<option value="161+02:30"<% asp_TZ(161); %>>America/St_Johns
<option value="162+04:00"<% asp_TZ(162); %>>America/St_Kitts
<option value="163+04:00"<% asp_TZ(163); %>>America/St_Lucia
<option value="164+04:00"<% asp_TZ(164); %>>America/St_Thomas
<option value="165+04:00"<% asp_TZ(165); %>>America/St_Vincent
<option value="166+06:00"<% asp_TZ(166); %>>America/Swift_Current
<option value="167+06:00"<% asp_TZ(167); %>>America/Tegucigalpa
<option value="168+03:00"<% asp_TZ(168); %>>America/Thule
<option value="169+04:00"<% asp_TZ(169); %>>America/Thunder_Bay
<option value="170+07:00"<% asp_TZ(170); %>>America/Tijuana
<option value="171+04:00"<% asp_TZ(171); %>>America/Tortola
<option value="172+07:00"<% asp_TZ(172); %>>America/Vancouver
<option value="173+04:00"<% asp_TZ(173); %>>America/Virgin
<option value="174+07:00"<% asp_TZ(174); %>>America/Whitehorse
<option value="175+05:00"<% asp_TZ(175); %>>America/Winnipeg
<option value="176+08:00"<% asp_TZ(176); %>>America/Yakutat
<option value="177+06:00"<% asp_TZ(177); %>>America/Yellowknife
<option value="178-08:00"<% asp_TZ(178); %>>Antarctica/Casey
<option value="179-07:00"<% asp_TZ(179); %>>Antarctica/Davis
<option value="180-10:00"<% asp_TZ(180); %>>Antarctica/DumontDUrville
<option value="181-06:00"<% asp_TZ(181); %>>Antarctica/Mawson
<option value="182-12:00"<% asp_TZ(182); %>>Antarctica/McMurdo
<option value="183+04:00"<% asp_TZ(183); %>>Antarctica/Palmer
<option value="184+03:00"<% asp_TZ(184); %>>Antarctica/Rothera
<option value="185-12:00"<% asp_TZ(185); %>>Antarctica/South_Pole
<option value="186-03:00"<% asp_TZ(186); %>>Antarctica/Syowa
<option value="187-06:00"<% asp_TZ(187); %>>Antarctica/Vostok
<option value="188-02:00"<% asp_TZ(188); %>>Arctic/Longyearbyen
<option value="189-03:00"<% asp_TZ(189); %>>Asia/Aden
<option value="190-07:00"<% asp_TZ(190); %>>Asia/Almaty
<option value="191-02:00"<% asp_TZ(191); %>>Asia/Amman
<option value="192-13:00"<% asp_TZ(192); %>>Asia/Anadyr
<option value="193-05:00"<% asp_TZ(193); %>>Asia/Aqtau
<option value="194-06:00"<% asp_TZ(194); %>>Asia/Aqtobe
<option value="195-05:00"<% asp_TZ(195); %>>Asia/Ashgabat
<option value="196-05:00"<% asp_TZ(196); %>>Asia/Ashkhabad
<option value="197-03:00"<% asp_TZ(197); %>>Asia/Baghdad
<option value="198-03:00"<% asp_TZ(198); %>>Asia/Bahrain
<option value="199-05:00"<% asp_TZ(199); %>>Asia/Baku
<option value="200-07:00"<% asp_TZ(200); %>>Asia/Bangkok
<option value="201-03:00"<% asp_TZ(201); %>>Asia/Beirut
<option value="202-06:00"<% asp_TZ(202); %>>Asia/Bishkek
<option value="203-08:00"<% asp_TZ(203); %>>Asia/Brunei
<option value="204-05:30"<% asp_TZ(204); %>>Asia/Calcutta
<option value="205-09:00"<% asp_TZ(205); %>>Asia/Choibalsan
<option value="206-08:00"<% asp_TZ(206); %>>Asia/Chongqing
<option value="207-08:00"<% asp_TZ(207); %>>Asia/Chungking
<option value="208-06:00"<% asp_TZ(208); %>>Asia/Colombo
<option value="209-06:00"<% asp_TZ(209); %>>Asia/Dacca
<option value="210-02:00"<% asp_TZ(210); %>>Asia/Damascus
<option value="211-06:00"<% asp_TZ(211); %>>Asia/Dhaka
<option value="212-09:00"<% asp_TZ(212); %>>Asia/Dili
<option value="213-04:00"<% asp_TZ(213); %>>Asia/Dubai
<option value="214-05:00"<% asp_TZ(214); %>>Asia/Dushanbe
<option value="215-03:00"<% asp_TZ(215); %>>Asia/Gaza
<option value="216-08:00"<% asp_TZ(216); %>>Asia/Harbin
<option value="217-08:00"<% asp_TZ(217); %>>Asia/Hong_Kong
<option value="218-07:00"<% asp_TZ(218); %>>Asia/Hovd
<option value="219-09:00"<% asp_TZ(219); %>>Asia/Irkutsk
<option value="220-03:00"<% asp_TZ(220); %>>Asia/Istanbul
<option value="221-07:00"<% asp_TZ(221); %>>Asia/Jakarta
<option value="222-09:00"<% asp_TZ(222); %>>Asia/Jayapura
<option value="223-02:00"<% asp_TZ(223); %>>Asia/Jerusalem
<option value="224-04:30"<% asp_TZ(224); %>>Asia/Kabul
<option value="225-13:00"<% asp_TZ(225); %>>Asia/Kamchatka
<option value="226-05:00"<% asp_TZ(226); %>>Asia/Karachi
<option value="227-08:00"<% asp_TZ(227); %>>Asia/Kashgar
<option value="228-05:45"<% asp_TZ(228); %>>Asia/Katmandu
<option value="229-08:00"<% asp_TZ(229); %>>Asia/Krasnoyarsk
<option value="230-08:00"<% asp_TZ(230); %>>Asia/Kuala_Lumpur
<option value="231-08:00"<% asp_TZ(231); %>>Asia/Kuching
<option value="232-03:00"<% asp_TZ(232); %>>Asia/Kuwait
<option value="233-08:00"<% asp_TZ(233); %>>Asia/Macao
<option value="234-08:00"<% asp_TZ(234); %>>Asia/Macau
<option value="235-12:00"<% asp_TZ(235); %>>Asia/Magadan
<option value="236-08:00"<% asp_TZ(236); %>>Asia/Makassar
<option value="237-08:00"<% asp_TZ(237); %>>Asia/Manila
<option value="238-04:00"<% asp_TZ(238); %>>Asia/Muscat
<option value="239-03:00"<% asp_TZ(239); %>>Asia/Nicosia
<option value="240-07:00"<% asp_TZ(240); %>>Asia/Novosibirsk
<option value="241-07:00"<% asp_TZ(241); %>>Asia/Omsk
<option value="242-05:00"<% asp_TZ(242); %>>Asia/Oral
<option value="243-07:00"<% asp_TZ(243); %>>Asia/Phnom_Penh
<option value="244-07:00"<% asp_TZ(244); %>>Asia/Pontianak
<option value="245-09:00"<% asp_TZ(245); %>>Asia/Pyongyang
<option value="246-03:00"<% asp_TZ(246); %>>Asia/Qatar
<option value="247-07:00"<% asp_TZ(247); %>>Asia/Qyzylorda
<option value="248-06:30"<% asp_TZ(248); %>>Asia/Rangoon
<option value="249-03:00"<% asp_TZ(249); %>>Asia/Riyadh
<option value="250-03:07"<% asp_TZ(250); %>>Asia/Riyadh87
<option value="251-03:07"<% asp_TZ(251); %>>Asia/Riyadh88
<option value="252-03:07"<% asp_TZ(252); %>>Asia/Riyadh89
<option value="253-07:00"<% asp_TZ(253); %>>Asia/Saigon
<option value="254-11:00"<% asp_TZ(254); %>>Asia/Sakhalin
<option value="255-05:00"<% asp_TZ(255); %>>Asia/Samarkand
<option value="256-09:00"<% asp_TZ(256); %>>Asia/Seoul
<option value="257-08:00"<% asp_TZ(257); %>>Asia/Shanghai
<option value="258-08:00"<% asp_TZ(258); %>>Asia/Singapore
<option value="259-08:00"<% asp_TZ(259); %>>Asia/Taipei
<option value="260-05:00"<% asp_TZ(260); %>>Asia/Tashkent
<option value="261-05:00"<% asp_TZ(261); %>>Asia/Tbilisi
<option value="262-03:30"<% asp_TZ(262); %>>Asia/Tehran
<option value="263-02:00"<% asp_TZ(263); %>>Asia/Tel_Aviv
<option value="264-06:00"<% asp_TZ(264); %>>Asia/Thimbu
<option value="265-06:00"<% asp_TZ(265); %>>Asia/Thimphu
<option value="266-09:00"<% asp_TZ(266); %>>Asia/Tokyo
<option value="267-08:00"<% asp_TZ(267); %>>Asia/Ujung_Pandang
<option value="268-08:00"<% asp_TZ(268); %>>Asia/Ulaanbaatar
<option value="269-08:00"<% asp_TZ(269); %>>Asia/Ulan_Bator
<option value="270-08:00"<% asp_TZ(270); %>>Asia/Urumqi
<option value="271-07:00"<% asp_TZ(271); %>>Asia/Vientiane
<option value="272-11:00"<% asp_TZ(272); %>>Asia/Vladivostok
<option value="273-10:00"<% asp_TZ(273); %>>Asia/Yakutsk
<option value="274-06:00"<% asp_TZ(274); %>>Asia/Yekaterinburg
<option value="275-05:00"<% asp_TZ(275); %>>Asia/Yerevan
<option value="276-00:00"<% asp_TZ(276); %>>Atlantic/Azores
<option value="277+03:00"<% asp_TZ(277); %>>Atlantic/Bermuda
<option value="278-01:00"<% asp_TZ(278); %>>Atlantic/Canary
<option value="279+01:00"<% asp_TZ(279); %>>Atlantic/Cape_Verde
<option value="280-01:00"<% asp_TZ(280); %>>Atlantic/Faeroe
<option value="281-02:00"<% asp_TZ(281); %>>Atlantic/Jan_Mayen
<option value="282-01:00"<% asp_TZ(282); %>>Atlantic/Madeira
<option value="283-00:00"<% asp_TZ(283); %>>Atlantic/Reykjavik
<option value="284+02:00"<% asp_TZ(284); %>>Atlantic/South_Georgia
<option value="285-00:00"<% asp_TZ(285); %>>Atlantic/St_Helena
<option value="286+03:00"<% asp_TZ(286); %>>Atlantic/Stanley
<option value="287-10:00"<% asp_TZ(287); %>>Australia/ACT
<option value="288-09:30"<% asp_TZ(288); %>>Australia/Adelaide
<option value="289-10:00"<% asp_TZ(289); %>>Australia/Brisbane
<option value="290-09:30"<% asp_TZ(290); %>>Australia/Broken_Hill
<option value="291-10:00"<% asp_TZ(291); %>>Australia/Canberra
<option value="292-09:30"<% asp_TZ(292); %>>Australia/Darwin
<option value="293-10:00"<% asp_TZ(293); %>>Australia/Hobart
<option value="294-10:30"<% asp_TZ(294); %>>Australia/LHI
<option value="295-10:00"<% asp_TZ(295); %>>Australia/Lindeman
<option value="296-10:30"<% asp_TZ(296); %>>Australia/Lord_Howe
<option value="297-10:00"<% asp_TZ(297); %>>Australia/Melbourne
<option value="298-10:00"<% asp_TZ(298); %>>Australia/NSW
<option value="299-09:30"<% asp_TZ(299); %>>Australia/North
<option value="300-08:00"<% asp_TZ(300); %>>Australia/Perth
<option value="301-10:00"<% asp_TZ(301); %>>Australia/Queensland
<option value="302-09:30"<% asp_TZ(302); %>>Australia/South
<option value="303-10:00"<% asp_TZ(303); %>>Australia/Sydney
<option value="304-10:00"<% asp_TZ(304); %>>Australia/Tasmania
<option value="305-10:00"<% asp_TZ(305); %>>Australia/Victoria
<option value="306-08:00"<% asp_TZ(306); %>>Australia/West
<option value="307-09:30"<% asp_TZ(307); %>>Australia/Yancowinna
<option value="308+05:00"<% asp_TZ(308); %>>Brazil/Acre
<option value="309+02:00"<% asp_TZ(309); %>>Brazil/DeNoronha
<option value="310+03:00"<% asp_TZ(310); %>>Brazil/East
<option value="311+04:00"<% asp_TZ(311); %>>Brazil/West
<option value="312-02:00"<% asp_TZ(312); %>>CET
<option value="313+05:00"<% asp_TZ(313); %>>CST6CDT
<option value="314+03:00"<% asp_TZ(314); %>>Canada/Atlantic
<option value="315+05:00"<% asp_TZ(315); %>>Canada/Central
<option value="316+06:00"<% asp_TZ(316); %>>Canada/East-Saskatchewan
<option value="317+04:00"<% asp_TZ(317); %>>Canada/Eastern
<option value="318+06:00"<% asp_TZ(318); %>>Canada/Mountain
<option value="319+02:30"<% asp_TZ(319); %>>Canada/Newfoundland
<option value="320+07:00"<% asp_TZ(320); %>>Canada/Pacific
<option value="321+06:00"<% asp_TZ(321); %>>Canada/Saskatchewan
<option value="322+07:00"<% asp_TZ(322); %>>Canada/Yukon
<option value="323+04:00"<% asp_TZ(323); %>>Chile/Continental
<option value="324+06:00"<% asp_TZ(324); %>>Chile/EasterIsland
<option value="325+04:00"<% asp_TZ(325); %>>Cuba
<option value="326-03:00"<% asp_TZ(326); %>>EET
<option value="327+05:00"<% asp_TZ(327); %>>EST
<option value="328+04:00"<% asp_TZ(328); %>>EST5EDT
<option value="329-02:00"<% asp_TZ(329); %>>Egypt
<option value="330-01:00"<% asp_TZ(330); %>>Eire
<option value="331-02:00"<% asp_TZ(331); %>>Europe/Amsterdam
<option value="332-02:00"<% asp_TZ(332); %>>Europe/Andorra
<option value="333-03:00"<% asp_TZ(333); %>>Europe/Athens
<option value="334-01:00"<% asp_TZ(334); %>>Europe/Belfast
<option value="335-02:00"<% asp_TZ(335); %>>Europe/Belgrade
<option value="336-02:00"<% asp_TZ(336); %>>Europe/Berlin
<option value="337-02:00"<% asp_TZ(337); %>>Europe/Bratislava
<option value="338-02:00"<% asp_TZ(338); %>>Europe/Brussels
<option value="339-03:00"<% asp_TZ(339); %>>Europe/Bucharest
<option value="340-02:00"<% asp_TZ(340); %>>Europe/Budapest
<option value="341-03:00"<% asp_TZ(341); %>>Europe/Chisinau
<option value="342-02:00"<% asp_TZ(342); %>>Europe/Copenhagen
<option value="343-01:00"<% asp_TZ(343); %>>Europe/Dublin
<option value="344-02:00"<% asp_TZ(344); %>>Europe/Gibraltar
<option value="345-03:00"<% asp_TZ(345); %>>Europe/Helsinki
<option value="346-03:00"<% asp_TZ(346); %>>Europe/Istanbul
<option value="347-03:00"<% asp_TZ(347); %>>Europe/Kaliningrad
<option value="348-03:00"<% asp_TZ(348); %>>Europe/Kiev
<option value="349-01:00"<% asp_TZ(349); %>>Europe/Lisbon
<option value="350-02:00"<% asp_TZ(350); %>>Europe/Ljubljana
<option value="351-01:00"<% asp_TZ(351); %>>Europe/London
<option value="352-02:00"<% asp_TZ(352); %>>Europe/Luxembourg
<option value="353-02:00"<% asp_TZ(353); %>>Europe/Madrid
<option value="354-02:00"<% asp_TZ(354); %>>Europe/Malta
<option value="355-03:00"<% asp_TZ(355); %>>Europe/Minsk
<option value="356-02:00"<% asp_TZ(356); %>>Europe/Monaco
<option value="357-04:00"<% asp_TZ(357); %>>Europe/Moscow
<option value="358-03:00"<% asp_TZ(358); %>>Europe/Nicosia
<option value="359-02:00"<% asp_TZ(359); %>>Europe/Oslo
<option value="360-02:00"<% asp_TZ(360); %>>Europe/Paris
<option value="361-02:00"<% asp_TZ(361); %>>Europe/Prague
<option value="362-03:00"<% asp_TZ(362); %>>Europe/Riga
<option value="363-02:00"<% asp_TZ(363); %>>Europe/Rome
<option value="364-05:00"<% asp_TZ(364); %>>Europe/Samara
<option value="365-02:00"<% asp_TZ(365); %>>Europe/San_Marino
<option value="366-02:00"<% asp_TZ(366); %>>Europe/Sarajevo
<option value="367-03:00"<% asp_TZ(367); %>>Europe/Simferopol
<option value="368-02:00"<% asp_TZ(368); %>>Europe/Skopje
<option value="369-03:00"<% asp_TZ(369); %>>Europe/Sofia
<option value="370-02:00"<% asp_TZ(370); %>>Europe/Stockholm
<option value="371-03:00"<% asp_TZ(371); %>>Europe/Tallinn
<option value="372-02:00"<% asp_TZ(372); %>>Europe/Tirane
<option value="373-03:00"<% asp_TZ(373); %>>Europe/Tiraspol
<option value="374-03:00"<% asp_TZ(374); %>>Europe/Uzhgorod
<option value="375-02:00"<% asp_TZ(375); %>>Europe/Vaduz
<option value="376-02:00"<% asp_TZ(376); %>>Europe/Vatican
<option value="377-02:00"<% asp_TZ(377); %>>Europe/Vienna
<option value="378-03:00"<% asp_TZ(378); %>>Europe/Vilnius
<option value="379-02:00"<% asp_TZ(379); %>>Europe/Warsaw
<option value="380-02:00"<% asp_TZ(380); %>>Europe/Zagreb
<option value="381-03:00"<% asp_TZ(381); %>>Europe/Zaporozhye
<option value="382-02:00"<% asp_TZ(382); %>>Europe/Zurich
<option value="383-01:00"<% asp_TZ(383); %>>GB
<option value="384-01:00"<% asp_TZ(384); %>>GB-Eire
<option value="385+10:00"<% asp_TZ(385); %>>HST
<option value="386-00:00"<% asp_TZ(386); %>>Iceland
<option value="387-03:00"<% asp_TZ(387); %>>Indian/Antananarivo
<option value="388-06:00"<% asp_TZ(388); %>>Indian/Chagos
<option value="389-07:00"<% asp_TZ(389); %>>Indian/Christmas
<option value="390-06:30"<% asp_TZ(390); %>>Indian/Cocos
<option value="391-03:00"<% asp_TZ(391); %>>Indian/Comoro
<option value="392-05:00"<% asp_TZ(392); %>>Indian/Kerguelen
<option value="393-04:00"<% asp_TZ(393); %>>Indian/Mahe
<option value="394-05:00"<% asp_TZ(394); %>>Indian/Maldives
<option value="395-04:00"<% asp_TZ(395); %>>Indian/Mauritius
<option value="396-03:00"<% asp_TZ(396); %>>Indian/Mayotte
<option value="397-04:00"<% asp_TZ(397); %>>Indian/Reunion
<option value="398-03:30"<% asp_TZ(398); %>>Iran
<option value="399-02:00"<% asp_TZ(399); %>>Israel
<option value="400+05:00"<% asp_TZ(400); %>>Jamaica
<option value="401-09:00"<% asp_TZ(401); %>>Japan
<option value="402-12:00"<% asp_TZ(402); %>>Kwajalein
<option value="403-02:00"<% asp_TZ(403); %>>Libya
<option value="404-02:00"<% asp_TZ(404); %>>MET
<option value="405+07:00"<% asp_TZ(405); %>>MST
<option value="406+06:00"<% asp_TZ(406); %>>MST7MDT
<option value="407+07:00"<% asp_TZ(407); %>>Mexico/BajaNorte
<option value="408+06:00"<% asp_TZ(408); %>>Mexico/BajaSur
<option value="409+05:00"<% asp_TZ(409); %>>Mexico/General
<option value="410-03:07"<% asp_TZ(410); %>>Mideast/Riyadh87
<option value="411-03:07"<% asp_TZ(411); %>>Mideast/Riyadh88
<option value="412-03:07"<% asp_TZ(412); %>>Mideast/Riyadh89
<option value="413-12:00"<% asp_TZ(413); %>>NZ
<option value="414-12:45"<% asp_TZ(414); %>>NZ-CHAT
<option value="415+06:00"<% asp_TZ(415); %>>Navajo
<option value="416-08:00"<% asp_TZ(416); %>>PRC
<option value="417+07:00"<% asp_TZ(417); %>>PST8PDT
<option value="418+11:00"<% asp_TZ(418); %>>Pacific/Apia
<option value="419-12:00"<% asp_TZ(419); %>>Pacific/Auckland
<option value="420-12:45"<% asp_TZ(420); %>>Pacific/Chatham
<option value="421+06:00"<% asp_TZ(421); %>>Pacific/Easter
<option value="422-11:00"<% asp_TZ(422); %>>Pacific/Efate
<option value="423-13:00"<% asp_TZ(423); %>>Pacific/Enderbury
<option value="424+10:00"<% asp_TZ(424); %>>Pacific/Fakaofo
<option value="425-12:00"<% asp_TZ(425); %>>Pacific/Fiji
<option value="426-12:00"<% asp_TZ(426); %>>Pacific/Funafuti
<option value="427+06:00"<% asp_TZ(427); %>>Pacific/Galapagos
<option value="428+09:00"<% asp_TZ(428); %>>Pacific/Gambier
<option value="429-11:00"<% asp_TZ(429); %>>Pacific/Guadalcanal
<option value="430-10:00"<% asp_TZ(430); %>>Pacific/Guam
<option value="431+10:00"<% asp_TZ(431); %>>Pacific/Honolulu
<option value="432+10:00"<% asp_TZ(432); %>>Pacific/Johnston
<option value="433-14:00"<% asp_TZ(433); %>>Pacific/Kiritimati
<option value="434-11:00"<% asp_TZ(434); %>>Pacific/Kosrae
<option value="435-12:00"<% asp_TZ(435); %>>Pacific/Kwajalein
<option value="436-12:00"<% asp_TZ(436); %>>Pacific/Majuro
<option value="437+09:30"<% asp_TZ(437); %>>Pacific/Marquesas
<option value="438+11:00"<% asp_TZ(438); %>>Pacific/Midway
<option value="439-12:00"<% asp_TZ(439); %>>Pacific/Nauru
<option value="440+11:00"<% asp_TZ(440); %>>Pacific/Niue
<option value="441-11:30"<% asp_TZ(441); %>>Pacific/Norfolk
<option value="442-11:00"<% asp_TZ(442); %>>Pacific/Noumea
<option value="443+11:00"<% asp_TZ(443); %>>Pacific/Pago_Pago
<option value="444-09:00"<% asp_TZ(444); %>>Pacific/Palau
<option value="445+08:00"<% asp_TZ(445); %>>Pacific/Pitcairn
<option value="446-11:00"<% asp_TZ(446); %>>Pacific/Ponape
<option value="447-10:00"<% asp_TZ(447); %>>Pacific/Port_Moresby
<option value="448+10:00"<% asp_TZ(448); %>>Pacific/Rarotonga
<option value="449-10:00"<% asp_TZ(449); %>>Pacific/Saipan
<option value="450+11:00"<% asp_TZ(450); %>>Pacific/Samoa
<option value="451+10:00"<% asp_TZ(451); %>>Pacific/Tahiti
<option value="452-12:00"<% asp_TZ(452); %>>Pacific/Tarawa
<option value="453-13:00"<% asp_TZ(453); %>>Pacific/Tongatapu
<option value="454-10:00"<% asp_TZ(454); %>>Pacific/Truk
<option value="455-12:00"<% asp_TZ(455); %>>Pacific/Wake
<option value="456-12:00"<% asp_TZ(456); %>>Pacific/Wallis
<option value="457-10:00"<% asp_TZ(457); %>>Pacific/Yap
<option value="458-02:00"<% asp_TZ(458); %>>Poland
<option value="459-01:00"<% asp_TZ(459); %>>Portugal
<option value="460-08:00"<% asp_TZ(460); %>>ROC
<option value="461-09:00"<% asp_TZ(461); %>>ROK
<option value="462-08:00"<% asp_TZ(462); %>>Singapore
<option value="463-03:00"<% asp_TZ(463); %>>Turkey
<option value="464-00:00"<% asp_TZ(464); %>>UCT
<option value="465+08:00"<% asp_TZ(465); %>>US/Alaska
<option value="466+09:00"<% asp_TZ(466); %>>US/Aleutian
<option value="467+07:00"<% asp_TZ(467); %>>US/Arizona
<option value="468+05:00"<% asp_TZ(468); %>>US/Central
<option value="469+05:00"<% asp_TZ(469); %>>US/East-Indiana
<option value="470+04:00"<% asp_TZ(470); %>>US/Eastern
<option value="471+10:00"<% asp_TZ(471); %>>US/Hawaii
<option value="472+05:00"<% asp_TZ(472); %>>US/Indiana-Starke
<option value="473+04:00"<% asp_TZ(473); %>>US/Michigan
<option value="474+06:00"<% asp_TZ(474); %>>US/Mountain
<option value="475+07:00"<% asp_TZ(475); %>>US/Pacific
<option value="476+07:00"<% asp_TZ(476); %>>US/Pacific-New
<option value="477+11:00"<% asp_TZ(477); %>>US/Samoa
<option value="478-00:00"<% asp_TZ(478); %>>UTC
<option value="479-00:00"<% asp_TZ(479); %>>Universal
<option value="480-04:00"<% asp_TZ(480); %>>W-SU
<option value="481-01:00"<% asp_TZ(481); %>>WET
<option value="482-00:00"<% asp_TZ(482); %>>Zulu
</select>
</td></tr>
<tr><td bgColor=#aaddff>NTP Server 1</td><td bgColor=#ddeeff><input type=text maxlength=32 size=32 name=ntpServer1 value="<% asp_TZ2("ntpServer1"); %>"</td></tr>
<tr><td bgColor=#aaddff>NTP Server 2</td><td bgColor=#ddeeff><input type=text maxlength=32 size=32 name=ntpServer2 value="<% asp_TZ2("ntpServer2"); %>"</td></tr>
<tr><td bgColor=#aaddff>NTP Server 3</td><td bgColor=#ddeeff><input type=text maxlength=32 size=32 name=ntpServer3 value="<% asp_TZ2("ntpServer3"); %>"</td></tr>
<tr><td bgColor=#aaddff>Time</td><td bgColor=#ddeeff><% asp_TZ2("dateTime"); %></td></tr>
<tr><td colspan=2><input type=submit name="submit" value="Save / Time Sync">&nbsp;&nbsp;&nbsp;&nbsp;<input type=submit name="submit" value="Refresh Time"></td></tr>
</table>

</form>
<form ENCTYPE=multipart/form-data method="post" action="/goform/asp_upload">
<b>Firmware Management</b><BR>
<BR>
<table cellSpacing=1 cellPadding=2 border=0>
	
	<tr>
	<td bgColor=#aaddff>Firmware Version</td>
	<td bgColor=#ddeeff><%asp_flashGetString("fwVer");%></td></tr>

	<tr>
	<td bgColor=#aaddff>Firmware Update</td>
	<td bgColor=#ddeeff><input type=file name="run.bin">
	<input type=submit value="Update">
	</td></tr>
	
	</form>
	
<tr><td colspan=2>&nbsp;</td></tr>
<tr><td colspan=2>
<form ENCTYPE=multipart/form-data method="get" action="/goform/asp_systemDefault" name=funcfrm>
<input type=hidden name="ExeFunc" value="Factory Default">
<input type=button value="Factory Default" onclick="javascript:set_defaults();">
</form>
<form ENCTYPE=multipart/form-data method="get" action="/goform/asp_restart" name=funcfrm2>
<input type=hidden name="ExeFunc" value="System Restart">
<input type=button value="System Restart" onclick="javascript:restart();">
</form>

</td></tr>

</table>

</body>
</html>
