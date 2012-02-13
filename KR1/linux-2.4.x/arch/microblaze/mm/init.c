/*
 *  arch/microblaze/mm/init.c
 *
 *  Copyright (C) 2004 Atmark Techno, Inc.
 *
 *  Yasushi SHOJI <yashi@atmark-techno.com>
 *
 *  Based on
 *        arch/microblaze/setup.c
 *   and
 *        arch/arm/mm/init.c
 *        arch/sparc/mm/init.c
 *        arch/ia64/mm/init.c
 *        arch/m68knommu/kernel/setup.c
 *        arch/h8300/kernel/setup.c
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file COPYING in the main directory of this
 * archive for more details.
 *
 */

#include <linux/mm.h>
#include <linux/bootmem.h>

#include <asm/machdep.h>

/* these function prototypes are taken form arch/microblaze/kernel/mach.h */
extern void mach_reserve_bootmem (void) __attribute__ ((__weak__));
extern void mach_get_physical_ram (unsigned long *ram_start, unsigned long *ram_len);

extern char _kram_start __attribute__ ((__weak__));
extern char _kram_end __attribute__ ((__weak__));

/* erase following when we port to 2.6, include <asm/sections.h> instead */
extern char _text[], _stext[], _etext[];
extern char _data[], _sdata[], _edata[];
extern char __bss_start[], __bss_stop[];
extern char __init_begin[], __init_end[];
extern char _sinittext[], _einittext[];

/* We'll get rid of _bootmap in next patch - yashi Apr 10, 2004 */
extern char _bootmap;

/* Physical System RAM.  */
static unsigned long ram_start = 0, ram_len = 0;

#define ADDR_TO_PAGE_UP(x)   ((((unsigned long)x) + PAGE_SIZE-1) >> PAGE_SHIFT)
#define ADDR_TO_PAGE(x)	     (((unsigned long)x) >> PAGE_SHIFT)
#define PAGE_TO_ADDR(x)	     (((unsigned long)x) << PAGE_SHIFT)

/* Memory not used by the kernel.  */

/* for 2.4, declare totalram_pages here.  for 2.6, totalram_pages is
 * declared in mm/page_alloc.c. remove from here and include
 * linux/swap.h, instead.  - yashi Apr 10, 2004 */
static unsigned long totalram_pages;

void __init mem_init (void)
{
	max_mapnr = MAP_NR (ram_start + ram_len);

	num_physpages = ADDR_TO_PAGE (ram_len);

	totalram_pages = free_all_bootmem ();

	printk (KERN_INFO
		"Memory: %luK/%luK available"
		" (%luK kernel code, %luK data)\n",
		PAGE_TO_ADDR (nr_free_pages()) / 1024,
		ram_len / 1024,
		((unsigned long)_etext - (unsigned long)_stext) / 1024,
		((unsigned long)__bss_stop - (unsigned long)_sdata) / 1024);
}

void free_initmem (void)
{
	unsigned long ram_end = ram_start + ram_len;
	unsigned long start = PAGE_ALIGN ((unsigned long)(__init_begin));

	if (start >= ram_start && start < ram_end) {
		unsigned long addr;
		unsigned long end = PAGE_ALIGN ((unsigned long)(__init_end));

		if (end > ram_end)
			end = ram_end;

		printk("Freeing unused kernel memory: %ldK freed\n",
		       (end - start) / 1024);

		for (addr = start; addr < end; addr += PAGE_SIZE) {
			mem_map_t *page = virt_to_page (addr);
			ClearPageReserved (page);
			set_page_count (page, 1);
			__free_page (page);
			totalram_pages++;
		}
	}
}

/* Initialize the `bootmem allocator'.  RAM_START and RAM_LEN identify
   what RAM may be used.  */
