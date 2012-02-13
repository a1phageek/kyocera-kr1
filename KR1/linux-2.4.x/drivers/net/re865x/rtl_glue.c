#include "rtl865x/rtl_types.h"

#ifndef RTL865X_TEST
#ifdef __linux__
#include <linux/netdevice.h>
#include <linux/interrupt.h>
#include <linux/skbuff.h>
#endif
#include "swNic2.h"
#include "rtl865x/rtl_glue.h"
#endif
#include "rtl865x/mbuf.h"
#include "rtl865x/assert.h"
#include "rtl865x/rtl8651_tblDrv.h"
#include "rtl865x/rtl8651_tblAsicDrv.h"
#include "rtl865x/rtl8651_tblDrvFwd.h"


#undef RTL_GLUE_DEBUG
//#define RTL_GLUE_DEBUG

/*
 * Since tblDrv cannot know how freq tick is,
 *   this function returns system tick freq.
 * In linux, a typical case of system tick is 100HZ.
 */
uint32 rtl865x_getHZ(void)
{
#ifdef RTL865X_TEST
	return 100;
#else/*RTL865X_TEST*/
	return HZ;
#endif/*RTL865X_TEST*/
}

//cfliu: This function is only for debugging. Should not be used in production code...
static void memDump (void *start, uint32 size, int8 * strHeader){
  int32 row, column, index, index2, max;
  uint8 *buf, ascii[17];
  int8 empty = ' ';

  if(!start ||(size==0))
  	return;
  buf = (uint8 *) start;

  /*
     16 bytes per line
   */
  if (strHeader)
    rtlglue_printf ("%s", strHeader);
  column = size % 16;
  row = (size / 16) + 1;
  for (index = 0; index < row; index++, buf += 16) {
      memset (ascii, 0, 17);
      rtlglue_printf ("\n%08x ", (memaddr) buf);

      max = (index == row - 1) ? column : 16;

      //Hex
      for (index2 = 0; index2 < max; index2++){
	  if (index2 == 8)
	    rtlglue_printf ("  ");
	  rtlglue_printf ("%02x ", (uint8) buf[index2]);
	  ascii[index2] = ((uint8) buf[index2] < 32) ? empty : buf[index2];
	}

      if (max != 16){
	  if (max < 8)
	    rtlglue_printf ("  ");
	  for (index2 = 16 - max; index2 > 0; index2--)
	    rtlglue_printf ("   ");
	}

      //ASCII
      rtlglue_printf ("  %s", ascii);
    }
  rtlglue_printf ("\n");
  return;
}

#ifdef RTL865X_TEST
//------------------------  MODULE TEST CODE GLUE STARTS HERE---------------//
int test_drvMutex = 0;
int test_mbufMutex = 0;
//This is not real Linux sk_buff. Simplified for module test.
struct sk_buff {
        /* These two members must be first. */
        struct sk_buff  * next;                 /* Next buffer in list                          */
        struct sk_buff  * prev;                 /* Previous buffer in list                      */
        struct sk_buff_head * list;             /* List we are on                               */
        struct sock     *sk;                    /* Socket we are owned by                       */
        unsigned int    len;                    /* Length of actual data                        */
        unsigned int    data_len;
        unsigned int    csum;                   /* Checksum                                     */
        unsigned char   cloned;                 /* head may be cloned (check refcnt to be sure). */
        unsigned int    truesize;               /* Buffer size                                  */

        unsigned char   *head;                  /* Head of buffer                               */
        unsigned char   *data;                  /* Data head pointer                            */
        unsigned char   *tail;                  /* Tail pointer                                 */
        unsigned char   *end;                   /* End pointer                                  */
};

struct sk_buff_head {
        struct sk_buff  * next;
        struct sk_buff  * prev;

        uint32          qlen;
};

struct skb_shared_info {
        unsigned int    nr_frags;
        struct sk_buff  *frag_list;
};

typedef struct { } spinlock_t;
int32 spin_lock_irqsave(spinlock_t *spinlock, int32 s){return 0;}
int32 spin_unlock_irqrestore(spinlock_t *spinlock, int32 s){return 0;}
int32 swNic_isrReclaim(uint32 rxDescIdx, struct rtl_pktHdr*pPkthdr,struct rtl_mBuf *pMbuf){}
int32 rtlglue_drvMutexLock(void){test_drvMutex ++; return 0;}
int32 rtlglue_drvMutexUnlock(void){test_drvMutex --; return 0;}
int32 rtlglue_getDrvMutex(void){return test_drvMutex;}
int32 rtlglue_reinitDrvMutex(void){test_drvMutex=0; return 0;}
int32 rtlglue_mbufMutexLock(void){test_mbufMutex ++; return 0;}
int32 rtlglue_mbufMutexUnlock(void){test_mbufMutex --;  return 0;}
int32 rtlglue_getMbufMutex(void){return test_mbufMutex;}
int32 rtlglue_reinitMbufMutex(void){test_mbufMutex=0; return 0;}
void local_irq_restore(void){}
void local_irq_save(void){}

