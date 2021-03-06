/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994, 1995 Waldorf Electronics
 * Written by Ralf Baechle and Andreas Busse
 * Copyright (C) 1995 - 1999 Ralf Baechle
 * Copyright (C) 1996 Paul M. Antoine
 * Modified for DECStation and hence R3000 support by Paul M. Antoine
 * Further modifications by David S. Miller and Harald Koerfgen
 * Copyright (C) 1999 Silicon Graphics, Inc.
 *
 * Kevin Kissell, kevink@mips.com and Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 2000 MIPS Technologies, Inc.  All rights reserved.
 */
#include <linux/config.h>
#include <linux/init.h>
#include <linux/threads.h>

#include <asm/asm.h>
#include <asm/current.h>
#include <asm/offset.h>
#include <asm/pgtable-bits.h>
#include <asm/processor.h>
#include <asm/regdef.h>
#include <asm/cachectl.h>
#include <asm/mipsregs.h>
#include <asm/stackframe.h>

		.text
		/*
		 * Reserved space for exception handlers.
		 * Necessary for machines which link their kernels at KSEG0.
		 */
		la	k0, kernel_entry
		jr	k0


		.fill	0x400

		__INIT

		/* Cache Error */
		LEAF(except_vec2_generic)
		.set	noreorder
		.set	noat
		.set    mips0
		/*
		 * This is a very bad place to be.  Our cache error
		 * detection has triggered.  If we have write-back data
		 * in the cache, we may not be able to recover.  As a
		 * first-order desperate measure, turn off KSEG0 cacheing.
		 */
		mfc0	k0,CP0_CONFIG
		li	k1,~CONF_CM_CMASK
		and	k0,k0,k1
		ori	k0,k0,CONF_CM_UNCACHED
		mtc0	k0,CP0_CONFIG
		/* Give it a few cycles to sink in... */
		nop
		nop
		nop

		j	cache_parity_error
		nop
		END(except_vec2_generic)

		.set	at

		/*
		 * Special interrupt vector for embedded MIPS.  This is a
		 * dedicated interrupt vector which reduces interrupt processing
		 * overhead.  The jump instruction will be inserted here at
		 * initialization time.  This handler may only be 8 bytes in
		 * size!
		 */
		NESTED(except_vec4, 0, sp)
1:		j	1b			/* Dummy, will be replaced */
		 nop
		END(except_vec4)

		/*
		 * EJTAG debug exception handler.
		 * The EJTAG debug exception entry point is 0xbfc00480, which
		 * normally is in the boot PROM, so the boot PROM must do a
		 * unconditional jump to this vector.
		 */	
		NESTED(except_vec_ejtag_debug, 0, sp)
		j	ejtag_debug_handler
		 nop
		END(except_vec_ejtag_debug)

		/*
		 * EJTAG debug exception handler.
		 */
		NESTED(ejtag_debug_handler, PT_SIZE, sp)
		.set	noat
		.set	noreorder
		mtc0	k0, CP0_DESAVE
		mfc0	k0, CP0_DEBUG	

		sll	k0, k0, 30	# Check for SDBBP.
		bgez	k0, ejtag_return

		la	k0, ejtag_debug_buffer
		sw	k1, 0(k0)
		SAVE_ALL
		jal	ejtag_exception_handler
		 move	a0, sp
		RESTORE_ALL
		la	k0, ejtag_debug_buffer
		lw	k1, 0(k0)	

ejtag_return:
		mfc0	k0, CP0_DESAVE	
		.word	0x4200001f     # DERET, return from EJTAG debug exception.
		 nop
		.set	at
		END(ejtag_debug_handler)
	
		/*
		* NMI debug exception handler for MIPS reference boards.
		* The NMI debug exception entry point is 0xbfc00000, which
		* normally is in the boot PROM, so the boot PROM must do a
		* unconditional jump to this vector.
		*/
		NESTED(except_vec_nmi, 0, sp)
		j       nmi_handler
		 nop
		END(except_vec_nmi)

		NESTED(nmi_handler, PT_SIZE, sp)
		.set    noat
		.set    noreorder
		.set    mips3
		SAVE_ALL
		jal     nmi_exception_handler
		 move   a0, sp
		RESTORE_ALL
		eret
		.set    at
		.set    mips0
		END(nmi_handler)	

		/*
		 * Kernel entry point
		 */
		NESTED(kernel_entry, 16, sp)
		.set	noreorder
		/* The following two symbols are used for kernel profiling. */
		EXPORT(stext)
		EXPORT(_stext)

		/*
		 * Stack for kernel and init, current variable
		 */

		mtc0	zero,CP0_STATUS


