<html>
<head>
<meta http-equiv="Content-Type" content="text/html">
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Cache-Control" CONTENT="no-cache">
<title>Ratio Based QoS</title>

<SCRIPT LANGUAGE="JavaScript">

<% asp_ratio_qos("js_check_new_add_s"); %>

function ratio_queue_select()
{
	var ratio_h = <% asp_ratio_qos("get_ratio_high"); %>;
	var ratio_l = <% asp_ratio_qos("get_ratio_low"); %>;
	var el;
	var max_ratio;
	var i;

	if (ratio_h > ratio_l)
		max_ratio = ratio_h;
	else
		max_ratio = ratio_l;

	for (i = 0 ; i < max_ratio ; i ++)
		ratio_qos.ratio.remove(0);

	if (ratio_h > 0 && ratio_l > 0)
	{
		if (ratio_qos.queue.value == 1)
		{
			for (i = 1 ; i <= ratio_h ; i ++)
			{
				el = document.createElement("option");
				el.text = i;
				el.value = i;
				ratio_qos.ratio.add(el);
			}
		} else
		{
			for (i = 1 ; i <= ratio_l ; i ++)
			{
				el = document.createElement("option");
				el.text = i;
				el.value = i;
				ratio_qos.ratio.add(el);
			}
		}
	} else if (ratio_h == 0)
	{
		for (i = 1 ; i <= ratio_l ; i ++)
		{
			el = document.createElement("option");
			el.text = i;
			el.value = i;
			ratio_qos.ratio.add(el);
		}
	} else if (ratio_l == 0)
	{
		for (i = 1 ; i <= ratio_h ; i ++)
		{
			el = document.createElement("option");
			el.text = i;
			el.value = i;
			ratio_qos.ratio.add(el);
		}
	}
	ratio_qos.ratio.selectedIndex = 0;
}
<% asp_ratio_qos("js_check_new_add_e"); %>

function ratio_qos_enab()
{
	if (ratio_qos.enable.checked==true)
	{
		ratio_qos.uprate.disabled = false;
		ratio_qos.uprateUnit.disabled = false;
		<% asp_ratio_qos("js_check_new_add_s"); %>
		ratio_qos.isSrc.disabled = false;
		ratio_qos.ip0.disabled = false;
		ratio_qos.ip1.disabled = false;
		ratio_qos.ip2.disabled = false;
		ratio_qos.ip3.disabled = false;
		ratio_qos.mask0.disabled = false;
		ratio_qos.mask1.disabled = false;
		ratio_qos.mask2.disabled = false;
		ratio_qos.mask3.disabled = false;
		ratio_qos.protoType.disabled = false;
		ratio_qos.ratio.disabled = false;
		ratio_qos.addNew.disabled = false;
		ratio_qos.deleteSelected.disabled = false;
		ratio_qos.deleteAll.disabled = false;
		ratio_qos_prototypeSel();	// s_port, e_port
		<% asp_ratio_qos("js_check_new_add_e"); %>
		<% asp_ratio_qos("js_check_queue_s"); %>
		ratio_qos.queue.disabled = false;
		<% asp_ratio_qos("js_check_queue_e"); %>
		<% asp_ratio_qos("ratio_qos", "list_esnab"); %>
	} else
	{
		ratio_qos.uprate.disabled = true;
		ratio_qos.uprateUnit.disabled = true;
		<% asp_ratio_qos("js_check_new_add_s"); %>
		ratio_qos.isSrc.disabled = true;
		ratio_qos.ip0.disabled = true;
		ratio_qos.ip1.disabled = true;
		ratio_qos.ip2.disabled = true;
		ratio_qos.ip3.disabled = true;
		ratio_qos.mask0.disabled = true;
		ratio_qos.mask1.disabled = true;
		ratio_qos.mask2.disabled = true;
		ratio_qos.mask3.disabled = true;
		ratio_qos.protoType.disabled = true;
		ratio_qos.ratio.disabled = true;
		ratio_qos.addNew.disabled = true;
		ratio_qos.deleteSelected.disabled = true;
		ratio_qos.deleteAll.disabled = true;
		ratio_qos_prototypeSel();	// s_port, e_port
		<% asp_ratio_qos("js_check_new_add_e"); %>
		<% asp_ratio_qos("js_check_queue_s"); %>
		ratio_qos.queue.disabled = true;
		<% asp_ratio_qos("js_check_queue_e"); %>
		<% asp_ratio_qos("ratio_qos", "list_disab"); %>
	}
}

<% asp_ratio_qos("js_check_new_add_s"); %>
function ratio_qos_prototypeSel()
{
	if ((ratio_qos.protoType.value == 2) || (ratio_qos.protoType.disabled == true))
	{
		ratio_qos.s_port.disabled = true;
		ratio_qos.e_port.disabled = true;
	}else
	{
		ratio_qos.s_port.disabled = false;
		ratio_qos.e_port.disabled = false;
	}
}
<% asp_ratio_qos("js_check_new_add_e"); %>

function ratio_qos_onLoad()
{
	ratio_qos_enab();
	<% asp_ratio_qos("js_check_new_add_s"); %>
	ratio_queue_select();
	<% asp_ratio_qos("js_check_new_add_e"); %>
}

