#CC		= gcc
COPT		= -O1
CDEF		= -D_REENTRANT
CWARN		= -Wall 
CDBG		= -ggdb
#CFLAGS		= ${CWARN} ${COPT} ${CDEF} ${CDBG} -DEMBED
#LDFLAGS		+= -Wl,-elf2flt="-s 0x2000"
#LIBS            += -lpthread
LIBS		+=$(LDLIBS)
INSTDIR		= /usr/local/sbin
MANDIR		= /usr/local/man/man8
SHELL		= /bin/sh

all:	dnrd

clean:
	rm -f dnrd core tags ID *.o *.d *~ *.bak *.gdb

install: dnrd
	strip dnrd
	install -m 0751 -o root -g root -s dnrd ${INSTDIR}
	install -m 0644 -o root -g root -s ../doc/dnrd.8 ${MANDIR}

sources = args.c cache.c common.c dns.c lib.c main.c  \
	  query.c relay.c sig.c  udp.c 

include $(sources:.c=.d)

dnrd:	$(sources:.c=.o)
	$(CC) $(LDFLAGS)  $^ -o $@ $(LIBS)

%.d: %.c
	@$(SHELL) -ec '$(CC) -MM $(CFLAGS) $< \
		      | sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		      [ -s $@ ] || rm -f $@'
