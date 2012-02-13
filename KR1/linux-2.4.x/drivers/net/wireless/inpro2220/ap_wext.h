/*
** $Id: ap_wext.h,v 1.2 2004/04/08 12:23:19 cfliu Exp $
*/

/******************************************************************************
Copyright (c) 2002-2003 Inprocomm, Inc.

All rights reserved. Copying, compilation, modification, distribution
or any other use whatsoever of this material is strictly prohibited
except in accordance with a Software License Agreement with Inprocomm, Inc.
******************************************************************************/

#ifndef _AP_WEXT_H
#define _AP_WEXT_H

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
#if WIRELESS_EXT >= 12
    /* New wireless extensions API - SET/GET convention (even ioctl numbers are
     * root only)
     */
    #define IOCTL_SET_INT       (SIOCIWFIRSTPRIV + 0)
    #define IOCTL_GET_INT       (SIOCIWFIRSTPRIV + 1)
    #define IOCTL_SET_ADDRESS   (SIOCIWFIRSTPRIV + 2)
    #define IOCTL_GET_ADDRESS   (SIOCIWFIRSTPRIV + 3)
    #define IOCTL_SET_STR       (SIOCIWFIRSTPRIV + 4)
    #define IOCTL_GET_STR       (SIOCIWFIRSTPRIV + 5)
    #define IOCTL_SET_KEY       (SIOCIWFIRSTPRIV + 6)
    #define IOCTL_GET_KEY       (SIOCIWFIRSTPRIV + 7)
    #define IOCTL_SET_STRUCT    (SIOCIWFIRSTPRIV + 8)
    #define IOCTL_GET_STRUCT    (SIOCIWFIRSTPRIV + 9)

    #define SIOCIWFIRSTPLUGIN   (SIOCIWFIRSTPRIV + 16)

    #define FIRSTPISUBCMD       100
    #define THIS_IS_A_SUBCMD    256

#endif /* WIRELESS_EXT >= 12 */

/************************************************************************
*                         D A T A   T Y P E S
*************************************************************************
*/
struct iw_request_info {
        unsigned int    cmd;            /* Wireless Extension command */
        unsigned int    flags;          /* More to come ;-) */
};

/************************************************************************
*                        P U B L I C   D A T A
*************************************************************************
*/

/************************************************************************
*                             M A C R O S
*************************************************************************
*/

/************************************************************************
*              F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/
/* wireless extensions' ioctls */
int wext_support_ioctl(struct net_device *dev, struct iwreq *iwr, int cmd);

/* wireless extensions' private ioctls */
int private_support_ioctl(struct net_device *dev, struct iwreq *iwr, int cmd);
int do_ioctl_plugin(struct net_device *dev, int sub_cmd, struct iwreq *ifreq);
//int ioctl_plugin(struct net_device *dev, int sub_cmd, struct iwreq *ifreq);
/************************************************************************
*           I N L I N E   F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/

#endif /* _AP_WEXT_H */

