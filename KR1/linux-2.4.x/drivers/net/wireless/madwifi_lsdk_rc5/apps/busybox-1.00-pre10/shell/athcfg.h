/*
 * $Id: //depot/sw/releases/linuxsrc/apps/busybox-1.00-pre10/shell/athcfg.h#13 $
 */
#ifndef _ATHCFG_H
#define _ATHCFG_H

#define MAX_SSID        32
#define MAX_RADIO_NUM   2
#define MAX_BSS_NUM     4
#define MAX_VLAN_NUM    4

#define MIN_PASSPHRASE  8
#define MAX_PASSPHRASE  64

#define MAX_KEYTABLE_SIZE       4
#define MAX_KEY_LEN             32
#define JS_MAX_PASSWORD         32
#define KEY_ENTRY_METHOD_HEX    0
#define KEY_ENTRY_METHOD_ASCII  1
/* Static WEP key */
struct ap_wep_key {
    int key_length;
    char key_material[MAX_KEY_LEN+1];
};

#define CLI_BEST_RATE   -1

struct acl_entry {
	int used;					/* whether it's free or not */
	char macaddr[20];			/* store mac address as string */
};
#define CLI_INITIAL_ACL_TABLE_SIZE 8
#define CLI_ACL_TABLE_SIZE_INCREMENT 4

#define MAX_RADIUS_NAME             64
#define MAX_SECRET_LENGTH           128

/* VLAN IP info */
struct vlan_ip {
    int             vlan_tag;
    int             ref_count;
    struct in_addr  ipaddr;
    struct in_addr  ipmask;
    int             ip_set;
    struct in_addr  radius_name;
    int             radius_port;
    char            radius_secret[MAX_SECRET_LENGTH+1];
};

/* AP BSS Configuration */
struct ap_bss {
    char          iface[IFNAMSIZ + 1]; /* interface name for this bss */    

    int           flags;

    char          ssid[MAX_SSID + 1];

    int           opmode;
    int           pvid;
    int           rate;
    int           wlanstate;
    int           authType;
    int           groupKeyUpdateInterval;
    int           cipher;
    int           defaultKey;
    struct ap_wep_key *keytable;
    int           keyEntryMethod;

    int           acl_mode;
    int           acl_size;
    struct acl_entry *acl_table;
	
    int           priv_params[CLI_PRIV_BSS_MAX_PARAM];
    
    char          passphrase[MAX_PASSPHRASE + 1];
    struct vlan_ip *vlan_ip;
    int           preauth;
    int		  js_enabled;
    char	  js_passwd[JS_MAX_PASSWORD + 1];
};

#define CLI_BSS_F_FREE              0x01
#define CLI_BSS_F_SSID_SUPRESS      0x02
#define CLI_BSS_F_SECURE            0x04
#define CLI_BSS_F_HASHED_KEY        0x08
#define CLI_BSS_F_VAP_FORWARDING    0x10

#define MAX_CHANNEL_LENGTH      15

#define CLI_AUTO_POWER      0xff
#define CLI_MAX_POWER       100
#define CLI_MIN_POWER       -100

/* AP Radio Configuration */
struct ap_radio {
    char        *iface;                     /* interface name for this radio */
    char        vapctl_iface[IFNAMSIZ + 1];  /* control interface to create VAPs. */

    int         flags;
    int         wep_index;       /* which BSS is doing static WEP */
    int         num_bss;
    
    int         wmode;
    int         power;
    int         priv_params[CLI_PRIV_RADIO_MAX_PARAM];
    char        channel[MAX_CHANNEL_LENGTH+1];

    struct ap_wep_key keytable[MAX_KEYTABLE_SIZE];
    
    struct ap_bss bss_array[MAX_BSS_NUM];
};

#define CLI_RADIO_F_AUTO_CHANNEL    0x01
#define CLI_RADIO_F_AUTO_RATE       0x02
#define CLI_RADIO_F_REPEATER        0x04

/* AP System Configuration */
struct ap_cfg {
    int             num_radio;
    struct ap_radio radio_array[MAX_RADIO_NUM];
     
    int             cc;         /* country code */

    struct in_addr  ipaddr;
    struct in_addr  ipmask;
    
    struct in_addr  radius_name;
    int             radius_port;
    char            radius_secret[MAX_SECRET_LENGTH+1];
    struct vlan_ip  vlan[MAX_RADIO_NUM * MAX_VLAN_NUM];
};


/* Some default settings */
#define DEFAULT_RADIO_IDX       0
#define DEFAULT_BSS_IDX         0
#define DEFAULT_RADIO_NUM       2
#define DEFAULT_BSS_NUM         1
#define DEFAULT_VAPCTL_IFACE    "wifi"
#define DEFAULT_RADIO_IFACE     "ath"
#define DEFAULT_BSS_IFACE       "ath"
#define DEFAULT_SSID            "Atheros Wireless Network"
#define DEFAULT_GROUPKEY_UPDATE 600 /* 10 minutes */
#define DEFAULT_RADIUS_NAME     "192.168.0.10"
#define DEFAULT_RADIUS_SECRET   "Atheros"
#define DEFAULT_RADIUS_PORT     1812
#define DEFAULT_IP_ADDR         "192.168.0.1"
#define DEFAULT_IP_MASK         "255.255.255.0"

/* VxWorks ABOLT compatibility */
enum {
    ABOLT_TURBO          = 0x01,      /* Legacy Turbo G */
    ABOLT_TURBO_PRIME    = 0x02,      /* Turbo Prime */
    ABOLT_COMPRESSION    = 0x04,      /* Compression */
    ABOLT_FAST_FRAME     = 0x08,      /* Fast Frames */
    ABOLT_BURST          = 0x10,      /* Bursting */
    ABOLT_WME_ELE        = 0x20,      /* WME based cwmin/max/burst tuning */
    ABOLT_XR             = 0x40,      /* XR */
    ABOLT_FRIENDLY_TURBO = 0x80,  /* Friendly turbo that switches out based on adjacent non-turbo traffic */
    ABOLT_ATH_ADVCAP  = (
                          ABOLT_TURBO_PRIME |
                          ABOLT_COMPRESSION |
                          ABOLT_FAST_FRAME  |
                          ABOLT_XR          |
                          ABOLT_FRIENDLY_TURBO
                        ),         /* Capabilties causes negotiation through AdvCap */
};

/* Configuration files and their location */
#define WLAN_CFG_PATH       "/etc/wlan"
#define WLAN_CFG_DFT_PATH   WLAN_CFG_PATH "/default"

#define WLAN_CFG_FILENAME   "wlanconfig"

#define WLAN_CFG_FILE       WLAN_CFG_PATH "/" WLAN_CFG_FILENAME
#define WLAN_CFG_BACKUP     WLAN_CFG_PATH "/" WLAN_CFG_FILENAME ".bak"
#define HOSTAPD_CFG_FILE    WLAN_CFG_PATH "/hostapd.conf"
#define HOSTAPD_CFG_BACKUP  WLAN_CFG_PATH "/hostapd.bak"

#endif /* end of _ATHCFG_H */
