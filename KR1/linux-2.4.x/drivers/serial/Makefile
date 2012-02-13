#
# Makefile for the kernel serial device drivers.
#
# Note! Dependencies are done automagically by 'make dep', which also
# removes any old dependencies. DON'T put your own dependencies here
# unless it's something special (ie not a .c file).
#
# Note 2! The CFLAGS definitions are now inherited from the
# parent makes..
#
#  $Id: Makefile,v 1.1 2004/05/31 02:32:38 rupert Exp $
#

O_TARGET := serial.o

export-objs	:= core.o 
obj-y		:=
obj-m		:=
obj-n		:=
obj-		:=

obj-$(CONFIG_SERIAL_CORE) += core.o
obj-$(CONFIG_SERIAL_AT91US3) += at91us3.o

include $(TOPDIR)/Rules.make

fastdep:

