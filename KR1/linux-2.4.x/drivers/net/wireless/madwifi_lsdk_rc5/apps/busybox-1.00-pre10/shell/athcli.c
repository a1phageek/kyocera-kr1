/*
 * This is Atheros CLI enhance to ash. This file is intended to be included in ash.c
 *
 * $Id: //depot/sw/releases/linuxsrc/apps/busybox-1.00-pre10/shell/athcli.c#30 $
 */

#include "athcli.h"

/*
 * CLI parameters
 */
enum {
    CLI_PARAM_ACL = 0,
    CLI_PARAM_AUTH,
    CLI_PARAM_AUTOCHANNEL,
    CLI_PARAM_ABOLT,
    CLI_PARAM_BSS,
    CLI_PARAM_CHANNEL,
    CLI_PARAM_CIPHER,
    CLI_PARAM_CONFIG,
    CLI_PARAM_COUNTRY,
    CLI_PARAM_DUALAP,
    CLI_PARAM_ENCRYPTION,
    CLI_PARAM_FACTORY,
    CLI_PARAM_GROUPKEYUPDATE,
    CLI_PARAM_INTERFACE,
    CLI_PARAM_IPADDR,
    CLI_PARAM_IPMASK,
    CLI_PARAM_VLAN_IPADDR,
    CLI_PARAM_VLAN_IPMASK,
    CLI_PARAM_KEY,
    CLI_PARAM_OPMODE,
    CLI_PARAM_PASSPHRASE,
    CLI_PARAM_POWER,
    CLI_PARAM_PVID,
    CLI_PARAM_RADIUSNAME,
    CLI_PARAM_RADIUSPORT,
    CLI_PARAM_RADIUSSECRET,
    CLI_PARAM_PREAUTHENTICATION,
    CLI_PARAM_RATE,
    CLI_PARAM_REPEATER,
    CLI_PARAM_REPEATER_FLAG,
    CLI_PARAM_SSID,
    CLI_PARAM_STA,
    CLI_PARAM_VAP,
    CLI_PARAM_VLAN,
    CLI_PARAM_WLANSTATE,
    CLI_PARAM_WMODE,
    CLI_PARAM_JS,
    CLI_PARAM_JS_PASSWD,
    CLI_PARAM_KEY_ENTRYMETHOD,
    CLI_MAX_PARAM
};

/*
 * CLI iwpriv parameters for radio. The parameter listed here will appear in the same order
 * of configuration scripts generated later.
 */
enum {
    CLI_PRIV_RADIO_PARAM_XR = 0,
    CLI_PRIV_RADIO_PARAM_AR,
    CLI_PRIV_RADIO_PARAM_COVERAGECLASS,
    CLI_PRIV_RADIO_PARAM_CHANBW,
    CLI_PRIV_RADIO_MAX_PARAM
};

/*
 * CLI iwpriv parameters for BSS. The parameter listed here will appear in the same order
 * of configuration scripts generated later.
 */
enum {
    CLI_PRIV_BSS_PARAM_COMPRESSION = 0,
    CLI_PRIV_BSS_PARAM_FF,
    CLI_PRIV_BSS_PARAM_BURST,
    CLI_PRIV_BSS_PARAM_WMM,
    CLI_PRIV_BSS_PARAM_WDS,
	CLI_PRIV_BSS_PARAM_SSID_SUPPRESS,
    CLI_PRIV_BSS_PARAM_AP_BRIDGE,
	CLI_PRIV_BSS_PARAM_WMM_AGGRMODE,
    CLI_PRIV_BSS_MAX_PARAM
};

/* athcfg.h depends on CLI_MAX_PRIV_PARAM */
#include "athcfg.h"

/* CLI command interface */
static int cli_get_opmode(struct cli_param *param, int param_index, char *op_txt);
static int cli_set_opmode(struct cli_param *param, int param_index, char *op_txt);
static int cli_get_wmode(struct cli_param *param,  int param_index);
static int cli_set_wmode(struct cli_param *param, int param_index, char **argv);
static int cli_get_ssid(struct cli_param *param, int param_index, char *ssid_txt);
static int cli_set_ssid(struct cli_param *param, int param_index, char *ssid_txt);
static int cli_set_factory(struct cli_param *param, int param_index);
static int cli_get_channel(struct cli_param *param, int param_index, char *channel_txt);
static int cli_set_channel(struct cli_param *param, int param_index, char *channel_txt);
static int cli_get_autochansel(struct cli_param *param, int param_index);
static int cli_set_autochansel(struct cli_param *param, int param_index, int enable);
static int cli_get_wlanstate(struct cli_param *param, int param_index);
static int cli_set_wlanstate(struct cli_param *param, int param_index, int enable);
static int cli_get_encryption(struct cli_param *param, int param_index);
static int cli_set_encryption(struct cli_param *param, int param_index, int enable);
static int cli_get_auth(struct cli_param *param, int param_index, char *auth_txt);
static int cli_set_auth(struct cli_param *param, int param_index, char *auth_txt);
static int cli_get_cipher(struct cli_param *param, int param_index, char *cipher_txt);
static int cli_set_cipher(struct cli_param *param, int param_index, char *cipher_txt);
static int cli_get_key(struct cli_param *param, int param_index);
static int cli_set_key(struct cli_param *param, int param_index, char **argv);
static int cli_get_keyentrymethod(struct cli_param *param,int param_index,char *key_method);
static int cli_set_keyentrymethod(struct cli_param *param,int param_index,char *key_method);
static int cli_get_radiusname(struct cli_param *param, int param_index, struct in_addr *addr);
static int cli_set_radiusname(struct cli_param *param, int param_index, struct in_addr *addr);
static int cli_get_radiusport(struct cli_param *param, int param_index);
static int cli_set_radiusport(struct cli_param *param, int param_index, int port);
static int cli_set_radiussecret(struct cli_param *param, int param_index, char *secret);
static int cli_get_preauthentication(struct cli_param *param, int param_index);
static int cli_set_preauthentication(struct cli_param *param, int param_index, int enable);
static int cli_get_rate(struct cli_param *param, int param_index, char *rate_txt);
static int cli_set_rate(struct cli_param *param, int param_index, char *rate_txt);
static int cli_set_passphrase(struct cli_param *param, int param_index, char *passphrase);
static int cli_get_grpkey_interval(struct cli_param *param, int param_index);
static int cli_set_grpkey_interval(struct cli_param *param, int param_index, int interval);
static int cli_get_ipaddr(struct cli_param *param, int param_index, struct in_addr *addr);
static int cli_set_ipaddr(struct cli_param *param, int param_index, struct in_addr *addr);
static int cli_get_ipmask(struct cli_param *param, int param_index, struct in_addr *addr);
static int cli_set_ipmask(struct cli_param *param, int param_index, struct in_addr *addr);
static int cli_get_vlan_ipaddr(struct cli_param *param, int param_index, struct in_addr *addr);
static int cli_set_vlan_ipaddr(struct cli_param *param, int param_index, struct in_addr *addr);
static int cli_get_vlan_ipmask(struct cli_param *param, int param_index, struct in_addr *addr);
static int cli_set_vlan_ipmask(struct cli_param *param, int param_index, struct in_addr *addr);
static int cli_get_config(struct cli_param *param, int param_index);
static int cli_get_param(struct cli_param *param, int param_index);
static int cli_set_param(struct cli_param *param, int param_index, int value);
static int cli_get_sta_info(struct cli_param *param,  int param_index);
static int cli_get_country(struct cli_param *param, int param_index, char *cc_txt);
static int cli_set_country(struct cli_param *param, int param_index, char *cc_txt);
static int cli_set_power(struct cli_param *param, int param_index, char *pwr_txt);
static int cli_get_acl(struct cli_param *param, int param_index, char *acl_txt);
static int cli_set_acl(struct cli_param *param, int param_index, char *acl_txt);

/* vlan interface */
static int cli_get_vlan(struct cli_param *param, int param_index);
static int cli_set_vlan(struct cli_param *param, int param_index, int enable);
static int cli_get_pvid(struct cli_param *param, int param_index);
static int cli_set_pvid(struct cli_param *param, int param_index, int pvid);

/* repeater interface */
static int cli_get_repeater(struct cli_param *param, int param_index);
static int cli_set_repeater(struct cli_param *param, int param_index, char **argv);
static int cli_set_repeater_flag(struct cli_param *param, int param_index, int enable);

/* Jumpstart interface */
static int cli_get_js(struct cli_param *param, int param_index);
static int cli_set_js(struct cli_param *param, int param_index, int enable);
static int cli_get_js_passwd(struct cli_param *param, int param_index, char *password);
static int cli_set_js_passwd(struct cli_param *param, int param_index, char *password);

/* Debug commands support */
static int cli_get_abolt(struct cli_param *param, int param_index);
static int cli_set_abolt(struct cli_param *param, int param_index, int abolt);

/* Hidden helper interfaces */
static int cli_get_interface(struct cli_param *param, int param_index);
static int cli_get_bss(struct cli_param *param, int param_index);
static int cli_set_vap(struct cli_param *param, int param_index, int enable);
static int cli_set_dualap(struct cli_param *param, int param_index, int enable);

/* CLI parameter handling function */
static int cli_param_cmd(int argc, char **argv, int cmd);

/* Other helper functions */
static int cli_run_config_dir(char *config_dir);
static void cli_reset_pointers(void);
static struct vlan_ip * cli_vlan_lookup(int pvid);

/* Debug stuff */
int athcli_debug_flag;
static char clidebug_trace[] = "/tmp/cli_trace";
FILE *cli_debug_file = NULL;
static void cli_trace(const char *fmt, ...);

/*
 * CLI command tables
 */
static struct cli_param cli_param_list[] = {
    { "acl", CLI_F_STOPIF | CLI_F_BSS | CLI_FORMAT_TEXT,
      (cli_handler)cli_get_acl, (cli_handler)cli_set_acl },	/* CLI_PARAM_ACL */
	
    { "authentication", CLI_F_STOPIF | CLI_F_BSS | CLI_FORMAT_TEXT,
      (cli_handler)cli_get_auth, (cli_handler)cli_set_auth }, /* CLI_PARAM_AUTH */

    { "autochannelselect", CLI_F_STOPIF | CLI_F_RADIO | CLI_FORMAT_BOOLEAN,
      (cli_handler)cli_get_autochansel, (cli_handler)cli_set_autochansel }, /* CLI_PARAM_AUTOCHANNEL */
    
    { "abolt", CLI_F_STOPIF | CLI_F_RADIO | CLI_F_BSS | CLI_F_HIDDEN | CLI_FORMAT_INTEGER,
      (cli_handler)cli_get_abolt, (cli_handler)cli_set_abolt }, /* CLI_PARAM_ABOLT */

    { "bss", CLI_F_HIDDEN | CLI_F_RADIO | CLI_FORMAT_SELF, /* CLI_PARAM_BSS */
      (cli_handler)cli_get_bss, NULL },

    { "channel", CLI_F_STOPIF | CLI_F_RADIO | CLI_FORMAT_TEXT,
      (cli_handler)cli_get_channel, (cli_handler)cli_set_channel }, /* CLI_PARAM_CHANNEL */

    { "cipher", CLI_F_STOPIF | CLI_F_BSS | CLI_FORMAT_TEXT,
      (cli_handler)cli_get_cipher, (cli_handler)cli_set_cipher }, /* CLI_PARAM_CIPHER */

    { "config", CLI_F_ZERO_PARAM | CLI_F_RADIO | CLI_F_BSS | CLI_FORMAT_NULL,
      (cli_handler)cli_get_config, NULL }, /* CLI_PARAM_CONFIG */
    
    { "countrycode", CLI_FORMAT_TEXT,
      (cli_handler)cli_get_country, (cli_handler)cli_set_country }, /* CLI_PARAM_COUNTRY */

    { "dualap", CLI_F_HIDDEN | CLI_FORMAT_BOOLEAN,
      NULL, (cli_handler)cli_set_dualap }, /* CLI_PARAM_DUALAP */
    
    { "encryption", CLI_F_STOPIF | CLI_F_BSS | CLI_FORMAT_BOOLEAN,
      (cli_handler)cli_get_encryption, (cli_handler)cli_set_encryption }, /* CLI_PARAM_ENCRYPTION */

    { "factory", CLI_F_STOPIF | CLI_F_ZERO_PARAM | CLI_FORMAT_NULL,
      NULL, (cli_handler)cli_set_factory }, /* CLI_PARAM_FACTORY */
    
    { "groupkeyupdate", CLI_F_BSS | CLI_FORMAT_INTEGER,
      (cli_handler)cli_get_grpkey_interval, (cli_handler)cli_set_grpkey_interval }, /* CLI_PARAM_GROUPKEYUPDATE */

    { "interface", CLI_F_HIDDEN | CLI_F_BSS | CLI_FORMAT_SELF, /* CLI_PARAM_INTERFACE */
      (cli_handler)cli_get_interface, NULL },
    
    { "ipaddr", CLI_FORMAT_IP,
      (cli_handler)cli_get_ipaddr, (cli_handler)cli_set_ipaddr }, /* CLI_PARAM_IPADDR */
    
    { "ipmask", CLI_FORMAT_IP,
      (cli_handler)cli_get_ipmask, (cli_handler)cli_set_ipmask }, /* CLI_PARAM_IPMASK */

    { "vlan_ipaddr", CLI_FORMAT_IP,
      (cli_handler)cli_get_vlan_ipaddr, (cli_handler)cli_set_vlan_ipaddr }, /* CLI_PARAM_VLAN_IPADDR */
    
    { "vlan_ipmask", CLI_FORMAT_IP,
      (cli_handler)cli_get_vlan_ipmask, (cli_handler)cli_set_vlan_ipmask }, /* CLI_PARAM_VLAN_IPMASK */
      
    { "key", CLI_F_STOPIF | CLI_F_BSS | CLI_FORMAT_SELF,
      (cli_handler)cli_get_key, (cli_handler)cli_set_key }, /* CLI_PARAM_KEY */

    { "opmode", CLI_F_STOPIF | CLI_F_BSS | CLI_FORMAT_TEXT,
      (cli_handler)cli_get_opmode, (cli_handler)cli_set_opmode }, /* CLI_PARAM_OPMODE */

    { "passphrase", CLI_F_STOPIF | CLI_F_BSS | CLI_FORMAT_TEXT,
      NULL, (cli_handler)cli_set_passphrase }, /* CLI_PARAM_PASSPHRASE */
    
    { "power", CLI_F_STOPIF | CLI_F_RADIO | CLI_FORMAT_TEXT,
      NULL, (cli_handler)cli_set_power }, /* CLI_PARAM_POWER */
    
    { "pvid", CLI_F_BSS | CLI_FORMAT_INTEGER,
      (cli_handler)cli_get_pvid, (cli_handler)cli_set_pvid }, /* CLI_PARAM_PVID */

    { "radiusname", CLI_FORMAT_IP,
      (cli_handler)cli_get_radiusname, (cli_handler)cli_set_radiusname }, /* CLI_PARAM_RADIUSNAME */
    
    { "radiusport", CLI_FORMAT_INTEGER,
      (cli_handler)cli_get_radiusport, (cli_handler)cli_set_radiusport }, /* CLI_PARAM_RADIUSPORT */

    { "radiussecret", CLI_FORMAT_TEXT,
      NULL, (cli_handler)cli_set_radiussecret }, /* CLI_PARAM_RADIUSSECRET */
    
    { "preauthentication", CLI_F_BSS | CLI_FORMAT_BOOLEAN,
      (cli_handler)cli_get_preauthentication, (cli_handler)cli_set_preauthentication }, /* CLI_PARAM_PREAUTHENTICATION */     
    
    { "rate", CLI_F_STOPIF | CLI_F_BSS | CLI_FORMAT_TEXT,
      (cli_handler)cli_get_rate, (cli_handler)cli_set_rate }, /* CLI_PARAM_RATE */
    
