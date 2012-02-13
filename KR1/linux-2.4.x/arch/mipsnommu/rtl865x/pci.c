#include <linux/config.h>
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
#include <asm-mipsnommu/rtl865x/interrupt.h>
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
#ifdef CONFIG_PCI
	if(rtl865x_isLinuxIncompliantEndianMode)
		(*(volatile unsigned char*)(addr))=val;
	else
#endif	
		(*(volatile unsigned char *)(addr)) = __ioswab8(val);
}

 void rtl865x_pci_iowrite16(u32 addr, u16 val){
#ifdef CONFIG_PCI
	if(rtl865x_isLinuxIncompliantEndianMode)
		(*(volatile unsigned short*)(addr))=val;
	else
#endif	
		(*(volatile unsigned short *)(addr)) = __ioswab16(val);
}

 void rtl865x_pci_iowrite32(u32 addr, u32 val){
#ifdef CONFIG_PCI
	if(rtl865x_isLinuxIncompliantEndianMode)
		(*(volatile unsigned long*)(addr))=val;
	else
#endif	
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

int pcibios_enable_resources(struct pci_dev *dev)
{
#if CONFIG_RTL865X
	printk("%s:%d: \n   No need to enable again in PCI driver. Already enabled when device probed!!\n",__FUNCTION__, __LINE__);
#else
	u16 cmd, old_cmd;
	int idx;
	struct resource *r;

	pci_read_config_word(dev, PCI_COMMAND, &cmd);
	old_cmd = cmd;
	for(idx=0; idx<6; idx++) {
		r = &dev->resource[idx];
		if (!r->start && r->end) {
			printk(KERN_ERR "PCI: Device %s not available because of resource collisions (start:0x%08x, end: 0x%08x)\n", dev->slot_name, r->start, r->end);
			return -EINVAL;
		}
		if (r->flags & IORESOURCE_IO)
			cmd |= PCI_COMMAND_IO;
		if (r->flags & IORESOURCE_MEM)
			cmd |= PCI_COMMAND_MEMORY;
	}
	if (dev->resource[PCI_ROM_RESOURCE].start)
		cmd |= PCI_COMMAND_MEMORY;
	cmd &= ~PCI_COMMAND_PARITY;
	if (cmd != old_cmd) {
		printk("PCI: Enabling device %s (%04x -> %04x)\n", dev->slot_name, old_cmd, cmd);
		pci_write_config_word(dev, PCI_COMMAND, cmd);
	}
#endif	
	return 0;
	
}


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

