
.EXPORT_ALL_VARIABLES:

DIRS = src

all:
	for i in $(DIRS) ; do make -C $$i -f Makefile.rtl865x || exit $? ; done
	make -C src/dlisten -f Makefile.rtl865x
	
romfs:
	for i in $(DIRS) ; do make -C $$i -f Makefile.rtl865x romfs || exit $? ; done
	make -C src/dlisten -f Makefile.rtl865x romfs

clean:
	-for i in $(DIRS) ; do make -C $$i -f Makefile.rtl865x clean ; done
	make -C src/dlisten -f Makefile.rtl865x clean

