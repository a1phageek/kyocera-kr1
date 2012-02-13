#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1994, 1995, 1996 by Ralf Baechle
# DECStation modifications by Paul M. Antoine, 1996
# Copyright (C) 2002  Maciej W. Rozycki
#
# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies. Remember to do have actions
# for "archclean" and "archdep" for cleaning up and making dependencies for
# this architecture
#

#
# Select the object file format to substitute into the linker script.
#
ifdef CONFIG_CPU_LITTLE_ENDIAN
#tool-prefix	= mipsel-linux-
#tool-prefix	= mips-linux-
tool-prefix	= mips-uclibc-
else
tool-prefix	= mips-uclibc-
endif

ifdef CONFIG_CROSSCOMPILE
CROSS_COMPILE	= $(tool-prefix)
endif

#
# GCC uses -G0 -mabicalls -fpic as default.  We don't want PIC in the kernel
# code since it only slows down the whole thing.  At some point we might make
# use of global pointer optimizations but their use of $28 conflicts with
# the current pointer optimization.
#
# The DECStation requires an ECOFF kernel for remote booting, other MIPS
# machines may also.  Since BFD is incredibly buggy with respect to
# crossformat linking we rely on the elf2ecoff tool for format conversion.
#
GCCFLAGS	:= -I $(TOPDIR)/include/asm/gcc
GCCFLAGS	+= -G 0 -mno-abicalls -fno-pic -pipe
LINKFLAGS	+= -G 0 -static # -N
MODFLAGS	+= -mlong-calls

ifdef CONFIG_REMOTE_DEBUG
GCCFLAGS	+= -g
endif

CFLAGS += -DNO_MM
AFLAGS += -DNO_MM

#
# CPU-dependent compiler/assembler options for optimization.
#
ifdef CONFIG_CPU_R3000
GCCFLAGS	+= -mcpu=r3000 -mips1
endif
ifdef CONFIG_CPU_TX39XX
GCCFLAGS	+= -mcpu=r3000 -mips1
endif
ifdef CONFIG_CPU_R6000
GCCFLAGS	+= -mcpu=r6000 -mips2 -Wa,--trap
endif
ifdef CONFIG_CPU_R4300
GCCFLAGS	+= -mcpu=r4300 -mips2 -Wa,--trap
endif
ifdef CONFIG_CPU_VR41XX
GCCFLAGS	+= -mcpu=r4600 -mips2 -Wa,--trap
endif
ifdef CONFIG_CPU_R4X00
GCCFLAGS	+= -mcpu=r4600 -mips2 -Wa,--trap
endif
ifdef CONFIG_CPU_TX49XX
GCCFLAGS	+= -mcpu=r4600 -mips2 -Wa,--trap
endif
ifdef CONFIG_CPU_MIPS32
GCCFLAGS	+= -mcpu=r4600 -mips2 -Wa,--trap
endif
ifdef CONFIG_CPU_MIPS64
GCCFLAGS	+= -mcpu=r4600 -mips2 -Wa,--trap
endif
ifdef CONFIG_CPU_R5000
GCCFLAGS	+= -mcpu=r5000 -mips2 -Wa,--trap
endif
ifdef CONFIG_CPU_R5432
GCCFLAGS	+= -mcpu=r5000 -mips2 -Wa,--trap
endif
ifdef CONFIG_CPU_NEVADA
# Cannot use -mmad with currently recommended tools
GCCFLAGS	+= -mcpu=r5000 -mips2 -Wa,--trap
endif
ifdef CONFIG_CPU_RM7000
GCCFLAGS	+= -mcpu=r5000 -mips2 -Wa,--trap
endif
ifdef CONFIG_CPU_SB1
GCCFLAGS	+= -mcpu=sb1 -mips2 -Wa,--trap
ifdef CONFIG_SB1_PASS_1_WORKAROUNDS
MODFLAGS	+= -msb1-pass1-workarounds
endif
endif

AFLAGS		+= $(GCCFLAGS)
CFLAGS		+= $(GCCFLAGS)


#
# We unconditionally build the math emulator
#
CORE_FILES	+= arch/mipsnommu/math-emu/fpu_emulator.o
SUBDIRS		+= arch/mipsnommu/math-emu

#
# ramdisk/initrd support
# You need a compressed ramdisk image, named ramdisk.gz in
# arch/mipsnommu/ramdisk
#
ifdef CONFIG_EMBEDDED_RAMDISK
CORE_FILES	+= arch/mipsnommu/ramdisk/ramdisk.o
SUBDIRS		+= arch/mipsnommu/ramdisk
endif