    { "repeater", CLI_F_STOPIF | CLI_F_RADIO | CLI_FORMAT_SELF,
      (cli_handler)cli_get_repeater, (cli_handler)cli_set_repeater }, /* CLI_PRIV_PARAM_REPEATER */

    { "repeater_flag", CLI_F_HIDDEN | CLI_F_RADIO | CLI_FORMAT_BOOLEAN,
      NULL, (cli_handler)cli_set_repeater_flag }, /* CLI_PRIV_PARAM_REPEATER_FLAG */

    { "ssid", CLI_F_STOPIF | CLI_F_BSS | CLI_FORMAT_TEXT,
      (cli_handler)cli_get_ssid, (cli_handler)cli_set_ssid }, /* CLI_PARAM_SSID */
    
    { "sta", CLI_F_ZERO_PARAM | CLI_F_BSS | CLI_F_HIDDEN | CLI_FORMAT_NULL,
      (cli_handler)cli_get_sta_info, NULL }, /* CLI_PARAM_STA */
    
    { "vap", CLI_F_HIDDEN | CLI_FORMAT_BOOLEAN,
      NULL, (cli_handler)cli_set_vap }, /* CLI_PARAM_VAP */
    
    { "vlan", CLI_FORMAT_BOOLEAN,
      (cli_handler)cli_get_vlan, (cli_handler)cli_set_vlan }, /* CLI_PARAM_VLAN */
    
    { "wlanstate", CLI_F_BSS | CLI_FORMAT_BOOLEAN,
      (cli_handler)cli_get_wlanstate, (cli_handler)cli_set_wlanstate }, /* CLI_PARAM_WLANSTATE */

    { "wirelessmode", CLI_F_STOPIF | CLI_F_RADIO | CLI_FORMAT_SELF,
      (cli_handler)cli_get_wmode, (cli_handler)cli_set_wmode }, /* CLI_PRIV_PARAM_WMODE */
      
    { "jumpstart", CLI_F_BSS | CLI_FORMAT_BOOLEAN,
      (cli_handler)cli_get_js, (cli_handler)cli_set_js }, /* CLI_PARAM_JS */
    
    { "js_password", CLI_F_BSS | CLI_FORMAT_TEXT,
      (cli_handler)cli_get_js_passwd, (cli_handler)cli_set_js_passwd }, /* CLI_PARAM_PASSWORD */

    { "keyentrymethod", CLI_F_STOPIF | CLI_F_BSS | CLI_FORMAT_TEXT,
      (cli_handler)cli_get_keyentrymethod, (cli_handler)cli_set_keyentrymethod }, /* CLI_PARAM_KEY_ENTRYMETHOD */
};

static struct cli_param cli_priv_radio_param_list[] = {
    { "xr", CLI_F_STOPIF | CLI_F_RADIO | CLI_FORMAT_BOOLEAN
                | IEEE80211_PARAM_XR,
      (cli_handler)cli_get_param, (cli_handler)cli_set_param }, /* CLI_PRIV_PARAM_XR */

    { "ar", CLI_F_STOPIF | CLI_F_RADIO | CLI_F_HIDDEN | CLI_FORMAT_BOOLEAN
                | IEEE80211_PARAM_AR,
      (cli_handler)cli_get_param, (cli_handler)cli_set_param }, /* CLI_PRIV_PARAM_AR */
    { "coverageclass", CLI_F_STOPIF | CLI_F_RADIO | CLI_FORMAT_INTEGER
                | IEEE80211_PARAM_COVERAGECLASS,
      (cli_handler)cli_get_param, (cli_handler)cli_set_param }, /* CLI_PRIV_PARAM_COVERAGECLASS */
    { "chanbw", CLI_F_STOPIF | CLI_F_RADIO | CLI_FORMAT_INTEGER
                | IEEE80211_PARAM_CHANBW,
      (cli_handler)cli_get_param, (cli_handler)cli_set_param }, /* CLI_PRIV_PARAM_CHANBW */
};

static struct cli_param cli_priv_bss_param_list[] = {
    { "compression", CLI_F_STOPIF | CLI_F_BSS | CLI_F_HIDDEN | CLI_FORMAT_BOOLEAN
                | IEEE80211_PARAM_COMPRESSION,
      (cli_handler)cli_get_param, (cli_handler)cli_set_param }, /* CLI_PRIV_PARAM_COMPRESSION */

    { "ff", CLI_F_STOPIF | CLI_F_BSS | CLI_F_HIDDEN | CLI_FORMAT_BOOLEAN
                | IEEE80211_PARAM_FF,
      (cli_handler)cli_get_param, (cli_handler)cli_set_param }, /* CLI_PRIV_PARAM_FF */

    { "burst", CLI_F_STOPIF | CLI_F_BSS | CLI_F_HIDDEN | CLI_FORMAT_BOOLEAN
                | IEEE80211_PARAM_BURST,
      (cli_handler)cli_get_param, (cli_handler)cli_set_param }, /* CLI_PRIV_BURST */
    
    { "wmm", CLI_F_STOPIF | CLI_F_BSS | CLI_FORMAT_BOOLEAN
                | IEEE80211_PARAM_WMM,
      (cli_handler)cli_get_param, (cli_handler)cli_set_param }, /* CLI_PRIV_PARAM_WMM */

    { "wds", CLI_F_STOPIF | CLI_F_BSS | CLI_FORMAT_BOOLEAN
                | IEEE80211_PARAM_WDS,
      (cli_handler)cli_get_param, (cli_handler)cli_set_param }, /* CLI_PRIV_PARAM_WDS */

    { "ssidsuppress", CLI_F_STOPIF | CLI_F_BSS | CLI_FORMAT_BOOLEAN
                | IEEE80211_PARAM_HIDESSID,
      (cli_handler)cli_get_param, (cli_handler)cli_set_param }, /* CLI_PRIV_PARAM_SSID_SUPPRESS */

    { "intraVF", CLI_F_STOPIF | CLI_F_BSS | CLI_FORMAT_BOOLEAN
                | IEEE80211_PARAM_APBRIDGE,
      (cli_handler)cli_get_param, (cli_handler)cli_set_param }, /* CLI_PRIV_PARAM_AP_BRIDGE */

    { "wmm_aggrmode", CLI_F_STOPIF | CLI_F_BSS | CLI_F_HIDDEN | CLI_FORMAT_BOOLEAN
                | IEEE80211_PARAM_WMM_AGGRMODE,
      (cli_handler)cli_get_param, (cli_handler)cli_set_param }, /* CLI_PRIV_PARAM_WMM_AGGRMODE */
};

struct cli_cmd_table {
    int count;                 /* number of parameters */
    struct cli_param *list;    /* paramter table */
};

#define CLI_NUM_PARAM_TABLE     3

struct cli_cmd_table cli_param_table[CLI_NUM_PARAM_TABLE] = {
    { CLI_MAX_PARAM,                cli_param_list },
    { CLI_PRIV_RADIO_MAX_PARAM,     cli_priv_radio_param_list },
    { CLI_PRIV_BSS_MAX_PARAM,       cli_priv_bss_param_list },
};

/* Global ap_cfg variables */
struct ap_cfg       gApCfg;

struct ap_cfg       *apcfg;
struct ap_radio     *current_radio;
struct ap_bss       *current_bss;
struct vlan_ip      *current_vlan;

int current_radio_index;
int current_bss_index;

#define CLI_INVALID_INDEX   -1

char cli_prompt[16];

int cli_vap_enabled;     /* global flag to indicate if VAP enabled in the driver */
int cli_driver_check;    /* global flag to indicate whether we should
                          * program the driver before acceptting the setting */

int cli_vlan_enabled = 0;    /* global flag to indicat if VLAN is enabled */

/* Some constant text strings */
static char *enable_txt = "Enable";
static char *disable_txt = "Disable";
static char *invalid_txt = "Invalid setting";
static char *best_txt = "best";
static char *wep_txt = "Only one BSS can run static WEP.";

/*
 * CLI command implemenations
 */
static int cli_exec_cmd(char *cmd)
{
    int rc = 0;
    if (cli_driver_check) {
	cli_trace("%s%s", cmd, "\n");
        /* ???: system() will call /bin/sh -c to execute the command.
         * However, we are the shell now! Is it gonna be okay? */
        rc = system(cmd);
    }
    return rc;
}

static void cli_set_prompt(int unit, int bss)
{
    if (bss == CLI_INVALID_INDEX)
        sprintf(cli_prompt, "wlan%d-> ", unit);
    else
        sprintf(cli_prompt, "wlan[%d,%d]-> ", unit, bss);
}

static int cli_iface_updown(char *iface, int up)
{
    char cmdbuf[64];
    sprintf(cmdbuf, "ifconfig %s %s", iface, (up ? "up" : "down"));
    return cli_exec_cmd(cmdbuf);
}

static int cli_get_param(struct cli_param *priv, int param_index)
{
    int sub_ioctl = CLI_PARAM_SUBIOCTL(priv->index);
    int value;

    if (priv->index & CLI_F_PROBE_ONLY) {
        char cmdbuf[64];
        sprintf(cmdbuf, "iwpriv %s getparam %d",
                current_radio->iface, sub_ioctl);
        return cli_exec_cmd(cmdbuf);
    }
    else {
        /* Just return the value cached in memory */
        if (priv->index & CLI_F_RADIO)
            value = current_radio->priv_params[param_index];
        else
            value = current_bss->priv_params[param_index];

        return value;
    }
}

static int cli_set_param(struct cli_param *priv, int param_index, int value)
{
    int rc;
    int sub_ioctl = CLI_PARAM_SUBIOCTL(priv->index);
	char *iface = (priv->index & CLI_F_BSS) ?
			current_bss->iface : current_radio->iface;
    char cmdbuf[64];

    sprintf(cmdbuf, "iwpriv %s setparam %d %d",
            iface, sub_ioctl, value);
    rc = cli_exec_cmd(cmdbuf);

    if (rc == 0) {
        if (priv->index & CLI_F_RADIO)
            current_radio->priv_params[param_index] = value;
        else
            current_bss->priv_params[param_index] = value;
    }
    return rc;
}

static int cli_set_default(void)
{
    int i, j;
    struct ap_radio *radio;
    struct ap_bss *bss;
    
    memset(apcfg, 0, sizeof(struct ap_cfg));

    cli_vlan_enabled = 0;
    apcfg->num_radio = DEFAULT_RADIO_NUM;
    inet_pton(AF_INET, DEFAULT_IP_ADDR, &apcfg->ipaddr);
    inet_pton(AF_INET, DEFAULT_IP_MASK, &apcfg->ipmask);
    inet_pton(AF_INET, DEFAULT_RADIUS_NAME, &apcfg->radius_name);
    strcpy(apcfg->radius_secret, DEFAULT_RADIUS_SECRET);
    apcfg->radius_port = DEFAULT_RADIUS_PORT;

    for (i = 0; i < MAX_RADIO_NUM; i++) {
        radio = &apcfg->radio_array[i];

        radio->iface = radio->bss_array[0].iface;
        sprintf(radio->vapctl_iface, "%s%d", DEFAULT_VAPCTL_IFACE, i);
        radio->num_bss = 0;
        radio->wep_index = CLI_INVALID_INDEX;
        radio->power = CLI_AUTO_POWER;

        /* some radio defaults */
        radio->flags = (CLI_RADIO_F_AUTO_CHANNEL |
                        CLI_RADIO_F_AUTO_RATE);

        for (j = 0; j < MAX_BSS_NUM; j++) {
            bss = &radio->bss_array[j];
            bss->flags |= CLI_BSS_F_FREE;

            bss->rate = CLI_BEST_RATE;
        
            bss->defaultKey = CLI_INVALID_INDEX;
            bss->keytable = radio->keytable;
            bss->wlanstate = 1;
            bss->js_enabled = 0;

            bss->acl_mode = IEEE80211_MACCMD_DETACH;
            bss->acl_table = NULL;
            
            sprintf(bss->iface, "%s%d", DEFAULT_BSS_IFACE, i);
            strcpy(bss->ssid, DEFAULT_SSID);
            bss->groupKeyUpdateInterval = DEFAULT_GROUPKEY_UPDATE;
            bss->priv_params[CLI_PRIV_BSS_PARAM_AP_BRIDGE] = 1;
            memset(bss->passphrase, 0, sizeof(bss->passphrase));
            memset(bss->js_passwd, 0, sizeof(bss->js_passwd));
        }
    }

    return 0;
}

static int cli_get_wlanstate(struct cli_param *param, int param_index)
{
    return current_bss->wlanstate;
}

static int cli_set_wlanstate(struct cli_param *param, int param_index, int enable)
{
    /* wlanstate means the BSS should be up or down on reboot or restart,
     * not the current up or down status of interface */
    current_bss->wlanstate = enable;

    if (!enable) {
        cli_iface_updown(current_bss->iface, 0);
    }
    
    return 0;
}

static struct vlan_ip * cli_vlan_lookup(int pvid) {
    int found = 0;
    int idx = 0;
    int max_vlan_num = apcfg->num_radio * MAX_VLAN_NUM;
    struct vlan_ip *avail_vlan = NULL;
    struct vlan_ip *cur_vlan = NULL;
   
    while (idx < max_vlan_num && !found) {
        cur_vlan = &apcfg->vlan[idx];
        if (cur_vlan->vlan_tag == pvid) {
           /* found an existing vlan */
           found = 1;
        } else {
           if (!avail_vlan && cur_vlan->vlan_tag == 0) {
               avail_vlan = cur_vlan;
           }
           idx++;
        }
    }
    /* Set default values */
    if (idx == max_vlan_num && avail_vlan) {
        inet_pton(AF_INET, DEFAULT_IP_ADDR, &avail_vlan->ipaddr);
        inet_pton(AF_INET, DEFAULT_IP_MASK, &avail_vlan->ipmask);
        inet_pton(AF_INET, DEFAULT_RADIUS_NAME, &avail_vlan->radius_name);
        strcpy(avail_vlan->radius_secret, DEFAULT_RADIUS_SECRET);
        avail_vlan->radius_port = DEFAULT_RADIUS_PORT;
    }
    return ((idx == max_vlan_num) ? avail_vlan : cur_vlan);
}

struct cli_txt {
    char *cli_name, *iw_name;
};

/* operation modes */
enum {
    CLI_OPMODE_STA = 0,
    CLI_OPMODE_AP,
    CLI_OPMODE_ADHOC,
    CLI_OPMODE_REPEATER,
    CLI_MAX_OPMODE
};

static struct cli_txt cli_opmode_list[CLI_MAX_OPMODE] = {
    { "STA",        "Managed" },
    { "AP",         "Master" },
    { "AHDOC",      "Ad-Hoc" },
    { "REPEATER",   "Repeator"}
};

static int cli_get_opmode(struct cli_param *param, int param_index, char *op_txt)
{
    strcpy(op_txt, cli_opmode_list[current_bss->opmode].cli_name);
    return 0;
}

static int cli_set_opmode(struct cli_param *param, int param_index, char *op_txt)
{
    int opmode;

    if (cli_vap_enabled) {
        /* Acutally we don't need to set opmode in VAP mode. */
        if (strcasecmp(cli_opmode_list[current_bss->opmode].cli_name, op_txt) == 0) {
            return 0;
        }
        goto invalid;
    }
    
    for (opmode = 0; opmode < CLI_MAX_OPMODE; opmode++) {
        if (strncasecmp(cli_opmode_list[opmode].cli_name, op_txt,
                        strlen(cli_opmode_list[opmode].cli_name)) == 0) {
            /* try to set the opmode first */
            int rc;
            char cmdbuf[64];

            sprintf(cmdbuf, "iwconfig %s mode %s\n",
                    current_bss->iface, cli_opmode_list[opmode].iw_name);
            rc = cli_exec_cmd(cmdbuf);

            if (rc == 0)
                current_bss->opmode = opmode;
            return rc;
        }
    }

invalid:    
    printf("%s: %s\n", param->name, invalid_txt);
    return 1;
}

