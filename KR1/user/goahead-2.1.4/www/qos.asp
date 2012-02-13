

<html>
<head>
<meta http-equiv="Content-Type" content="text/html">
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Cache-Control" CONTENT="no-cache">
<title>QoS</title>

<SCRIPT LANGUAGE="JavaScript">

function onload_func()
{
	itemSwitch1();
	itemSwitch2();
}

function itemSwitch2()
{
	if (qosform2.ab_switch.checked == false) {
		qosform2.pq_priority_0.disabled=true;
		qosform2.pq_flowctl_0.disabled=true;
		qosform2.pq_inrl_0.disabled=true;
		qosform2.pq_outrl_0.disabled=true;

		qosform2.pq_priority_1.disabled=true;
		qosform2.pq_flowctl_1.disabled=true;
		qosform2.pq_inrl_1.disabled=true;
		qosform2.pq_outrl_1.disabled=true;

		qosform2.pq_priority_2.disabled=true;
		qosform2.pq_flowctl_2.disabled=true;
		qosform2.pq_inrl_2.disabled=true;
		qosform2.pq_outrl_2.disabled=true;

		qosform2.pq_priority_3.disabled=true;
		qosform2.pq_flowctl_3.disabled=true;
		qosform2.pq_inrl_3.disabled=true;
		qosform2.pq_outrl_3.disabled=true;

		qosform2.pq_priority_4.disabled=true;
		qosform2.pq_flowctl_4.disabled=true;
		qosform2.pq_inrl_4.disabled=true;
		qosform2.pq_outrl_4.disabled=true;

	}
	else {
		qosform2.pq_priority_0.disabled=false;
		qosform2.pq_flowctl_0.disabled=false;
		qosform2.pq_inrl_0.disabled=false;
		qosform2.pq_outrl_0.disabled=false;

		qosform2.pq_priority_1.disabled=false;
		qosform2.pq_flowctl_1.disabled=false;
		qosform2.pq_inrl_1.disabled=false;
		qosform2.pq_outrl_1.disabled=false;

		qosform2.pq_priority_2.disabled=false;
		qosform2.pq_flowctl_2.disabled=false;
		qosform2.pq_inrl_2.disabled=false;
		qosform2.pq_outrl_2.disabled=false;

		qosform2.pq_priority_3.disabled=false;
		qosform2.pq_flowctl_3.disabled=false;
		qosform2.pq_inrl_3.disabled=false;
		qosform2.pq_outrl_3.disabled=false;

		qosform2.pq_priority_4.disabled=false;
		qosform2.pq_flowctl_4.disabled=false;
		qosform2.pq_inrl_4.disabled=false;
		qosform2.pq_outrl_4.disabled=false;
	}
}

function itemSwitch1()
{
	if (qosform1.ab_switch.checked == false) {
		qosform1.ftp_switch[0].disabled=true;
		qosform1.ftp_switch[1].disabled=true;
		qosform1.http_switch[0].disabled=true;
		qosform1.http_switch[1].disabled=true;
		qosform1.telnet_switch[0].disabled=true;
		qosform1.telnet_switch[1].disabled=true;
		qosform1.smtp_switch[0].disabled=true;
		qosform1.smtp_switch[1].disabled=true;
		qosform1.pop3_switch[0].disabled=true;
		qosform1.pop3_switch[1].disabled=true;
		qosform1.port_1.disabled=true;
		qosform1.specific_1[0].disabled=true;
		qosform1.specific_1[1].disabled=true;
		qosform1.port_2.disabled=true;
		qosform1.specific_2[0].disabled=true;
		qosform1.specific_2[1].disabled=true;
		qosform1.port_3.disabled=true;
		qosform1.specific_3[0].disabled=true;
		qosform1.specific_3[1].disabled=true;
		
	}
	else {
		qosform1.ftp_switch[0].disabled=false;
		qosform1.ftp_switch[1].disabled=false;
		qosform1.http_switch[0].disabled=false;
		qosform1.http_switch[1].disabled=false;
		qosform1.telnet_switch[0].disabled=false;
		qosform1.telnet_switch[1].disabled=false;
		qosform1.smtp_switch[0].disabled=false;
		qosform1.smtp_switch[1].disabled=false;
		qosform1.pop3_switch[0].disabled=false;
		qosform1.pop3_switch[1].disabled=false;
		qosform1.port_1.disabled=false;
		qosform1.specific_1[0].disabled=false;
		qosform1.specific_1[1].disabled=false;
		qosform1.port_2.disabled=false;
		qosform1.specific_2[0].disabled=false;
		qosform1.specific_2[1].disabled=false;
		qosform1.port_3.disabled=false;
		qosform1.specific_3[0].disabled=false;
		qosform1.specific_3[1].disabled=false;
	}
		

}


</SCRIPT>
</head>

<body bgcolor="#ffffff" text="#000000" onload="onload_func()">
	<%asp_qos();%>
</body>
</html>




