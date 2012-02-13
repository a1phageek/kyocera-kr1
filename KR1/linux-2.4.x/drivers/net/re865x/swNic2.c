/*
* ----------------------------------------------------------------
* Copyright c                  Realtek Semiconductor Corporation, 2002  
* All rights reserved.
* 
* $Header: /home/cvsroot/uClinux-dist/linux-2.4.x/drivers/net/re865x/swNic2.c,v 1.61 2005/01/26 08:38:33 cfliu Exp $
*
* Abstract: Switch core NIC driver source code version 2.
*
* $Author: cfliu $
*
* $Log: swNic2.c,v $
* Revision 1.61  2005/01/26 08:38:33  cfliu
* *: Remove MBUF_DESC_RUNOUT_IE from Intr mask since we always allocate one
* mbuf for a pkthdr in code so when pkthdr's runout event is solved, mbuf
* runout is naturally solved.
*
* Revision 1.60  2005/01/26 02:42:08  cfliu
* *** empty log message ***
*
* Revision 1.59  2005/01/11 16:16:06  cfliu
* +:Add mBuf_setNICRxRingSize to tell mbuf module the size of NIC Rx ring
*
* Revision 1.58  2005/01/10 03:17:44  danwu
* * add CONFIG_RTL865XB_EXP_PERFORMANCE_EVALUATION for DMA throughput measurement
*
* Revision 1.57  2005/01/09 08:38:55  cfliu
* -:Undefine SWNIC_EARLYSTOP,
* +:Always put to IRAM: swNic_intHandler, _swNic_recvLoop, swNic_rxThread, swNic_isrReclaim, swNic_isrTxRecycle, swNic_write
* +:Move rtl8651_timeUpdate() from per pkt based to per intr based to save time
* +:Add link change Intr statistics counter.
*
* Revision 1.56  2004/12/27 01:53:16  yjlou
* +: add ROMEPERF code to measure performance.
*
* Revision 1.55  2004/12/20 12:57:21  yjlou
* +: add romeperf point to measure performance.
*
* Revision 1.54  2004/11/29 13:40:32  yjlou
* +: support ROMEperf
*
* Revision 1.53  2004/11/22 04:36:09  chenyl
* *: move TX-alignment code from layer2fwd to swNic2
*
* Revision 1.52  2004/11/19 09:56:39  cfliu
* *: Add nic cmd group, add 5 cmds: counter, ring, loopback, hub and cmd
* 	counter: dump counter value or reset counters
* 	ring: dump Tx/Rx rings or cleanup and reallocate whole ring
* 	loopback: Dummy port A in, port B out diagnostics mode
* 	hub: Make RTL865x as a Hub
* 	cmd: Let user resume/stop Tx/Rx or specify max pkts processed in
* each ISR
* *: Fix Tx pending pkt stops Rx recv bug.
*
* Revision 1.51  2004/11/12 10:55:10  chhuang
* *: bug fixed: when init switch nic, enable Tx/Rx after setting CPUICR to shift 2 bytes
*
* Revision 1.50  2004/11/10 05:13:52  cfliu
* -: remove call to rtl8651_getChipVersion, use GetChipVersion in setup.c instead
*
* Revision 1.49  2004/10/22 11:42:24  cfliu
* *: Fix CPU Rx overrun to pkts on Tx ring problem. Each pkt rcvd from/sent to
* NIC is now labeled. Rx loop checks this bit to prevent overrun.
*
* Revision 1.48  2004/10/21 06:12:59  chenyl
* *: bug fix: free L2-broadcasted Mcast packet when we send it fail.
*
* Revision 1.47  2004/10/08 09:05:29  chenyl
* *: remove debugging message with potential-risks
*
* Revision 1.46  2004/09/30 08:29:45  danwu
* * reset m_data in swNic_isrReclaim (for either CONFIG_RTL865X_MBUF_HEADROOM or CONFIG_RTL865X_MULTILAYER_BSP is undefined)
*
* Revision 1.45  2004/09/27 12:49:16  yjlou
* +: support ROMEreal function (CONFIG_RTL865X_ROMEREAL)
*
* Revision 1.44  2004/09/16 04:25:17  cfliu
* Add SwNic early Rx stop function
*
* Revision 1.43  2004/08/05 06:39:11  chenyl
* +: fragment shortest path forwarding
* *: UDPBomb don't aware ignore Type Checking result
*
* Revision 1.42  2004/07/28 10:48:20  chenyl
* *: bug fix : [re_core.c]
* 		- rtl865x_fastRx: queue-packets to Protocol Stack and failed, free Mbuf ERROR
* *: bug fix : [swNic2.c]
* 		- swNic_write: process ERROR when mbuf padding failed
* *: bug fix : [rtl8651_tblDrvFwd.c]
* 		- _rtl8651_l34ActExec: process _RTL8651_NAT_IGNORE in _RTL8651_NPT case
* *: bug fix : [rtl8651_tblDrvFwd.c]
* 		- rtl8651_fwdEngineInput: when process result is FWDENG_ACL_DROP_NOTIFY,
* 				we must either generate ICMP Error message with newly allocated packet and trap
* 				the original packet, or generate ICMP Error message using original packet and return
* 				SUCCESS.
* 				we can not send ICMP Error message with the original packet and trap it to Procotol Stack.
*
* Revision 1.41  2004/07/27 10:39:02  cfliu
* *: Fix all compile warnings
*
* Revision 1.40  2004/07/13 07:08:45  cfliu
* +: Update NIC descriptor ring dump command
*
* Revision 1.39  2004/07/09 12:19:39  cfliu
* no message
*
* Revision 1.38  2004/07/08 14:30:48  cfliu
* +:  Reserve 128 byte headroom for any pkt get from mbuf pool
*
* Revision 1.37  2004/07/06 14:08:37  cfliu
* +: Flush D-cache when pkt send to protocol stack. Don't copy packet if on 8650B and NIC enabled iphdr aligned feature.
*
* Revision 1.36  2004/07/06 01:15:13  cfliu
* +: Add CONFIG_RTL865X_NETWORK_IO flag
*
* Revision 1.35  2004/07/04 14:53:01  cfliu
* +: Change NIC Rx align to 2 byte aligned. change recv loop prototype so NIC can be forced stop by fwd engine.
*
* Revision 1.34  2004/06/29 07:11:46  cfliu
* no message
*
* Revision 1.33  2004/06/25 13:53:53  chhuang
* regular update
*
* Revision 1.32  2004/06/21 07:18:16  chhuang
* *:  modify pPkthdr->ph_rxdesc = -1; to
*       pPkthdr->ph_rxdesc = totalRxPkthdr;
*
* Revision 1.31  2004/06/08 10:26:24  cfliu
* *: Fix runout lost interrupt problem
*
* Revision 1.30  2004/06/08 08:10:56  tony
* *: fix PPTP broadcast problem for Windows 2000 Server.
*
* Revision 1.29  2004/05/20 03:41:58  cfliu
* Bugfix: Fix losing Rx runout interrupt problem.
*
* Revision 1.28  2004/05/19 08:45:09  orlando
* add code for bi color led gpio control
*
* Revision 1.27  2004/05/13 05:47:32  tony
* new features: support PPTP/L2TP with WLAN on RTL865XB.
*
* Revision 1.26  2004/05/12 05:15:04  tony
* support PPTP/L2TP in RTL865XB.
*
* Revision 1.25  2004/05/03 15:11:44  cfliu
* no message
*
* Revision 1.24  2004/05/03 14:36:35  cfliu
* add extension port support on WLAN acceleration
*
* Revision 1.23  2004/05/03 02:53:03  chenyl
* *: swNic_getRingSize -> rtlglue_getRingSize and move it from swNic2.c to rtl_glue.c
* *: set default upstream ip to 0 if we config interface to dhcp
*
* Revision 1.22  2004/04/30 12:59:00  chenyl
* *: bug fix in swNic2.c, isrReclaim()
*
* Revision 1.21  2004/04/30 08:58:08  chenyl
* +: ip multicast/igmp proxy
*
* Revision 1.20  2004/04/21 07:05:25  danwu
* swNic_write: do not clear extension port settings while verification package included
*
* Revision 1.19  2004/04/19 13:48:17  tony
* make m_data offset 128 bytes, and make it can run well with WLAN mode.
*
* Revision 1.18  2004/04/16 06:24:50  cfliu
* Turn off m_data+128 code temporarily. Must revise it...
*
* Revision 1.17  2004/04/12 05:59:13  tony
* make mbuf->m_data = mbuf->m_extbuf + 128
*
* Revision 1.16  2004/04/08 12:07:40  cfliu
* no message
*
* Revision 1.15  2004/04/08 02:21:41  chhuang
* turn off PKTHDR_BRIDGING and PKTHDR_HWLOOKUP flags
*
* Revision 1.14  2004/04/01 03:31:28  jzchen
* Reserved field need configured to zero for 8650B able to work
*
* Revision 1.13  2004/03/30 11:48:49  cfliu
* Add link change interrupt
*
* Revision 1.12  2004/03/26 11:21:55  cfliu
* swNic_intHandler handles most Rx ISR jobs now...
*
* Revision 1.11  2004/03/24 08:40:02  cfliu
* Fix intr lost problem.
*
* Revision 1.10  2004/03/24 01:10:07  cfliu
* Rewrite NIC recv loop.
*
* Revision 1.9  2004/03/23 09:31:03  cfliu
* Fix NIC overrun problemEnlarge mbuf & NIC init buffer size.
*
* Revision 1.8  2004/03/19 12:07:18  cfliu
* Bug fix to handle runout stuck and Rx intr lost risk..
*
* Revision 1.7  2004/03/10 11:27:46  cfliu
* separate layer2 functions to separate C src file
*
* Revision 1.6  2004/03/04 09:29:02  yjlou
* *: change directory of include file: from rtl86xx_tbl/ to rtl865x/
*
* Revision 1.5  2004/02/27 09:42:40  cfliu
* Move away linux dependent definition
*
* Revision 1.4  2004/02/24 04:08:02  cfliu
* Add WLAN HW acceleration
*
* Revision 1.3  2003/12/22 12:30:59  orlando
* mark enableLbcTimeout()
*
* Revision 1.2  2003/12/18 12:47:42  rupert
* fix new nic driver
*
* Revision 1.1  2003/12/09 15:35:31  cfliu
* new NIC driver. To enable it, use CONFIG_RTL865X_NICDRV2. Performance gain is 2.5X
*
* Revision 1.1  2003/11/26 00:06:05  cfliu
* switch NIC driver version 2 initial version.
*
*/
#ifdef __linux__
#include <linux/config.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#endif
#include <asm/rtl865x/interrupt.h>
#include "rtl865x/rtl_types.h"
#include "rtl865x/rtl_glue.h"
#include "rtl865x/mbuf.h"
#include "swNic2.h"
#ifdef CONFIG_RTL865X_ROMEPERF
#include "romeperf.h"
#endif
#include "rtl865x/assert.h"
#include "rtl865x/asicRegs.h"
#include "rtl865x/rtl8651_layer2.h"

