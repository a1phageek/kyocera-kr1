# Generated automatically from Makefile.in by configure.

KERNEL_HEADERS=-I$(ROOTDIR)/linux-2.4.x
SUBDIRS=src
all:
	for x in $(SUBDIRS); do (cd $$x && $(MAKE) ); done

clean:
	for x in $(SUBDIRS); do (cd $$x && $(MAKE) clean ); done

romfs:
 
	$(ROMFSINST)   src/dnrd /bin/dnrd

