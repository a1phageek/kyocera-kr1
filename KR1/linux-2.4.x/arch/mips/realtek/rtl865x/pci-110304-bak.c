#include <linux/config.h>
#ifdef CONFIG_RTL865XB_OLD_PCI


#include <linux/ioport.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/paccess.h>
#include <asm/pci_channel.h>
#include "rtl865x.h"
#include <asm-mips/rtl865x/interrupt.h>
#include "pci.h"
static int rtl865x_isLinuxIncompliantEndianMode=1;//Default assume 8650B 

#undef RTL865X_PCI_DEBUG

#ifdef CONFIG_RTL865X_PCI0
	#define RTL865X_PCI_SLOT0\
	{ 0, PCI_SLOT0_CONFIG_BASE},
#else
	#define RTL865X_PCI_SLOT0
#endif

#ifdef CONFIG_RTL865X_PCI1
	#define RTL865X_PCI_SLOT1\
	{ 1, PCI_SLOT1_CONFIG_BASE},
#else
	#define RTL865X_PCI_SLOT1
#endif

#ifdef CONFIG_RTL865X_PCI2
	#define RTL865X_PCI_SLOT2\
	{ 2, PCI_SLOT2_CONFIG_BASE},
#else
	#define RTL865X_PCI_SLOT2
#endif

#ifdef CONFIG_RTL865X_PCI3
	#define RTL865X_PCI_SLOT3\
	{ 3, PCI_SLOT3_CONFIG_BASE},
#else
	#define RTL865X_PCI_SLOT3
#endif

#define RTL865X_PCI_SLOT_BASEADDR_DEFINES\
	RTL865X_PCI_SLOT0\
	RTL865X_PCI_SLOT1\
	RTL865X_PCI_SLOT2\
	RTL865X_PCI_SLOT3

struct pci_slot_baseaddr_s  rtl865x_pcicard_slotaddr[]={ 	
	RTL865X_PCI_SLOT_BASEADDR_DEFINES
};
//{0, PCI_SLOT0_CONFIG_BASE}
//{1, PCI_SLOT1_CONFIG_BASE}
//{2,PCI_SLOT2_CONFIG_BASE},
//{3,PCI_SLOT3_CONFIG_BASE}



/*
 * Functions for accessing external PCI configuration space
 */

int rtl_pci_isLinuxCompliantEndianMode(){
   	rtl865x_isLinuxIncompliantEndianMode=!((*(volatile unsigned int *)(0xbd012064))&2);
	return ((*(volatile unsigned int *)(0xbd012064))&2);
}
int rtl_pci_get_slot(int index, unsigned *slot, unsigned *addr){
	if(index>=sizeof(rtl865x_pcicard_slotaddr)/sizeof(struct pci_slot_baseaddr_s))
		return -1;
	if(rtl865x_pcicard_slotaddr[index].slotNum<0)
		return -1;
	if(slot)
		*slot=rtl865x_pcicard_slotaddr[index].slotNum;
	if(addr)
		*addr=rtl865x_pcicard_slotaddr[index].addr;
	return 0;
}

int rtl_pci_check_slot(unsigned int devfn){
	int count;
	if((devfn>>3)>=sizeof(rtl865x_pcicard_slotaddr)/sizeof(struct pci_slot_baseaddr_s))
		return -1;
	if(rtl865x_pcicard_slotaddr[devfn>>3].slotNum<0)
		return -1;
	return devfn;
}

int rtl_pci_reset(){
	u32 tmp, divisor, scr;	
    tmp = REG32(0xbc803000)&0xFCFF8FFF; 
    REG32(0xbc803004)=0;
    scr=(REG32(0xbc803004)>>16)&7;
    if(scr==3)
	   divisor=2;
    else{
		u32 ratio[]={5,5,5,5,4,4,3,2};//check Datasheet SCR and MACCR.
		REG32(0xbc803000) = tmp|(1<<15);//reset PCI bridge
		divisor=ratio[scr];
    }
    REG32(0xbc803000) = tmp|(divisor<<12);
	return divisor;
}

int rtl_pci_scan_slot(struct pci_dev *dev){
	u8 hdr_type=0xff;
	if(0>rtl_pci_check_slot(dev->devfn))
		return -1;
	if(REG32(0xbd012004)&0x10000){
		printk("PCI reset , bus timeout at %s %d!\n",__FILE__,__LINE__);
		rtl_pci_reset();
		//REG32(0xbc803000)&=~0x8000;
		//REG32(0xbd012004)=0x10000;
	}
	rtlpci_read_config_byte(dev, PCI_HEADER_TYPE, &hdr_type);
#if 0
	if(REG32(0xbd012004)&0x10000){
		rtl_pci_reset();
		REG32(0xbc803000)&=~0x8000;
		REG32(0xbd012004)=0x10000;
		printk("No device found on PCI slot %d\n",rtl865x_pcicard_slotaddr[dev->devfn>>3].slotNum);
		rtl865x_pcicard_slotaddr[dev->devfn>>3].slotNum=-1;
		return -1;
	}
#endif
	return 0;
}

void rtl_pci_init(){
	char chipVersion[16];
	int count;
	u32 divisor;
	int rev;
#define REG32(reg) (*(volatile unsigned int *)(reg))	
	printk("OLD PCI Driver...\n");
    rtl8651_getChipVersion(chipVersion, sizeof(chipVersion), &rev);
   if(!rtl_pci_get_slot(0,NULL,NULL)){
	int  slot;
	printk("PCI: Expecting PCI devices on slot(s)");
	for (count=0;;count++){
		if(rtl_pci_get_slot(count, &slot, NULL))
			break;
		printk("%d ", slot);
	    if(!strcmp(chipVersion,"8651")&&slot>1){
		printk("\n!!!!!!!!WARNING!!!!!!!!\n\tRTL%s doesn't support PCI slot %d. \n\tYou must replace the firmware image!!!\n",chipVersion,slot);
		break;
	    }
	}
	printk(". Total %d device(s).\n", count);
   }
    if(!strcmp(chipVersion,"8650")||(!strcmp(chipVersion,"8651")&&count>2)){
		printk("!!!!!!!!WARNING!!!!!!!!\n\tRTL%s doesn't support %d PCI slots. \n\tYou must replace the firmware image!!!\n",chipVersion,count);
    }
	divisor=rtl_pci_reset();
    printk("PCI: PIO/MMIO register endian mode %s Linux io.h, clk div=%x\n",rtl_pci_isLinuxCompliantEndianMode()? "=": "!=", divisor);
	
}

static int rtlpci_read_config_byte(struct pci_dev *dev, int where, u8 *value)
{

	int i;
	u32 tmp;
	u32 addr=0;
	u32 shift=3-(where & 0x3);
	if (dev->bus->number != 0){
		*value = 0;
		return PCIBIOS_SUCCESSFUL;
	}

	addr=rtl865x_pcicard_slotaddr[dev->devfn>>3].addr;
#ifdef RTL865X_PCI_DEBUG	
	printk(" %s:%d(%d)  REQ 0x%08x.", __FUNCTION__, __LINE__, dev->devfn>>3,addr+where);
#endif
	addr +=(where&~0x3);
	tmp = REG32(addr);
#ifdef RTL865X_PCI_DEBUG	
	printk("LE32 0x%08x ", tmp);
#endif
	if(rtl865x_isLinuxIncompliantEndianMode)
		tmp = cpu_to_le32(tmp);

	for (addr=0;addr<shift;addr++)
		tmp = tmp >>8;
	*value = (u8)tmp;

#ifdef RTL865X_PCI_DEBUG	
	printk("RET:0x%02x\n", *value);
#endif	
	return PCIBIOS_SUCCESSFUL;
}

