#include <linux/types.h>
#undef C_PLUS_MODE
#include "pci.h"
#include "pci8139.h"
#include "rtl8650.h"

#define RXBFRSIZE 8192

int getchar(char in);
enum ClearBitMasks {
	MultiIntrClear = 0xF000,
	ChipCmdClear = 0xE2,
	Config1Clear = (1<<7)|(1<<6)|(1<<3)|(1<<2)|(1<<1),
};

enum ChipCmdBits {
	CmdReset = 0x10,
	CmdRxEnb = 0x08,
	CmdTxEnb = 0x04,
	RxBufEmpty = 0x01,
};

/* Interrupt register bits, using my own meaningful names. */
enum IntrStatrtl8139nicits {
	PCIErr = 0x8000,
	PCSTimeout = 0x4000,
	RxFIFOOver = 0x40,
	RxUnderrun = 0x20,
	RxOverflow = 0x10,
	TxErr = 0x08,
	TxOK = 0x04,
	RxErr = 0x02,
	RxOK = 0x01,
};
enum TxStatrtl8139nicits {
	TxHostOwns = 0x2000,
	TxUnderrun = 0x4000,
	TxStatOK = 0x8000,
	TxOutOfWindow = 0x20000000,
	TxAborted = 0x40000000,
	TxCarrierLost = 0x80000000,
};
enum RxStatrtl8139nicits {
	RxMulticast = 0x8000,
	RxPhysical = 0x4000,
	RxBroadcast = 0x2000,
	RxBadSymbol = 0x0020,
	RxRunt = 0x0010,
	RxTooLong = 0x0008,
	RxCRCErr = 0x0004,
	RxBadAlign = 0x0002,
	RxStatusOK = 0x0001,
};

/* Bits in RxConfig. */
enum rx_mode_bits {
	AcceptErr = 0x20,
	AcceptRunt = 0x10,
	AcceptBroadcast = 0x08,
	AcceptMulticast = 0x04,
	AcceptMyPhys = 0x02,
	AcceptAllPhys = 0x01,
};

/* Bits in TxConfig. */
enum tx_config_bits {
	TxIFG1 = (1 << 25),	/* Interframe Gap Time */
	TxIFG0 = (1 << 24),	/* Enabling these bits violates IEEE 802.3 */
	TxLoopBack = (1 << 18) | (1 << 17), /* enable loopback test mode */
	TxCRC = (1 << 16),	/* DISABLE appending CRC to end of Tx packets */
	TxClearAbt = (1 << 0),	/* Clear abort (WO) */
	TxDMAShift = 8,		/* DMA burst value (0-7) is shift this many bits */

	TxVersionMask = 0x7C800000, /* mask out version bits 30-26, 23 */
};

/* Bits in Config1 */
enum Config1Bits {
	Cfg1_PM_Enable = 0x01,
	Cfg1_VPD_Enable = 0x02,
	Cfg1_PIO = 0x04,
	Cfg1_MMIO = 0x08,
	Cfg1_LWAKE = 0x10,
	Cfg1_Driver_Load = 0x20,
	Cfg1_LED0 = 0x40,
	Cfg1_LED1 = 0x80,
};

enum RxConfigBits {
	/* Early Rx threshold, none or X/16 */
	RxCfgEarlyRxNone = 0,
	RxCfgEarlyRxShift = 24,

	/* rx fifo threshold */
	RxCfgFIFOShift = 13,
	RxCfgFIFONone = (7 << RxCfgFIFOShift),

	/* Max DMA burst */
	RxCfgDMAShift = 8,
	RxCfgDMAUnlimited = (7 << RxCfgDMAShift),

	/* rx ring buffer length */
	RxCfgRcv8K = 0,
	RxCfgRcv16K = (1 << 11),
	RxCfgRcv32K = (1 << 12),
	RxCfgRcv64K = (1 << 11) | (1 << 12),

	/* Disable packet wrap at end of Rx buffer */
	RxNoWrap = (1 << 7),
};


