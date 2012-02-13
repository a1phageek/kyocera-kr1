/*
 *  arch/mips/philips/nino/setup.c
 *
 *  Copyright (C) 2001 Steven J. Hill (sjhill@realitydiluted.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Interrupt and exception initialization for Philips Nino
 */
#include <linux/console.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <asm/addrspace.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/reboot.h>
#include <asm/time.h>
#include "rtl865x.h"
// For Interrupt Controller
#define GIMR		0xbd012000
#define GISR		0xbd012004
#define IRR		0xbd012008


static void nino_machine_restart(char *command)
{
	unsigned long flags;
	static void (*back_to_prom)(void) = (void (*)(void)) 0xbfc00000;
	printk("Enable Watch Dog to Reset whole system\n");
	save_flags(flags); cli();
	*(volatile unsigned long *)(0xBD010058)=0x10; // this is to enable watch dog reset
	*(volatile unsigned long *)(0xBD01005c)=0x00; // this is to enable watch dog reset
	for(;;);
	/* Reboot */
	back_to_prom();
}

static void nino_machine_halt(void)
{
	printk("Nino halted.\n");
	while(1);
}

static void nino_machine_power_off(void)
{
	printk("Nino halted. Please turn off power.\n");
	while(1);
}

static void __init nino_board_init()
{
	/*
	 * Set up the master clock module. The value set in
	 * the Clock Control Register by WindowsCE is 0x00432ba.
	 * We set a few values here and let the device drivers
	 * handle the rest.
	 *
	 * NOTE: The UART clocks must be enabled here to provide
	 *       enough time for them to settle.
	 */
}

static __init void nino_time_init(void)
{
}
#define  CPU_CLOCK_RATE 100000000
static __init void nino_timer_setup(struct irqaction *irq)
{
	printk("irqaction %x %s %d\n",irq,__FILE__,__LINE__);	
	irq->dev_id = (void *) irq;
	setup_irq(0, irq);
	REG32(CDBR)=(DIVISOR) << DIVF_OFFSET;
    	REG32(TC0DATA) = (((CPU_CLOCK_RATE/DIVISOR)/HZ)+1) << TCD_OFFSET;	
	REG32(TCCNR) = TC0EN | TC0MODE_TIMER;
	REG32(TCIR)=TCIR_TC0IE;

#if 0
    outl(0x0, TCCNR);
    outl(0, TC0DATA); // what does this counter mean?
    outl(0, TC1DATA); // what does this counter mean?
    outl(0, TC2DATA); // what does this counter mean?
    outl(0, TC3DATA); // what does this counter mean?
    //outl(0x30D40, TC0DATA); // what does this counter mean?
    outl(0x35B60, TC0DATA); // correct setting for 200MHz
    //outl(0x35B6, TC0DATA); // what does this counter mean?
    //outl(0x30D4, TC0DATA); // what does this counter mean?
   
#ifdef CONFIG_RTL8181_WTDOG 
    *(volatile unsigned long *)(0xbd010058)=0xfff; // watch dog time-out for about 12 s
    *(volatile unsigned long *)(0xbd01005c)=0x700;
#endif    
    outl(0x3, TCCNR);
    outl(0x01, TCIR);
#endif

}
void timer_ack()
{
	REG32(TCIR) |= TCIR_TC0IP;
}

/*
 * I/O ASIC systems use a standard writeback buffer that gets flushed
 * upon an uncached read.
 */
static void wbflush_mips(void)
{
//	__fast_iob();
}


void (*__wbflush) (void);
void __init nino_setup(void)
{
	extern void nino_irq_setup(void);
	//clear GIMR first!

	irq_setup = nino_irq_setup;
	set_io_port_base(KSEG1ADDR(0x1d010000));
	//mips_io_port_base=RTL8181_REG_BASE;

	_machine_restart = nino_machine_restart;
	_machine_halt = nino_machine_halt;
	_machine_power_off = nino_machine_power_off;

	board_time_init = nino_time_init;
	board_timer_setup = nino_timer_setup;
	mips_timer_ack=timer_ack;

#ifdef CONFIG_FB
	conswitchp = &dummy_con;
#endif

	__wbflush = wbflush_mips;
	nino_board_init();
}