static proc_input_pkt_funcptr_t installedProcessInputPacket;
int8 swNic_Id[] = "$Id: swNic2.c,v 1.61 2005/01/26 08:38:33 cfliu Exp $";
/* descriptor ring related variables */
 uint32	totalRxPkthdr, totalRxMbuf,totalTxPkthdr, totalTxPkthdrShift;  /*total desc count on each ring */
 uint32 	rxMbufIndex, rxPhdrIndex, txDoneIndex, txFreeIndex; /* current desc index */
 uint32 	*RxPkthdrRing,*RxMbufRing, *TxPkthdrRing; /* descriptor ring */
static int32	rxRunoutIdx, lastReclaim, lastIntRxDescIdx;
static int32  rxEnable, txEnable;
#undef SWNIC_EARLYSTOP 
#ifdef SWNIC_EARLYSTOP
static int32 nicRxEarlyStop;
static int32 nicRxAbort;
#endif

/* swnic debug counters */
static int32   rxIntNum;
static int32 linkChanged;
static int32   txIntNum;
 int32   rxPkthdrRunoutNum,rxPkthdrRunoutSolved;
 int32 rxPkthdr_runout;
static int32   rxMbufRunoutNum;
static int32   rxPktErrorNum;
static int32   txPktErrorNum;
static int32   rxPktCounter;
static int32   txPktCounter;
void swNic_rxThread(unsigned long);
static void swNic_isrTxRecycle(int32);
#ifdef CONFIG_RTL865X_CLE
static int32 swNic_resetCounters(void);
#endif
 int32 swNic_write(void*);
#define SWNIC_DEBUG
/* NIC_TX_ALIGN is used to let some special tx packet lengths become 4-byte alignment */
#define NIC_TX_ALIGN

#ifdef CONFIG_RTL865XB_EXP_PERFORMANCE_EVALUATION
static int32 _pernicStart = FALSE;
static int32 _pernicInst = TRUE;
static uint32 _pernicPktLimit = 10000;
static uint32 _pernicPktCount = 0;
static uint32 _pernicByteCount = 0;

void swNic_pernicrxStart(int32 instMode, uint32 totalPkt){
	_pernicPktCount = _pernicByteCount = 0;
	_pernicInst = instMode;
	_pernicPktLimit = totalPkt;
	_pernicStart = TRUE;
}

void swNic_pernicrxEnd(uint32 *pktCount, uint32 *byteCount){
	*pktCount = _pernicPktCount;
	*byteCount = _pernicByteCount;
	_pernicStart = FALSE;
}
#endif

int32 swNic_linkChgIntHandler(void){
	REG32(CPUIISR) = (uint32)LINK_CHANG_IP;
	//rtlglue_printf("interrupt link change %x\n",(uint32)jiffies);
	rtl8651_updateLinkChangePendingCount();
	if(SUCCESS==rtl8651_updateLinkStatus()){
		#if 0
		#if defined(CONFIG_RTL865X_BICOLOR_LED)
		int32 i;
		for(i=0; i<5; i++) {  
			//while(!(REG32(PHY_BASE+(i<<5)+0x4) & 0x20));
			if(REG32((PHY_BASE+(i<<5)+0x4)) & 0x4) { //link is up
				//printf("port %d phyControlRegister 0[%08x] 4[%08x]\n",i,REG32(PHY_BASE+(i<<5)),REG32(PHY_BASE+(i<<5)+4));
				//printf("Port %d Link Change!\n",i);
					if(REG32((PHY_BASE+(i<<5))) & 0x2000){
					/* link up speed 100Mbps */
					/* set gpio port  with high */
					//printf("port %d speed 100M %08x\n",i,REG32(PHY_BASE+(i<<5)));
					REG32(PABDAT) |= (1<<(16+i));
				}else{
					/* link up speed 10Mbps */
					//rtlglue_printf("linkSpeed10M\n");
					/* set gpio port  with high */
					//printf("port %d speed 10M %08x\n",i,REG32(PHY_BASE+(i<<5)));
					REG32(PABDAT) &= ~(1<<(16+i));
				} /* end if  100M */
			}/* end if  link up */
		}/* end for */
		#endif /* CONFIG_RTL865X_BICOLOR_LED */
		#endif
		return SUCCESS;
	}else
		rtlglue_printf("Link chg int handle error!\n");
	//rtlglue_printf("interrupt link change %x\n",jiffies);	
	return FAILED;
}