static int rtlpci_read_config_word(struct pci_dev *dev, int where, u16 *value)
{

	int i;
	u32 tmp,addr=0;
	//printk("   %s:%d(%d) .\n", __FUNCTION__, __LINE__, dev->devfn>>3);	
	if (dev->bus->number != 0){
		*value = 0;
		return PCIBIOS_SUCCESSFUL;
	}
	addr=rtl865x_pcicard_slotaddr[dev->devfn>>3].addr;
#ifdef RTL865X_PCI_DEBUG		
	printk(" %s:%d(%d) REQ 0x%08x.", __FUNCTION__, __LINE__, dev->devfn>>3,addr+where);
#endif
	addr +=(where&~0x3);
	tmp = REG32(addr);
#ifdef RTL865X_PCI_DEBUG		
	printk("LE32 0x%08x ", tmp);
#endif
	if(!rtl865x_isLinuxIncompliantEndianMode)
		tmp=le32_to_cpu(tmp);
	if (where&0x2)
		tmp = (tmp>>16) ;
	*value = (u16)tmp;	
#ifdef RTL865X_PCI_DEBUG		
	printk("RET:0x%04x\n", *value);
#endif
	return PCIBIOS_SUCCESSFUL;
}

static int rtlpci_read_config_dword(struct pci_dev *dev, int where, u32 *value)
{

	unsigned int addr=0, tmp=0;
	int i;
	if (dev->bus->number != 0)
	{
		*value=0;
		return PCIBIOS_SUCCESSFUL;
	}

	addr=rtl865x_pcicard_slotaddr[dev->devfn>>3].addr;
	
#ifdef RTL865X_PCI_DEBUG		
	printk(" %s:%d(%d) REQ 0x%08x.", __FUNCTION__, __LINE__, dev->devfn>>3,addr+where);
#endif
	
	addr +=(where);

	tmp = REG32(addr);
#ifdef RTL865X_PCI_DEBUG		
	printk("LE32:0x%08x ", tmp);
#endif
	if(!rtl865x_isLinuxIncompliantEndianMode)
		tmp=le32_to_cpu(tmp);
	*value = tmp;
#ifdef RTL865X_PCI_DEBUG
	printk("RET:0x%08x\n", *value);
#endif
	return PCIBIOS_SUCCESSFUL;
}


static int rtlpci_write_config_byte(struct pci_dev *dev, int where, u8 value)
{

	int i;
	u32 addr=0;
	if (dev->bus->number != 0)		
		return PCIBIOS_SUCCESSFUL;

	addr=rtl865x_pcicard_slotaddr[dev->devfn>>3].addr;
	addr +=(where);

#ifdef RTL865X_PCI_DEBUG
	printk(" %s:%d(%d) REQ 0x%08x=%02x.", __FUNCTION__, __LINE__, dev->devfn>>3,addr, value);
	printk("W 0x%02x\n", value);	
#endif	
	REG8(addr)=value;
	return PCIBIOS_SUCCESSFUL;
}

static int rtlpci_write_config_word(struct pci_dev *dev, int where, u16 value)
{

	int i;
	u32 addr=0;
	if (dev->bus->number != 0)		
		return PCIBIOS_SUCCESSFUL;
	addr=rtl865x_pcicard_slotaddr[dev->devfn>>3].addr;
	addr +=(where);
#ifdef RTL865X_PCI_DEBUG
	printk(" %s:%d(%d) REQ 0x%08x=%04x.", __FUNCTION__, __LINE__, dev->devfn>>3,addr, value);
	printk("W 0x%04x\n", value);	
#endif	
	REG16(addr)=value;
	return PCIBIOS_SUCCESSFUL;

}

static int rtlpci_write_config_dword(struct pci_dev *dev, int where, u32 value)
{

	u32 addr=0;
	int i;
	if (dev->bus->number != 0)		
		return PCIBIOS_SUCCESSFUL;

	addr=rtl865x_pcicard_slotaddr[dev->devfn>>3].addr;
	addr +=(where);
#ifdef RTL865X_PCI_DEBUG
	printk(" %s:%d(%d) REQ 0x%08x=%08x.", __FUNCTION__, __LINE__, dev->devfn>>3,addr, value);
	printk("W 0x%08x\n", value);	
#endif	
	REG32(addr)=value;
	return PCIBIOS_SUCCESSFUL;
}


 u8 rtl865x_pci_ioread8(u32 addr){
 	//For legacy 8650's linux incompliant PCI access endian mode 	
	if(rtl865x_isLinuxIncompliantEndianMode){
		u8 tmp;
		u8 swap[4]={3,2,1,0};
		int diff = swap[addr&0x3];
#ifdef RTL865X_PCI_DEBUG		
		printk("R8 0x%08x->", addr);
#endif
		addr=addr&(~0x3);
#ifdef RTL865X_PCI_DEBUG
		printk("0x%08x=", addr);
#endif
		tmp= *(volatile u8*)(addr+diff);
#ifdef RTL865X_PCI_DEBUG
		printk("0x%02x\n", tmp);
#endif
		return tmp;
	}
	return (*(volatile unsigned char *)(addr));
}

 u16 rtl865x_pci_ioread16(u32 addr){
 	//For legacy 8650's linux incompliant PCI access endian mode 	
	if(rtl865x_isLinuxIncompliantEndianMode){
		u16 tmp;
		int diff=2-(addr&0x3);
#ifdef RTL865X_PCI_DEBUG
		printk("R16 0x%08x->", addr);
#endif
		addr=addr&(~0x3);
#ifdef RTL865X_PCI_DEBUG
		printk("0x%08x=", addr);
#endif
		tmp= *(volatile u16*)(addr+diff);
#ifdef RTL865X_PCI_DEBUG
		printk("0x%04x\n", tmp);
#endif
		return tmp;
	}
	return __ioswab16((*(volatile unsigned short *)(addr)));
}

u32 rtl865x_pci_ioread32(u32 addr){
 	//For legacy 8650's linux incompliant PCI access endian mode 	
	if(rtl865x_isLinuxIncompliantEndianMode){
		u32 tmp;
#ifdef RTL865X_PCI_DEBUG
		printk("R32 0x%08x=", addr);
#endif
		tmp= *(volatile u32*)(addr);
#ifdef RTL865X_PCI_DEBUG
		printk("0x%08x\n", tmp);
#endif
		return tmp;
	}
	return __ioswab32((*(volatile unsigned int *)(addr)));
}


void rtl865x_pci_iowrite8(u32 addr, u8 val){
	if(rtl865x_isLinuxIncompliantEndianMode)
		(*(volatile unsigned char*)(addr))=val;
	else
		(*(volatile unsigned char *)(addr)) = __ioswab8(val);
}

 void rtl865x_pci_iowrite16(u32 addr, u16 val){
	if(rtl865x_isLinuxIncompliantEndianMode)
		(*(volatile unsigned short*)(addr))=val;
	else
		(*(volatile unsigned short *)(addr)) = __ioswab16(val);
}

 void rtl865x_pci_iowrite32(u32 addr, u32 val){
	if(rtl865x_isLinuxIncompliantEndianMode)
		(*(volatile unsigned long*)(addr))=val;
	else
		(*(volatile unsigned long *)(addr)) = __ioswab32(val);
}


static struct pci_ops pcibios_ops = {
	rtlpci_read_config_byte,
	rtlpci_read_config_word,
	rtlpci_read_config_dword,
	rtlpci_write_config_byte,
	rtlpci_write_config_word,
	rtlpci_write_config_dword
};
#define PCI_IO_START      0x10000000
#define PCI_IO_END        0x1000ffff
#define PCI_MEM_START     0x18000000
#define PCI_MEM_END       0x18ffffff



static struct resource pci_io_resource = {
	"pci IO space", 
	PCI_IO_START,
	PCI_IO_END,
	IORESOURCE_IO
};

static struct resource pci_mem_resource = {
	"pci memory space", 
	PCI_MEM_START,
	PCI_MEM_END,
	IORESOURCE_MEM
};


#if 1 
struct pci_channel mips_pci_channels[] = {
	{&pcibios_ops, &pci_io_resource, &pci_mem_resource, 0, 1},
	{(struct pci_ops *) NULL, (struct resource *) NULL,
	 (struct resource *) NULL, (int) NULL, (int) NULL}
};
#endif