/* Twister tuning parameters from RealTek.
   Completely undocumented, but required to tune bad links. */
enum CSCRBits {
	CSCR_LinkOKBit = 0x0400,
	CSCR_LinkChangeBit = 0x0800,
	CSCR_LinkStatrtl8139nicits = 0x0f000,
	CSCR_LinkDownOffCmd = 0x003c0,
	CSCR_LinkDownCmd = 0x0f3c0,
};


enum Cfg9346Bits {
	Cfg9346_Lock = 0x00,
	Cfg9346_Unlock = 0xC0,
};


int rtl8139nic_ISR(pci_config_s *pcfg);

static const u16 rtl8139_intr_mask =
	PCIErr | PCSTimeout | RxUnderrun | RxOverflow | RxFIFOOver |
	TxErr | TxOK | RxErr | RxOK;

u32 ioaddr1;
u8 *txbfr, *rxbfr;
int tx_producer;

//use 32bit RW for PCI device
u8 Reg8r(u32 addr)
{
    return REG8(addr);
}

void Reg8w(u32 addr, u8 data)
{
    REG8(addr)=data;
}

u16 Reg16r(u32 addr)
{
    return REG16(addr);
}

void Reg16w(u32 addr, u16 data)
{
    REG16(addr) = data;
}


/*
enable 8139 nic card
rtn 0:fail, -1 ok
*/
int pci8139nic_Enable(int slot, int func, pci_config_s *pcfg)
{ 
  u8 tmp8;
          
	/* Soft reset the chip. */
	Reg8w(ioaddr1+ChipCmd, (Reg8r(ioaddr1+rChipCmd)&ChipCmdClear) | CmdReset);
	while (Reg8r(ioaddr1+rChipCmd)&CmdReset) ;  //wait reset complete
	/* Bring the chip out of low-power mode. */	
	Reg8w(ioaddr1+Config1, 0);
	/* make sure chip thinks PIO and MMIO are enabled */
	tmp8 = Reg8r(ioaddr1+rConfig1);
	if ((tmp8 & Cfg1_PIO) == 0) {
		printk("PIO not enabled, Cfg1=%02X, aborting\n", tmp8);
		return 0;
	}
	if ((tmp8 & Cfg1_MMIO) == 0) {
		printk("MMIO not enabled, Cfg1=%02X, aborting\n", tmp8);
		return 0;
	}
	return -1;
}

