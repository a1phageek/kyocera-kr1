#
# Makefile for the common code of XIPOS MIPS Simulator.
#

.S.s:
	$(CPP) $(AFLAGS) $< -o $*.s
.S.o:
	$(CC) $(AFLAGS) -c $< -o $*.o

O_TARGET:= simulator.o

#obj-y    += muldiv_lib.o gdbstub.o sp720_init.o tx39uart.o sim_int_handle.o sim_timer.o sim_irq.o \
#			simvideo.o flash.o tpanel.o

obj-y    += muldiv_lib.o sim_init.o gdbstub.o tx39uart.o sim_int_handle.o sim_timer.o sim_irq.o \
			flash.o tpanel.o
include $(TOPDIR)/Rules.make