void rtlglue_getMacAddress(ether_addr_t * macAddress, uint32 * number){
	uint8 mac[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x08 };
        memcpy((int8*)macAddress, (int8 *)mac, 6);
        *number = 8;
}

void *rtlglue_malloc(uint32 NBYTES){return malloc(NBYTES);}
void rtlglue_free(void *APTR){free(APTR);}
int32 rtlglue_mbufClusterRefcnt(void *buffer, uint32 id, uint32 *count, int8 operation){
        if (operation==0){
                if (count)
                        *count=1;
        }
        return SUCCESS;
}
int32 rtlglue_mbufAllocCluster(void **buffer, uint32 size, uint32 *id){
        struct sk_buff *skb;
	void* data;
	skb = (struct sk_buff *)malloc((sizeof(struct sk_buff_head) + size + sizeof(struct skb_shared_info)));
	data = skb + sizeof(struct sk_buff_head);
	skb->truesize = size + sizeof(struct sk_buff);
	skb->head = data;
	skb->data = data;
	skb->tail = data;
	skb->end = data+size;
	skb->len = 0;
        skb->cloned = 0;
        skb->data_len = 0;

	*id = (int)skb;
	*buffer = (void*)((uint32)skb->data|0xa0000000);
	return SUCCESS;
}
int32 rtlglue_mbufFreeCluster(void *buffer, uint32 size, uint32 id){
	free((void *)id);
        return 0;
}

void rtlglue_extDeviceSend(uint16 vid, int32 linkId, struct rtl_pktHdr *orig_pktHdr){
	freeMbuf(((struct rtl_pktHdr*)orig_pktHdr)->ph_mbuf);

}

 void rtlglue_drvSend(void * pkthdr){
	struct rtl_mBuf* new_m;
	// duplicate packet
	
	new_m = mBuf_dupPacket(((struct rtl_pktHdr*)pkthdr)->ph_mbuf, MBUF_DONTWAIT);	
	if (!new_m){
		
		printf("@mbuf duplicate error!\n");
		return;
	}

	// free original packet
	freeMbuf(((struct rtl_pktHdr*)pkthdr)->ph_mbuf);

	// enqueue new packet
	if (mbufList_enqueue(new_m)!=0){

		printf("@mbuf enqueue error!\n");
	}	
	return;
}
void * rtlglue_mbufClusterToData(void *buffer){
	return buffer;
}

void rtlglue_getRingSize(uint32 *rx, uint32 *tx)
{
	if (rx)
		*rx = 8;
	if (tx)
		*tx = 8;
}

int32 rtlglue_reclaimRxBD(uint32 rxDescIdx,struct rtl_pktHdr *pThisPkthdr, struct rtl_mBuf *pThisMbuf){pThisPkthdr->ph_rxdesc = -1; mBuf_driverFreeMbufChain(pThisMbuf);  return 0;};
int32 rtlglue_mbufTsleep(uint32 wait_channel){return 0;}
int32 rtlglue_mbufTwakeup(uint32 wait_channel){return 0;}



//------------------------MODULE  TEST CODE GLUE ENDS HERE---------------//


#else /*RTL865X_TEST*/
//------------------------  LINUX DEPEND GLUE STARTS HERE-------------//

#define RTLGLUE_MAX_DEVLINKS	10
//Don't use rtl8650DevLink[0]. It is reserved for FWD engine to send a bcast pkt to PCI/WLAN
//if rtlglue_extDeviceSend() is called with linkId=0, then this pkt should be bcast to all WDS links. 
static int32 rtl8650DevLinkCount;
struct DevLinkRecord {
	int8 devName[16];
	uint16 vid;
	uint32 portmask;
	struct net_device *pci_device;
	//void (*descFreeFunc)(void *);
} rtl8650DevLink[RTLGLUE_MAX_DEVLINKS]; 



int32 rtlglue_init(void){
	memset(&rtl8650DevLink[0],0,sizeof(struct DevLinkRecord));
	rtl8650DevLinkCount=0;

	return 0;
}


static spinlock_t mbuf_lock = SPIN_LOCK_UNLOCKED;
static int __mbuf_flags;
int dd_cc;

int32 rtlglue_mbufMutexLock(void){
	if (dd_cc>=1){
		dd_cc++;
		return 0;
	}	
	spin_lock_irqsave(&mbuf_lock,__mbuf_flags);
	//enableLbcTimeout();
	dd_cc++;
        return 0;
}

int32 rtlglue_mbufMutexUnlock(void){
	dd_cc--;
	if (!dd_cc)
		spin_unlock_irqrestore(&mbuf_lock,__mbuf_flags);
        return 0;
}

int32 rtlglue_mbufTsleep(uint32 wait_channel){
        return 0;
}

