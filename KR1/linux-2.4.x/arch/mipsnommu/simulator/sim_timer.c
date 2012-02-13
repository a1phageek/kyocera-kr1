#include <linux/init.h>
#include <linux/interrupt.h>


#include <asm/io.h>
#include <asm/ptrace.h>

#include "tx39.h"
#include "tsbref.h"
#include "lx4180.h"

extern struct irqaction timer_irq;
extern int request_irq(unsigned int, void (*)(int, void *, struct pt_regs *), unsigned long, const char *, void *);
extern void timer_interrupt(int irq, void *dev_id, struct pt_regs *regs);

void lx4180_timer_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
	/* Clear the interrupt. */
	REG32(TCIR) |= TCIR_TC0IP;
	/* call the generic one */
	do_timer(regs);
}



void  setup_timer (void)
{

	REG32(CDBR)=(DIVISOR-1) << DIVF_OFFSET;
	//REG32(TC0DATA) = (((CPU_CLOCK_RATE/DIVISOR)/TICK_FREQ)+1) << TCD_OFFSET;	
	REG32(TC0DATA) = (((GetSysClockRate()/DIVISOR)/TICK_FREQ)+1) << TCD_OFFSET;	
	REG32(TCCNR) = TC0EN | TC0MODE_TIMER;
	REG32(TCIR)=TCIR_TC0IE;
	REG32(IRR2)=0;
	REG8(ILR)=0;

	timer_irq.handler = lx4180_timer_interrupt;

	setup_irq(1, &timer_irq);
	REG16(GIMR) |= Timer_IM;
	REG32(IRR1) |= TICK_ILEV << Timer_IPS;
}
#if 0
void sim_timer_init()
{
	int ret;
	/* rupert*/
	ret=request_irq(1, lx4180_timer_interrupt, 0, "timer0", 0);
	/*ret= request_irq(0, gvpda_counter_interrupt, SA_INTERRUPT, "timer0", 0);
	*/
	printk(__FUNCTION__"() request_irq=%d\n", ret);
}
#endif
void enable_lx4180_irq(int irq)
{
	switch (irq)
	{
		case 0:
			REG16(GIMR) |= UART_IM ;
			REG32(IRR1) |=UART_ILEV << UART_IPS;
			break;

		case 1:
			REG16(GIMR) |= Timer_IM;
			REG32(IRR1) |= TICK_ILEV << Timer_IPS;
		case 4:
			
			REG16(GIMR) |= Ethernet_IM ;
			REG32(IRR1) |=Ethernet_ILEV << Ethernet_IPS;

	}
}