int32 swNic_rxRunoutTxPending(struct rtl_pktHdr *pPkthdr){
		//exception handling
        struct rtl_pktHdr *freePkthdrListHead, *freePkthdrListTail;
        struct rtl_mBuf *freeMbufListHead, *freeMbufListTail;
        uint32  wrap=0;
   	  rtlglue_printf("Desc %d (ph: %08x): Rx runout by pending Tx\n", rxPhdrIndex,(uint32)pPkthdr );
        pPkthdr->ph_rxdesc = totalRxPkthdr;
        if (rxPhdrIndex==totalRxPkthdr-1)
		wrap=0x2;
        if ((mBuf_driverGetPkthdr(1, &freePkthdrListHead, &freePkthdrListTail))!=1){
		rtlglue_printf("No more pkthdr. runout NOT solved!!\n");
		return FAILED;
        }
        if(1!=mBuf_driverGet(1, &freeMbufListHead, &freeMbufListTail)){
		rtlglue_printf("No more mbuf. runout NOT solved!!\n");
             mBuf_driverFreePkthdr(freePkthdrListHead, 1, 0);
             return FAILED;
        }
	#ifndef CONFIG_RTL865X_CACHED_NETWORK_IO
            freeMbufListHead->m_extbuf=(uint8 *)UNCACHE(freeMbufListHead->m_extbuf);
            freeMbufListHead->m_data=(uint8 *)UNCACHE(freeMbufListHead->m_data);
	#endif

	#if defined(CONFIG_RTL865X_MBUF_HEADROOM)&&defined(CONFIG_RTL865X_MULTILAYER_BSP)
            if(mBuf_reserve(freeMbufListHead, CONFIG_RTL865X_MBUF_HEADROOM))
                    rtlglue_printf("Failed when init Rx %d\n", rxPhdrIndex);
	#endif

            freePkthdrListHead->ph_flags&=~PKTHDR_DRIVERHOLD;
            RxMbufRing[rxPhdrIndex] = (uint32) freeMbufListHead | DESC_SWCORE_OWNED|wrap;
            RxPkthdrRing[rxPhdrIndex] = (uint32) freePkthdrListHead | DESC_SWCORE_OWNED | wrap;
	
	return SUCCESS;
}


//#pragma ghs section text=".iram"
__IRAM int32 swNic_intHandler(int32 *param)
{
	int32 curDesc=-1;
	int32 sched=0;
	uint32  cpuiisr,intPending;
	spin_lock(rtl865xSpinlock);
	while(1){
		/* Read the interrupt status register */
	    	cpuiisr=intPending = REG32(CPUIISR);
		// filter those intr we don't care. only those intrs with its IIMR bit set would trigger ISR.
		intPending &= REG32(CPUIIMR);
		//rtlglue_printf("mask:%x, iisr:%x, iimr:%x\n", INTPENDING_NIC_MASK,cpuiisr,REG32(CPUIIMR));
		/* Check and handle NIC interrupts */
		if (intPending & INTPENDING_NIC_MASK){
			REG32(CPUIISR) = intPending;
			if(intPending & PKTHDR_DESC_RUNOUT_IP){
				REG32(CPUIIMR)&=~PKTHDR_DESC_RUNOUT_IE;
				sched=1;
			}
			if ( intPending &RX_DONE_IP){
				REG32(CPUIIMR)&=~RX_DONE_IE;
				sched=1;
			}
			if (sched){
				curDesc=(uint32 *)REG32(CPURPDCR)-RxPkthdrRing;

				if (intPending & PKTHDR_DESC_RUNOUT_IP){
					rxPkthdrRunoutNum++;
					rxRunoutIdx=curDesc;
					rxPkthdr_runout=1;
				}

				if(curDesc-1<0)
					curDesc=totalRxPkthdr-1;
				else
					curDesc-=1;
				lastIntRxDescIdx=curDesc; //Last pkt of this run. ASIC won't run cross it before we consume this last pkt.

				if(intPending & RX_DONE_IP)
					rxIntNum++;

			}

			if ( intPending & (RX_ERR_IP|TX_ERR_IP)){
				if ( intPending& TX_ERR_IP)
					txPktErrorNum++;
				else
					rxPktErrorNum++;
			}

		}
		/* Check and handle link change interrupt */
		if (intPending & LINK_CHANG_IP) 
			swNic_linkChgIntHandler();
		if ((intPending & (INTPENDING_NIC_MASK|LINK_CHANG_IP)) == 0){
			REG32(CPUIISR) = cpuiisr & ~(INTPENDING_NIC_MASK|LINK_CHANG_IP); //clear all uninterested intrs 
			break;
		}
	}
	spin_unlock(rtl865xSpinlock);	
	return sched;
}

__IRAM static int32 _swNic_recvLoop(int32 last){
	volatile struct rtl_pktHdr * pPkthdr; //don't optimize
	volatile struct rtl_mBuf * pMbuf;	//don't optimize
	int32 count=0;
	do{
#ifdef CONFIG_RTL865X_ROMEPERF
		rtl8651_romeperfEnterPoint(ROMEPERF_INDEX_RECVLOOP);
#endif
		/* Increment counter */
		if((RxPkthdrRing[rxPhdrIndex]&DESC_OWNED_BIT)==1){ 
			goto out;
		}
#ifdef SWNIC_EARLYSTOP		
		if(nicRxEarlyStop && ((count & nicRxEarlyStop)==nicRxEarlyStop)){//check global interrupt status
			uint32 gisrNow = REG32(GISR);
			if(gisrNow & 0x65000000){ //Bit 30: USB, Bit 29:PCMCIA, Bit 26: PCI, Bit 24:GPIO
				nicRxAbort=1;
				goto out;						
			}
		}
#endif		
#ifdef CONFIG_RTL865X_CACHED_NETWORK_IO
#if 0
		/*Invalidate D-Cache*/
		lx4180_writeCacheCtrl(0);
		lx4180_writeCacheCtrl(1);
		lx4180_writeCacheCtrl(0);
		pPkthdr = (struct rtl_pktHdr *) (RxPkthdrRing[rxPhdrIndex] & ~(DESC_OWNED_BIT | DESC_WRAP));
		pMbuf = pPkthdr->ph_mbuf;

#else
		pPkthdr = (struct rtl_pktHdr *) (RxPkthdrRing[rxPhdrIndex] & ~(DESC_OWNED_BIT | DESC_WRAP));
			
		pMbuf = pPkthdr->ph_mbuf;
		//force update d-cache if hit.
		src32=(uint32 *)UNCACHE(pPkthdr);
		dst32=(uint32 *)CACHED(pPkthdr);
		dst32[0]=src32[0];
		dst32[1]=src32[1];
		dst32[2]=src32[2];
		dst32[3]=src32[3];
		src32=(uint32 *)UNCACHE(pMbuf);
		dst32=(uint32 *)CACHED(pMbuf);
		dst32[0]=src32[0];
		dst32[1]=src32[1];
		dst32[2]=src32[2];
		dst32[3]=src32[3];
		//pkt from ASIC, convert to uncached data pointer for used in 
		//fwd engine
		pMbuf->m_data=UNCACHE(pMbuf->m_data);
		pMbuf->m_extbuf=UNCACHE(pMbuf->m_extbuf);
#endif

#else
		pPkthdr = (struct rtl_pktHdr *) (RxPkthdrRing[rxPhdrIndex] & ~(DESC_OWNED_BIT | DESC_WRAP));
		pMbuf = pPkthdr->ph_mbuf;

#endif //CONFIG_RTL865X_CACHED_NETWORK_IO

#ifdef CONFIG_RTL865XB_EXP_PERFORMANCE_EVALUATION
		if(_pernicStart == TRUE){
			static uint32 start, end;
			if(!_pernicPktCount){
				startCOP3Counters(_pernicInst);
				start = jiffies;
				}
			else if(_pernicPktCount == _pernicPktLimit){
				end = jiffies;
				stopCOP3Counters();
				printk("%d pkts. Total %d bytes, %d ms.  %u KBps\n", _pernicPktCount, _pernicByteCount, (uint32)((end-start)*10), (uint32)(_pernicByteCount/((end-start)*10)));
				_pernicStart = FALSE;
				}
			_pernicPktCount++;
			_pernicByteCount += pPkthdr->ph_len + 4;
			swNic_isrReclaim(rxPhdrIndex, pPkthdr, pMbuf);
			/* Increment index */
			if ( ++rxPhdrIndex == totalRxPkthdr )
				rxPhdrIndex = 0;
			if ( ++rxMbufIndex == totalRxMbuf )
				rxMbufIndex = 0;
			continue;
			}
#endif

		assert(pPkthdr->ph_len>0);
		if((pPkthdr->ph_flags&PKTHDR_DRIVERHOLD)==0){
			//exception handling
			swNic_rxRunoutTxPending((struct rtl_pktHdr *)pPkthdr);
			goto out;
		}
		count++;
		//SETBITS(pPkthdr->ph_flags, PKT_INCOMING); //packet from physical port
		pPkthdr->ph_rxdesc=rxPhdrIndex;
		pPkthdr->ph_flags&=~PKTHDR_DRIVERHOLD;
		//Transform extension port numbers to continuous number for fwd engine.

#ifdef CONFIG_RTL865X_MULTILAYER_BSP //Default run this except L2 BSP.
		//must call this API after rxPhdrIndex is assigned...
		if(rtl8651_rxPktPreprocess(pPkthdr)){
			rtlglue_printf("Drop rxDesc=%d\n",rxPhdrIndex );
			//memDump(pPkthdr->ph_mbuf->m_data,  pPkthdr->ph_len,"Loopback Pkt");
			swNic_isrReclaim(rxPhdrIndex, pPkthdr, pMbuf);
		}else
#endif
		{
#ifdef CONFIG_RTL865X_ROMEREAL
			rtl8651_romerealRecord( pPkthdr, 0x00000001 );
#endif/*CONFIG_RTL865X_ROMEREAL*/

			/* Invoked installed function pointer to handle packet */
			(*installedProcessInputPacket)((struct rtl_pktHdr*)pPkthdr);
		}
#ifdef SWNIC_DEBUG
		assert(rxPhdrIndex==rxMbufIndex);
#endif
		/* Increment index */
		if ( ++rxPhdrIndex == totalRxPkthdr )
			rxPhdrIndex = 0;
		if ( ++rxMbufIndex == totalRxMbuf )
			rxMbufIndex = 0;
	}while(last>=0&&rxPhdrIndex!=last);
out:
	return count;
}

