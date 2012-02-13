/*
 * Copyright 2001 MontaVista Software Inc.
 * Author: Jun Sun, jsun@mvista.com or jsun@junsun.net
 *
 * Modified to be mips generic, ppopov@mvista.com
 * arch/mips/kernl/pci.c
 *     Common MIPS PCI routines.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

/*
 * This file contains common PCI routines meant to be shared for
 * all MIPS machines.
 *
 * Strategies:
 *
 * . We rely on pci_auto.c file to assign PCI resources (MEM and IO)
 *   TODO: this shold be optional for some machines where they do have
 *   a real "pcibios" that does resource assignment.
 *
 * . We then use pci_scan_bus() to "discover" all the resources for
 *   later use by Linux.
 *
 * . We finally reply on a board supplied function, pcibios_fixup_irq(), to
 *   to assign the interrupts.  We may use setup-irq.c under drivers/pci
 *   later.
 *
 * . Specifically, we will *NOT* use pci_assign_unassigned_resources(),
 *   because we assume all PCI devices should have the resources correctly
 *   assigned and recorded.
 *
 * Limitations:
 *
 * . We "collapse" all IO and MEM spaces in sub-buses under a top-level bus
 *   into a contiguous range.
 *
 * . In the case of Memory space, the rnage is 1:1 mapping with CPU physical
 *   address space.
 *
 * . In the case of IO space, it starts from 0, and the beginning address
 *   is mapped to KSEG0ADDR(mips_io_port) in the CPU physical address.
 *
 * . These are the current MIPS limitations (by ioremap, etc).  In the
 *   future, we may remove them.
 *
 * Credits:
 *	Most of the code are derived from the pci routines from PPC and Alpha,
 *	which were mostly writtne by
 *		Cort Dougan, cort@fsmlabs.com
 *		Matt Porter, mporter@mvista.com
 *		Dave Rusling david.rusling@reo.mts.dec.com
 *		David Mosberger davidm@cs.arizona.edu
 */
#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/pci.h>

#include <asm/pci_channel.h>

extern void pcibios_fixup(void);
extern void pcibios_fixup_irqs(void);

#if 0
struct pci_fixup pcibios_fixups[] = {
	{ PCI_FIXUP_HEADER, PCI_ANY_ID, PCI_ANY_ID, pcibios_fixup_resources },
	{ 0 }
};
#endif

extern int pciauto_assign_resources(int busno, struct pci_channel * hose);
extern int rtl_pci_assign_resource();
void __init pcibios_init(void)
{
	struct pci_channel *p;
	struct pci_bus *bus;
	int busno;
	u32 tmp, divisor, scr;
	
#ifdef CONFIG_RTL865X	
	char chipVersion[16];
	int count;
	int rev;
#define REG32(reg) (*(volatile unsigned int *)(reg))	
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
    printk("PCI: PIO/MMIO register endian mode %s Linux io.h, clk div=%x\n",rtl_pci_isLinuxCompliantEndianMode()? "=": "!=", divisor);

#else

	#ifdef CONFIG_PCI_AUTO
		/* assign resources */
		busno=0;
		for (p= mips_pci_channels; p->pci_ops != NULL; p++) {
			busno = pciauto_assign_resources(busno, p) + 1;
		}
	#endif

#endif /*CONFIG_RTL865X*/

	/* scan the buses */
	busno = 0;
	for (p= mips_pci_channels; p->pci_ops != NULL; p++) {
		bus = pci_scan_bus(busno, p->pci_ops, p);
		busno = bus->subordinate+1;
	}

#ifdef CONFIG_RTL865X	
	//assign resources for probed PCI devices.
	rtl_pci_assign_resource();
#else
	/* machine dependent fixups */
	/* fixup irqs (board specific routines) */
	pcibios_fixup_irqs();
#endif
}

extern int pcibios_enable_resources(struct pci_dev *dev);
int pcibios_enable_device(struct pci_dev *dev,int mask)
{
	pcibios_enable_resources(dev);// will enable all devices found */
	return 0;
}

unsigned long __init pci_bridge_check_io(struct pci_dev *bridge)
{
	u16 io;

	pci_read_config_word(bridge, PCI_IO_BASE, &io);
	if (!io) {
		pci_write_config_word(bridge, PCI_IO_BASE, 0xf0f0);
		pci_read_config_word(bridge, PCI_IO_BASE, &io);
		pci_write_config_word(bridge, PCI_IO_BASE, 0x0);
	}
	if (io)
		return IORESOURCE_IO;
	printk(KERN_WARNING "PCI: bridge %s does not support I/O forwarding!\n",
				bridge->name);
	return 0;
}

void __init pcibios_fixup_bus(struct pci_bus *bus)
{
	/* Propogate hose info into the subordinate devices.  */

	struct pci_channel *hose = bus->sysdata;
	struct pci_dev *dev = bus->self;

	if (!dev) {
		/* Root bus */
		bus->resource[0] = hose->io_resource;
		bus->resource[1] = hose->mem_resource;
	} else {
		/* This is a bridge. Do not care how it's initialized,
		   just link its resources to the bus ones */
		int i;

		for(i=0; i<3; i++) {
			bus->resource[i] =
				&dev->resource[PCI_BRIDGE_RESOURCES+i];
			bus->resource[i]->name = bus->name;
		}
		bus->resource[0]->flags |= pci_bridge_check_io(dev);
		bus->resource[1]->flags |= IORESOURCE_MEM;
		/* For now, propogate hose limits to the bus;
		   we'll adjust them later. */
		bus->resource[0]->end = hose->io_resource->end;
		bus->resource[1]->end = hose->mem_resource->end;
		/* Turn off downstream PF memory address range by default */
		bus->resource[2]->start = 1024*1024;
		bus->resource[2]->end = bus->resource[2]->start - 1;
	}
}

char *pcibios_setup(char *str)
{
	return str;
}

void
pcibios_align_resource(void *data, struct resource *res, unsigned long size,unsigned long align)
{
	/* this should not be called */
}

void
pcibios_update_resource(struct pci_dev *dev, struct resource *root,
			struct resource *res, int resource)
{
	/* this should not be called */
}