/*
 *  If we set up a device for bus mastering, we need to check the latency
 *  timer as certain crappy BIOSes forget to set it properly.
 *
 *  
 */

static void __init quirk_pci_bridge(struct pci_dev *dev)
{
}	
unsigned int pcibios_assign_all_busses(void)
{
	return 0;
}
void __init pcibios_fixup(void)
{



#if 0
	unsigned long pci_mem_start = (unsigned long) PCI_MEM_START;

	writel(0, PCI_BRIDGE_CONFIG); // set extend byte to 0
	writel(0, SDRAM_MBAR);        // set mbar to 0
	writel(0x2, SDRAM_CMD);       // enable memory accesses
	au_sync_delay(1);

	// set extend byte to mbar of ext slot
	writel(((pci_mem_start >> 24) & 0xff) |
	       (1 << 8 | 1 << 9 | 1 << 10 | 1 << 27), PCI_BRIDGE_CONFIG);
	DBG("Set bridge config to %x\n", readl(PCI_BRIDGE_CONFIG));
#endif
}

#define PCI_SLOT_NUMBER 4

struct res_req {
	struct pci_dev *dev;
	struct resource *res;
	u32 size;
};

struct res_req mem_space_size[PCI_SLOT_NUMBER*8*6];
struct res_req io_space_size[PCI_SLOT_NUMBER*8*6];

static void bubble_sort(struct res_req *array, int num)
{
	int i, j;
	struct res_req tmp;  
	for (i=0;i<num-1;i++) {
		for (j=i;j<num-1;j++) {
			if (array[j].size>array[j+1].size) {
				tmp = array[j];
				array[j]=array[j+1];
				array[j+1]=tmp;	
			};
		};
	};
}


int rtl_pci_assign_resource()
{
	struct pci_dev *dev;
	u16 flags=0;
	u8 tmp8;
	u16 tmp16;
	u32 reg,pos,sz;
	u32 l,i, gpio=0, tmp;
	u32 base_addr,mem_start,io_start;
	  u32 mem_idx=0, io_idx=0, offset;
	mem_start = PCI_SLOT_MEM_BASE&0x1FFFFFFF;
	io_start  = PCI_SLOT_IO_BASE&0x1FFFFFFF;

	memset(mem_space_size,0, sizeof(mem_space_size));
	memset(io_space_size,0, sizeof(io_space_size));
	
	//collect resources.
	pci_for_each_dev(dev) {
		if((dev->devfn>>3)>=sizeof(rtl865x_pcicard_slotaddr)/sizeof(struct pci_slot_baseaddr_s)){
			printk("Conf space base addr for %d th device unknown, skip this device\n", (dev->devfn>>3));
			continue;
		}
		if(!gpio&&rtl865x_pcicard_slotaddr[dev->devfn>>3].slotNum>=2){
			//slot 2, 3 are used. Must config these shared GPIO pins for PCI or
			//PCI2 & PCI3 won't work
			printk("PCI: PCI slot 2/3 inuse. Config GPIOA pins for PCI use\n");
			REG32(0xbd012054)|=0xf0000000;
			REG32(0xbd01200c)|=0xf0000000;
			gpio=1;
		}
		
		for (pos=0;pos<6;pos++) {
			struct resource *res=&dev->resource[pos];	
			sz = res->end-res->start+1;
			if (sz<=1 || sz == 0xffffffff)
				continue;						
			if(IORESOURCE_MEM&res->flags){
		      		mem_space_size[mem_idx].dev=dev;
				mem_space_size[mem_idx].res=res;
				mem_space_size[mem_idx].size = sz;
				mem_idx++;
			}else{
		      		io_space_size[io_idx].dev=dev;
				io_space_size[io_idx].res=res;
				io_space_size[io_idx].size = sz;
				io_idx++;
			}
		}
	}
	//sort by size
	if (mem_idx>1) 
		bubble_sort(mem_space_size, mem_idx);
	if (io_idx>1)  
		bubble_sort(io_space_size, io_idx);
	//cfliu: I don't check total I/O size requested. Maybe too large???

	//assign IO space
	offset = 0;
	for (i=0;i<io_idx;i++) {
		struct pci_dev *thisDev=io_space_size[i].dev;
		struct resource *thisRes=io_space_size[i].res;
		tmp = io_space_size[i].size-1;
		offset = (offset+tmp)&(~tmp);
		pos= thisRes- &thisDev->resource[0];
		reg =  PCI_BASE_ADDRESS_0 + (pos << 2);
		thisRes->start=io_start+offset;
		thisRes->end=thisRes->start+io_space_size[i].size-1;
		rtlpci_write_config_dword(thisDev, reg, thisRes->start);
		offset += io_space_size[i].size;  //next address
	};
	//assign memory space
	offset = 0;
	for (i=0;i<mem_idx;i++) {
		struct pci_dev *thisDev=mem_space_size[i].dev;
		struct resource *thisRes=mem_space_size[i].res;
		tmp = mem_space_size[i].size-1;
		offset = (offset+tmp)&(~tmp);
		pos= thisRes- &thisDev->resource[0];
		reg =  PCI_BASE_ADDRESS_0 + (pos << 2);
		thisRes->start=mem_start+offset;
		thisRes->end=thisRes->start+mem_space_size[i].size-1;
		rtlpci_write_config_dword(thisDev, reg, thisRes->start);
		offset += mem_space_size[i].size;  //next address
	};
	//enable device
	pci_for_each_dev(dev) {
		if((dev->devfn>>3)>=sizeof(rtl865x_pcicard_slotaddr)/sizeof(struct pci_slot_baseaddr_s)){
			printk("Conf space base addr for %d th device unknown, skip this device\n", (dev->devfn>>3));
			continue;
		}	
		rtlpci_write_config_dword(dev, PCI_CONFIG_INT_LINE, 0x04020100);
		//enable cache line size, lantancy
		rtlpci_read_config_dword(dev,PCI_CONFIG_CACHE,&tmp);
		tmp&=0xFFFF0000;
		rtlpci_write_config_dword(dev, PCI_CONFIG_CACHE, tmp|0x0204);//32 byte cache,4 clk latency
		rtlpci_read_config_word(dev,PCI_CONFIG_COMMAND,&tmp16);
		flags|= (u16)tmp16|CMD_PARITY_ERROR_RESPONSE;//|CMD_BUS_MASTER;
	
		dev->irq=ICU_PCI;
		for (pos=0;pos<6;pos++) {
			struct resource *res=&dev->resource[pos];	
			sz = res->end-res->start+1;
			if (sz<=1 || sz == 0xffffffff)
				continue;						
			if (IORESOURCE_MEM&res->flags)
				flags |=CMD_MEM_SPACE;
			else if (IORESOURCE_IO&res->flags)
				flags |=CMD_IO_SPACE;
		}
		rtlpci_write_config_word(dev, PCI_CONFIG_COMMAND, flags);
	}
	return 0;

}


struct pci_fixup pcibios_fixups[] = {
	{ PCI_FIXUP_HEADER, PCI_ANY_ID, PCI_ANY_ID, quirk_pci_bridge },
	{ 0 }
};


//======================================================================================================================================
#else //new driver

/*
 *  arch/mips/realtek/rtl865x/pci.c
 *
 *  Copyright (C) 2004 Hsin-I Wu (hiwu@realtek.com.tw)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */ 



#include <linux/ioport.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/paccess.h>
#include <asm/pci_channel.h>


#include "pci.h"


#define REG8(offset)	(*(volatile unsigned char *)(offset))
#define REG16(offset)	(*(volatile unsigned short *)(offset))
#define REG32(offset)	(*(volatile unsigned long *)(offset))

#define PCI_ILEV 5

#define PCI_SLOT_NUMBER 4
#if (PCI_SLOT_NUMBER < 1) || (PCI_SLOT_NUMBER > 4)
#error "wrong range of PCI_SLOT_NUMBER, rtl865xB should between 1 and 4"
#endif