#if 0
u32 rtl8139nic_SendPacket(packet_desc_t *pkt)//, int desc_num)
{
    printk("rtl8139nic Send Pkt %X\n", pkt);
    return 0;
}
#endif
char rx_buff[12000];
#define RX_MODE 0x0000E00F
#define TX_MODE 0x00000700
u32 rtl8139nic_Open(void)
{
  int i;
  u8 tmp;
  
    
  	printk("Open rtl8139nic\n");
    
    pci_int_Register(0, 0, rtl8139nic_ISR);

    //set Tx/Rx data bfr
    txbfr = (u8*)((u32)(kmalloc(8192+256)+256)&0xFFFFFF00|0x20000000);
    //txbfr=(u8*)0xA0410000;
    memset(txbfr, 0, RXBFRSIZE);
    for (i=0;i<4;i++) {
      REG32(ioaddr1+TxAddr0+i*4) = (u32)(txbfr+i*2048)&0x1FFFFFFF;  //map to physical addr
    };
    //rxbfr = (u8*)((u32)(kmalloc(8192+256+16)+256)&0xFFFFFF00|0x20000000);
    rxbfr = (u8*)((u32)(rx_buff+256)&0xFFFFFF00|0x20000000);
    //rxbfr=(u8*)0xA0430000;
    memset(rxbfr, 0, RXBFRSIZE);
    REG32(ioaddr1+RxBuf) = (u32)rxbfr&0x1FFFFFFF;  //map to physical addr
    
	/* Soft reset the chip. */
	Reg8w(ioaddr1+ChipCmd, (Reg8r(ioaddr1+rChipCmd)&ChipCmdClear) | CmdReset);
	while (Reg8r(ioaddr1+rChipCmd)&CmdReset) ;  //wait reset complete
	/* Must enable Tx/Rx before setting transfer thresholds! */
	Reg8w(ioaddr1+ChipCmd, (Reg8r(ioaddr1+rChipCmd) & ChipCmdClear) | CmdRxEnb | CmdTxEnb);
        //RX config
        if (RXBFRSIZE==8192)
            REG32(ioaddr1+RxConfig) = RX_MODE;//0xFF00E00F;
        else
            REG32(ioaddr1+RxConfig) = 0x0000E90F;//0xFF00E00F;
        //reset CARP
        Reg16w(ioaddr1+RxBufPtr, 0x0000FFF0);
    	//TX config
	REG32(ioaddr1+TxConfig) = TX_MODE;//0x00010000;

	/* unlock Config[01234] and BMCR register writes */
	Reg8w(ioaddr1+Cfg9346, Cfg9346_Unlock);
	for (i=0;i<100;i++);
	//config 
    //auto flow control
    //Reg8w(ioaddr1+rMediaStatus, 0xc0);        
    //auto neg
    Reg16w(ioaddr1+BasicModeCtrl, 0x1100);        
	tmp = Reg8r(ioaddr1+rConfig1) & Config1Clear;
	tmp |= Cfg1_Driver_Load;
	tmp |= 3; /* Enable PM/VPD */
	Reg8w(ioaddr1+Config1, tmp);
	tmp = Reg8r(ioaddr1+rConfig4) & ~(1<<2);
	/* chip will clear Rx FIFO overflow automatically */
	tmp |= (1<<7);
	Reg8w(ioaddr1+Config4, tmp);
	/* disable magic packet scanning, which is enabled
	 * when PM is enabled above (Config1) */
	Reg8w(ioaddr1+Config3, Reg8r(ioaddr1+rConfig3) & ~(1<<5));
	/* lock Config[01234] and BMCR register writes */
	Reg8w(ioaddr1+Cfg9346, Cfg9346_Lock);
	for (i=0;i<100;i++);
	
	//reset miss counter
	REG32(ioaddr1+RxMissed) = 0;
	/* no early-rx interrupts */
	Reg16w(ioaddr1+MultiIntr, Reg16r(ioaddr1+rMultiIntr) & MultiIntrClear);
	/* make sure RxTx has started */
	Reg8w(ioaddr1+ChipCmd, (Reg8r(ioaddr1+rChipCmd)& ChipCmdClear) | CmdRxEnb | CmdTxEnb);
	//reset Tx index
	tx_producer = 3;
	/* Enable all known interrupts by setting the interrupt mask. */
	Reg16w(ioaddr1+IntrMask, rtl8139_intr_mask);
	//Reg16w(ioaddr1+IntrMask, 0);
	
    	
    return 0;
}

u32 rtl8139nic_Close(void)
{
	
    printk("Close rtl8139nic\n");
    return 0;
}

u32 rtl8139nic_IOControl(u32 io_id, void *data)
{
    printk("rtl8139nic IO Request\n");
	
    return 0;
}

void reset_rx(u32 *loc)
{
    //disable Rx
    Reg8w(ioaddr1+ChipCmd, CmdTxEnb);
    
    //reset counter
    *loc=0;
    Reg16w(ioaddr1+RxBufPtr, 0xFFF0);
    //set Rx mode
    REG32(ioaddr1+RxConfig) = 0x0000E10F;//0xFF00E00F;
    
    //re-enable Rx
    Reg8w(ioaddr1+ChipCmd, (Reg8r(ioaddr1+rChipCmd)& ChipCmdClear) | CmdRxEnb | CmdTxEnb);
    while ((Reg8r(ioaddr1+rChipCmd)&(CmdRxEnb|CmdTxEnb))!=(CmdRxEnb|CmdTxEnb) ) ;

    //set Rx mode again
    REG32(ioaddr1+RxConfig) = 0x0000E10F;//0xFF00E00F;

    return;
}   

