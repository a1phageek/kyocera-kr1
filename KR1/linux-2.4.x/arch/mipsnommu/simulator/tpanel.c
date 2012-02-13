/* $Id: tpanel.c,v 1.1.1.1 2003/08/27 06:19:41 rupert Exp $
 *
 * TC35143f Touch Panel Driver
 * by DJH
 *
 * Loosely based on drivers/char/busmouse.c
 */

#include <linux/config.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/poll.h>
#include <linux/miscdevice.h>
#include <linux/random.h>
#include <linux/init.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/irq.h>
#include "tsbref.h"
#include "tpanel.h"
#include "tx39.h"

#define	AFC_TPANEL_ID	0	/* see sib/afc.c */

struct tpanel_struct tpanel;

struct tpanel_buf_struct tbuf[256];

int	tbuf_h;
int	tbuf_t;

int	tpanel_current_flag;

#ifdef  DEBUG
//#if 1
#define DEBUGINFO(a) printk a
#else
#define DEBUGINFO(a)
#endif



#define	AFC_TSC_PENDOWN	(AFC_TSC_TSPXLOW|AFC_TSC_TSMXLOW)

void tpanel_irq_handler(int irq, void *data, struct pt_regs *regs)
{
	static int in_interrupt = 0;
	unsigned long flags;
	struct tpanel_buf_struct *p;
	unsigned long tpanel_reg;

////printk("~");
	if( in_interrupt )
		return;
	in_interrupt = 1;
	save_flags(flags);              // colin : 20010808
        cli();
	p = &tbuf[tbuf_h];

	WRITE_TX39REG(1<<21,OFF_INTR_CLEAR1);
	tpanel_reg	= READ_TX39REG(0x300);
	/* down */
	if(tpanel_reg & 0x80000000)
	{
		p->x = (tpanel_reg >> 10) & 0x3ff;
		p->y = tpanel_reg & 0x3ff;
		p->flag = TPANEL_PEN_DOWN;
		tpanel_current_flag = 1;
		//STANDBY_BIT_ON(TP_STANDBY);
	}
	else
	{
		p->x = 0;
		p->y = 1;
		p->flag = TPANEL_PEN_UP;
		tpanel_current_flag = 0;
		//STANDBY_BIT_OFF(TP_STANDBY);
	}
	if( tpanel_current_flag != 3 )
	{
		tbuf_h++;
		tbuf_h &= 0xFF;
		if( tbuf_h == tbuf_t )
		{
			tbuf_t++; 
			tbuf_t &= 0xFF;
		}
	}

	
	//SIBIRQ_newmask(sibid);/* disable interrupt until timer wakeup */
	wake_up_interruptible(&tpanel.wait);
	in_interrupt = 0;
	restore_flags(flags);
}

static int fasync_tpanel(int fd, struct file *filp, int on)
{
	int retval;

	retval = fasync_helper(fd, filp, on, &tpanel.fasyncptr);
	if (retval < 0)
		return retval;
	return 0;
}

static int close_tpanel(struct inode * inode, struct file * file)
{
	
	fasync_tpanel(-1, file, 0);
	tpanel.active--;
	if(tpanel.active == 0)
	{//jmt:off DeInit(sib),(possible cause problem of other sib modules),20010516
//		AFCDeassignIRQ(tpanel.sib, tpanel.sibirq);
//		AFCDeInit(tpanel.sib);
	}
//	MOD_DEC_USE_COUNT;
	return 0;
}

char start_tp_flag = 0;
static int open_tpanel(struct inode * inode, struct file * file)
{
	start_tp_flag = 1;
	tbuf_h = tbuf_t = 0;
	tpanel_current_flag = 0;
	init_waitqueue_head(&tpanel.wait);
	tpanel.fasyncptr = NULL;
printk("open tpanel\n");
//	MOD_INC_USE_COUNT;
#if 1
	if(tpanel.active > 0)
		return 0;
	tpanel.active++;
	//open_touch_detect_mode(TPANEL_PEN_UP);
#endif
	return 0;
}

/*
 *  Read touch panel data.  
 *  No calibration is done on the driver side, that is expected to be done on
 *  the user side.
 */
#include <linux/mm.h>
static ssize_t read_tpanel(struct file * file, char * buffer, size_t count, loff_t *ppos)
{
	int r;
	unsigned long flags;
	struct tpanel_buf_struct *p;
	if( tbuf_h == tbuf_t )
		return -EAGAIN;
	// buffer too small?
	if (count < 6)
		return -EINVAL;

	// something wrong with the buffer address?
	if (!access_ok(VERIFY_WRITE, buffer, count))
	{
		printk("access buffer error \n");
		return -EFAULT;
	}
	save_flags(flags);
	cli();

	p = &tbuf[tbuf_t];
	__put_user(p->flag, (short*)buffer);

#if 0	 
	__put_user(p->x, (short*)(buffer+2));
	__put_user(p->y, (short*)(buffer+4));
#else	// colin : for PDA product 0612
	__put_user(p->y, (short*)(buffer+2));
	__put_user(p->x, (short*)(buffer+4));
#endif

////printk("tbuf_h=%d, tbuf_t=%d, flag=0x%x,x: %d, y:%d pid=%d\n", tbuf_h, tbuf_t, p->flag, p->x,p->y, current->pid);
	tbuf_t++;
	tbuf_t &= 0xff;
	restore_flags(flags);
	// clear any remaining user buffer area
	for (r=6 ; r < count; r++)
	    __put_user(0x00, buffer + r);
	return r;
}

//static unsigned int poll_tpanel(struct file *file, poll_table * wait)
static int poll_tpanel(struct file *file, poll_table * wait)
{
////printk("polling tpanel\n");
	poll_wait(file, &tpanel.wait, wait);
	if(tbuf_h != tbuf_t)
		return POLLIN | POLLRDNORM;
	return 0;
}


static struct file_operations tpanel_fops = {
	read:		read_tpanel,
	poll:		poll_tpanel,
	open:		open_tpanel,
	release:	close_tpanel,
	fasync:		fasync_tpanel,
};

static int __init tpanel_init(void)
{
	return 0;
	tpanel.active = 0;
	tpanel.sib = 0;//AFCInit(AFC_TPANEL_ID);
	//airq_mask.falmask = 0;
	//airq_mask.rismask = AFC_RISINT_TSPX | AFC_RISINT_TSMX ; 
	//airq_mask.rismask = AFC_RISINT_TSPX | AFC_RISINT_TSMX | AFC_RISINT_ADC;  // colin:20011108
	tpanel.sibirq = 0;//AFCAssignIRQ(tpanel.sib, &airq_mask, &tpanel, tpanel_irq_handler);
	register_chrdev(MISC_MAJOR, "simtpanel", &tpanel_fops);
	if(request_irq(TSBREF_SND_IRQ, tpanel_irq_handler, SA_INTERRUPT, "tpanel", NULL))
	{
		//printk("request irq for tpanel error\n");
	}
	return 0;
}


module_init(tpanel_init);

