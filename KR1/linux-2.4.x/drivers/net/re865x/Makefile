#
# Makefile for the Tulip ethernet driver
#
# Note! Dependencies are done automagically by 'make dep', which also
# removes any old dependencies. DON'T put your own dependencies here
# unless it's something special (ie not a .c file).
#
# Note 2! The CFLAGS definitions are now in the main makefile...

O_TARGET := re865x.o

obj-y := rtl_glue.o mmap.o flashdrv.o
export-objs += rtl_glue.o

obj-$(CONFIG_RE865X) += rtl865x/rtl865x.o swNic2.o 
subdir-$(CONFIG_RE865X) := rtl865x

ifeq ($(CONFIG_RE865X),y)
  ifeq ($(CONFIG_RTL865X_MULTILAYER_BSP),y)
    obj-y += re_core.o rtl865x_proc.o rtl865x_log.o 
    obj-y += rtl865x_mpt.o 
  else
    obj-y += re865x_nic.o
  endif
endif

obj-$(CONFIG_RTL865X_CLE) += cle/cle.o cle_cmdRoot.o
subdir-$(CONFIG_RTL865X_CLE) += cle 
ifeq ($(CONFIG_RTL865X_CLE),y)
   export-objs += cle_cmdRoot.o
endif

obj-$(CONFIG_RTL865X_ROMEREAL) += romereal.o
obj-$(CONFIG_RTL865X_ROMEPERF) += romeperf.o
obj-$(CONFIG_RTL865XB_EXP_CRYPTOENGINE) += crypto/re865x_crypto.o
obj-$(CONFIG_RTL865X_PCM) += pcm.o

subdir-$(CONFIG_RTL865XB_EXP_CRYPTOENGINE) += crypto

EXTRA_CFLAGS += -DRTL865X_DEBUG -DRTL865X_OVER_KERNEL -DRTL865X_OVER_LINUX -D__KERNEL__ -Wno-implicit -Werror



#obj-m   := $(O_TARGET)


include $(TOPDIR)/Rules.make