__IRAM void swNic_rxThread(unsigned long param)
{
	int32 rxDescIdx=lastIntRxDescIdx;
	int32 latestIdx;
	int32 s;
	spin_lock_irqsave(rtl865xSpinlock,s);
#if EVENT_TRIGGER_TIMEUPDATE > 0
	rtl8651_timeUpdate(0);  //cfliu: check once per interrupt, not per pkt.
#endif

next_round:
#ifdef CONFIG_RTL865X_ROMEPERF
	rtl8651_romeperfExitPoint(ROMEPERF_INDEX_UNTIL_RXTHREAD);/* start at irq.c:irq_dispatch() */
#endif

#ifdef SWNIC_EARLYSTOP	
	nicRxAbort=0;
#endif
	if(rxPhdrIndex!=rxDescIdx){
		rxPktCounter+=_swNic_recvLoop(rxDescIdx);
	}	

	if(rxPhdrIndex!=rxDescIdx){
#ifdef SWNIC_EARLYSTOP	
		if(!nicRxAbort)
#endif
		{
			REG32(CPUIIMR)|= RX_DONE_IE;
			goto out; //abort in _swNic_recvLoop(). 
		}
	}
	//Last one to recv, but see if we can receive again first
	latestIdx=(uint32 *)REG32(CPURPDCR)-RxPkthdrRing;
	if(latestIdx-1<0)
		latestIdx = totalRxPkthdr-1;
	else
		latestIdx-=1;
	
	//Did ASIC recv more pkt since we last see it?
	if(rxDescIdx!=latestIdx){
#ifdef SWNIC_EARLYSTOP	
		if(!nicRxAbort)
#endif
		{
			//yes, keep intr closed. we still have more job to do.
			rxDescIdx=latestIdx;
			goto next_round;
		}
	}else{
		//No, this is the last to recv.
		REG32(CPUIIMR)|= RX_DONE_IE;	//enable intr before we recv last pkt, in case intr lost.
		rxPktCounter+=_swNic_recvLoop(-1);//just recv one pkt.
	}	
	//No more pkts in this run.
out:
	if(rxPkthdr_runout==0 &&(REG32(CPUIISR) & PKTHDR_DESC_RUNOUT_IP)){
		rxRunoutIdx=(uint32 *)REG32(CPURPDCR)-RxPkthdrRing;
		rxPkthdr_runout=1;
		rxPkthdrRunoutNum++;
	}
	if(rxPkthdr_runout==1){
		if(txEnable)
			swNic_isrTxRecycle(1);
		if(rxRunoutIdx<0)
			rxPkthdr_runout=0;
		REG32(CPUIIMR)|=PKTHDR_DESC_RUNOUT_IE;
	}
	spin_unlock_irqrestore(rtl865xSpinlock,s);
}
	


__IRAM int32 swNic_isrReclaim(uint32 rxDescIdx, struct rtl_pktHdr*pPkthdr,struct rtl_mBuf *pMbuf){
	int32 retval=FAILED;

	pPkthdr->ph_len = pMbuf->m_len =0;	//reset pkt length.
	pPkthdr->ph_srcExtPortNum=pPkthdr->ph_extPortList=pPkthdr->ph_portlist=0;
	pPkthdr->ph_flags|=PKTHDR_DRIVERHOLD;
	if(rxDescIdx<totalRxPkthdr){

		assert(!(RxPkthdrRing[rxDescIdx] & DESC_SWCORE_OWNED));
		assert(!(RxMbufRing[rxDescIdx] & DESC_SWCORE_OWNED));
		
#if defined(CONFIG_RTL865X_MBUF_HEADROOM)&&defined(CONFIG_RTL865X_MULTILAYER_BSP)
//		if(mBuf_reserve(pMbuf, CONFIG_RTL865X_MBUF_HEADROOM)){
//			rtlglue_printf("%s:mBuf_reserve(%08x)\n",__FUNCTION__,(uint32)pMbuf);	
//		}
		pMbuf->m_data=pMbuf->m_extbuf +CONFIG_RTL865X_MBUF_HEADROOM;
#else
		pPkthdr->ph_mbuf->m_data = pPkthdr->ph_mbuf->m_extbuf;	//recover buffer pointer
#endif
		/* Reclaim packet header and mbuf descriptors. reset OWN bit */
		RxMbufRing[rxDescIdx] |= DESC_SWCORE_OWNED;
		wmb();
		RxPkthdrRing[rxDescIdx] |= DESC_SWCORE_OWNED;
		wmb();
		lastReclaim=rxDescIdx;
		retval=SUCCESS;
	}else{
		//if (rxDescIdx==totalRxPkthdr)
		//cfliu: All pkthdr's index is by default "totalRxPkthdr", not -1 anymore
		//pPkthdr->ph_rxdesc = -1;
		pPkthdr->ph_rxdesc = totalRxPkthdr;
	}
	
	return retval;
	//rtlglue_printf("%d ",rxDescIdx);
}


__IRAM static void swNic_isrTxRecycle(int32 flag)
{
	struct rtl_pktHdr *     pPkthdr;
	int32 rxDescIdx;
	struct rtl_mBuf *pMbuf;
	/* Continuously check OWN bit of descriptors */
	while ( ((TxPkthdrRing[txDoneIndex] & DESC_OWNED_BIT) == DESC_RISC_OWNED)&&txDoneIndex!=txFreeIndex){
		/* Fetch pkthdr */
		pPkthdr = (struct rtl_pktHdr *) (TxPkthdrRing[txDoneIndex] & ~(DESC_OWNED_BIT | DESC_WRAP));
		pMbuf = pPkthdr->ph_mbuf;
		rxDescIdx = pPkthdr->ph_rxdesc;

		if((rxDescIdx<0)||(rxDescIdx>=totalRxPkthdr)){
			int32 retval;
			pPkthdr->ph_rxdesc=-1;
			//was sent directly from protocol stack.
		       retval= mBuf_driverFreeMbufChain(pPkthdr->ph_mbuf);
		}else {
			swNic_isrReclaim(rxDescIdx, pPkthdr,pMbuf);
		}		
		TxPkthdrRing[txDoneIndex]&=DESC_WRAP; //this clears Tx desc but keeps WRAP bit if any.
		wmb();		
		txPktCounter++;

		/* Increment index */
		if ( ++txDoneIndex == totalTxPkthdr )
			txDoneIndex = 0;
	}
	if(rxRunoutIdx>=0&&(RxPkthdrRing[rxRunoutIdx] & DESC_OWNED_BIT) != DESC_RISC_OWNED){
		rxPkthdrRunoutSolved++;
		rxRunoutIdx=-1;
	}
}