int32 rtlglue_mbufTwakeup(uint32 wait_channel){
        return 0;
}

int32 rtlglue_mbufClusterRefcnt(void *buffer, uint32 id, uint32 *count, int8 operation){
	if (operation==0)
	{
		if (count)
			*count=1;

	}
        return SUCCESS;
}

void * rtlglue_mbufClusterToData(void *buffer){
	
	return (buffer);
}

int32 rtlglue_mbufAllocCluster(void **buffer, uint32 size, uint32 *id){
	struct sk_buff *skb;
	skb =	dev_alloc_skb (size);
	if (skb)
	{
		*id = (int)skb;
#ifdef CONFIG_RTL865X_CACHED_NETWORK_IO
		*buffer = (void*)((u32)skb->data);
#else
		//skb->data was allocated as cached but we make it uncached to mbuf chain
		*buffer = (void*)((u32)skb->data|0xa0000000);
#endif
       		 return SUCCESS;
	}
	else 
	{
		printk("skb exhausted. alloc failed\n");
		return FAILED;
	}
}
#define DBG 1
int32 rtlglue_mbufFreeCluster(void *buffer, uint32 size, uint32 id){
	struct sk_buff *skb=(struct sk_buff*)id;

#if DBG
	if (!skb)
	{
		printk(" %s %d skb null data %x\n",__FILE__,__LINE__,(uint32)buffer);
		return 0;
	}			
#endif
	dev_kfree_skb_any(skb);
        return 0;
}
static spinlock_t int_lock = SPIN_LOCK_UNLOCKED;
int int_cc=0;
static int __flags;
__IRAM 
int32 rtlglue_drvMutexLock(void){
	
	if (int_cc>=1) 
	{
		int_cc++;
		return 0;
	}
	spin_lock_irqsave(&int_lock,__flags);
	//enableLbcTimeout();	
	int_cc++;
        return 0;
}

__IRAM
int32 rtlglue_drvMutexUnlock(void){
	int_cc--;
	if (!int_cc)
	spin_unlock_irqrestore(&int_lock,__flags);
        return 0;
}


int chkValidAddr(uint32 addr){
	if(addr&0x80000000){
		if(addr>>28==0x8 || addr>>28==0xa||addr>>28==0xb)
			return 0;
	}
	printk("%08x invalid!!\n", addr);
	return 1;
}
int checkSkbAddr(struct sk_buff *skb){
	if(chkValidAddr((uint32)skb->head))
		return 1;
	if(chkValidAddr((uint32)skb->data))
		return 2;
	if(chkValidAddr((uint32)skb->tail))
		return 3;
	if(chkValidAddr((uint32)skb->end))
		return 4;
	return 0;
}


#ifdef CONFIG_RE865X  //switch core and NIC is enabled

EXPORT_SYMBOL(rtl8651_setSpanningTreeInstanceProtocolWorking);
EXPORT_SYMBOL(rtl8651_setSpanningTreeInstancePortState);



__IRAM void rtlglue_drvSend(void *data)
{
	if (FAILED == swNic_write(data))
	{
		struct rtl_pktHdr *pktHdr=(struct rtl_pktHdr *)data;
		mBuf_freeMbufChain(pktHdr->ph_mbuf);
	}
	#if 0
	if(FAILED==swNic_write((void *)pktHdr) &&((pktHdr->ph_rxdesc<0)||(pktHdr->ph_rxdesc>=totalRxPkthdr))){
		//buffer was allocated by upper layer. Free both mbuf and skbuf here...
		mBuf_freeMbufChain(pktHdr->ph_mbuf);
	}
	#endif
}


int32 rtlglue_reclaimRxBD(uint32 rxDescIdx,struct rtl_pktHdr *pThisPkthdr, struct rtl_mBuf *pThisMbuf){
	int32 s,retval;
	spin_lock_irqsave(rtl865xSpinlock,s);
	assert(!pThisMbuf->m_next);
	//reclaim the pkthdr--mbuf--cluster...
	retval=swNic_isrReclaim(rxDescIdx, pThisPkthdr,  pThisMbuf);
	spin_unlock_irqrestore(rtl865xSpinlock,s);
	return retval;
}



#ifdef CONFIG_RTL865X_MULTILAYER_BSP

EXPORT_SYMBOL(rtl8651_fwdEngineExtPortRecv);
EXPORT_SYMBOL(rtl8651_fwdEngineEnableWDS);
EXPORT_SYMBOL(rtl8651_fwdEngineRemoveHostsOnExtLinkID);
EXPORT_SYMBOL(rtl8651_fwdEngineDelWlanSTA);
EXPORT_SYMBOL(rtl8651_fwdEngineAddWlanSTA);