void reset_tx(void)
{
    //disable Tx
    Reg8w(ioaddr1+ChipCmd, CmdRxEnb);
    
    //re-enable Tx
    Reg8w(ioaddr1+ChipCmd, (Reg8r(ioaddr1+rChipCmd)& ChipCmdClear) | CmdRxEnb | CmdTxEnb);
    while ((Reg8r(ioaddr1+rChipCmd)&(CmdRxEnb|CmdTxEnb))!=(CmdRxEnb|CmdTxEnb) ) ;

    //set Tx mode again
    REG32(ioaddr1+TxConfig) = TX_MODE;//0xFF00E00F;

    return;
}   

/*
parameter: 
	*loc: rx bfr consumer location, it will move to pkt header if receive a good pkt,
	      or the total pkt will be skipped if rx status header report any error
	*getbytenum: byte count of this RX
rtn: 
	number of bytes in this pkt, 0 means pkt is bad and skipped
	-1 means byte count is not enough to parse a kt
*/
u32 parsePkt(u32 *loc, u32 *getbytenum)
{
  u32 num;
  u32 tmp_loc;
  u8 *pbfr;
  u32 status=0;
  
    if (*getbytenum<64) return -2;  //pkt is too smalll, wait next turn
    
    //align to 4 byte
    *loc=((*loc+3)&0xFFFFFFFC)&(RXBFRSIZE-1);
    tmp_loc=*loc;
    pbfr = (u8*)rxbfr;
    //get status
    status = (u16)pbfr[tmp_loc];
    tmp_loc = (tmp_loc+1)&(RXBFRSIZE-1);
    status = status|(pbfr[tmp_loc]<<8);
    tmp_loc = (tmp_loc+1)&(RXBFRSIZE-1);
    //get pkt size
    num = (u16)pbfr[tmp_loc];
    tmp_loc = (tmp_loc+1)&(RXBFRSIZE-1);
    num = num|(pbfr[tmp_loc]<<8);
    tmp_loc = (tmp_loc+1)&(RXBFRSIZE-1);
    if ((num>1600)||(num<8)) {  //frame out of sync
      	//reset_rx(loc);
      	*getbytenum = 0;
      	return -1;
    };
      
    if (num>(*getbytenum-4)) 
      return -1;  //this is not a complete pkt
      
    //update ptr
    *loc = tmp_loc;  //skip rx desc
    *getbytenum -= 4;
    //check pkt status
    if (!(status&1)) {  //skip bad packet
        tmp_loc = (tmp_loc+num)&(RXBFRSIZE-1);
        *loc = tmp_loc;
        *getbytenum -= num;
        num = 0;
    };
    return num;
}

void wait_TXOK(u32 ioaddr1, u32 TxStatus)
{
  u32 status;
    
#if 1
wait_pause:
        status = Reg8r(ioaddr1+rMediaStatus);  //get pause flag
        if (status&0x00000001) goto wait_pause;
wait_txok:
        //status = Reg16r(ioaddr1+rIntrStatus);
        //if (status&0x2020) 
        //    Reg16w(ioaddr1+IntrStatus, 0x2020);
  //for (i=0;i<0x80000;i++) {
  //};
        status = REG32(ioaddr1+TxStatus);
        //printk("1 %X\r",status);
        if (!(status&0x00002000)) goto wait_txok;
#else
  int i;
//loop wait
  for (i=0;i<0x00200000;i++) {
  };
check_sts:
//          status = Reg16r(ioaddr1+rIntrStatus);
//        if (status&0x2020) {
//          if (status!=0) {
//            Reg16w(ioaddr1+IntrStatus, status);
//            goto check_sts;
//        };
#endif        
};	
static volatile int dd;