#
# Board-dependent options and extra files
#

# XIPOS mips simulator
ifdef CONFIG_SIMULATOR
CORE_FILES	+= arch/mipsnommu/simulator/simulator.o
SUBDIRS		+= arch/mipsnommu/simulator
#LOADADDR	:= 0x88200000
LOADADDR	:= 0x80080000
endif

ifdef CONFIG_RTL8670 
CORE_FILES	+= arch/mipsnommu/rtl8670/rtl8670.o
SUBDIRS		+= arch/mipsnommu/rtl8670
#LOADADDR	:= 0x88200000
LOADADDR	:= 0x8002000
endif

ifdef CONFIG_RTL865X
CORE_FILES	+= arch/mipsnommu/rtl865x/rtl865x.o
SUBDIRS		+= arch/mipsnommu/rtl865x
LOADADDR	:= 0x80080000
endif

#
# Acer PICA 61, Mips Magnum 4000 and Olivetti M700.
#
ifdef CONFIG_MIPS_JAZZ
CORE_FILES	+= arch/mipsnommu/jazz/jazz.o
SUBDIRS		+= arch/mipsnommu/jazz arch/mipsnommu/arc
LIBS		+= arch/mipsnommu/arc/arclib.a
LOADADDR	:= 0x80080000
endif

#
# Au1000 (Alchemy Semi PB1000) eval board
#
ifdef CONFIG_MIPS_PB1000
LIBS		+= arch/mipsnommu/au1000/pb1000/pb1000.o \
		   arch/mipsnommu/au1000/common/au1000.o
SUBDIRS		+= arch/mipsnommu/au1000/pb1000 arch/mipsnommu/au1000/common
LOADADDR	:= 0x80100000
endif

#
# Au1000 (Alchemy Semi PB1500) eval board
#
ifdef CONFIG_MIPS_PB1500
LIBS		+= arch/mipsnommu/au1000/pb1500/pb1500.o \
		   arch/mipsnommu/au1000/common/au1000.o
SUBDIRS		+= arch/mipsnommu/au1000/pb1500 arch/mipsnommu/au1000/common
LOADADDR	:= 0x80100000
endif

#
# Algorithmics P4032
#
ifdef CONFIG_ALGOR_P4032
CORE_FILES	+= arch/mipsnommu/algor/algor.o
SUBDIRS		+= arch/mipsnommu/algor
LOADADDR	:= 0x80000000
endif

#
# Baget/MIPS
#
ifdef CONFIG_BAGET_MIPS
SUBDIRS		+= arch/mipsnommu/baget arch/mipsnommu/baget/prom
LIBS		+= arch/mipsnommu/baget/baget.a arch/mipsnommu/baget/prom/bagetlib.a
LOADADDR	:= 0x80001000
endif

#
# Cobalt Server
#
ifdef CONFIG_MIPS_COBALT
SUBDIRS		+= arch/mipsnommu/cobalt
CORE_FILES	+= arch/mipsnommu/cobalt/cobalt.o
LOADADDR	:= 0x80080000
endif

#
# DECstation family
#
ifdef CONFIG_DECSTATION
CORE_FILES	+= arch/mipsnommu/dec/dec.o
SUBDIRS		+= arch/mipsnommu/dec arch/mipsnommu/dec/prom
LIBS		+= arch/mipsnommu/dec/prom/rexlib.a
LOADADDR	:= 0x80040000
endif

#
# Galileo EV64120 Board
#
ifdef CONFIG_MIPS_EV64120
LIBS		+= arch/mipsnommu/galileo-boards/ev64120/ev64120.o
SUBDIRS		+= arch/mipsnommu/galileo-boards/ev64120
LOADADDR	:= 0x80100000
endif

#
# Galileo EV96100 Board
#
ifdef CONFIG_MIPS_EV96100
LIBS		+= arch/mipsnommu/galileo-boards/ev96100/ev96100.o \
		   arch/mipsnommu/galileo-boards/generic/galboards.o
SUBDIRS		+= arch/mipsnommu/galileo-boards/generic \
		   arch/mipsnommu/galileo-boards/ev96100
LOADADDR	:= 0x80100000
endif

#
# Globespan IVR eval board with QED 5231 CPU
#
ifdef CONFIG_MIPS_IVR
LIBS		+= arch/mipsnommu/ite-boards/ivr/ivr.o \
		   arch/mipsnommu/ite-boards/generic/it8172.o
