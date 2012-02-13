/*
* ----------------------------------------------------------------
* Copyright c                  Realtek Semiconductor Corporation, 2002  
* All rights reserved.
* 
* $Header: /home/cvsroot/uClinux-dist/linux-2.4.x/drivers/net/re865x/swNic2.h,v 1.18 2005/01/09 08:39:51 cfliu Exp $
*
* Abstract: Switch core NIC header file.
*
* $Author: cfliu $
*
* $Log: swNic2.h,v $
* Revision 1.18  2005/01/09 08:39:51  cfliu
* +:Add link change Intr statistics counter.
*
* Revision 1.17  2004/11/19 16:30:04  jzchen
* *: Enable shift 2
*
* Revision 1.16  2004/11/19 09:58:24  cfliu
* *: Add NIC cmd group, removed unused symbols
*
* Revision 1.15  2004/11/18 11:45:36  chenyl
* *: default "DONT Shift 2 byte for Alginment" !!!
* *: add more choice when add new port bouncing entry
* 	priority:
* 		1. Empty entry
* 		2. Closed TCP entry
* 		3. UDP entry with minimum age
* 		4. ICMP entry with minimum age
* 		5. Other non-closed TCP entry with minimum age
*
* Revision 1.14  2004/08/05 06:39:11  chenyl
* +: fragment shortest path forwarding
* *: UDPBomb don't aware ignore Type Checking result
*
* Revision 1.13  2004/07/27 10:39:15  cfliu
* *: Fix all compile warnings
*
* Revision 1.12  2004/07/06 14:08:49  cfliu
* +: Flush D-cache when pkt send to protocol stack. Don't copy packet if on 8650B and NIC enabled iphdr aligned feature.
*
* Revision 1.11  2004/07/04 14:53:16  cfliu
* +: Change NIC Rx align to 2 byte aligned. change recv loop prototype so NIC can be forced stop by fwd engine.
*
* Revision 1.10  2004/06/08 10:26:44  cfliu
* +:Add runout solved counter
*
* Revision 1.9  2004/05/03 14:36:51  cfliu
* no message
*
* Revision 1.8  2004/05/03 02:53:03  chenyl
* *: swNic_getRingSize -> rtlglue_getRingSize and move it from swNic2.c to rtl_glue.c
* *: set default upstream ip to 0 if we config interface to dhcp
*
* Revision 1.7  2004/04/30 08:58:08  chenyl
* +: ip multicast/igmp proxy
*
* Revision 1.6  2004/03/26 11:21:04  cfliu
* swNic_intHandler handles most Rx ISR jobs now...
*
* Revision 1.5  2004/03/23 09:31:03  cfliu
* Fix NIC overrun problemEnlarge mbuf & NIC init buffer size.
*
* Revision 1.4  2004/02/27 09:42:40  cfliu
* Move away linux dependent definition
*
* Revision 1.3  2004/02/24 04:08:23  cfliu
* Add WLAN HW acceleration
*
* Revision 1.2  2003/12/18 12:47:42  rupert
* fix new nic driver
*
* Revision 1.1  2003/12/09 15:35:31  cfliu
* new NIC driver. To enable it, use CONFIG_RTL865X_NICDRV2. Performance gain is 2.5X
*
* Revision 1.33  2003/11/26 00:22:39  cfliu
* add miiPhyAddress.
*
* Revision 1.32  2003/06/16 11:53:39  cfliu
* Fix SWNIC_PH_SET_PORT_LIST
*
* Revision 1.31  2003/06/03 10:07:27  cfliu
* no message
*
* Revision 1.30  2003/06/03 06:35:49  cfliu
* no message
*
*
* Revision 1.26  2003/06/03 02:10:49  danwu
* modify SWNIC_PH_SET_PORT_LIST definition
*
* Revision 1.25  2003/05/23 07:49:28  cfliu
* add 8651B macros
*
* Revision 1.24  2003/05/22 11:24:12  jzchen
* add id string
*
* Revision 1.23  2003/05/22 08:48:05  cfliu
* Rename mBuf, pktHdr ==> rtl_mBuf, rtl_pktHdr to avoid name conflict with VxWorks
*
* Revision 1.22  2003/05/13 06:08:49  jzchen
* Remove dump function and add counter get funciton
*
* Revision 1.21  2003/05/06 08:10:10  cfliu
* Redefine ph_proto field in pktHdr structure. Define bit fields to avoid manual shift operations.
*
* Revision 1.20  2003/04/17 11:39:52  cfliu
* no message
*
* Revision 1.19  2003/04/16 10:30:10  cfliu
* Global renaming for mbuf API and symbols
*
* Revision 1.18  2003/04/11 07:54:00  danwu
* modify prototype of swNic_init
*
* Revision 1.17  2003/03/31 10:27:36  cfliu
* add swNic_toomanyUnconfirmed()
*
* Revision 1.16  2003/03/19 06:03:15  jzchen
* Change command prototype
*
* Revision 1.15  2003/03/05 13:33:00  jzchen
* export debuging function swNic_cmdCounterDump
*
* Revision 1.14  2002/10/23 03:16:24  danwu
* Support short cut.
*
* Revision 1.13  2002/10/22 11:37:56  danwu
* no message
*
* Revision 1.12  2002/10/14 10:11:32  henryCho
* Add tx confirm queue
*
* Revision 1.11  2002/10/08 14:15:20  henryCho
* Add statistics counters
*
* Revision 1.10  2002/10/06 08:55:19  jzchen
* Add refill function for application usage
*
* Revision 1.9  2002/10/05 10:13:50  jzchen
* Re-construct driver style update
*
* Revision 1.8  2002/10/02 09:55:02  jzchen
* Update new prototype
*
* Revision 1.7  2002/09/26 05:28:07  henryCho
* no message
*
* Revision 1.6  2002/09/26 01:49:05  danwu
* Allow flexible descriptor number and cluster size.
*
* Revision 1.5  2002/09/10 07:14:41  danwu
* Debug declaration of swNic_receivePkt().
*
* Revision 1.4  2002/08/29 01:28:08  danwu
* Add declaration of swNic_receivePkt.
*
* Revision 1.3  2002/08/28 11:23:28  danwu
* Add SWNIC_PH_SET_PORT_LIST & SWNIC_PH_GET_PORT_NUMBER macros.
*
* Revision 1.2  2002/08/23 06:02:50  danwu
* Add pkthdr access macros.
*
* Revision 1.1  2002/08/20 05:58:22  danwu
* Create.
*
* ---------------------------------------------------------------
*/