void test_tx()
{
  int i, send_num, size;
  u32 status;
  char bfr[8];
  REG32(GIMR)=0;
    //send 1 pkt
    for (i=0;i<2048;i++) {
    	txbfr[i+6]=i;
    	txbfr[1*2048+i+6]=i;
    	txbfr[2*2048+i+6]=i;
    	txbfr[3*2048+i+6]=i;
    };
    for (i=0;i<4;i++)
    {
    	txbfr[0+i*2048]=0x00;    txbfr[1+i*2048]=0x50;
    txbfr[2+i*2048]=0xfc;    txbfr[3+i*2048]=0x82;
    txbfr[4+i*2048]=0x9e;    txbfr[5+i*2048]=0x4e;
    }

    
///////repeat send pkt test
    	//TX config
	REG32(ioaddr1+TxConfig) = TX_MODE;//0x00010000;
	Reg16w(ioaddr1+IntrMask, 0);
        //REG32(GIMR) = REG32(GIMR)&0xFDFFFFFF;  //disable pci int
    
    size=1510;
    i=0; send_num=0;
    while (1) {
    	send_num++;
    	if ((send_num&0x00FF)==0) {
          if  (getchar(bfr) != 0) {
            if (bfr[0]=='q') {
                printk("Quit Tx Test\n");    	
                break;
            };
          };
        };
wait1:
        status = Reg16r(ioaddr1+rTxSummary);
        //printk("1 %X\r",status);
        if (!(status&0x00000001)) {
            for (i=0;i<100;i++) dd++;
            goto wait1;
        };
        i+=33;
        if (i<60) i=60;
        if (i>1510) i=1510;
        REG32(ioaddr1+TxStatus0) = 0x00FF0000|size;  //size=256
        if (i==1510) i=60;

wait2:
        //for (i=0;i<0x20000;i++) ;
        status = Reg16r(ioaddr1+rTxSummary);
        //printk("2 %X\r",status);
        if (!(status&0x00000002)) {
            for (i=0;i<100;i++) dd++;        	
            goto wait2;
        };
        i+=4;
        if (i<60) i=60;
        if (i>1510) i=1510;
        REG32(ioaddr1+TxStatus0+4) = 0x00FF0000|size;  //size=256
        if (i==1510) i=60;

wait3:
        //for (i=0;i<0x20000;i++) ;
        status = Reg16r(ioaddr1+rTxSummary);
        //printk("3 %X\r",status);
        if (!(status&0x00000004)) {
            for (i=0;i<100;i++) dd++;        	
            goto wait3;
        };
        i+=9;
        if (i<60) i=60;
        if (i>1510) i=1510;
        REG32(ioaddr1+TxStatus0+8) = 0x00FF0000|size;  //size=256
        if (i==1510) i=60;

wait4:
        status = Reg16r(ioaddr1+rTxSummary);
        //printk("4 %X\r",status);
        if (!(status&0x00000008)) {
            for (i=0;i<100;i++) dd++;
            goto wait4;
	};
        i+=17;
        if (i<60) i=60;
        if (i>1510) i=1510;
        REG32(ioaddr1+TxStatus0+12) = 0x00FF0000|size;  //size=256
        if (i==1510) i=60;
    };	
}

int txnum;
int TxWait[4];
u32 getApkt;