SUBDIRS		+= arch/mipsnommu/ite-boards/generic arch/mipsnommu/ite-boards/ivr
LOADADDR	:= 0x80100000
endif

#
# HP LaserJet
#
ifdef CONFIG_HP_LASERJET
SUBDIRS		+= arch/mipsnommu/hp-lj
LIBS		+= arch/mipsnommu/hp-lj/hp-lj.o
LOADADDR	:= 0x80030000
endif

#
# ITE 8172 eval board with QED 5231 CPU
#
ifdef CONFIG_MIPS_ITE8172
LIBS		+= arch/mipsnommu/ite-boards/qed-4n-s01b/ite.o \
		   arch/mipsnommu/ite-boards/generic/it8172.o
SUBDIRS		+= arch/mipsnommu/ite-boards/generic arch/mipsnommu/ite-boards/qed-4n-s01b
LOADADDR	:= 0x80100000
endif

#
# MIPS Atlas board
#
ifdef CONFIG_MIPS_ATLAS
LIBS		+= arch/mipsnommu/mips-boards/atlas/atlas.o \
		   arch/mipsnommu/mips-boards/generic/mipsboards.o
SUBDIRS		+= arch/mipsnommu/mips-boards/generic arch/mipsnommu/mips-boards/atlas
LOADADDR	:= 0x80100000
endif

#
# MIPS Malta board
#
ifdef CONFIG_MIPS_MALTA
LIBS		+= arch/mipsnommu/mips-boards/malta/malta.o \
		   arch/mipsnommu/mips-boards/generic/mipsboards.o
SUBDIRS		+= arch/mipsnommu/mips-boards/malta arch/mipsnommu/mips-boards/generic
LOADADDR	:= 0x80100000
endif

#
# Momentum Ocelot board
#
ifdef CONFIG_MOMENCO_OCELOT
# The Ocelot setup.o must be linked early - it does the ioremap() for the
# mips_io_port_base.
CORE_FILES	+= arch/mipsnommu/gt64120/common/gt64120.o \
		   arch/mipsnommu/gt64120/momenco_ocelot/momenco_ocelot.o
SUBDIRS		+= arch/mipsnommu/gt64120/common arch/mipsnommu/gt64120/momenco_ocelot
LOADADDR	:= 0x80100000
endif

#
# NEC DDB Vrc-5074
#
ifdef CONFIG_DDB5074
SUBDIRS		+= arch/mipsnommu/ddb5074
LIBS		+= arch/mipsnommu/ddb5074/ddb5074.a
LOADADDR	:= 0x80080000
endif

#
# NEC DDB Vrc-5476
#
ifdef CONFIG_DDB5476
SUBDIRS		+= arch/mipsnommu/ddb5xxx/common arch/mipsnommu/ddb5xxx/ddb5476
LIBS		+= arch/mipsnommu/ddb5xxx/common/ddb5xxx.o \
		   arch/mipsnommu/ddb5xxx/ddb5476/ddb5476.o
LOADADDR	:= 0x80080000
endif

#
# NEC DDB Vrc-5477
#
ifdef CONFIG_DDB5477
SUBDIRS		+= arch/mipsnommu/ddb5xxx/common arch/mipsnommu/ddb5xxx/ddb5477
LIBS		+= arch/mipsnommu/ddb5xxx/common/ddb5xxx.o \
		   arch/mipsnommu/ddb5xxx/ddb5477/ddb5477.o
LOADADDR	:= 0x80080000
endif

#
# NEC Osprey (vr4181) board
#
ifdef CONFIG_NEC_OSPREY
SUBDIRS		+= arch/mipsnommu/vr4181/common arch/mipsnommu/vr4181/osprey
LIBS		+= arch/mipsnommu/vr4181/common/vr4181.o \
		   arch/mipsnommu/vr4181/osprey/osprey.o
LOADADDR	:= 0x80002000
endif

#
# Philips Nino
#
ifdef CONFIG_NINO
CORE_FILES	+= arch/mipsnommu/philips/nino/nino.o
SUBDIRS		+= arch/mipsnommu/philips/nino
LOADADDR	:= 0x80000000
endif

