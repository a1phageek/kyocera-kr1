# Generated automatically from Makefile.in by configure.


all:	common
	make -C upnp TARGET=$(CROSS)
#	make -C upnp/sample/pseudoicsd TARGET=$(CROSS)
	make -C upnp/sample/linux-igd TARGET=$(CROSS)

clean:	common-clean
	make -C upnp clean
#	make -C upnp/sample/pseudoicsd clean
	make -C upnp/sample/linux-igd clean
	
mrproper:	common-clean 
	make -C upnp clean
#	make -C upnp/sample/pseudoicsd clean
	make -C upnp/sample/linux-igd clean

romfs:	common
#	$(ROMFSINST) upnp/sample/pseudoicsd/pseudoicsd /bin/upnp
#	$(ROMFSINST) upnp/sample/pseudoicsd/web /etc/pseudoicsd
#	$(ROMFSINST) -s /var/run/picsdecs.xml /etc/pseudoicsd/picsdesc.xml
	$(ROMFSINST) upnp/sample/linux-igd/upnpd /bin/upnpd

common:
#	cp -f upnp/src/inc/common.h include/upnp/common.h

common-clean:
#	rm -f include/upnp/common.h