/* turbo */
static int cli_set_turbo(int enable)
{
    char cmdbuf[64];
	
    sprintf(cmdbuf, "iwpriv %s turbo %d\n", current_radio->iface, enable);
    return cli_exec_cmd(cmdbuf);
}

/* wireless modes */
enum {
    CLI_WMODE_AUTO = 0,
    CLI_WMODE_11A,
    CLI_WMODE_11B,
    CLI_WMODE_11G,
    CLI_WMODE_11A_DT,
    CLI_WMODE_11A_ST,
    CLI_WMODE_11G_DT,
    CLI_MAX_WMODE
};

static struct cli_txt cli_wmode_list[CLI_MAX_WMODE] = {
    { "auto",           "0" },
    { "11a",            "11a" },
    { "11b",            "11b" },
    { "11g",            "11g" },
    { "turbo dynamic",  "11a" },
    { "turbo static",   "11aST" },
    { "108g dynamic",   "11g" }
};

static int cli_get_wmode(struct cli_param *param, int param_index)
{
    /* Note: this is a SELF format, so we have to print it here */
    printf("Wireless LAN Mode: %s\n", cli_wmode_list[current_radio->wmode].cli_name);
    return 0;
}

static int cli_set_wmode_value(int wmode)
{
    int rc;
    char cmdbuf[64];

    sprintf(cmdbuf, "iwpriv %s mode %s\n",
            current_radio->iface, cli_wmode_list[wmode].iw_name);
    rc = cli_exec_cmd(cmdbuf);

	if (rc == 0) {
		if (wmode == CLI_WMODE_11A_DT || wmode == CLI_WMODE_11G_DT)
			rc = cli_set_turbo(1);
		else if (wmode != CLI_WMODE_11A_ST)
			rc = cli_set_turbo(0);
	}
	
    if (rc == 0)
        current_radio->wmode = wmode;

    return rc;
}

static int cli_set_wmode(struct cli_param *priv, int param_index, char **argv)
{
    int wmode;
    int argc;
	int rc;

    /* get the number of arguments */
    for (argc = 0; argv[argc]; argc++);
    
    for (wmode = 0; wmode < CLI_MAX_WMODE; wmode++) {
        if (strncasecmp(argv[1], cli_wmode_list[wmode].cli_name,
                        strlen(argv[1])) == 0) {
            if (wmode > CLI_WMODE_11G) {
                /* additional turbo setting */
                if (argc != 3 || !argv[2]) {
                    break;
                }

                if (wmode == CLI_WMODE_11G_DT &&
                    strcasecmp(argv[2], "dynamic") != 0)
                    break;

                if (strcasecmp(argv[1], "turbo") == 0) {
                    if (strcasecmp(argv[2], "dynamic") == 0)
                        wmode = CLI_WMODE_11A_DT;
                    else if(strcasecmp(argv[2], "static") == 0)
                        wmode = CLI_WMODE_11A_ST;
                    else
                        break;
                }
            }

            /* application rules for wireless mode */
            if (apcfg->num_radio == 2) {
                if (current_radio_index == 0) {
                    /* Only 11A modes are allowed on Radio 0 */
                    if (wmode != CLI_WMODE_11A &&
                        wmode != CLI_WMODE_11A_ST &&
                        wmode != CLI_WMODE_11A_DT)
                        break;
                }
                else {
                    /* 11A modes are not allowed on Radio 1 */
                    if (wmode != CLI_WMODE_11B &&
                        wmode != CLI_WMODE_11G &&
                        wmode != CLI_WMODE_11G_DT)
                        break;
                }
            }

			/* set the wireless mode */
			rc = cli_set_wmode_value(wmode);

			/* automatically turn on AR for dynamic turbo */
			if (rc == 0 && (wmode == CLI_WMODE_11G_DT ||
							wmode == CLI_WMODE_11A_DT)) {

				rc = cli_set_param(&cli_priv_radio_param_list[CLI_PRIV_RADIO_PARAM_AR],
								   CLI_PRIV_RADIO_PARAM_AR, 1);
			}
            return rc;
        }
    }

    printf("%s: %s\n", priv->name, invalid_txt);
    return 1;
}

#define is_compression_on(bss)      ((bss)->priv_params[CLI_PRIV_BSS_PARAM_COMPRESSION])
#define is_ff_on(bss)               ((bss)->priv_params[CLI_PRIV_BSS_PARAM_FF])
#define is_burst_on(bss)            ((bss)->priv_params[CLI_PRIV_BSS_PARAM_BURST])
#define is_wmm_on(bss)              ((bss)->priv_params[CLI_PRIV_BSS_PARAM_WMM])
#define is_wmm_aggrmode_on(bss)     ((bss)->priv_params[CLI_PRIV_BSS_PARAM_WMM_AGGRMODE])

#define is_xr_on(radio)             ((radio)->priv_params[CLI_PRIV_RADIO_PARAM_XR])
#define is_ar_on(radio)             ((radio)->priv_params[CLI_PRIV_RADIO_PARAM_AR])

static int cli_get_abolt_value(void)
{
    /* There is no abolt stored. We have to assemble an abolt */
    int abolt = 0;

    switch (current_radio->wmode) {
        case CLI_WMODE_11A_DT:
        case CLI_WMODE_11G_DT:
            abolt |= (ABOLT_TURBO | ABOLT_TURBO_PRIME);
            break;

        case CLI_WMODE_11A_ST:
            abolt |= ABOLT_TURBO;
            break;
    }

    if (is_compression_on(current_bss))
        abolt |= ABOLT_COMPRESSION;

    if (is_ff_on(current_bss))
        abolt |= ABOLT_FAST_FRAME;

    if (is_burst_on(current_bss))
        abolt |= ABOLT_BURST;

    if (is_wmm_aggrmode_on(current_bss))
        abolt |= ABOLT_WME_ELE;

    if (is_xr_on(current_radio))
        abolt |= ABOLT_XR;

    if (is_ar_on(current_radio))
        abolt |= ABOLT_FRIENDLY_TURBO;

    return abolt;
}

static int cli_set_abolt_value(int abolt)
{
    int rc = 0;
    int enable;
    
    /* Wireless Mode */
    if (rc == 0) {
        int turbo = (abolt & ABOLT_TURBO) ? 1 : 0;
        int dynamic = (abolt & ABOLT_TURBO_PRIME) ? 1 : 0;
        int wmode = current_radio->wmode;

        switch (current_radio->wmode) {
            case CLI_WMODE_11A:
            case CLI_WMODE_11A_DT:
            case CLI_WMODE_11A_ST:
                if (!turbo)
                    wmode = CLI_WMODE_11A;
                else
                    wmode = dynamic ? CLI_WMODE_11A_DT : CLI_WMODE_11A_ST;
                break;

            case CLI_WMODE_11G:
            case CLI_WMODE_11G_DT:
                if (!turbo)
                    wmode = CLI_WMODE_11G;
                else if (dynamic)
                    wmode = CLI_WMODE_11G_DT;
                else {
                    printf("Static turbo G not supported.\n");
                    return 1;
                }
                break;

            default:
                printf("Incompatible wireless mode\n");
                return 1;
        }

        rc = cli_set_wmode_value(wmode);
    }
    
    /* Compression */
    if (rc == 0) {
        enable = (abolt & ABOLT_COMPRESSION) ? 1 : 0;
        rc = cli_set_param(&cli_priv_bss_param_list[CLI_PRIV_BSS_PARAM_COMPRESSION],
                           CLI_PRIV_BSS_PARAM_COMPRESSION, enable);
    }
    /* Fast Frame */
    if (rc == 0) {
        enable = (abolt & ABOLT_FAST_FRAME) ? 1 : 0;
        rc = cli_set_param(&cli_priv_bss_param_list[CLI_PRIV_BSS_PARAM_FF],
                           CLI_PRIV_BSS_PARAM_FF, enable);
    }

    /* Bursting */
    if (rc == 0) {
        enable = (abolt & ABOLT_BURST) ? 1 : 0;
        rc = cli_set_param(&cli_priv_bss_param_list[CLI_PRIV_BSS_PARAM_BURST],
                           CLI_PRIV_BSS_PARAM_BURST, enable);
    }

    /* WMM */
    if (rc == 0) {
        enable = (abolt & ABOLT_WME_ELE) ? 1 : 0;
        rc = cli_set_param(&cli_priv_bss_param_list[CLI_PRIV_BSS_PARAM_WMM_AGGRMODE],
                           CLI_PRIV_BSS_PARAM_WMM_AGGRMODE, enable);
    }

    /* XR */
    if (rc == 0) {
        enable = (abolt & ABOLT_XR) ? 1 : 0;
        rc = cli_set_param(&cli_priv_radio_param_list[CLI_PRIV_RADIO_PARAM_XR],
                           CLI_PRIV_RADIO_PARAM_XR, enable);
    }

    /* AR */
    if (rc == 0) {
        enable = (abolt & ABOLT_FRIENDLY_TURBO) ? 1 : 0;
        rc = cli_set_param(&cli_priv_radio_param_list[CLI_PRIV_RADIO_PARAM_AR],
                           CLI_PRIV_RADIO_PARAM_AR, enable);
    }
    
    return rc;
}

static int cli_get_abolt(struct cli_param *param, int param_index)
{
    return cli_get_abolt_value();
}

static int cli_set_abolt(struct cli_param *param, int param_index, int abolt)
{
    return cli_set_abolt_value(abolt);
}

/* auto channel select */
static int cli_get_autochansel(struct cli_param *param, int param_index)
{
    return (current_radio->flags & CLI_RADIO_F_AUTO_CHANNEL);
}

static int cli_set_autochansel(struct cli_param *param, int param_index, int enable)
{
	int rc = 0;
	
    if (enable) {
		char cmdbuf[64];
		sprintf(cmdbuf, "iwconfig %s channel 0", current_radio->iface);
		rc = cli_exec_cmd(cmdbuf);

		if (rc == 0)
			current_radio->flags |= CLI_RADIO_F_AUTO_CHANNEL;
	}
    else
        current_radio->flags &= ~CLI_RADIO_F_AUTO_CHANNEL;

	return rc;
}

/* channel */
static int cli_get_channel(struct cli_param *param, int param_index, char *channel_txt)
{
    long channel = atol(current_radio->channel);

    if (current_radio->flags & CLI_RADIO_F_AUTO_CHANNEL) {
        strcpy(channel_txt, "Auto");
        return 0;
    }
    
    if (channel > 1000 || channel == 0) {
        /* display as frequency */
        sprintf(channel_txt, "%s HZ", current_radio->channel);
    }
    else {
        /* display as channel */
        sprintf(channel_txt, "IEEE %s", current_radio->channel);
    }
    return 0;
}

static int cli_set_channel(struct cli_param *param, int param_index, char *channel_txt)
{
    int rc;
    char cmdbuf[128];

    sprintf(cmdbuf, "iwconfig %s channel %s", current_radio->iface, channel_txt);
    rc = cli_exec_cmd(cmdbuf);
    
    if (rc == 0) {
        memset(current_radio->channel, 0, sizeof(current_radio->channel));
        strncpy(current_radio->channel, channel_txt, MAX_CHANNEL_LENGTH);

        /* turn off auto channel select */
        current_radio->flags &= ~CLI_RADIO_F_AUTO_CHANNEL;
    };
    return rc;
}

static int cli_get_rate(struct cli_param *param, int param_index, char *rate_txt)
{
    if (current_bss->rate == CLI_BEST_RATE) {
        strcpy(rate_txt, best_txt);
    }
    else {
        sprintf(rate_txt, "%d Mbps", current_bss->rate);
    }
    return 0;
}

static int cli_set_rate(struct cli_param *param, int param_index, char *rate_txt)
{
    int rc = 0;
    int rate;
    char rate_buf[16];
    char cmdbuf[64];
    
    if (strcasecmp(rate_txt, best_txt) == 0) {
        rate = CLI_BEST_RATE;
        strcpy(rate_buf, "auto");
    }
    else {
        rate = atoi(rate_txt);
        if (rate == 0)
            goto bad;

        sprintf(rate_buf, "%dM", rate);
    }

    sprintf(cmdbuf, "iwconfig %s rate %s", current_bss->iface, rate_buf);
    rc = cli_exec_cmd(cmdbuf);

    if (rc == 0) {
        current_bss->rate = rate;
    }
    
    return rc;

bad:
    printf("%s: %s\n", param->name, invalid_txt);
    return 1;
}

/* power */
static int cli_set_power(struct cli_param *param, int param_index, char *pwr_txt)
{
    int rc = 0;
    int power;
    char pwr_buf[16];
    char cmdbuf[64];
    
    if (strcasecmp(pwr_txt, "auto") == 0) {
        power = CLI_AUTO_POWER;
        strcpy(pwr_buf, "auto");
    }
    else {
        if (strcasecmp(pwr_txt, "full") == 0)
            power = CLI_MAX_POWER;
        else if (strcasecmp(pwr_txt, "min") == 0)
            power = CLI_MIN_POWER;
        else {
            power = atoi(pwr_txt);
            if (!power)
                goto bad;
        }
        sprintf(pwr_buf, "%d", power);
    }

    sprintf(cmdbuf, "iwconfig %s txpower %s", current_radio->iface, pwr_buf);
    rc = cli_exec_cmd(cmdbuf);

    if (rc == 0) {
        current_radio->power = power;
    }
    
    return rc;

bad:
    printf("%s: %s\n", param->name, invalid_txt);
    return 1;
}

/* ssid */
static int cli_get_ssid(struct cli_param *param, int param_index, char *ssid_txt)
{
    strcpy(ssid_txt, current_bss->ssid);
    return 0;
}

static int cli_set_ssid(struct cli_param *param, int param_index, char *ssid_txt)
{
    int rc = 0;
    char ssid[MAX_SSID+1];
    char cmdbuf[128];

    memset(ssid, 0, MAX_SSID+1);
    strncpy(ssid, ssid_txt, MAX_SSID); /* this will truncate ssid */

    sprintf(cmdbuf, "iwconfig %s essid \"%s\"", current_bss->iface, ssid);
    rc = cli_exec_cmd(cmdbuf);
    
    if (rc == 0) {
        strcpy(current_bss->ssid, ssid);
    }
    return rc;
}

static int cli_get_sta_info(struct cli_param *param,  int param_index)
{
    char cmdbuf[128];

    sprintf(cmdbuf, "wlanconfig %s list sta", current_bss->iface);
    return cli_exec_cmd(cmdbuf);
}

static int cli_get_encryption(struct cli_param *param, int param_index)
{
    return (current_bss->flags & CLI_BSS_F_SECURE);
}

static int cli_set_encryption(struct cli_param *param, int param_index, int enable)
{
    if (enable) {
        current_bss->flags |= CLI_BSS_F_SECURE;
    }
    else {
        current_bss->flags &= ~CLI_BSS_F_SECURE;
        if (current_radio->wep_index == current_bss_index) {
            /* reset static wep index */
            current_radio->wep_index = CLI_INVALID_INDEX;
        }
    }
    return 0;
}

static char *cli_acl_cmd[IEEE80211_MACCMD_MAX] = {
	"Open", "Allow", "Deny", "flush", "Disable"
};

static int cli_get_acl(struct cli_param *param, int param_index, char *acl_txt)
{
	char *buf = acl_txt;
	int slot;
	struct acl_entry *entry;
	
	/* print the mode first */
	buf += sprintf(buf, "%s\n", cli_acl_cmd[current_bss->acl_mode]);

	/* print the list */
	if (current_bss->acl_table) {
		buf += sprintf(buf, "acl table:\n");
		for (slot = 0; slot < current_bss->acl_size; slot++) {
			entry = &current_bss->acl_table[slot];
			if (entry->used)
				buf += sprintf(buf, "%s\n", entry->macaddr);
		}
	}

	/* trim the last newline character */
	if (*(--buf) == '\n')
		*buf = '\0';
	return 0;
}

