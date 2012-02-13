# Generated automatically from Makefile.in by configure.


all:	common
	make -C upnp TARGET=$(CROSS)
	make -C upnp/sample/pseudoicsd TARGET=$(CROSS)

clean:	common-clean
	make -C upnp clean
	make -C upnp/sample/pseudoicsd clean
	
mrproper:	common-clean 
	make -C upnp clean
	make -C upnp/sample/pseudoicsd clean
romfs:	common
	$(ROMFSINST) upnp/sample/pseudoicsd/pseudoicsd /bin/upnp
	$(ROMFSINST) upnp/sample/pseudoicsd/web /etc/pseudoicsd
	$(ROMFSINST) -s /var/run/picsdecs.xml /etc/pseudoicsd/picsdesc.xml

common:
	cp -f upnp/src/inc/common.h include/upnp/common.h
	cp -f upnp/src/inc/common.h ixml/src/inc/common.h
	cp -f upnp/src/inc/common.h threadutil/inc/common.h
	cp -f upnp/src/inc/common.h upnp/inc/common.h
	cp -f upnp/src/inc/upnp_mm.h include/upnp/upnp_mm.h
	cp -f upnp/src/inc/upnp_mm.h ixml/src/inc/upnp_mm.h
	cp -f upnp/src/inc/upnp_mm.h threadutil/inc/upnp_mm.h
	cp -f upnp/src/inc/upnp_mm.h upnp/inc/upnp_mm.h

common-clean:
	rm -f include/upnp/common.h
	rm -f ixml/src/inc/common.h
	rm -f threadutil/inc/common.h
	rm -f upnp/inc/common.h
	rm -f include/upnp/upnp_mm.h
	rm -f ixml/src/inc/upnp_mm.h
	rm -f threadutil/inc/upnp_mm.h
	rm -f upnp/inc/upnp_mm.h