//This function register a WLAN driver instance or a WDS link to a locally maintained database. 
//This function is not pat of 8651 driver glue interface but is implemented here to demonstrate how 
//to create linkId and bind it with OS-dependent driver interface.
//Valid linkId number can be any value except 0. linkId=0 would be used by 8651 driver when 
//rtlglue_extDeviceSend() is called to tell WLAN driver that a broadcast must be sent to all WLAN 
//cards or all WDS links on the designated vlan..
// ***This function is not called/used by 8651 driver 
int32 devglue_regExtDevice(int8 *devName, uint16 vid, uint8 extPortNum, uint32 *linkId){//, uint16 vid){//, void (*descFreeFunc)(void *)){
	int8 chipVersion[16];
	int32 i;
	if(!devName){//||!descFreeFunc){
		printk("%s:No device name given\n", __FUNCTION__);
		return FAILED;
	}
	if(!vid||vid>4094){
		printk("%s:Last parameter can't be NULL\n", __FUNCTION__);
		return FAILED;
	}

	GetChipVersion(chipVersion, sizeof(chipVersion), NULL);
	if(chipVersion[strlen(chipVersion)-1]!='B'){
		printk("%s:WLAN acceleration requires RTL865xB chip\n", __FUNCTION__);
		return FAILED;
	}else if(!extPortNum||extPortNum>RTL8651_PORT_NUMBER+rtl8651_totalExtPortNum)
	{
		printk("%s:Illegal 865XB ext port 0x%x\n", __FUNCTION__, extPortNum);
		return FAILED;
	}
	if(!linkId){
		printk("%s:Last parameter can't be NULL\n", __FUNCTION__);
		return FAILED;
	}

	//start from 1. 0 is reserved for "ALL WDS ports"
	for(i=1;i<RTLGLUE_MAX_DEVLINKS;i++)
		if(rtl8650DevLink[i].devName[0]=='\0')
			break; //found a unused extension port
	if(i>=RTLGLUE_MAX_DEVLINKS){
		printk("No more free ext port\n");
		return FAILED;
	}
	rtl8650DevLink[i].pci_device=__dev_get_by_name(devName);
	if(!rtl8650DevLink[i].pci_device){
		printk("No such device:%s\n", devName);
		return FAILED;
	}
	strcpy(rtl8650DevLink[i].devName, devName);
	rtl8650DevLink[i].vid=vid;
	rtl8650DevLink[i].portmask=1<<extPortNum;
	if(FAILED==rtl8651_addPeripheralExtPort(extPortNum)){
		printk("Failed when register peripheral extport %d\n", extPortNum);
		return FAILED;
	}
	rtl8650DevLinkCount++;
	printk("Total WLAN/WDS links: %d\n", rtl8650DevLinkCount);
	if(rtl8650DevLinkCount>1)
		rtl8651_fwdEngineEnableWDS(1);
	else
		rtl8651_fwdEngineEnableWDS(0);
	*linkId = i;//i is the registered extension port number. User may use this number from now on
	return SUCCESS; 
}


int32 devglue_unregExtDevice(uint32 linkId){
	int32 i;
	uint32 portmask;
	if(RTLGLUE_MAX_DEVLINKS<linkId)
		return FAILED;
	portmask=rtl8650DevLink[linkId].portmask;
	for(i=0;portmask==1;i++)
		portmask>>=1;		
	if(FAILED==rtl8651_delPeripheralExtPort(i)){
		printk("Failed when deregister peripheral extport %d\n", i);
		return FAILED;
	}
	rtl8650DevLink[linkId].devName[0]='\0';
	rtl8650DevLink[linkId].vid=rtl8650DevLink[linkId].portmask=0;
	rtl8650DevLink[linkId].pci_device=NULL;	
	rtl8650DevLinkCount--;
	printk("Total WLAN/WDS links: %d\n", rtl8650DevLinkCount);
	return SUCCESS;
}


//This function is for OS-dependent part of WLAN driver to get 'devName's liniId number 
//before sending the pkt to 8651 river. Each WLAN pkt to be fast forwarded by 8651 ASIC 
//must be passed with its belonging linkId number to 8651 driver when 
//rtl8651_fwdEngineExtPortRecv() is called.
// ***This function is not called/used by 8651 driver 
int32 devglue_getLinkIDByName(int8 *devName){
	int32 i;
	//start from 1. 
	//0 is reserved for "ALL WDS ports"	
	for(i=1;i<RTLGLUE_MAX_DEVLINKS;i++){
		if(strcmp(devName, rtl8650DevLink[i].devName)==0)
			return i;
	}
	return FAILED;
}

EXPORT_SYMBOL(devglue_regExtDevice);
EXPORT_SYMBOL(devglue_getLinkIDByName);
EXPORT_SYMBOL(devglue_unregExtDevice);