static int cli_set_acl(struct cli_param *param, int param_index, char *acl_txt)
{
	int acl_cmd;
	for (acl_cmd = 0; acl_cmd < IEEE80211_MACCMD_MAX; acl_cmd++) {
		if (strcasecmp(acl_txt, cli_acl_cmd[acl_cmd]) == 0) {
			int rc;
			char cmdbuf[128];

			sprintf(cmdbuf, "iwpriv %s maccmd %d", current_bss->iface, acl_cmd);
			rc = cli_exec_cmd(cmdbuf);

			if (rc == 0) {
				if (acl_cmd == IEEE80211_MACCMD_FLUSH) {
					/* flush CLI's internal list */
					if (current_bss->acl_table)
						memset(current_bss->acl_table, 0,
							   current_bss->acl_size * sizeof(struct acl_entry));
				}	
				else
					current_bss->acl_mode = acl_cmd;
				return 0;
			}
		}
	}

	printf("%s: %s\n", param->name, invalid_txt);
    return 1;
}

static int cli_acl_table_realloc(int newsize)
{
	struct acl_entry *newtable;
	
	if (current_bss->acl_size == 0 ||
		current_bss->acl_table == NULL) {
		current_bss->acl_table = malloc(newsize * sizeof(struct acl_entry));
	}
	else if (newsize > current_bss->acl_size) {
		current_bss->acl_table = realloc(current_bss->acl_table,
										 newsize * sizeof(struct acl_entry));
	}
	else
		return 0;

	if (current_bss->acl_table) {
		/* initialize newly allocated table */
		newtable = current_bss->acl_table + current_bss->acl_size;
		memset(newtable, 0, (newsize - current_bss->acl_size) * sizeof(struct acl_entry));

		current_bss->acl_size = newsize;
		return 0;
	}

	printf("Not enough memory\n");
	return 1;
}

static struct acl_entry *cli_acl_find_free_entry(void)
{
	int slot;
	struct acl_entry *entry;
	
	for (slot = 0; slot < current_bss->acl_size; slot++) {
		entry = &current_bss->acl_table[slot];
		if (!entry->used)
			return entry;
	}
	return NULL;
}

static struct acl_entry *cli_acl_find_entry(char *macaddr)
{
	int slot;
	struct acl_entry *entry;
	
	for (slot = 0; slot < current_bss->acl_size; slot++) {
		entry = &current_bss->acl_table[slot];
		if (entry->used && strcmp(entry->macaddr, macaddr) == 0)
			return entry;
	}
	return NULL;
}

static int cli_add_acl(char *macaddr)
{
	struct acl_entry *entry;
	int rc;
	char cmdbuf[128];
	
	if (current_bss->acl_table == NULL &&
		cli_acl_table_realloc(CLI_INITIAL_ACL_TABLE_SIZE)) {
		return 1;
	}

	/* try to find this mac address first */
	entry = cli_acl_find_entry(macaddr);
	if (entry) {
		printf("Address %s has already been added to ACL\n", macaddr);
		return 0;
	}

	/* add a new entry */
	entry = cli_acl_find_free_entry();
	if (entry == NULL) {
		/* increase the ACL table */
		if (cli_acl_table_realloc(current_bss->acl_size + CLI_ACL_TABLE_SIZE_INCREMENT))
			return 1;

		/* try again */
		entry = cli_acl_find_free_entry();
		if (entry == NULL)
			return 1;
	}

	/* Now add the address to the driver */
	sprintf(cmdbuf, "iwpriv %s addmac %s", current_bss->iface, macaddr);
	rc = cli_exec_cmd(cmdbuf);
	if (rc == 0) {
		entry->used = 1;
		memset(entry->macaddr, 0, 20);
		strcpy(entry->macaddr, macaddr);
	}
	
	return rc;
}

static int cli_del_acl(char *macaddr)
{
	struct acl_entry *entry;
	int rc;
	char cmdbuf[128];
	
	if (current_bss->acl_table == NULL)
		return 0;

	entry = cli_acl_find_entry(macaddr);
	if (entry == NULL) {
		printf("Address %s is not in the ACL\n", macaddr);
		return 1;
	}

	/* Now delete it from the driver */
	sprintf(cmdbuf, "iwpriv %s delmac %s", current_bss->iface, macaddr);
	rc = cli_exec_cmd(cmdbuf);
	if (rc == 0)
		entry->used = 0;

	return rc;
}

enum {
    CLI_AUTH_OPEN = 0,
    CLI_AUTH_SHARED,
    CLI_AUTH_AUTO,
    CLI_AUTH_8021X,
    CLI_AUTH_WPA,
    CLI_AUTH_WPAPSK,
    CLI_AUTH_WPA2,
    CLI_AUTH_WPA2PSK,
    CLI_AUTH_WPA_AUTO,
    CLI_AUTH_WPA_AUTO_PSK,
    CLI_MAX_AUTH
};

static char *cli_auth_list[CLI_MAX_AUTH] = {
    "open", "shared", "auto", "802.1x",
    "wpa", "wpa-psk", "wpa2", "wpa2-psk", "wpa-auto", "wpa-auto-psk"
};

static int cli_get_auth(struct cli_param *param, int param_index, char *auth_txt)
{
    strcpy(auth_txt, cli_auth_list[current_bss->authType]);
    return 0;
}

static int cli_set_auth(struct cli_param *param, int param_index, char *auth_txt)
{
    int auth;
    for (auth = 0; auth < CLI_MAX_AUTH; auth++) {
        if (strcasecmp(auth_txt, cli_auth_list[auth]) == 0) {
	    /* Only one VAP can do static wep, with the exception of WDS/Repeater */
            if (auth == CLI_AUTH_SHARED || auth == CLI_AUTH_AUTO) {      
                if ((current_radio->wep_index == CLI_INVALID_INDEX) ||
                    (current_radio->flags & CLI_RADIO_F_REPEATER)) {
                    /* set WEP flag */
                    current_radio->wep_index = current_bss_index;
                }
                else if (current_radio->wep_index != current_bss_index) {
                    /* another VAP is using static wep */
                    printf("%s\n", wep_txt);
                    break;
                }
            }
            
            if (auth == CLI_AUTH_OPEN || auth == CLI_AUTH_SHARED || auth == CLI_AUTH_AUTO) {
                /* If it's open or shared key, then we need to actually set it now */
                int rc;
                char cmdbuf[64];
		int mapped_auth;
		mapped_auth = (auth == CLI_AUTH_AUTO ? 4 : (auth + 1));
                sprintf(cmdbuf, "iwpriv %s authmode %d", current_bss->iface, mapped_auth);
                rc = cli_exec_cmd(cmdbuf);
                if (rc)
                    break;
            }

            if (auth >= CLI_AUTH_8021X) {
                /* turn off static wep */
                current_bss->defaultKey = CLI_INVALID_INDEX;
                if (current_radio->wep_index == current_bss_index)
                    current_radio->wep_index = CLI_INVALID_INDEX;
            }
            
            current_bss->authType = auth;
            return 0;
        }
    }    
    printf("%s: %s\n", param->name, invalid_txt);
    return 1;
}

enum {
    CLI_CIPHER_AUTO = 0,
    CLI_CIPHER_WEP,
    CLI_CIPHER_TKIP,
    CLI_CIPHER_AES,
    CLI_MAX_CIPHER
};

static struct cli_txt cli_cipher_list[CLI_MAX_CIPHER] = {
    { "auto",   "CCMP TKIP" },
    { "wep",    "WEP" },
    { "tkip",   "TKIP" },
    { "aes",    "CCMP" }
};

static int cli_get_cipher(struct cli_param *param, int param_index, char *cipher_txt)
{
    strcpy(cipher_txt, cli_cipher_list[current_bss->cipher].cli_name);
    return 0;
}

static int cli_set_cipher(struct cli_param *param, int param_index, char *cipher_txt)
{
    int cipher;
    for (cipher = 0; cipher < CLI_MAX_CIPHER; cipher++) {
        if (strcasecmp(cipher_txt, cli_cipher_list[cipher].cli_name) == 0) {
            current_bss->cipher = cipher;
            return 0;
        }
    }
    printf("%s: %s\n", param->name, invalid_txt);
    return 1;
}
static int cli_get_keyentrymethod(struct cli_param *param,int param_index,char *key_method)
{
    switch (current_bss->keyEntryMethod) {
        case KEY_ENTRY_METHOD_HEX:
            strcpy(key_method,"hex");
            break;
        case KEY_ENTRY_METHOD_ASCII:
            strcpy(key_method,"ascii");
            break;
        default :
            return 1;
    }
    return 0;
}
static int cli_set_keyentrymethod(struct cli_param *param,int param_index,char *key_method)
{
    if (strcmp(key_method,"ascii")==0) {
        current_bss->keyEntryMethod = KEY_ENTRY_METHOD_ASCII;
        return 0;
    } else if (strcmp(key_method,"hex")==0) {
        current_bss->keyEntryMethod = KEY_ENTRY_METHOD_HEX;
        return 0;
    }

    printf("Invalid setting:method should be ascii or hex ..\n");
    return 1;
}
static int check_hex_string(char *p)
{
    int i;
    char c;
    for (i = 0; i < strlen(p); i++) {
        c = p[i];
        if ((c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'f') ||
            (c >= 'A' && c <= 'F'))
            continue;

        /* if reach here, then non hex string */
        return -1;
    }
    return 1;
}

static int cli_get_key(struct cli_param *param, int param_index)
{
    int i;
    struct ap_wep_key *key;
    
    if ((current_radio->wep_index == current_bss_index) ||
        (current_radio->flags & CLI_RADIO_F_REPEATER)) {
        for (i = 0; i < MAX_KEYTABLE_SIZE; i++) {
            key = &current_bss->keytable[i];
            if (key->key_length) {
                printf("key %d: %s\n", i+1, key->key_material);
            }
        }
    }

    if (current_bss->defaultKey != CLI_INVALID_INDEX) {
        printf("default key index: %d\n", current_bss->defaultKey);
    }
    return 0;
}

static int cli_set_key(struct cli_param *param, int param_index, char **argv)
{
    int key_index;
    int argc;
    int rc;
    char cmdbuf[128];

    /* first check if this BSS is allowed to do static wep */
    if (current_radio->wep_index != CLI_INVALID_INDEX &&
        current_radio->wep_index != current_bss_index &&
        !(current_radio->flags & CLI_RADIO_F_REPEATER)) {
        printf("%s\n", wep_txt);
        goto bad;
    }
    
    for (argc = 0; argv[argc]; argc++);
    
    if (argc != 3 && argc != 4)
        goto bad;

    key_index = atoi(argv[1]);
    if (key_index < 1 || key_index > 4)
        goto bad;

    if ((argc == 3) && (strcasecmp(argv[2], "default") == 0)) {
        /* set default key */
        sprintf(cmdbuf, "iwconfig %s key \"[%d]\"", current_bss->iface, key_index);
        rc = cli_exec_cmd(cmdbuf);

        if (rc == 0)
            current_bss->defaultKey = key_index;
    }
    else {
        int keylen, keystrlen;

        keylen = atoi(argv[2]);
        if (keylen != 40 && keylen != 104 && keylen != 128) {
            /* invalid key length */
            goto bad;
        }
        switch (current_bss->keyEntryMethod) {
            case KEY_ENTRY_METHOD_HEX:
                keystrlen = keylen >> 2;
                if ((strlen(argv[3]) != keystrlen ) ||
                    check_hex_string(argv[3]) < 0 ) {
                    goto bad;
                }
                /* set key */
                sprintf(cmdbuf, "iwconfig %s key \"[%d]\" %s",
                	    current_bss->iface, key_index, argv[3]);
                break;
            case KEY_ENTRY_METHOD_ASCII:
                keystrlen = keylen >> 3;
                /* set key */
                if ((strlen(argv[3]) != keystrlen)) {
                    goto bad;
                }
                sprintf(cmdbuf, "iwconfig %s key \"[%d]\" s:%s",
                        current_bss->iface, key_index, argv[3]);
                break;
        }
        rc = cli_exec_cmd(cmdbuf);

        if (rc == 0) {
            struct ap_wep_key *key = &current_bss->keytable[key_index - 1];
            key->key_length = keylen;
            strcpy(key->key_material, argv[3]);
        }
    }

    if (rc == 0) {
        /* set the wep index */
        current_radio->wep_index = current_bss_index;
    }
    
    return rc;
    
bad:
    printf("%s: %s\n", param->name, invalid_txt);
    return 1;
}

static int cli_get_radiusname(struct cli_param *param, int param_index, struct in_addr *addr)
{
    if (cli_vlan_enabled) {
        if (!current_vlan) {
            printf("Error. VLAN enabled. set pvid first\n");
            return 1;
        }
        *addr = current_vlan->radius_name;
        
    } else {
         *addr = apcfg->radius_name;
    }
    return 0;
}

static int cli_set_radiusname(struct cli_param *param, int param_index, struct in_addr *addr)
{
    if (cli_vlan_enabled) {
        if (!current_vlan) {
            printf("Error. VLAN enabled. set pvid first\n");
            return 1;
        }
        current_vlan->radius_name = *addr;
    } else {
        apcfg->radius_name = *addr;
    }
    return 0;
}

static int cli_get_radiusport(struct cli_param *param, int param_index)
{
    if (cli_vlan_enabled) {
        if (!current_vlan) {
            printf("Error. VLAN enabled. set pvid first\n");
            return 1;
        }
        return current_vlan->radius_port;
        
    } else {
         return apcfg->radius_port;
    }
    return apcfg->radius_port;
}

static int cli_set_radiusport(struct cli_param *param, int param_index, int port)
{
    if (cli_vlan_enabled) {
        if (!current_vlan) {
            printf("Error. VLAN enabled. set pvid first\n");
            return 1;
        }
        current_vlan->radius_port = port;
    } else {
        apcfg->radius_port = port;
    }
    return 0;
}

static int cli_set_radiussecret(struct cli_param *param, int param_index, char *secret)
{
    if (strlen(secret) > MAX_SECRET_LENGTH) {
        printf("%s: %s\n", param->name, invalid_txt);
        return 1;
    }
    if (cli_vlan_enabled) {
        if (!current_vlan) {
            printf("Error. VLAN enabled. set pvid first\n");
            return 1;
        }
        strcpy(current_vlan->radius_secret, secret);
    } else {
        strcpy(apcfg->radius_secret, secret);
    }
    return 0;
}

static int cli_get_preauthentication(struct cli_param *param, int param_index)
{
    return current_bss->preauth;
}

static int cli_set_preauthentication(struct cli_param *param, int param_index, int enable)
{
    current_bss->preauth = enable;
    return 0;
}

static int cli_set_passphrase(struct cli_param *param, int param_index, char *passphrase)
{
    /* check the string length. If the string length is MAX_PASSPHRASE,
     * then it's a key. Otherwise it's a passpharse. */
    int len = strlen(passphrase);
    if (len == MAX_PASSPHRASE) {
        /* check for hex string */
        if (check_hex_string(passphrase) < 0) {
            printf("Invalid hex key.\n");
            return 1;
        }
        current_bss->flags |= CLI_BSS_F_HASHED_KEY;
    }
    else if (len < MIN_PASSPHRASE) {
        /* passphrase must be greater than 8 characters */
        printf("Passphrase not long enough.\n");
        return 1;
    }
    else if (len > MAX_PASSPHRASE) {
        printf("Passphrase too long.\n");
        return 1;
    }
    else {
        /* valid passphrase */
        current_bss->flags &= ~CLI_BSS_F_HASHED_KEY;
    }

    strcpy(current_bss->passphrase, passphrase);
    return 0;
}

