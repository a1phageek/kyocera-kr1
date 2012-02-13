#include <linux/fs.h>
#include <linux/iobuf.h>
#include <linux/major.h>
#include <linux/module.h>
#include <linux/blkdev.h>
#include <linux/capability.h>
#include <linux/smp_lock.h>
#include <asm/io.h>
#include <asm/ptrace.h>
#include <asm/uaccess.h>

#include "tx39.h"
#include "tsbref.h"

#define TX39_FB_BASE 0x80E00000

static int fb_open(struct inode *inode, struct file *file)
{
	MOD_INC_USE_COUNT;
	return 0;
}

static int fb_close(struct inode *inode, struct file *file)
{
	MOD_DEC_USE_COUNT;
	return 0;
}

static int fb_mmap(struct file *file, struct vm_area_struct *vm)
{
	return TX39_FB_BASE;
}

static int fb_ioctl(struct inode *inode, struct file *flip, unsigned int command, unsigned long arg)
{
	return -EINVAL;
}

static struct file_operations fb_fops = {
	open:		fb_open,
	release:	fb_close,
	mmap:		fb_mmap,
	ioctl:		fb_ioctl,
};

static void tx39_video_init(ulong addr)
{
	ulong phy_addr;
	ulong addr_baselo;
	unsigned bitsel,videoregs;
	
	
	addr_baselo = (1 + addr + ((int)320 * (int)240 *(int)24) / 8);
	bitsel	= VIDEO_BITSEL_8BPP;
	phy_addr =addr & (0x0fffffff);
    AND_TX39REG( ~(VIDEO_DISPON|VIDEO_ENVID), OFF_VIDEO_CTL1);
    WRITE_TX39REG( (59<<12)|159, OFF_VIDEO_CTL2 );
	videoregs = READ_TX39REG(OFF_VIDEO_CTL1);
    videoregs &= ~VIDEO_BITSELMASK;
    videoregs |= (bitsel|VIDEO_BAUDVAL(9)) ;
    WRITE_TX39REG(videoregs, OFF_VIDEO_CTL1);
    WRITE_TX39REG( VIDEO_VIDBANK(phy_addr)|VIDEO_VIDBASEHI(phy_addr), OFF_VIDEO_CTL3 );
    WRITE_TX39REG(  VIDEO_DFVAL(31)| VIDEO_VIDBASELO(addr_baselo), OFF_VIDEO_CTL4 );
    OR_TX39REG( VIDEO_DISPON|VIDEO_ENVID, OFF_VIDEO_CTL1);
}


static int __init fb_init(void)
{
	tx39_video_init(TX39_FB_BASE);
	register_chrdev(GRAPHDEV_MAJOR, "simvideo", &fb_fops);
	return 0;
}


module_init(fb_init);
