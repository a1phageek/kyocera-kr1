#
# Makefile for the common code of XIPOS MIPS Simulator.
#

.S.s:
	$(CPP) $(AFLAGS) $< -o $*.s
.S.o:
	$(CC) $(AFLAGS) -c $< -o $*.o

O_TARGET:= rtl865x.o
obj-y    += muldiv_lib.o init.o  int.o timer.o irq.o   traceStk.o enableLbcTimeout.o
obj-$(CONFIG_REMOTE_DEBUG)	+= gdb_hook.o
obj-$(CONFIG_USB)	+= sys_pool.o
obj-$(CONFIG_NEW_PCI)		+= pci.o 
			 
include $(TOPDIR)/Rules.make