static int cli_get_grpkey_interval(struct cli_param *param, int param_index)
{
    return current_bss->groupKeyUpdateInterval;
}

static int cli_set_grpkey_interval(struct cli_param *param, int param_index, int interval)
{
    current_bss->groupKeyUpdateInterval = interval;
    return 0;
}

static int cli_get_ipaddr(struct cli_param *param, int param_index, struct in_addr *addr)
{
    *addr = apcfg->ipaddr;
    return 0;
}

static int cli_set_ipaddr(struct cli_param *param, int param_index, struct in_addr *addr)
{
    apcfg->ipaddr = *addr;
    return 0;
}

static int cli_set_ipmask(struct cli_param *param, int param_index, struct in_addr *addr)
{
    apcfg->ipmask = *addr;
    return 0;
}

static int cli_get_ipmask(struct cli_param *param, int param_index, struct in_addr *addr)
{
    *addr = apcfg->ipmask;
    return 0;
}

static int cli_get_vlan_ipaddr(struct cli_param *param, int param_index, struct in_addr *addr)
{
    if (cli_vlan_enabled) {
        if (!current_vlan) {
            printf("Error. VLAN enabled. set pvid first\n");
            return 1;
        }
        *addr = current_vlan->ipaddr;    
    } else {
            printf("Error. VLAN currently disabled\n");
            return 1;
    }
    return 0;
}

static int cli_set_vlan_ipaddr(struct cli_param *param, int param_index, struct in_addr *addr)
{
    if (cli_vlan_enabled) {
        if (!current_vlan) {
            printf("Error. VLAN enabled. set pvid first\n");
            return 1;
        }
        current_vlan->ipaddr = *addr;
        current_vlan->ip_set = 1;  
    } else {
            printf("Error. VLAN currently disabled\n");
            return 1;
    }
    return 0;
}

static int cli_set_vlan_ipmask(struct cli_param *param, int param_index, struct in_addr *addr)
{
    if (cli_vlan_enabled) {
        if (!current_vlan) {
            printf("Error. VLAN enabled. set pvid first\n");
            return 1;
        }
        current_vlan->ipmask = *addr;
    } else {
            printf("Error. VLAN currently disabled\n");
            return 1;
    }
    return 0;
}

static int cli_get_vlan_ipmask(struct cli_param *param, int param_index, struct in_addr *addr)
{
    if (cli_vlan_enabled) {
        if (!current_vlan) {
            printf("Error. VLAN enabled. set pvid first\n");
            return 1;
        }
        *addr = current_vlan->ipmask;
        
    } else {
            printf("Error. VLAN currently disabled\n");
            return 1;
    }
    return 0;
}

/* Jumpstart interface */
static int cli_set_js(struct cli_param *param, int param_index, int enable)
{
	current_bss->js_enabled = enable;
	return 0;
}

static int cli_get_js(struct cli_param *param, int param_index)
{
	return current_bss->js_enabled;
}

static int cli_set_js_passwd(struct cli_param *param, int param_index, char *password)
{
    int len = strlen(password);
    if (len > JS_MAX_PASSWORD) {
        printf("Password too long.\n");
        return 1;
    }
    strcpy(current_bss->js_passwd, password);
    return 0;
}

static int cli_get_js_passwd(struct cli_param *param, int param_index, char *password)
{
    strcpy(password, current_bss->js_passwd);
    return 0;
}

static int cli_create_bss(struct ap_radio *radio, int bss_index, int opmode);
static int cli_delete_bss_all(struct ap_radio *radio);

static int cli_get_repeater(struct cli_param *param,  int param_index)
{
	printf("repeater: %s\n", ((current_radio->flags & CLI_RADIO_F_REPEATER) ?
						      enable_txt : disable_txt));
	return 0;
}

/* XXX: this should NOT be called from interactive shell */
static int cli_set_repeater_flag(struct cli_param *param, int param_index, int enable)
{
	if (enable)
		current_radio->flags |= CLI_RADIO_F_REPEATER;
	else
		current_radio->flags &= ~CLI_RADIO_F_REPEATER;
	return 0;
}

static int cli_set_repeater(struct cli_param *param, int param_index, char **argv)
{
	int argc;
	int rc;
	struct ap_bss *old_current_bss;
	int old_bss_index;

	/* get the number of arguments */
	for (argc = 0; argv[argc]; argc++);

	if (argc < 2)
		goto bad;
	
	/*
	 * enable or disable repeater
	 */
	if (strcasecmp(argv[1], enable_txt) == 0) {
		/* always tear down old configuration */
		cli_delete_bss_all(current_radio);

		/* create a STA vap first */
		rc = cli_create_bss(current_radio, 0, CLI_OPMODE_STA);

		/* enable WDS for STA */
		if (rc == 0) {
			current_bss = &current_radio->bss_array[0];
			current_bss_index = 0;
			
			rc = cli_set_param(&cli_priv_bss_param_list[CLI_PRIV_BSS_PARAM_WDS],
						  CLI_PRIV_BSS_PARAM_WDS, 1);
		}

		/* create an AP vap */
		if (rc == 0)
			rc = cli_create_bss(current_radio, 1, CLI_OPMODE_AP);

		if (rc == 0)
			current_radio->flags |= CLI_RADIO_F_REPEATER;

		cli_set_prompt(current_radio_index, current_bss_index);
		return rc;
	}
	else if (strcasecmp(argv[1], disable_txt) == 0) {
		cli_delete_bss_all(current_radio);
		current_radio->flags &= ~CLI_RADIO_F_REPEATER;
		return 0;
	}

	/*
	 * Set repeater parameters. For most of parameters, we just set the
	 * parameters on both STA and AP.
	 */
	if (argc < 3)	/* must be in the format of "set repeater param value" */
		goto bad;

	if (!(current_radio->flags & CLI_RADIO_F_REPEATER)) {
		printf("Repeater hasn't been enabled yet. Run \"set repeater enable\" first!\n");
		return 1;
	}
	
	argv[0] = "set";
	old_current_bss = current_bss;
	old_bss_index = current_bss_index;

	if (strcasecmp(argv[1], "wds") == 0)
		return 0;		/* ignore wds param, already been taken care of */
	else if (strcasecmp(argv[1], "ssid") == 0) {
		/* apply only to AP vap */
		current_bss = &current_radio->bss_array[1];
		current_bss_index = 1;
		rc = cli_param_cmd(argc, argv, CLI_CMD_SET);
	}
	else if (strcasecmp(argv[1], "remote-ssid") == 0) {
		/* apply only to STA vap */
		current_bss = &current_radio->bss_array[0];
		current_bss_index = 0;
		argv[1] = "ssid";
		rc = cli_param_cmd(argc, argv, CLI_CMD_SET);
	}
	else {
		/* apply to both STA and AP */
		current_bss = &current_radio->bss_array[0];
		current_bss_index = 0;
		rc = cli_param_cmd(argc, argv, CLI_CMD_SET);

		if (rc == 0) {
			current_bss = &current_radio->bss_array[1];
			current_bss_index = 1;
			rc = cli_param_cmd(argc, argv, CLI_CMD_SET);
		}
	}

	/* restore old prompt */
	current_bss = old_current_bss;
	current_bss_index = old_bss_index;
	cli_set_prompt(current_radio_index, current_bss_index);
	
	return rc;
	
bad:
    printf("%s: %s\n", param->name, invalid_txt);
    return 1;
}

static int cli_set_factory(struct cli_param *param, int param_index)
{
    int rc, i;
    char cmdbuf[128];
    
    /* delete all bss first */
    for (i = 0; i < apcfg->num_radio; i++)
        cli_delete_bss_all(&apcfg->radio_array[i]);

    /* WAR for bug 17107. "del bss" will cause ath interface removed
     * from bridge, thus causing brige mac address to change. We need
     * to broadcast an ARP to cause our neighbours to refresh their
     * ARP cache.*/
    {
        char dst[32];

        inet_ntop(AF_INET, &apcfg->ipaddr, dst, 32);

        printf("Updating neighbor's ARP cache...\n");
        sprintf(cmdbuf, "arping -c 3 -U -I br0 -s %s 255.255.255.255 > /dev/null", dst);
        cli_exec_cmd(cmdbuf);
    }

    /* Remove Jumpstart P2 configuration file if any */
    sprintf(cmdbuf, "rm -Rf %s/js_p2.conf", WLAN_CFG_PATH);
    cli_exec_cmd(cmdbuf);
    
    cli_set_default();
    
    /* relode default configuration files */
    rc = cli_run_config_dir(WLAN_CFG_DFT_PATH);
    cli_reset_pointers();
    return rc;
}

static int cli_get_config(struct cli_param *param, int param_index)
{
    int i, j;
    struct cli_cmd_table *table;
    struct cli_param *param_get;
    char *argv[2];

    argv[0] = "get";
    for (j = 0; j < CLI_NUM_PARAM_TABLE; j++) {
        table = &cli_param_table[j];
        
        for (i = 0; i < table->count; i++) {
            param_get = &table->list[i];

            if (i == CLI_PARAM_CONFIG)
                continue;
            
            if (!(param_get->index & CLI_F_HIDDEN)) {
                argv[1] = param_get->name;
                if (strcmp(param_get->name, "vlan_ipaddr") == 0 ||
                    strcmp(param_get->name, "vlan_ipmask") == 0) {
                        if ((!cli_vlan_enabled) ||
                            (cli_vlan_enabled && !current_vlan) ) {
                            continue;
                        }
                  }
                  cli_param_cmd(2, argv, CLI_CMD_GET);
            }
        }
    }
    return 0;
}

static struct cli_country countries[] = {
    {0,                "NA" },
    {CTRY_ALBANIA,     "AL" },
    {CTRY_ALGERIA,     "DZ" },
    {CTRY_ARGENTINA,   "AR" },
    {CTRY_ARMENIA,     "AM" },
    {CTRY_AUSTRALIA,   "AU" },
    {CTRY_AUSTRIA,     "AT" },
    {CTRY_AZERBAIJAN,  "AZ" },
    {CTRY_BAHRAIN,     "BH" },
    {CTRY_BELARUS,     "BY" },
    {CTRY_BELGIUM,     "BE" },
    {CTRY_BELIZE,      "BZ" },
    {CTRY_BOLIVIA,     "BO" },
    {CTRY_BRAZIL,      "BR" },
    {CTRY_BRUNEI_DARUSSALAM, "BN" },
    {CTRY_BULGARIA,    "BG" },
    {CTRY_CANADA,      "CA" },
    {CTRY_CHILE,       "CL" },
    {CTRY_CHINA,       "CN" },
    {CTRY_COLOMBIA,    "CO" },
    {CTRY_COSTA_RICA,  "CR" },
    {CTRY_CROATIA,     "HR" },
    {CTRY_CYPRUS,      "CY" },
    {CTRY_CZECH,       "CZ" },
    {CTRY_DENMARK,      "DK" },
    {CTRY_DOMINICAN_REPUBLIC, "DO" },
    {CTRY_ECUADOR,     "EC" },
    {CTRY_EGYPT,       "EG" },
    {CTRY_EL_SALVADOR, "SV" },
    {CTRY_ESTONIA,     "EE" },
    {CTRY_FINLAND,     "FI" },
    {CTRY_FRANCE,      "FR" },
    {CTRY_FRANCE2,     "F2" },
    {CTRY_GEORGIA,     "GE" },
    {CTRY_GERMANY,     "DE" },
    {CTRY_GREECE,      "GR" },
    {CTRY_GUATEMALA,   "GT" },
    {CTRY_HONDURAS,    "HN" },
    {CTRY_HONG_KONG,   "HK" },
    {CTRY_HUNGARY,     "HU" },
    {CTRY_ICELAND,     "IS" },
    {CTRY_INDIA,       "IN" },
    {CTRY_INDONESIA,   "ID" },
    {CTRY_IRAN,        "IR" },
    {CTRY_IRELAND,     "IE" },
    {CTRY_ISRAEL,      "IL" },
    {CTRY_ITALY,       "IT" },
    {CTRY_JAPAN,       "JP" },
    {CTRY_JAPAN1,      "J1" },
    {CTRY_JAPAN2,      "J2" },
    {CTRY_JAPAN3,      "J3" },
    {CTRY_JAPAN4,      "J4" },
    {CTRY_JAPAN5,      "J5" },
    {CTRY_JAPAN6,      "J6" },
	{CTRY_JAPAN7,      "J7" },
	{CTRY_JAPAN8,      "J8" },
	{CTRY_JAPAN9,      "J9" },
	{CTRY_JAPAN10,     "J10" },
	{CTRY_JAPAN11,     "J11" },
	{CTRY_JAPAN12,     "J12" },
	{CTRY_JAPAN13,     "J13" },
	{CTRY_JAPAN14,     "J14" },
	{CTRY_JAPAN15,     "J15" },
	{CTRY_JAPAN16,     "J16" },
	{CTRY_JAPAN17,     "J17" },
	{CTRY_JAPAN18,     "J18" },
	{CTRY_JAPAN19,     "J19" },
	{CTRY_JAPAN20,     "J20" },
	{CTRY_JAPAN21,     "J21" },
	{CTRY_JAPAN22,     "J22" },
	{CTRY_JAPAN23,     "J23" },
	{CTRY_JAPAN24,     "J24" },
    {CTRY_JORDAN,      "JO" },
    {CTRY_KAZAKHSTAN,  "KZ" },
    {CTRY_KOREA_NORTH, "KP" },
    {CTRY_KOREA_ROC,   "KR" },
    {CTRY_KOREA_ROC2,  "K2" },
    {CTRY_KOREA_ROC3,  "K3" },
    {CTRY_KUWAIT,      "KW" },
    {CTRY_LATVIA,      "LV" },
    {CTRY_LEBANON,     "LB" },
    {CTRY_LIECHTENSTEIN, "LI" },
    {CTRY_LITHUANIA,   "LT" },
    {CTRY_LUXEMBOURG,  "LU" },
    {CTRY_MACAU,       "MO" },
    {CTRY_MACEDONIA,   "MK" },
    {CTRY_MALAYSIA,    "MY" },
    {CTRY_MALTA,       "MT" },
    {CTRY_MEXICO,      "MX" },
    {CTRY_MONACO,      "MC" },
    {CTRY_MOROCCO,     "MA" },
    {CTRY_NETHERLANDS, "NL" },
    {CTRY_NEW_ZEALAND, "NZ" },
    {CTRY_NORWAY,      "NO" },
    {CTRY_OMAN,        "OM" },
    {CTRY_PAKISTAN,    "PK" },
    {CTRY_PANAMA,      "PA" },
    {CTRY_PERU,        "PE" },
    {CTRY_PHILIPPINES, "PH" },
    {CTRY_POLAND,      "PL" },
    {CTRY_PORTUGAL,    "PT" },
    {CTRY_PUERTO_RICO, "PR" },
    {CTRY_QATAR,       "QA" },
    {CTRY_ROMANIA,     "RO" },
    {CTRY_RUSSIA,      "RU" },
    {CTRY_SAUDI_ARABIA, "SA" },
    {CTRY_SINGAPORE,   "SG" },
    {CTRY_SLOVAKIA,    "SK" },
    {CTRY_SLOVENIA,    "SI" },
    {CTRY_SOUTH_AFRICA, "ZA" },
    {CTRY_SPAIN,       "ES" },
    {CTRY_SWEDEN,      "SE" },
    {CTRY_SWITZERLAND, "CH" },
    {CTRY_SYRIA,       "SY" },
    {CTRY_TAIWAN,      "TW" },
    {CTRY_THAILAND,    "TH" },
    {CTRY_TRINIDAD_Y_TOBAGO, "TT" },
    {CTRY_TUNISIA,     "TN" },
    {CTRY_TURKEY,      "TR" },
    {CTRY_UKRAINE,     "UA" },
    {CTRY_UAE,         "AE" },
    {CTRY_UNITED_KINGDOM, "GB" },
    {CTRY_UNITED_STATES, "US" },
    {CTRY_UNITED_STATES_FCC49, "PS" },
    {CTRY_URUGUAY,     "UY" },
    {CTRY_UZBEKISTAN,  "UZ" },
    {CTRY_VENEZUELA,   "VE" },
    {CTRY_VIET_NAM,    "VN" },
    {CTRY_YEMEN,       "YE" },
    {CTRY_ZIMBABWE,    "ZW" },
};

