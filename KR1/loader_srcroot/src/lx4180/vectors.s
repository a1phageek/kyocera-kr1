#/*
# * Copyright c                Realtek Semiconductor Corporation, 2002
# * All rights reserved.                                                    
# * 
# * $Header: /home/cvsroot/uClinux-dist/loader_srcroot/src/lx4180/vectors.s,v 1.3 2004/08/26 13:20:00 yjlou Exp $
# *
# * $Author: yjlou $
# *
# * Abstract:
# *
# *   Lx4180 vector assembly codes.
# *
# * $Log: vectors.s,v $
# * Revision 1.3  2004/08/26 13:20:00  yjlou
# * *: Loader upgrades to "00.00.12".
# * +: support "Loader Segment Descriptors Table"
# * -: remove romcopystart/pause/resume
# *
# * Revision 1.2  2004/03/31 01:49:20  yjlou
# * *: all text files are converted to UNIX format.
# *
# * Revision 1.1  2004/03/16 06:36:13  yjlou
# * *** empty log message ***
# *
# * Revision 1.1.1.1  2003/09/25 08:16:55  tony
# *  initial loader tree 
# *
# * Revision 1.1.1.1  2003/05/07 08:16:06  danwu
# * no message
# *
# */

	.globl	_rstvector
	.extern	_start
	.extern _saveContext
	.extern genexcpt_handler
	.extern _restoreContext

#/*-------------------------------------------------------------------
#**
#**  For the ROM version, the vector section is linked at BFC00000.
#**
#**  BOOTSTRAP EXCEPTION HANDLERS
#**
#**  BFC00000	RESET
#**  BFC00180    Other Exceptions
#**
#**
#**  For the RAM version, the vector section is linked at 0 in kseg0/1.
#**  Note that the entry point in this case will be offset 0x0.
#**
#**  RAM EXCEPTION HANDLERS
#**
#**  80000080    General Exceptions
#**
#**-------------------------------------------------------------------
#*/
	.section ".vector", "ax"
#	.text
	.ent	_rstvector
_rstvector:
	j	_start
	nop
	j	_start
	nop
	j	_start
	nop
	j	_start
	nop
	j	_start
	nop
	j	_start
	nop
	j	_start
	nop
	j	_start
	nop
	j	_start
	nop
	j	_start
	nop
	
	#--- General exception handler for RAM, offset 80
	.align  7	#force next address to multiple of 0x80
	.set noreorder
	.set noat
	
	move	$27,$31								# backup ra
	la		$26,_saveContext
	jal		$26
	nop
	la		$26,genexcpt_handler
	jal		$26
	nop
	la		$26,_restoreContext
	jal		$26
	nop
	move	$31,$27								# restore ra
	j		$26									# return from exception
	rfe
	
	.set at
	.set reorder
	
	.align  8	#force next address to multiple of 0x100

	#--- Loader Segment Descriptors Table
	.globl	ldrSegDesTable
ldrSegDesTable:
	#       Start addr  End addr
	.word	0x57888651, 0x57888651	# magic number
	.word	0x00000000, 0x00004000	# segment 0
	.word	0x00008000, 0x00010000	# segment 1
	.word	0x00010000, 0x00020000	# segment 2
	.word	0x00000000, 0x00000000	# ending mark
	
	#--- General exception handler for ROM, offset 180
	.align  7	#force next address to multiple of 0x80
	la	$26, _gen_exception_rom
	jr	$26
	
	.end _rstvector
	
