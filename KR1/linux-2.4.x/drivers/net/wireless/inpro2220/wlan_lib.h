/*
** $Id: wlan_lib.h,v 1.1 2004/02/24 05:06:48 cfliu Exp $
*/

/******************************************************************************
Copyright (c) 2002-2003 Inprocomm, Inc.

All rights reserved. Copying, compilation, modification, distribution
or any other use whatsoever of this material is strictly prohibited
except in accordance with a Software License Agreement with Inprocomm, Inc.
******************************************************************************/

#ifndef __WLAN_LIB_H
#define __WLAN_LIB_H

/************************************************************************
*                     C O M P I L E R   F L A G S
*************************************************************************
*/

/************************************************************************
*                E X T E R N A L   R E F E R E N C E S
*************************************************************************
*/

/************************************************************************
*                          C O N S T A N T S
*************************************************************************
*/
#define PARAM_MAX_SSID_LEN              32
#define PARAM_MAX_PHRASE_LEN            64
#define PARAM_MAX_KEY_LEN               32
#define PARAM_MAC_ADDR_LEN              6

/* Define regulation domain for function param_set_regDomain() */
#define MIB_REG_DOMAIN_FCC              0x10    /* FCC (US) */
#define MIB_REG_DOMAIN_IC               0x20    /* IC or DOC (Canada) */
#define MIB_REG_DOMAIN_ETSI             0x30    /* ETSI (Europe) */
#define MIB_REG_DOMAIN_SPAIN            0x31    /* Spain */
#define MIB_REG_DOMAIN_FRANCE           0x32    /* France */
#define MIB_REG_DOMAIN_JAPAN_1          0x40    /* MPHPT (Japan) Channel */
#define MIB_REG_DOMAIN_JAPAN_2          0x41    /* MPHPT (Japan) Channel */

/* Define RF testing mode for function param_set_rftestMode() */
#define RFTEST_MODE_STOP                0
#define RFTEST_MODE_RX                  1
#define RFTEST_MODE_TONE                2
#define RFTEST_MODE_BURST               3

typedef enum {
        Legacy_Associated,
        TOTAL_AUTH_STAGE
} param_auth_stage_t;

/* Define authMode for function param_set_authmode() */
typedef enum {
        AuthMode_OPEN     = 0,
        AuthMode_OPEN_1X  = 1,
        AuthMode_SHARED   = 2,
        AuthMode_SWITCH   = 3,
        AuthMode_WPA      = 4,
        AuthMode_WPAPSK   = 5

} param_authMode_t;

/* Define cipherMode for functions param_set_cipherMode() and
 * param_set_wds_cipherMode(). Note: WDS only supports WEP and CCMP.
 */
typedef enum {
        CipherMode_NONE   = 0,
        CipherMode_WEP40  = 1,
        CipherMode_TKIP   = 2,
        CipherMode_WRAP   = 3,
        CipherMode_CCMP   = 4,
        CipherMode_WEP104 = 5,
        CipherMode_WEP128 = 6

} param_cipherMode_t;

/* Define bit-map of basic rate set for function param_set_basicRateSet()
 * Default: BasicRate_1M | BasicRate_2M | BasicRate_5_5M | BasicRate_11M
 */
typedef enum {
        BasicRate_1M    = 0x0001,
        BasicRate_2M    = 0x0002,
        BasicRate_5_5M  = 0x0004,
        BasicRate_11M   = 0x0008,
        BasicRate_6M    = 0x0010,
        BasicRate_9M    = 0x0020,
        BasicRate_12M   = 0x0040,
        BasicRate_18M   = 0x0080,
        BasicRate_24M   = 0x0100,
        BasicRate_36M   = 0x0200,
        BasicRate_48M   = 0x0400,
        BasicRate_54M   = 0x0800

} param_basicRate_t;

/************************************************************************
*                         D A T A   T Y P E S
*************************************************************************
*/
typedef struct param_ssid_s {
        char            ssid[PARAM_MAX_SSID_LEN];
        int             len;
} param_ssid_t;

typedef struct param_cipher_key_s {
        int             len;
        unsigned char   key[PARAM_MAX_KEY_LEN];
} param_cipher_key_t;