#define NUM_OF_COUNTRIES    (sizeof(countries)/sizeof(countries[0]))

static char *cli_get_countryname(int cc)
{
    int i;
    for (i = 0; i < NUM_OF_COUNTRIES; i++) {
        if (countries[i].cc == cc)
            return countries[i].iso_name;
    }
    return NULL;
}

static int cli_get_country(struct cli_param *param, int param_index, char *cc_txt)
{
    char *name = cli_get_countryname(apcfg->cc);
    if (name) {
        strcpy(cc_txt, name);
        return 0;
    }
    return 1;
}

static int cli_set_country(struct cli_param *param, int param_index, char *cc_txt)
{
    int i;
    for (i = 0; i < NUM_OF_COUNTRIES; i++) {
        if (strcmp(cc_txt, countries[i].iso_name) == 0) {
            apcfg->cc = countries[i].cc;
            return 0;
        }
    }
    printf("Invalid country code\n");
    return 1;
}

/*
 * Create/Destroy BSS interface
 */
static int cli_bss_bits = 0;

static int cli_get_interface(struct cli_param *param, int param_index)
{
    printf("%s\n", current_bss->iface);
    return 0;
}

static int cli_get_bss(struct cli_param *param, int param_index)
{
    int i;
    for (i = 0; i < MAX_BSS_NUM; i++) {
        if (!(current_radio->bss_array[i].flags & CLI_BSS_F_FREE))
            printf("%d ", i);
    }
    printf("\n");
    return 0;
}

static int cli_set_vap(struct cli_param *param, int param_index, int enable)
{
    cli_vap_enabled = enable;
    return 0;
}

static int cli_set_dualap(struct cli_param *param, int param_index, int enable)
{
    apcfg->num_radio = (enable) ? 2 : 1;
    return 0;
}

static int cli_get_vlan(struct cli_param *param, int param_index)
{
    return cli_vlan_enabled;
}

static int cli_set_vlan(struct cli_param *param, int param_index, int enable)
{
    cli_vlan_enabled = enable;
    return 0;
}

static int cli_get_pvid(struct cli_param *param, int param_index)
{
    return current_bss->pvid;
}

static int cli_set_pvid(struct cli_param *param, int param_index, int pvid)
{
    if (pvid == 0 || pvid == 0xfff) {
        printf("Invalid VLAN ID\n");
        return 1;
    }
    
    if (current_bss->vlan_ip == NULL) {
        /* Original VLAN setting for this BSS */
        current_vlan = cli_vlan_lookup(pvid);
        if (current_vlan == NULL) {
            printf("VLAN set up error\n");
            return 1;
        }
        current_bss->vlan_ip = current_vlan;
        current_vlan->ref_count++;
    } else {
        if (current_bss->vlan_ip->vlan_tag != pvid) {
            /* VLAN setting change for this BSS */  
            current_bss->vlan_ip->ref_count--;
            if (!current_bss->vlan_ip->ref_count) {
                /* Reuse VLAN_IP structure */
                current_vlan = current_bss->vlan_ip;
            } else {
                current_vlan = cli_vlan_lookup(pvid);
                if (current_vlan == NULL) {
                    printf("VLAN set up error\n");
                    return 1;
                }
                current_bss->vlan_ip = current_vlan;
                current_vlan->ref_count++;
            }
        } else {
            current_vlan = current_bss->vlan_ip;
        }
    }
    current_vlan->vlan_tag = pvid;
    current_bss->pvid = pvid;
    return 0;
}

static int cli_get_free_bss(void)
{
    int i;
    for (i = 0; i < MAX_BSS_NUM; i++) {
        if (current_radio->bss_array[i].flags & CLI_BSS_F_FREE) {
            return i;
        }
    }
    return -1;
}

static int cli_create_bss(struct ap_radio *radio, int bss_index, int opmode)
{
    struct ap_bss *bss;
    int rc = 0;

    /* bss_index already been checked */
    bss = &radio->bss_array[bss_index];
    if (!(bss->flags & CLI_BSS_F_FREE)) {
        printf("BSS %d has already been created.\n", bss_index);
        return 1;
    }

    /* Create VAP */
    if (cli_vap_enabled) {
#if 0
        char cmdbuf[64];

        /* Only send down command when VAP is enabled */
        sprintf(cmdbuf, "wlanconfig %s create wlandev %s wlanmode ap",
                DEFAULT_BSS_IFACE, radio->vapctl_iface);
        rc = cli_exec_cmd(cmdbuf);
#else
        if (cli_driver_check) {
            int sockfd;
            struct ieee80211_clone_params cp;
            struct ifreq ifr;

            memset(&cp, 0, sizeof(cp));
            strncpy(cp.icp_name, DEFAULT_BSS_IFACE, IFNAMSIZ);
            cp.icp_flags = IEEE80211_CLONE_BSSID;
            
            if (opmode == CLI_OPMODE_AP) {
                cp.icp_opmode = IEEE80211_M_HOSTAP;
            }
            else if (opmode == CLI_OPMODE_STA) {
                cp.icp_opmode = IEEE80211_M_STA;
                cp.icp_flags |= IEEE80211_NO_STABEACONS;
            }
            
            memset(&ifr, 0, sizeof(ifr));
            strncpy(ifr.ifr_name, radio->vapctl_iface, IFNAMSIZ);
            ifr.ifr_data = (void *)&cp;

            sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (sockfd < 0) {
                perror("Socket");
                rc = 1;
            }
            else {
                rc = ioctl(sockfd, SIOC80211IFCREATE, &ifr);
                if (rc)
                    perror("SIOC80211IFCREATE");
            }
        }
        
#endif        
    }
    else {
        /* if VAP is not enabled, we're only allowed to create VAP 0
         * as a special case for non-VAP AP. */
        if (bss_index != 0) {
            printf("BSS %d not supported on non-VAP AP.\n", bss_index);
            rc = 1;
        }
    }
    
    if (rc == 0) {
        int bit = 0;

        /* get the first available number used for interface name */
        while (cli_bss_bits & (1 << bit))
            bit++;

        /* assign the interface name to the new created bss */
        sprintf(bss->iface, "%s%d", DEFAULT_BSS_IFACE, bit);

        /* set corresponding flags */
        bss->flags &= ~CLI_BSS_F_FREE;
        cli_bss_bits |= (1 << bit);

        if (cli_vap_enabled)
            bss->opmode = opmode;
        
        radio->num_bss++;
    }
    return rc;
}

static int cli_delete_bss(struct ap_radio *radio, int bss_index)
{
    struct ap_bss *bss;
    char *p;
    int bit;
    int rc = 0;

    /* bss index has been checked */
    bss = &radio->bss_array[bss_index];
    p = bss->iface + strlen(DEFAULT_BSS_IFACE);
    
    if (bss->flags & CLI_BSS_F_FREE ||
        *p < '0' ||
        *p > '9') {
        printf("BSS %d hasn't been created.\n", bss_index);
        return 1;
    }

    if (cli_vap_enabled) {
#if 0        
        char cmdbuf[64];
        /* only send down command when VAP is enabled */
        sprintf(cmdbuf, "wlanconfig %s destroy", bss->iface);
        rc = cli_exec_cmd(cmdbuf);
#else
        if (cli_driver_check) {
            int sockfd;
            struct ifreq ifr;

            memset(&ifr, 0, sizeof(ifr));
            strncpy(ifr.ifr_name, bss->iface, IFNAMSIZ);

            sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (sockfd < 0) {
                perror("Socket");
                rc = 1;
            }
            else {
                rc = ioctl(sockfd, SIOC80211IFDESTROY, &ifr);
                if (rc)
                    perror("SIOC80211IFDESTROY");
            }
        }
#endif
    }
    
    if (rc == 0) {
		/* free ACL table if necessary */
		if (bss->acl_table) {
			free(bss->acl_table);
			bss->acl_table = NULL;
			bss->acl_size = 0;
		}

		/* free this BSS */
        bit = atoi(p);
        cli_bss_bits &= ~(1 << bit);
        bss->flags |= CLI_BSS_F_FREE;
        
        radio->num_bss--;

		/* reset the WEP index if necessary */
		if (current_radio->wep_index == bss_index)
			current_radio->wep_index = CLI_INVALID_INDEX;
		
        /* if we're deleting the current bss,
         * we have to change the prompt and current bss index */
        if (current_radio == radio && current_bss_index == bss_index) {
            current_bss_index = -1;
            current_bss = NULL;
            cli_set_prompt(current_radio_index, -1);
        }
    }
    return rc;
}

static int cli_delete_bss_all(struct ap_radio *radio)
{
    int i, rc = 0;
    for (i = 0; i < MAX_BSS_NUM; i++) {
        if (!(radio->bss_array[i].flags & CLI_BSS_F_FREE)) {
            rc += cli_delete_bss(radio, i);
        }
    }
    return rc;
}

static int cli_add_cmd(int argc, char **argv)
{
    int rc = 0;
    
    if (argc < 2)
        goto invalid;

    if (strcmp(argv[1], "bss") == 0 ||
        strcmp(argv[1], "sta") == 0) { /* add new bss */
        int bss_index;

        if (argc == 2) {
            /* we pick a free BSS */
            bss_index = cli_get_free_bss();
            if (bss_index < 0) {
                printf("No more free BSS available.\n");
                return 1;
            }
        }
        else if (argc == 3) {
            if (strcmp(argv[2], "0") == 0) {
                bss_index = 0;
            }
            else {
                bss_index = atoi(argv[2]);
                if (bss_index >= MAX_BSS_NUM || bss_index <= 0) {
                    printf("BSS index must be in [0-3]\n");
                    goto invalid;
                }
            }
        }
        else {
            goto invalid;
        }
        
        rc = cli_create_bss(current_radio, bss_index,
                            strcmp(argv[1], "bss") ?
                            CLI_OPMODE_STA : CLI_OPMODE_AP);
    }
	else if (strcmp(argv[1], "acl") == 0) {	/* add a new ACL entry */
		if (current_bss == NULL) {
			printf("Must select a BSS first\n");
			goto invalid;
		}

		if (!argv[2]) {
			printf("Must specify a MAC address\n");
			goto invalid;
		}

		rc = cli_add_acl(argv[2]);
	}
	
    return rc;
    
invalid:
    printf("add: %s\n", invalid_txt);
    return 1;
}

static int cli_del_cmd(int argc, char **argv)
{
    int rc = 0;

    if (strcmp(argv[1], "bss") == 0 ||
        strcmp(argv[1], "sta") == 0) {  /* delete bss */

        if (argc < 3)
            goto invalid;

        if (strcmp(argv[2], "all") == 0) { /* delete all bss'es */
            rc = cli_delete_bss_all(current_radio);
        }
        else {
            int bss_index;

            if (strcmp(argv[2], "0") == 0) {
                bss_index = 0;
            }
            else {
                bss_index = atoi(argv[2]);
                if (bss_index <= 0 || bss_index >= MAX_BSS_NUM) {
                    printf("BSS index must be in [0-3]\n");
                    goto invalid;
                }
            }

            rc = cli_delete_bss(current_radio, bss_index);
        }
    }
	else if (strcmp(argv[1], "acl") == 0) {	/* delete an ACL entry */
		if (current_bss == NULL) {
			printf("Must select a BSS first\n");
			goto invalid;
		}

		if (!argv[2]) {
			printf("Must specify a MAC address\n");
			goto invalid;
		}

		rc = cli_del_acl(argv[2]);
	}

    return rc;
    
invalid:
    printf("del: %s\n", invalid_txt);
    return 1;
}

static int cli_help(int argc, char **argv)
{
    int d, i, j, k;
    struct cli_param *param;
    
    if (argc == 1)
        goto fall_through;

    if (strcmp(argv[1], "get") == 0)
        d = CLI_CMD_GET;
    else if (strcmp(argv[1], "set") == 0)
        d = CLI_CMD_SET;
    else if (strcmp(argv[1], "config") == 0) {
        printf("Switch radio and virtual BSS:\n");
        printf("config <[wlan <unit>] [bss <bss>]>\n\n");
        return 0;
    }
    else if (strcmp(argv[1], "commit") == 0) {
        printf("Save configuration files.\n\n");
        return 0;
    }
    else if (strcmp(argv[1], "add") == 0) {
        printf("add bss [0-%d]\n", MAX_BSS_NUM);
        return 0;
    }
    else if (strcmp(argv[1], "del") == 0) {
        printf("del bss <all|0-%d>\n", MAX_BSS_NUM);
        return 0;
    }
    else
        goto fall_through;

    printf("Available parameters:\n\n");

    k = 0;
    for (j = 0; j < CLI_NUM_PARAM_TABLE; j++) {
        struct cli_cmd_table *table = &cli_param_table[j];
        
        for (i = 0; i < table->count; i++) {
            param = &table->list[i];

            if (!(param->index & CLI_F_HIDDEN)) {
                if (((d == CLI_CMD_GET) && (param->get != NULL)) ||
                    ((d == CLI_CMD_SET) && (param->set != NULL))) {
                    printf("%s ", param->name);

                    if (((++k)%4) == 0)
                        printf("\n");
                }
            }
        }
    }

    printf("\n\n");
    return 0;
    
fall_through:
    return 0xff;
}

/*
 * CLI configuration files handling
 */
#define CLI_FILE_EXCEPTION  1

static char *cli_header = "# Copyright (C) 2005 Atheros Communications, Inc., All Rights Reserved\n\
# DO NOT EDIT -- This file is will be overwritten by CLI.\n\n";

static char cli_filename[64];
static char cli_backupname[64];

/*
 * Use exception handling (long jump) to reduce code size,
 * otherwise, each fputs would require a branch
 */
static void cli_fputs(char *p, FILE *fp)
{
    if (fputs(p, fp) == EOF) {
        fclose(fp);
        exraise(CLI_FILE_EXCEPTION);
    }
}

static FILE *cli_fopen(void)
{
    FILE *fp;
    
    /* save the old copy */
    rename(cli_filename, cli_backupname);

    /* create config file */
    fp = fopen(cli_filename, "w");
    if (fp == NULL) {
        printf("Can't create %s\n", cli_filename);
        exraise(CLI_FILE_EXCEPTION);
    }
    return fp;
}

