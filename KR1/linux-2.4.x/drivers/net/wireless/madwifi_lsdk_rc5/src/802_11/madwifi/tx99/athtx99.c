/*
 * Copyright (c) 2002-2004 Atheros Communications, Inc.
 * All rights reserved.
 *
 */

#include <linux/config.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/random.h>
#include <linux/delay.h>
#include <linux/cache.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/if_arp.h>

#include <asm/uaccess.h>

#include "if_ethersubr.h"		/* for ETHER_IS_MULTICAST */
#include "if_media.h"
#include "if_llc.h"
#include "net80211/if_athproto.h"
#include <net80211/ieee80211_var.h>

#include "if_athvar.h"			/* XXX to identify IBM cards */
#include "ah_desc.h"
#include "ah_devid.h"			/* XXX to identify IBM cards */

#ifdef ATH_PCI		/* PCI BUS */
#include "if_ath_pci.h"
#endif			/* PCI BUS */
#ifdef ATH_AHB		/* AHB BUS */
#include "if_ath_ahb.h"
#endif			/* AHB BUS */

static u_int8_t PN9Data[] = {0xff, 0x87, 0xb8, 0x59, 0xb7, 0xa1, 0xcc, 0x24, 
			     0x57, 0x5e, 0x4b, 0x9c, 0x0e, 0xe9, 0xea, 0x50, 
			     0x2a, 0xbe, 0xb4, 0x1b, 0xb6, 0xb0, 0x5d, 0xf1, 
			     0xe6, 0x9a, 0xe3, 0x45, 0xfd, 0x2c, 0x53, 0x18, 
			     0x0c, 0xca, 0xc9, 0xfb, 0x49, 0x37, 0xe5, 0xa8, 
			     0x51, 0x3b, 0x2f, 0x61, 0xaa, 0x72, 0x18, 0x84, 
			     0x02, 0x23, 0x23, 0xab, 0x63, 0x89, 0x51, 0xb3, 
			     0xe7, 0x8b, 0x72, 0x90, 0x4c, 0xe8, 0xfb, 0xc0};
static u_int8_t test_addr[6] ={ 0x0, 0x0, 0x2, 0x3, 0x0, 0x0};

extern void ath_stoprecv(struct ath_softc *sc);
extern void	ath_draintxq(struct ath_softc *);


static struct sk_buff *
ath_alloc_skb(u_int size, u_int align)
{
	struct sk_buff *skb;
	u_int off;

	skb = dev_alloc_skb(size + align-1);
	if (skb != NULL) {
		off = ((unsigned long) skb->data) % align;
		if (off != 0)
			skb_reserve(skb, align - off);
	}
	return skb;
}