void test_loopback(int mode)
{
  u8 tmp;
  u32 tmp_sts, status, ovfl_sts;
  int i, kk, pkt_size, count, hw_count, consumer, bus_error, wait_num;
  char bfr[8];
  
    //clear IRQA
    status = Reg16r(ioaddr1+rIntrStatus);
    Reg16w(ioaddr1+IntrStatus, status);

    //REG16(GISR)=PCIIP;  //PCI IRQ pending clear
    REG32(GIMR)=0;  //PCI IRQ pending clear
    
    kk=0; consumer=0; getApkt=0;
    TxWait[0]= TxWait[1]=TxWait[2]=TxWait[3] =0;

    //REG32(GIMR) = REG32(GIMR)&0xFDFFFFFF;  //disable pci int
    Reg16w(ioaddr1+IntrMask, 0);

wait_num=0;
repeat_test:    

    i=0; bus_error=0; 
waitRx:
    wait_num++;
#if 0 
    if ((wait_num&0x00FF)==0) {
        if  (getchar(bfr) != 0) {
            if (bfr[0]=='q') {
                printk("Quit Loopback Test\n");    	
                return;
            };
        }
    };
#endif
    status = Reg16r(ioaddr1+rIntrStatus);
    //status = REG32(PCI_SLOT0_CONFIG_BASE);
    //if (i&1) printk("Rx status: 0x%4X <\r", status);
    //else printk("Rx status: 0x%4X >\r", status);
    tmp_sts = 0;
    if (status&0x8000) {  //PCI bus error
      //printk("PCI bus error sts %X\n", status);
      //while (1);
      tmp_sts |= 0x8000;
      bus_error++;
    };
    if (status&0x0004)  //TxOK
      tmp_sts |= 0x0004;
    if (status&0x0008)  //TxError
      tmp_sts |= 0x0008;
    if (status&0x2000)  //cable change
      tmp_sts |= 0x2000;
    if (status&0x0020)  //link change
      tmp_sts |= 0x0020;
    if (status&0x0050) {  //RX buffer overflow //RX FIFO overflow
      if (status&0x0010) tmp_sts |= 0x0010;  //clear RX bfr overflow
      if (status&0x0040) tmp_sts |= 0x0040;  //clear RX FIFO overflow
      if (!(status&0x0003)) {  //no pkt rcv in, clear to allow RX again
        if (status&0x0010) tmp_sts |= 0x0010;  //clear RX bfr overflow
        if (status&0x0040) tmp_sts |= 0x0040;  //clear RX FIFO overflow
        //reset_rx((u32*)&consumer);  //clear rx bfr
      } else {  //we have pkt in rx bfr though RXBFR is overflow, process pkts if possible
        ovfl_sts = 0;
        if (status&0x0010) ovfl_sts |= 0x0010;  //clear RX bfr overflow
        if (status&0x0040) ovfl_sts |= 0x0040;  //clear RX FIFO overflow
      };
    };
    if (tmp_sts!=0)  //clear status, rx bfr/fifo overflow not included
      Reg16w(ioaddr1+IntrStatus, tmp_sts);
    if (!(status&0x0003)) goto waitRx;

    //clear rx pkt related status only, we need to handle overflow case
    Reg16w(ioaddr1+IntrStatus, status&0x03);
    
    hw_count=Reg16r(ioaddr1+rRxBufAddr);
    count=(hw_count&(RXBFRSIZE-1))-consumer;
    if (count<0) count+=RXBFRSIZE;
    //printk("RxOK: No.0x%X size:0x%X\n", kk, count);
    

#if 0
    //move ptr forward, no parse needed
    consumer=(consumer+count)&(RXBFRSIZE-1);
#else
    //*((u16*)rxbfr+14);
    //copy pkt data from Rx bfr to Tx bfr
    //parse packet
re_parse:
    //loopback
    tmp=(tx_producer+1)&3;
    pkt_size=parsePkt((u32*)&consumer, (u32*)&count);
    if (pkt_size>0) {
      if (mode==1) {  //loopback w/ data copy
    	if ((consumer+pkt_size)<RXBFRSIZE) {
            memcpy(txbfr+tmp*2048, rxbfr+consumer, pkt_size);
        } else {
            memcpy(txbfr+tmp*2048, rxbfr+consumer, (RXBFRSIZE-consumer));
            memcpy(txbfr+tmp*2048+(RXBFRSIZE-consumer), rxbfr, (pkt_size-(RXBFRSIZE-consumer)));
        };
      };
        
wait_tx_ready:
        status = Reg16r(ioaddr1+rTxSummary);
        if (!(status&(1<<tmp))) {
            for (i=0;i<100;i++) ;
            goto wait_tx_ready;
        };
        REG32(ioaddr1+TxStatus0+tmp*4) = 0x003F0000|(pkt_size-4);  //send it
//wait_tx_ready1:        
//        status = Reg16r(ioaddr1+rTxSummary);
//            for (i=0;i<100;i++) ;
//        if (!(status&(1<<tmp))) goto wait_tx_ready1;
    
        consumer=((consumer+pkt_size+3)&0xFFFFFFFC)&(RXBFRSIZE-1);
        count=count-pkt_size;
        //printk("hwcount %X rest %X cons %X pktsz %X\n", hw_count, count, consumer, pkt_size);
        
        tx_producer = tmp;  //update pointer
     	goto re_parse;
    };
#endif

    kk++;
    //update CAPR
    Reg16w(ioaddr1+RxBufPtr, (consumer-16)&(RXBFRSIZE-1));
    if(ovfl_sts!=0) {  //clear overflow case
      //reset_rx((u32*)&consumer);
      Reg16w(ioaddr1+IntrStatus, ovfl_sts);
      ovfl_sts = 0;
    };
    getApkt--;
    goto repeat_test;	
}

