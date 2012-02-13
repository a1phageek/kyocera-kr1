# Generated automatically from Makefile.in by configure.

DESTDIR=
KERNEL_HEADERS=-I$(ROOTDIR)/linux-2.4.x

prefix=/usr/local
exec_prefix=${prefix}
bindir=${exec_prefix}/bin
sbindir=${exec_prefix}/sbin
mandir=${prefix}/man

SUBDIRS=LINUX

all:
	for x in $(SUBDIRS); do (cd $$x && $(MAKE) ); done

clean:
	for x in $(SUBDIRS); do (cd $$x && $(MAKE) clean ); done

romfs:
	$(ROMFSINST) LINUX/webs /bin/webs