function ratio_qos_submit()
{
		ratio_qos.uprate.disabled = false;
		ratio_qos.uprateUnit.disabled = false;
		<% asp_ratio_qos("js_check_new_add_s"); %>
		ratio_qos.isSrc.disabled = false;
		ratio_qos.ip0.disabled = false;
		ratio_qos.ip1.disabled = false;
		ratio_qos.ip2.disabled = false;
		ratio_qos.ip3.disabled = false;
		ratio_qos.mask0.disabled = false;
		ratio_qos.mask1.disabled = false;
		ratio_qos.mask2.disabled = false;
		ratio_qos.mask3.disabled = false;
		ratio_qos.protoType.disabled = false;
		ratio_qos.s_port.disabled = false;
		ratio_qos.e_port.disabled = false;
		ratio_qos.ratio.disabled = false;
		ratio_qos.addNew.disabled = false;
		ratio_qos.deleteSelected.disabled = false;
		ratio_qos.deleteAll.disabled = false;
		<% asp_ratio_qos("js_check_new_add_e"); %>
		<% asp_ratio_qos("js_check_queue_s"); %>
		ratio_qos.queue.disabled = false;
		<% asp_ratio_qos("js_check_queue_e"); %>
		<% asp_ratio_qos("ratio_qos", "list_esnab"); %>
}

</SCRIPT>
</head>
<body bgcolor=#ffffffff onload="ratio_qos_onLoad();">
<form method="post" name="ratio_qos" id="ratio_qos" action="/goform/asp_setRatio_qos" onsubmit="return ratio_qos_submit()">
<B>Hardware Guaranteed/Spare Class QoS</B><BR>
<BR>

<table cellSpacing=1 cellPadding=1 border=0 bgColor=white>
<tr align=center>
	<td bgColor=#aaddff>
		Enable QoS Control
	</td>
	<td bgColor=#ddeeff>
		<input type="checkbox" name="enable" value=1 onClick="ratio_qos_enab();" <% asp_ratio_qos("enable"); %> >
	</td>
</tr>
<tr align=center>
	<td bgColor=#aaddff>
		WAN side Total Bandwidth
	</td>
	<td bgColor=#ddeeff>
		<input type="text" name="uprate" size=3 maxlength=3 value=<% asp_ratio_qos("upRate"); %> >
		<select name="uprateUnit">
			<option value=0 <%asp_ratio_qos("upRateM");%> >Mbps
			<option value=1 <%asp_ratio_qos("upRateK");%> >Kbps
		</select>
		<input type="submit" name="update" value="Apply">
	</td>
</tr>
</table>
<table cellSpacing=1 cellPadding=2 border=0 bgColor=white>
	<tr bgColor=#aaddff align=center>
		<td>No.</td>
		<td>Src/Dst</td>
		<td>IP&nbsp;Addr</td>
		<td>IP&nbsp;Mask</td>
		<td>Protocol</td>
		<td>Start&nbsp;Port</td>
		<td>End&nbsp;Port</td>
		<td>Bandwidth Allocation Policy</td>
		<td>Percentage(%)</td>
		<td>Select</td>
	</tr>
	<% asp_ratio_qos("list"); %>
	<% asp_ratio_qos("check_new_add_s"); %>
	<tr bgColor=#aaddff><td>New</td>
		<td>
			<select name="isSrc">
				<option value=1 selected>src&nbsp;IP
				<option value=0>dst&nbsp;IP
			</select>
		</td>
		<td>
			<table width=100%><tr><td>
			<input type="text" name="ip0" size=3 value="0" maxlength=3>.</td><td>
			<input type="text" name="ip1" size=3 value="0" maxlength=3>.</td><td>
			<input type="text" name="ip2" size=3 value="0" maxlength=3>.</td><td>
			<input type="text" name="ip3" size=3 value="0" maxlength=3>
			</td></tr></table>
		</td>
		<td>
			<table width=100%><tr><td>
			<input type="text" name="mask0" value="255" size=3 maxlength=3>.</td><td>
			<input type="text" name="mask1" value="255" size=3 maxlength=3>.</td><td>
			<input type="text" name="mask2" value="255" size=3 maxlength=3>.</td><td>
			<input type="text" name="mask3" value="255" size=3 maxlength=3>
			</td></tr></table>
		</td>
		<td>
			<select name="protoType" onchange="ratio_qos_prototypeSel();">
				<option value=0 selected>TCP
				<option value=1>UDP
				<option value=2>IP
			</select>
		</td>
		<td>
			<input type="text" name="s_port" value="0" size=5 maxlength=5>
		</td>
		<td>
			<input type="text" name="e_port" value="65535" size=5 maxlength=5>
		</td>
		<td>
			<% asp_ratio_qos("queue_select", "ratio_queue_select"); %>
		</td>
		<td>
			<select name="ratio">
			</select>
		</td>
		<td></td>
	</tr>
	<% asp_ratio_qos("check_new_add_e"); %>
</table>
<% asp_ratio_qos("check_new_add_s"); %>
<input type="submit" name="addNew" value="Add New">
<% asp_ratio_qos("check_new_add_e"); %>
<input type="submit" name="deleteSelected" value="Delete Selected">
<input type="submit" name="deleteAll" value="Delete All">
</form>
</body>
</html>