static int rtl865x_isLinuxIncompliantEndianMode=1;//Default assume 8650B 

static void __init quirk_pci_bridge(struct pci_dev *dev)
{

}

struct pci_fixup pcibios_fixups[] = {	{ PCI_FIXUP_HEADER, PCI_ANY_ID, PCI_ANY_ID, quirk_pci_bridge },	{ 0 }};


#if 0 //8181
#define PCI_SLOT_MEM_BASE 0xbd600000
#define PCI_SLOT_IO_BASE  0xbd500000
#define PCI_MEM_SPACE_SIZE 0x00100000
#define PCI_IO_SPACE_SIZE  0x00100000
#endif

typedef struct BASE_ADDRESS_STRUCT{
  u32 Address;
  enum type_s {
    IO_SPACE=0x80,
    MEM_SPACE
  }Type;
  enum width_s {
    WIDTH32,
    WIDTH64
  }Width;
  u32 Prefetch;
  u32 Size;
}base_address_s;

typedef struct PCI_CONFIG_SPACE_STRUCT{
  u32 Config_Base;  //config space base address
  enum status_s{
    ENABLE,
    DISABLE
  }Status;  //device is enable or disable
  u32 Vendor_Device_ID;
  u8  Revision_ID;
  u32 Class_Code;
  u8  Header_Type;
  base_address_s BAR[6];
  u32 SubSystemVendor_ID;
}pci_config_s;


static pci_config_s *pci_slot[PCI_SLOT_NUMBER][8];

/* scan the resource needed by PCI device */
void scan_resource(pci_config_s *pci_config, int slot_num, int dev_function, u32 config_base)
{
	int i;
	u32 BaseAddr, data;
		
	BaseAddr = config_base+PCI_CONFIG_BASE0;	

    for (i=0;i<6;i++) {  //detect resource usage
    	REG32(BaseAddr) = 0xFFFFFFFF;
    	data = REG32(BaseAddr);
    	    	
    	if (data!=0) {  //resource request exist
    	    int j;
    	    if (data&1) {  //IO space
    	    
    	    	pci_config->BAR[i].Type = IO_SPACE;
    	    	    	    	
  	        //scan resource size
#if 0  	        
  	        for (j=2;j<32;j++)
  	            if (data&(1<<j)) break;
  	        if (j<32) pci_config->BAR[i].Size = 1<<j;
  	              else  pci_config->BAR[i].Size = 0;
#else
			pci_config->BAR[i].Size=((~(data&PCI_BASE_ADDRESS_IO_MASK))+1)&0xffff;
#endif
  	        printk("IO Space %i, data=0x%x size=0x%x \n",i,data,pci_config->BAR[i].Size);
  	  
    	    } else {  //Memory space
    	    	
    	    	pci_config->BAR[i].Type = MEM_SPACE;
    	    	//bus width
    	    	if ((data&0x0006)==4) pci_config->BAR[i].Width = WIDTH64; //bus width 64
    	    	else pci_config->BAR[i].Width = WIDTH32;  //bus width 32
    	    	//prefetchable
    	    	if (data&0x0008) pci_config->BAR[i].Prefetch = 1; //prefetchable
    	    	else pci_config->BAR[i].Prefetch = 0;  //no prefetch
  	        //scan resource size
  	        if (pci_config->BAR[i].Width==WIDTH32) {
  	          for (j=4;j<32;j++)
  	            if (data&(1<<j)) break;
  	          if (j<32) pci_config->BAR[i].Size = 1<<j;
  	          else  pci_config->BAR[i].Size = 0;  	          
  	        } else //width64 is not support
  	          {
  	          	pci_config->BAR[i].Size = 0;
  	          }
  	        printk("Memory Space %i data=0x%x size=0x%x\n",i,data,pci_config->BAR[i].Size);	
    	    }
    	} else {  //no resource
    	    memset(&(pci_config->BAR[i]), 0, sizeof(base_address_s));    		
    	}
	BaseAddr += 4;  //next base address
    }
    
}

/* scan the PCI bus, save config information into pci_slot,
   return the number of PCI functions */
int rtl_pci_scan_slot(void)
{
    u32 config_base, data, vendor_device;
    int i, dev_function;
    int dev_num = 0;
    u16 device,vendor;
	
	for (i=0;i<PCI_SLOT_NUMBER;i++) {  //probe 2 pci slots
#ifndef CONFIG_RTL865XB_PCI_SLOT0
		if(i==0) continue;
#endif
#ifndef CONFIG_RTL865XB_PCI_SLOT1
		if(i==1) continue;
#endif
#ifndef CONFIG_RTL865XB_PCI_SLOT2
		if(i==2) continue;
#endif
#ifndef CONFIG_RTL865XB_PCI_SLOT3
		if(i==3) continue;
#endif


   		// this delay is necessary to 
   		// work around pci configuration timing issue       		
   	   	config_base = 0;

   	   	while(config_base < 3000000){
   	   		config_base++;
   	   	}   
       	   	
    	switch(i){	
       	case 0:      	   	       	   	
           	config_base = PCI_SLOT0_CONFIG_BASE;           
           	break;

        case 1:
           	config_base = PCI_SLOT1_CONFIG_BASE;
           	break;
           	
       	case 2:      	   	       	   	
           	config_base = PCI_SLOT2_CONFIG_BASE;           
           	break;
           	
       	case 3:      	   	       	   	
           	config_base = PCI_SLOT3_CONFIG_BASE;           
           	break;
           	
       	default:
       		return 0;
		}
      
      dev_function=0;


//  	  printk("timeout=%x base=%x\n",REG32(0xbd012004),config_base+PCI_CONFIG_VENDER_DEV);
//  	  printk("timeout=%x\n",REG32(0xbd012004));
	  vendor_device=REG32(config_base+PCI_CONFIG_VENDER_DEV);
  	  vendor=vendor_device&0xffff;
  	  device=vendor_device>>16;

 	  
      while (dev_function<8) {  //pci device exist

	  switch(vendor)
	  {
	  	case 0x17a0: //Genesys
	      	if(!((dev_function==0)||(dev_function==3)) )
      		{
      			goto next_func;
      		}
	  		printk("Found Genesys USB 2.0 PCI Card!, function=%d!\n",dev_function);
	      	break;
	      	
		case PCI_VENDOR_ID_AL: //ALi
		    if(dev_function>3)
      		{
      			goto next_func;
      		}
	  		printk("Found ALi USB 2.0 PCI Card, function=%d!\n",dev_function);			      	
	      	break;
	
		case PCI_VENDOR_ID_VIA: //0x1106
	      	if(dev_function>2)
      		{
      			goto next_func;
      		}
	  		printk("Found VIA USB 2.0 PCI Card!, function=%d!\n",dev_function);			      	
	      	break;

		case PCI_VENDOR_ID_NEC: //0x1033
	      	if(dev_function>2)
      		{
      			goto next_func;
      		}
	  		printk("Found NEC USB 2.0 PCI Card, function=%d!\n",dev_function);			      	
	      	break;

		case PCI_VENDOR_ID_REALTEK:
			if(dev_function>0) ////assume realtek's chip only have one function.
			{
				if((device==0x8139)||(device==0x8185))
				{
					goto next_func;
				}
			}
			if(device==0x8139)
		  		printk("Found Realtek 8139 PCI Card, function=%d!\n",dev_function);
			else if(device==0x8185)
		  		printk("Found Realtek 8185 PCI Card, function=%d!\n",dev_function);						
			break;

		case 0x168c: // Atheros
			if(dev_function>0) //assume atheros's chip only have one function.
			{
				//if(device==0x0013)
				{
					goto next_func;
				}
			}
			if(device==0x0013)			
				printk("Found Atheros AR5212 802.11a/b/g PCI Card, function=%d!\n",dev_function);
			else if(device==0x0012)
				printk("Found Atheros AR5211 802.11a/b PCI Card, function=%d!\n",dev_function);				
			else
				printk("Found Atheros unknow PCI Card, function=%d!\n",dev_function);				
			break;
	      	
		case PCI_VENDOR_ID_PROMISE: //0x105a
	      	if(dev_function>0)
      		{
next_func:     
      			dev_function++;
      			config_base += 256;
      			continue;
      		}
	  		printk("Found Promise IDE PCI Card, function=%d!\n",dev_function);			      	
	      	break;

      	default:
      		break;
	  	}

   		  if ((0==REG32(config_base+PCI_CONFIG_VENDER_DEV))||
   		  	  ((REG32(config_base+PCI_CONFIG_VENDER_DEV)&0xffff)==0xffff))
   		  { 		  
   		  	
		  	dev_function++;
        	  config_base += 256;  //next function's config base
        	  continue;
   		 }   		  

          pci_slot[i][dev_function] = kmalloc(sizeof(pci_config_s),GFP_KERNEL);
          pci_slot[i][dev_function]->Config_Base = config_base;
          pci_slot[i][dev_function]->Status = DISABLE;
          pci_slot[i][dev_function]->Vendor_Device_ID = REG32(config_base+PCI_CONFIG_VENDER_DEV);
          data = REG32(config_base+PCI_CONFIG_CLASS_REVISION);
          pci_slot[i][dev_function]->Revision_ID = data&0x00FF;
          pci_slot[i][dev_function]->Class_Code = data>>8;
          pci_slot[i][dev_function]->Header_Type = (REG32(config_base+PCI_CONFIG_CACHE)>>16)&0x000000FF;
          pci_slot[i][dev_function]->SubSystemVendor_ID = REG32(config_base+PCI_CONFIG_SUBSYSTEMVENDOR);
          scan_resource(pci_slot[i][dev_function], i, dev_function, config_base);  //probe resource request
          	
          printk("PCI device exists: slot %d function %d VendorID %x DeviceID %x %x\n", i, dev_function,           	
              pci_slot[i][dev_function]->Vendor_Device_ID&0x0000FFFF,
              pci_slot[i][dev_function]->Vendor_Device_ID>>16,config_base);
          dev_num++;
          dev_function++;
          ////config_base += dev_function*64*4;  //next function's config base
          config_base += 256;  //next function's config base
//          if (!(pci_slot[i][0]->Header_Type&0x80)) break;  //single function card
          if (dev_function>=8) break;  //only 8 functions allow in a PCI card  
      }      
    }	
    return dev_num;
}

