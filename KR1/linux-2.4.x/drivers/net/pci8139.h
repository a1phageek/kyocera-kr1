#ifndef PCI8139_H
#define PCI8139_H

#define MAC0   0		/* Ethernet hardware address. */
#define MAR0   8 		/* Multicast filter. */
#ifdef C_PLUS_MODE
#define DTCCR	0x10		/* Dump tally counter command reg*/
#define TNPDS   0x20 		/* Tx normal descriptors (also four 32bit). */
#define THPDS   0x28 		/* Tx hi descriptors (also four 32bit). */
#else
#define TxStatus0   0x10 	/* Transmit status (Four 32bit registers). */
#define TxAddr0   0x20 		/* Tx descriptors (also four 32bit). */
#define RxBuf   0x30 
#endif

#define RxEarlyCnt   0x34 
#define rRxEarlyCnt   0x36 
#define RxEarlyStatus   0x36 
#define rRxEarlyStatus   0x34 
#define ChipCmd   0x37 
#define rChipCmd   0x34 

#ifndef C_PLUS_MODE
#define RxBufPtr   0x38 
#define rRxBufPtr   0x3A 
#define RxBufAddr   0x3A 
#define rRxBufAddr   0x38 
#endif

#define IntrMask   0x3C 
#define rIntrMask   0x3E 
#define IntrStatus   0x3E 
#define rIntrStatus   0x3C 
#define TxConfig   0x40 
#define ChipVersion   0x43 
#define RxConfig   0x44 
#define Timer   0x48 		/* A general-purpose counter. */
#define RxMissed   0x4C 	/* 24 bits valid  write clears. */
#define Cfg9346   0x50 
#define rCfg9346   0x53 
#define Config0   0x51 
#define rConfig0   0x52 
#define Config1   0x52 
#define rConfig1   0x51 
#define FlashReg   0x54 
#define MediaStatus   0x58 
#define rMediaStatus   0x5B
#define Config3   0x59 
#define rConfig3   0x5A 
#define Config4   0x5A 		/* absent on RTL-8139A */
#define rConfig4   0x59 		/* absent on RTL-8139A */
//#define HltClk   0x5B 
#define MultiIntr   0x5C 
#define rMultiIntr   0x5E 

#ifndef C_PLUS_MODE
#define TxSummary   0x60 
#define rTxSummary   0x62 
#endif

#define BasicModeCtrl   0x62 
#define rBasicModeCtrl   0x60 
#define BasicModeStatus   0x64 
#define rBasicModeStatus   0x66 
#define NWayAdvert   0x66 
#define rNWayAdvert   0x64 
#define NWayLPAR   0x68 
#define rNWayLPAR   0x6A 
#define NWayExpansion   0x6A 
#define rNWayExpansion   0x68 
/* Undocumented registers  but required for proper operation. */
#define FIFOTMS   0x70 		/* FIFO Control and test. */
#define CSCR8139   0x74 		/* Chip Status and Configuration Register. */
#define rCSCR8139   0x76 		/* Chip Status and Configuration Register. */
#define PARA78   0x78 
#define PARA7c   0x7c 		/* Magic transceiver parameter register. */
#define Config5   0xD8 		/* absent on RTL-8139A */

#ifdef C_PLUS_MODE
#define TPPoll   0xD9 		/* Tx priority polling reg */
#define rTPPoll   0xDA 		/* Tx priority polling reg */
#endif

#define rConfig5   0xDB 		/* absent on RTL-8139A */

#ifdef C_PLUS_MODE
#define CPCR   0xE0 		/* C+ mode reg */
#define rCPCR   0xE2 		/* C+ mode reg */
#define RDSAR   0xE4 		/* Rx desc addr reg */
#define ETThR   0xEC 		/* Early Tx threshold reg */
#endif

#endif
