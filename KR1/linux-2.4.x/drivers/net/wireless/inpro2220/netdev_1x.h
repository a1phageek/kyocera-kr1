/*
** $Id: netdev_1x.h,v 1.1 2004/04/08 12:23:36 cfliu Exp $
*/

/******************************************************************************
Copyright (c) 2000-2002 Integrated Programmable Communications, Inc.

All rights reserved. Copying, compilation, modification, distribution
or any other use whatsoever of this material is strictly prohibited
except in accordance with a Software License Agreement with
Integrated Programmable Communications, Inc.
******************************************************************************/

#ifndef _IPN2120_PLUG_IN_FOR_802_1X_
#define _IPN2120_PLUG_IN_FOR_802_1X_

//#include "plug_hostapd.h"

#define PLG_SUCCESSFUL 0

/* kernel < 2.4.0 */
//#define wlan_set_dev_name(dev, pos) (dev)->name = (char *) (pos)
/* kernel > 2.4.0 */
#define wlan_set_dev_name(dev, pos) do { } while (0)

typedef struct pluginfo_ pluginfo_t;

struct pluginfo_ {
        char   name[IFNAMSIZ];
        struct module *hw_module;

        int ieee_802_1x; /* is IEEE 802.1X used */

        kal_net_device_t *dev_1x;
        kal_net_device_t *apdev;
        //struct net_device_stats stats_1x;

#if WLAN_INCLUDE_PROC
        struct proc_dir_entry *proc;
#endif

#ifdef WIRELESS_EXT
        struct iw_statistics wstats;
#if WIRELESS_EXT > 13
        unsigned long scan_timestamp; /* Time started to scan */
#endif /* WIRELESS_EXT > 13 */
#endif /* WIRELESS_EXT */
};
pluginfo_t* init_local_data(char *devname);
void        free_local_data(pluginfo_t *local);
extern int  wlan_1x_request(kal_net_device_t *dev, kal_skb_t *skb);
#endif
