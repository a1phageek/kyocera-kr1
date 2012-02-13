/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Stub IDE routines to keep Linux from crashing on machine which don't
 * have IDE like the Indy.
 *
 * Copyright (C) 1998, 1999 by Ralf Baechle
 */
#include <linux/hdreg.h>
#include <linux/kernel.h>
#include <linux/ide.h>
#include <asm/hdreg.h>
#include <asm/ptrace.h>
#include "../realtek/rtl865x/pci.h"
#define REG32(reg)	(*(volatile unsigned int *)(reg))
#define PDC20265 0x0d30105a //device ID & vendor ID
extern int rtl865x_isLinuxIncompliantEndianMode;

static int no_ide_default_irq(ide_ioreg_t base)
{
	if(base) return 5;
	return 0;
}

static ide_ioreg_t no_ide_default_io_base(int index)
{
	unsigned int haveIDE=0;	

#ifdef CONFIG_RTL865XB_PCI_SLOT0
	if(!rtl865x_isLinuxIncompliantEndianMode)
	{
		if(cpu_to_le32(REG32(PCI_SLOT0_CONFIG_BASE))==PDC20265) haveIDE=PCI_SLOT0_CONFIG_BASE;
	}
	else
	{		
		if(REG32(PCI_SLOT0_CONFIG_BASE)==PDC20265) haveIDE=PCI_SLOT0_CONFIG_BASE;	
	}
#endif

#ifdef CONFIG_RTL865XB_PCI_SLOT1
	if(!rtl865x_isLinuxIncompliantEndianMode)
	{
		if(cpu_to_le32(REG32(PCI_SLOT1_CONFIG_BASE))==PDC20265) haveIDE=PCI_SLOT1_CONFIG_BASE;
	}
	else
	{
		if(REG32(PCI_SLOT1_CONFIG_BASE)==PDC20265) haveIDE=PCI_SLOT1_CONFIG_BASE;	
	}
#endif

#ifdef CONFIG_RTL865XB_PCI_SLOT2
	if(!rtl865x_isLinuxIncompliantEndianMode)
	{
		if(cpu_to_le32(REG32(PCI_SLOT2_CONFIG_BASE))==PDC20265) haveIDE=PCI_SLOT2_CONFIG_BASE;
	}
	else
	{
		if(REG32(PCI_SLOT2_CONFIG_BASE)==PDC20265) haveIDE=PCI_SLOT2_CONFIG_BASE;	
	}
#endif

#ifdef CONFIG_RTL865XB_PCI_SLOT3
	if(!rtl865x_isLinuxIncompliantEndianMode)
	{
		if(cpu_to_le32(REG32(PCI_SLOT3_CONFIG_BASE))==PDC20265) haveIDE=PCI_SLOT3_CONFIG_BASE;
	}
	else
	{
		if(REG32(PCI_SLOT3_CONFIG_BASE)==PDC20265) haveIDE=PCI_SLOT3_CONFIG_BASE;	
	}
#endif

	if(haveIDE)
	{
		if(!rtl865x_isLinuxIncompliantEndianMode)
		{
			switch (index) {
				case 0:	return cpu_to_le32(REG32(haveIDE+0x10))&0xfffffffc;
				case 1:	return cpu_to_le32(REG32(haveIDE+0x18))&0xfffffffc;
				default:			
					return 0;
			}
		}
		else
		{
			switch (index) {
				case 0:	return REG32(haveIDE+0x10)&0xfffffffc;
				case 1:	return REG32(haveIDE+0x18)&0xfffffffc;
				default:			
					return 0;
			}			
		}
	}
	return 0; //only for primany ide controller
}

static void no_ide_init_hwif_ports (hw_regs_t *hw, ide_ioreg_t data_port,
                                    ide_ioreg_t ctrl_port, int *irq)
{

	ide_ioreg_t reg = data_port;
	int i;
	if(data_port)
	{
		for (i = IDE_DATA_OFFSET; i <= IDE_STATUS_OFFSET; i++) {
			hw->io_ports[i] = reg;
			reg ++;
		}
		if (ctrl_port) {
			hw->io_ports[IDE_CONTROL_OFFSET] = ctrl_port;
		} else {	
			hw->io_ports[IDE_CONTROL_OFFSET] = hw->io_ports[IDE_DATA_OFFSET] + 0x80;
		}
		if (irq != NULL)
			*irq = 0;
		hw->io_ports[IDE_IRQ_OFFSET] = 0;
	}
}

struct ide_ops no_ide_ops = {
	&no_ide_default_irq,
	&no_ide_default_io_base,
	&no_ide_init_hwif_ports
};