void _mbuf2skbDebug(struct rtl_pktHdr* pPkt, struct rtl_mBuf *pkt_mbuf){
		uint8 *data= pkt_mbuf->m_data;
		uint8 macStr[sizeof"ff:ff:ff:ff:ff:ff"];			
		ether_ntoa_r((ether_addr_t *)&data[6], macStr);
		printk("\nPCI Tx(%d)%s->",pPkt->ph_rxdesc,macStr);
		ether_ntoa_r((ether_addr_t *)&data[0], macStr);
		printk("%s  0x%x  Len:%d...\n",macStr,*((uint16 *)(data+12)), pPkt->ph_len);
		memDump(data+14,28,"PCI Tx");	
}

#define PKT_BUF_SZ 2048
static  struct sk_buff *re865x_mbuf2skb(struct rtl_pktHdr* pPkt) {
	struct sk_buff *new_skb, *pkt_skb;
	struct rtl_mBuf *pkt_mbuf=pPkt->ph_mbuf;
	struct sk_buff *retval=NULL;
	assert(pPkt->ph_mbuf);	
	
	#ifdef RTL865X_DEBUG
	if(wlan_acc_debug>1){
		_mbuf2skbDebug(pPkt,pkt_mbuf);
	}
	#endif
	if((pPkt->ph_rxdesc<0)||(pPkt->ph_rxdesc>=totalRxPkthdr)){
		//cfliu: <0: from upper layer stack, >=totalRxPkthdr: duplicated from fwdEngine
		struct rtl_mBuf *m= pkt_mbuf;
		assert(m);
		pkt_skb = (struct sk_buff *)pkt_mbuf->m_extClusterId;
		if(pkt_skb->len==0){
			int32 lead= UNCACHE(pkt_mbuf->m_data)-UNCACHE(pkt_mbuf->m_extbuf);
			pkt_skb->data+= lead;
			skb_put(pkt_skb, pPkt->ph_len);
		}
		//free mbuf/pkthdr
		mBuf_freeOneMbufPkthdr(m, (void * *) &m->m_extbuf, &m->m_extClusterId,&m->m_extsize);
		return pkt_skb;
	}
	//not from upper layer stack, allocate a new pkt buffer and skb header.
	//new pkt buffer would be linked with pPkt so as to refill swnic's rx ring
	//new skb would have its skb->data points to pPkt->ph_mbuf->m_data.
	new_skb = dev_alloc_skb (PKT_BUF_SZ);
	if (new_skb) 
	{
		int32 align;
		pkt_skb = (struct sk_buff *)pkt_mbuf->m_extClusterId;
		pkt_skb->ip_summed = CHECKSUM_NONE;
		//force set length to 0
		pkt_skb->data = pkt_skb->tail = pkt_skb->head ;
		pkt_skb->len=0;
		//adjust data pointer by mbuf
		skb_reserve(pkt_skb, 16+mBuf_leadingSpace(pkt_mbuf));
		//set real pkt length
		skb_put(pkt_skb, pPkt->ph_len);
#ifdef CONFIG_RTL865X_MBUF_HEADROOM
		align=CONFIG_RTL865X_MBUF_HEADROOM;
#endif
		//refill Rx ring, use new buffer
		mBuf_attachCluster(pkt_mbuf, (void *)UNCACHE(new_skb->data), (uint32)new_skb, 2048, 0, align);

		if (rtlglue_reclaimRxBD(pPkt->ph_rxdesc, pPkt, pkt_mbuf))
			mBuf_freeMbufChain(pkt_mbuf);
		retval= pkt_skb; 
	}else{
		mBuf_freeMbufChain(pkt_mbuf);
		retval= NULL;
	}
	return retval;
}


 
__IRAM static void _extDeviceSendUnicast(uint16 vid, int32 thisLink, struct rtl_pktHdr *thisPkthdr){
	int32 ret;
	struct sk_buff *skb;
	if ((vid!=rtl8650DevLink[thisLink].vid)||!rtl8650DevLink[thisLink].pci_device||!netif_running(rtl8650DevLink[thisLink].pci_device)){
		#ifdef RTL865X_DEBUG
		if(wlan_acc_debug>1) 
			printk("link %d Not running. (%d,%d) Freed!\n", thisLink, vid,rtl8650DevLink[thisLink].vid);
		#endif
		//free unicast pkt since port not ready for send.
		mBuf_driverFreeMbufChain(thisPkthdr->ph_mbuf);
		return;
	}

	skb= (struct sk_buff *)re865x_mbuf2skb(thisPkthdr);
	//mbuf/pkthdr is already freed after re865x_mbuf2skb()
	if(!skb)
		return;
	skb->dev=rtl8650DevLink[thisLink].pci_device;

	#ifdef RTL865X_DEBUG
	if(wlan_acc_debug>1) {
		printk("to pci (linkId %d) %s...\n", thisLink, skb->dev->name);
	}
	#endif


#ifdef CONFIG_RTL865XB_EXP_PERFORMANCE_EVALUATION
	if((_perpciStart == TRUE) && (_perpciPktCount == 0))
			_perpciSkb = skb_clone(skb, GFP_ATOMIC);
#endif
	
#ifdef CONFIG_RTL865XB_3G
	#ifdef RTL_GLUE_DEBUG
	printk("%s %d\n",__FUNCTION__,__LINE__);
	printk("%s %s[before strcpy]\n",__FUNCTION__,skb->cb);
	#endif
	/* for Globalsun 3G project, we set CB[] to indicate this packet is from ROMEDRV */
	strncpy(skb->cb, "FROM_ROMEDRV", 12);
	#ifdef RTL_GLUE_DEBUG
	printk("%s %s[after strcpy]\n",__FUNCTION__,skb->cb);
	#endif
#endif

	ret=dev_queue_xmit(skb);
	if(ret<0){
		#ifdef RTL865X_DEBUG
		if(wlan_acc_debug>1)
			printk("dev_queue_xmit dropped. Reason: %d \n", ret);
		#endif
	}
	return;	
}


