#
# drivers/net/wireless/Makefile
#
# Makefile for the Linux Wireless network device drivers.
#

O_TARGET := rtl8180.o

obj-y 	= r8180_pci_init.o r8180_if.o\
            p8180_export.o p8180_include.o p8180_hw.o\
            p8180_tx.o p8180_rx.o p8180_mlme.o\
            p8180_mlme_ap.o p8180_mlme_tbl.o\
            p8180_phycfg.o p8180_eeprom.o p8180_proc.o p8180_ioctl.o\
            p818_encrypt.o p818_prf.o p8180_security.o

override CFLAGS	+= -DRTL_ACT_AS_AP

obj-m   := $(O_TARGET)
include $(TOPDIR)/Rules.make
