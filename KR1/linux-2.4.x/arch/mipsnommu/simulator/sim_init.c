
#include <asm/io.h>
#include <linux/irq.h>
extern void (*board_timer_setup)(struct irqaction *irq);
extern void sim_time_init(void *);
extern void init_timer_and_rtc(void *);

//extern unsigned long mips_io_port_base;

void simulator_setup()
{
	mips_io_port_base	= 0xa0000000;		// for simulator
	
	//board_timer_setup	= init_timer_and_rtc;	// for board
	return;
}