#ifdef CONFIG_RTL865XB_EXP_PERFORMANCE_EVALUATION
static int32 _perpciStart = FALSE;
static int32 _perpciInst = TRUE;
static uint32 _perpciPktLimit = 10000;
static uint32 _perpciPktCount = 0;
static uint32 _perpciPktDmzCount = 0;
static uint32 _perpciByteCount = 0;
static struct sk_buff *_perpciSkb; 

void rtlglue_perpcitxStart(int32 instMode, uint32 totalPkt){
	_perpciPktCount = _perpciByteCount = _perpciPktDmzCount = 0;
	_perpciInst = instMode;
	_perpciPktLimit = totalPkt;
	_perpciStart = TRUE;
}

void rtlglue_perpcitxGo(void){
	if(_perpciStart == TRUE){
		struct sk_buff *tmp_skb;
		
		startCOP3Counters(_perpciInst);
		
		while(_perpciPktCount < _perpciPktLimit){
			tmp_skb = skb_clone(_perpciSkb, GFP_ATOMIC);
			dev_queue_xmit(tmp_skb);
			_perpciPktCount++;
			_perpciByteCount += tmp_skb->len + 4;
			}
		}
}

void rtlglue_perpciCount(void){
	if(_perpciStart == FALSE)
		return;
	
	if(++_perpciPktDmzCount == _perpciPktLimit){
		uint32 cycles;
		cycles = stopCOP3Counters();
		printk("%d pkts. Total %d bytes, %d ms.  %u Kbps\n", _perpciPktCount, _perpciByteCount, (uint32)(cycles/200000), (uint32)(_perpciByteCount*200000/cycles));
		_perpciStart = FALSE;
		}
}
#endif