/* sort resource by its size */
void bubble_sort(int space_size[PCI_SLOT_NUMBER*8*6][2], int num)
{
  int i, j;
  int tmp_swap[2];
  
    for (i=0;i<num-1;i++) {
    	for (j=i;j<num-1;j++) {
          if (space_size[j][1]<space_size[j+1][1]) {
            tmp_swap[0] = space_size[j][0];
            tmp_swap[1] = space_size[j][1];
            space_size[j][0] = space_size[j+1][0];
            space_size[j][1] = space_size[j+1][1];
            space_size[j+1][0] = tmp_swap[0];
            space_size[j+1][1] = tmp_swap[1];
          };
        };
    };
}



int rtl_pci_reset(){
	u32 tmp, divisor, scr;	
    tmp = REG32(0xbc803000)&0xFCFF8FFF; 
    REG32(0xbc803004)=0;
    scr=(REG32(0xbc803004)>>16)&7;
    if(scr==3)
	   divisor=2;
    else{
		u32 ratio[]={5,5,4,5,4,4,3,2};//check Datasheet SCR and MACCR.
		REG32(0xbc803000) = tmp|(1<<15);//reset PCI bridge
		divisor=ratio[scr];
    }
	//patch: set PCI clk = /6
	//divisor=5; 
    
    REG32(0xbc803000) = tmp|(divisor<<12);
	return divisor;
}

/* scan pci bus, assign Memory & IO space to PCI card, 
	and call init_XXXX to enable & register PCI device
	rtn 0:ok, else:fail */
int rtl_pci_init(void)
{
  	int function_num;
	u32 tmp, divisor, scr;	
	printk("NEW PCI Driver...\n");

	rtl_pci_reset();	

#if 1
	printk("SLOT 0: %x\n", REG32(PCI_SLOT0_CONFIG_BASE));
	printk("SLOT 1: %x\n", REG32(PCI_SLOT1_CONFIG_BASE));
	printk("SLOT 2: %x\n", REG32(PCI_SLOT2_CONFIG_BASE));
	printk("SLOT 3: %x\n", REG32(PCI_SLOT3_CONFIG_BASE));
	printk("REG32(0xbd012004)&0x10000=%x\n",REG32(0xbd012004)&0x10000);
#endif

  	
    //printk("Probe PCI Bus : There must be one device at the slot.\n");
    
    memset(pci_slot, 0, 4*PCI_SLOT_NUMBER*8);

    function_num = rtl_pci_scan_slot();

    if (function_num==0) {
    	printk("No PCI device exist!!\n");
    	return -1;
    };
	
    //auto assign resource
    if (rtl_pci_assign_resource()) {
    	printk("PCI Resource assignment failed!\n");
    	return -2;
    };


    printk("Find Total %d PCI function\n", function_num);
#if 1
	printk("SLOT 0: %x\n", REG32(PCI_SLOT0_CONFIG_BASE));
	printk("SLOT 1: %x\n", REG32(PCI_SLOT1_CONFIG_BASE));
	printk("SLOT 2: %x\n", REG32(PCI_SLOT2_CONFIG_BASE));
	printk("SLOT 3: %x\n", REG32(PCI_SLOT3_CONFIG_BASE));
	printk("REG32(0xbd012004)&0x10000=%x\n",REG32(0xbd012004)&0x10000);
#endif	
    return 0;
      
}


static int rtlpci_read_config_byte(struct pci_dev *dev, int where, u8 *value)
{
	u32 tmp;
	u32 addr;
	u32 shift=3-(where & 0x3);	
	
	
	if(PCI_FUNC(dev->devfn) >= 8) return PCIBIOS_FUNC_NOT_SUPPORTED;
	if(PCI_SLOT(dev->devfn) >= PCI_SLOT_NUMBER) return PCIBIOS_FUNC_NOT_SUPPORTED;

	if(pci_slot[PCI_SLOT(dev->devfn)][PCI_FUNC(dev->devfn)] == NULL)
		return PCIBIOS_FUNC_NOT_SUPPORTED;

	if(pci_slot[PCI_SLOT(dev->devfn)][PCI_FUNC(dev->devfn)]->Status != ENABLE)
		return PCIBIOS_FUNC_NOT_SUPPORTED;
			
	addr = pci_slot[PCI_SLOT(dev->devfn)][PCI_FUNC(dev->devfn)]->Config_Base;
	
	if(addr == 0) return PCIBIOS_FUNC_NOT_SUPPORTED;
	
	if (dev->bus->number != 0)
	{
		*value = 0;
		return PCIBIOS_SUCCESSFUL;
	}

	addr |= (where&~0x3);
	

	tmp = REG32(addr);
	tmp = cpu_to_le32(tmp);
	for (addr=0;addr<shift;addr++)
		tmp = tmp >>8;

	*value = (u8)tmp;
	
	return PCIBIOS_SUCCESSFUL;
}

