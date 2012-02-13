/*
** $Id: wlan_kal.h,v 1.3 2004/05/03 14:59:48 cfliu Exp $
*/

/******************************************************************************
Copyright (c) 2002-2003 Inprocomm, Inc.

All rights reserved. Copying, compilation, modification, distribution
or any other use whatsoever of this material is strictly prohibited
except in accordance with a Software License Agreement with Inprocomm, Inc.
******************************************************************************/

#ifndef __WLAN_KAL_H
#define __WLAN_KAL_H

/************************************************************************
*                     C O M P I L E R   F L A G S
*************************************************************************
*/
#define __KAL_PACK_BEGIN__      /* _Pragma("pack(1)") */
#define __KAL_PACK_END__        /* _Pragma("pack()") */
#define __KAL_ATTRIB_PACKED__   __attribute__ ((__packed__))
#define __KAL_INLINE__          static __inline__

/************************************************************************
*                E X T E R N A L   R E F E R E N C E S
*************************************************************************
*/

/*================================================================*/
/* System Includes */

#include <linux/config.h>
#include <linux/version.h>
#include <linux/init.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/rtnetlink.h>
#include <linux/timer.h>
#include <linux/list.h>
#include <linux/if_arp.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/vmalloc.h>
#include <linux/proc_fs.h>
#include <asm/io.h>
#include <asm/byteorder.h>
#include <asm/uaccess.h>
#include <asm/semaphore.h>

#include <linux/ioport.h>
#include <linux/pci.h>

/* NOTE we are using local folder for wireless.h */
#include "wireless.h"

/************************************************************************
*                          C O N S T A N T S
*************************************************************************
*/
// cefiro 2004/3/29 20:55 For iwpriv ioctl
#define WLAN_INCLUDE_PROC 1
#define WLAN_INCLUDE_WDS 1
#define SUPPORT_RSN 1

// cefiro 2004/1/14 22:04 for Inprocomm 2220AP Memory-mapped I/O
#define PCI_MAP_MEM             1


#define MAX_RX_PKT_LEN          1600 /* including encryption needed */
                                     /* the Ethernet backbone is considered */

#ifndef IFNAMSIZ
#define IFNAMSIZ                16
#endif

/* HZ must be larger than 2 (500ms) */
#ifndef HZ
#define HZ                      100  /* 10ms, basic time unit */
#endif

/* kmalloc allocation flag. Please note this flag is used in both
 * interrupt routine and general routine. In Linux, use GFP_ATOMIC.
 */
#define KAL_MALLOC_FLAG         GFP_ATOMIC

/* Define directory arg to DMA mapping routines. i.e.,
 * kal_pci_map_single() and kal_pci_unmap_single()
 */
#ifndef PCI_DMA_BIDIRECTIONAL
#define PCI_DMA_BIDIRECTIONAL   0
#define PCI_DMA_TODEVICE        1
#define PCI_DMA_FROMDEVICE      2
#define PCI_DMA_NONE            3
#endif

#if defined(CONFIG_INPRO2220_HWACCEL) && defined(CONFIG_RTL865X_MULTILAYER_BSP)
	//This is the configuration parameters required for L3/4 acceleration when running on
	//RTL865X SoC.
extern int ipn2220_totalWDS;
extern unsigned inpro2220_rtl8650extPortNum;//loopback/extension port used.
extern unsigned ipn2220_rtl8650MainLinkID;//to save the link ID allocated from glue interface
	
#endif
/************************************************************************
*                         D A T A   T Y P E S
*************************************************************************
*/
typedef struct pci_dev          kal_pci_dev_t;
typedef struct net_device       kal_net_device_t;
typedef struct sk_buff          kal_skb_t;
typedef dma_addr_t              kal_dma_addr_t;

/************************************************************************
*                        P U B L I C   D A T A
*************************************************************************
*/
#define kal_jiffies             jiffies

/************************************************************************
*                             M A C R O S
*************************************************************************
*/

/* The readl() and writel() are Linux macros, depending on kernel platforms.
 * The endian conversion is done in these two marcos.
 */
#define kal_pci_readl(membase, offset) \
        readl((UINT32)(membase) + (offset))
#define kal_pci_writel(_value, membase, offset) \
        writel(_value, (UINT32)(membase) + (offset))

/* Endian conversion of MAC Hardware intergers to/from HOST */
#define chip2host_16(n)         __le16_to_cpu(n)
#define chip2host_32(n)         __le32_to_cpu(n)
#define host2chip_16(n)         __cpu_to_le16(n)
#define host2chip_32(n)         __cpu_to_le32(n)

/* Endian conversion of network fields to/from HOST */
#define kal_ntohs(n)            ntohs(n)
#define kal_htons(n)            htons(n)


/* Pointer to private data in kal_net_device_t */
#define kal_dev_priv(dev)               (dev)->priv
#define kal_set_dev_macAddr(dev,mac)    memcpy((dev)->dev_addr,mac,6)


/* For some CPU's MMU, the D-Cache policy is write-back, rather write-through.
 * In order to maintain the data consistency between the Cache and memory,
 * we need the following wrapper functions, which handle D-Cache specifically.
 */
#define kal_pci_map_single(pcidev,mem,size,dir)       pci_map_single((pcidev),(mem),(size),dir)
#define kal_pci_unmap_single(pcidev,bus,size,dir)     pci_unmap_single((pcidev),(bus),(size),dir)

/* Mapping virtual address to bus (physical) address */
#define kal_virt_to_bus(v)                  virt_to_bus(v)

/* Non-cache memory allocate/free for descriptors */
#define kal_pci_alloc_sz(pci_p,sz,dma_p) \
        pci_alloc_consistent(pci_p,sz,dma_p)

#define kal_pci_free_sz(pci_p,sz,m,dma) \
        pci_free_consistent(pci_p,sz,m,dma)

/* Cache memory allocation. If kal_pci_map_single() related functions
 * do not work well in target platform, we use non-cache memory for
 * all memory allocation.
 */
#define kal_malloc_sz(sz,flg)   kmalloc(sz,flg)
#define kal_free(m)             kfree(m)
#define kal_alloc_skb(sz)       dev_alloc_skb(sz)
#define kal_free_skb(skb)       dev_kfree_skb(skb)

/* Functions acting on socket buffers */
#define kal_skb_put(skb,len)            skb_put(skb,len)
#define kal_skb_push(skb,len)           skb_push(skb,len)
#define kal_skb_pull(skb,len)           skb_pull(skb,len)
#define kal_skb_reserve(skb,len)        skb_reserve(skb,len)
#define kal_skb_data(skb)               (skb)->data
#define kal_skb_len(skb)                (skb)->len

/* Timer related functions */
#define kal_udelay(t)                   udelay(t)
#define kal_mdelay(t)                   mdelay(t)

/************************************************************************
*              F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/
void kal_netdev_rx(kal_skb_t *skb, kal_net_device_t *dev);

/************************************************************************
*           I N L I N E   F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/

#endif  /* __WLAN_KAL_H */

