/*
 * Copyright c                Realtek Semiconductor Corporation, 2003
 * All rights reserved.                                                    
 * 
 *  $Header: /home/cvsroot/uClinux-dist/linux-2.4.x/drivers/net/re865x/re_core.c,v 1.223 2005/02/25 07:49:05 chenyl Exp $
 *
 * $Author: chenyl $
 *
 * Abstract:
 *
 *   re865x.c: A Linux Ethernet driver for the RealTek 865* chips.
 *
 */


#define DRV_RELDATE		"May 27, 2004"
#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/compiler.h>
#include <linux/netdevice.h>
#include <linux/inetdevice.h>
#include <linux/etherdevice.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/ethtool.h>
#include <linux/mii.h>
#include <linux/if_vlan.h>
#include <linux/crc32.h>
#include <linux/sched.h>
#include <linux/signal.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/rtl865x/interrupt.h>
#include <asm/rtl865x/re865x.h>
#include <linux/slab.h>
#include <linux/signal.h>
#include <linux/proc_fs.h>
#include <linux/time.h>
#include <linux/rtc.h>
#include "version.h"
#include "rtl865x/rtl_types.h"
#include "rtl865x/rtl_queue.h"
#include "rtl865x/mbuf.h"
#include "rtl865x/asicRegs.h"
#include "rtl865x/rtl8651_tblAsicDrv.h"
#include "rtl865x/rtl8651_tblDrv.h"
#include "rtl865x/rtl8651_tblDrvProto.h"
#include "rtl865x/rtl8651_tblDrvFwd.h"
#include "rtl865x/rtl_utils.h"
#include "rtl865x/assert.h"
#include "flashdrv.h"
#include "rtl865x/rtl8651_alg_init.h" // 2005-10-07: Peter add for 3G (VPN)
#ifdef CONFIG_RTL865X_ROMEPERF
#include "romeperf.h"
#endif
#ifdef _RTL_NEW_LOGGING_MODEL
#include "rtl865x_log.h"
#endif
#include "swNic2.h"
#ifdef CONFIG_RTL8305S
#include "rtl865x/rtl8305s.h"
#endif
#include "rtl865x_mpt.h"
#ifdef CONFIG_RTL865X_VOIP
#include "rtl865x/voip_support.h"
#endif

#ifdef CONFIG_RTL865XB_3G
#undef RE_CORE_DEBUG
//#define RE_CORE_DEBUG

/* for GlobalSun 3G wan type */
struct net_device *wanDev = NULL;
uint32 wanDevModification = FALSE; // Because current solution using eth0 as a bridge

int32 reCore_registerWANDevice(struct net_device* newWanDev)
{
	if (wanDevModification != TRUE)
		return FAILED;

	assert(newWanDev);
	wanDev = newWanDev;
	return SUCCESS;
}

void reCore_unregisterWANDevice(void)
{
	wanDev = NULL;
}
#endif


/* These identify the driver base version and may not be removed. */
MODULE_DESCRIPTION("RealTek RTL-8650 series 10/100 Ethernet driver");
MODULE_LICENSE("GPL");
MODULE_PARM (debug, "i");
MODULE_PARM_DESC (debug, "re865x bitmapped message enable number");

/* Maximum number of multicast addresses to filter (vs. Rx-all-multicast).
   The RTL chips use a 64 element hash table based on the Ethernet CRC.  */
static int multicast_filter_limit = 32;
MODULE_PARM (multicast_filter_limit, "i");
MODULE_PARM_DESC (multicast_filter_limit, "maximum number of filtered multicast addresses");

#define DRV_NAME		"re865x"
#define PFX			DRV_NAME ": "
#define DRV_VERSION		"0.1.0"
#define LDR_COMMUN_BASE 0x80000300
#define LDR_VERSION_ADDR (LDR_COMMUN_BASE+0xF0)
#define IS_865XB() ( ( REG32( CRMR ) & 0xffff ) == 0x5788 )
#define IS_865XA() ( !IS_865XB() )
#define FLASH_BASE	(IS_865XA()?0xBFC00000:0xBE000000)
#define RE865X_DEFAULT_ETH_INTERFACES		2//3//2	//eth0: WAN, eth1: LAN , need more WAN or LAN??
#if defined(CONFIG_BRIDGE) ||defined(CONFIG_BRIDGE_MODULE)
	#define RE865X_STP_BPDU_TXRX
#endif

struct cp_extra_stats {
	unsigned long		rx_frags;
	unsigned long 		tx_timeouts;
	unsigned long		mbufout_cnt;
	unsigned long		pktrunt;
	unsigned long		linkchg;
};

#define INIT_CHECK(expr) do {\
	if((expr)!=SUCCESS){\
		printk("Error >>> %s:%d failed !\n", __FUNCTION__,__LINE__);\
			return FAILED;\
	}\
}while(0)

static int32  rtl865x_slowRx(struct rtl_pktHdr* pPkt, int32 reason);

#define warn(expr) expr


spinlock_t *rtl865xSpinlock;
struct tasklet_struct	*rtl8651RxTasklet;


 struct re865x_priv
{
	u16			ready;
	u16			addIF;
	u16			devnum;
	u32			sec_count;
	u32			sec;
	struct	net_device	*dev[RTL8651_VLAN_NUMBER];
#ifdef RE865X_STP_BPDU_TXRX
	struct	net_device	*stp_port[9];
#endif
	spinlock_t		lock;
	void			*regs;
	struct cp_extra_stats	rp_stats;
	struct tasklet_struct	rx_tasklet;
	struct timer_list timer;	/* Media monitoring timer. */
	struct timer_list timer2; /* MNQ timer. */
	int	pppoeIdleTimeout[6];
};

static  struct re865x_priv _rtl86xx_dev; 

#ifndef _RTL_NEW_LOGGING_MODEL
//for circular buffer
struct circ_buf 
{	
	int size;	
	int head;	
	int tail;	
	char data[1];
};
#endif

struct dev_priv {
	u32			id;            /* VLAN id, not vlan index */
	u32			portmask;     /* member port mask */
	u32			portnum;     	/* number of member ports */
	u32			netinit;
	struct net_device	*dev;
	//struct re865x_priv	*priv;
#if CP_VLAN_TAG_USED
	struct vlan_group	*vlgrp;
#endif
	u32			msg_enable;
	struct net_device_stats net_stats;
	int			ip;
	int			mask;
	int			dns;
	int			pppoe_status;
	int			gateway;
	int			setup;
//	struct mii_if_info	mii_if;
};

static int udhcpc_pid=0;
static int wan_port=0;
void recore_updatePortStatus(uint32 port, int8 linkUp) {
	
	rtlglue_drvMutexLock();
	//remove all arp entries learned from this port.
	if(linkUp == FALSE) 
		_rtl8651_removeArpAndNaptFlowWhenLinkDown(port,NULL);
	else 
	{
		if((port==wan_port)&&(udhcpc_pid!=0)) //WAN Port
		{
			struct siginfo info;
			struct task_struct *p=NULL;

			memset(&info,0,sizeof(info));
			
			p=find_task_by_pid(udhcpc_pid);
			if(p!=NULL) 
			{
				info.si_signo = 16;
				info.si_errno = 0;
				info.si_code = SI_USER;
				info.si_pid=current->pid;
				info.si_uid=current->uid;			
				send_sig_info(info.si_signo, &info, p);			
			}		

		}
	}
	rtlglue_drvMutexUnlock();
	return;
}


#ifdef CONFIG_RTL865XB_3G

void rtl865x_getWanMac(char *mac)
{
	memcpy(mac, _rtl86xx_dev.dev[0]->dev_addr,6);
}

#endif

#ifdef _RTL_LOGGING
#ifndef _RTL_NEW_LOGGING_MODEL
static int8 logMsgBuf[256];
static int32 timezone_diff;
int32 myLoggingFunction
(
unsigned long  dsid,
unsigned long  moduleId,
unsigned char  proto,
char           direction,
unsigned long  sip,
unsigned long  dip,
unsigned short sport,
unsigned short dport,
unsigned char  type,
unsigned char  action,
char         * msg
);
#endif
#endif /* _RTL_LOGGING */

#define PKT_BUF_SZ		2048 /* Size of each temporary Rx buffer.*/
#define FREQ	 	10
static int rtl8651_user_pid;
static int re865x_ioctl (struct net_device *dev, struct ifreq *rq, int cmd);
static void re865x_interrupt (int irq, void *dev_instance, struct pt_regs *regs);
#ifndef _RTL_NEW_LOGGING_MODEL
static void circ_msg(struct circ_buf *buf,const char *msg);
#endif

#if defined(CONFIG_RTL865X_INIT_BUTTON)
/*
 * lid diagnosis LED with gpio-porta-6
 */