static int rtlpci_read_config_word(struct pci_dev *dev, int where, u16 *value)
{
	
	u32 tmp,addr;

	if(PCI_FUNC(dev->devfn) >= 8) return PCIBIOS_FUNC_NOT_SUPPORTED;
	if(PCI_SLOT(dev->devfn) >= PCI_SLOT_NUMBER) return PCIBIOS_FUNC_NOT_SUPPORTED;

	if(pci_slot[PCI_SLOT(dev->devfn)][PCI_FUNC(dev->devfn)] == NULL) 
		return PCIBIOS_FUNC_NOT_SUPPORTED;

	if(pci_slot[PCI_SLOT(dev->devfn)][PCI_FUNC(dev->devfn)]->Status != ENABLE)
		return PCIBIOS_FUNC_NOT_SUPPORTED;

	addr = pci_slot[PCI_SLOT(dev->devfn)][PCI_FUNC(dev->devfn)]->Config_Base;
	
	if(addr == 0) return PCIBIOS_FUNC_NOT_SUPPORTED;

	if (dev->bus->number != 0)
	{
		*value = 0;
		return PCIBIOS_SUCCESSFUL;
	}

	addr |= (where&~0x3);
	

	tmp = REG32(addr);
	if (where&0x2)
		*value = (u16)tmp>>16 ;
	else
		*value = (u16)(tmp);

	return PCIBIOS_SUCCESSFUL;
}

static int rtlpci_read_config_dword(struct pci_dev *dev, int where, u32 *value)
{
	u32 addr;


	if(PCI_FUNC(dev->devfn) >= 8) return PCIBIOS_FUNC_NOT_SUPPORTED;

	if(PCI_SLOT(dev->devfn) >= PCI_SLOT_NUMBER) return PCIBIOS_FUNC_NOT_SUPPORTED;

	if(pci_slot[PCI_SLOT(dev->devfn)][PCI_FUNC(dev->devfn)] == NULL)
		return PCIBIOS_FUNC_NOT_SUPPORTED;
		
	if(pci_slot[PCI_SLOT(dev->devfn)][PCI_FUNC(dev->devfn)]->Status != ENABLE)
		return PCIBIOS_FUNC_NOT_SUPPORTED;		

	addr = pci_slot[PCI_SLOT(dev->devfn)][PCI_FUNC(dev->devfn)]->Config_Base;
	

	if(addr == 0) return PCIBIOS_FUNC_NOT_SUPPORTED;

		
	if (dev->bus->number != 0)
	{
		*value=0;
		return PCIBIOS_SUCCESSFUL;
	}

	*value = REG32(addr+where);
	return PCIBIOS_SUCCESSFUL;
}


static int rtlpci_write_config_byte(struct pci_dev *dev, int where, u8 value)
{
	u32 addr;
	
	if(PCI_FUNC(dev->devfn) >= 8) return PCIBIOS_FUNC_NOT_SUPPORTED;
	if(PCI_SLOT(dev->devfn) >= PCI_SLOT_NUMBER) return PCIBIOS_FUNC_NOT_SUPPORTED;
		
	if(pci_slot[PCI_SLOT(dev->devfn)][PCI_FUNC(dev->devfn)] == NULL)
		return PCIBIOS_FUNC_NOT_SUPPORTED;
				
	if(pci_slot[PCI_SLOT(dev->devfn)][PCI_FUNC(dev->devfn)]->Status != ENABLE)
		return PCIBIOS_FUNC_NOT_SUPPORTED;				
				
	addr = pci_slot[PCI_SLOT(dev->devfn)][PCI_FUNC(dev->devfn)]->Config_Base;
	if(addr == 0) return PCIBIOS_FUNC_NOT_SUPPORTED;

	if (dev->bus->number != 0)		
		return PCIBIOS_SUCCESSFUL;
	REG8(addr+where)=value;
		return PCIBIOS_SUCCESSFUL;
}

static int rtlpci_write_config_word(struct pci_dev *dev, int where, u16 value)
{
	u32 addr;
	
	if(PCI_FUNC(dev->devfn) >= 8) return PCIBIOS_FUNC_NOT_SUPPORTED;
	if(PCI_SLOT(dev->devfn) >= PCI_SLOT_NUMBER) return PCIBIOS_FUNC_NOT_SUPPORTED;	
	
	if(pci_slot[PCI_SLOT(dev->devfn)][PCI_FUNC(dev->devfn)] == NULL)
		return PCIBIOS_FUNC_NOT_SUPPORTED;

	if(pci_slot[PCI_SLOT(dev->devfn)][PCI_FUNC(dev->devfn)]->Status != ENABLE)
		return PCIBIOS_FUNC_NOT_SUPPORTED;
				
	addr = pci_slot[PCI_SLOT(dev->devfn)][PCI_FUNC(dev->devfn)]->Config_Base;
	
	if(addr == 0) return PCIBIOS_FUNC_NOT_SUPPORTED;
	
	if (dev->bus->number != 0)		
		return PCIBIOS_SUCCESSFUL;
	REG16(addr+where)=value;
		return PCIBIOS_SUCCESSFUL;
}

static int rtlpci_write_config_dword(struct pci_dev *dev, int where, u32 value)
{
	u32 addr;

	if(PCI_FUNC(dev->devfn) >= 8) return PCIBIOS_FUNC_NOT_SUPPORTED;
	if(PCI_SLOT(dev->devfn) >= PCI_SLOT_NUMBER) return PCIBIOS_FUNC_NOT_SUPPORTED;
	
	if(pci_slot[PCI_SLOT(dev->devfn)][PCI_FUNC(dev->devfn)] == NULL)
		return PCIBIOS_FUNC_NOT_SUPPORTED;
		
	if(pci_slot[PCI_SLOT(dev->devfn)][PCI_FUNC(dev->devfn)]->Status != ENABLE)
		return PCIBIOS_FUNC_NOT_SUPPORTED;		
				
	addr = pci_slot[PCI_SLOT(dev->devfn)][PCI_FUNC(dev->devfn)]->Config_Base;
	
	if(addr == 0) return PCIBIOS_FUNC_NOT_SUPPORTED;
	
	if (dev->bus->number != 0)		
		return PCIBIOS_SUCCESSFUL;
	REG32(addr+where)=value;
	return PCIBIOS_SUCCESSFUL;
}


static struct pci_ops pcibios_ops = {
	rtlpci_read_config_byte,
	rtlpci_read_config_word,
	rtlpci_read_config_dword,
	rtlpci_write_config_byte,
	rtlpci_write_config_word,
	rtlpci_write_config_dword
};

#if 1 //865x
/*
#define PCI_IO_START      0x10000000
#define PCI_IO_END        0x1000ffff
#define PCI_MEM_START     0x18000000
#define PCI_MEM_END       0x18ffffff
*/

#define PCI_IO_START      0x1be00000
#define PCI_IO_END        0x1befffff
#define PCI_MEM_START     0x1bf00000
#define PCI_MEM_END       0x1bffffff



#else //8181

#define PCI_IO_START      (0xbd500000 - 0xbd010000)
#define PCI_IO_END        (0xbdfffff - 0xbd010000)
#define PCI_MEM_START     0xbd600000
#define PCI_MEM_END       0xbd6fffff
#endif

static struct resource pci_io_resource = {
	"pci IO space", 
	PCI_IO_START,
	PCI_IO_END,
	IORESOURCE_IO
};

static struct resource pci_mem_resource = {
	"pci memory space", 
	PCI_MEM_START,
	PCI_MEM_END,
	IORESOURCE_MEM
};


struct pci_channel mips_pci_channels[] = {
	{&pcibios_ops, &pci_io_resource, &pci_mem_resource, 0, 1},
	{(struct pci_ops *) NULL, (struct resource *) NULL,
	 (struct resource *) NULL, (int) NULL, (int) NULL}
};


int pcibios_enable_resources(struct pci_dev *dev)
{
	return 0;
}

#if 0
static void memdump(unsigned address, unsigned length) {
	unsigned data, currentaddress, i, j;
	char c;

	i=0;
	printk("Memory dump: start address 0x%8.8x , length %d bytes\n\n\r",address,length);
	printk("Address        Data\n\r");

	while(i<length) {
		currentaddress = address + i;
		data = *(unsigned long*)(currentaddress);
		printk("0x%8.8x:   %8.8x    ",currentaddress,data);
		for(j=0;j<4;j++) {
			c = (data >> 8*j) & 0xFF;
			if(c<0x20 || c>0x7e) {
				printk("\\%3.3d ",(int)c);
			} else {
				printk(" %c   ",c);
			}
		}
		printk("\n\r");
		i += 4;
	}
	printk("\n\rEnd of memory dump.\n\n\r");
}
#endif /* if 0 */