static void __init bootmem_init (unsigned long ram_start, unsigned long ram_len)
{
	/* The part of the kernel that's in the same managed RAM space
	   used for general allocation.  */
	unsigned long kram_start = (unsigned long)&_kram_start;
	unsigned long kram_end = (unsigned long)&_kram_end;

	/* End of the managed RAM space.  */
	unsigned long ram_end = ram_start + ram_len;

	/* Address range of the interrupt vector table.  */
	unsigned long intv_start = 0; //(unsigned long)&_intv_start;
	unsigned long intv_end = 0x20; //(unsigned long)&_intv_end;

	/* True if the interrupt vectors are in the managed RAM area.  */
	int intv_in_ram = (intv_end > ram_start && intv_start < ram_end);

	/* True if the interrupt vectors are inside the kernel's RAM.  */
	int intv_in_kram = (intv_end > kram_start && intv_start < kram_end);

	/* Machine specific bootmem reserve function */
	void (*volatile mrb) (void) = mach_reserve_bootmem;

	/* The bootmem allocator's allocation bitmap.  */
	unsigned long bootmap = (unsigned long)&_bootmap;
	unsigned long bootmap_len;

	/* Round bootmap location up to next page.  */
	bootmap = PAGE_TO_ADDR (ADDR_TO_PAGE_UP (bootmap));

	/* Initialize bootmem allocator.  */
	bootmap_len = init_bootmem_node (NODE_DATA (0),
					 ADDR_TO_PAGE (bootmap),
					 ADDR_TO_PAGE (PAGE_OFFSET),
					 ADDR_TO_PAGE (ram_end));

	/* Now make the RAM actually allocatable (it starts out `reserved'). */
	free_bootmem (ram_start, ram_len);

	if (kram_end > kram_start)
		/* Reserve the RAM part of the kernel's address space, so it
		   doesn't get allocated.  */
		reserve_bootmem (kram_start, kram_end - kram_start);
	
	if (intv_in_ram && !intv_in_kram)
		/* Reserve the interrupt vector space.  */
		reserve_bootmem (intv_start, intv_end - intv_start);

	if (bootmap >= ram_start && bootmap < ram_end)
		/* Reserve the bootmap space.  */
		reserve_bootmem (bootmap, bootmap_len);

	/* Reserve space for the bss segment.  Also save space for rootfs as well */
	{
		unsigned int rootfs_len = ((unsigned int *)__bss_stop)[2];
		char *end_point;

		/* Is there a romfs sitting at __bss_stop? */
		if(!strcmp(__bss_stop, "-rom1fs-"))
			end_point = __bss_stop + rootfs_len;
		else
			end_point = __bss_stop;

		reserve_bootmem((unsigned long) __bss_start, (unsigned long) (end_point - __bss_start));
	}

	/* Let the platform-dependent code reserve some too.  */
	if (mrb)
		(*mrb) ();
}

/* Setup memory zone. We don't have no page table to warry about.
 * Initialize memory zone and call free_area_init_node().
 *
 * Note: One we get discon mem support, we need to chage the function
 */
void __init paging_init (void)
{
	unsigned i;
	unsigned long zones_size[MAX_NR_ZONES];

	/* Find out what mem this machine has.  */
	mach_get_physical_ram (&ram_start, &ram_len);

	bootmem_init (ram_start, ram_len);

	for (i = 0; i < MAX_NR_ZONES; i++)
		zones_size[i] = 0;

	/* We stuff all the memory into one area, which includes the
	   initial gap from PAGE_OFFSET to ram_start.  */
	zones_size[ZONE_DMA]
		= ADDR_TO_PAGE (ram_len + (ram_start - PAGE_OFFSET));

	free_area_init_node (0, 0, 0, zones_size, PAGE_OFFSET, 0);
}

/* For 2.6, remove this function. mm/page_alloc.c has generic one. */
void si_meminfo (struct sysinfo *info)
{
	info->totalram = totalram_pages;
	info->sharedram = 0;
	info->freeram = nr_free_pages ();
	info->bufferram = atomic_read (&buffermem_pages);
	info->totalhigh = 0;
	info->freehigh = 0;
	info->mem_unit = PAGE_SIZE;
}
