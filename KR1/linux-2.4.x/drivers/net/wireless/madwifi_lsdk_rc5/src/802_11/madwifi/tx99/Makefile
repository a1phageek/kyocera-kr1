#
# Copyright (c) 2004 Sam Leffler, Errno Consulting
# Copyright (c) 2004 Atheros Communications, Inc.
# All rights reserved.
#
# $Id: //depot/sw/releases/linuxsrc/src/802_11/madwifi/tx99/Makefile#3 $
#

#
# Makefile for the Atheros Rate Control Support.
#
ifeq ($(obj),)
obj=	.
endif

DEPTH=	../madwifi

obj-m		+= ath_tx99.o
ath_rate_atheros-objs	:= athtx99.o 
export-objs	:= linux.o
list-multi	:= ath_tx99.o

include $(obj)/${DEPTH}/Makefile.inc

ifeq ($(strip ${BUS}),AHB)
BUSNAME=ahb
EXTRA_CFLAGS+= -DATH_AHB
else
BUSNAME=pci
EXTRA_CFLAGS+= -DATH_PCI
endif


INCS+=	-I${HAL} -I${HAL}/${OS} -I${obj}/${ATH} -I${obj}/${WLAN} \
	-I${obj}/${DEPTH}

EXTRA_CFLAGS+= ${INCS} ${COPTS}

-include $(TOPDIR)/Rules.make

all:
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(shell pwd) modules

install: all
	test -d ${DESTDIR}/${MODULEPATH} || mkdir -p ${DESTDIR}/${MODULEPATH}
	${TOOLPREFIX}strip -S ath_tx99.$(KMODSUF)
	cp ath_tx99.${KMODSUF} ${DESTDIR}/${MODULEPATH}
ifeq ($(DESTDIR),)
	(export MODULEPATH=${MODULEPATH}; depmod -ae)
endif

clean:
	-rm -f *~ *.o *.ko *.mod.c
	-rm -f .version .*.o.flags .*.o.d .*.o.cmd .*.ko.cmd

ath_tx99.o: ${ath_rate_atheros-objs}
	$(LD) $(LDOPTS) -o ath_tx99.$(KMODSUF) -r $(ath_rate_atheros-objs)