/*
 *  If we set up a device for bus mastering, we need to check the latency
 *  timer as certain crappy BIOSes forget to set it properly.
 *
 *  
 */


unsigned int pcibios_assign_all_busses(void)
{
	return 0;
}


void __init pcibios_fixup_resources(struct pci_dev *dev)
{	
#if 1	
	int i;
	/* Search for the IO base address.. */
	for (i = 0; i < DEVICE_COUNT_RESOURCE; i++) {
		unsigned int io_addr = pci_resource_start(dev, i);
		unsigned int io_size = pci_resource_len(dev, i);
		unsigned int new_io_addr ;
		if (!(pci_resource_flags(dev,i) & IORESOURCE_IO))
			continue;
		// to work around pci card with IO mappping <64K limitation
		io_addr &= 0x000fffff;
		io_addr |= 0x1d500000;

		pci_resource_start(dev, i) = (io_addr - 0x1D010000);
		pci_resource_end(dev, i) = (io_addr + io_size - 0x1D010000 -1 );
		new_io_addr = pci_resource_start(dev, i);
		printk("pcibios_fixup_resources IO form %x to %x\n",io_addr,new_io_addr);
		
		/* Is it already in use? */
		if (check_region (new_io_addr, io_size))
			break;		
	}
#endif 		
}



void __init pcibios_fixup(void)
{
	/* nothing to do here */
}

void __init pcibios_fixup_irqs(void)
{
    struct pci_dev *dev;

	pci_for_each_dev(dev) {
		dev->irq = 5; // fix irq
	}
}

/*	assign memory location to MEM & IO space
	return 0:OK, else:fail */