enum chip_flags {
	HasHltClk = (1 << 0),
	HasLWake = (1 << 1),
};


const static struct {
	const char *name;
	u8 version; /* from RTL8139C docs */
	u32 RxConfigMask; /* should clear the bits supported by this chip */
	u32 flags;
} rtl_chip_info[] = {
	{ "RTL-8139",
	  0x40,
	  0xf0fe0040, /* XXX copied from RTL8139A, verify */
	  HasHltClk,
	},

	{ "RTL-8139 rev K",
	  0x60,
	  0xf0fe0040,
	  HasHltClk,
	},

	{ "RTL-8139A",
	  0x70,
	  0xf0fe0040,
	  HasHltClk, /* XXX undocumented? */
	},

	{ "RTL-8139B",
	  0x78,
	  0xf0fc0040,
	  HasLWake,
	},

	{ "RTL-8130",
	  0x7C,
	  0xf0fe0040, /* XXX copied from RTL8139A, verify */
	  HasLWake,
	},

	{ "RTL-8139C",
	  0x74,
	  0xf0fc0040, /* XXX copied from RTL8139B, verify */
	  HasLWake,
	},

};




int *init_RTL8139nic(int slot, int func, pci_config_s *pcfg)
{
  //driverif_t *drvif;
  u8 tmp;
  int i;
  u8 bfr[8];
 
     txbfr=0;
     rxbfr=0;
    ioaddr1 = pcfg->BAR[1].Address;
    getApkt = 0;
    /* Put the chip into low-power mode. */
    Reg8w(ioaddr1+Cfg9346, Cfg9346_Unlock);


     tmp = Reg8r(ioaddr1+ChipVersion);
	for (i = 0; i <  5; i++)
		if (tmp == rtl_chip_info[i].version) {
			break;
		}

    for (i=0;i<100;i++); 
    i=Reg8r(ioaddr1+rCfg9346);

    /*reset phy*/
    Reg16w(ioaddr1+BasicModeCtrl, (Reg16r(ioaddr1+rBasicModeCtrl)&0x7FFF) | 0x8000);
    while ((Reg16r(ioaddr1+rBasicModeCtrl)&0x8000)) ;  //wait reset complete
    
    //LED on/OFF
    tmp = Reg8r(ioaddr1+rConfig1);
    Reg8w(ioaddr1+Config1, tmp&0x3F);
    Reg8w(ioaddr1+Config1, tmp|0xc0);
    Reg8w(ioaddr1+Config1, tmp&0x3F);
    //enable 8139 nic device
    if (!pci8139nic_Enable(slot, func, pcfg)) return NULL;
                   
    
    tmp = Reg8r(ioaddr1+rConfig1) & Config1Clear;
    tmp |= 3; /* Enable PM/VPD */
    Reg8w(ioaddr1+Config1, tmp);
    
    rtl8139nic_Open();
    
reread_cmd:        
    printk("\n8139 [0]Loopback Test without data copy, [1]Loopback Test w/ data copy, [2]Tx Test, [q]Quit? (return for 0)");


        test_loopback(1);
    while (getchar(bfr) == 0);
    printk("%c\n", *bfr);

    if (*bfr=='2') {
        printk("enter 'q' to quit test!\n");
        test_tx();
    } else if ((*bfr=='0')||(*bfr==0x0d)) {
        printk("enter 'q' to quit test!\n");
        test_loopback(0);
    } else if ((*bfr=='1')||(*bfr==0x0d)) {
        printk("enter 'q' to quit test!\n");
        test_loopback(1);
    } else if (*bfr!='q')
        goto reread_cmd;
    if (txbfr!=0) kfree(txbfr);
    if (rxbfr!=0) kfree(rxbfr);
    return NULL;
}

