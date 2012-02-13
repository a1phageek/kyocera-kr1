#
# Copyright (c) 2004 Sam Leffler, Errno Consulting
# Copyright (c) 2004 Atheros Communications, Inc.
# All rights reserved.
#
# $Id: //depot/sw/releases/SuperG_Linux_AP/src/802_11/madwifi/ratectrl/Makefile#3 $
#

#
# Makefile for the Atheros Rate Control Support.
#
ifeq ($(obj),)
obj=	.
endif

DEPTH=	../madwifi

obj-m		+= ath_rate_atheros.o
ath_rate_atheros-objs	:= linux.o ratectrl.o ar5212Phy.o ar5211Phy.o if_ath_phyerr.o
export-objs	:= linux.o if_ath_phyerr.o
list-multi	:= ath_rate_atheros.o

include $(obj)/${DEPTH}/Makefile.inc

INCS+=	-I${HAL} -I${HAL}/${OS} -I${obj}/${ATH} -I${obj}/${WLAN} \
	-I${obj}/${DEPTH}

EXTRA_CFLAGS+= ${INCS} ${COPTS}

-include $(TOPDIR)/Rules.make

all:
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(shell pwd) modules

install: all
	test -d ${DESTDIR}/${MODULEPATH} || mkdir -p ${DESTDIR}/${MODULEPATH}
	${TOOLPREFIX}strip -S ath_rate_atheros.$(KMODSUF)
	cp ath_rate_atheros.${KMODSUF} ${DESTDIR}/${MODULEPATH}

clean:
	-rm -f *~ *.o *.ko *.mod.c
	-rm -f .version .*.o.flags .*.o.d .*.o.cmd .*.ko.cmd

ath_rate_atheros.o: ${ath_rate_atheros-objs}
	$(LD) $(LDOPTS) -o ath_rate_atheros.$(KMODSUF) -r $(ath_rate_atheros-objs)
