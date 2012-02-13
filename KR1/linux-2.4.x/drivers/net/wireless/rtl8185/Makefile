#
# drivers/net/wireless/rtl8185
#
# Makefile for the Linux Wireless network device drivers.
#

O_TARGET := rtl8185.o
ifeq ($(RTL8185_SRC),y)
obj-y := 8185ag_util.o\
	8185ag_tx.o\
	8185ag_sme.o\
	8185ag_rx.o\
	8185ag_hw.o\
	8185ag_osdep.o\
	8185ag_ioctl.o\
	8185ag_security.o\
	8185ag_tkip.o\
	8185ag_aes.o\
	8185ag_cam.o\
	8185ag_proc.o\
	8185ag_br_ext.o\
	8185ag_omc8255.o\
	8185ag_dig.o\
	8185ag_eeprom.o
else
obj-y := rtl8185-$(WLANDRVPKG_VERSION).obj
endif
EXTRA_VFLAGS += -O2

obj-m   := $(O_TARGET)
include $(TOPDIR)/Rules.make