/* This function generates the hostapd configuration file */
static int cli_save_hostapd(int unit, int bss_index)
{
    struct ap_bss *bss = &apcfg->radio_array[unit].bss_array[bss_index];
    FILE *fp;
    int wpa, psk, radius;
    char buf[128];

    sprintf(cli_filename, "%s.%d.%d", HOSTAPD_CFG_FILE, unit, bss_index);
    sprintf(cli_backupname, "%s.%d.%d", HOSTAPD_CFG_BACKUP, unit, bss_index);

    /* if security is not turned on, or auth type is not wpa or 802.1x,
     * there's no need to have a hostapd.conf */
    if ((bss->flags & CLI_BSS_F_FREE) ||
        (!(bss->flags & CLI_BSS_F_SECURE)) ||
        (bss->authType < CLI_AUTH_8021X)) {
        /* delete the hostapd.conf if there's any */
        rename(cli_filename, cli_backupname);
        return 0;
    }

    fp = cli_fopen();
    cli_fputs(cli_header, fp);

    /* CLI only save the "variable" configurations.
     * The "fixed" configurations are done by ap_service scripts */

    /* ssid */
    sprintf(buf, "ssid=%s\n", bss->ssid);
    cli_fputs(buf, fp);

    /* WPA version */
    wpa = 0; radius = 0; psk = 0;   
    switch (bss->authType) {
        case CLI_AUTH_8021X: radius = 1; break;
        case CLI_AUTH_WPA: wpa = 1, radius = 1; break;              
        case CLI_AUTH_WPAPSK: wpa = 1; psk = 1; break;               
        case CLI_AUTH_WPA2: wpa = 2; radius = 1; break;
        case CLI_AUTH_WPA2PSK: wpa = 2, psk = 1; break;
        case CLI_AUTH_WPA_AUTO: wpa = 3, radius = 1; break;
        case CLI_AUTH_WPA_AUTO_PSK: wpa = 3, psk = 1; break;
        default:
            printf("Unknown auth type\n");
    }

    /* Write out WPA version */
    if (wpa) {
        sprintf(buf, "wpa=%d\n", wpa);
        cli_fputs(buf, fp);
    }
    
    /* write out WPAPSK info if necessary */
    if (psk) {
        if (bss->flags & CLI_BSS_F_HASHED_KEY)
            sprintf(buf, "wpa_psk=%s\n", bss->passphrase);
        else
            sprintf(buf, "wpa_passphrase=%s\n", bss->passphrase);
        cli_fputs(buf, fp);
        cli_fputs("wpa_key_mgmt=WPA-PSK\n", fp);
    }

    /* write out RADIUS info if necessary */
    if (radius) {
        char dst[32];

        if (cli_vlan_enabled) {
            if (bss->vlan_ip->ip_set) {
                inet_ntop(AF_INET, &bss->vlan_ip->ipaddr, dst, 32);
                sprintf(buf, "own_ip_addr=%s\n", dst);
            } else {
                inet_ntop(AF_INET, &apcfg->ipaddr, dst, 32);
                sprintf(buf, "own_ip_addr=%s\n", dst);
            }
            cli_fputs(buf, fp);
            inet_ntop(AF_INET, &bss->vlan_ip->radius_name, dst, 32);
            sprintf(buf, "auth_server_addr=%s\n", dst);
            cli_fputs(buf, fp);
            sprintf(buf, "auth_server_port=%d\n", bss->vlan_ip->radius_port);
            cli_fputs(buf, fp);
            sprintf(buf, "auth_server_shared_secret=%s\n", bss->vlan_ip->radius_secret);
            cli_fputs(buf, fp);
        } else {
            inet_ntop(AF_INET, &apcfg->ipaddr, dst, 32);
            sprintf(buf, "own_ip_addr=%s\n", dst);
            cli_fputs(buf, fp);
            inet_ntop(AF_INET, &apcfg->radius_name, dst, 32);
            sprintf(buf, "auth_server_addr=%s\n", dst);
            cli_fputs(buf, fp);
            sprintf(buf, "auth_server_port=%d\n", apcfg->radius_port);
            cli_fputs(buf, fp);
            sprintf(buf, "auth_server_shared_secret=%s\n", apcfg->radius_secret);
            cli_fputs(buf, fp);
        }
        cli_fputs("ieee8021x=1\n", fp);

        if (wpa) {
            /* WPA/WPA2 */
            cli_fputs("wpa_key_mgmt=WPA-EAP\n", fp);
            sprintf(buf, "rsn_preauth=%d\n", bss->preauth);
             cli_fputs(buf, fp);
            if (cli_vlan_enabled && bss->pvid) {
                sprintf(buf, "rsn_preauth_interfaces=%s%d\n", "br", bss->pvid);
                cli_fputs(buf, fp);
            } else {
                sprintf(buf, "rsn_preauth_interfaces=%s\n", "br0");
                cli_fputs(buf, fp);
            }
        }
        else {
            /* 802.1x. Always use 104 as the key length */
            cli_fputs("wep_key_len_unicast=13\n", fp);
            cli_fputs("wep_key_len_broadcast=13\n", fp);
        }
    }

    if (wpa) {
        /* write out cipher info */
        sprintf(buf, "wpa_pairwise=%s\n", cli_cipher_list[bss->cipher].iw_name);
        cli_fputs(buf, fp);

        /* write out group key update interval */
        sprintf(buf, "wpa_group_rekey=%d\n", bss->groupKeyUpdateInterval);
        cli_fputs(buf, fp);
    }
    
    fclose(fp);
    return 0;
}

static void cli_save_set_number(FILE *fp, struct cli_param *param, int num)
{
    char buf[128];
    sprintf(buf, "set %s %d\n", param->name, num);
    cli_fputs(buf, fp);
}

static void cli_save_set_txt(FILE *fp, struct cli_param *param, char *txt)
{
    char buf[128];
    if (txt && strlen(txt)) {
        sprintf(buf, "set %s %s\n", param->name, txt);
        cli_fputs(buf, fp);
    }
}

static void cli_save_set_boolean(FILE *fp, struct cli_param *param, int enable)
{
    char buf[128];
    sprintf(buf, "set %s %s\n", param->name, (enable ? enable_txt : disable_txt));
    cli_fputs(buf, fp);
}

static void cli_save_set_ip(FILE *fp, struct cli_param *param, struct in_addr *addr)
{
    char dst[32];
    char buf[128];

    inet_ntop(AF_INET, addr, dst, 32);
    sprintf(buf, "set %s %s\n", param->name, dst);
    cli_fputs(buf, fp);
}

/* This function generate the bss portion of wlan configuratio scripts */
static int cli_save_bss(int unit, int bss_index)
{
    struct ap_radio *radio = &apcfg->radio_array[unit];
    struct ap_bss *bss = &radio->bss_array[bss_index];    
    struct cli_param *priv;
    FILE *fp;
    int i;
    char buf[128];
    
    sprintf(cli_filename, "%s.%d.%d", WLAN_CFG_FILE, unit, bss_index);
    sprintf(cli_backupname, "%s.%d.%d", WLAN_CFG_BACKUP, unit, bss_index);

    if (bss->flags & CLI_BSS_F_FREE) {
        /* If the BSS is free, just delete the old configuration file */
        rename(cli_filename, cli_backupname);
        goto done;
    }
    
    fp = cli_fopen();
    cli_fputs(cli_header, fp);

    /* switch to the bss */
    sprintf(buf, "config bss %d\n\n", bss_index);
    cli_fputs(buf, fp);

    /* operation mode */
    if (cli_vap_enabled == 0) {
        /* Operation mode. STA or AP
         * set opmode is only allowed in non-VAP AP. */
        cli_save_set_txt(fp, &cli_param_list[CLI_PARAM_OPMODE],
                         cli_opmode_list[bss->opmode].cli_name);
    }

    /* ssid */
    sprintf(buf, "\"%s\"", bss->ssid);
    cli_save_set_txt(fp, &cli_param_list[CLI_PARAM_SSID], buf);

    /* vlan id, ipaddr, ipmask */
    if (cli_vlan_enabled && bss->pvid) {
        cli_save_set_number(fp, &cli_param_list[CLI_PARAM_PVID], bss->pvid);
        cli_save_set_ip(fp, &cli_param_list[CLI_PARAM_VLAN_IPADDR], &bss->vlan_ip->ipaddr);
        cli_save_set_ip(fp, &cli_param_list[CLI_PARAM_VLAN_IPMASK], &bss->vlan_ip->ipmask);
        cli_save_set_ip(fp, &cli_param_list[CLI_PARAM_RADIUSNAME],
                        &bss->vlan_ip->radius_name);
        cli_save_set_number(fp, &cli_param_list[CLI_PARAM_RADIUSPORT], 
                            bss->vlan_ip->radius_port);
        sprintf(buf, "\"%s\"", bss->vlan_ip->radius_secret);
        cli_save_set_txt(fp, &cli_param_list[CLI_PARAM_RADIUSSECRET], buf);
    }
    /* rate */
	/* XXX: WAR for bug 17615: Do NOT set rate in STA mode
	 * before bring up the interface.*/
	if (bss->opmode != CLI_OPMODE_STA) {
		if (bss->rate == CLI_BEST_RATE) {
			cli_save_set_txt(fp, &cli_param_list[CLI_PARAM_RATE], best_txt);
		}
		else {
			cli_save_set_number(fp, &cli_param_list[CLI_PARAM_RATE], bss->rate);
		}
	}
    /* iwpriv settings */
    for (i = 0; i < CLI_PRIV_BSS_MAX_PARAM; i++) {
        priv = &cli_priv_bss_param_list[i];

        if (priv->index & CLI_F_PROBE_ONLY) /* debug commands. don't save */
            continue;
        
        if (priv->index & CLI_FORMAT_INTEGER) {
            cli_save_set_number(fp, priv, bss->priv_params[i]);
        }
        else if (priv->index & CLI_FORMAT_BOOLEAN) {
            cli_save_set_boolean(fp, priv, bss->priv_params[i]);
        }
    }

	/* ACL settings */
	if (bss->acl_mode <= IEEE80211_MACCMD_POLICY_DENY) {
		cli_save_set_txt(fp, &cli_param_list[CLI_PARAM_ACL], cli_acl_cmd[bss->acl_mode]);

		if (bss->acl_table) {
			struct acl_entry *entry;
			for (i = 0; i < bss->acl_size; i++) {
				entry = &bss->acl_table[i];
				if (entry->used) {
					sprintf(buf, "add acl %s\n", entry->macaddr);
					cli_fputs(buf, fp);
				}
			}
		}
	}

    /* Jumpstart setting */
    cli_save_set_boolean(fp, &cli_param_list[CLI_PARAM_JS],
                         (bss->js_enabled));
    cli_save_set_txt(fp, &cli_param_list[CLI_PARAM_JS_PASSWD],
                     bss->js_passwd);
    
    /* security settings */
    cli_save_set_boolean(fp, &cli_param_list[CLI_PARAM_ENCRYPTION],
                         (bss->flags & CLI_BSS_F_SECURE));

    if (bss->flags & CLI_BSS_F_SECURE) {
        cli_save_set_txt(fp, &cli_param_list[CLI_PARAM_AUTH], cli_auth_list[bss->authType]);

        if (bss->authType >= CLI_AUTH_WPA) {
            cli_save_set_txt(fp, &cli_param_list[CLI_PARAM_CIPHER], 
                             cli_cipher_list[bss->cipher].cli_name);
            cli_save_set_number(fp, &cli_param_list[CLI_PARAM_GROUPKEYUPDATE], 
                                bss->groupKeyUpdateInterval);
            if (bss->authType == CLI_AUTH_WPAPSK ||
                bss->authType == CLI_AUTH_WPA2PSK || 
                bss->authType == CLI_AUTH_WPA_AUTO_PSK) {
                sprintf(buf, "\"%s\"", bss->passphrase);
                cli_save_set_txt(fp, &cli_param_list[CLI_PARAM_PASSPHRASE], buf);
            }
            cli_save_set_boolean(fp, &cli_param_list[CLI_PARAM_PREAUTHENTICATION], 
                                 bss->preauth);    
        }
        else if ((radio->wep_index == bss_index) ||  /* only set static wep for one BSS */
                 (radio->flags & CLI_RADIO_F_REPEATER)) { /* or WDS/Repeater mode */
            struct ap_wep_key *key;
            char keybuf[64];

            /* set keys first */
            for (i = 0; i < MAX_KEYTABLE_SIZE; i++) {
                key = &bss->keytable[i];
                if (key->key_length) {
                    sprintf(keybuf, "%d %d %s",
                            i+1, key->key_length, key->key_material);
                    cli_save_set_txt(fp, &cli_param_list[CLI_PARAM_KEY], keybuf);
                }
            }

            if (bss->defaultKey >= 1 && bss->defaultKey <= 4) {
                /* set default key index */
                sprintf(keybuf, "%d default", bss->defaultKey);
                cli_save_set_txt(fp, &cli_param_list[CLI_PARAM_KEY], keybuf);
            }                
        }
    }

    /* wlanstate: up or down upon restart or reboot */
    cli_save_set_boolean(fp, &cli_param_list[CLI_PARAM_WLANSTATE], bss->wlanstate);
    switch (bss->keyEntryMethod) {
        case KEY_ENTRY_METHOD_ASCII:
            cli_save_set_txt(fp,&cli_param_list[CLI_PARAM_KEY_ENTRYMETHOD],"ascii");
            break;
        case KEY_ENTRY_METHOD_HEX:
            cli_save_set_txt(fp,&cli_param_list[CLI_PARAM_KEY_ENTRYMETHOD],"hex");
            break;
    }
    fclose(fp);
    
done:
    cli_save_hostapd(unit, bss_index);
    return 0;
}

/* This function generate the scripts to configure a radio */
static int cli_save_radio(int unit)
{
    struct ap_radio *radio = &apcfg->radio_array[unit];
    struct ap_bss *bss;
    struct cli_param *priv;
    int i;
    FILE *fp;
    char buf[128];

    sprintf(cli_filename, "%s.%d", WLAN_CFG_FILE, unit);
    sprintf(cli_backupname, "%s.%d", WLAN_CFG_BACKUP, unit);

    fp = cli_fopen();
    cli_fputs(cli_header, fp);

    /* switch to current radio */
    sprintf(buf, "config wlan %d\n\n", unit);
    cli_fputs(buf, fp);
    
    /* Delete all BSS first
     * This is just a hack to reset states of the newly spawned shell.
     * Otherwise we'll end up creating the same VAP twice. */
    cli_save_set_boolean(fp, &cli_param_list[CLI_PARAM_VAP], 0);
    cli_fputs("del bss all > /dev/null 2>&1\n", fp);
        
    /* set VAP enable flag */
    cli_save_set_boolean(fp, &cli_param_list[CLI_PARAM_VAP], cli_vap_enabled);

    /* create virtual BSS'es if necessary */
    for (i = 0; i < MAX_BSS_NUM; i++) {
        bss = &radio->bss_array[i];
        if (!(bss->flags & CLI_BSS_F_FREE)) {
            if (bss->opmode == CLI_OPMODE_AP)
                sprintf(buf, "add bss %d\n", i);
            else if (bss->opmode == CLI_OPMODE_STA)
                sprintf(buf, "add sta %d\n", i);
            cli_fputs(buf, fp);
        }
    }
    if (radio->num_bss == 0) {
        /* No BSS created so far, no need to configure this radio */
        fclose(fp);
        return 0;
    }

    cli_fputs("\n", fp);

    /* wireless mode. 11ABG */
    cli_save_set_txt(fp, &cli_param_list[CLI_PARAM_WMODE], cli_wmode_list[radio->wmode].cli_name);

    /* set iwpriv parameters */
    for (i = 0; i < CLI_PRIV_RADIO_MAX_PARAM; i++) {
        priv = &cli_priv_radio_param_list[i];

        if (priv->index & CLI_F_PROBE_ONLY) /* debug commands. don't save */
            continue;
        
        if (priv->index & CLI_FORMAT_INTEGER) {
            cli_save_set_number(fp, priv, radio->priv_params[i]);
        }
        else if (priv->index & CLI_FORMAT_BOOLEAN) {
            cli_save_set_boolean(fp, priv, radio->priv_params[i]);
        }
    }
    
    /* autochannelselect and channel */
    cli_save_set_boolean(fp, &cli_param_list[CLI_PARAM_AUTOCHANNEL],
                         (radio->flags & CLI_RADIO_F_AUTO_CHANNEL));
    
    if (!(radio->flags & CLI_RADIO_F_AUTO_CHANNEL)) {
        cli_save_set_txt(fp, &cli_param_list[CLI_PARAM_CHANNEL], radio->channel);
    }

    /* power */
    if (radio->power != CLI_AUTO_POWER) {
        if (radio->power == CLI_MAX_POWER)
            strcpy(buf, "full");
        else if (radio->power == CLI_MIN_POWER)
            strcpy(buf, "min");
        else
            sprintf(buf, "%d", radio->power);

        cli_save_set_txt(fp, &cli_param_list[CLI_PARAM_POWER], buf);
    }

	/* repeater */
	if (radio->flags & CLI_RADIO_F_REPEATER)
		cli_save_set_boolean(fp, &cli_param_list[CLI_PARAM_REPEATER_FLAG], 1);
	
    /* TBD: wmm queue settings */
    
    fclose(fp);
    
    /* bss settings */
    for (i = 0; i < MAX_BSS_NUM; i++) {
        cli_save_bss(unit, i);
    }
    
    return 0;
}