void ath_cont_data(struct ath_softc *sc ,int val,ath_callback ath_draintxq, ath_callback ath_stoprecv)
{
  static struct sk_buff *skb = NULL;
  struct ieee80211_frame *hdr;
  static struct ieee80211com *ic;
  static struct ath_buf *bf,*prev,*first;
  static struct ath_desc *ds;
  static struct ath_hal *ah;
  static STAILQ_HEAD(tpc_buf,ath_buf) tmp_q;
  static int is_inited=0;
  struct ath_txq *txq;
  const HAL_RATE_TABLE *rt;
  u_int8_t *p;
  u_int32_t flags, txrate, r,i;
  u_int16_t hdrlen, framelen, dmalen,delay=0;
#define	MIN(a,b)	((a) < (b) ? (a) : (b))

  if(ath_hal_getdiagstate(sc->sc_ah, 19,0,10,NULL,NULL) == AH_FALSE)
  {
	  printk("HAL does not support TX99 mode \n");
	  printk("compile HAL with AH_PRIVATE_DIAG turned on \n");
      return;
  } 
  if(is_inited == 0)
  {
    STAILQ_INIT(&tmp_q);
    is_inited=1;
  }
	
  /* enter CONT_DATA mode */
  if (val && skb==NULL) {
    skb = ath_alloc_skb(4096, 32);
    if (skb == NULL)
      goto out;

    /* build output packet */
    hdr = (struct ieee80211_frame *)skb_put(skb, sizeof(*hdr));
    IEEE80211_ADDR_COPY(&hdr->i_addr1, test_addr);
    IEEE80211_ADDR_COPY(&hdr->i_addr2, test_addr);
    IEEE80211_ADDR_COPY(&hdr->i_addr3, test_addr);
    hdr->i_dur[0] = 0x0;
    hdr->i_dur[1] = 0x0;
    hdr->i_seq[0] = 0x5a;
    hdr->i_seq[1] = 0x5a;
    hdr->i_fc[0] = IEEE80211_FC0_TYPE_DATA;
    hdr->i_fc[1] = 0;
    hdrlen = sizeof(*hdr);
    for(r=0; r<2000; ) {
      p = skb_put(skb, sizeof(PN9Data));
      memcpy(p, PN9Data, sizeof(PN9Data));
      r += sizeof(PN9Data);
    }
    framelen = hdrlen + r + IEEE80211_CRC_LEN;

    ic = &sc->sc_ic;
    ah = sc->sc_ah;
	rt = sc->sc_currates;
	if (rt==NULL) {
	  printk("no rate table\n");
	  goto out;
	}
	txrate = rt->info[rt->rateCount-1].rateCode;	/* send at highest rate */
	{
	  int rix;
	  if (sc->sc_txrx99.txrate==0)
	    sc->sc_txrx99.txrate = 6000;
	  for(rix=0; rix<rt->rateCount; rix++) {
	    if (rt->info[rix].rateKbps==sc->sc_txrx99.txrate) {
	      txrate = rt->info[rix].rateCode;
	      printk("txrate set to %dKbps\n", sc->sc_txrx99.txrate);
	      break;
	    }
	  }
	}
    ath_draintxq(sc);
    prev=first=NULL;
	printk("txpower set to %d\n", sc->sc_txrx99.txpower);
    /* send 20 frames for the Power Amp to settle down */
    for(i=0;i<20;++i)
    {
	ATH_TXBUF_LOCK_BH(sc);						

	bf = STAILQ_FIRST(&sc->sc_txbuf);
	if (bf != NULL) {
	  STAILQ_REMOVE_HEAD(&sc->sc_txbuf,bf_list);
	}
	ATH_TXBUF_UNLOCK_BH(sc);				
	
	if (bf==NULL) {
	    printk("no tx buf\n");
	    goto out; 
	}
        if(!i)  first=bf;
	framelen = skb->len + IEEE80211_CRC_LEN;
	dmalen = skb->len;
	txq = sc->sc_ac2q[WME_AC_VO];

	bf->bf_skbaddr = bus_map_single(sc->sc_bdev, skb->data, framelen, BUS_DMA_TODEVICE);
	bf->bf_skb = skb;
	bf->bf_node = 0;
	flags = HAL_TXDESC_CLRDMASK;  
	ds = bf->bf_desc;
        if(prev)
	  prev->bf_desc->ds_link = bf->bf_daddr;		/* link from prev desc  */
	ds->ds_data = bf->bf_skbaddr;

	r = ath_hal_setuptxdesc(ah, ds, framelen, hdrlen,
				HAL_PKT_TYPE_NORMAL,
				sc->sc_txrx99.txpower,
				txrate,			    /* tx rate */
				1,			    /* max retries */
				HAL_TXKEYIX_INVALID,	    /* no WEP */
				1,			    /* select Omni Antenna 0 */
				flags,
				0,			    /* rts/cts rate */
				0			    /* rts/cts duration */
				);

	if (r==AH_FALSE) {
	  printk("fail setuptxdesc r(%d)\n", r);
	  goto out;
	}  

	r = ath_hal_filltxdesc(ah, ds, skb->len, AH_TRUE, AH_TRUE,ds);
	if (r==AH_FALSE) {
	  printk("fail fill tx desc r(%d)\n", r);
	  goto out;
	}
	ath_hal_setupxtxdesc(ah, ds
			     , txrate, 15	/* series 1 */
			     , txrate, 15	/* series 2 */
			     , txrate, 15	/* series 3 */
			     );

        /* insert the buffers in to tmp_q */
        STAILQ_INSERT_HEAD(&tmp_q,bf,bf_list);

        prev=bf;
      }
      ath_hal_intrset(ah, 0);	    	/* disable interrupts */
      //sc->sc_imask = HAL_INT_RX | HAL_INT_TX
      //		  | HAL_INT_RXEOL | HAL_INT_RXORN
      //		  | HAL_INT_FATAL | HAL_INT_GLOBAL;
      sc->sc_imask = 0; 
      //ath_hal_intrset(ah, sc->sc_imask);

      bf->bf_desc->ds_link = 0;
      r = ath_hal_puttxbuf(ah, txq->axq_qnum, first->bf_daddr);
      ath_hal_txstart(ah, txq->axq_qnum);

      while(ath_hal_txprocdesc(ah,bf->bf_desc)  == HAL_EINPROGRESS)
      {
        udelay(2000);
        ++delay;
      }
      
      /* sleep for 20ms */
      udelay(20000);
      printk("took %d msec to transmit the 20 frames \n",2*delay);

      /* start TX99 mode */
      ath_stoprecv(sc);		    	/* stop recv side */
      bf->bf_desc->ds_link = first->bf_daddr;		/* link to self */
      ath_hal_getdiagstate(ah, 19,(void *) sc->sc_txrx99.prefetch,9,NULL,NULL);
      ath_hal_getdiagstate(ah, 19, (void *)txq->axq_qnum, val,NULL,NULL);
      r = ath_hal_puttxbuf(ah, txq->axq_qnum, first->bf_daddr);
      ath_hal_txstart(ah, txq->axq_qnum);
  }

  /* leave CONT_DATA mode, reset the chip */
  if (val==0 && skb) {
    int j=0;
    ath_hal_getdiagstate(ah, 19, 0, 0,NULL,NULL);
    /* insert the buffers back into txbuf list */

	ATH_TXBUF_LOCK_BH(sc);						

        bf = STAILQ_FIRST(&tmp_q);
        while(bf)
	{
	  bf->bf_skb=NULL;
	     STAILQ_REMOVE_HEAD(&tmp_q,bf_list);
             STAILQ_INSERT_HEAD(&sc->sc_txbuf,bf,bf_list);
             bf = STAILQ_FIRST(&tmp_q);
	     ++j;
	}
	ATH_TXBUF_UNLOCK_BH(sc);				
        printk("inserted back %d buffers \n",j);
    ic->ic_reset(ic->ic_dev);
    skb = NULL;
    bf = NULL;
  }

  if (val==7 && skb) {
    ath_hal_getdiagstate(ah, 19, ds, 7,NULL,NULL);
  }

  sc->sc_txrx99.tx99mode=val;
 out:
  return;
#undef MIN
}

