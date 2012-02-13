/*
 *  arch/mips/philips/nino/irq.c
 *
 *  Copyright (C) 2001 Steven J. Hill (sjhill@realitydiluted.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Interrupt service routines for Philips Nino
 */
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/mipsregs.h>
//#include <asm/rtl8181.h>
#include "rtl865x.h"
//#define ALLINTS (IE_IRQ0 | IE_IRQ1 | IE_IRQ2 | IE_IRQ3 | IE_IRQ4 | IE_IRQ5)
#define ALLINTS (IE_IRQ0 | IE_IRQ2 | IE_IRQ3 | IE_IRQ4 | IE_IRQ5)

static void  unmask_irq(unsigned int irq)
{
	printk("irq %d\n",irq);
	REG32(GIMR) = (REG32(GIMR)) | (1 << (31-irq));
	if (irq==0)
		REG32(IRR)|=1<<(31-irq);
	if (irq==4)
		REG32(IRR)|=3<<(31-irq);
	printk("GIMR %x irr %x\n",REG32(GIMR),REG32(IRR));
}
static void  mask_irq(unsigned int irq)
{
	printk("irq %d\n",irq);
	REG32(GIMR)=(REG32(GIMR)) & (~(1 << (31-irq)));
}


static void enable_irq(unsigned int irq)
{

	return 0;
}

static unsigned int startup_irq(unsigned int irq)
{
	unmask_irq(irq);
	return 0;		/* Never anything pending  */
}

static void disable_irq(unsigned int irq)
{

	return 0;
}

#define shutdown_irq		disable_irq

static void mask_and_ack_irq(unsigned int irq)
{

	return 0;
}
static void end_irq(unsigned int irq)
{
	return 0;
}

static struct hw_interrupt_type irq_type = {
	"MIPS",
	startup_irq,
	shutdown_irq,
	enable_irq,
	disable_irq,
	mask_and_ack_irq,
	end_irq,
	NULL
};

void irq_dispatch(struct pt_regs *regs)
{
	int i;
	volatile unsigned int gimr, gisr,irq_x;  
    
	gimr = REG32(GIMR);
	gisr = REG32(GISR);
	if (gisr & (1<<21)) printk("LBC timeout\n");
	//if (gisr & ~(0x80000000)) printk("gisr %x\n",gisr);
	irq_x = (gimr & gisr);
        for (i=0; i<=7; i++)
    	{
       		if (irq_x & 0x80000000)
		{
			//if (i!=0) printk("i %d\n",i);
			do_IRQ(i, regs);
		}  
        	irq_x = irq_x << 1;
    	}
	//do_IRQ(irq, regs);
}

void __init nino_irq_setup(void)
{
	extern asmlinkage void ninoIRQ(void);

	unsigned int i;
	
	REG32(GIMR)=0;
	REG32(IRR)=0;
	/* Disable all hardware interrupts */
//	change_cp0_status(ST0_IM, 0x00);
//	cil();

	/* Initialize IRQ vector table */
	//init_generic_irq();

	/* Initialize IRQ action handlers */
	for (i = 0; i < 16; i++) {
		hw_irq_controller *handler = NULL;
		handler		= &irq_type;

		irq_desc[i].status	= IRQ_DISABLED;
		irq_desc[i].action	= 0;
		irq_desc[i].depth	= 1;
		irq_desc[i].handler	= handler;
	}

	/* Set up the external interrupt exception vector */
	set_except_vector(0, ninoIRQ);

	/* Enable all interrupts */
//	change_cp0_status(ST0_IM, ALLINTS);
}

void (*irq_setup)(void);

void __init init_IRQ(void)
{
	int flags;
//	printk("\nfile %s cp0 status %x\n",__FILE__,read_c0_status());
	flags = read_c0_status();
	flags |= CAUSEF_IP4|CAUSEF_IP3|CAUSEF_IP5|0xfc00;
	write_c0_status(flags);	
	printk("\nfile %s cp0 status %x\n",__FILE__,read_c0_status());
#ifdef CONFIG_REMOTE_DEBUG
	extern void breakpoint(void);
	extern void set_debug_traps(void);

	printk("Wait for gdb client connection ...\n");
	set_debug_traps();
	breakpoint();
#endif

	/* Invoke board-specific irq setup */
	irq_setup();
}