typedef unsigned char   param_mac_addr_t[PARAM_MAC_ADDR_LEN];
typedef unsigned char   param_passphrase_t[PARAM_MAX_PHRASE_LEN + 1];


typedef union param_req {
/* Basic AP */
        param_ssid_t            ssid;           /* refer to param_ssid_t */
        int                     bcnPeriod;      /* 20 ~ 100 */
        int                     dtimPeriod;     /* 1 ~ 5 */
        int                     setChannel;     /* 1 ~ 14 based on regulation domain */
        int                     curChannel;
        int                     rtsThreshold;   /* 0 ~ 2347 */
        int                     fragThreshold;  /* 0 ~ 2346 */

        int                     basicRateSet;   /* refer to param_basicRate_t */
        int                     basicRateMode;  /* 0: b+g, 1: g only */

/* Extra function */
        int                     bcastSSID;      /* 0: disable, 1: enable */
        int                     ctsProtect;     /* 0:auto, 1: always, 2: none */
        int                     dataRate;       /* 1Mbps will be 1000000 */
        int                     burstMode;      /* 0: disable, 1: enable */
        int                     noRelay;        /* 0: disable, 1: enable */
        param_mac_addr_t        killmac;

/* Security */
        param_authMode_t        authmode;       /* refer to param_authMode_t */
        param_cipherMode_t      cipherMode;     /* refer to param_cipherMode_t */
        int                     wepDefaultKeyId;/* 1, 2, 3, 4 */
        int                     wepKeyId;       /* 1, 2, 3, 4 */
        param_cipher_key_t      wepKey;
        param_passphrase_t      passphrase;     /* string */

/* WDS */
        int                     noStation;      /* 0: disable, 1: enable */
        int                     wds_rateMode;   /* 0: 802.11b, 1: 802.11g */
        param_mac_addr_t        wds_addmac;
        param_mac_addr_t        wds_delmac;
        param_cipher_key_t      wds_cipherKey;
        param_cipherMode_t      wds_cipherMode; /* refer to param_cipherMode_t */

/* Regulation and EMI */
        int                     regDomain;      /* refer to MIB_REG_DOMAIN_FCC */
        int                     rftestMode;     /* 0:stop, 1:rx, 2:tone, 3:burst */
        int                     rftestPower;    /* 0 ~ 255 */

} param_req_t;

/*
 * Structure used by wlan to store the log.
 */
struct net_stat
{
        unsigned long   txunicastframes;
        unsigned long   txmulticastframes;
        unsigned long   txfragments;
        unsigned long   txsingleretryframes;
        unsigned long   txmultipleretryframes;
        unsigned long   txretrylimitexceeded;
        unsigned long   txdiscards;
        unsigned long   txbytes;
        unsigned long   rxmulticastframes;
        unsigned long   rxbytes;
        unsigned long   rxfragments	;
        unsigned long   rxfcserrors;
        unsigned long   rxdiscardsnobuffer;
        unsigned long   rxdiscardswepundecr;
};

typedef struct sta_status_ {
        short           aid;         /* STA's unique AID (1 .. 2007) or 0 if not yet assigned */
        unsigned char   adAddr[6];      /* address of associated station */
        unsigned char   auth_state;  /* station status */
        unsigned char   ps_state;    /* station power save status */
        unsigned long assoc_time;
} sta_status_t;
/************************************************************************
*                        P U B L I C   D A T A
*************************************************************************
*/

/* A string shows the version of IPN2220.o */
extern char *ipn2220_drv_ver;

/************************************************************************
*                             M A C R O S
*************************************************************************
*/

/************************************************************************
*              F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/

/* IPN2220 HW initialization */
extern int i2220ap_pci_probe(kal_net_device_t *dev);

/* Restore IPN2220 and free memory */
extern void i2220ap_remove_pci(kal_net_device_t *dev);

/* Create private data structure */
extern void *i2220ap_priv_create(kal_pci_dev_t *pdev,
                        kal_net_device_t *netdev, unsigned long membase);