static int ath_rx99_hardstart(struct sk_buff *skb, struct net_device *dev)
{
	dev_kfree_skb(skb);
    return 0;
}

static int ath_rx99_mgtstart(struct ieee80211com *ic, struct sk_buff *skb)
{
	dev_kfree_skb(skb);
    return 0;
}


void ath_rx99_ctl(struct ath_softc *sc ,int val)
{
	struct ath_hal *ah = sc->sc_ah;
	if(val && !sc->sc_txrx99.rx99mode) {
		sc->sc_txrx99.prev_hard_start = sc->sc_dev.hard_start_xmit;
		sc->sc_txrx99.prev_mgt_start = sc->sc_ic.ic_mgtstart;
		sc->sc_dev.hard_start_xmit = ath_rx99_hardstart;
		sc->sc_ic.ic_mgtstart = ath_rx99_mgtstart;
		/*
		 * Disable interrupts.
		 */
        sc->sc_txrx99.imask=sc->sc_imask;
        sc->sc_imask  &=  (~(HAL_INT_SWBA | HAL_INT_BMISS));
		ath_hal_intrset(ah, sc->sc_imask );
	}
    else {
		if(!val && sc->sc_txrx99.rx99mode) {
			sc->sc_dev.hard_start_xmit = sc->sc_txrx99.prev_hard_start;
			sc->sc_ic.ic_mgtstart = sc->sc_txrx99.prev_mgt_start;
			sc->sc_txrx99.prev_hard_start = sc->sc_txrx99.prev_mgt_start=NULL;
		    sc->sc_imask |= (sc->sc_txrx99.imask & (HAL_INT_SWBA | HAL_INT_BMISS));
			ath_hal_intrset(ah, sc->sc_imask);
            sc->sc_txrx99.imask=0;
        }
    }
    sc->sc_txrx99.rx99mode=val;
}

/*
 * Module glue.
 */
static	char *dev_info = "ath_tx99";

MODULE_AUTHOR("Atheros");
MODULE_DESCRIPTION("tx99 support for Atheros devices");
#ifdef MODULE_LICENSE
MODULE_LICENSE("Proprietary");
#endif

static int __init
init_ath_tx99(void)
{	
    /* XXX version is a guess; where should it come from? */
	printk(KERN_INFO "%s: Version 1.0\n"
		"Copyright (c) 2001-2004 Atheros Communications, Inc, "
		"All Rights Reserved\n", dev_info);
#ifdef MODULE
    inter_module_register("tx99ctl",THIS_MODULE,ath_cont_data);
    inter_module_register("rx99ctl",THIS_MODULE,ath_rx99_ctl);
#endif
	return 0;
}

module_init(init_ath_tx99);

static void __exit
exit_ath_tx99(void)
{
	printk(KERN_INFO "%s: driver unloaded\n", dev_info);
    inter_module_unregister("tx99ctl");
}

module_exit(exit_ath_tx99);


