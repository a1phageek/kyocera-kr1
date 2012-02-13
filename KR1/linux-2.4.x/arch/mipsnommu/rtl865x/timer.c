#include <linux/types.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/ptrace.h>
#include <linux/serialP.h>
#include <linux/serial_reg.h>
#include <asm/serial.h>



#include "rtl865x.h"
#include "asm/rtl865x/interrupt.h"
extern struct irqaction timer_irq;
struct irqaction lbc_irq = {
	name: "lbc",
	flags:SA_INTERRUPT,
};



extern  void do_LBC(int irq, void *dev_id, struct pt_regs *regs);

static __inline__ unsigned int serial_in(struct async_struct *info, int offset)
{
	return readb((unsigned long) info->iomem_base +
		     (offset<<info->iomem_reg_shift));
}

static __inline__ void serial_out(struct async_struct *info, int offset,
				  int value)
{
	writeb(value, (unsigned long) info->iomem_base +
	       (offset<<info->iomem_reg_shift));
}


extern struct async_struct kdb_port_info ;
extern void set_async_breakpoint(unsigned int );


unsigned int GetSysClockRate()
{
	unsigned int SysClkRate;
	
    REG32(MACMR) = 0;
    switch ( REG32(MACMR) & SYS_CLK_MASK )
    {
        case SYS_CLK_100M:
            SysClkRate = 100000000;
            break;
        case SYS_CLK_90M:
            SysClkRate = 90000000;
            break;
        case SYS_CLK_85M:
            SysClkRate = 85000000;
            break;
        case SYS_CLK_96M:
            SysClkRate = 96000000;
            break;
        case SYS_CLK_80M:
            SysClkRate = 80000000;
            break;
        case SYS_CLK_75M:
            SysClkRate = 75000000;
            break;
        case SYS_CLK_70M:
            SysClkRate = 70000000;
            break;
        case SYS_CLK_50M:
            SysClkRate = 50000000;
            break;
        default:
            while(1);
    }

    return SysClkRate;
}


void enable_lx4180_irq(int irq);
static void timer_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
	/* Clear the interrupt. */
	int val;
	int status;
	REG32(TCIR) |= TCIR_TC0IP;
#ifdef CONFIG_REMOTE_DEBUG
	/* call the generic one */
	if (kdb_port_info.state) 
	{ 	/* need to init device first */
		status = serial_in(&kdb_port_info,UART_LSR);
		if (status)
		{
			if (status & (UART_LSR_BI | UART_LSR_FE))
				serial_out(&kdb_port_info,UART_LSR_BI,status);
			else
			{
				val=serial_in(&kdb_port_info, UART_RX);
				if(val ==(int)'\003')
				set_async_breakpoint(read_32bit_cp0_register(CP0_EPC));
			}
		}
	}
#endif
	do_timer(regs);
}
void  setup_timer (void)
{
	if ( IS_865XB() )
		REG32(CDBR)=(DIVISOR) << DIVF_OFFSET; // 50B
	else
		REG32(CDBR)=(DIVISOR-1) << DIVF_OFFSET; // 51
    //REG32(TC0DATA) = (((CPU_CLOCK_RATE/DIVISOR)/HZ)+1) << TCD_OFFSET;	
	REG32(TC0DATA) = (((GetSysClockRate()/DIVISOR)/HZ)+1) << TCD_OFFSET;	
	REG32(TCCNR) = TC0EN | TC0MODE_TIMER;
	REG32(TCIR)=TCIR_TC0IE;
	lbc_irq.handler = do_LBC;
	setup_irq(ICU_LBCT, &lbc_irq);
	enable_lx4180_irq(ICU_LBCT);
	timer_irq.handler = timer_interrupt;
	
	setup_irq(ICU_TMR, &timer_irq);
	enable_lx4180_irq(ICU_TMR);
}