#ifndef _SWNIC_H
#define _SWNIC_H
struct rtl_pktHdr;
typedef int    (*proc_input_pkt_funcptr_t) (struct rtl_pktHdr*);

/* --------------------------------------------------------------------
 * ROUTINE NAME - swNic_init
 * --------------------------------------------------------------------
 * FUNCTION: This service initializes the switch NIC.
 * INPUT   : 
        nRxPkthdrDesc: Number of Rx pkthdr descriptors.
        nRxMbufDesc: Number of Rx mbuf descriptors.
        nTxPkthdrDesc: Number of Tx pkthdr descriptors.
        clusterSize: Size of a mbuf cluster.
        processInputPacket: Received packet processing function.
        shortCut: Short cut callback function. User can install a 
            routine which could decide whether the received packet 
            should be forwarded and (if necessary) modify the packet 
            in a very short time. If this routine returns 0, the 
            packet is immediately sent out, otherwise it is handled 
            as usual. Note that this routine must be fast, never 
            allocate or release pkthdr or mbuf, and all shared data 
            must be protected if this routine touches it, because this 
            routine will be called inside the interrupt handler.
 * OUTPUT  : None.
 * RETURN  : Upon successful completion, the function returns ENOERR. 
        Otherwise, 
		EINVAL: Invalid argument.
		ENFILE: Packet header or mbuf available.
 * NOTE    : None.
 * -------------------------------------------------------------------*/
int32 swNic_init(uint32 nRxPkthdrDesc, uint32 nRxMbufDesc, uint32 nTxPkthdrDesc, 
                        uint32 clusterSize, 
                        proc_input_pkt_funcptr_t processInputPacket, 
                        int32 shortCut(struct rtl_pktHdr *));
                        
proc_input_pkt_funcptr_t swNic_installedProcessInputPacket(proc_input_pkt_funcptr_t);


/* --------------------------------------------------------------------
 * ROUTINE NAME - swNic_intHandler
 * --------------------------------------------------------------------
 * FUNCTION: This function is the NIC interrupt handler.
 * INPUT   :
		intPending: Pending interrupts.
 * OUTPUT  : None.
 * RETURN  : None.
 * NOTE    : None.
 * -------------------------------------------------------------------*/

int32 swNic_intHandler(int32 *lastRxDescIdx);

#define UNCACHE_MASK		0x20000000
#define UNCACHE(x)		((u32)x|UNCACHE_MASK)
#define CACHED(x)		((u32)x & ~UNCACHE_MASK)
#define SWNIC_RX_ALIGNED_IPHDR

typedef struct swNicCounter_s {
    int32 rxIntNum, txIntNum, rxPkthdrRunoutNum, rxMbufRunoutNum, rxPkthdrRunoutSolved, rxPktErrorNum, txPktErrorNum, rxPktCounter, txPktCounter;
    int32 currRxPkthdrDescIndex, currRxMbufDescIndex, currTxPkthdrDescIndex, freeTxPkthdrDescIndex;
	int32 linkChanged;
} swNicCounter_t;
void swNic_rxThread(unsigned long param);
 int32 swNic_write(void * output);
extern int8 swNic_Id[];
extern int32 miiPhyAddress;
extern spinlock_t *rtl865xSpinlock;
extern uint32	totalRxPkthdr, totalRxMbuf,totalTxPkthdr, totalTxPkthdrShift;  /*total desc count on each ring */
extern uint32 	rxMbufIndex, rxPhdrIndex, txDoneIndex, txFreeIndex; /* current desc index */
extern uint32 	*RxPkthdrRing,*RxMbufRing, *TxPkthdrRing; /* descriptor ring */

#ifdef CONFIG_RTL865X_CLE
#include "cle/rtl_cle.h" 
extern cle_exec_t swNic_cmds[];
#define CMD_RTL8651_SWNIC_CMD_NUM		5

#endif
#endif /* _SWNIC_H */
