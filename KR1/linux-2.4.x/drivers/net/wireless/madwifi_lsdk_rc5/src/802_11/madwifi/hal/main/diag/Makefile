#
# Copyright (c) 2002-2005 Sam Leffler, Errno Consulting
# Copyright (c) 2002-2005 Atheros Communications, Inc.
# All rights reserved.
#
# $Id: //depot/sw/releases/linuxsrc/src/802_11/madwifi/hal/main/diag/Makefile#3 $
#
DEPTH=	..

ATHDIR=	${DEPTH}/../../madwifi/ath
#ATHDIR=	/data/freebsd/head/sys/dev/ath
DESTDIR=

CFLAGS=	-I. -I${DEPTH} -I${ATHDIR} -g

ALL=	eeprom rfgain ani txpow dumpregs key radar

all:	${ALL}

eeprom:	eeprom.c
	${CC} -o eeprom ${CFLAGS} eeprom.c
rfgain:	rfgain.o diag.o
	${CC} -o rfgain ${CFLAGS} rfgain.o diag.o
ani:	ani.o diag.o
	${CC} -o ani ${CFLAGS} ani.o diag.o
radar:	radar.o diag.o
	${CC} -o radar ${CFLAGS} radar.o diag.o
txpow:	txpow.c
	${CC} -o txpow ${CFLAGS} txpow.c
dumpregs: dumpregs.o dumpregs_5210.o dumpregs_5211.o dumpregs_5212.o
	${CC} -o dumpregs ${CFLAGS} dumpregs.o \
		dumpregs_5210.o dumpregs_5211.o dumpregs_5212.o
key:	key.c
	${CC} -o key ${CFLAGS} key.c

install: ${ALL}
	for i in ${ALL}; do \
		rm -f /usr/local/bin/$$i; \
		cp $$i /usr/local/bin/$$i; \
	done

clean:
	rm -f ${ALL} *.o core