static void _extDeviceSendBcast(uint16 vid, int32 linkId, struct rtl_pktHdr *orig_pktHdr)
{
	struct sk_buff *skb = NULL;
	struct rtl_pktHdr *this_pktHdr = NULL;
	struct rtl_mBuf *orig_mBuf, *this_mBuf = NULL;
	uint8 txList[RTLGLUE_MAX_DEVLINKS];
	int32 incomingLink, idx, txCnt, ret;

	#ifdef RTL865X_DEBUG
	if(wlan_acc_debug > 1)
	{
		printk("Bcast, source LinkID [%d]\n", linkId);
	}
	#endif /* RTL865X_DEBUG */

	// mark off for testing
	//assert(linkId<=0);
	//assert((orig_pktHdr->ph_rxdesc<0)||(orig_pktHdr->ph_rxdesc>=totalRxPkthdr));

	orig_mBuf = orig_pktHdr->ph_mbuf;
	incomingLink = -linkId;

	/*
		Due to there are many overheads to duplicate packet,
		so we must process it very carefully.
		We only copy packet when ther following things are made sure:

			1. There is last one device need this broadcast packet.
			2. That device can deliver this packet.
	*/

	/* check each interface for packet forwarding */

	memset(txList, 0, sizeof(txList));
	txCnt = 0;
	for (idx = 1; idx < RTLGLUE_MAX_DEVLINKS ; idx ++)
	{
		if (	(idx == incomingLink) ||
			(rtl8650DevLink[idx].pci_device == NULL) ||
			(rtl8650DevLink[idx].vid != vid) ||
			(!netif_running(rtl8650DevLink[idx].pci_device))	)
		{
			continue;
		}
		#ifdef RTL865X_DEBUG
		if(wlan_acc_debug > 2)
		{
			printk("[%s] LinkID-%d[%s] receive this packet\n", __FUNCTION__, idx, rtl8650DevLink[idx].pci_device->name);
		}
		#endif /* RTL865X_DEBUG */
		/* mark this interface to indicate it need this packet's forwarding */
		txList[idx] = 1;
		txCnt ++;
	}

	/* If there is no any interface want to receive this packet, we silently ignore it */
	if (txCnt == 0)
	{
		#ifdef RTL865X_DEBUG
		if(wlan_acc_debug > 2)
		{
			printk("[%s] No broadcast needed, drop packet: P[%p]M[%p]LinkID[%d]\n",
						__FUNCTION__,
						orig_pktHdr,
						orig_mBuf,
						linkId);
		}
		#endif /* RTL865X_DEBUG */

		mBuf_driverFreeMbufChain(orig_mBuf);

		goto out;
	}

	/* send to each interface */
	for (idx = 1; idx < RTLGLUE_MAX_DEVLINKS ; idx ++)
	{
		/* this interface need to receive this packet */
		if (txList[idx] == 1)
		{
			txCnt --;	/* decrease remaining tx Count */

			if (txCnt > 0)
			{	/* other interfaces need this packet, so we MUST duplicate it */
				this_mBuf = mBuf_dupPacket(orig_mBuf, MBUF_DONTWAIT);

				if(this_mBuf == NULL)
				{
					#ifdef RTL865X_DEBUG
					if(wlan_acc_debug > 2)
					{
						printk("[%s] Bcast pkt duplicated failed!!!\n", __FUNCTION__);
					}
					#endif
					
					mBuf_driverFreeMbufChain(orig_mBuf);

					goto out;
				}

				this_pktHdr = this_mBuf->m_pkthdr;
			} else
			{	/* no other interfaces need it, so we MUST use the original packet to send. */

				this_mBuf = orig_mBuf;
				this_pktHdr = orig_pktHdr;
			}

			/* we have gotten mbuf and packet header, we send it now */

			/* format translation and related field setting */
			skb = (struct sk_buff *)re865x_mbuf2skb(this_pktHdr);

			if(skb == NULL)
			{
				/*
					Allocation FAIL, release mbuf and continue next process.
				*/
				#ifdef RTL865X_DEBUG
				printk("FATAL Error: [%s] Out of skb!\n", __FUNCTION__);
				#endif

				mBuf_driverFreeMbufChain(this_mBuf);
				continue;
			}

			/* fill skb fields */
			skb->dev = rtl8650DevLink[idx].pci_device;

			#ifdef RTL865X_DEBUG
			if(wlan_acc_debug > 1)
			{
				printk("to pci (linkId %d) %s...\n", idx, skb->dev->name);
			}
			#endif

#ifdef CONFIG_RTL865XB_3G
			#ifdef RTL_GLUE_DEBUG
			printk("%s %d\n",__FUNCTION__,__LINE__);
			printk("%s %s[before strcpy]\n",__FUNCTION__,skb->cb);
			#endif
			/* for Globalsun 3G project, we set CB[] to indicate this packet is from ROMEDRV */
			strncpy(skb->cb, "FROM_ROMEDRV", 12);
			#ifdef RTL_GLUE_DEBUG			
			printk("%s %s[after strcpy]\n",__FUNCTION__,skb->cb);
			#endif
#endif

			/* send packet */
			ret = dev_queue_xmit(skb);
			if(ret < 0)
			{
				#ifdef RTL865X_DEBUG
				if(wlan_acc_debug > 1)
				{
					printk("[%s] dev_queue_xmit dropped. Reason: %d \n", __FUNCTION__, ret);
				}
				#endif
			}
		}
	}

out:
	return;
}