#define TxPkthdrDescFull	(((txFreeIndex+1)&((1<<totalTxPkthdrShift)-1))==txDoneIndex)

__IRAM int32 swNic_write(void * output)
{
    	struct rtl_pktHdr * pPkt = (struct rtl_pktHdr *) output;
	struct rtl_mBuf	*pMbuf=pPkt->ph_mbuf;
	uint32 len=pPkt->ph_len;
	int32 rxDescIdx;
	int32 s;
#ifdef NIC_TX_ALIGN
	uint16 pLen;
#endif /* NIC_TX_ALIGN */

#ifdef CONFIG_RTL865X_ROMEPERF
	rtl8651_romeperfEnterPoint(ROMEPERF_INDEX_12);
#endif
	spin_lock_irqsave(rtl865xSpinlock,s);
	if(!txEnable){ //Tx is not enabled now
		spin_unlock_irqrestore(rtl865xSpinlock,s);
		return FAILED;
	}
#ifdef CONFIG_RTL865X_ROMEPERF
	rtl8651_romeperfExitPoint(ROMEPERF_INDEX_12);
#endif
#ifdef CONFIG_RTL865X_ROMEPERF
	rtl8651_romeperfEnterPoint(ROMEPERF_INDEX_13);
#endif
#ifdef CONFIG_RTL865X_MULTILAYER_BSP
	rtl8651_txPktPostProcessing(pPkt);
#endif
#ifdef CONFIG_RTL865X_ROMEPERF
	rtl8651_romeperfExitPoint(ROMEPERF_INDEX_13);
#endif
#ifdef CONFIG_RTL865X_ROMEPERF
	rtl8651_romeperfEnterPoint(ROMEPERF_INDEX_14);
#endif
	if(len <60 && mBuf_padding(pMbuf, 60-len, MBUF_DONTWAIT)==NULL)
	{		
		spin_unlock_irqrestore(rtl865xSpinlock,s);
		return FAILED;
	}
   	assert(pPkt->ph_len>=60);

#ifdef CONFIG_RTL865X_ROMEPERF
	rtl8651_romeperfExitPoint(ROMEPERF_INDEX_14);
#endif
#ifdef CONFIG_RTL865X_ROMEPERF
	rtl8651_romeperfEnterPoint(ROMEPERF_INDEX_15);
#endif
#ifdef NIC_TX_ALIGN
	pLen = pPkt->ph_len + 4;	/* CRC is included */
	if (((pLen&0x1c) == 0) && ((pLen&0x03) != 0))
	{
		assert(pPkt->ph_len == pMbuf->m_len);
		pPkt->ph_len = (pPkt->ph_len&0xFFFC) + 0x4;	/* let it become 4-byte alignment */
		pMbuf->m_len = pPkt->ph_len;
	}
#endif /* NIC_TX_ALIGN */

	//rtlglue_printf("len[%d] rx[%d] plist[%x] sepn[%d] espl[%x]\n", pPkt->ph_len, pPkt->ph_rxdesc, pPkt->ph_portlist, pPkt->ph_srcExtPortNum, pPkt->ph_extPortList);

	pPkt->ph_nextHdr = (struct rtl_pktHdr *) NULL;
	rxDescIdx=pPkt->ph_rxdesc;

	if(TxPkthdrDescFull){
#ifdef CONFIG_RTL865X_ROMEPERF
	rtl8651_romeperfEnterPoint(ROMEPERF_INDEX_16);
#endif
		swNic_isrTxRecycle(0);//force reclaim Tx descriptors if any.
#ifdef CONFIG_RTL865X_ROMEPERF
	rtl8651_romeperfExitPoint(ROMEPERF_INDEX_16);
#endif
		if(TxPkthdrDescFull){//check again, if still full, drop the pkt.
//			if((rxDescIdx>=0)&&(rxDescIdx<totalRxPkthdr))
//				swNic_isrReclaim(rxDescIdx,  pPkt, pMbuf);//pkt was rcvd from network. free it..
			spin_unlock_irqrestore(rtl865xSpinlock,s);
			return FAILED;
		}
	}

#ifdef CONFIG_RTL865X_ROMEREAL
	rtl8651_romerealRecord( pPkt, 0x00000002 );
#endif/*CONFIG_RTL865X_ROMEREAL*/

	//yes, we have a Tx desc fro sending this pkt.
	//mark desc as swcore own to send the packet...
	//printfByPolling("T%d ,",txFreeIndex);	
      TxPkthdrRing[txFreeIndex]|=((uint32)pPkt|DESC_OWNED_BIT);

	wmb();
	txFreeIndex++;
	txFreeIndex&=(1<<totalTxPkthdrShift)-1;
	spin_unlock_irqrestore(rtl865xSpinlock,s);

	//memDump(pMbuf->m_len, 64, "sending...");
	//rtlglue_printf("LEN[%d] rx[%d] plist[%x] sepn[%d] espl[%x]\n", pPkt->ph_len, pPkt->ph_rxdesc, pPkt->ph_portlist, pPkt->ph_srcExtPortNum, pPkt->ph_extPortList);
	REG32(CPUICR) |= TXFD;
	
#ifdef CONFIG_RTL865X_ROMEPERF
	rtl8651_romeperfExitPoint(ROMEPERF_INDEX_15);
#endif
	return SUCCESS;
}

int32 swNic_txRxSwitch(uint32 tx, uint32 rx){
	uint32 icr = REG32(CPUICR), imr = REG32(CPUIIMR);
	if(tx==0){
		icr &= ~TXCMD;
		imr &= ~TX_ERR_IE;
	}else{
		icr |= TXCMD;
		imr |= TX_ERR_IE;
	}
	if(rx==0){
		icr&=~RXCMD;
		imr &= ~(RX_ERR_IE | PKTHDR_DESC_RUNOUT_IE |  RX_DONE_IE);
	}else{
		icr |= RXCMD;
		imr |=(RX_ERR_IE | PKTHDR_DESC_RUNOUT_IE |  RX_DONE_IE);
	}
	rxEnable=rx;
	txEnable=tx;
	REG32(CPUICR) = icr;
	REG32(CPUIIMR) = imr;
	return SUCCESS;
}


//#pragma ghs section text=default

