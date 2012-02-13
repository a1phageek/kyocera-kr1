#
# Makefile for the common code of XIPOS MIPS Simulator.
#

.S.s:
	$(CPP) $(AFLAGS) $< -o $*.s
.S.o:
	$(CC) $(AFLAGS) -c $< -o $*.o

O_TARGET:= rtl8670.o
obj-y    += muldiv_lib.o init.o gdbstub.o int.o timer.o irq.o 
			 
include $(TOPDIR)/Rules.make
