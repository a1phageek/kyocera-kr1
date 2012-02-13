SHELL=/bin/bash

ROOT		= .
BINDIR		= $(ROOT)/bin
SRC		= $(ROOT)/src
OBJDIR 		= .
INC 		= $(ROOT)/inc $(ROOT)/inc/rtl8650 $(ROOT)/inc/compiler/gnu
TARBALLTMP	= /tmp/loader_tarball.tmp
TARBALLTGZ	= loader_srcroot.tgz


INC1		= $(INC)
INC2		= $(INC)/compiler/gnu
INC3		= $(SRC)/libc/sys-include
INC4		= ../linux-2.4.x/include


INCPATHS := $(patsubst %,-I%,$(INC1) $(INC2) $(INC3) $(INC4))
PACKBIN		= $(ROOT)/bin/packbin

# If you want to change the target name, please change it with a .bin suffix.
# For instance, you can change target.bin -> runtime.bin.
# Don't change the name from target.bin to something like run.image.
LDR_TARGET 	= ldr.bin

TARGET_OUT 	= $(addsuffix .out, $(basename $(TARGET))) # run.out
LDR_TARGET_OUT = $(addsuffix .out, $(basename $(LDR_TARGET))) # ldr.out

#
# Compiler
#
CC = mips-uclibc-gcc
DEBUG = -gdwarf-2
#CFLAGS	= $(DEBUG) -G 0 -mips1 -EB -mno-gpopt -Wall -DGNU $(INCPATHS) -fno-pic -mno-abicalls
CFLAGS	= $(DEBUG) -G 0 -mips16 -EB -mno-gpopt -Wno-implicit -Werror -DGNU $(INCPATHS) -fno-pic -mno-abicalls -DLOADER

#CFLAGS	= -DWEBS -DUEMF -DOS="LINUX" -DLINUX \
#		  -Dlinux -D__linux__ -Dunix -D__uClinux__ -DEMBED -D__NO_CGI_BIN \
#		  -O1 -msoft-float $(LIBPATHS) -DFIXMEM $(INCPATHS)
		  
#
# Assembler
#
AS				= mips-uclibc-gcc
ASMFLAGS 		= -G 0 -mips1 -c -EB -Wno-implicit -Werror -fno-pic -mno-abicalls
ASMOBJ_PARAMS	= -o $@
SRCLIST 		= $(SRC)/lx4180/vectors.s $(SRC)/lx4180/crt0.s $(SRC)/lx4180/lx4180.s $(SRC)/rtl8650/initmem.s

#
# System defined libraries. Libraries to be linked.
#

SRCLIST += \
$(SRC)/filesystem.c \
$(SRC)/lx4180/genexcpt \
$(SRC)/uart.c \
$(SRC)/ns16550.c \
$(SRC)/flashdrv.c \
$(SRC)/rtl8650/int.c \
$(SRC)/rtl8650/tick.c \
$(SRC)/rtl8650/swCore.c \
$(SRC)/rtl8650/swTable.c \
$(SRC)/rtl8650/vlanTable.c \
$(SRC)/rtl8650/swNic_poll.c \
$(SRC)/rtl8650/phy.c \
$(SRC)/rtl8650/swUtil.c \
$(SRC)/calloc.c \
$(SRC)/gzip/gzip.c \
$(SRC)/gzip/inflate.c \
$(SRC)/gzip/unzip.c \
$(SRC)/gzip/gziputil.c \
$(SRC)/loader.c \
$(SRC)/libc/ctype/ctype_.c \
$(SRC)/libc/reent/impure.c \
$(SRC)/libc/stdlib/atoi.c \
$(SRC)/libc/stdlib/strtol.c \
$(SRC)/libc/string/bzero.c \
$(SRC)/libc/string/memcmp.c \
$(SRC)/libc/string/memcpy.c \
$(SRC)/libc/string/memset.c \
$(SRC)/libc/string/strchr.c \
$(SRC)/libc/string/strcpy.c \
$(SRC)/libc/string/strncmp.c \
$(SRC)/libc/string/strlen.c \
$(ROOT)/hwid_def.c 

OBJS	= $(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(SRCLIST))))		   