int rtl_pci_assign_resource(void)
{
  	int i, slot, func, BARnum;
  	int mem_space_size[PCI_SLOT_NUMBER*8*6][2]; //[0]:store device index, [1]:store resource size
  	int io_space_size[PCI_SLOT_NUMBER*8*6][2];  //[0]:store device index, [1]:store resource size
  	int mem_idx, io_idx, total_size, tmp;
  	u16 config_command[PCI_SLOT_NUMBER][8];  	//record config space resource usage, 
  												// 1:use IO, 2:use memory, 3:both

	if(PCI_SLOT_NUMBER>2)
	{
		printk("PCI: PCI slot 2/3 inuse. Config GPIOA pins for PCI use\n");
	  	REG32(0xbd012054)|=0xf0000000;			
  		REG32(0xbd01200c)|=0xf0000000;			
	}  	
  
    memset(mem_space_size, 0, sizeof(mem_space_size));
    memset(io_space_size, 0, sizeof(io_space_size));
    memset(config_command, 0, sizeof(config_command));
    
    //collect resource
    mem_idx = io_idx =0;
    for (slot=0;slot<PCI_SLOT_NUMBER;slot++) {
#ifndef CONFIG_RTL865XB_PCI_SLOT0
		if(slot==0) continue;
#endif
#ifndef CONFIG_RTL865XB_PCI_SLOT1
		if(slot==1) continue;
#endif
#ifndef CONFIG_RTL865XB_PCI_SLOT2
		if(slot==2) continue;
#endif
#ifndef CONFIG_RTL865XB_PCI_SLOT3
		if(slot==3) continue;
#endif



    	
      if (pci_slot[slot][0]==0) continue;  //this slot is null      
      if (pci_slot[slot][0]->Vendor_Device_ID==0) continue;  //this slot is null      
      for (func=0;func<8;func++) {
        if (pci_slot[slot][func]==0) continue;
        pci_slot[slot][func]->Status = ENABLE;
        for (BARnum=0;BARnum<6;BARnum++) {
      		if (pci_slot[slot][func]->BAR[BARnum].Type==MEM_SPACE){  //memory space
      			printk("memory mapping BAnum=%d slot=%d func=%d\n",BARnum,slot,func);
      		    config_command[slot][func] |= CMD_MEM_SPACE;  //this device use Memory
      		    mem_space_size[mem_idx][0] = (slot<<16)|(func<<8)|(BARnum<<0);
      		    mem_space_size[mem_idx][1] = pci_slot[slot][func]->BAR[BARnum].Size;
      		    mem_idx++;
      		} else if (pci_slot[slot][func]->BAR[BARnum].Type==IO_SPACE){  //io space
	      		printk("io mapping BAnum=%d slot=%d func=%d\n",BARnum,slot,func);
      		    config_command[slot][func] |= CMD_IO_SPACE;  //this device use IO
      		    io_space_size[io_idx][0] = (slot<<16)|(func<<8)|(BARnum<<0);
      		    io_space_size[io_idx][1] = pci_slot[slot][func]->BAR[BARnum].Size;
      		    io_idx++;
      		}
      		else
      		{
//      			printk("unknow mapping BAnum=%d slot=%d func=%d\n",BARnum,slot,func);
      		}
	    }  //for (BARnum=0;BARnum<6;BARnum++) 
      }  //for (func=0;func<8;func++)
    } //for (slot=0;slot<PCI_SLOT_NUMBER;slot++)

    //sort by size
    if (mem_idx>1) bubble_sort(mem_space_size, mem_idx);
    if (io_idx>1)  bubble_sort(io_space_size, io_idx);   
    
    //check mem total size
    total_size = 0;
    for (i=0;i<mem_idx;i++) {  	
    	tmp = mem_space_size[i][1]-1;
        total_size = (total_size+tmp)&(~tmp);     
        total_size = total_size + mem_space_size[i][1];
    };
    if (total_size>PCI_MEM_SPACE_SIZE) {
	printk("lack of memory map space resource \n");
	return -1;  //lack of memory space resource
    }

    //check io total size
    total_size = 0;
    for (i=0;i<io_idx;i++) {     	
    	tmp = io_space_size[i][1]-1;
        total_size = (total_size+tmp)&(~tmp);     
        total_size = total_size + io_space_size[i][1];
    }
    
    if (total_size>PCI_IO_SPACE_SIZE) 
    {
	printk("lack of io map space resource\n");
		return -2;  //lack of io space resource
    }
    

    //assign memory space
    total_size = 0;
    for (i=0;i<mem_idx;i++) {
    	unsigned int config_base;     	
    	tmp = mem_space_size[i][1]-1;
        total_size = (total_size+tmp)&(~tmp);        
        tmp = mem_space_size[i][0];
        //assign to struct
        if (pci_slot[(tmp>>16)][(tmp>>8)&0x00FF]->BAR[tmp&0x00FF].Type!= MEM_SPACE)
        	continue;
        
        pci_slot[(tmp>>16)][(tmp>>8)&0x00FF]->BAR[tmp&0x00FF].Address = PCI_SLOT_MEM_BASE+total_size;
     	switch((tmp>>16)){
     	case 0:
     		config_base = PCI_SLOT0_CONFIG_BASE;
     		break;
     	case 1:
     		config_base = PCI_SLOT1_CONFIG_BASE;
     		break;
     	case 2:
     		config_base = PCI_SLOT2_CONFIG_BASE;
     		break;     		
     	case 3:
     		config_base = PCI_SLOT3_CONFIG_BASE;
     		break;     		
     	default:
     		panic("PCI slot assign error");
    	}        
        //get BAR address and assign to PCI device
        tmp = config_base   //SLOT
              +((tmp>>8)&0x00FF)*64*4					//function
              +((tmp&0x00FF)*4+PCI_CONFIG_BASE0);			//BAR bumber
        REG32(tmp) = (PCI_SLOT_MEM_BASE+total_size)&0x1FFFFFFF;  //map to physical address
        
        total_size = total_size + mem_space_size[i][1];  //next address

        printk("assign  mem base %x at %x total_size=%d\n",(PCI_SLOT_MEM_BASE+total_size)&0x1FFFFFFF,tmp,total_size);
    };
    //assign IO space
    total_size = 0;
    for (i=0;i<io_idx;i++) {
    	unsigned int config_base;    	
    	tmp = io_space_size[i][1]-1;
        //total_size = (total_size+tmp)&(~tmp);        
        tmp = io_space_size[i][0];
        //assign to struct
        if (pci_slot[(tmp>>16)][(tmp>>8)&0x00FF]->BAR[tmp&0x00FF].Type!= IO_SPACE)
        	continue;

        pci_slot[(tmp>>16)][(tmp>>8)&0x00FF]->BAR[tmp&0x00FF].Address = PCI_SLOT_IO_BASE+total_size;
        //get BAR address and assign to PCI device
     	switch((tmp>>16)){
     	case 0:
     		config_base = PCI_SLOT0_CONFIG_BASE;
     		break;
     	case 1:
     		config_base = PCI_SLOT1_CONFIG_BASE;
     		break;
     	case 2:
     		config_base = PCI_SLOT2_CONFIG_BASE;
     		break;
     	case 3:
     		config_base = PCI_SLOT3_CONFIG_BASE;
     		break;      		
     	default:
     		panic("PCI slot assign error");
    	}
        tmp = config_base   //SLOT
              +((tmp>>8)&0x00FF)*64*4					//function
              +((tmp&0x00FF)*4+PCI_CONFIG_BASE0);			//BAR bumber
        
        REG32(tmp) = (PCI_SLOT_IO_BASE+total_size)&0x1FFFFFFF;  //map to physical address
		printk("assign I/O base %x~%x at %x total_size=%d\n",(PCI_SLOT_IO_BASE+total_size)&0x1FFFFFFF,((PCI_SLOT_IO_BASE+total_size)&0x1FFFFFFF)+io_space_size[i][1]-1,tmp,total_size);
        total_size += io_space_size[i][1];  //next address
        
    }
    
    //enable device
    for (slot=0;slot<PCI_SLOT_NUMBER;slot++) {

	#ifndef CONFIG_RTL865XB_PCI_SLOT0
			if(slot==0) continue;
	#endif
	#ifndef CONFIG_RTL865XB_PCI_SLOT1
			if(slot==1) continue;
	#endif
	#ifndef CONFIG_RTL865XB_PCI_SLOT2
			if(slot==2) continue;
	#endif
	#ifndef CONFIG_RTL865XB_PCI_SLOT3
			if(slot==3) continue;
	#endif
    	
      if (pci_slot[slot][0]==0) continue;  //this slot is null      
      if (pci_slot[slot][0]->Vendor_Device_ID==0) continue;  //this slot is null      
      for (func=0;func<8;func++) {
      	unsigned int config_base;
        if (pci_slot[slot][func]==0) continue;  //this slot:function is null      
        if (pci_slot[slot][func]->Vendor_Device_ID==0) continue;  //this slot:function is null      
        //get config base address
     	switch(slot){
     	case 0:
     		config_base = PCI_SLOT0_CONFIG_BASE;
     		break;
     	case 1:
     		config_base = PCI_SLOT1_CONFIG_BASE;
     		break;
     	case 2:
     		config_base = PCI_SLOT2_CONFIG_BASE;
     		break;
     	case 3:
     		config_base = PCI_SLOT3_CONFIG_BASE;
     		break;      		
     	default:
     		panic("PCI slot assign error");
    	}        
        
        tmp = config_base   //SLOT
              +func*64*4;					   //function
        //set Max_Lat & Min_Gnt, irq
        //printk("1 REG32(tmp+PCI_CONFIG_INT_LINE) = %x\n",REG32(tmp+PCI_CONFIG_INT_LINE));
		REG32(tmp+PCI_CONFIG_INT_LINE) = 0x40200100|(PCI_ILEV); //IRQ level
		//printk("2 REG32(tmp+PCI_CONFIG_INT_LINE) = %x\n",REG32(tmp+PCI_CONFIG_INT_LINE));

		//enable cache line size, lantancy
		REG32(tmp+PCI_CONFIG_CACHE) = (REG32(tmp+PCI_CONFIG_CACHE)&0xFFFF0000)|0x2004;  //32 byte cache, 20 latency
		
        //set command register
        //REG32(tmp+PCI_CONFIG_COMMAND) = (REG32(tmp+PCI_CONFIG_COMMAND)&0xFFFF0000)|config_command[slot][func]
        //					|CMD_BUS_MASTER|CMD_WRITE_AND_INVALIDATE|CMD_PARITY_ERROR_RESPONSE;
	// this delay is necessary to 
       	// work around pci configuration timing issue
       	config_base = 0;	       	
       	while(config_base < 1000){
       	config_base++;
       	} 

        REG16(tmp+PCI_CONFIG_COMMAND) = config_command[slot][func]
        					|CMD_BUS_MASTER|CMD_PARITY_ERROR_RESPONSE;//|CMD_WRITE_AND_INVALIDATE;
      };
    };
    
    return 0;
}


 u8 rtl865x_pci_ioread8(u32 addr){
 	//For legacy 8650's linux incompliant PCI access endian mode 	
	if(rtl865x_isLinuxIncompliantEndianMode){
		u8 tmp;
		u8 swap[4]={3,2,1,0};
		int diff = swap[addr&0x3];
#ifdef RTL865X_PCI_DEBUG		
		printk("R8 0x%08x->", addr);
#endif
		addr=addr&(~0x3);
#ifdef RTL865X_PCI_DEBUG
		printk("0x%08x=", addr);
#endif
		tmp= *(volatile u8*)(addr+diff);
#ifdef RTL865X_PCI_DEBUG
		printk("0x%02x\n", tmp);
#endif
		return tmp;
	}
	return (*(volatile unsigned char *)(addr));
}

 u16 rtl865x_pci_ioread16(u32 addr){
 	//For legacy 8650's linux incompliant PCI access endian mode 	
	if(rtl865x_isLinuxIncompliantEndianMode){
		u16 tmp;
		int diff=2-(addr&0x3);
#ifdef RTL865X_PCI_DEBUG
		printk("R16 0x%08x->", addr);
#endif
		addr=addr&(~0x3);
#ifdef RTL865X_PCI_DEBUG
		printk("0x%08x=", addr);
#endif
		tmp= *(volatile u16*)(addr+diff);
#ifdef RTL865X_PCI_DEBUG
		printk("0x%04x\n", tmp);
#endif
		return tmp;
	}
	return __ioswab16((*(volatile unsigned short *)(addr)));
}

u32 rtl865x_pci_ioread32(u32 addr){
 	//For legacy 8650's linux incompliant PCI access endian mode 	
	if(rtl865x_isLinuxIncompliantEndianMode){
		u32 tmp;
#ifdef RTL865X_PCI_DEBUG
		printk("R32 0x%08x=", addr);
#endif
		tmp= *(volatile u32*)(addr);
#ifdef RTL865X_PCI_DEBUG
		printk("0x%08x\n", tmp);
#endif
		return tmp;
	}
	return __ioswab32((*(volatile unsigned int *)(addr)));
}


void rtl865x_pci_iowrite8(u32 addr, u8 val){
	if(rtl865x_isLinuxIncompliantEndianMode)
		(*(volatile unsigned char*)(addr))=val;
	else
		(*(volatile unsigned char *)(addr)) = __ioswab8(val);
}

 void rtl865x_pci_iowrite16(u32 addr, u16 val){
	if(rtl865x_isLinuxIncompliantEndianMode)
		(*(volatile unsigned short*)(addr))=val;
	else
		(*(volatile unsigned short *)(addr)) = __ioswab16(val);
}

 void rtl865x_pci_iowrite32(u32 addr, u32 val){
	if(rtl865x_isLinuxIncompliantEndianMode)
		(*(volatile unsigned long*)(addr))=val;
	else
		(*(volatile unsigned long *)(addr)) = __ioswab32(val);
}



#endif //end new driver