int32 swNic_setup(uint32 pkthdrs, uint32 mbufs, uint32 txpkthdrs)
{
	struct rtl_pktHdr *freePkthdrListHead,*freePkthdrListTail;
	struct rtl_mBuf *freeMbufListHead, *freeMbufListTail;
	int i;
	 /* Disable Rx & Tx ,bus burst size, etc */
	swNic_txRxSwitch(0,0);

#ifdef SWNIC_EARLYSTOP
	nicRxEarlyStop=0;
#endif
	/* Initialize index of Tx pkthdr descriptor */
	txDoneIndex = 0;
	txFreeIndex = 0;

	/* Allocate rx pkthdrs */
	if(pkthdrs!=mBuf_driverGetPkthdr(pkthdrs, &freePkthdrListHead, &freePkthdrListTail)){
		rtlglue_printf("Can't allocate all pkthdrs\n");
		return EINVAL;
	}
	assert(freePkthdrListHead);
	assert(freePkthdrListTail);
	/* Allocate rx mbufs and clusters */
	if(mbufs!=mBuf_driverGet(mbufs, &freeMbufListHead, &freeMbufListTail)){
		rtlglue_printf("Can't allocate all mbuf/clusters\n");
		return EINVAL;
	}
	assert(freeMbufListHead);
	assert(freeMbufListTail);
    /////////////////////////////////////////////////	
    /* Initialize Tx packet header descriptors */
    for (i=0; i<txpkthdrs; i++)
        TxPkthdrRing[i] = DESC_RISC_OWNED;

    /* Set wrap bit of the last descriptor */
    TxPkthdrRing[txpkthdrs - 1] |= DESC_WRAP;

    /* Fill Tx packet header FDP */
    REG32(CPUTPDCR) = (uint32) TxPkthdrRing;
	
    /////////////////////////////////////////////////	
    /* Initialize index of current Rx pkthdr descriptor */
    rxPhdrIndex = 0;
    /* Initialize Rx packet header descriptors */
    for (i=0; i<pkthdrs; i++)
    {
        assert( freePkthdrListHead );
        RxPkthdrRing[i] = (uint32) freePkthdrListHead | DESC_SWCORE_OWNED;
        if ( (freePkthdrListHead = freePkthdrListHead->ph_nextHdr) == NULL )
            freePkthdrListTail = NULL;
    }
    /* Set wrap bit of the last descriptor */
    RxPkthdrRing[pkthdrs - 1] |= DESC_WRAP;
    /* Fill Rx packet header FDP */
    REG32(CPURPDCR) = (uint32) RxPkthdrRing;

    /////////////////////////////////////////////////	
    /* Initialize index of current Rx pkthdr descriptor */
    rxMbufIndex = 0;
    /* Initialize Rx mbuf descriptors */
    for (i=0; i<mbufs; i++)
    {
        assert( freeMbufListHead );
        RxMbufRing[i] = (uint32) freeMbufListHead | DESC_SWCORE_OWNED;
#ifndef CONFIG_RTL865X_CACHED_NETWORK_IO
		freeMbufListHead->m_extbuf=(uint8 *)UNCACHE(freeMbufListHead->m_extbuf);
		freeMbufListHead->m_data=(uint8 *)UNCACHE(freeMbufListHead->m_data);
#endif

#if defined(CONFIG_RTL865X_MBUF_HEADROOM)&&defined(CONFIG_RTL865X_MULTILAYER_BSP)
		if(mBuf_reserve(freeMbufListHead, CONFIG_RTL865X_MBUF_HEADROOM))
			rtlglue_printf("Failed when init Rx %d\n", i);
#endif
        if ( (freeMbufListHead = freeMbufListHead->m_next) == NULL )
            freeMbufListTail = NULL;
    }
    /* Set wrap bit of the last descriptor */
    RxMbufRing[mbufs - 1] |= DESC_WRAP;

    /* Fill Rx mbuf FDP */
	REG32(CPURMDCR) = (uint32) RxMbufRing;
	REG32(CPUICR) =0;

#ifdef CONFIG_RTL865XB
	{
		char chipVersion[16];
		uint32 align=0;

		REG32(CPUICR)|=EXCLUDE_CRC;
		GetChipVersion(chipVersion, sizeof(chipVersion), NULL);
		if(chipVersion[strlen(chipVersion)-1]=='B')
		{
			//865xB chips support free Rx align from 0~256 bytes
			#ifdef SWNIC_RX_ALIGNED_IPHDR					
			align+=2;
			#endif
			REG32(CPUICR)|=align; 
			rtlglue_printf("Rx shift=%x\n",REG32(CPUICR));
		}
	}
#endif

    /* Enable Rx & Tx. Config bus burst size and mbuf size. */
    REG32(CPUICR) |= BUSBURST_32WORDS | MBUF_2048BYTES;
    REG32(CPUIIMR) |= LINK_CHANG_IE;
	swNic_txRxSwitch(1,1);
    return SUCCESS;

}

int32 swNic_descRingCleanup(void){
	int i;
	swNic_txRxSwitch(0,0);
    /* cleanup Tx packet header descriptors */
    for (i=0; i<totalTxPkthdr; i++){
		struct rtl_pktHdr *ph;
		TxPkthdrRing[i]&= ~DESC_SWCORE_OWNED; //mark it own by CPU
		ph = (struct rtl_pktHdr *) (TxPkthdrRing[i] & ~ (0x3));
		if(ph){
			//has somthing send or sent...
			struct rtl_mBuf *m;
			uint32 rxDescIdx;
			rxDescIdx = ph->ph_rxdesc;
			ph->ph_rxdesc = totalRxPkthdr; 
			m=ph->ph_mbuf;
			assert(m);
			if((rxDescIdx<0 || rxDescIdx>=totalRxPkthdr)){
				//came from protocol stack or fwd engine
				SETBITS(m->m_flags, MBUF_USED);
				mBuf_freeMbufChain(m);
			}
		}
		TxPkthdrRing[i] &= 0x3;//clean desc on Rx Pkthdr ring
  }	

    /* cleanup Rx packet header descriptors */
    for (i=0; i<totalRxPkthdr; i++){
		struct rtl_pktHdr *ph;
		struct rtl_mBuf *m;
		RxPkthdrRing[i]&= ~DESC_SWCORE_OWNED; //mark it own by CPU
		RxMbufRing[i]&= ~DESC_SWCORE_OWNED;
		ph = (struct rtl_pktHdr *) (RxPkthdrRing[i] & ~ (0x3));
		m = (struct rtl_mBuf *)(RxMbufRing[i] & ~ (0x3));
		ph->ph_mbuf=m;
		m->m_pkthdr=ph;
		RxPkthdrRing[i] &= 0x3;//clean desc on Rx Pkthdr ring
		RxMbufRing[i]&=0x3;
		assert(ph);
		ph->ph_rxdesc = -1; 
		m->m_next=NULL;
		SETBITS(ph->ph_flags, PKTHDR_USED);
		SETBITS(m->m_flags, MBUF_USED|MBUF_PKTHDR);
		mBuf_freeMbufChain(ph->ph_mbuf);
    }
    	
    /////////////////////////////////////////////////	
    /* Initialize index of current Rx pkthdr descriptor */
    rxMbufIndex = rxPhdrIndex = txDoneIndex = txFreeIndex = 0;
	REG32(CPURMDCR) = 0;
	REG32(CPURPDCR) = 0;
	REG32(CPUTPDCR) = 0;	
	return SUCCESS;
}

int32 swNic_init(uint32 nRxPkthdrDesc, uint32 nRxMbufDesc, uint32 nTxPkthdrDesc, 
                        uint32 clusterSize, 
                        proc_input_pkt_funcptr_t processInputPacket, 
                        int32 reserved(struct rtl_pktHdr *))
{
	uint32  exponent;

	//check input parameters.
	if(isPowerOf2(nRxPkthdrDesc, &exponent)==FALSE)
		return EINVAL;
	if(isPowerOf2(nRxMbufDesc, &exponent)==FALSE)
		return EINVAL;
	if(isPowerOf2(nTxPkthdrDesc, &totalTxPkthdrShift)==FALSE)
		return EINVAL;
    	if (processInputPacket == NULL)
		return EINVAL;

	//we only accept 2048 byte cluster now.
	if(clusterSize!=m_clusterSize) //mbuf should be init first and cluster size must equal to the number used in mbuf init
		return EINVAL;
	assert(m_clusterSize==2048); //mBuf should be init with 2048 as cluster value
	totalRxPkthdr = nRxPkthdrDesc;
	totalRxMbuf = nRxMbufDesc;
	totalTxPkthdr = nTxPkthdrDesc;
	rxRunoutIdx=-1;
	installedProcessInputPacket = processInputPacket;
	rxPkthdr_runout=0;

	/* Allocate Rx/Tx descriptor ring.*/
	RxPkthdrRing=(uint32 *)(UNCACHE_MASK|(uint32)kmalloc((totalRxPkthdr * sizeof(struct rtl_pktHdr *)),GFP_KERNEL));
	RxMbufRing = (uint32 *)(UNCACHE_MASK|(uint32)kmalloc((totalRxMbuf * sizeof(struct rtl_mBuf *)),GFP_KERNEL));
	TxPkthdrRing = (uint32 *)(UNCACHE_MASK|(uint32)kmalloc((totalTxPkthdr * sizeof(struct rtl_pktHdr *)),GFP_KERNEL));

	if(!RxPkthdrRing||!RxMbufRing||!TxPkthdrRing)
		return EINVAL;
	mBuf_setNICRxRingSize(totalRxPkthdr);
	/* Initialize interrupt statistics counter */
	rxIntNum = rxPkthdrRunoutSolved= txIntNum = rxPkthdrRunoutNum = rxMbufRunoutNum = rxPktErrorNum = txPktErrorNum = 0;
	rxPktCounter = txPktCounter = linkChanged=0;

	//All Ring buffers allocated. Now initialize all rings and swNic itself.
	return  swNic_setup(totalRxPkthdr,totalRxMbuf,totalTxPkthdr);
}