/* Free private data structure */
extern void i2220ap_priv_destroy(kal_net_device_t *dev);

/* Change MAC address */
extern int i2220ap_set_mac_address(kal_net_device_t *dev, char *new_mac);

/* Packet transmission */
extern int i2220ap_hard_start_xmit(kal_net_device_t *netdev, kal_skb_t *skb);

/* IPN2220 interrupt routine */
extern int i2220hw_interrupt(void *dev_priv);

/* Defer interrupt routine */
extern void do_isr_tasklet(unsigned long obj);

/* Periodic time out checking routine */
void do_timer_check(void);

#if WLAN_INCLUDE_PROC
extern void init_2220_proc(kal_net_device_t *dev, char *dev_name);
extern void remove_2220_proc(kal_net_device_t *dev, char *dev_name);
#endif

/*========================================================
 * Utilitis for WDS management
 *========================================================*/
extern int is_main_device(kal_net_device_t *dev);
extern int is_wds_by_mac(kal_net_device_t *dev, unsigned char *addr);

/*========================================================
 * Functions of get/set parameter (IOCTL config)
 *========================================================*/

/* Functions to Setting Parameter */
/* Basic AP */
extern int param_set_ssid(kal_net_device_t *dev, param_ssid_t *data);
extern int param_set_bcnPeriod(kal_net_device_t *dev, int data);
extern int param_set_dtimPeriod(kal_net_device_t *dev, int data);
extern int param_set_setChannel(kal_net_device_t *dev, int data);
extern int param_set_rts(kal_net_device_t *dev, int data);
extern int param_set_frag(kal_net_device_t *dev, int data);
extern int param_set_basicRateSet(kal_net_device_t *dev, int data);
extern int param_set_basicRateMode(kal_net_device_t *dev, int data);

/* Extra function */
extern int param_set_bcastSSID(kal_net_device_t *dev, int data);
extern int param_set_ctsProtect(kal_net_device_t *dev, int data);
extern int param_set_dataRate(kal_net_device_t *dev, int data);
extern int param_set_burstMode(kal_net_device_t *dev, int data);
extern int param_set_noRelay(kal_net_device_t *dev, int data);
extern int param_set_killmac(kal_net_device_t *dev, param_mac_addr_t *data);

/* Security */
extern int param_set_authmode(kal_net_device_t *dev, param_authMode_t data);
extern int param_set_cipherMode(kal_net_device_t *dev, param_cipherMode_t data);
extern int param_set_wepDefaultKeyId(kal_net_device_t *dev, int data);
extern int param_set_wepKeyId(kal_net_device_t *dev, int data);
extern int param_set_wepKey(kal_net_device_t *dev, param_cipher_key_t *data);
extern int param_set_passphrase(kal_net_device_t *dev, param_passphrase_t *data);

/* WDS */
extern int param_set_noStation(kal_net_device_t *dev, int data);
extern int param_set_wds_rateMode(kal_net_device_t *dev, int data);
extern int param_set_wds_cipherKey(kal_net_device_t *dev, param_cipher_key_t *data);
extern int param_set_wds_cipherMode(kal_net_device_t *dev, param_cipherMode_t data);
extern kal_net_device_t *param_del_wdslink2(kal_net_device_t *main_dev,
        unsigned char *remote_addr);
extern int param_add_wdslink2(kal_net_device_t *main_dev,
        unsigned char *remote_addr, kal_net_device_t *new_dev, int mode,
        int auth, char *remote_bssid);

/* Regulation and EMI */
extern int param_set_regDomain(kal_net_device_t *dev, int data);
extern int param_set_rftestMode(kal_net_device_t *dev, int data);
extern int param_set_rftestPower(kal_net_device_t *dev, int data);

/* Functions to Getting Parameter */
extern void param_get_curChannel(kal_net_device_t *dev, int *data);

extern int param_get_Statistic(kal_net_device_t *dev, struct net_stat *out_stat);
extern int param_get_StaList(kal_net_device_t *dev, int size_of_buffer,
        sta_status_t *staS);

/************************************************************************
*           I N L I N E   F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/

#endif  /* __WLAN_LIB_H */