#if 0
//For more info about this function, read API definition in rtl_glue.h
static void _extDeviceSendBcast(uint16 vid, int32 linkId, struct rtl_pktHdr *orig_pktHdr){
	struct sk_buff *skb;
	struct rtl_pktHdr *thisPkthdr, *firstCopy;
	int32 ret, thisLink;
	struct rtl_mBuf *newMbuf;

	thisLink=linkId;
	thisPkthdr=orig_pktHdr;
	firstCopy=NULL;

	//linkId<=0, it's a bcast pkt or unknown unicast
	thisLink=1;//start from 1
	//duplicate and free original packet ASAP to avoid rx ring runout.
	newMbuf= mBuf_dupPacket(orig_pktHdr->ph_mbuf, MBUF_DONTWAIT);
	mBuf_driverFreeMbufChain(orig_pktHdr->ph_mbuf);
	if(!newMbuf){
		#ifdef RTL865X_DEBUG
		if(wlan_acc_debug>2)
			printk("Bcast pkt duplicated failed!!!\n");
		#endif
		return;		
	}
	
	#ifdef RTL865X_DEBUG
	if(wlan_acc_debug>2) {
		printk("Bcast pkt duplicated. New:%x\n", (uint32)newMbuf);
	}
	#endif
	firstCopy=newMbuf->m_pkthdr;//firstCopy would be copied to each extport later...
next_link:
	//last pkt sent freed. prepare a copy for this link from firstCopy
	assert(firstCopy);
	newMbuf= mBuf_dupPacket(firstCopy->ph_mbuf, MBUF_DONTWAIT);
	if(!newMbuf){
		goto end_bcast;
	}
next_link_pktready:
	//send this copied pkt to thisLink.
	#ifdef RTL865X_DEBUG
	if(wlan_acc_debug>2) {
		printk("dup pkt for bcast to link %d\n", thisLink);
	}
	#endif
	thisPkthdr=newMbuf->m_pkthdr;

	assert((thisPkthdr->ph_rxdesc<0)||(thisPkthdr->ph_rxdesc>=totalRxPkthdr));
	if (rtl8650DevLink[thisLink].vid!=vid||
		thisLink==-linkId||!rtl8650DevLink[thisLink].pci_device||
		!netif_running(rtl8650DevLink[thisLink].pci_device)
	){
		#ifdef RTL865X_DEBUG
		if(wlan_acc_debug>1) 
			printk("bcast to link %d aborted.Freed!\n", thisLink);
		#endif

		//loop until next registered link found in this vlan or quit.
		do{
			if(thisLink>=RTLGLUE_MAX_DEVLINKS){
				mBuf_driverFreeMbufChain(thisPkthdr->ph_mbuf);
				goto end_bcast;
			}
			thisLink++;
			if(rtl8650DevLink[thisLink].devName[0]&&rtl8650DevLink[thisLink].vid==vid){
				#ifdef RTL865X_DEBUG
				if(wlan_acc_debug>1) 
					printk("Try send to next link %d\n", thisLink);
				#endif
				goto next_link_pktready;
			}
		}while(1);
		assert(0);//should not come here.
	}
	skb= (struct sk_buff *)re865x_mbuf2skb(thisPkthdr);
	//mbuf/pkthdr is already freed after re865x_mbuf2skb()
	if(!skb){
		#ifdef RTL865X_DEBUG
		printk("%s:%d: out of skb\n", __FUNCTION__,__LINE__);
		#endif
		goto end_bcast;
	}
	skb->dev=rtl8650DevLink[thisLink].pci_device;

	#ifdef RTL865X_DEBUG
	if(wlan_acc_debug>1) {
		printk("to pci (linkId %d) %s...\n", thisLink, skb->dev->name);
	}
	#endif

	ret=dev_queue_xmit(skb);
	if(ret<0){
		#ifdef RTL865X_DEBUG
		if(wlan_acc_debug>1)
			printk("dev_queue_xmit dropped. Reason: %d \n", ret);
		#endif
	}

	if(thisLink<RTLGLUE_MAX_DEVLINKS){
		thisLink++;
		goto next_link;
	}

end_bcast:
	//remember to free the template 'firstCopy' before we return
	mBuf_driverFreeMbufChain(firstCopy->ph_mbuf);
	//printk("\n");
	return;
}
#endif



__IRAM void rtlglue_extDeviceSend(uint16 vid, int32 linkId, struct rtl_pktHdr *orig_pktHdr){
	
	#ifdef RTL_GLUE_DEBUG
	printk("%s linkID = %d\n",__FUNCTION__,linkId);
	#endif
	
	#ifdef RTL865X_DEBUG
	if(wlan_acc_debug>2)
		printk("(d:%08x)",(uint32)orig_pktHdr->ph_mbuf->m_data );
	#endif

	//linkId>0: unicast
	//linkId=0: bcast from LAN port, send to all existing links.
	//linkId<0: bcast from abs(linkId) WLAN card or WDS link. Would avoid sending to that link again.
	if(linkId > 0)
		return _extDeviceSendUnicast(vid, linkId, orig_pktHdr);
	return _extDeviceSendBcast(vid, linkId, orig_pktHdr);
}
#endif


#define BDINFO_ADDR 0xbfc04000
void rtlglue_getMacAddress(ether_addr_t * macAddress, uint32 * number) {
	rtl8651_memcpy((char*)macAddress,(char*)BDINFO_ADDR,6);
}

void rtlglue_getRingSize(uint32 *rx, uint32 *tx){
	if(rx)
		*rx = totalRxPkthdr;
	if(tx)
		*tx = totalTxPkthdr;
}

#endif /*CONFIG_RE865X*/

//extern int jiffies;
/*
 * Note: the unit of jiffies is 10ms.
 */

/*
 * getsectime() returns seconds.
 */
uint32 getsectime(void) {
	return jiffies/100;
		
}

/*
 * getmstime() returns milli-second (1/1000 sec).
 */
uint32 getmstime(void) {
        return jiffies*10;
}

uint32 rtlglue_getmstime( uint32* pTime )
{
	return *pTime = getmstime();
}



static int totalmem;
void *rtlglue_malloc(size_t NBYTES) {
	totalmem = totalmem+NBYTES;
	if(NBYTES==0) return NULL;
	return kmalloc(NBYTES,GFP_KERNEL);
}

void rtlglue_free(void *APTR) {
	kfree(APTR);
}
//------------------------  LINUX DEPEND GLUE STARTS HERE-------------//
#endif /*RTL865X_TEST*/