/*************************************************************************
*   FUNCTION                                                              
*       swNic_installedProcessInputPacket                                         
*                                                                         
*   DESCRIPTION                                                           
*       This function installs given function as the handler of received  
*       packets.
*                                                                         
*   INPUTS                                                                
*       nRxPkthdrDesc   Number of Rx pkthdr descriptors.
*       nRxMbufDesc     Number of Tx mbuf descriptors.
*       nTxPkthdrDesc   Number of Tx pkthdr descriptors.
*       clusterSize     Size of cluster.
*                                                                         
*   OUTPUTS                                                               
*       Status.
*************************************************************************/
proc_input_pkt_funcptr_t swNic_installedProcessInputPacket(
                            proc_input_pkt_funcptr_t processInputPacket)
{
    proc_input_pkt_funcptr_t ret = installedProcessInputPacket;
    installedProcessInputPacket = processInputPacket;
    
    return ret;
}



#ifdef CONFIG_RTL865X_CLE
static int32 swNic_getCounters(swNicCounter_t * counter) {
    counter->rxIntNum = rxIntNum;
    counter->txIntNum = txIntNum;
    counter->rxPkthdrRunoutNum = rxPkthdrRunoutNum;
	counter->rxPkthdrRunoutSolved=rxPkthdrRunoutSolved;
    counter->rxMbufRunoutNum = rxMbufRunoutNum;
    counter->rxPktErrorNum = rxPktErrorNum;
    counter->txPktErrorNum = txPktErrorNum;
    counter->rxPktCounter = rxPktCounter;
    counter->txPktCounter = txPktCounter;
    counter->currRxPkthdrDescIndex = rxPhdrIndex;
    counter->currRxMbufDescIndex = rxMbufIndex;
    counter->currTxPkthdrDescIndex = txDoneIndex;
    counter->freeTxPkthdrDescIndex = txFreeIndex;
    counter->rxIntNum = rxIntNum;
    counter->txIntNum = txIntNum;
	counter->linkChanged = linkChanged;
    return SUCCESS;
}
static int32 swNic_resetCounters(void) {
	rxIntNum= txIntNum=rxPkthdrRunoutNum=rxPkthdrRunoutSolved=rxMbufRunoutNum=0;
	rxPktErrorNum= txPktErrorNum = rxPktCounter= txPktCounter= rxPhdrIndex=0;
	rxMbufIndex=txDoneIndex=txFreeIndex=rxIntNum=txIntNum=0;
    return SUCCESS;
}

static void swNic_dumpPkthdrDescRing(void){
	uint32  i, *temp=(uint32 *)rtlglue_malloc(sizeof(uint32)*totalRxPkthdr);
	uint32 value,asicIdx = ((uint32 *)REG32(CPURPDCR))-RxPkthdrRing;
	for(i=0;i<totalRxPkthdr;i++)
		temp[i]=RxPkthdrRing[i];
	rtlglue_printf("Rx phdr ring starts at 0x%x\n",(uint32)RxPkthdrRing );
	for(i=0;i<totalRxPkthdr;i++){
		struct rtl_pktHdr *ph;
		struct rtl_mBuf *m;
		value=temp[i];
		ph=(struct rtl_pktHdr *)(value &~0x3);
		rtlglue_printf("%03d.",(uint16)i);
		if(ph){
			rtlglue_printf("p:%08x ",value &~0x3);
			m=ph->ph_mbuf;
			if(m)
				rtlglue_printf("m:%08x c:%08x d:%08x", (uint32)m, (uint32)m->m_extbuf, (uint32)m->m_data);
			else
				rtlglue_printf("No mbuf!! ");
		}else
			rtlglue_printf("No pkthdr!! ");
		rtlglue_printf("%s ", ((value & 1)== 0)?"(CPU)":"(SWC)");
		if(asicIdx==i)
			rtlglue_printf("ASIC ");
		if(rxPhdrIndex==i)
			rtlglue_printf("Rx ");
		if(lastReclaim==i)
			rtlglue_printf("Reclaim ");
		
		if((value & 2)== 2){
			rtlglue_printf("WRAP!!\n");
			return;
		}else
			rtlglue_printf("\n");
	}
	rtlglue_free(temp);
}


void swNic_dumpTxDescRing(void){	
	int32 i;
	uint32 value, asicIdx = ((uint32 *)REG32(CPUTPDCR))-TxPkthdrRing;
	rtlglue_printf("Tx phdr ring starts at 0x%x\n",(uint32)TxPkthdrRing );
	for(i=0; i<totalTxPkthdr; i++){
		volatile struct rtl_pktHdr *ph;
		volatile struct rtl_mBuf *m;
		value=TxPkthdrRing[i];
		ph=(struct rtl_pktHdr *)(value &~0x3);
		if(ph){
			rtlglue_printf("%3d. p%08x ", i,(uint32)ph);
			m=ph->ph_mbuf;
			if(m)
				rtlglue_printf("m:%08x d:%08x",(uint32)m,(uint32)m->m_data);
			else
				rtlglue_printf("m=NULL");
		}else
			rtlglue_printf("%3d. p:NULL", i);
		rtlglue_printf("%s ", ((value & 1)== 0)?"(CPU)":"(SWC)");
		if(ph){
			rtlglue_printf("id:%d ", ph->ph_rxdesc);
			rtlglue_printf("s%d:e%02x:p%03x", ph->ph_srcExtPortNum, ph->ph_extPortList, ph->ph_portlist);
		}
		if(asicIdx==i)
			rtlglue_printf("ASIC ");
		if(txDoneIndex==i)
			rtlglue_printf("Done ");
		if(txFreeIndex==i)
			rtlglue_printf("Next ");
		if((value & 2)== 2){
			rtlglue_printf("WRAP!!\n");
			return;
		}else
			rtlglue_printf("\n");
	}
	return;

}

static int32 loopbackPort1,loopbackPort2;
static void re865x_testL2loopback(struct rtl_pktHdr * pPkt)
{
	if(pPkt->ph_portlist ==loopbackPort1)
		pPkt->ph_portlist = 1<<loopbackPort2;
	else
		pPkt->ph_portlist = 1<<loopbackPort1;
	swNic_write((void *) pPkt);
}


static proc_input_pkt_funcptr_t old_funcptr=NULL;
static int32	swNic_loopbackCmd(uint32 userId,  int32 argc,int8 **saved){
	int8 *nextToken;
	int32 size;
	cle_getNextCmdToken(&nextToken,&size,saved); 
	if(strcmp(nextToken, "enable") == 0){
		REG32(MSCR)&=~0x7;//disable L2~L4
		cle_getNextCmdToken(&nextToken,&size,saved); 
		loopbackPort1=U32_value(nextToken);
		cle_getNextCmdToken(&nextToken,&size,saved); 
		loopbackPort2=U32_value(nextToken);
	     old_funcptr = swNic_installedProcessInputPacket((proc_input_pkt_funcptr_t)re865x_testL2loopback);
		rtlglue_printf("Turn off ASIC L2/3/4 functions. Enter NIC loopback mode. Bridge pkts between port %d and %d\n", loopbackPort1, loopbackPort2);
	}else if(old_funcptr){
		rtlglue_printf("Turn on ASIC L2/3/4 functions. Exit NIC loopback mode\n");
	       swNic_installedProcessInputPacket(old_funcptr);
		REG32(MSCR)|=0x7;//enable L2~L4
	}else 
		return FAILED;
	return SUCCESS;
}

static uint32 hubMbrmask;

static void swNic_hubMode(struct rtl_pktHdr * pPkt)
{
	uint16 bcastPorts = 0x1f & ~(1<<pPkt->ph_portlist);
	pPkt->ph_portlist = bcastPorts;
	swNic_write((void *) pPkt);
}


