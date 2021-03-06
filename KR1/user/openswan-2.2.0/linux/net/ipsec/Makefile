# Makefile for KLIPS kernel code as a module
# Copyright (C) 1998, 1999, 2000,2001  Richard Guy Briggs.
# Copyright (C) 2002	Michael Richardson <mcr@freeswan.org>
# 
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2 of the License, or (at your
# option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# RCSID $Id: Makefile,v 1.1 2004/12/24 07:17:31 rupert Exp $
#
# Note! Dependencies are done automagically by 'make dep', which also
# removes any old dependencies. DON'T put your own dependencies here
# unless it's something special (ie not a .c file).
#

ifeq ($(strip $(KLIPSMODULE)),)
OPENSWANSRCDIR=.
else
OPENSWANSRCDIR=../../..
endif
-include ${OPENSWANSRCDIR}/Makefile.ver

ifeq ($(strip $(KLIPS_TOP)),)
KLIPS_TOP=../..
endif

ifneq ($(strip $(KLIPSMODULE)),)

ifndef TOPDIR
TOPDIR:=/usr/src/linux
endif
export TOPDIR

endif

#
# This magic from User-Mode-Linux list. It gets list of -I options, as
# UML needs some extra, that varry by revision.
#
KERNEL_CFLAGS= $(shell $(MAKE) -C $(TOPDIR) --no-print-directory -s -f Makefile ARCH=$(ARCH) MAKEFLAGS= script SCRIPT='@echo $$(CFLAGS)'   )

MODULE_CFLAGS= $(shell $(MAKE) -C $(TOPDIR) --no-print-directory -s -f Makefile ARCH=$(ARCH) MAKEFLAGS= script SCRIPT='@echo $$(MODFLAGS)'  )

subdir-  := 
subdir-n := 
subdir-y :=
subdir-m :=


MOD_DESTDIR:=net/ipsec

export TOPDIR

all: ipsec.o

foo:
	echo KERNEL: ${KERNEL_CFLAGS}
	echo MODULE: ${MODULE_CFLAGS}

ipsec.o: foo

O_TARGET := ipsec.o
obj-y := ipsec_init.o ipsec_sa.o ipsec_radij.o radij.o
obj-y += ipsec_life.o ipsec_proc.o
obj-y += ipsec_tunnel.o ipsec_xmit.o ipsec_rcv.o ipsec_ipip.o
obj-y += sysctl_net_ipsec.o 
obj-y += pfkey_v2.o pfkey_v2_parser.o pfkey_v2_ext_process.o 
obj-y += version.o
obj-$(CONFIG_IPSEC_AH)  += ipsec_ah.o
obj-$(CONFIG_IPSEC_ESP) += ipsec_esp.o
obj-$(CONFIG_IPSEC_IPCOMP)+= ipsec_ipcomp.o

CFLAGS_ipsec_alg.o += -DEXPORT_SYMTAB
obj-$(CONFIG_IPSEC_ALG) += ipsec_alg.o 
obj-$(CONFIG_IPSEC_ENC_AES) += ipsec_alg_aes.o
obj-$(CONFIG_IPSEC_ENC_CRYPTOAPI) += ipsec_alg_cryptoapi.o

export-objs += ipsec_alg.o


LIBDESDIR=${KLIPS_TOP}/crypto/ciphers/des
VPATH+= ${LIBDESDIR}

include ${LIBDESDIR}/Makefile.objs

LIBFREESWANDIR=${KLIPS_TOP}/lib/libfreeswan
VPATH+=${LIBFREESWANDIR}

include ${LIBFREESWANDIR}/Makefile.objs

# IPcomp stuff
obj-$(CONFIG_IPSEC_IPCOMP) += ipcomp.o 

LIBZLIBSRCDIR=${KLIPS_TOP}/lib/zlib
VPATH+=${LIBZLIBSRCDIR}

LIBAESDIR=$(KLIPS_TOP)/crypto/ciphers/aes
VPATH+=${LIBAESDIR}
include ${LIBAESDIR}/Makefile.objs

#	CFLAGS='$(CFLAGS)' \
#	MODULE_CFLAGS='$(MODULE_CFLAGS)' KERNEL_CFLAGS='$(KERNEL_CFLAGS)' \
#
include ${LIBZLIBSRCDIR}/Makefile.objs

export-objs := radij.o

EXTRA_CFLAGS += $(ALGO_FLAGS)


# include file with .h-style macros that would otherwise be created by
# config. Must occur before other includes.
ifneq ($(strip $(MODULE_DEF_INCLUDE)),)
EXTRA_CFLAGS += -include ${MODULE_DEF_INCLUDE}
endif

# 'override CFLAGS' should really be 'EXTRA_CFLAGS'
#EXTRA_CFLAGS += -nostdinc
EXTRA_CFLAGS += -I${KLIPS_TOP}/include

EXTRA_CFLAGS += -I${TOPDIR}/include 
EXTRA_CFLAGS += -I${LIBZLIBSRCDIR}

ifeq ($(VERSION).$(PATCHLEVEL).$(SUBLEVEL)$(EXTRAVERSION),2.4.2-2)
EXTRA_CFLAGS += -DREDHAT_BOGOSITY
endif

ifeq ($(VERSION).$(PATCHLEVEL).$(SUBLEVEL)$(EXTRAVERSION),2.4.3-12)
EXTRA_CFLAGS += -DREDHAT_BOGOSITY
endif


#ifeq ($(CONFIG_IPSEC_DEBUG),y)
#EXTRA_CFLAGS += -g
#endif

#ifeq ($(CONFIG_IPSEC_ALG), y)
EXTRA_CFLAGS += -DCONFIG_IPSEC_ALG
#endif
# MOST of these flags are in KERNEL_CFLAGS already!

EXTRA_CFLAGS += $(KLIPSCOMPILE)
EXTRA_CFLAGS += -Wall
#EXTRA_CFLAGS += -Werror
#EXTRA_CFLAGS += -Wconversion 
#EXTRA_CFLAGS += -Wmissing-prototypes 
# cannot use both -Wpointer-arith and -Werror with CONFIG_HIGHMEM
# include/linux/highmem.h has an inline function definition that uses void* arithmentic.
ifeq ($(CONFIG_NOHIGHMEM),y)
EXTRA_CFLAGS += -Wpointer-arith 
endif
#EXTRA_CFLAGS += -Wcast-qual 
#EXTRA_CFLAGS += -Wmissing-declarations 
#EXTRA_CFLAGS += -Wstrict-prototypes
#EXTRA_CFLAGS += -pedantic
#EXTRA_CFLAGS += -O3
#EXTRA_CFLAGS += -W
#EXTRA_CFLAGS += -Wwrite-strings 
#EXTRA_CFLAGS += -Wbad-function-cast 

ifneq ($(strip $(KLIPSMODULE)),)
# for when we aren't building in the kernel tree
EXTRA_CFLAGS += -DARCH=${ARCH} 
EXTRA_CFLAGS += -DMODVERSIONS
EXTRA_CFLAGS += -include ${TOPDIR}/include/linux/modversions.h
EXTRA_CFLAGS += ${MODULE_CFLAGS} 
endif

EXTRA_CFLAGS += ${KERNEL_CFLAGS}

#EXTRA_CFLAGS += -DRJ_DEBUG -DRJ_DEBUG2


# GCC 3.2 (and we presume any other 3.x) wants -falign-functions
# in place of the traditional -malign-functions.  Getting this
# wrong leads to a warning, which is fatal due to our use of -Werror.
ifeq ($(patsubst 3.%,3,$(shell $(CC) -dumpversion)),3)
override CFLAGS:=$(subst -malign-functions=,-falign-functions=,$(CFLAGS))
endif


obj-$(CONFIG_IPSEC_AUTH_HMAC_MD5) += ipsec_md5c.o
obj-$(CONFIG_IPSEC_AUTH_HMAC_SHA1) += ipsec_sha1.o

###
### Pre Rules.make
###
# undo O_TARGET, obj-y if no static
ifneq ($(CONFIG_IPSEC),y)
O_TARGET := 
ipsec_obj-y := $(obj-y)
obj-y :=
subdir-y :=
endif

# Define obj-m if modular ipsec 
ifeq ($(CONFIG_IPSEC),m)
obj-m += ipsec.o
endif


# These rules translate from new to old makefile rules
# Translate to Rules.make lists.
multi-used      := $(filter $(list-multi), $(obj-y) $(obj-m))
multi-objs      := $(foreach m, $(multi-used), $($(basename $(m))-objs))
active-objs     := $(sort $(multi-objs) $(obj-y) $(obj-m))
O_OBJS          := $(obj-y)
M_OBJS          := $(obj-m)
MIX_OBJS        := $(filter $(export-objs), $(active-objs))
OX_OBJS := $(export-objs)
SUB_DIRS := $(subdir-y)
ALL_SUB_DIRS := $(subdir-y) $(subdir-m)
MOD_SUB_DIRS := $(subdir-m)

#   dunno why, but some 2.2 setups may need explicit -DEXPORT_SYMTAB
#   uncomment next line if ipsec_alg.c compilation fails with
#   "parse error before `EXPORT_SYMTAB_not_defined'"  --Juanjo

include $(TOPDIR)/Rules.make

###
### Post Rules.make
###
# for modular ipsec, no O_TARGET defined => define ipsec.o creation rules
ifeq ($(CONFIG_IPSEC),m)
ipsec.o : $(ipsec_obj-y)
	rm -f $@
	$(LD) $(LD_EXTRAFLAGS) -r $(ipsec_obj-y) -o $@
endif

$(ipsec_obj-y) $(obj-y) $(obj-m):  $(TOPDIR)/include/linux/config.h $(TOPDIR)/include/linux/autoconf.h

#$(obj-y) $(obj-m):  $(TOPDIR)/include/linux/config.h $(TOPDIR)/include/linux/autoconf.h

USE_STANDARD_AS_RULE=true

clean:
	$(MAKE) -C alg clean
	-rm -f *.o
	-rm -f .*.o.flags
	-rm -f version.c

tags TAGS: *.c *.h libfreeswan/*.c libfreeswan/*.h
	etags *.c ../../include/*.h ../../include/freeswan/*.h
	ctags *.c ../../include/*.h ../../include/freeswan/*.h

tar:
		tar -cvf /dev/f1 .

#
# $Log: Makefile,v $
# Revision 1.1  2004/12/24 07:17:31  rupert
# +: Add OPENSWANS Package
#
# Revision 1.72  2004/06/22 14:44:07  ken
# Merge nice version of Nate's CryptoAPI patch
#
# Revision 1.71  2004/04/18 03:04:21  mcr
# 	removed duplicate version.o reference.
#
# Revision 1.70  2004/04/14 05:09:39  ken
# We need to link version.o
#
# Revision 1.69  2004/04/12 04:02:39  ken
# version.o no longer exists
#
# Revision 1.68  2004/04/11 17:08:41  mcr
# 	moved PASSTHROUGH definitions to openswan.h
# 	requirement for internal.h removed.
# 	version.c is now generated by patch at patch-time.
#
# Revision 1.67  2004/04/06 02:49:25  mcr
# 	pullup of algo code from alg-branch.
#
# Revision 1.66  2004/04/03 19:44:41  ken
# FREESWANSRCDIR -> OPENSWANSRCDIR (patch by folken)
#
# Revision 1.65  2004/02/09 16:22:07  paul
# Added -f to rm version.c in clean target to prevent bogus error
#
# Revision 1.64  2003/12/22 19:40:57  mcr
# 	NAT-T patches 0.6c.
#
# Revision 1.63  2003/12/13 19:10:21  mcr
# 	refactored rcv and xmit code - same as FS 2.05.
#
# Revision 1.62.4.2  2004/04/05 04:30:46  mcr
# 	patches for alg-branch to compile/work with 2.x openswan
#
# Revision 1.62.4.1  2003/12/22 15:25:52  jjo
#       Merged algo-0.8.1-rc11-test1 into alg-branch
#
# Revision 1.62  2003/10/31 02:27:55  mcr
# 	pulled up port-selector patches and sa_id elimination.
#
# Revision 1.61.4.1  2003/10/29 01:30:41  mcr
# 	elimited "struct sa_id".
#
# Revision 1.61  2003/06/22 21:07:46  mcr
# 	adjusted TAGS target in makefile to be useful in 2.00 source layout.
#
# Revision 1.60  2003/05/03 23:45:23  mcr
# 	rm .o.flags and generated version.c file.
#
# Revision 1.59  2003/02/12 19:32:47  rgb
# Added ipsec_xmit to the list of object files.
#
# Revision 1.58  2003/01/03 00:36:44  rgb
#
# Added emacs compile-command.
#
# Revision 1.57  2002/11/08 23:49:53  mcr
# 	use KERNEL_CFLAGS and MODULE_CFLAGS to get proper list
# 	of include directories.
# 	This also eliminates some of the guesswork in the kernel
# 	configuration file.
#
# Revision 1.56  2002/11/08 23:23:18  mcr
# 	attempt to guess kernel compilation flags (i.e. list of -I)
# 	by using some magic targets in the kernel makefile.
#
# Revision 1.55  2002/11/08 10:13:33  mcr
# 	added additional include directories for module builds for 2.4.19.
#
# Revision 1.54  2002/10/20 06:10:30  build
# CONFIG_NOHIGHMEM for -Wpointer-arith RPM building issues.
#
# (elided rest of log)
#
# Local Variables:
# compile-command: "(cd ../../.. && source umlsetup.sh && make -C ${POOLSPACE} module/ipsec.o)"
# End Variables:
#