#if defined(CONFIG_RTL8670)
		li	t0,0xb9c03010
		sh	zero,(t0)
		li	t0,0xb9c03014
		sw	zero,(t0)
#endif
#if defined(CONFIG_RTL865X)
		li	t0,0xbd012000
		sh	zero,(t0)
		li	t0,0xbd012004
		sw	zero,(t0)
#endif
		mfc0	$9,$13
		mtc0	$0,$13
		la	$28, init_task_union
		addiu	t0, $28, KERNEL_STACK_SIZE-32
		subu	sp, t0, 4*SZREG

		sw	t0, kernelsp
		#if  1	
		jal		_rom_flush_cache
		nop
	
		/*--- flush the write buffer*/
		jal		_rom_wbflush
		nop
		#endif
	



		/* The firmware/bootloader passes argc/argp/envp
		 * to us as arguments.  But clear bss first because
		 * the romvec and other important info is stored there
		 * by prom_init().
		 */
		la	t0, _edata
		sw	zero, (t0)
		la	t1, (_end - 4)
1:
		addiu	t0, 4
		bne	t0, t1, 1b
		 sw	zero, (t0)


		
	
		jal	init_arch
		 nop
		END(kernel_entry)

#if 1 
		.text
		LEAF(_rom_flush_cache)
		.set noreorder
	/*--- transition to kseg1 from undetermined kernel segment*/
	la	$9,2f
	or	$9,0xa0000000
	jr	$9

	.set	noreorder
	

	#--- invalidate the icache and dcache with a 0->1 transition
2:	mtc0	$0, $20	# CCTL
	nop
	nop
	li		$8,0x00000003
	mtc0	$8, $20
	nop
	nop
	#j	3f
	#--- initialize and start COP3
	mfc0	$8,$12
	nop
	nop
	or		$8,0x80000000
	mtc0	$8,$12
	nop
	nop
	
	#--- load iram base and top
	la		$8,__iram
	la		$9,0x0ffffc00
	and		$8,$8,$9
	mtc3	$8,$0								# IW bas
	nop
	nop
	addiu	$8,$8,0x1fff
	mtc3	$8,$1								# IW top
	nop
	nop
	
	#--- load dram base and top
	la		$8,__dram
	la		$9,0xfffffc00
	and		$8,$8,$9
	mtc3	$8,$4								# DW bas
	nop
	nop
	addiu	$8,$8,0x0fff
	mtc3	$8,$5								# DW top
	nop
	nop
	

3:	
	#--- enable icache and dcache
	mtc0	$0, $20	# CCTL
	nop
	nop

	.set	reorder
	j		$31


	END(_rom_flush_cache)



	LEAF(_rom_wbflush)
_rom_wbflush:
	li		$8, 0xa0000000				#/* load uncached address	*/
	lw		$8, 0($8)					#/* read in order to flush 	*/
	j		$31							#/* return to caller		*/
	END(_rom_wbflush)


#endif

#ifdef CONFIG_SMP

/*
 * SMP slave cpus entry point.  Board specific code for bootstrap calls this
 * function after setting up the stack and gp registers. 
 */
		LEAF(smp_bootstrap)
		.set push
		.set noreorder
		mtc0	zero, CP0_WIRED
		CLI
		mfc0	t0, CP0_STATUS
		li	t1, ~(ST0_CU1|ST0_CU2|ST0_CU3|ST0_KX|ST0_SX)
		and	t0, t1
		or	t0, (ST0_CU0);
		jal	start_secondary
		mtc0	t0, CP0_STATUS
		.set pop
		END(smp_bootstrap)
#endif

		__FINIT

		/*
		 * This buffer is reserved for the use of the EJTAG debug 
		 * handler.
		 */
		.data
		EXPORT(ejtag_debug_buffer)
		.fill	4
	
		.comm	kernelsp,    NR_CPUS * 8, 8
		.comm	pgd_current, NR_CPUS * 8, 8

	.macro	page name, order=0
	.globl	\name
\name:	.size	\name, (_PAGE_SIZE << \order)
	.org	. + (_PAGE_SIZE << \order)
	.type	\name, @object
	.endm

	.data
	.align	12

	page	swapper_pg_dir, _PGD_ORDER
	page	empty_bad_page, 0
	page	empty_bad_page_table, 0
	page	invalid_pte_table, 0