static int32	swNic_hubCmd(uint32 userId,  int32 argc,int8 **saved){
	int8 *nextToken;
	int32 size;
	cle_getNextCmdToken(&nextToken,&size,saved); 
	if(strcmp(nextToken, "enable") == 0){
		hubMbrmask=0;
		REG32(MSCR)&=~0x7;//disable L2~L4
		while(cle_getNextCmdToken(&nextToken,&size,saved) !=FAILED){
			uint32 thisPort;
			thisPort=U32_value(nextToken);
			if(thisPort<5)
				hubMbrmask|=(1<<thisPort);
		}
	     old_funcptr = swNic_installedProcessInputPacket((proc_input_pkt_funcptr_t)swNic_hubMode);
		rtlglue_printf("Turn off ASIC L2/3/4 functions. Enter Hub mode. Portmask:%08x\n", hubMbrmask);
	}else if(old_funcptr){
		rtlglue_printf("Turn on ASIC L2/3/4 functions. Exit Hub mode\n");
	       swNic_installedProcessInputPacket(old_funcptr);
		REG32(MSCR)|=0x7;//enable L2~L4
	}else 
		return FAILED;
	return SUCCESS;
}


static int32	swNic_counterCmd(uint32 userId,  int32 argc,int8 **saved){
	int8 *nextToken;
	int32 size;

	cle_getNextCmdToken(&nextToken,&size,saved); 
	if(strcmp(nextToken, "dump") == 0){
		swNicCounter_t counter;
		rtlglue_printf("Switch NIC statistics:\n\n");
		swNic_getCounters(&counter);
		rtlglue_printf("rxIntNum: %u, txIntNum: %u, pktRunout: %u, , rxErr: %u, txErr: %u\n",
			counter.rxIntNum, counter.txIntNum, counter.rxPkthdrRunoutNum,  counter.rxPktErrorNum, counter.txPktErrorNum);
		rtlglue_printf("Interrupt register 0x%08x interrupt mask 0x%08x\n", REG32(CPUIISR), REG32(CPUIIMR));
		rtlglue_printf("Rx Packet Counter: %d Tx Packet Counter: %d\n", counter.rxPktCounter, counter.txPktCounter);
		rtlglue_printf("Run Out: %d Rx solved: %d linkChanged: %d\n", counter.rxPkthdrRunoutNum, counter.rxPkthdrRunoutSolved, counter.linkChanged);
		rtlglue_printf("currRxPkthdrDescIndex: %d currRxMbufDescIndex: %d\n",counter.currRxPkthdrDescIndex,counter.currRxMbufDescIndex);
		rtlglue_printf("currTxPkthdrDescIndex: %d freeTxPkthdrDescIndex: %d\n",counter.currTxPkthdrDescIndex, counter.freeTxPkthdrDescIndex);
		rtlglue_printf("ASIC dropped %d.\n", rtl8651_returnAsicCounter(0x4));

	}else{
		rtlglue_printf("Reset all NIC internal counters\n");
		swNic_resetCounters();

	}
	return SUCCESS;
}

static int32	swNic_cmd(uint32 userId,  int32 argc,int8 **saved){
	int8 *nextToken;
	int32 size;

	cle_getNextCmdToken(&nextToken,&size,saved); 
	if(strcmp(nextToken, "burst") == 0){
		cle_getNextCmdToken(&nextToken,&size,saved); 
#ifdef SWNIC_EARLYSTOP
		nicRxEarlyStop = U32_value( nextToken );
#endif
	}else if(strcmp(nextToken, "rx") == 0){
		cle_getNextCmdToken(&nextToken,&size,saved); 
		if(strcmp(nextToken, "suspend") == 0)
			swNic_txRxSwitch(txEnable, 0);
		else
			swNic_txRxSwitch(txEnable, 1);
	}else if(strcmp(nextToken, "tx") == 0){
		cle_getNextCmdToken(&nextToken,&size,saved); 	
		if(strcmp(nextToken, "suspend") == 0)
			swNic_txRxSwitch(0, rxEnable);
		else
			swNic_txRxSwitch(1, rxEnable);
	}
	return SUCCESS;
}


static int32	swNic_ringCmd(uint32 userId,  int32 argc,int8 **saved){
	int8 *nextToken;
	int32 size;
	cle_getNextCmdToken(&nextToken,&size,saved); 
	if(strcmp(nextToken, "mbufRxRing") == 0) {
		uint32  i, value, asicIdx = ((uint32 *)REG32(CPURMDCR))-RxMbufRing;
		rtlglue_printf("Rx mbuf ring starts at 0x%x\n",(uint32)RxMbufRing );
		for(i=0;i<totalRxMbuf;i++){
			value=RxMbufRing[i];
			rtlglue_printf("%3d. 0x%08x ",i, value&~0x3 );
			rtlglue_printf("%s ", ((value & 1)== 0)?"(CPU)":"(SWC)");
			if(asicIdx==i)
				rtlglue_printf("ASIC ");
			if(rxMbufIndex==i)
				rtlglue_printf("Rx ");
			if((value & 2)== 2){
				rtlglue_printf("WRAP!!\n");
				return SUCCESS;
			}else
				rtlglue_printf("\n");
		}
	}else if(strcmp(nextToken, "pkthdrRxRing") == 0) {
		swNic_dumpPkthdrDescRing();
		return SUCCESS;
	}else if(strcmp(nextToken, "txRing") == 0) {	
		swNic_dumpTxDescRing();
		return SUCCESS;
	}else{
		struct rtl_mBufStatus mbs;
		rtlglue_printf("Reset all NIC rings...\n");
		swNic_descRingCleanup();
		rtlglue_printf("  Init all NIC rings...\n");
		swNic_setup(totalRxPkthdr, totalRxMbuf, totalTxPkthdr);
		rtlglue_printf("  mbuf pool...\n");
		if(mBuf_getBufStat(&mbs)==SUCCESS){
			rtlglue_printf( "\tSizeof\tTotal\tFree\n");
			rtlglue_printf( "Cluster\t%d\t%d\t%d\n",mbs.m_mclbytes, mbs.m_totalclusters, mbs.m_freeclusters);
			rtlglue_printf( "Mbuf\t%d\t%d\t%d\n",mbs.m_msize, mbs.m_totalmbufs, mbs.m_freembufs);
			rtlglue_printf( "Pkthdr\t%d\t%d\t%d\n",mbs.m_pkthdrsize, mbs.m_totalpkthdrs, mbs.m_freepkthdrs);
		}
	
	}
	return SUCCESS;
}


cle_exec_t swNic_cmds[] = {
	{	"counter",
		"Dump NIC internal counters",
		"{ dump | clear }",
		swNic_counterCmd,
		CLE_USECISCOCMDPARSER,	
		0,
		0
	},
	{
		"loopback",		//cmdStr
		"Layer2 loopback.",	//cmdDesc
		" { enable %d'loopback port 1' %d'loopback port 2' | disable }",  //usage
		swNic_loopbackCmd,		//execution function
		CLE_USECISCOCMDPARSER,	
		0,
		NULL
	},	
	{
		"hub",		//cmdStr
		"Hub mode",	//cmdDesc
		" { enable (1~5)%d'Hub member port' | disable }",  //usage
		swNic_hubCmd,		//execution function
		CLE_USECISCOCMDPARSER,	
		0,
		NULL
	},
	{	"ring",
		"NIC internal ring cmd",
		"{ txRing'dump Tx ring' | pkthdrRxRing'dump Rx ring' | mbufRxRing'dump Rx mbuf ring' | reset'Reset all rescriptor rings and NIC.' }",
		swNic_ringCmd,
		CLE_USECISCOCMDPARSER,	
		0,
		0
	},
	{	"cmd",
		"NIC internal cmds",
		"{ burst  %d'#( 2^n-1) of continuous pkt processed in ISR, 0 for infinite' | { { tx | rx } { suspend | resume } } }",
		swNic_cmd,
		CLE_USECISCOCMDPARSER,	
		0,
		0
	},
};
#endif