#define DIAG_LED_ON()   {REG32(PABDAT) &= ~0x40000000;}
#define DIAG_LED_OFF()  {REG32(PABDAT) |=  0x40000000;}
#define DELAY_500_MSEC  {int cnt;for(cnt=0;cnt<500;cnt++)udelay(1000);}
#define DELAY_3000_MSEC {int cnt;for(cnt=0;cnt<3000;cnt++)udelay(1000);}
/* global variable for init button */
uint32 init_button_event = 0;
uint32 init_button_count = 0;
uint32 ledCtrlFreq = 0;
uint32 ledCtrlTick = 0;  /* every 10ms per ledCtrlTick */
uint32 ledCtrlTick2 = 0; /* every 500ms per ledCtrlTick2 */
uint32 ledCtrlPattern[23]={0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
uint32 ledCtrlPatternIndexLimit[10]={0,7,9,11,0,15,0,19,0,23}; /* use ledCtrlFreq as index to this array */
#endif /* CONFIG_RTL865X_INIT_BUTTON */

#ifdef _RTL_LOGGING
#ifndef _RTL_NEW_LOGGING_MODEL
static int8 logMsgBuf[256];
static int32 timezone_diff;
int32 myLoggingFunction
(
unsigned long  dsid,
unsigned long  moduleId,
unsigned char  proto,
char           direction,
unsigned long  sip,
unsigned long  dip,
unsigned short sport,
unsigned short dport,
unsigned char  type,
unsigned char  action,
char         * msg
);
static void to_tm(unsigned long tim, struct rtc_time * tm);
#endif
#endif /* _RTL_LOGGING */

/* 
 * call diagnosis functions and lid LEDs accordingly
 * if errors occured
 * diag spec:
 * 1 (blink 1time) RAM check --> implemented in bootloader/assembly
 * 2 (blink 2times)FLASH-ROM check --> checked in board.c thru ioctl
 * 3 (blink 3times)Cable LAN abnormality --> do cable loopback in bootloader/loader.c
 * 4 (blink 5times)Network abnormality --> checked in board.c thru ioctl
 * 5 (blink 9times)inside error --> install this in some kernel checking
 */
#if defined(CONFIG_RTL865X_INIT_BUTTON)||defined(CONFIG_RTL865X_DIAG_LED)
static void  gpioLed(uint32 ledFreq);
#endif /* CONFIG_RTL865X_INIT_BUTTON || CONFIG_RTL865X_DIAG_LED */

static inline void skb_cpy(u16 *dst, void *src,int pktlen)
{
	u32 i=0,k;
	for (i=0;i<=pktlen;i+=4)
	{
		k=*(u32*)src;
		dst[0]=(u16)(k>>16)&0xffff;
		dst[1]=(u16)(k)&0xffff;
		src+=4;
		dst+=2;
	}
}

static inline void re865x_rx_skb (struct dev_priv *dp , struct sk_buff *skb)
{
	skb->protocol = eth_type_trans (skb, dp->dev);
	dp->net_stats.rx_packets++;
	dp->net_stats.rx_bytes += skb->len;
	dp->dev->last_rx = jiffies;

#ifdef CONFIG_RTL865XB_3G
	#ifdef RE_CORE_DEBUG
	printk("%s %d\n",__FUNCTION__,__LINE__);
	printk("%s name=%s,protocol=%x: [before assign protocol]\n",__FUNCTION__,skb->dev->name,skb->protocol);
	memDump(skb->data,skb->len,"re865x_rx_skb");
	#endif
	if (	(strlen(skb->dev->name) >= 3) && 
		(strncmp(skb->dev->name, "ppp", 3) == 0)) 
	{	
		/* need more check */
		skb->protocol = (*(skb->data - 2) << 8) | (*(skb->data - 1));
		#ifdef RE_CORE_DEBUG
		printk("%s name=%s,protocol=%x: [after assign protocol]\n",__FUNCTION__,skb->dev->name,skb->protocol);
		#endif
	}
	
	// For Cardbus USB v1.1 throughput test
	else if (	(strlen(skb->dev->name) >= 4) && 
		(strncmp(skb->dev->name, "eth2", 4) == 0)) 
	{	/* need more check */
			skb->protocol = (*(skb->data - 2) << 8) | (*(skb->data - 1));
	}
#endif

#if CP_VLAN_TAG_USED
	if (dp->vlgrp && (desc->opts2 & RxVlanTagged)) {
		vlan_hwaccel_rx(skb, dp->vlgrp, desc->opts2 & 0xffff);
	} else
#endif
	netif_rx(skb);
}


/* Set or clear the multicast filter for this adaptor.
 * This routine is not state sensitive and need not be SMP locked.
 */
static void __re865x_set_rx_mode (struct net_device *dev)
{
	/*struct re_private *cp = dev->priv;*/
	u32 mc_filter[2];	/* Multicast hash filter */
	int i;
	/*u32 tmp;*/

	return ;
	/* Note: do not reorder, GCC is clever about common statements. */
	if (dev->flags & IFF_PROMISC) {
		/* Unconditionally log net taps. */
		printk (KERN_NOTICE "%s: Promiscuous mode enabled.\n",
			dev->name);
		mc_filter[1] = mc_filter[0] = 0xffffffff;
	} else if ((dev->mc_count > multicast_filter_limit)
		   || (dev->flags & IFF_ALLMULTI)) {
		/* Too many to filter perfectly -- accept all multicasts. */
		mc_filter[1] = mc_filter[0] = 0xffffffff;
	} else {
		struct dev_mc_list *mclist;
		mc_filter[1] = mc_filter[0] = 0;
		for (i = 0, mclist = dev->mc_list; mclist && i < dev->mc_count;
		     i++, mclist = mclist->next) {
			int bit_nr = ether_crc(ETH_ALEN, mclist->dmi_addr) >> 26;

			mc_filter[bit_nr >> 5] |= 1 << (bit_nr & 31);
		}
	}

	/* We can safely update without stopping the chip. */
}

static void re865x_set_rx_mode (struct net_device *dev)
{
	unsigned long flags;

	spin_lock_irqsave (&_rtl86xx_dev.lock, flags);
	__re865x_set_rx_mode(dev);
	spin_unlock_irqrestore (&_rtl86xx_dev.lock, flags);
}

static struct net_device_stats *re865x_get_stats(struct net_device *dev)
{
	struct dev_priv  *dp = dev->priv;
	return &dp->net_stats;
}

#define MNQ_10MS	1	/* 1: activate 10ms timer for MNQ */

#if !MNQ_10MS

/*
 *	This function will be triggered 10 times per second.
 */
static void re865x_timer(unsigned long data)
{
//	struct net_device *dev = (struct net_device *)data;
//	struct re865x_priv *rp = dp->priv;

#ifdef CONFIG_RTL865XB	
	//reset watchdog timer. Reboot when 2^18 watchdog cycles reached*/
	REG32(WDTCNR)|=WDTCLR;
#endif	
	static int cnt=0;

	spin_lock_irq(&_rtl86xx_dev.lock);

	cnt++;
	if (cnt == 2) {
		_rtl8651_mnQueueTimer();
		cnt = 0;
	}
		
#if EVENT_TRIGGER_TIMEUPDATE > 0
	rtl8651_timeUpdate(0);
#else/*EVENT_TRIGGER_TIMEUPDATE*/
	if (jiffies-_rtl86xx_dev.sec_count >=HZ)
	{
		rtl8651_timeUpdate(1);
		_rtl86xx_dev.sec_count = jiffies;
	}
#endif/*EVENT_TRIGGER_TIMEUPDATE*/

	#if defined(CONFIG_RTL865X_INIT_BUTTON)	
	if ((init_button_event==1) && (init_button_count>0)){
		init_button_count--;
		//printk("\n%d",init_button_count);
		if (init_button_count==0){
			sys_kill(rtl8651_user_pid,50);
 		}
	}
	#endif /* CONFIG_RTL865X_INIT_BUTTON */
	#if defined(CONFIG_RTL865X_DIAG_LED)	
	if (ledCtrlFreq){
		ledCtrlTick++;/* kick the heartbeat */
		if ((ledCtrlTick%5)==0){
			if (ledCtrlPattern[ledCtrlTick2]){
				DIAG_LED_ON();
			}else{
				DIAG_LED_OFF();
			}
			ledCtrlTick2++;/* kick the hearbeat */
			ledCtrlTick2 = ledCtrlTick2 % (ledCtrlPatternIndexLimit[ledCtrlFreq]);			
		}
	}
	#endif /* CONFIG_RTL865X_DIAG_LED */
	spin_unlock_irq(&_rtl86xx_dev.lock);
	_rtl86xx_dev.timer.expires = jiffies + FREQ;
	add_timer(&_rtl86xx_dev.timer); //schedule next timer event
}

#endif


#if MNQ_10MS

#define RESET_GST_WATCHDOG

#ifdef RESET_GST_WATCHDOG
#define GST_COUNTER_THRESHOLD    40
static unsigned long ext_watch_dog_counter = 0;
#endif
/*
 *	This function will be triggered 10 times per second.
 */
static void re865x_timer(unsigned long data)
{
	//struct net_device *dev = (struct net_device *)data;
//	struct re865x_priv *rp = dp->priv;

#ifdef CONFIG_RTL865XB	
	//reset watchdog timer. Reboot when 2^18 watchdog cycles reached*/
	REG32(WDTCNR)|=WDTCLR;
	
	// reset GST external watch dog
	if(++ext_watch_dog_counter >= GST_COUNTER_THRESHOLD)
	{
		//REG32(PABCDAT) |= 0x1000;
		REG32(PABCDAT) |= 0x00100000;
		ext_watch_dog_counter = 0; 
	}
#endif	

	spin_lock_irq(&_rtl86xx_dev.lock);

#if EVENT_TRIGGER_TIMEUPDATE > 0
	rtl8651_timeUpdate(0);
#else/*EVENT_TRIGGER_TIMEUPDATE*/
	if (jiffies-_rtl86xx_dev.sec_count >=HZ)
	{
		rtl8651_timeUpdate(1);
		_rtl86xx_dev.sec_count = jiffies;
	}
#endif/*EVENT_TRIGGER_TIMEUPDATE*/

	#if defined(CONFIG_RTL865X_INIT_BUTTON)	
	if ((init_button_event==1) && (init_button_count>0)){
		init_button_count--;
		//printk("\n%d",init_button_count);
		if (init_button_count==0){
			sys_kill(rtl8651_user_pid,50);
 		}
	}
	#endif /* CONFIG_RTL865X_INIT_BUTTON */
	#if defined(CONFIG_RTL865X_DIAG_LED)	
	if (ledCtrlFreq){
		ledCtrlTick++;/* kick the heartbeat */
		if ((ledCtrlTick%5)==0){
			if (ledCtrlPattern[ledCtrlTick2]){
				DIAG_LED_ON();
			}else{
				DIAG_LED_OFF();
			}
			ledCtrlTick2++;/* kick the hearbeat */
			ledCtrlTick2 = ledCtrlTick2 % (ledCtrlPatternIndexLimit[ledCtrlFreq]);			
		}
	}
	#endif /* CONFIG_RTL865X_DIAG_LED */
	spin_unlock_irq(&_rtl86xx_dev.lock);
	_rtl86xx_dev.timer.expires = jiffies + FREQ;
	add_timer(&_rtl86xx_dev.timer); //schedule next timer event
}

#define SHOW_CYCLE		0

/*
 *	This function will be triggered 100 times per second. (10ms)
 */
static void re865x_timer2(unsigned long data)
{
#if SHOW_CYCLE
	extern int mnq_toggle;
	uint64 scnt, ecnt;

	spin_lock_irq(&_rtl86xx_dev.lock);


	if (mnq_toggle) {
		rtl8651_romeperfInit();
		rtl8651_romeperfGet(&scnt);
		rtl8651_romeperfGet(&ecnt);
		printk("Empty Cycles: %lld\n", ecnt - scnt);

		rtl8651_romeperfInit();
		rtl8651_romeperfGet(&scnt);			
	}
#endif
	_rtl8651_mnQueueTimer();
#if SHOW_CYCLE
	if (mnq_toggle) {
		rtl8651_romeperfGet(&ecnt);
		printk("MnQ Cycles: %lld ; around %lld mill-sec\n", ecnt - scnt, (ecnt -scnt) * 5  / 1024 /1024);
	}
#endif		
	spin_unlock_irq(&_rtl86xx_dev.lock);
	_rtl86xx_dev.timer2.expires = jiffies + 1;
	add_timer(&_rtl86xx_dev.timer2); //schedule next timer event
}

#endif

/* Time in jiffies before concluding the transmitter is hung. */
#define TX_TIMEOUT		(10*HZ)
static int _re865xNIC_init;



static int32 rtl865x_toProtocolStack(struct rtl_pktHdr *pPkt,struct net_device *dev){
	struct dev_priv *dp= (struct dev_priv *)dev->priv;	
	struct sk_buff *new_skb;
#ifdef SWNIC_RX_ALIGNED_IPHDR
	struct sk_buff *old_skb;
#endif /* SWNIC_RX_ALIGNED_IPHDR */
	uint16 pktLen=pPkt->ph_len;		
	struct rtl_mBuf *m=pPkt->ph_mbuf;
	unsigned char *dataPtr=m->m_data;

	new_skb = dev_alloc_skb (PKT_BUF_SZ);
	if(!new_skb)
		return FAILED;

	//when pkt is sending to protocol stack, flush d-cache
	lx4180_writeCacheCtrl(0);
	lx4180_writeCacheCtrl(1);
	lx4180_writeCacheCtrl(0);

#ifdef CONFIG_RTL865X_CACHED_NETWORK_IO
#if 1
	//when pkt is sending to protocol stack, flush d-cache
	lx4180_writeCacheCtrl(0);
	lx4180_writeCacheCtrl(1);
	lx4180_writeCacheCtrl(0);
#endif
#endif

#ifdef SWNIC_RX_ALIGNED_IPHDR
	if(IS_865XB())
	{
		int32 align;
		/*For 8650B, 8651B, chip can do Rx at any position, no copy required*/
		old_skb=(struct sk_buff *)m->m_extClusterId;
		old_skb->dev = dev;
		//force set length to 0
		old_skb->data = old_skb->tail = old_skb->head ;
		old_skb->len=0;
		//adjust data pointer by mbuf
		skb_reserve(old_skb, 16+mBuf_leadingSpace(m)); 
		//set real pkt length
		skb_put(old_skb, pktLen);
		//refill Rx ring use new buffer
#ifdef CONFIG_RTL865X_MBUF_HEADROOM
		align=CONFIG_RTL865X_MBUF_HEADROOM;
#endif
		mBuf_attachCluster(m, (void*)UNCACHE(new_skb->data), (uint32)new_skb, 2048, 0, align);
		mBuf_freeMbufChain(m);

#ifdef CONFIG_RTL865XB_3G
		#ifdef RE_CORE_DEBUG
		printk("%s %d: IS_865XB\n",__FUNCTION__,__LINE__);
		printk("dev->name = %s\n",dev->name);
		#endif
		/* for Globalsun 3G project */
		if (	(strlen(dev->name) >= 3) &&
			(strncmp(dev->name, "ppp", 3) == 0))
		{	/* need more check */
			#ifdef RE_CORE_DEBUG
			printk("%s old_skb->protocol = %x[1]\n",__FUNCTION__,old_skb->protocol);
			#endif
			old_skb->protocol = ((old_skb->data[12] << 8) | old_skb->data[13]);
			old_skb->data += 14;
			assert(old_skb->len > 14);
			old_skb->len -= 14;
			#ifdef RE_CORE_DEBUG
			printk("%s old_skb->protocol = %x[2]\n",__FUNCTION__,old_skb->protocol);
			#endif
		}
		
		/* eth type(rtl8150) do not need to right shift the header, it will be done in  eth_type_trans  */	
#endif		
		
		//send to protocol stack...
		re865x_rx_skb((struct dev_priv*)dp, old_skb);
	}else
#endif
	{
		/*For 8650,8651, chip can only do 4 byte aligned Rx
		 which makes IP hdr starts at 2 byte aligned position. 
		 However, Linux protocol stack requires IP hdr aligned 
		 to 4 byte position so we do a copy here*/
		new_skb->dev = dev;
		skb_reserve(new_skb, 2);	// for alignment issue
		skb_cpy((u16*)new_skb->data, dataPtr, pktLen);
		skb_put(new_skb, pktLen);
		mBuf_freeMbufChain(pPkt->ph_mbuf);

#ifdef CONFIG_RTL865XB_3G
		#ifdef RE_CORE_DEBUG
		printk("%s %d: not IS_865XB\n",__FUNCTION__,__LINE__);
		printk("dev->name = %s\n",dev->name);
		#endif
		/* for Globalsun 3G project */
		if (	(strlen(dev->name) >= 3) &&
			(strncmp(dev->name, "ppp", 3) == 0))
		{
			#ifdef RE_CORE_DEBUG
			printk("%s new_skb->protocol = %x[1]\n",__FUNCTION__,new_skb->protocol);
			#endif
			new_skb->protocol = ((new_skb->data[12] << 8) | new_skb->data[13]);
			new_skb->data += 14;
			assert(new_skb->len > 14);
			new_skb->len -= 14;
			
			#ifdef RE_CORE_DEBUG
			printk("%s new_skb->protocol = %x[2]\n",__FUNCTION__,new_skb->protocol);
			#endif
		}
		
		/* eth type(rtl8150) do not need to right shift the header, it will be do in  eth_type_trans  */	
		
#endif		
		
		re865x_rx_skb((struct dev_priv*)dp, new_skb);
	}
	return SUCCESS;
}


#define rtl8651_vlanTableIndex(vid)  (vid& (RTL8651_VLAN_NUMBER-1)) /* for _modifyPassthruVid() only */
/*
 *  This records the mapping from port number to VLAN id.
 *  Given a port number, VLAN ID is returned.
 *  This variable is set by re86xx_add_vlan(), and queried by _modifyPassthruVid().
 */
uint16 passthruPortMappingToVlan[MAX_PORT_NUM];


/* Since PPPoE/IPv6 packets come from passthru VLAN,
 *   we must modify the VLAN ID before goto protocol stack.
 * (Cause passthru VLAN does NOT have corresponding network interface)
 */
void _modifyPassthruVid( struct rtl_pktHdr * pkthdrPtr )
{
#if 1
	uint32 newVlanIdx;
	if ( rtl8651_transformPassthruVlanId( pkthdrPtr->ph_vlanIdx, &newVlanIdx ) == SUCCESS )
	{
		pkthdrPtr->ph_vlanIdx = newVlanIdx;
	}
#else
	int32 getVlanId;
	if ( ( ((getVlanId=rtl8651_getPppoePassthruVid())!=FAILED) && 
	       pkthdrPtr->ph_vlanIdx == rtl8651_vlanTableIndex(getVlanId) ) ||
	     ( ((getVlanId=rtl8651_getIpv6PassthruVid())!=FAILED) && 
	       pkthdrPtr->ph_vlanIdx == rtl8651_vlanTableIndex(getVlanId) ) ||
	     ( ((getVlanId=rtl8651_getIpxPassthruVid())!=FAILED) && 
	       pkthdrPtr->ph_vlanIdx == rtl8651_vlanTableIndex(getVlanId) ) ||
	     ( ((getVlanId=rtl8651_getNetbiosPassthruVid())!=FAILED) && 
	       pkthdrPtr->ph_vlanIdx == rtl8651_vlanTableIndex(getVlanId) ) )

	{
		/* Change the source VLAN index to corresponding interface. */
		if ( passthruPortMappingToVlan[pkthdrPtr->ph_portlist] == 0 )
		{
#if 0/* for debug only */
			rtlglue_printf("[%s:%d] VLAN ID is zero!\n", __FILE__, __LINE__ );
#endif/*0*/
		}
		else
		{
#if 0/* for debug only */
			rtlglue_printf("[%s:%d] Passthru, VLAN is changed from %d to %d (port=%d)!\n", 
			               __FILE__, __LINE__, pkthdrPtr->ph_vlanIdx, 
			               rtl8651_vlanTableIndex( passthruPortMappingToVlan[pkthdrPtr->ph_portlist] ),
			               pkthdrPtr->ph_portlist);
#endif/*0*/
			pkthdrPtr->ph_vlanIdx = rtl8651_vlanTableIndex( passthruPortMappingToVlan[pkthdrPtr->ph_portlist] );
		}
		
	}
#endif
}


#ifdef RE865X_STP_BPDU_TXRX
static unsigned char STPmac[6]={ 1, 0x80, 0xc2, 0,0,0};
#endif
__IRAM static int32  rtl865x_fastRx(struct rtl_pktHdr* pPkt) {
	int32 retval;
#ifdef CONFIG_RTL865XB	
	//reset watchdog timer. Reboot when 2^18 watchdog cycles reached*/
	REG32(WDTCNR)|=WDTCLR;
#endif

	if(rtl8651_l2protoPassThrough)
	{
		/* change VLAN index of PPPoE/IPv6/Ipx/Netbios Passthru for IP Multicast. */
		_modifyPassthruVid( pPkt );
	}
#ifdef CONFIG_RTL865X_ETH8899
	if(pPkt->ph_proto==PKTHDR_ETHERNET){
		//Realtek's proprietary ethtype: 0x8899
		struct rtl_mBuf *m=pPkt->ph_mbuf;
		if(*((uint16 *)&m->m_data[12])==0x8899)
			return rtl865x_slowRx(pPkt, FWDENG_RTKPROTO);
	}
#endif

	
#ifdef CONFIG_RTL865X_ROMEPERF
	rtl8651_romeperfEnterPoint(ROMEPERF_INDEX_FWDENG_INPUT);
#endif

	retval=rtl8651_fwdEngineInput((void *)pPkt);
#ifdef CONFIG_RTL865X_ROMEPERF
	rtl8651_romeperfExitPoint(ROMEPERF_INDEX_FWDENG_INPUT);
#endif
	if(retval!=SUCCESS)
		return rtl865x_slowRx(pPkt, retval);
	return SUCCESS;
}

static int32  rtl865x_slowRx(struct rtl_pktHdr* pPkt, int32 reason) 
{
	//forwarding engine can't handle the packet...
	struct rtl_mBuf *m=pPkt->ph_mbuf;
	struct dev_priv *dp;
	struct net_device *dev;

	#ifdef RE_CORE_DEBUG
	{
		int i;
		for(i = 0; i < RTL8651_VLAN_NUMBER; i++)
			printk("RTL8651_VLAN_NUMBER = %d, dev->name = %s, %d\n", RTL8651_VLAN_NUMBER, _rtl86xx_dev.dev[i]->name, __LINE__);
	}
	#endif
	
	if (reason == FWDENG_QUEUED_TO_GW) 
	{
		//deliver IP fragments one by one to protocol stack.
		//fragments are sorted in order.
		struct rtl_pktHdr *fpkthdr, *ppkthdr;
		ppkthdr = pPkt;
		while (ppkthdr)
		{
			fpkthdr = ppkthdr->ph_nextHdr;
			ppkthdr->ph_nextHdr = NULL;
			m=ppkthdr->ph_mbuf;
#ifdef RE865X_STP_BPDU_TXRX
			if(memcmp(&m->m_data[0],STPmac,6)==0 ){
				//It's a BPDU
				assert(ppkthdr->ph_portlist<RTL8651_AGGREGATOR_NUMBER);
				dev = _rtl86xx_dev.stp_port[ppkthdr->ph_portlist];
				assert(!dev->irq);//we rely on this assumption
				assert(dev);
			}else
#endif
			{
				dev = _rtl86xx_dev.dev[ppkthdr->ph_vlanIdx];

#ifdef CONFIG_RTL865XB_3G
				/* GST 3G project */
				#ifdef RE_CORE_DEBUG
				printk("%s %d\n",__FUNCTION__,__LINE__);
				printk("ppketgdr->ph_vlanIdx = %x\n",ppkthdr->ph_vlanIdx);
				#endif
				if (wanDevModification == TRUE &&  wanDev && ppkthdr->ph_vlanIdx==0x02)
				{
					#ifdef RE_CORE_DEBUG
					printk("%s: FWDENG_QUEUED_TO_GW\n",__FUNCTION__);
					#endif
					dev = wanDev;
				}
#endif
					
				assert(dev);
				assert(dev->br_port==NULL);
			}
			dp=(struct dev_priv *)dev->priv;
			assert(dp);
			if(FAILED==rtl865x_toProtocolStack(ppkthdr, dev))
			{
				ppkthdr->ph_nextHdr = fpkthdr;
				while (ppkthdr)
				{
					fpkthdr = ppkthdr->ph_nextHdr;
					ppkthdr->ph_nextHdr = NULL;
					m=ppkthdr->ph_mbuf;
					assert(dp);
					dp->net_stats.rx_dropped ++;
					mBuf_freeMbufChain(m);
					ppkthdr = fpkthdr;
				}
				goto out;
			}
			ppkthdr = fpkthdr; /*get next reassembled pkt*/
		}
	}
	else if (reason == FWDENG_RTKPROTO) 
	{
		if (rtl865x_MPTest_Process(pPkt) == SUCCESS)
			goto out;
	}
	else
	{
#ifdef RE865X_STP_BPDU_TXRX
		if(memcmp(&m->m_data[0],STPmac,6)==0 )
		{
			//It's a BPDU
			assert(pPkt->ph_portlist<RTL8651_AGGREGATOR_NUMBER);
			dev = _rtl86xx_dev.stp_port[pPkt->ph_portlist];
		}else
#endif
		{
			dev = _rtl86xx_dev.dev[pPkt->ph_vlanIdx];
#ifdef CONFIG_RTL865XB_3G				
			/* GST 3G project */
			#ifdef RE_CORE_DEBUG
			printk("%s %d\n",__FUNCTION__,__LINE__);
			printk("pPkt->ph_vlanIdx = %x\n",pPkt->ph_vlanIdx);
			#endif
			if (wanDevModification == TRUE &&  wanDev && pPkt->ph_vlanIdx==0x02)
			{
				#ifdef RE_CORE_DEBUG
				printk("%s: FWDENG_3G_WAN\n",__FUNCTION__);
				#endif
				dev = wanDev;
			}
#endif
		}
		assert(dev);
		dp=(struct dev_priv *)dev->priv;
		assert(dp);
		//send original packet to protocol stack and allocate a new
		//one to refill NIC Rx ring.
		if(FAILED==rtl865x_toProtocolStack(pPkt, dev)){
			assert(dp);
			dp->net_stats.rx_dropped++;
			mBuf_freeMbufChain(m);//drop the packet....reclaim pkthdr and mbuf only.
			goto out;
		}
	}
		//stop NIC to give protocol stack a chance to process this pkt.
		return 1; 
out:	
	//successfully handled by driver, go Rx next pkt.
	return 0;		
}


/*
 * RTL865X_ISR_RECV,
 *  If defined, we will wake up rxThread() right away! (faster)
 *  If not defined, we will schedule rxThread() in tasklet. (slower)
 */
#define RTL865X_ISR_RECV

__IRAM void re865x_interrupt(int32 irq, void *dev_instance, struct pt_regs *regs){
#ifdef RTL865X_ISR_RECV
	if(swNic_intHandler(NULL))
		rtl8651RxTasklet->func( 0 );
#else
	if(swNic_intHandler(NULL))
		tasklet_schedule(rtl8651RxTasklet);
#endif
}

/*
 * implemenation spec:
 * riseEdge -> set init_button_count = 100
 *          -> set init_button_event = 1
 */
/* 1 re865x_timer interrupt
 * ==(10ms)*FREQ==100ms in uClinux
 * spec: 3sec=3000ms*
 */ 
#if defined(CONFIG_RTL865X_INIT_BUTTON)
#define INIT_BUTTON_3SEC_LOOP_COUNT (3000/(FREQ*10))
static spinlock_t gpio_spin_lock;
void re865x_gpioInterrupt(int32 irq, void *dev_instance, struct pt_regs *regs){
	uint32 intPending;
	//spin_lock(&_rtl86xx_dev.lock);
	spin_lock(&gpio_spin_lock);
	/* Read the interrupt status register */
    intPending = REG32(PABISR);
	if (intPending & 0x80000000){
		/* clr interrupt */
		REG32(PABISR) = (uint32)0x80000000;
		
		/* swap rising/faling mask bit
		 * falling edge -- bit 30 -- push init button
		 * rising edge  -- bit 31 -- rls  init button
		 */
		if(REG32(PABIMR) &  0x40000000){
			/* push inint button int */
			printk("\npush init button");
			REG32(PABIMR) &=  ~0x40000000; /* x0 */ /* clr push init button int */
			REG32(PABIMR) |=   0x80000000; /* 10 */ /* enable rls init button int */
			if (init_button_event==0){
				init_button_event=1;
				init_button_count=INIT_BUTTON_3SEC_LOOP_COUNT;
			}else
				init_button_event=0;
		}else{
			printk("\nrls init button");
			/* rls init button int */
			REG32(PABIMR) &=  ~0x80000000; /* 0x */ /* clr rls init button int */
			REG32(PABIMR) |=   0x40000000; /* 01 */ /* enable push init button init */
			init_button_event=0;
			init_button_count=0;
		}
	}
	//spin_unlock(&_rtl86xx_dev.lock);
	spin_unlock(&gpio_spin_lock);
} /* end re865x_gpioInterrupt */
#endif /* CONFIG_RTL865X_INIT_BUTTON */

static int re865x_open (struct net_device *dev)
{
	struct dev_priv *dp = dev->priv;
	if(!_re865xNIC_init){
		if(SUCCESS!=swNic_init(128,128,16,PKT_BUF_SZ, rtl865x_fastRx ,NULL)){
			printk("Failed to initialize NIC!!!\n\n\n");
			return -1;
		}
	}
	_re865xNIC_init++;

	if (netif_msg_ifup(dp))
		printk(KERN_DEBUG "%s: enabling interface\n", dev->name);
	if (_rtl86xx_dev.ready)
		netif_start_queue(dev);
	flashdrv_init();
	rtl865x_MPTest_setRTKProcotolMirrorFlag(0);
	return 0;
}


static int re865x_close (struct net_device *dev)
{
	struct dev_priv *dp = dev->priv;

	if (netif_msg_ifdown(dp))
		printk(KERN_DEBUG "%s: disabling interface\n", dev->name);

	netif_stop_queue(dev);
	return 0;
}

static int re865x_start_xmit (struct sk_buff *skb, struct net_device *dev){
	struct dev_priv		*dp = dev->priv;
	struct rtl_mBuf		*Mblk;
	uint32	len = skb->len;
	int32 iphdrOffset=-1;
	int32 retval=FAILED;
	unsigned short EtherType, vid=0;
	unsigned short PppProtocolType;	
#ifdef RE865X_STP_BPDU_TXRX
	if(!dev->irq){
		//virtual interfaces have no IRQ assigned. We use this to identify STP port interfaces.
		struct net_device *dev2=_rtl86xx_dev.dev[1]; //Ah....bad habit.....assume eth1 is LAN..
		struct dev_priv *dp2 = dev2->priv;	
		assert(RE865X_DEFAULT_ETH_INTERFACES==2);//add an assert. If assumption changed.....
		vid= dp2->id;
	}else
#endif
	{
		if(dp->portnum==0) /*no member ports in this vlan, no need to send*/
			goto no_member;
		vid = dp->id;
	}
#ifdef CONFIG_RTL865X_CACHED_NETWORK_IO
	Mblk=mBuf_attachHeader((void*)skb->data,(uint32)(skb),2048, len,0);
#else
	Mblk=mBuf_attachHeader((void*)UNCACHE(skb->data),(uint32)(skb),2048, len,0);
#endif
	if(!Mblk)
		goto no_buffer;

	/* init packet property before sending it in ROMEDRV */
	Mblk->m_pkthdr->ph_pkt_property = 0;
	
#ifdef RE865X_STP_BPDU_TXRX
	if(!dev->irq){
		uint8 stpPortNum= dev->name[strlen(dev->name)-1]-'0';
		Mblk->m_pkthdr->ph_portlist=1<<stpPortNum;  //by explicitly assign a port number would skip L2 DA lookup in fwd engine.
		//printk("%s: stp send to %x\n", dev->name, 1<<stpPortNum);
		/* set packet property : STP */
		Mblk->m_pkthdr->ph_pkt_property |= PH_PKT_PROPER_STPCTL;
	}else
#endif
	Mblk->m_pkthdr->ph_portlist=0; //Not yet assigned.

	/*identify where the iphdr is */
	EtherType = *((unsigned short*)(skb->data + 12));
	
	#ifdef RE_CORE_DEBUG
	printk("%s EtherType = %x\n",__FUNCTION__,EtherType);
	#endif
	
	if(EtherType==0x0800)  /* etherhdr */
		iphdrOffset=14;
	else if(EtherType==0x8864){ /* etherhdr/pppoehdr/ppphdr/ip */
		PppProtocolType = *((unsigned short*)(skb->data + 22));
		if (PppProtocolType==0x0021)
			iphdrOffset=24;
		else
			iphdrOffset=22;
		}
	/* No matter xmit success or failed, pkt is freed inside this function */
	retval=rtl8651_fwdEngineSend((void*)Mblk->m_pkthdr,vid,iphdrOffset);

	if(retval&FWDENG_DROP){
		goto xmit_error;
	}
	dev->trans_start = jiffies;
	dp->net_stats.tx_packets++;
	dp->net_stats.tx_bytes+=len;
	return 0;

no_member:
	printk("%s:no member port.\n",dev->name);
	dp->net_stats.tx_dropped++;	
	/*Error occurred. pkt would be freed by dev.c */
	return -1;
no_buffer:
	printk("%s:no buffer\n",dev->name);
	dp->net_stats.tx_dropped++;	
	/*Error occurred. pkt would be freed by dev.c */	
	return -1;
xmit_error:
	printk("%s:xmit error. errno:%d\n",dev->name, retval);
	dp->net_stats.tx_errors++;	
	/*Error occurred. but pkt already freed in fwdEngine so return success */
	return 0;

}

static void re865x_tx_timeout (struct net_device *dev)
{
	printk("%s:Tx Timeout!!! Can't send packet\n",__FUNCTION__);
}

int  __init rtl865x_probe (void)
{
	int32 i;
	struct	re865x_priv *rp;
	u16 fid=0, sid;
	printk("Probing RTL8651 home gateway controller...\n");
	rp = &_rtl86xx_dev;
	printk("Initialize RTL865x ASIC and driver\n");

	//disable all interrupts
    	REG32(CPUIIMR) = 0x00;
	/* Stop the chip's Tx and Rx DMA processes. */
    	REG32(CPUICR) &= ~(TXCMD | RXCMD);

	//Initialize table driver (ctrl plane)
	INIT_CHECK(rtl8651_tblDrvInit(NULL));
	fid=0; sid=0;

	INIT_CHECK(rtl8651_addSpanningTreeInstance(sid));
	//disable SPT. ASIC would ignore spanning tree status of each port.
	INIT_CHECK(rtl8651_setSpanningTreeInstanceProtocolWorking(sid, FALSE)); 
	INIT_CHECK(rtl8651_addFilterDatabase(fid));
	INIT_CHECK(rtl8651_specifyFilterDatabaseSpanningTreeInstance(fid, sid));

	//Initialize forwarding engine (data plane)
	/*If you pass a non-NULL pointer to rtl8651_fwdEngineInit(),
	* Please make sure that ALL parameters are set properly
	* For example, did you set the "mbufHeadroom" field?
	*/
	INIT_CHECK(rtl8651_fwdEngineInit(NULL));

	//Register user defined logging function. To turn on each log module, please
	//use rtl8651_enableLogging()
	INIT_CHECK(rtl8651_installLoggingFunction((void*)myLoggingFunction));
	
	/* Don't reply ARP requests in fwd engine, always let protocol stack do it. */
	INIT_CHECK(rtl8651_fwdEngineArp(0));
	/* Don't reply ICMP echo requests in fwd engine, always let protocol stack do it. */
	INIT_CHECK(rtl8651_fwdEngineIcmp(0));

	/*Let fwd engine send TIME exceed ICMP mesg. */
	INIT_CHECK(rtl8651_fwdEngineIcmpRoutingMsg(1));

	//Turn on fwd engine's L3/4 data forwarding capability
	INIT_CHECK(rtl8651_fwdEngineProcessL34(1)); 
	
	/* don't forward icmp request to dmz host, reply it by gatway */
	INIT_CHECK(rtl8651_fwdEngineDMZHostIcmpPassThrough(0));

	//Always disable ASIC auto learn new TCP/UDP/ICMP flows
	INIT_CHECK(rtl8651_enableNaptAutoAdd(0));

	//config NAPT port range used in ASIC and driver
	//rtl8651_setNaptTcpUdpPortRange(32768,63353);

	//init PHY LED style
	#if defined(CONFIG_RTL865X_BICOLOR_LED)
		#ifdef BICOLOR_LED_VENDOR_BXXX
			REG32(LEDCR) |= (1 << 19); // 5 ledmode set to 1 for bi-color LED
			REG32(PABCNR) &= ~0x001f0000; /* set port port b-4/3/2/1/0 to gpio */
			REG32(PABDIR) |=  0x001f0000; /* set port port b-4/3/2/1/0 gpio direction-output */
		#else
			//8650B demo board default: Bi-color 5 LED
			WRITE_MEM32(LEDCR, READ_MEM32(LEDCR) | 0x01180000 ); // bi-color LED
		#endif
		
		/* config LED mode */
		WRITE_MEM32(SWTAA, PORT5_PHY_CONTROL);
		WRITE_MEM32(TCR0, 0x000002C2); //8651 demo board default: 15 LED boards
		WRITE_MEM32(SWTACR, CMD_FORCE | ACTION_START); // force add
#ifndef RTL865X_TEST
		while ( (READ_MEM32(SWTACR) & ACTION_MASK) != ACTION_DONE ); /* Wait for command done */
#endif /* RTL865X_TEST */

	#else /* CONFIG_RTL865X_BICOLOR_LED */

		/* config LED mode */
		WRITE_MEM32(LEDCR, 0x00000000 ); // 15 LED
		WRITE_MEM32(SWTAA, PORT5_PHY_CONTROL);
		WRITE_MEM32(TCR0, 0x000002C7); //8651 demo board default: 15 LED boards
		WRITE_MEM32(SWTACR, CMD_FORCE | ACTION_START); // force add
#ifndef RTL865X_TEST
		while ( (READ_MEM32(SWTACR) & ACTION_MASK) != ACTION_DONE ); /* Wait for command done */
#endif /* RTL865X_TEST */

	#endif /* CONFIG_RTL865X_BICOLOR_LED */

	#if defined(CONFIG_RTL865X_DIAG_LED)
	REG32(PABCNR) &= ~0x40000000; /* set port a-6 to gpio */
	REG32(PABDIR) |=  0x40000000; /* set port a-6 to output */
	REG32(PABDAT) |=  0x40000000; /* set port a-6 to output 1, but this pin is active low, equivalent to output 0 (turn off led) */
	#endif /* CONFIG_RTL865X_DIAG_LED */

	#if defined(CONFIG_RTL865X_INIT_BUTTON)
	REG32(PABCNR) &= ~0x80000000; /* set port a-7 to gpio */
	REG32(PABDIR) |=  0x80000000; /* set port a-7 to input */
	//REG32(PABISR) |=  0x80000000; /* wirte 1 to port a-7 to clear this int */
	REG32(PABIMR) |=  0x40000000; /* enable falling edge int (push) by setting 30 to 1 */
	#endif /* CONFIG_RTL865X_INIT_BUTTON */

	memset(rp,0,sizeof(struct re865x_priv));	
	synchronize_irq();
	udelay(10);

	spin_lock_init (&_rtl86xx_dev.lock);
	rtl865xSpinlock = &_rtl86xx_dev.lock;
	_re865xNIC_init=0;

	INIT_CHECK(rtlglue_init());

	printk("   Initialize mbuf...\n");
	INIT_CHECK(mBuf_init(256,  0/* use external cluster pool */, 256, PKT_BUF_SZ, 0)); 

	//create all default 2 VLANs
	printk("   creating default %d interfaces...", RE865X_DEFAULT_ETH_INTERFACES);
	for(i=0;i<RE865X_DEFAULT_ETH_INTERFACES;i++){
		struct net_device *dev;
		struct dev_priv	  *dp;
		int rc = 0;
		rp = &_rtl86xx_dev;
		dev = alloc_etherdev(sizeof(struct dev_priv));
		if (!dev){
			printk("failed to allocate eth%d\n", i);
			return -1;
		}
		SET_MODULE_OWNER(dev);
		dp = dev->priv;
		memset(dp,0,sizeof(*dp));
		dp->dev = dev;
		dev->open = re865x_open;
		dev->stop = re865x_close;
		dev->set_multicast_list = re865x_set_rx_mode;
		dev->hard_start_xmit = re865x_start_xmit;
		dev->get_stats = re865x_get_stats;
		dev->do_ioctl = re865x_ioctl;
		dev->tx_timeout = re865x_tx_timeout;
		dev->watchdog_timeo = TX_TIMEOUT;
		dev->irq = ICU_NIC;
		
		//if( i == 2 )
		//{
		//	memset(dev->name, 0, sizeof(dev->name));
		//	memcpy(dev->name, "ppp0", 4);
		//}
		
		//if( i < 2 )	
			rc = register_netdev(dev);
		
		#ifdef RE_CORE_DEBUG
		printk("dev->name=%s, %d\n",dev->name, __LINE__);
		#endif
		if(!rc){
			_rtl86xx_dev.dev[i]=dev;
			printk("eth%d ", i);
		}else{
			printk("Failed to allocate eth%d\n", i);
			return -1;
		}
		if(i==0){//only need to do once. 
			int rc;
			rc = request_irq(dev->irq, re865x_interrupt, SA_INTERRUPT, dev->name, dev);
			#if defined(CONFIG_RTL865X_INIT_BUTTON)
			rc = request_irq(ICU_GPIO, re865x_gpioInterrupt, SA_INTERRUPT, dev->name, dev);
			#endif /* CONFIG_RTL865X_INIT_BUTTON */

			_rtl86xx_dev.sec_count=jiffies;
			_rtl86xx_dev.ready=1;
			init_timer(&_rtl86xx_dev.timer);
			_rtl86xx_dev.timer.expires = jiffies + 3*HZ;
			_rtl86xx_dev.timer.data = (unsigned long)dev;
			_rtl86xx_dev.timer.function = &re865x_timer;/* timer handler */
			add_timer(&_rtl86xx_dev.timer);
#if MNQ_10MS
			init_timer(&_rtl86xx_dev.timer2);
			_rtl86xx_dev.timer2.expires = jiffies + 3*HZ;
			_rtl86xx_dev.timer2.data = (unsigned long)0;
			_rtl86xx_dev.timer2.function = &re865x_timer2;/* timer handler */
			add_timer(&_rtl86xx_dev.timer2);			
#endif			
		}
	}
	printk("...OK\n");
	
	
	//#ifdef RE_CORE_DEBUG
	{
		int i;
		for(i = 0; i < RTL8651_VLAN_NUMBER; i++)
			printk("RTL8651_VLAN_NUMBER = %d, dev->name = %s, %d\n", RTL8651_VLAN_NUMBER, _rtl86xx_dev.dev[i]->name, __LINE__);
	}
	//#endif

#ifdef RE865X_STP_BPDU_TXRX
	//create 9 port interfaces for STP protocol
	printk("   creating port0~port%d for 802.1d/1s/1w...", RTL8651_AGGREGATOR_NUMBER);
	for(i=0;i<RTL8651_AGGREGATOR_NUMBER;i++){
		struct net_device *dev;
		struct dev_priv	  *dp;
		int rc;
		rp = &_rtl86xx_dev;
		{
			int alloc_size;

			/* ensure 32-byte alignment of the private area */
			alloc_size = sizeof (*dev) + sizeof (struct dev_priv) + 31;
			dev = (struct net_device *) kmalloc (alloc_size, GFP_KERNEL);
			if(dev){
				memset(dev, 0, alloc_size);
				if (sizeof (struct dev_priv))
					dev->priv = (void *) (((long)(dev + 1) + 31) & ~31);
				ether_setup(dev);
				strcpy(dev->name, "port%d");	
			}else{
				printk("failed to allocate port%d", i);
				return -1;
			}
		}
		SET_MODULE_OWNER(dev);
		dp = dev->priv;
		memset(dp,0,sizeof(*dp));
		dp->dev = dev;
		dev->open = re865x_open;
		dev->stop = re865x_close;
		dev->set_multicast_list = NULL;
		dev->hard_start_xmit = re865x_start_xmit;
		dev->get_stats = re865x_get_stats;
		dev->do_ioctl = re865x_ioctl;
		dev->tx_timeout = NULL;
		dev->watchdog_timeo = TX_TIMEOUT;
		dev->irq = 0; //virtual interfaces has no IRQ allocated
		rc = register_netdev(dev);
		if(!rc){
			_rtl86xx_dev.stp_port[i]=dev;
			printk("%d ", i);
		}else{
			printk("Failed to allocate port%d\n", i);
			return -1;
		}
	}	
	printk("..OK!\n");
#endif
	
#ifdef CONFIG_RTL8305S
	rtl8305s_init();
#endif
	rtl8651RxTasklet=&_rtl86xx_dev.rx_tasklet;
	tasklet_init(&_rtl86xx_dev.rx_tasklet, swNic_rxThread, (unsigned long)0);
	rtl865x_init_procfs();

	//register new link change callback function for DHCPC renew!
	rtl8651_installPortStatusChangeNotifier(recore_updatePortStatus);

#if defined(CONFIG_RTL865XB) && defined(CONFIG_RTL865X_WATCHDOG)
	/*enable watchdog timer. Reboot when 2^18 watchdog cycles reached*/
	REG32(WDTCNR)=(0x00<<WDTE_OFFSET)|WDTCLR|(3<<21/*2^18*/)|WDTIND;
#else
	/*disable watchdog timer.*/
	REG32(WDTCNR)= (WDSTOP_PATTERN<<WDTE_OFFSET)|WDTIND;
#endif	

	return 0;
}

static void __exit rtl865x_exit (void)
{
	rtl865x_cleanup_procfs();
	return;
}

module_init(rtl865x_probe);
module_exit(rtl865x_exit);




///////////////////////////////////////////////////////////////////////
///		ioctl interface of RTL865X driver
///////////////////////////////////////////////////////////////////////
#define RTL8651_IOCTL_FUNCTIONS

#if !defined(MAX_PORT_NUM)
#define MAX_PORT_NUM 9
#endif

#ifdef _RTL_LOGGING
#ifndef _RTL_NEW_LOGGING_MODEL
#define RTL_MAX_LOG_MODULES 9
static char *log_buf[RTL_MAX_LOG_MODULES]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
static int reg_proc=0;
static int proc_read(char* buffer,char** start, off_t  offset,int count, int* eof, void*  data)
{
	char *buf=buffer;
	int i;
	//struct proc_dir_entry *file = (struct proc_dir_entry*)data;	
	for(i=0;i<RTL_MAX_LOG_MODULES;i++)
		buf+=sprintf(buf,"%x\n",(uint32)log_buf[i]);
	return (buf-buffer);
}
int32 myLoggingFunction
(
unsigned long  dsid,
unsigned long  moduleId,
unsigned char  proto,
char           direction,
unsigned long  sip,
unsigned long  dip,
unsigned short sport,
unsigned short dport,
unsigned char  type,
unsigned char  action,
char         * msg
)
//(BYTE proto,CHAR direction,DWORD sip,DWORD tip,WORD sport,WORD dport,BYTE type,BYTE action,CHAR *msg)
{
	uint8 * pSip=(uint8*)&sip;
	uint8 * pDip=(uint8*)&dip;
	uint8 protoStrIdx=0;
	char *pProto[]={"TCP","UDP","ICMP","--"};
	char *pDir[]={"LAN->WAN","WAN->LAN"};
	char *pAction[]={"Unknown","Drop","Reset","Unknown","Forward"};
	struct circ_buf *cbuf;
	struct timeval tv;
	struct rtc_time tm;
	unsigned long	moduleId_tmp;
	unsigned long	idx;
	assert(direction>=0);
	/*
		Calculate Index of log_buf
	*/
	idx = 0;
	moduleId_tmp = moduleId;
	while (moduleId_tmp)
	{
		idx ++;
		moduleId_tmp = moduleId_tmp >> 1;
	}
	if (idx)
	{
		idx --;
	}
	assert(idx<RTL_MAX_LOG_MODULES);

	do_gettimeofday(&tv);
	tv.tv_sec+=timezone_diff;
	to_tm(tv.tv_sec, &tm);	

	//if (dsid!=0) return FAILED;
//	if (sip==0||dip==0||dport==0||sport==0||msg==NULL)
//		return FAILED;
	if (msg==NULL)
		return FAILED;
	//allocate circular buffer for log
	if(log_buf[idx]==NULL)
	{	
		uint32 buf_size = 16000;
		uint32 total_size = buf_size + sizeof(struct circ_buf);	// our buffer size

		if(reg_proc==0)
		{
			struct proc_dir_entry* pProcDirEntry = NULL;	
			pProcDirEntry = create_proc_entry("log_module", 0444, NULL );
			pProcDirEntry->uid=0;
			pProcDirEntry->gid=0;
			pProcDirEntry->read_proc=proc_read;
			pProcDirEntry->write_proc=NULL;
			reg_proc=1;
		}			

		log_buf[idx]=kmalloc(total_size,GFP_ATOMIC);
	
		if(log_buf[idx]==NULL) {printk("out of memory!\n");return FAILED;}

		cbuf=(struct circ_buf *)log_buf[idx];
		
		cbuf->size=buf_size;

		cbuf->head = cbuf->tail = 0;		
	}
	else
	{
		cbuf=(struct circ_buf *)log_buf[idx];
	}

	switch(proto){
		case 6 : protoStrIdx=0;break; /* TCP */
		case 17: protoStrIdx=1;break; /* UDP */
		case 1 : protoStrIdx=2;break; /* ICMP */
		default: protoStrIdx=3;break; /* others */
	}
	//sprintf(logMsgBuf,"url '%s' from host(%u.%u.%u.%u) filtered!\n",url,pIp[0],pIp[1],pIp[2],pIp[3]);
	if (protoStrIdx==3){ /* others */
		sprintf(logMsgBuf,"%u%04d-%02d-%02d %02d:%02d:%02d  [%s] protocol:%u %s %u.%u.%u.%u->%u.%u.%u.%u action:%s\n",
			(uint32)dsid,
			tm.tm_year,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec,
			msg,proto,pDir[(unsigned)direction],
			pSip[0],pSip[1],pSip[2],pSip[3],
			pDip[0],pDip[1],pDip[2],pDip[3],
			pAction[action]);
	}else if (protoStrIdx==2){ /* ICMP */
		sprintf(logMsgBuf,"%u%04d-%02d-%02d %02d:%02d:%02d  [%s] %s %s %u.%u.%u.%u->%u.%u.%u.%u type:%u action:%s\n",
			(uint32)dsid,
			tm.tm_year,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec,	
			msg,pProto[protoStrIdx],pDir[(unsigned)direction],
			pSip[0],pSip[1],pSip[2],pSip[3],
			pDip[0],pDip[1],pDip[2],pDip[3],
			type,
			pAction[action]);
	}else{
		sprintf(logMsgBuf,"%u%04d-%02d-%02d %02d:%02d:%02d  [%s] %s %s %u.%u.%u.%u/%u->%u.%u.%u.%u/%u action:%s\n",
			(uint32)dsid,
			tm.tm_year,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec,		
			msg,pProto[protoStrIdx],pDir[(unsigned)direction],
			pSip[0],pSip[1],pSip[2],pSip[3],sport,
			pDip[0],pDip[1],pDip[2],pDip[3],dport,
			pAction[action]);
	}
	circ_msg(cbuf,logMsgBuf);
	return SUCCESS;
} /* end myLoggingFunction */
#endif /* _RTL_NEW_LOGGING_MODEL */
#endif /* _RTL_LOGGING */



static int re86xx_set_vlan_type(struct net_device *dev,int type)
{
	struct dev_priv *dp = dev->priv;
	int rc;
	unsigned long i;
	rc=rtl8651_addNetworkIntf(dev->name);
	//printk("Adding '%s', ret=%d\n", dev->name, rc);
	if (type==RTL8651_LL_PPPOE)
	{
       	//warn(rtl8651_setAsicNaptAutoAddDelete(FALSE, FALSE));
       	warn(rtl8651_enableNaptAutoAdd(FALSE));
		for (i=0;i<RTL8651_MAX_DIALSESSION;i++)
			warn(rtl8651_addPppoeSession(i,dp->id));
		warn(rtl8651_specifyNetworkIntfLinkLayerType(dev->name,RTL8651_LL_PPPOE,dp->id));
	}
	else
	{
		if ((rc = rtl8651_specifyNetworkIntfLinkLayerType(dev->name,type,dp->id)))
			printk("rtl8651_specifyNetworkIntfLinkLayerType( %s,  %d)=%d\n", dev->name, dp->id, rc);
	}
	return 0;
}

static int re86xx_del_vlan_ip(struct net_device *dev)
{
	struct dev_priv *dp = dev->priv;
	warn(rtl8651_delRoute((ipaddr_t)0, (ipaddr_t)0));
   	warn(rtl8651_delNaptMapping(dp->ip));
	warn(rtl8651_delIpIntf(dev->name,dp->ip,dp->mask));
	return 0;
}

static int re86xx_set_pppoe_ip(struct net_device *dev,int ip,int mask,int id )
{
	struct dev_priv *dp = dev->priv;
	//struct re865x_priv *rp = dp->priv;
	dp->ip=ip;
	dp->mask=mask;
	if (!ip) 
	{
		printk("setting ip  %x not valid\n",ip);
		return 0;
	}
	warn(rtl8651_addIpIntf(dev->name,ip, mask));
	warn(rtl8651_bindPppoeSession(ip, id));
	dp->setup=1;
	return 0;
}

static int re86xx_set_vlan_ip(struct net_device *dev,int ip,int mask )
{
	struct dev_priv *dp = dev->priv;
	//struct re865x_priv *rp = dp->priv;
	dp->ip=ip;
	dp->mask=mask;
	if (!ip) 
	{
		printk("setting ip  %x not valid\n",ip);
		return 0;
	}
	warn(rtl8651_addIpIntf(dev->name,ip, mask));
	dp->setup=1;
	return 0;
}

static int re86xx_get_conf(struct net_device *dev,int dst)
{
	u32 *data;
	struct dev_priv *dp = dev->priv;
	u32  info[4];
	data = (u32*)dst;
	info[0]=dp->ip;
	info[1]=dp->mask;
	info[2]=dp->gateway;
	info[3]=dp->dns;

	if (copy_to_user(data,&info,16))
	{
		printk("copy error\n");
	}
	return 0;
}


static int set_vlan_ext_interface(struct net_device *dev,int direction,u32 ip)
{
	warn((rtl8651_addExtNetworkInterface(dev->name)));
	warn(rtl8651_addNaptMapping((ipaddr_t)ip));
	return 0;
}

static int re86xx_set_net_para(void)
{
#if 1 
	rtl8651_setNaptIcmpTimeout(60);
	rtl8651_setNaptUdpTimeout(60*15);
	rtl8651_setNaptTcpLongTimeout(3600);
	rtl8651_setNaptTcpMediumTimeout(900);
	rtl8651_setNaptTcpFastTimeout(20);
	rtl8651_setNaptTcpFinTimeout(20);
#endif
	return 0;
}


static int re86xx_add_vlan(struct net_device *dev,int id,int port,int mac_addr)
{
	struct dev_priv *dp = dev->priv;
	u16 fid=0,i;
	int rc;
	char *mac=(char*)mac_addr;

	dp->id = id;
	if(port==0){
		printk("No member port on vlan %d\n", id);
		return -1;
	}		
	dp->portmask =  port;
	dp->portnum  =0;
	for(i=0;i<RTL8651_AGGREGATOR_NUMBER;i++){
		if(port & (1<<i))
			dp->portnum++;
	}		
	_rtl86xx_dev.dev[id&0x7]=dev;
	
	//#ifdef RE_CORE_DEBUG
	printk(" id %x port %x,[1]= %s [2] = %s, %d\n",id,port, _rtl86xx_dev.dev[id&0x7]->name, dev->name, __LINE__);
	//#endif
	
	if ((rc = rtl8651_addVlan(dp->id))!=SUCCESS)
		printk("add vlan fail %d \n",rc);
	if (dp->id == 8)
	    rtl8651_assignVlanMacAddress(dp->id,(ether_addr_t*)mac,1);
	else
	    rtl8651_assignVlanMacAddress(dp->id,(ether_addr_t*)mac,1);
	
	memcpy((char*)dev->dev_addr,(char*)mac,6);

	rtl8651_specifyVlanFilterDatabase(dp->id, fid);
	//printk("Adding member port 0x%x for vlan %d\n", dp->portmask, dp->id);
	for(i=0; i<MAX_PORT_NUM; i++)
	{
		if(dp->portmask& (1<<i))
		{
			int ret;

			/* record VLAN ID for passthru */
			passthruPortMappingToVlan[i] = id;
			
			if(i>=RTL8651_PORT_NUMBER){
				rtl8651_setEthernetPortLinkStatus(i, TRUE);
				printk("Bring up ext  port %d..\n", i);
			}
			ret=rtl8651_addVlanPortMember(dp->id, i);
			if(ret)
				printk("Adding port %d to vlan %d...Err: %d\n", i, dp->id, ret);
			ret=rtl8651_setPvid(i, dp->id);
			if(ret)
				printk("Set pvid %d to port %d...Err: %d\n",  dp->id, i, ret);
				
			// if marked, Removed switch link down
			ret=rtl8651_setEthernetPortAutoNegotiation(i,1,RTL8651_ETHER_AUTO_100FULL);
			if(ret)
				printk("Set port capability  100Full(Err:%d)", ret);
				
			// RealTek change the definition of this API in ROME 3.6 
			if(strcmp(ROMEDRIVER_VERSION, "3.6"))
			{				
				ret=rtl8651_setEthernetPortDuplexMode(i,1);			
				if(ret)
					printk("full dulplex(Err:%d)\n", ret);
			}
		}
	}
	return 0;
}



static int re865x_DemandRouteCallback(unsigned int identity)
{
	printk("trigger on demand %d ,pid %d \n",identity,rtl8651_user_pid);
	sys_kill(rtl8651_user_pid,identity|40);
	return 0;
}

static int	rtl86xx_pppoeIdleHangupCallback(unsigned int id)
{	
	_rtl86xx_dev.pppoeIdleTimeout[id]=1;
	return 0;	
}



static int re865x_ethtool_ioctl (struct dev_priv *dp, void *useraddr)
{
	u32 ethcmd;

	/* dev_ioctl() in ../../net/core/dev.c has already checked
	   capable(CAP_NET_ADMIN), so don't bother with that here.  */

	if (get_user(ethcmd, (u32 *)useraddr))
		return -EFAULT;

	switch (ethcmd) {

	case ETHTOOL_GDRVINFO: {
		struct ethtool_drvinfo info = { ETHTOOL_GDRVINFO };
		strcpy (info.driver, DRV_NAME);
		strcpy (info.version, DRV_VERSION);
		if (copy_to_user (useraddr, &info, sizeof (info)))
			return -EFAULT;
		return 0;
	}

	/* get settings */
	case ETHTOOL_GSET: {
		struct ethtool_cmd ecmd = { ETHTOOL_GSET };
		spin_lock_irq(&_rtl86xx_dev.lock);
		spin_unlock_irq(&_rtl86xx_dev.lock);
		if (copy_to_user(useraddr, &ecmd, sizeof(ecmd)))
			return -EFAULT;
		return 0;
	}
	/* set settings */
	case ETHTOOL_SSET: {
		struct ethtool_cmd ecmd;
		if (copy_from_user(&ecmd, useraddr, sizeof(ecmd)))
			return -EFAULT;
		spin_lock_irq(&_rtl86xx_dev.lock);
		spin_unlock_irq(&_rtl86xx_dev.lock);
		return 0;
	}
	/* restart autonegotiation */
	/* get link status */
	case ETHTOOL_GLINK: {
		struct ethtool_value edata = {ETHTOOL_GLINK};
		if (copy_to_user(useraddr, &edata, sizeof(edata)))
			return -EFAULT;
		return 0;
	}

	/* get message-level */
	case ETHTOOL_GMSGLVL: {
		struct ethtool_value edata = {ETHTOOL_GMSGLVL};
		edata.data = dp->msg_enable;
		if (copy_to_user(useraddr, &edata, sizeof(edata)))
			return -EFAULT;
		return 0;
	}
	/* set message-level */
	case ETHTOOL_SMSGLVL: {
		struct ethtool_value edata;
		if (copy_from_user(&edata, useraddr, sizeof(edata)))
			return -EFAULT;
		dp->msg_enable = edata.data;
		return 0;
	}

	default:
		break;
	} /* end switch */

	return -EOPNOTSUPP;
}


 int re865x_ioctl (struct net_device *dev, struct ifreq *rq, int cmd)
{
	struct dev_priv *dp = dev->priv;
	int32 rc = 0;
	int32  ret=FAILED;
	int32  * pRet;
	unsigned long args[4];
	unsigned long *data;
	u32	stub,id;
	int8 *pStr,char0;
	void *pPtr,*pPtr2;	
	u32 *pU32,*pU32_1,*pU32_2,*pU32_3,u32_0;
	u8 *pU8,*pU8_2,*pU8_3,*pU8_4;
	u16 *pU16_1,*pU16_2,*pU16_3,*pU16_4;
	uint32 *ptr;
	// char *mac;
	pppoeCfg_t    *pppCfg; 



	if (cmd != SIOCDEVPRIVATE)
		goto normal;

	data = (unsigned long *)rq->ifr_data;
	if (copy_from_user(args, data, 4*sizeof(unsigned long)))
		return -EFAULT;

	#if 0
	printk("\n\tioctl(%d): %d,  %d,  %d \n",args[0],args[1],args[2],args[3]);
	#endif

	switch(args[0])
	{
		#if defined(CONFIG_RTL865X_DIAG_LED)||defined(CONFIG_RTL865X_INIT_BUTTON)
		case RTL8651_IOCTL_DIAG_LED:
			switch(args[1]){
				case 1:
					gpioLed(1);
					break;
				case 2:
					gpioLed(2);
					break;
				case 3:
					gpioLed(3);
					break;
				case 5:
					gpioLed(5);
					break;
				case 9:
					gpioLed(9);
					break;
				case 100:
					{
						void (*start)(void);
						printk("\nFactory Default done! Restarting system...\n");
						REG32(GIMR)=0;
						start = (void(*)())FLASH_BASE;	
						start();
					}
					break;
				default:
					break;
			} /* end switch */
			break;
		#endif /* CONFIG_RTL865X_DIAG_LED || CONFIG_RTL865X_INIT_BUTTON */
		case ADD_VLAN:
			re86xx_add_vlan(dev,args[1],args[2],args[3]);
			break;
		case ADD_SIGNAL_RECEIVE_PROCESS:
			rtl8651_user_pid = args[1];
			printk("rtl8651_user_pid set to %d\n",rtl8651_user_pid);
			break;
		case RTL8651_IOCTL_ADDDEMANDROUTE:
			ret =  rtl8651_addDemandRoute((rtl8651_tblDrvDemandRoute_t *)args[1],args[2],re865x_DemandRouteCallback);
			pRet = (int32 *)args[3];				
			*pRet = ret;
			break;

		case RTL8651_IOCTL_FLUSHDEMANDROUTE:
			ret =  rtl8651_flushDemandRoute(args[1]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_DELSESSION:
			ret =  rtl8651_delSession(args[1]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_BINDSESSION:
			ret =  rtl8651_bindSession((ipaddr_t)args[1],args[2]);
			pRet = (int32 *)args[3];
			*pRet = ret;
			break;			

		case RTL8651_IOCTL_ADDSESSION:
			ptr=(int32 *)(args[1]);
			pU32  = (uint32 *)ptr[0];
			pU32_1= (uint32 *)ptr[1];
			pStr  = (int8*  )ptr[2];
			pU16_1 = (uint16 *)ptr[3];
			rc = rtl8651_addSession(*pU32,*pU32_1,pStr,*pU16_1);
			pU32_2 =(uint32 *)args[2];
			*pU32_2=rc;
			break;

#if defined(CONFIG_RTL865X_PPTPL2TP)||defined(CONFIG_RTL865XB_PPTPL2TP)

		case RTL8651_IOCTL_SETPPTPPROPERTY:
			ptr=(int32 *)(args[1]);
			pU32  = (uint32 *)ptr[0];
			pU16_1= (uint16 *)ptr[1];
			pU16_2= (uint16 *)ptr[2];			
			pU32_1 = (uint32 *)ptr[3];
			rc=rtl8651_setPptpProperty(*pU32,*pU16_1,*pU16_2,(ipaddr_t)*pU32_1);
			pU32_2 =(uint32 *)args[2];
			*pU32_2=rc;
			break;
		case RTL8651_IOCTL_SETL2TPPROPERTY:
			ptr=(int32 *)(args[1]);
			pU32  = (uint32 *)ptr[0];
			pU16_1= (uint16 *)ptr[1];
			pU16_2= (uint16 *)ptr[2];			
			pU32_1 = (uint32 *)ptr[3];
			rc=rtl8651_setL2tpProperty(*pU32,*pU16_1,*pU16_2,(ipaddr_t)*pU32_1);
			pU32_2 =(uint32 *)args[2];
			*pU32_2=rc;
			break;
		case RTL8651_IOCTL_RESETPPTPPROPERTY:
			rc=rtl8651_resetPptpProperty(args[1]);
			pU32_2 =(uint32 *)args[2];
			*pU32_2=rc;
			break;
		case RTL8651_IOCTL_RESETL2TPPROPERTY:
			rc=rtl8651_resetL2tpProperty(args[1]);
			pU32_2 =(uint32 *)args[2];
			*pU32_2=rc;
			break;
#endif			

		case RTL8651_IOCTL_SETLOOPBACKPORTPHY:			
			rc=rtl8651_setLoopbackPortPHY((uint32)args[1], (int32)args[2]);
			pU32_2 =(uint32 *)args[3];
			*pU32_2=rc;
			break;
			
		case ADD_IF:
			_rtl86xx_dev.addIF=1;
			break;
		case ADD_GW:
			stub=0;
			dp->dns=args[3];
			dp->gateway=args[1];

			if (!args[1])
			{
				printk("add route ip %x\n",(u32)args[1]);
				return -1;
			}
			rc = rtl8651_delRoute(stub,stub);
			rc = rtl8651_addRoute(stub,stub,dev->name,args[1]);
			if (rc)
				printk("\nerror addRoute %d \n",rc);
			break;
		case  SET_VLAN_IP:
			re86xx_set_vlan_ip(dev,args[1],
					args[2]);
			break;
		case  SET_PPPOE_IP:
			re86xx_set_pppoe_ip(dev,args[1],
					args[2],args[3]);
			break;
		case  SET_VLAN_TYPE:
			re86xx_set_vlan_type(dev,args[1]);
			break;
		case GET_IF:
			re86xx_get_conf(dev,args[1]);
			break;
		case GET_MAC:
			copy_to_user((char*)args[1],(char*)&dev->dev_addr,6);
			break;
		case GET_IP:
			copy_to_user((char*)args[1],(char*)&dp->ip,4);
			break;
		case SET_NET_PARA:
			re86xx_set_net_para();
			break;
		case SET_EXT_INTERFACE:
			set_vlan_ext_interface(dev,EXTERNAL_INTERFACE,args[1]);
			break;
		case GET_VLAN_STAT:
			copy_to_user((char*)args[1],(char*)&dp->setup,4);
			break;
		case DEL_VLAN_IP:
			re86xx_del_vlan_ip(dev);
			break;
#if 0
		case RTL8651_FLUSHIPACL:						
			pStr=(int32 *)(args[1]);
			pU32=(int32 *)args[3];
			rc = rtl8651_flushIpAcl(pStr,(int8)args[2]);
			*pU32=rc;
			break;
#endif
		case RTL8651_FLUSHACLRULE:
			ptr=(int32 *)(args[1]);
			u32_0 = (uint32 )ptr[0];
			pStr  = (int8*  )ptr[1];
			char0 = (int8   )ptr[2];
			pU32  = (int32 *)args[3];
			rc = rtl8651a_flushAclRule((uint32)u32_0,(int8*)pStr,(int8)char0);
			//rc = rtl8651_flushAclRule((int8*)pStr,(int8)args[2]);
			*pU32=rc;
			break;
		case RTL8651_SETDEFAULTACL:
			//pStr=(int32 *)(args[1]);
			//pU32=(int32 *)args[3];
			ptr   = (uint32 *)(args[1]);
			u32_0 = (uint32  ) ptr[0];
			pStr  = (int8*   ) ptr[1];
			char0 = (int8    ) ptr[2];
			rc = rtl8651a_setDefaultAcl((uint32)u32_0,(int8*)pStr,(int8)char0);
			//rc = rtl8651a_setDefaultAcl(pStr, (int8)args[2]);
			pU32  = (uint32 *)(args[3]);
			*pU32 = rc;
			break;
		case RTL8651_ADDACL:
			ptr   =(uint32 *)args[1];
			u32_0 =(uint32  )ptr[0];
			pStr  =(int8   *)ptr[1];
			char0 =(int8    )ptr[2];
			pPtr  =(uint32 *)ptr[3];
			//rc = rtl8651_addAclRule(pStr,*pU8,(rtl8651_tblDrvAclRule_t *)pPtr);
			rc = rtl8651a_addAclRule((uint32)u32_0,(int8*)pStr,(int8)char0,(rtl8651_tblDrvAclRule_t *)pPtr);
			pU32=(uint32 *)args[3];
			*pU32=rc;
			break;	
		case RTL8651_DELACL:
			ptr   =(uint32 *)args[1];
			u32_0 =(uint32  )ptr[0];
			pStr  =(int8   *)ptr[1];
			char0 =(int8    )ptr[2];
			pPtr  =(uint32 *)ptr[3];
			rc = rtl8651a_delAclRule((uint32)u32_0,(int8*)pStr,(int8)char0,(rtl8651_tblDrvAclRule_t *)pPtr);
			pU32=(uint32 *)args[3];
			*pU32=rc;
			break;		
			
		// Dino Chang add, 2004/11/08
		case RTL8651_ADDACLEXT:
			ptr   =(uint32 *)args[1];
			u32_0 =(uint32  )ptr[0];
			pStr  =(int8   *)ptr[1];
			char0 =(int8    )ptr[2];
			pPtr  =(uint32 *)ptr[3];
			rc = rtl8651a_addAclRuleExt((uint32)u32_0,(int8*)pStr,(int8)char0,(rtl8651_tblDrvAclRule_t *)pPtr);
			pU32=(uint32 *)args[3];
			*pU32=rc;
			break;			
		case RTL8651_DELACLEXT:
			ptr   =(uint32 *)args[1];
			u32_0 =(uint32  )ptr[0];
			pStr  =(int8   *)ptr[1];
			char0 =(int8    )ptr[2];
			pPtr  =(uint32 *)ptr[3];
			rc = rtl8651a_delAclRuleExt((uint32)u32_0,(int8*)pStr,(int8)char0,(rtl8651_tblDrvAclRule_t *)pPtr);
			pU32=(uint32 *)args[3];
			*pU32=rc;
			break;			
		// Dino Chang	
				
		case RTL8651_ADDNAPTSERVERPORTMAPPING:
			ptr=(uint32 *)args[1];
			u32_0 = (uint32)ptr[0];
			pU8 =	(uint8 *)ptr[1];
			pPtr =	(void *)ptr[2];
			pU16_1 =(uint16 *)ptr[3];
			pPtr2 =	(void *)ptr[4];
			pU16_2 =(uint16 *)ptr[5];			
		
			//printk("rtl8651_addNaptServerPortMapping istcp:%u ext=%x:%u in=%x:%u ret=%u\n",*pU8,*(ipaddr_t*)pPtr,*pU16_1,*(ipaddr_t*)pPtr2,*pU16_2,rc);		

			rc = rtl8651a_addNaptServerPortMapping((uint32)u32_0,*pU8,*(ipaddr_t*)pPtr,*pU16_1,*(ipaddr_t*)pPtr2,*pU16_2);
			pU32=(uint32 *)args[3];
			*pU32=rc;
			break;

		case RTL8651_IOCTL_ADDNAPTSERVERPORTRANGE:
			ptr=(uint32 *)args[1];
			u32_0 = (uint32)ptr[0];
			pU8 =	(uint8 *)ptr[1];
			pPtr =	(void *)ptr[2];
			pU16_1 =(uint16 *)ptr[3];
			pPtr2 =	(void *)ptr[4];
			pU16_2 =(uint16 *)ptr[5];
			pU16_3 =(uint16 *)ptr[6];	
		
			//printk("rtl8651_addNaptServerPortMapping istcp:%u ext=%x:%u in=%x:%u ret=%u\n",*pU8,*(ipaddr_t*)pPtr,*pU16_1,*(ipaddr_t*)pPtr2,*pU16_2,rc);		

			rc = rtl8651a_addNaptServerPortRange((uint32)u32_0,*pU8,*(ipaddr_t*)pPtr,*pU16_1,*(ipaddr_t*)pPtr2,*pU16_2,*pU16_3);
			pU32=(uint32 *)args[3];
			*pU32=rc;
			break;



		case RTL8651_ADDNAPTUPNPPORTMAPPING:
			ptr=(uint32 *)args[1];
			pU8 =	(uint8 *)ptr[0];
			pPtr =	(void *)ptr[1];
			pU16_1 =(uint16 *)ptr[2];
			pPtr2 =	(void *)ptr[3];
			pU16_2 =(uint16 *)ptr[4];
			pU32_1=(uint32 *)ptr[5];
			pU32_2=(uint32 *)ptr[6];

			//printk("rtl8651_addNaptUpnpPortMapping istcp:%u ext=%x:%u in=%x:%u remote:%x time=%d ret=%u\n",*pU8,*(ipaddr_t*)pPtr,*pU16_1,*(ipaddr_t*)pPtr2,*pU16_2,*(ipaddr_t *)pU32_1,*pU32_2,rc);
			{
				int flag=0;
				if(*pU8) flag|=UPNP_TCP;
				//have remote host or lease time , one shot!
				if((*pU32_1)||(*pU32_2)) flag|=UPNP_ONESHOT;
					else flag|=UPNP_PERSIST;
				rtl8651_addUpnpMapLeaseTime(flag, *(ipaddr_t*)pU32_1, 0, 
					*(ipaddr_t*)pPtr,*pU16_1,*(ipaddr_t*)pPtr2,*pU16_2,*pU32_2);						
			}

			pU32=(uint32 *)args[3];
			*pU32=0;					
			break;

		case RTL8651_IOCTL_QUERYUPNPMAPTIMEAGE:
//uint32 		
			ptr=(uint32 *)args[1];
			pU32 = (uint32 *)ptr[0];
			pU32_1=(uint32 *)ptr[1];			
			pU16_1 =(uint16 *)ptr[2];
			pU32_2=(uint32 *)ptr[3];
			pU16_2 =(uint16 *)ptr[4];
			pU32_3=(uint32 *)args[3];
			*pU32_3=rtl8651_queryUpnpMapTimeAge(*pU32, *pU32_1, *pU16_1, *pU32_2,  *pU16_2);			
			break;
			
		case RTL8651_DELNAPTSERVERPORTMAPPING:
			ptr=(uint32 *)args[1];
			u32_0 = (uint32)ptr[0];
			pU8 =	(uint8 *)ptr[1];
			pPtr =	(void *)ptr[2];
			pU16_1 =(uint16 *)ptr[3];
			pPtr2 =	(void *)ptr[4];
			pU16_2 =(uint16 *)ptr[5];

			rc = rtl8651a_delNaptServerPortMapping((uint32)u32_0,*pU8,*(ipaddr_t*)pPtr,*pU16_1,*(ipaddr_t*)pPtr2,*pU16_2);
			//printk("rtl8651_delNaptServerPortMapping-istcp:%d ext=%x:%u in=%x:%u ret=%d\n",*pU8,*(ipaddr_t*)pPtr,*pU16_1,*(ipaddr_t*)pPtr2,*pU16_2,rc);
			pU32=(uint32 *)args[3];
			*pU32=rc;
			break;

		case RTL8651_IOCTL_DELNAPTSERVERPORTRANGE:
			ptr=(uint32 *)args[1];
			u32_0 = (uint32)ptr[0];
			pU8 =	(uint8 *)ptr[1];
			pPtr =	(void *)ptr[2];
			pU16_1 =(uint16 *)ptr[3];
			pPtr2 =	(void *)ptr[4];
			pU16_2 =(uint16 *)ptr[5];
			pU16_3 =(uint16 *)ptr[6];

			rc = rtl8651a_delNaptServerPortRange((uint32)u32_0,*pU8,*(ipaddr_t*)pPtr,*pU16_1,*(ipaddr_t*)pPtr2,*pU16_2,*pU16_3);
			//printk("rtl8651_delNaptServerPortMapping-istcp:%d ext=%x:%u in=%x:%u ret=%d\n",*pU8,*(ipaddr_t*)pPtr,*pU16_1,*(ipaddr_t*)pPtr2,*pU16_2,rc);
			pU32=(uint32 *)args[3];
			*pU32=rc;
			break;			
		
		case RTL8651_DELNAPTUPNPPORTMAPPING:
			ptr=(uint32 *)args[1];
			pU8 =	(uint8 *)ptr[0];
			pPtr =	(void *)ptr[1];
			pU16_1 =(uint16 *)ptr[2];
			pPtr2 =	(void *)ptr[3];
			pU16_2 =(uint16 *)ptr[4];	
			
			//printk("rtl8651_delNaptUpnpPortMapping-istcp:%d ext=%x:%u in=%x:%u ret=%d\n",*pU8,*(ipaddr_t*)pPtr,*pU16_1,*(ipaddr_t*)pPtr2,*pU16_2,rc);
			if(*pU8){
    			rtl8651_delUpnpMap(UPNP_TCP|UPNP_PERSIST, 0, 0, 
					*(ipaddr_t*)pPtr,*pU16_1,*(ipaddr_t*)pPtr2,*pU16_2);		
			}
			else{
    			rtl8651_delUpnpMap(UPNP_PERSIST, 0, 0, 
					*(ipaddr_t*)pPtr,*pU16_1,*(ipaddr_t*)pPtr2,*pU16_2);				
			}
			pU32=(uint32 *)args[3];
			*pU32=0;						
			break;
			
		case RTL8651_IOCTL_ADDDMZHOST:
			ptr=(uint32 *)args[1];
			u32_0 = (uint32)ptr[0];
			pPtr =(void *)ptr[1];	
			pPtr2 =(void *)ptr[2];
			rc = rtl8651a_addDmzHost((uint32)u32_0,*(ipaddr_t*)pPtr,*(ipaddr_t*)pPtr2);
			printk("rtl8651a_addDmzHost naptIp=%x dmzHostIp=%x return=%d\n",*(ipaddr_t*)pPtr,*(ipaddr_t*)pPtr2,rc);
			pU32=(uint32 *)args[3];
			*pU32=rc;
			break; 		

		case RTL8651_IOCTL_SETDMZHOSTL4FWD:
			ptr = (uint32 *)args[1];
			pU32 = (uint32 *)args[3];

			*pU32 = (rtl8651_fwdEngineFwdGeneralL4ToDMZ(*ptr) && rtl8651_fwdEngineFwdUnicastIGMPPkt(*ptr));
			break;

		case RTL8651_IOCTL_SETDMZHOSTICMPFWD:
			ptr = (uint32 *)args[1];
			pU32 = (uint32 *)args[3];

			*pU32 = rtl8651_fwdEngineDMZHostIcmpPassThrough(*ptr);
			break;

		case RTL8651_IOCTL_DELDMZHOST:
			ptr=(uint32 *)args[1];
			u32_0 =(uint32)ptr[0];
			pPtr =(void *)ptr[1];
			rc = rtl8651a_delDmzHost((uint32)u32_0,*(ipaddr_t*)pPtr);
			printk("rtl8651a_delDmzHost naptIp=%x return=%d\n",*(ipaddr_t*)pPtr,rc);
			pU32=(uint32 *)args[3];
			*pU32=rc;
			break; 
		case SET_DNS_GW:
			dp->dns=args[1];
			dp->gateway=args[2];
			break;
			
	 	case GET_PPPOE_STATUS: /* add pppoe interface par 2*/ 
			copy_to_user((char*)args[1],(char*)&dp->pppoe_status,4);
			break;
	 	case SET_PPPOE_STATUS: /* add pppoe interface par 2*/ 
			dp->pppoe_status=args[1];
			break;	
	 	case DEL_PPPOE: 
			dp->pppoe_status=0;
		    	warn(rtl8651_resetPppoeSessionProperty(0));	   
    	    		warn(rtl8651_delExtNetworkInterface(dev->name));
   	    		warn(rtl8651_delNaptMapping(args[1]));
			rtl8651_delRoute((ipaddr_t)0, (ipaddr_t)0);
    	    		rtl8651_delIpIntf(dev->name, dp->ip, 0xFFFFFFFF);
			break;
	 	case SET_VLAN_PPPOE_PARA: /* add pppoe interface par 2*/ 
			dp->pppoe_status=1;
			warn(rtl8651_setPppoeSessionProperty(0, 
					 (unsigned short)args[1], (ether_addr_t *)args[2], 0 ));
            		//warn(rtl8651_setAsicNaptAutoAddDelete(FALSE, TRUE));
            		warn(rtl8651_enableNaptAutoAdd(0));
		        re86xx_set_net_para();
		    break;

		case SET_SOFTPPPD_IDLE_TIME:
			warn(rtl8651_setSoftPppoeSessionHangUp(args[1],0,args[2],rtl86xx_pppoeIdleHangupCallback));
			_rtl86xx_dev.pppoeIdleTimeout[args[1]]=0;
			break;
		case SET_PPPD_IDLE_TIME:
			warn(rtl8651_setPppoeSessionHangUp(args[1],args[2],args[3],rtl86xx_pppoeIdleHangupCallback));
			_rtl86xx_dev.pppoeIdleTimeout[args[1]]=0;
			//printk("set_pppd_idle_time=%d\n",args[3]);
			break;
		case GET_PPPD_IDLE_TIME:
			//printk("get pppd idle sec_cnt=%d sec=%d\n",_rtl86xx_dev.sec_count,_rtl86xx_dev.sec);
			copy_to_user((char*)args[2],(char*)&_rtl86xx_dev.pppoeIdleTimeout[args[1]],4);
			break;
		case 1200: /* del pppoe interface */
			dp->pppoe_status=0;
		    	rtl8651_resetPppoeSessionProperty(0);		 	    	    	    
			warn(rtl8651_delRoute((ipaddr_t)0, (ipaddr_t)0));
    	    	    	warn(rtl8651_delNaptMapping(args[1]));
    	    		warn(rtl8651_delExtNetworkInterface(dev->name));
			warn(rtl8651_delIpIntf(dev->name, args[1], 0xFFFFFFFF));
			/* delete default route */
            /* delete napt mapping */
	              // if ((rc=rtl8651_delExtNetworkInterface(dev->name)))
			//	printk(" fail del pppoe ext\n");
		    break;		    		    		    
	
		case RTL8651_IOCTL_SETPPPOESESSIONPROPERTY:
			//(uint32 pppoeId, uint16 sid, ether_addr_t * macAddr, uint32 port)
			pU32=(uint32 *)args[1];
			ret =  rtl8651_setPppoeSessionProperty((uint32)pU32[0],(uint16)pU32[1],(ether_addr_t *)pU32[2],(uint32)pU32[3]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_RESETPPPOESESSIONPROPERTY:
			//(uint32 pppoeId);
			pU32=(uint32 *)args[1];
			ret =  rtl8651_resetPppoeSessionProperty((uint32)pU32[0]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_BINDPPPOESESSION:
			//(ipaddr_t ipaddr, uint32 pppoeId);
			pU32=(uint32 *)args[1];
			ret =  rtl8651_bindPppoeSession((ipaddr_t)pU32[0],(uint32)pU32[1]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_ADDPPPOESESSION:
			//(uint32 pppoeId, uint16 vid);
			pU32=(uint32 *)args[1];
			ret =  rtl8651_addPppoeSession((uint32)pU32[0],(uint16)pU32[1]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_SETPPPOEDEFAULTDIALSESSIONID:
			//(ipaddr_t ipaddr, uint32 pppoeId);
			pU32=(uint32 *)args[1];
			ret =  rtl8651_setPppoeDefaultSessionId((uint32)pU32[0]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_ADDPOLICYROUTE:
			//(uint32 pppoeId, uint16 vid);
			ret =  rtl8651_addPolicyRoute((rtl8651_tblDrvPolicyRoute_t *)args[1]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;
		case RTL8651_IOCTL_FLUSHPOLICYROUTE:
			//(uint32 pppoeId, uint16 vid);
			ret =  rtl8651_flushPolicyRoute(args[1]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;


		case RTL8651_IOCTL_DELPPPOESESSION:
			//rtl8651_delPppoeSession(uint32 pppoeId);
			pU32=(uint32 *)args[1];
			ret =  rtl8651_delPppoeSession((uint32)pU32[0]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_ADDIPINTF:
			//rtl8651_addIpIntf(int8 * ifName, ipaddr_t ipAddr, ipaddr_t ipMask);
			pU32=(uint32 *)args[1];
			ret =  rtl8651_addIpIntf((int8*)pU32[0],(ipaddr_t)pU32[1],(ipaddr_t)pU32[2]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_DELIPINTF:
			//rtl8651_delIpIntf(int8 * ifName, ipaddr_t ipAddr, ipaddr_t ipMask);			
			pU32=(uint32 *)args[1];
			ret =  rtl8651_delIpIntf((int8*)pU32[0],(ipaddr_t)pU32[1],(ipaddr_t)pU32[2]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_ADDNAPTMAPPING:
			//rtl8651_addNaptMapping(ipaddr_t extIpAddr);
			pU32=(uint32 *)args[1];
			ret =  rtl8651_addNaptMapping((ipaddr_t)pU32[0]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case PPPOEUP_CFG:
			spin_lock(&_rtl86xx_dev.lock);
			pppCfg=(pppoeCfg_t *)args[1];
			id = args[2];
			if(pppCfg->lanType==0){
				/* lanType == PPPOECFGPARAM_LANTYPE_NAPT */
				warn(rtl8651_setPppoeSessionProperty(id,pppCfg->sessionId,(ether_addr_t *)pppCfg->svrMac,args[3]));
				warn(rtl8651_addIpIntf("eth0",*(uint32*)pppCfg->ipAddr,(ipaddr_t)0xffffffff));
				warn(rtl8651_bindPppoeSession(*(uint32*)pppCfg->ipAddr,id));
				if (pppCfg->defaultSession)
				{
					warn(rtl8651_addRoute((ipaddr_t)0,(ipaddr_t)0,"eth0",0));
					warn(rtl8651_addNaptMapping(*(uint32*)pppCfg->ipAddr));
				}
			}else{
				/* lanType == PPPOECFGPARAM_LANTYPE_UNNUMBERED */
				warn(rtl8651_setPppoeSessionProperty(id,pppCfg->sessionId,(ether_addr_t *)pppCfg->svrMac,args[3]));
				warn(rtl8651_addIpIntf("eth0",*(uint32*)pppCfg->ipAddr,(ipaddr_t)0xffffffff));
				warn(rtl8651_bindPppoeSession(*(uint32*)pppCfg->ipAddr,id));
				if (pppCfg->defaultSession){
					/* default session, perform add with delRoute first
					 * (1) if not exists, add successfully
					 * (2) if exists, overwrite the old one
					 */					
					warn(rtl8651_delRoute((ipaddr_t)0,(ipaddr_t)0));
					warn(rtl8651_addRoute((ipaddr_t)0,(ipaddr_t)0,"eth0",0));
					warn(rtl8651_addNaptMapping(*(uint32*)pppCfg->ipAddr));
				}else{
					/* non-default session, perform add without delRoute first
					 * (1) if not exists, add successfully
					 * (2) if exists, add fail
					 */
					warn(rtl8651_addRoute((ipaddr_t)0,(ipaddr_t)0,"eth0",0));
				}
			}
			spin_unlock(&_rtl86xx_dev.lock);
			break;

		case PPPOEDOWN_CFG:
			spin_lock(&_rtl86xx_dev.lock);
			pppCfg=(pppoeCfg_t *)args[1];
			id = args[2];
			if(pppCfg->lanType==0){
				/* lanType == PPPOECFGPARAM_LANTYPE_NAPT */
				//rtl8651_flushNaptServerPortbyExtIp(*(uint32 *)pppCfg->ipAddr);
				rtl8651_delNaptMapping(*(uint32*)pppCfg->ipAddr);
				if (!pppCfg->defaultSession)
				{
					warn(rtl8651_delRoute(*(uint32*)pppCfg->ipAddr,(ipaddr_t)0xffffffff));
					rtl8651_flushPolicyRoute(*(uint32*)pppCfg->ipAddr);
				}
				else{
					//warn(rtl8651_delRoute((ipaddr_t)0,(ipaddr_t)0));
				}
				warn(rtl8651_resetPppoeSessionProperty(id));
				warn(rtl8651_delIpIntf("eth0",*(uint32*)pppCfg->ipAddr,0xffffffff));
			}else{
				/* lanType == PPPOECFGPARAM_LANTYPE_UNNUMBERED */
				//rtl8651_flushNaptServerPortbyExtIp(*(uint32 *)pppCfg->ipAddr);
				rtl8651_delNaptMapping(*(uint32*)pppCfg->ipAddr);
				if (!pppCfg->defaultSession)
				{
					warn(rtl8651_delRoute(*(uint32*)pppCfg->ipAddr,(ipaddr_t)0xffffffff));
					rtl8651_flushPolicyRoute(*(uint32*)pppCfg->ipAddr);
				}

				/* to solve s1/s2 up/down combination problem,
				 * let down-event always delete default route
				 * then pick another up session as default
				 * 2004/2/10
				 */
				//warn(rtl8651_delRoute((ipaddr_t)0,(ipaddr_t)0));
				
				warn(rtl8651_resetPppoeSessionProperty(id));
				warn(rtl8651_delIpIntf("eth0",*(uint32*)pppCfg->ipAddr,0xffffffff));			
			}
			spin_unlock(&_rtl86xx_dev.lock);
			break;
	
		case RTL8651_IOCTL_DELNAPTMAPPING:
			pU32=(uint32 *)args[1];
			ret =  rtl8651_delNaptMapping((ipaddr_t)pU32[0]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;
			
		case RTL8651_IOCTL_ADDEXTNETWORKINTERFACE:
			//rtl8651_addExtNetworkInterface(int8 * ifName);
			pU32=(uint32 *)args[1];
			ret =  rtl8651_addExtNetworkInterface((int8 *)pU32[0]);			
			pRet = (int32 *)args[2];
			*pRet = ret;
			{
				struct dev_priv *dp = dev->priv;
				int i;				
				for(i=1;i<MAX_PORT_NUM;i++)
				{
					if((dp->portnum>>i)==0)
					{
						wan_port=(i-1);
						break;
					}
				}
			}
			break;

		case RTL8651_IOCTL_DELEXTNETWORKINTERFACE:
			pU32=(uint32 *)args[1];
			ret =  rtl8651_delExtNetworkInterface((int8 *)pU32[0]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;
			
		case RTL8651_IOCTL_ADDROUTE:
			//rtl8651_addRoute(ipaddr_t ipAddr, ipaddr_t ipMask, int8 * ifName, ipaddr_t nextHop);
			pU32=(uint32 *)args[1];
			ret =  rtl8651_addRoute((ipaddr_t)pU32[0],(ipaddr_t)pU32[1],(int8*)pU32[2],(ipaddr_t)pU32[3]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_DELROUTE:
			//rtl8651_delRoute(ipaddr_t ipAddr, ipaddr_t ipMask);
			pU32=(uint32 *)args[1];
			ret =  rtl8651_delRoute((ipaddr_t)pU32[0],(ipaddr_t)pU32[1]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_ADDNATMAPPING:
			//rtl8651_addNatMapping(ipaddr_t extIpAddr, ipaddr_t intIpAddr);
			pU32=(uint32 *)args[1];
			ret =  rtl8651_addNatMapping((ipaddr_t)pU32[0],(ipaddr_t)pU32[1]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_DELNATMAPPING:
			//rtl8651_delNatMapping(ipaddr_t extIpAddr, ipaddr_t intIpAddr);
			pU32=(uint32 *)args[1];
			ret =  rtl8651_delNatMapping((ipaddr_t)pU32[0],(ipaddr_t)pU32[1]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_ADDNETWORKINTF:
			//rtl8651_addNetworkIntf(int8 *ifName);
			pU32=(uint32 *)args[1];
			ret =  rtl8651_addNetworkIntf((int8 *)pU32[0]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_DELNETWORKINTF:
			//rtl8651_delNetworkIntf(int8 *ifName);
			pU32=(uint32 *)args[1];
			ret =  rtl8651_delNetworkIntf((int8 *)pU32[0]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_SPECIFYNETWORKINTFLINKLAYERTYPE:
			//rtl8651_specifyNetworkIntfLinkLayerType(int8 * ifName, uint32 llType, uint16 vid);
			pU32=(uint32 *)args[1];
			ret =  rtl8651_specifyNetworkIntfLinkLayerType((int8 *)pU32[0],(uint32)pU32[1],(uint16)pU32[2]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_REMOVENETWORKINTFLINKLAYERTYPE:
			pU32=(uint32 *)args[1];
			ret =  rtl8651_removeNetworkIntfLinkLayerType((int8 *)pU32[0]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;
			
		case RTL8651_IOCTL_SETASICNAPTAUTOADDDELETE:
			//rtl8651_setAsicNaptAutoAddDelete(int8 autoAdd, int8 autoDelete);
			pU32=(uint32 *)args[1];
			ret =  rtl8651_setAsicNaptAutoAddDelete((int8)pU32[0],(int8)pU32[1]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_ENABLENAPTAUTOADD:
			//rtl8651_enableNaptAutoAdd(int8 autoAdd);
			pU32=(uint32 *)args[1];
			ret =  rtl8651_enableNaptAutoAdd((int8)pU32[0]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_ADDIPUNNUMBERED:
			//rtl8651_addIpUnnumbered(int8 *wanIfName, int8 *lanIfName, ipaddr_t netMask)
			pU32=(uint32 *)args[1];
			ret =  rtl8651_addIpUnnumbered((int8*)pU32[0],(int8*)pU32[1],(ipaddr_t)pU32[2]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_DELIPUNNUMBERED:
			//rtl8651_delIpUnnumbered(int8 *wanIfName, int8 *lanIfName)
			pU32=(uint32 *)args[1];
			ret =  rtl8651_delIpUnnumbered((int8*)pU32[0],(int8*)pU32[1]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_SETLANSIDEEXTERNALIPINTERFACE:
			//rtl8651_setLanSideExternalIpInterface(int8 * ifName, ipaddr_t ipAddr, ipaddr_t ipMask, int8 isExternal)
			pU32=(uint32 *)args[1];
			ret =  rtl8651_setLanSideExternalIpInterface((int8*)pU32[0],(ipaddr_t)pU32[1],(ipaddr_t)pU32[2],(int8)pU32[3]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_ADDURLFILTER:
			ptr=(uint32 *)args[1];
			u32_0=(uint32)ptr[0];
			pU8=(u8 *)ptr[1];
			pU32=(uint32 *)ptr[2];
			pU32_1=(uint32 *)ptr[3];
			pU32_2=(uint32 *)ptr[4];
			rc = rtl8651_addURLFilterRule((uint32)u32_0, (int8 *)pU8, (int32)*pU32, (uint32)*pU32_1, (uint32)*pU32_2);
			pU32=(uint32 *)args[3];
			*pU32=rc;
			break;

		case RTL8651_IOCTL_DELURLFILTER:
			ptr=(uint32 *)args[1];
			u32_0=(uint32)ptr[0];
			pU8=(uint8 *)ptr[1];
			pU32=(uint32 *)ptr[2];
			pU32_1=(uint32 *)ptr[3];
			pU32_2=(uint32 *)ptr[4];
			rc = rtl8651_delURLFilterRule((uint32)u32_0, (int8 *)pU8, (int32)*pU32, (uint32)*pU32_1, (uint32)*pU32_2);
			pU32=(uint32 *)args[3];
			*pU32=rc;
			break;
		
		case RTL8651_IOCTL_DELURLFILTERSTRING:
			//int32 rtl8651_delURLfilterString(int8 *string, int32 strlen)
			ptr=(uint32 *)args[1];
			u32_0=(uint32)ptr[0];
			pU8=(uint8 *)ptr[1];
			pU32=(uint32 *)ptr[2];
			rc = rtl8651a_delURLfilterString((uint32)u32_0,(int8 *)pU8,(int32)*pU32);
			pU32=(uint32 *)args[3];
			*pU32=rc;
			break;

		case RTL8651_IOCTL_ADDURLFILTERSTRING:
			//int32 rtl8651_delURLfilterString(int8 *string, int32 strlen)
			ptr=(uint32 *)args[1];
			u32_0=(uint32)ptr[0];
			pU8=(uint8 *)ptr[1];
			pU32=(uint32 *)ptr[2];
			rc = rtl8651a_addURLfilterString((uint32)u32_0,(int8 *)pU8,(int32)*pU32);
			pU32=(uint32 *)args[3];
			*pU32=rc;
			break;

		case RTL8651_IOCTL_SETPPPOESESSIONHANGUP:
			//rtl8651_setPppoeSessionHangUp(uint32 PPPoEID, int32 enable, uint32 sec, int32 (*p_callBack)(uint32));
			pU32=(uint32 *)args[1];
			ret =  rtl8651_setPppoeSessionHangUp((uint32)pU32[0],(int32)pU32[1],(uint32)pU32[2],(int32 (*)(uint32))pU32[3]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;
/*			
		case RTL8651_IOCTL_FLUSHPROTOSTACKUSEDUDPPORTS:
			rtl8651_flushProtoStackUsedUdpPorts();
			break;
			
		case RTL8651_IOCTL_ADDPROTOSTACKUSEDUDPPORT:
			ptr=(uint32 *)args[1];
			pU16_1=(uint16 *)ptr[0];
			rc = rtl8651_addProtoStackUsedUdpPort((uint16)*pU16_1);
			pU32=(int32 *)args[3];
			*pU32=rc;
			break;

		case RTL8651_IOCTL_FLUSHPROTOSTACKSERVERUSEDTCPPORTS:
			rtl8651_flushProtoStackServerUsedTcpPorts();
			break;
			
		case RTL8651_IOCTL_ADDPROTOSTACKSERVERUSEDTCPPORT:
			ptr=(uint32 *)args[1];
			pU16_1=(uint16 *)ptr[0];
			rc = rtl8651_addProtoStackServerUsedTcpPort((uint16)*pU16_1);
			pU32=(int32 *)args[3];
			*pU32=rc;
			break;
*/		

		case RTL8651_IOCTL_FLUSHPROTOSTACKACTIONS:
			rtl8651_flushProtoStackActions();
			break;

		case RTL8651_IOCTL_ADDPROTOSTACKACTIONS:
			ptr=(uint32 *)args[1];
			pU32_1=(uint32 *)ptr[0];
			pU32_2 =(uint32 *)ptr[1];			
			pU8_2 =	(uint8 *)ptr[2];
			pU8_3 = (uint8 *)ptr[3];
			pU8_4 = (uint8 *)ptr[4];
			pU16_1 =(uint16 *)ptr[5];
			pU16_2 =(uint16 *)ptr[6];
			
					
			rc = rtl8651_addProtoStackActions((ipaddr_t)*pU32_1,(ipaddr_t)*pU32_2,(uint8)*pU8_2,(uint8)*pU8_3,(uint8)*pU8_4,(uint16)*pU16_1,(uint16)*pU16_2);
			pU32=(int32 *)args[2];
			*pU32=rc;
			break;

		case RTL8651_IOCTL_ADDPROTOSTACKSERVERPORTRANGE:
			ptr=(uint32 *)args[1];
			pU32_1=(uint32 *)ptr[0];
			pU32_2 =(uint32 *)ptr[1];			
			pU8_2 =	(uint8 *)ptr[2];		
			pU16_1 =(uint16 *)ptr[3];
			pU16_2 =(uint16 *)ptr[4];			
			rc = rtl8651_addProtoStackServerPortRange((ipaddr_t)*pU32_1,(ipaddr_t)*pU32_2,(uint8)*pU8_2,(uint16)*pU16_1,(uint16)*pU16_2);
			pU32=(int32 *)args[2];
			*pU32=rc;
			break;			
			
		case RTL8651_IOCTL_FLUSHTRIGGERPORTRULES:
			rtl8651_flushTriggerPortRules();
			break;
		
		case RTL8651_IOCTL_ADDTRIGGERPORTRULE:
			ptr=(uint32 *)args[1];
			pU32_1=(uint32 *)ptr[0];
			pU8 =	(uint8 *)ptr[1];
			pU16_1 =(uint16 *)ptr[2];
			pU16_2 =(uint16 *)ptr[3];
			pU8_2 =	(uint8 *)ptr[4];
			pU16_3 =(uint16 *)ptr[5];
			pU16_4 =(uint16 *)ptr[6];
			pU32=(uint32 *)ptr[7];
			rc = rtl8651a_addTriggerPortRule((uint32)*pU32_1,(uint8)*pU8,(uint16)*pU16_1,(uint16)*pU16_2,(uint8)*pU8_2,(uint16)*pU16_3,(uint16)*pU16_4,(ipaddr_t)*pU32);
			pU32=(int32 *)args[3];
			*pU32=rc;
			break;

		case RTL8651_IOCTL_INSTALLLOGGINGFUNCTION:
			#ifdef _RTL_LOGGING
			//rtl8651_installLoggingFunction(void * pMyLoggingFunc);
			// this mapping is only valid for linux implementation
			pU32=(uint32 *)args[1];
			ret =  rtl8651_installLoggingFunction((void*)pU32[0]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			#endif /* _RTL_LOGGING */
			break;

		case RTL8651_IOCTL_ENABLELOGGING:
			#ifdef _RTL_LOGGING
			//int32 rtl8651_enableLogging(uint32 moduleId,int8 enable)
			pU32=(uint32 *)args[1];
			ret =  rtl8651a_enableLogging((uint32)pU32[0],(uint32)pU32[1],(int8)pU32[2]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			#endif /* _RTL_LOGGING */
			break;

		case RTL8651_IOCTL_SETWANSTATUS:
			rtl8651a_setWanStatus(args[1],args[2]);
			break;
		case RTL8651_IOCTL_SETUDPSIZETHRESH:
			rtl8651a_setUdpSizeThreshValue(args[1],args[2]);
			break;


		case RTL8651_IOCTL_SETPPPOEMTU:
			 rtl8651_setPppoeMtu((uint32)args[1],(uint16)args[2]);
			break;			

		case RTL8651_IOCTL_SETMTU:
			 rtl8651_setMtu((uint16)args[1]);
			break;

		case RTL8651_IOCTL_SETNETMTU:
 			 pU8 =	(uint8 *)args[1]; 			 
			 rtl8651_setNetMtu(pU8,(uint16)args[2]);
			break;

		case RTL8651_IOCTL_SETDOSSTATUS:
			rtl8651a_setDosStatus(args[1],args[2]);
			break;

		case RTL8651_IOCTL_SETDOSTHRESHOLD:
			rtl8651a_setDosThreshold(args[1],args[2],args[3]);
			break;

		case RTL8651_IOCTL_SETPERSRCDOSTHRESHOLD:
			rtl8651a_setPerSrcDosThreshold(args[1], args[2], args[3]);
			break;

		case RTL8651_IOCTL_SETDOSSCANTRACKINGPARA:
			ptr = (uint32 *)args[1];
			pU32	= (uint32 *)ptr[0];
			pU32_1	= (uint32 *)ptr[1];
			pU32_2	= (uint32 *)ptr[2];
			ret = rtl8651_dosScanTrackingSetPara((uint32)*pU32, (uint32)*pU32_1, (uint32)*pU32_2);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_ADDDOSIGNORETYPE:
			ptr		= (uint32 *)args[1];
			pU32	= (uint32 *)ptr[0];
			ret		= _rtl8651_dosIgnoreTypeAdd((uint32)*pU32);
			pRet		= (int32 *)args[2];
			*pRet	= ret;
			break;

		case RTL8651_IOCTL_DELDOSIGNORETYPE:
			ptr		= (uint32 *)args[1];
			pU32	= (uint32 *)ptr[0];
			ret		= _rtl8651_dosIgnoreTypeDel((uint32)*pU32);
			pRet		= (int32 *)args[2];
			*pRet	= ret;
			break;

		case RTL8651_IOCTL_SETALGSTATUS:
			ptr=(uint32 *)args[1];
			pU32 =	(uint32 *)ptr[0];
			pU32_1 =(uint32 *)ptr[1];
			pU32_2 =(uint32 *)ptr[2];
			ret=rtl8651a_setAlgStatus((uint32)*pU32,(uint32)(*pU32_1 | RTL8651_ALG_PPTP_SOFTWARE),(ipaddr_t *)pU32_2);
			//ret=rtl8651a_setAlgStatus((uint32)*pU32,(uint32)(*pU32_1 | 0x80000000),(ipaddr_t *)pU32_2);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

#ifdef _RTL_LOGGING
		case RTL8651_IOCTL_SETTIMEZONE:
			timezone_diff=(int32)args[1];
			break;
#endif

		case RTL8651_IOCTL_ADDNAPTCONNECTION:
			#if 0
			int32 rtl8651_addNaptConnection(
				int8 assigned,
				int8 flowType,
				ipaddr_t insideLocalIpAddr,
				uint16 insideLocalPort, 
				ipaddr_t *insideGlobalIpAddr,
				uint16 *insideGlobalPort,
				ipaddr_t dstIpAddr,
				uint16 dstPort);
			#endif
			pU32=(uint32 *)args[1];
			ret =  rtl8651_addNaptConnection(
				(int8)pU32[0],
				(int8)pU32[1],
				(ipaddr_t)pU32[2],
				(uint16)pU32[3],
				(ipaddr_t*)pU32[4],
				(uint16*)pU32[5],
				(ipaddr_t)pU32[6],
				(uint16)pU32[7]
				);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_FLUSHNAPTSERVERPORTBYEXTIP:
			ret=rtl8651_flushNaptServerPortbyExtIp((ipaddr_t)args[1]);
			pRet = (int32 *)args[2];
			*pRet=ret;
			break;		

		case RTL8651_IOCTL_ADDNAPTMAPPINGEXT:
			ret = rtl8651_addNaptMappingExt((ipaddr_t)args[1]);
			pRet = (int32 *)args[2];
			*pRet = ret;
			break;
		
		case RTL8651_IOCTL_SETDEFAULTIGMPUPSTREAM:
			pU32=(uint32 *)args[1];
			ret=rtl8651_multicastSetUpStream((int8*)pU32[0], (uint32)pU32[1]);
			pRet = (int32 *)args[2];
			*pRet=ret;
			break;

		case RTL8651_IOCTL_ADDMULTICASTFILTER:
			pU32=(uint32 *)args[1];
			ret=rtl8651_multicast_AddFilter((ipaddr_t)pU32[0], (uint32)pU32[1]);
			pRet = (int32 *)args[2];
			*pRet=ret;
			break;
		case RTL8651_IOCTL_DELMULITCASTFILTER:
			pU32=(uint32 *)args[1];
			ret=rtl8651_multicast_RemoveFilter((ipaddr_t)pU32[0], (uint32)pU32[1]);
			pRet = (int32 *)args[2];
			*pRet=ret;
			break;
#if 0
#ifdef RTL865X_DEBUG
		case RTL8651_IOCTL_SETDEFAULTIGMPUPSTREAM:
			assert(0);
			break;
#endif
#endif
		case RTL8651_IOCTL_SETLOOPBACKPORT:
			ret=rtl8651_setLoopbackPortPHY(args[1],args[2]);
			break;

		case RTL8651_IOCTL_SETSOUCEIPBLOCKTIMEOUT:
			ret=rtl8651_dosProc_blockSip_setPrisonTime(args[1]);
			break;

		case RTL8651_IOCTL_FREEBLOCKEDSOURCEIP:
			ret=rtl8651_dosProc_blockSip_freeAll();
			break;

		case RTL8651_IOCTL_ENABLESOURCEIPBLOCK:
			ret=rtl8651_dosProc_blockSip_enable(args[1], args[2]);
			break;

		case RTL8651_IOCTL_FLUSHABQOS:
			ret=rtl8651_flushPolicyBasedQoS();
			break;

		case RTL8651_IOCTL_ADDPOLICYBASEDQOS:
			pU32=(uint32 *)args[1];
			ret = rtl8651_addPolicyBasedQoS((int8 *)pU32[0], (rtl8651_tblDrvPolicy_t *)pU32[1], (int8)(pU32[2]&0xff) );
			pRet = (int32 *)args[3];
			*pRet=ret;
			break;

		case RTL8651_IOCTL_SETFLOWCTRL:
			pU32=(uint32 *)args[1];
			ret = rtl8651_setFlowControl((uint32)pU32[0], (int8)(pU32[1]&0xff));
			pRet = (int32 *)args[3];
			*pRet=ret;
			break;

		case RTL8651_IOCTL_SETPRIORITYQUEUE:
			pU32=(uint32 *)args[1];
			ret = rtl8651_setPortPriorityQueue((uint32)pU32[0], (int8)(pU32[1]&0xff));
			pRet = (int32 *)args[3];
			*pRet=ret;
			break;

		case RTL8651_IOCTL_SETBANDWIDTHCTRL:
			pU32=(uint32 *)args[1];
			ret = rtl8651_setEthernetPortBandwidthControl((uint32)pU32[0], (int8)(pU32[1]&0xff), (uint32)pU32[2]);
			pRet = (int32 *)args[3];
			*pRet=ret;
			break;

		case RTL8651_IOCTL_ADDSUBVLAN:
			pU32 = (uint32 *)args[1];
			ret = rtl8651_addSubVlan((uint16)pU32[0], (uint16)pU32[1], pU32[2]);
			pRet = (int32 *)args[3];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_DELSUBVLAN:
			ret = rtl8651_delSubVlan((uint16)args[1]);
			pRet = (int32 *)args[3];
			*pRet = ret;
			break;
		
		case RTL8651_IOCTL_ADDRATELIMITGROUP:
			pU32=(uint32 *)args[1];
			ret = rtl8651_addRateLimitGroup(pU32[0], (int8 *)pU32[1], pU32[2], pU32[3], (int8)pU32[4]);
			pRet = (int32 *)args[3];
			*pRet = ret;
			break;
			
		case RTL8651_IOCTL_ADDRATELIMIT:
			pU32=(uint32 *)args[1];
			ret = rtl8651_addRateLimitRule((rtl8651_tblDrvAclRule_t *)pU32[0], pU32[1]);
			pRet = (uint32 *)args[3];
			*pRet = ret;
			break;
			
		case RTL8651_IOCTL_FLUSHRATELIMITGROUP:
			pU32=(uint32 *)args[1];
			ret = rtl8651_flushRateLimitGroup((uint8 *)pU32[0]);
			pRet = (uint32 *)args[3];
			*pRet = ret;
			break;

		case RTL8651_IOCTL_ADDPBNAT:
			pU32=(uint32 *)args[1];
			ret = rtl8651_addProtocolBasedNAT( (uint8)pU32[0], pU32[1], pU32[2] );
			pRet = (int32 *)args[3];
			*pRet=ret;
			break;
			
		case RTL8651_IOCTL_DELPBNAT:
			pU32=(uint32 *)args[1];
			ret = rtl8651_delProtocolBasedNAT( (uint8)pU32[0], pU32[1], pU32[2] );
			pRet = (int32 *)args[3];
			*pRet=ret;
			break;

		case RTL8651_IOCTL_SINGLECOLOR5LEDMODE:
			// Set 0xBC805000
    			REG32(0xBC805000) &=~(1<<24);
    			REG32(0xBC805000) &=~(1<<20);
    			REG32(0xBC805000) |= (1<<19);

    			// Set 0xBC8020A0
    			REG32(SWTAA)=0xBC8020A0;
    			REG32(TCR0) =0x000002C4;
    			REG32(SWTACR)= CMD_FORCE | ACTION_START; // force add
			while ( (REG32(SWTACR) & ACTION_MASK) != ACTION_DONE ); /* Wait for command done */
                        break;
			
		case RTL8651_IOCTL_SOFTFWD:
			ret = rtl8651_pureSoftwareFwd(TRUE);
			break;




		case RTL8651_IOCTL_RESETCONTER:
			pU32=(uint32 *)args[1];
			pRet = (int32 *)args[3];
			*pRet = rtl8651_clearAsicSpecifiedCounter(pU32[0]);
			break;

		case RTL8651_IOCTL_CFGCONTER:
			pU32=(uint32 *)args[1];
			pRet = (int32 *)args[3];
			*pRet = rtl8651_clearAsicSpecifiedCounter(pU32[0]);
			if(*pRet == SUCCESS)
				*pRet = rtl8651_resetAsicCounterMemberPort(pU32[0]);
			if(*pRet == SUCCESS) {
				int32 i;
				for(i=0; i<RTL8651_PORT_NUMBER+3 && *pRet == SUCCESS; i++)
					if(pU32[1] & (1<<i))
						*pRet = rtl8651_addAsicCounterMemberPort(pU32[0], i);
			}
			break;
		case RTL8651_IOCTL_GETCOUNTER:
			{
				rtl865x_tblAsicDrv_basicCounterParam_t cnt;
				pU32=(uint32 *)args[1];
				rtl8651_getAsicCounter(pU32[0], &cnt);
				pU32[1]=cnt.rxBytes;
				pU32[2]=cnt.rxPackets;
				pU32[3]=cnt.txBytes;
				pU32[4]=cnt.txPackets;
				pU32[5]=cnt.rxErrors;
				pU32[6]=cnt.drops;
				pRet = (int32 *)args[3];
				*pRet=ret;
			}
			break;			
		case RTL8651_IOCTL_GETLINKSTATUS:
			{ int8 linkUp=0,  duplex=0, neg;
				
				uint32 speed=0,cap;
				pU32=(uint32 *)args[1];
				rtl8651_getAsicEthernetLinkStatus(pU32[0], &linkUp);
				if(linkUp){
					ret=rtl8651_getAsicEthernetPHY(pU32[0], &neg, &cap, &speed, &duplex);
				}else{
					ret = rtl8651_asicEthernetCableMeter( (uint8)pU32[0], &pU32[2], &pU32[2] );
				}
				pU32[1]=(linkUp<<2)|(speed<<1)|(duplex);
				pRet = (int32 *)args[3];
				*pRet=ret;
				break;
			}
		case RTL8651_IOCTL_GETLDVER:
			{
				/*
				 * Old-fashion loader places a function pointer in communication section.
				 * New-fashion loader places a string in communication section.
				 */
#ifdef CONFIG_RTL865X_LOADER_00_00_11
				/* To get version of loader, loader must provide the string at 0x80000300.
				   Therefore, we just copy from there to here.
				   We note that we no more use function pointer to get loader version.
				 */
				pRet = (int32 *)args[3];
				memcpy( pRet, (uint32*)LDR_VERSION_ADDR, 9 ); /* 00.00.00 */
				break;
#else /* CONFIG_RTL865X_LOADER_00_00_11 */
				uint32	ver;
				uint32	(*getLdVer)(void);
				getLdVer = (uint32(*)(void))(*(uint32*)LDR_COMMUN_BASE);
				ver=getLdVer();
				pRet = (int32 *)args[3];
				*pRet=ver;
				break;
#endif/* CONFIG_RTL865X_LOADER_00_00_11 */
			}
		case RTL8651_IOCTL_REBOOT:
			{
				#if 1
				/* jump to runtime start in flash memory */
				void (*start)(void);
				REG32(GIMR)=0;
				start = (void(*)(void))FLASH_BASE;	
				start();
				#else
				/* use watch dog */
				REG32(WDTCNR)=0x0; /* enable watchdog timer */
				#endif
				break;
			}

		case RTL8651_IOCTL_SETMNQUEUEUPSTREAMBW:
			{	
				pU32=(uint32 *)args[1];
				ret = rtl8651_setUpstreamBandwidth((int8 *)pU32[0], (uint32)pU32[1]);
				pRet = (int32 *)args[3];
				*pRet = ret;
				break;
			}
			
		case RTL8651_IOCTL_ADDUNLIMITEDQUEUE:
			{
				pU32=(uint32 *)args[1];
				ret = rtl8651_addFlowToUnlimitedQueue((int8 *)pU32[0], (rtl8651_tblDrvAclRule_t *)pU32[1], (uint32)pU32[2], (uint32)pU32[3]);
				pRet = (int32 *)args[3];
				*pRet = ret;
				break;
			}

		case RTL8651_IOCTL_ADDLIMITEDQUEUE:
			{
				pU32=(uint32 *)args[1];
				rtl8651_addFlowToLimitedQueue((int8 *)pU32[0], (rtl8651_tblDrvAclRule_t *)pU32[1], (uint32)pU32[2], (uint32)pU32[3]);
				pRet = (int32 *)args[3];
				*pRet = ret;
				break;
			}

		case RTL8651_IOCTL_SETDSCPTOMNQUEUE:
			{
				pU32=(uint32 *)args[1];
				rtl8651_setDscpToMNQueue((uint32)pU32[0], (uint32)pU32[1]);
				pRet = (int32 *)args[3];
				*pRet = ret;
				break;
			}

		case RTL8651_IOCTL_WEAKNAPTCTL:
			{
				ret = rtl8651_fwdEngineWeakTcpNaptProcess((int8)args[1]);
				pRet = (int32 *)args[3];
				*pRet = ret;
				break;
			}

		case RTL8651_IOCTL_TCPNAPTSTATICPORTMAPPINGCTL:
			{
				ret = rtl8651_fwdEngineTcpStaticNaptPortTranslation((int8)args[1]);
				pRet = (int32 *)args[3];
				*pRet = ret;
				break;
			}

		case RTL8651_IOCTL_UDPNAPTSTATICPORTMAPPINGCTL:
			{
				ret = rtl8651_fwdEngineUdpStaticNaptPortTranslation((int8)args[1]);
				pRet = (int32 *)args[3];
				*pRet = ret;
				break;
			}

		case RTL8651_IOCTL_LOOSEUDPCTL:
			{
				ret = rtl8651_fwdEngineInexactUdpFlow((int8)args[1]);
				pRet = (int32 *)args[3];
				*pRet = ret;
				break;
			}
			
                // Dino Chang add for MAC filter 2004/11/14
                case RTL8651_ENABLEMACFILTER:
                        {
                                ret = rtl8651_enableMacAccessControl((int8)args[1]);
                                pRet = (int32 *)args[3];
                                *pRet = ret;
                                break;
                        }
                case RTL8651_SETMACFILTERDEFAULTPERMIT:
                        {
                                ret = rtl8651_setMacAccessControlDefaultPermit((int8)args[1]);
                                pRet = (int32 *)args[3];
                                *pRet = ret;
                                break;
                        }
                case RTL8651_FLUSHMACFILTER:
                        {
                                ret = rtl8651_flushMacAccessControl();
                                pRet = (int32 *)args[3];
                                *pRet = ret;
                                break;
                        }
                case RTL8651_ADDMACFILTER:
                        {
                                pU32=(uint32 *)args[1];
                                ret = rtl8651_addMacAccessControl((ether_addr_t *)pU32[0], (ether_addr_t *)pU32[1], (int8)pU32[2]);
                                pRet = (int32 *)args[3];
                                *pRet = ret;
                                break;
                        }
                        break;
                case RTL8651_DELMACFILTER:
                        {
                                pU32=(uint32 *)args[1];
                                ret = rtl8651_delMacAccessControl((ether_addr_t *)pU32[0], (ether_addr_t *)pU32[1]);
                                pRet = (int32 *)args[3];
                                *pRet = ret;
                                break;
                        }
                // Dino Chang			
			
		case RTL8651_IOCTL_NAPTREDIRECT_REGISTER_RULE:
			{
				pU32=(uint32 *)args[1];
				ret = rtl8651_registerRedirectOutboundNaptFlow((uint8)pU32[0], (ipaddr_t)pU32[1], (uint16)pU32[2], (ipaddr_t)pU32[3], (uint16)pU32[4], (ipaddr_t)pU32[5], (uint16)pU32[6]);
				pRet = (int32 *)args[3];
				*pRet = ret;
				break;
			}

		case RTL8651_IOCTL_NAPTREDIRECT_UNREGISTER_RULE:
			{
				pU32=(uint32 *)args[1];
				ret = rtl8651_unregisterRedirectOutboundNaptFlow((uint8)pU32[0], (ipaddr_t)pU32[1], (uint16)pU32[2], (ipaddr_t)pU32[3], (uint16)pU32[4], (ipaddr_t)pU32[5], (uint16)pU32[6]);
				pRet = (int32 *)args[3];
				*pRet = ret;
				break;
			}

		case RTL8651_IOCTL_NAPTREDIRECT_QUERY_FLOW:
			{
				ipaddr_t originalDip;
				uint16 originalDport;

				pU32=(uint32 *)args[1];

				ret = rtl8651_queryRedirectOutboundNaptFlow((uint8)pU32[0], (ipaddr_t)pU32[1], (uint16)pU32[2], (ipaddr_t*)(&originalDip), (uint16*)(&originalDport), (ipaddr_t)pU32[5], (uint16)pU32[6]);

				if (ret == SUCCESS)
				{
					if (copy_to_user((void *)(pU32[3]), &originalDip, sizeof(ipaddr_t)))
						return -EFAULT;

					if (copy_to_user((void *)(pU32[4]), &originalDport, sizeof(uint16)))
						return -EFAULT;
				}
				pRet = (int32 *)args[3];
				*pRet = ret;
				break;
			}

		case RTL8651_IOCTL_ADDEXTMCASTPORT:
			{
				ret = rtl8651_addExternalMulticastPort((uint32)args[1]);
				pRet = (int32 *)args[3];
				*pRet = ret;
				break;
			}

		case RTL8651_IOCTL_DELEXTMCASTPORT:
			{
				ret = rtl8651_delExternalMulticastPort((uint32)args[1]);
				pRet = (int32 *)args[3];
				*pRet = ret;
				break;
			}

		case RTL8651_IOCTL_SET_DHCPC_PID:
			{
				udhcpc_pid=(uint32)args[1];
				break;
			}
			
		case RTL8651_IOCTL_ENABLE_PPPOE_PASSTHRU:
			{
				ret = rtl8651_EnablePppoePassthru((int8)args[1]);
				pRet = (uint32 *)args[3];
				*pRet = ret;
				rtlglue_printf("PPPoE Passthru %s.\n",args[1]?"enabled":"disabled");
				break;
			}

		case RTL8651_IOCTL_ENABLE_DROP_UNKNOWN_PPPOE_DROP:
			{
				ret = rtl8651_enableDropUnknownPppoePADT((int8)args[1]);
				pRet = (uint32 *)args[3];
				*pRet = ret;
				rtlglue_printf("Drop Unknown PPPoE PADT %s.\n",args[1]?"enabled":"disabled");
				break;
			}
			
		case RTL8651_IOCTL_ENABLE_IPV6_PASSTHRU:
			{
				ret = rtl8651_EnableIpv6Passthru((int8)args[1]);
				pRet = (uint32 *)args[3];
				*pRet = ret;
				rtlglue_printf("IPv6 Passthru %s.\n",args[1]?"enabled":"disabled");
				break;
			}

		case RTL8651_IOCTL_ENABLE_IPX_PASSTHRU:
			{
				ret = rtl8651_EnableIpxPassthru((int8)args[1]);
				pRet = (uint32 *)args[3];
				*pRet = ret;
				rtlglue_printf("IPX Passthru %s.\n",args[1]?"enabled":"disabled");
				break;
			}

		case RTL8651_IOCTL_ENABLE_NETBIOS_PASSTHRU:
			{
				ret = rtl8651_EnableNetbiosPassthru((int8)args[1]);
				pRet = (uint32 *)args[3];
				*pRet = ret;
				rtlglue_printf("NETBIOS Passthru %s.\n",args[1]?"enabled":"disabled");
				break;
			}

		case RTL8651_IOCTL_ENABLE_IPMULTICAST:
			{
				ret = rtl8651_fwdEngineProcessIPMulticast((int8)args[1]);
				pRet = (uint32 *)args[3];
				*pRet = ret;
				break;
			}

		case RTL8651_IOCTL_RTL8651_SETALGQOSQUEUEID:
			{
				ptr=(uint32 *)args[1];
				pU32_1 = (uint32 *)ptr[0];
				pU32_2 = (uint32 *)ptr[1];
				pU32 = (uint32 *)ptr[2];
				ret = rtl8651_setAlgQosQueueId(*pU32_1,*pU32_2,*pU32);
				pU32=(uint32 *)args[3];
				*pU32=ret;
				break;
			}
		
		case RTL8651_IOCTL_DELRATELIMITGROUP:
			{

				ret=rtl8651_delRateLimitGroup((uint32)args[1]);
				pRet = (uint32 *)args[3];
				*pRet = ret;
				break;
			}

		case RTL8651_IOCTL_GETMNQUEUEENTRY:
			{
				ptr=(uint32 *)args[1];
				pU32_1 = (uint32 *)ptr[0];
				pU32_2 = (uint32 *)ptr[1];
				pU32 = (uint32 *)ptr[2];
				ret = rtl8651_getMNQueueEntry((rtl8651_tblDrvAclRule_t *)pU32_1,*pU32_2,*pU32);
				pU32=(uint32 *)args[3];
				*pU32=ret;
				break;
			}

		case RTL8651_IOCTL_DELFLOWFROMMNQUEUE:
			{
				ptr=(uint32 *)args[1];
				pU32_1 = (uint32 *)ptr[0];
				pU32_2 = (uint32 *)ptr[1];
				ret = rtl8651_delFlowFromMNQueue((rtl8651_tblDrvAclRule_t *)pU32_1,*pU32_2);
				pU32=(uint32 *)args[3];
				*pU32=ret;
				break;
			}
		case RTL8651_IOCTL_DELRATELIMITRULE:
			{
				ptr=(uint32 *)args[1];
				pU32_1 = (uint32 *)ptr[0];
				pU32_2 = (uint32 *)ptr[1];
				ret = rtl8651_delRateLimitRule((rtl8651_tblDrvAclRule_t *)pU32_1,*pU32_2);
				pU32=(uint32 *)args[3];
				*pU32=ret;
				break;
			}
		case RTL8651_IOCTL_ADDARP:
			{
				ptr=(uint32 *)args[1];
				pU32_1 = (uint32 *)ptr[0];
				pU32_2 = (uint32 *)ptr[1];
				pU8 = (uint8 *)ptr[2];
				pU32_3 = (uint32 *)ptr[3];
				printk("===================================addArp ip=%x intf=%s port=%d\n",*pU32_1,pU8,*pU32_3);
				ret=rtl8651_addArp((ipaddr_t)*pU32_1, (ether_addr_t *)pU32_2, (int8 *)pU8, (uint32)*pU32_3);
				pU32=(uint32 *)args[3];
				*pU32=ret;
				break;
			}
		
		
	}
	return 0;

normal:
	if (!netif_running(dev))
		return -EINVAL;
	switch (cmd)
        {
	    case SIOCETHTOOL:
		return re865x_ethtool_ioctl(dp, (void *) rq->ifr_data);
	    default:
		rc = -EOPNOTSUPP;
		break;
	}

	return rc;
}




#ifndef _RTL_NEW_LOGGING_MODEL
static void circ_msg(struct circ_buf *buf,const char *msg)
{
	spinlock_t lock = SPIN_LOCK_UNLOCKED;	
	spin_lock(&lock);

	int l = strlen(msg) + 1;	 
	if ((buf->tail + l) < buf->size) {
		if (buf->tail < buf->head) {
			if ((buf->tail + l) >= buf->head) {
				int k = buf->tail + l - buf->head;
				char *c =
					memchr(buf->data + buf->head + k, '\0',
						   buf->size - (buf->head + k));
				if (c != NULL) {	
					buf->head = c - buf->data + 1;	
				} else {					
					buf->head = 0;
				}
			}
		}		
		strncpy(buf->data + buf->tail, msg, l);
		buf->tail += l;	
	} else {		
		char *c;
		int k = buf->tail + l - buf->size;
		c = memchr(buf->data + k, '\0', buf->size - k);
		if (c != NULL) {
			buf->head = c - buf->data + 1;
			strncpy(buf->data + buf->tail, msg, l - k - 1);
			buf->data[buf->size - 1] = '\0';
			strcpy(buf->data, &msg[l - k - 1]);
			buf->tail = k + 1;
		} else {
			buf->head = buf->tail = 0;
		}
	}
	spin_unlock(&lock);
}




#define FEBRUARY                2
#define STARTOFTIME             1970
#define SECDAY                  86400L
#define SECYR                   (SECDAY * 365)
#define leapyear(y)             ((!(y % 4) && (y % 100)) || !(y % 400))
#define days_in_year(a)         (leapyear(a) ? 366 : 365)
#define days_in_month(a)        (month_days[(a) - 1])
static int month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static void to_tm(unsigned long tim, struct rtc_time * tm)
{
	long hms, day, gday;
	int i;

	gday = day = tim / SECDAY;
	hms = tim % SECDAY;

	/* Hours, minutes, seconds are easy */
	tm->tm_hour = hms / 3600;
	tm->tm_min = (hms % 3600) / 60;
	tm->tm_sec = (hms % 3600) % 60;

	/* Number of years in days */
	for (i = STARTOFTIME; day >= days_in_year(i); i++)
	day -= days_in_year(i);
	tm->tm_year = i;

	/* Number of months in days left */
	if (leapyear(tm->tm_year))
	days_in_month(FEBRUARY) = 29;
	for (i = 1; day >= days_in_month(i); i++)
	day -= days_in_month(i);
	days_in_month(FEBRUARY) = 28;
	tm->tm_mon = i-1;	/* tm_mon starts from 0 to 11 */

	/* Days are what is left over (+1) from all that. */
	tm->tm_mday = day + 1;

	/*
	 * Determine the day of week
	 */
	tm->tm_wday = (gday + 4) % 7; /* 1970/1/1 was Thursday */
}

#endif

#if defined(CONFIG_RTL865X_INIT_BUTTON)||defined(CONFIG_RTL865X_DIAG_LED)
#if 0
static void gpioLed(uint32 ledFreq){
	switch(ledFreq){
		case 1:
			while(1){
				DIAG_LED_OFF();DELAY_500_MSEC;/*1*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_3000_MSEC;
			}
			break;
		case 2:
			while(1){
				DIAG_LED_OFF();DELAY_500_MSEC;/*1*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*2*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_3000_MSEC;
			}
			break;
		case 3:
			while(1){
				DIAG_LED_OFF();DELAY_500_MSEC;/*1*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*2*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*3*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_3000_MSEC;
			}
			break;
		case 5:
			while(1){
				DIAG_LED_OFF();DELAY_500_MSEC;/*1*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*2*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*3*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*4*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*5*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_3000_MSEC;
			}
			break;
		case 9:
			while(1){
				DIAG_LED_OFF();DELAY_500_MSEC;/*1*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*2*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*3*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*4*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*5*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*6*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*7*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*8*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*9*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_3000_MSEC;
			}
			break;
		default:
			break;
	}
}/* end gpioLed */
#else
static void gpioLed(uint32 ledFreq){
	switch(ledFreq){
		case 1:
		case 2:
		case 3:
		case 5:
		case 7:
		case 9:
			ledCtrlFreq   = ledFreq;
			ledCtrlTick=ledCtrlTick2=0;
			break;
		default:
			ledCtrlFreq =ledCtrlTick=ledCtrlTick2=0;
			break;
	}
}/* end gpioLed */
#endif
#endif /* CONFIG_RTL865X_INIT_BUTTON */
