/***********************************************************************************
$RCSfile: tx39uart.c,v $
Subject : UART Device Driver
$Date: 2003/08/27 06:19:41 $
$Author: rupert $
$Log: tx39uart.c,v $
Revision 1.1.1.1  2003/08/27 06:19:41  rupert
uclinux initial

Revision 1.1.1.1  2003/08/27 03:08:54  rupert
 initial version 

Revision 1.1.1.1  2003/06/11 05:38:10  small
first

Revision 1.11  2002/05/16 05:35:40  colinlee
DEL: remove WRITE_TX39REG(UART_DISTXD, OFF_UARTA_CTL1); in tx39_initialize

Revision 1.10  2002/05/14 09:27:36  colinlee
MOD: fix TX39_CLEAN_UARTA_INT written by vincent

Revision 1.9  2002/04/23 03:39:25  vincent
MOD: Don't touch UARTB when clean UARTA interrupt

Revision 1.8  2002/04/16 03:26:07  vincent
DEL: remove tags of CVS uptate contradictory

Revision 1.7  2002/04/16 02:39:46  vincent
ADD: add tx39uart_use_count variable to deside to free_irq or not
ADD: add TX39_CLEAN_UARTA_INT in tx39_initialize to fix bug when 1st open.

Revision 1.6  2002/04/10 02:34:52  colinlee
ADD: reset UART DMA counter when opening
DEL: remove MOD_IN_USE when closing

Revision 1.5  2002/03/14 08:24:16  colinlee
MOD: replace printk with DEBUGINFO and add #ifdef GV_CONFIG_DEBUG_MESSAG
ADD: set MFIO 23,24 to standard function at console_init, because bootrom DISABLE UART

Revision 1.4  2002/03/11 08:28:27  colinlee
DEL: remove some LED_control

Revision 1.3  2002/03/11 02:34:00  vincent
ADD: add current_baud variable and enable change_speed hardware control marked by Fanky

Revision 1.2  2002/03/07 01:20:05  colinlee
DEL: move DisableUARTA() to usb820d.c

Revision 1.1.1.1  2002/02/06 08:45:32  colinlee
import

Revision 1.3  2002/01/25 09:05:25  colin
*** empty log message ***

Revision 1.2  2002/01/09 01:24:37  colin
add: inmport CVS information

************************************************************************************/


#define SERIAL_INLINE

#include <linux/config.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial.h>
#include <linux/major.h>
#include <linux/string.h>
#include <linux/fcntl.h>
#include <linux/ptrace.h>
#include <linux/ioport.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/serialP.h>
#include <linux/delay.h>
#if 1
#include <linux/console.h>
#endif

#include <asm/system.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <asm/bitops.h>
#include <asm/serial.h>

//#include <asm/tsbref/systemmap.h>
//#include <asm/tsbref/deviceinfo.h>

//#include "u820.h"








/* for lx4180*/


extern struct console dbg_console;
/*
 *	Register console.
 */
//__initfunc (long tx39_console_init(long kmem_start, long kmem_end))
long __init tx39_serial_console_init(long kmem_start, long kmem_end)
{



	
	//unregister_console(&dbg_console);
	return kmem_start;
}

#ifndef	CONFIG_SERIAL_CONSOLE	// DJH : add for debug
void tx39_xxx_write(const char *s, unsigned count)
{
	for(; count; count--)
	{
		if( *s == '\n' )
			uart_putc('\r');
		uart_putc(*s++);
	}
}
#endif