int rtl8139nic_ISR(pci_config_s *pcfg)
{
#if 0
  u16 status, link_changed;
  int rtn_val;
  
        rtn_val=0;
    	//check if this is my IRQ
	status = Reg16r(ioaddr1+rIntrStatus);
	/* h/w no longer present (hotplug?) or major error, bail */
	if (status == 0xFFFF)
		return 0;
	/* Acknowledge all of the current interrupt sources ASAP, but
	   an first get an additional status bit from CSCR. */
	if (status & RxUnderrun)
		link_changed = Reg16r(ioaddr1+rCSCR8139) & CSCR_LinkChangeBit;
	//clear IRQ
  	Reg16w(ioaddr1+IntrStatus, status);
	if ((status & (PCIErr | PCSTimeout | RxUnderrun | RxOverflow |
		RxFIFOOver | TxErr | TxOK | RxErr | RxOK)) == 0)
		return 0;  //not my irq
	//check RX
	//if (status & (RxOK | RxUnderrun | RxOverflow | RxFIFOOver)) {	/* Rx interrupt */
	//	while (1);  //check data if it is correct
	//};
	if (status & RxOK) {	/* Rx interrupt, loopback */
	    if (!(Reg8r(ioaddr1+rChipCmd)&1)) {  //pkt in Rx bfr
	        //u16 rx_size;
	        getApkt += 1;
	        //disable RX
 	        //Reg8w(ioaddr1+0x0037, Reg8r(ioaddr1+rChipCmd)&0xF7);
	        //tmp=(tx_producer+1)&3;
	        //rx_size=*((u16*)rxbfr+14);
	        //copy pkt data from Rx bfr to Tx bfr
	        //memcpy(txbfr+tmp*2048, rxbfr+2, rx_size+18);
	        //memcpy(txbfr+tmp*2048, rxbfr, rx_size+18);
	        //REG32(ioaddr1+TxStatus0+tmp*4) = REG32(ioaddr1+TxStatus0+tmp*4)|rx_size;  //send it
	        //REG32(ioaddr1+TxStatus0+tmp*4) = REG32(ioaddr1+TxStatus0+tmp*4)|0x100;  //send it
	    	//tx_producer = tmp;  //update pointer
		    rtn_val= -1;
	    };
	};

	if (status & (TxOK | TxErr)) {  //enable rx
	    if (REG32(ioaddr1+TxStatus0)&0x00002000) TxWait[0]=0;
	    else  TxWait[0]=1;
	    if (REG32(ioaddr1+TxStatus0+4)&0x00002000) TxWait[1]=0;
	    else  TxWait[1]=1;
	    if (REG32(ioaddr1+TxStatus0+8)&0x00002000) TxWait[2]=0;
	    else  TxWait[2]=1;
	    if (REG32(ioaddr1+TxStatus0+12)&0x00002000) TxWait[3]=0;
	    else  TxWait[3]=1;
		rtn_val= -1;
	};
    return rtn_val;
#endif
}
extern jiffies;
int getchar(char in)
{
	if (jiffies%3==0)
		return 0;
	else return 1;
}