#
# SGI IP22 (Indy/Indigo2)
#
ifdef CONFIG_SGI_IP22
CORE_FILES	+= arch/mipsnommu/sgi-ip22/ip22-kern.o
LIBS		+= arch/mipsnommu/arc/arclib.a
SUBDIRS		+= arch/mipsnommu/sgi-ip22 arch/mipsnommu/arc
#
# Set LOADADDR to >= 0x88069000 if you want to leave space for symmon,
# 0x88002000 for production kernels.  Note that the value must be
# 8kb aligned or the handling of the current variable will break.
#
LOADADDR	:= 0x88002000
endif

#
# Sibyte SB1250 SOC
#
ifdef CONFIG_SIBYTE_SB1250
# This is a LIB so that it links at the end, and initcalls are later
# the sequence; but it is built as an object so that modules don't get
# removed (as happens, even if they have __initcall/module_init)
LIBS		+= arch/mipsnommu/sibyte/sb1250/sb1250.o
SUBDIRS		+= arch/mipsnommu/sibyte/sb1250
endif

#
# Sibyte SWARM board
#
ifdef CONFIG_SIBYTE_SWARM
LIBS		+= arch/mipsnommu/sibyte/swarm/sbswarm.a
SUBDIRS		+= arch/mipsnommu/sibyte/swarm
LOADADDR	:= 0x80100000
endif

#
# SB1 Cache Error handler
#
ifdef CONFIG_SB1_CACHE_ERROR
LIBS		+= arch/mipsnommu/sibyte/sb1/sb1kern.a
SUBDIRS		+= arch/mipsnommu/sibyte/sb1
endif

#
# SNI RM200 PCI
#
ifdef CONFIG_SNI_RM200_PCI
CORE_FILES	+= arch/mipsnommu/sni/sni.o
SUBDIRS		+= arch/mipsnommu/sni arch/mipsnommu/arc
LIBS		+= arch/mipsnommu/arc/arclib.a
LOADADDR	:= 0x80002000
endif

#
# Toshiba JMR-TX3927 board
#
ifdef CONFIG_TOSHIBA_JMR3927
CORE_FILES	+= arch/mipsnommu/jmr3927/rbhma3100/jmr3927.o \
		   arch/mipsnommu/jmr3927/common/tx3927.o
SUBDIRS		+= arch/mipsnommu/jmr3927/rbhma3100 arch/mipsnommu/jmr3927/common
LOADADDR	:= 0x80050000
endif


#
# Choosing incompatible machines durings configuration will result in
# error messages during linking.  Select a default linkscript if
# none has been choosen above.
#
vmlinux: arch/$(ARCH)/ld.script

#zxl for uClinux
linux: arch/$(ARCH)/ld.script

arch/$(ARCH)/ld.script: arch/$(ARCH)/ld.script.in arch/$(ARCH)/Makefile
	sed -e 's/@@LOADADDR@@/$(LOADADDR)/' <$< >$@
LINKFLAGS	+= -T arch/$(ARCH)/ld.script

HEAD := arch/mipsnommu/kernel/head.o arch/mipsnommu/kernel/init_task.o

SUBDIRS := $(addprefix arch/mipsnommu/, tools) $(SUBDIRS) $(addprefix arch/mipsnommu/, kernel mm lib)
CORE_FILES := arch/mipsnommu/kernel/kernel.o arch/mipsnommu/mm/mm.o $(CORE_FILES)
LIBS := arch/mipsnommu/lib/lib.a $(LIBS)

ifdef CONFIG_BAGET_MIPS

BAGETBOOT = $(MAKE) -C arch/$(ARCH)/baget

balo: vmlinux
	$(BAGETBOOT) balo

endif

ifdef CONFIG_MIPS_EV64120
GALILEOBOOT = $(MAKE) -C arch/$(ARCH)/galileo-boards/ev64120

gboot: vmlinux
	$(MAKE) -C arch/$(ARCH)/galileo-boards/ev64120/compressed
endif

MAKEBOOT = $(MAKE) -C arch/$(ARCH)/boot

vmlinux.ecoff: vmlinux
	@$(MAKEBOOT) $@

archclean:
	@$(MAKEBOOT) clean
	rm -f arch/$(ARCH)/ld.script
	$(MAKE) -C arch/$(ARCH)/tools clean
	$(MAKE) -C arch/mipsnommu/baget clean

archmrproper:
	@$(MAKEBOOT) mrproper
	$(MAKE) -C arch/$(ARCH)/tools mrproper

archdep:
	if [ ! -f $(TOPDIR)/include/asm-$(ARCH)/offset.h ]; then \
		touch $(TOPDIR)/include/asm-$(ARCH)/offset.h; \
	fi;
	@$(MAKEBOOT) dep