/* This function generates the scripts to configure overall system */
static int cli_save_wlan(void)
{
    FILE *fp;
    int i = 0;
    char buf[128];

    sprintf(cli_filename, "%s", WLAN_CFG_FILE);
    sprintf(cli_backupname, "%s", WLAN_CFG_BACKUP);

    fp = cli_fopen();
    cli_fputs(cli_header, fp);

    /* if it's in dual ap mode */
    cli_save_set_boolean(fp, &cli_param_list[CLI_PARAM_DUALAP],
                         ((apcfg->num_radio == 2) ? 1 : 0));


    /* if vlan is enabled */
    if (cli_vlan_enabled)
        cli_save_set_boolean(fp, &cli_param_list[CLI_PARAM_VLAN], cli_vlan_enabled);
    
    /* set country code if necessary */
    if (apcfg->cc) {
        char *name = cli_get_countryname(apcfg->cc);
        if (name)
            cli_save_set_txt(fp, &cli_param_list[CLI_PARAM_COUNTRY], name);
    }
    

    cli_save_set_ip(fp, &cli_param_list[CLI_PARAM_IPADDR], &apcfg->ipaddr);    
    cli_save_set_ip(fp, &cli_param_list[CLI_PARAM_IPMASK], &apcfg->ipmask);
    
    if (!cli_vlan_enabled) { 
        /* write out radius information */
        cli_save_set_ip(fp, &cli_param_list[CLI_PARAM_RADIUSNAME], &apcfg->radius_name);
        cli_save_set_number(fp, &cli_param_list[CLI_PARAM_RADIUSPORT], apcfg->radius_port);
        sprintf(buf, "\"%s\"", apcfg->radius_secret);
        cli_save_set_txt(fp, &cli_param_list[CLI_PARAM_RADIUSSECRET], buf);
        cli_fputs("\n", fp);
    }
    /* export COUNTRY_CODE */
    if (apcfg->cc) {
        sprintf(buf, "export COUNTRY_CODE=%d\n", apcfg->cc);
        cli_fputs(buf, fp);
    }

    fclose(fp);

    /* Radio settings */
    for (i = 0; i < apcfg->num_radio; i++) {
        cli_save_radio(i);
    }
    return 0;
}

static int cli_save_config(int argc, char **argv)
{
	struct jmploc *volatile savehandler;
	struct jmploc jmploc;
	int e;
    
    /* set up exception handling */
	if ((e = setjmp(jmploc.loc))) {
		goto savedone;
    }
	savehandler = handler;
	handler = &jmploc;

    /* save wlan settings */
    cli_save_wlan();
    
    /* clear any exception */
    e = 0;
    
savedone:
    if (e == CLI_FILE_EXCEPTION) {
        printf("Error when writing %s!\n", cli_filename);
    }
    
    /* restore old exception handler */
    handler = savehandler;
	return e;
}

static int cli_config_cmd(int argc, char **argv)
{
    int unit = current_radio_index;
    int bss = current_bss_index;
    int switch_wlan = 0;
    int switch_bss = 0;
    int i, k;
    int rc = 0;

    if (argc != 3 && argc != 5)
        goto bad;

    i = 1;
    while (i < argc) {
        if (argv[i+1]) {
            if (strcmp(argv[i+1], "0") == 0)
                k = 0;
            else {
                k = atoi(argv[i+1]);
                if (!k)
                    goto bad;
            }
        }
        else
            goto bad;

        if (strcasecmp(argv[i], "wlan") == 0) {
            switch_wlan = 1;
            unit = k;
        }
        else if (strcasecmp(argv[i], "bss") == 0) {
            switch_bss = 1;
            bss = k;
        }
        else
            goto bad;

        i += 2;
    }

    if ((unit >= apcfg->num_radio) ||
        (bss >= MAX_BSS_NUM))
        goto bad;

    current_radio_index = unit;
    current_radio = &apcfg->radio_array[unit];

    if ((bss >= 0) && (current_radio->bss_array[bss].flags & CLI_BSS_F_FREE)) {
        /* automatically set bss to -1 */
        bss = CLI_INVALID_INDEX;
        rc = (switch_wlan && !switch_bss) ? 0 : 1;
    }
    
    current_bss_index = bss;
    if (bss == CLI_INVALID_INDEX)
        current_bss = NULL;
    else
        current_bss = &current_radio->bss_array[bss];
    if (switch_wlan || switch_bss) {
        if (current_bss) 
            current_vlan = current_bss->vlan_ip;
        else 
            current_vlan = NULL;
    }
    cli_set_prompt(unit, bss);
    return rc;
    
bad:
    printf("config: %s\n", invalid_txt);
    return 1;
}

/*
 * CLI get/set parameter entry point
 */
static int cli_param_cmd(int argc, char **argv, int cmd)
{
    int i, j, rc;
    struct cli_cmd_table *table;
    struct cli_param *param;

    if (argc == 1 || !argv[1]) {
        goto fall_through;
    }
    
    /* search cmd table for supported command */
    for (j = 0; j < CLI_NUM_PARAM_TABLE; j++) {
        table = &cli_param_table[j];
        
        for (i = 0; i < table->count; i++) {
            param = &table->list[i];
            if (strcmp(argv[1], param->name) == 0) {
                /* find a matching */
                goto found;
            }
        }
    }

    goto fall_through;
    
found:
    argc --;
    argv ++;
    rc = 0;

    /* check for valid radio and bss pointers */
    if ((param->index & CLI_F_RADIO && current_radio == NULL) ||
        (param->index & CLI_F_BSS && current_bss == NULL)) {
        goto invalid;
    }
    
    if (cmd == CLI_CMD_GET) {
        if (param->get == NULL)
            goto done;

        if (param->index & (CLI_F_PROBE_ONLY |
                            CLI_FORMAT_SELF |
                            CLI_FORMAT_NULL)) {
            rc = param->get(param, i, NULL);
        }
        else if (param->index & CLI_FORMAT_INTEGER) {
            printf("%s: %d\n", param->name, param->get(param, i, NULL));
        }
        else if (param->index & CLI_FORMAT_BOOLEAN) {
            printf("%s: %s\n", param->name, ((param->get(param, i, NULL)) ?
                                             enable_txt : disable_txt));
        }
        else if (param->index & CLI_FORMAT_TEXT) {
            char s[128];
            param->get(param, i, &s);
            printf("%s: %s\n", param->name, s);
        }
        else if (param->index & CLI_FORMAT_IP) {
            struct in_addr addr;
            char dst[32];
            
            param->get(param, i, &addr);
            inet_ntop(AF_INET, &addr, dst, 32);
            printf("%s: %s\n", param->name, dst);
        }
    }
    else if (cmd == CLI_CMD_SET) {
        if (param->set == NULL)
            goto done;
        
        /* bring down the interface if necessary */
        if (param->index & CLI_F_STOPIF && cli_driver_check) {
            if (cli_vap_enabled && param->index & CLI_F_RADIO) {
                /*
                 * VAP enabled and it's a command for radio,
                 * we have to bring down every BSS of this radio.
                 */
                struct ap_bss *bss;
                
                for (j = 0; j < MAX_BSS_NUM; j++) {
                    bss = &current_radio->bss_array[j];

                    if (!(bss->flags & CLI_BSS_F_FREE)) {
                        cli_iface_updown(bss->iface, 0);
                    }
                }
            }
            else if (param->index & (CLI_F_BSS | CLI_F_RADIO)) {
                /* just bring down the current bss */
                cli_iface_updown(current_bss->iface, 0);
            }
        }

        /* check parameter format */
        if (argc == 1 && !(param->index & CLI_FORMAT_NULL)) {
            goto invalid;
        }

        if (param->index & CLI_FORMAT_INTEGER) {
            int value;
            if (strcmp(argv[1], "0") == 0) {
                value = 0;
            }
            else {
                value = atoi(argv[1]);
                if (!value)
                    goto invalid;
            }

            rc = param->set(param, i, (void *)value);
        }
        else if (param->index & CLI_FORMAT_BOOLEAN) {
            int enable;
            if (strcasecmp(argv[1], enable_txt) == 0) {
                enable = 1;
            }
            else if (strcasecmp(argv[1], disable_txt) == 0) {
                enable = 0;
            }
            else
                goto invalid;
            
            rc = param->set(param, i, (void *)enable);
        }
        else if (param->index & CLI_FORMAT_TEXT) {
            rc = param->set(param, i, (void *)argv[1]);
        }
        else if (param->index & CLI_FORMAT_IP) {
            struct in_addr ip;
    
            if (inet_pton(AF_INET, argv[1], &ip) <= 0)
                goto invalid;

            rc = param->set(param, i, (void *)&ip);
        }            
        else if (param->index & CLI_FORMAT_SELF) {
            rc = param->set(param, i, argv);
        }
        else {
            rc = param->set(param, i, NULL); /* NULL format */
        }
    }
    else {
        printf("CLI: Unknown CLI command\n");
        return 1;
    }

done:    
    return rc;

invalid:
    printf("%s: %s\n", param->name, invalid_txt);
    return 1;

fall_through:
    /* something wrong with shell parsing, silently fall through */
    return 0xff;
}

/* Run configuration scripts to setup internal data structures */
static int cli_run_config(FILE *fp)
{
    char *p1, *p2;
    int argc;
    char *argv[16];
    char line[256];

    fseek(fp, 0, SEEK_SET);
    while (fgets(line, 256, fp)) {
        argc = 0;
        memset(argv, 0, sizeof(argv));

        p1 = p2 = line;

        if (*p1 == '\n' ||      /* empty line */
            *p1 == '#')         /* comment */
            continue;

        p2 = index(line, '\n'); /* replace newline with NULL */
        if (p2)
            *p2 = '\0';

        while(*p1 == ' ')     /* find the first non-space character */
            p1++;
        
        if (*p1 == '\0')        /* empty line */
            continue;

        while ((p2 = index(p1, ' '))) {
            if (*p1 == '\"') {
                p2 = ++p1;

                /* search the other quote */
                while (*p2 != '\"') {
                    if (*p2 == '\0') /* missing the other half quote */
                        break;
                    p2++;
                }
            }
            
            argv[argc] = p1;
            *p2++ = '\0';

            while(*p2 == ' ')
                p2++;
            
            if (*p2 == '\0')
                break;
            
            p1 = p2;
            argc++;
            if (argc >= 15)
                break;
        }

        /* some left over */
        if (*p1 == '\"') {
            p1++;
            p2 = index(p1, '\"');
            if (p2)
                *p2 = '\0';
        }
        
        argv[argc++] = p1;

        /* execute this command */
        if (strcmp(argv[0], "set") == 0) {
            cli_param_cmd(argc, argv, CLI_CMD_SET);
        }
        else if(strcmp(argv[0], "config") == 0) {
            cli_config_cmd(argc, argv);
        }
        else if(strcmp(argv[0], "add") == 0) {
            cli_add_cmd(argc, argv);
        }
        else if(strcmp(argv[0], "del") == 0) {
            cli_del_cmd(argc, argv);
        }
    }

    return 0;
}

/* Run configuration files */
static int cli_run_config_dir(char *config_dir)
{
    FILE *fp;
    int i, j;
    char filename[32];

    /* wlanconfig */
    sprintf(filename, "%s/%s", config_dir, WLAN_CFG_FILENAME);
    fp = fopen(filename, "r");
    if (fp) {
        cli_run_config(fp);
        fclose(fp);
    }
    else
        return 1;

    /* radio config */
    for (i = 0; i < apcfg->num_radio; i++) {
        sprintf(filename, "%s/%s.%d", config_dir, WLAN_CFG_FILENAME, i);
        fp = fopen(filename, "r");
        if (fp) {
            cli_run_config(fp);
            fclose(fp);
        }
        else
            return 1;

        /* if radio configuration is all right, BSS should be created by now */
        for (j = 0; j < MAX_BSS_NUM; j++) {
            if (!(apcfg->radio_array[i].bss_array[j].flags & CLI_BSS_F_FREE)) {
                sprintf(filename, "%s/%s.%d.%d", config_dir, WLAN_CFG_FILENAME, i, j);
                fp = fopen(filename, "r");
                if (fp) {
                    cli_run_config(fp);
                    fclose(fp);
                }
                else
                    return 1;
            }
        }
    }
    
    return 0;
}

/* Reset current radio and bss pointers */
static void cli_reset_pointers(void)
{
    struct ap_bss *bss;
    int i;
        
    /* set current radio and bss */
    current_radio = &apcfg->radio_array[0];
    current_radio_index = 0;
    current_bss = NULL;
    current_bss_index = CLI_INVALID_INDEX;

    if (current_radio->num_bss) {
        /* find the first BSS */
        for (i = 0; i < MAX_BSS_NUM; i++) {
            bss = &current_radio->bss_array[i];
            if (!(bss->flags & CLI_BSS_F_FREE)) {
                current_bss_index = i;
                current_bss = bss;
                break;
            }
        }
    }
    cli_set_prompt(current_radio_index, current_bss_index);
}

/* Initialize CLI data structures */
static int cli_init(void)
{
    int rc;

    apcfg = &gApCfg;
    
    cli_set_default();

    cli_driver_check = 0;       /* turn off driver checking flag */
    
    /*
     * run configuration files to setup
     * internal data structures
     */
    rc = cli_run_config_dir(WLAN_CFG_PATH);

    if (rc) {
        /* something wrong with config path, revert to default path */
        rc = cli_run_config_dir(WLAN_CFG_DFT_PATH);
    }

    if (rc) {
        printf("Invalid configuration path.\n");
    }
    
    cli_driver_check = 1;       /* turn on driver checking flag */

    cli_reset_pointers();       /* reset current radio and bss pointers */
    
    athcli_debug_flag = 0;
    
    return 0;
}

/* Set / unset debug tracing capability for CLI */
static void
ath_debug_cli(const char *char_val)
{
	int new_flag;
	int debug_enable;
	int debug_disable;
	int debug_na;
	
	new_flag = atoi(char_val);
	debug_enable = (!athcli_debug_flag && (new_flag == 1));
	debug_disable = ((athcli_debug_flag == 1) && !new_flag);
	debug_na = (!debug_enable && !debug_disable);
	
	if (debug_na) 
		return;
		
	if (debug_enable) {
		/* Open trace file */
		if (cli_debug_file) {
			if (!freopen(clidebug_trace, "a", cli_debug_file)) {
				fprintf(stderr, "Can't re-open %s\n",
					clidebug_trace);
				return;
			}
		} else {
			cli_debug_file = fopen(clidebug_trace, "a");
			if (!cli_debug_file) {
				fprintf(stderr, "Can't open %s\n", 
					clidebug_trace);
				return;
			}
		}
		athcli_debug_flag = 1;
		setlinebuf(cli_debug_file);
		fputs("\nCLI Tracing started.\n", cli_debug_file);
	}
	
	if (debug_disable) {
		if (cli_debug_file) {
			fputs("CLI Tracing stopped.\n",
			      cli_debug_file);
			fflush(cli_debug_file);
			fclose(cli_debug_file);
			cli_debug_file = NULL;
		}
		athcli_debug_flag = 0;

	}
	
}

static void
cli_trace(const char *fmt, ...)
{
	va_list va;

	if (athcli_debug_flag != 1)
		return;
	va_start(va, fmt);
	(void) vfprintf(cli_debug_file, fmt, va);
	va_end(va);
	fflush(cli_debug_file);
}
	
