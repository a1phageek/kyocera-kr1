/*
 * arch/microblaze/kernel/setup.c -- Arch-dependent initialization functions
 *

 *  Copyright (C) 2004	     Brett Boren <borenb@eng.uah.edu>
 *  Copyright (C) 2003	     John Williams <jwilliams@itee.uq.edu.au>
 *  Copyright (C) 2001,2002  NEC Corporation
 *  Copyright (C) 2001,2002  Miles Bader <miles@gnu.org>
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file COPYING in the main directory of this
 * archive for more details.
 *
 * Written by Miles Bader <miles@gnu.org>
 * Microblaze port by John Williams <jwilliams@itee.uq.edu.au>
 * Microblaze command line param handling by Brett Boren <borenb@eng.uah.edu>
 */

#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/bootmem.h>
#include <linux/irq.h>
#include <linux/reboot.h>
#include <linux/personality.h>

#include <asm/irq.h>

#include "mach.h"

extern void paging_init(void);

#ifndef CONFIG_REGISTER_TASK_PTR
struct task_struct *current;
#endif

/* These symbols are all defined in the linker map to delineate various
   statically allocated regions of memory.  */

// extern char _intv_start, _intv_end;
/* `kram' is only used if the kernel uses part of normal user RAM.  */
extern char _kram_start __attribute__ ((__weak__));
extern char _kram_end __attribute__ ((__weak__));

/* erase following when we port to 2.6, include <asm/sections.h> instead */
extern char _text[], _stext[], _etext[];
extern char _data[], _sdata[], _edata[];

char saved_command_line[512];
#ifdef CONFIG_MBVANILLA_CMDLINE
/* This pointer gets set with the address of a buffer 
   allocated (or specified) by the bootloader 
*/
/* 
   BAB 1/8/2004 this initialization is needed to keep bootloader_buf_addr
   from being placed in bss and getting cleared in mach_early_setup 
*/
void *bootloader_buf_addr = 0xFFFFFFFF;
#else
char command_line[512];
#endif

void __init setup_arch (char **cmdline)
{
#ifdef CONFIG_MBVANILLA_CMDLINE
	/* 
	  If a cmdline has come from the bootlaoder, then this variable
	  will have been initialised with the addr of the cmdline buffer.
	  Just point the kernel there. 
	*/
	*cmdline = bootloader_buf_addr;

	/* Keep a copy of command line */
	memcpy (saved_command_line, *cmdline, sizeof saved_command_line);
#else
	*cmdline = command_line;

	/* Keep a copy of command line */
	memcpy (saved_command_line, command_line, sizeof saved_command_line);
#endif
	saved_command_line[sizeof saved_command_line - 1] = '\0';

	console_verbose ();

	init_mm.start_code = (unsigned long) &_stext;
	init_mm.end_code = (unsigned long) &_etext;
	init_mm.end_data = (unsigned long) &_edata;
	init_mm.brk = (unsigned long) &_kram_end;

	/* ... and tell the kernel about it.  */
	paging_init ();

	/* do machine-specific setups.  */
	mach_setup (cmdline);
}

void __init trap_init (void)
{
}


static void irq_nop (unsigned irq) { }
static unsigned irq_zero (unsigned irq) { return 0; }

static void nmi_end (unsigned irq)
{
	if (irq != IRQ_NMI (0)) {
		printk (KERN_CRIT "NMI %d is unrecoverable; restarting...",
			irq - IRQ_NMI (0));
		machine_restart (0);
	}
}

static struct hw_interrupt_type nmi_irq_type = {
	"NMI",
	irq_zero,		/* startup */
	irq_nop,		/* shutdown */
	irq_nop,		/* enable */
	irq_nop,		/* disable */
	irq_nop,		/* ack */
	nmi_end,		/* end */
};

void __init init_IRQ (void)
{
	init_irq_handlers (0, NUM_MACH_IRQS, 0);
	//init_irq_handlers (IRQ_NMI (0), NUM_NMIS, &nmi_irq_type);
	mach_init_irqs ();
}