ALIGNSRCLIST += $(SRC)/tftpnaive/busywait.c $(SRC)/tftpnaive/net.c $(SRC)/tftpnaive/ip.c $(SRC)/tftpnaive/icmp.c\
		$(SRC)/tftpnaive/arp.c $(SRC)/tftpnaive/udp.c $(SRC)/tftpnaive/bootp.c $(SRC)/tftpnaive/tftp.c\
		$(SRC)/uip/init.c $(SRC)/uip/uip.c $(SRC)/uip/uip_arch.c $(SRC)/uip/uip_arp.c $(SRC)/uip/httpd.c\
		$(SRC)/uip/fs.c $(SRC)/uip/cgi.c 

ALIGNOBJS  = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(ALIGNSRCLIST))))

all: env $(LDR_TARGET_OUT)

$(LDR_TARGET_OUT): $(OBJS) $(ALIGNOBJS) $(SRC)/gnu.ldr.rom.lnk Makefile
	
	@echo "Building $(LDR_TARGET):"
	mips-uclibc-ld -e _rstvector -T$(SRC)/gnu.ldr.rom.lnk $(LIBPATHS) -o \
		$(LDR_TARGET_OUT) $(OBJS) $(LOCALOBJS) $(ALIGNOBJS) -M > $(addsuffix .map, \
		$(basename $(LDR_TARGET)))
	#@mips-uclibc-objdump -DSlx ldr.out > code
	mips-uclibc-objcopy -v -I elf32-big -O binary -S $(LDR_TARGET_OUT) $(LDR_TARGET)
	$(PACKBIN) $(LDR_TARGET) b auto $(addsuffix .bix, $(basename $(LDR_TARGET)))
		
# Make Assembly Code *.s -> *.o
$(OBJDIR)/%.o: %.s
	@echo $@;
	$(AS) -c $(ASMFLAGS) $< $(ASMOBJ_PARAMS)

ram:
	@echo "Building $(LDR_TARGET):"
	#@rm -f ldr.*	
	mips-uclibc-ld -e _rstvector -T$(SRC)/gnu.ldr.ram.lnk $(LIBPATHS) -o \
		$(LDR_TARGET_OUT) $(OBJS) $(LOCALOBJS) $(ALIGNOBJS) -M > $(addsuffix .map, \
		$(basename $(LDR_TARGET)))	
	#@mips-uclibc-objdump -DSlx ldr.out > code		
	#mips-uclibc-objcopy -I elf32-big -O binary -S $(LDR_TARGET_OUT) $(LDR_TARGET)

env:
	if [ ! -f src/flashdrv.c ] ; then\
		ln -sf ../../user/goahead-2.1.4/LINUX/flashdrv.c src/ ;\
	fi

	if [ ! -f inc/flashdrv.h ] ; then\
		ln -sf ../../user/goahead-2.1.4/LINUX/flashdrv.h inc/ ;\
	fi

	if [ ! -f inc/rtl_flashdrv.h ] ; then\
		ln -sf ../../user/goahead-2.1.4/LINUX/rtl_flashdrv.h inc/ ;\
	fi

tarball:
	rm -rf $(TARBALLTMP)
	mkdir -p $(TARBALLTMP)
	cd ..; cp -rf loader_srcroot $(TARBALLTMP)
	find $(TARBALLTMP) -type l -exec rm -f {} \;
	cd $(TARBALLTMP)/loader_srcroot; make clean
	cp src/flashdrv.c $(TARBALLTMP)/loader_srcroot/src
	cp inc/flashdrv.h $(TARBALLTMP)/loader_srcroot/inc
	cp inc/rtl_flashdrv.h $(TARBALLTMP)/loader_srcroot/inc
	cd $(TARBALLTMP); tar -zcvf $(TARBALLTGZ) loader_srcroot
	mv $(TARBALLTMP)/$(TARBALLTGZ) ..
	rm -rf $(TARBALLTMP)
	@echo " "
	@echo "*******************************************************"
	@echo "*                                                     *"
	@echo "* tarball completed. stored in ../$(TARBALLTGZ)  *"
	@echo "*                                                     *"
	@echo "*******************************************************"
	@echo " "

clean:
	@find $(SRC) -type f -name '*.o' -exec rm -f {} \;
	@rm -f ldr.*
	@echo "all objects are cleared successful."
