/*
 *  arch/mips/philips/nino/prom.c
 *
 *  Copyright (C) 2001 Steven J. Hill (sjhill@realitydiluted.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *  
 *  Early initialization code for the Philips Nino
 */
#include <linux/config.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <asm/bootinfo.h>
#include <asm/addrspace.h>
#include <asm/page.h>

char arcs_cmdline[CL_SIZE];

#ifdef CONFIG_FB_TX3912
extern unsigned long tx3912fb_paddr;
extern unsigned long tx3912fb_vaddr;
extern unsigned long tx3912fb_size;
#endif

const char *get_system_type(void)
{
	return "Philips Nino";
}

/* Do basic initialization */
void __init prom_init(int argc, char **argv, unsigned long magic, int *prom_vec)
{
	unsigned long mem_size;
#ifdef CONFIG_MTD
	strcpy(arcs_cmdline, "root=/dev/mtdblock4");
#endif
	mips_machgroup = MACH_GROUP_PHILIPS;
	mips_machtype = MACH_PHILIPS_NINO;

#if 0 /* read MCR to get SDRAM size*/
	/* read SDRAM size */
	{
		unsigned int MCRsdram;
#ifndef MCR
#define MCR (0xbd013000)
#endif
#ifndef REG32
#define REG32(reg)                     (*((volatile unsigned int *)(reg)))
#endif
		switch ( MCRsdram = ( REG32( MCR ) & 0x30100800 ) )
		{
			/* SDRAM 16-bit mode */
			case 0x00000000: mem_size =  2<<20; break;
			case 0x10000000: mem_size =  8<<20; break;
			case 0x20000000: mem_size = 16<<20; break;
			case 0x30000000: mem_size = 32<<20; break;
			case 0x30000800: mem_size = 64<<20; break;

			/* SDRAM 32-bit mode */
			case 0x00100000: mem_size =  4<<20; break;
			case 0x10100000: mem_size = 16<<20; break;
			case 0x20100000: mem_size = 32<<20; break;
			case 0x30100000: mem_size = 64<<20; break;
			case 0x30100800: mem_size =128<<20; break;

			case 0x00000800: /* fall thru */
			case 0x10000800: /* fall thru */
			case 0x20000800: /* fall thru */
			default:
				printk( "SDRAM unknown(0x%08X)", MCRsdram ); 
				mem_size = 0;
				break;
		}
		printk( "SDRAM size: %dMB\n", mem_size>>20 );
	}
#else
#ifdef CONFIG_NINO_4MB
	mem_size = 4 << 20;
#elif CONFIG_NINO_8MB
	mem_size = 8 << 20;
#elif CONFIG_NINO_16MB
	mem_size = 16 << 20;
#elif CONFIG_NINO_32MB
	mem_size = 32 << 20;
#endif

	//mem_size = 8 << 20;
	//mem_size = 16 << 20;
	//mem_size = 32 << 20; 
	mem_size = 64 << 20; 
#endif/*0*/
	add_memory_region(0, mem_size, BOOT_MEM_RAM); 
}

void __init prom_free_prom_memory (void)
{
}
