#
# drivers/net/wireless/Makefile
#
# Makefile for the Linux Wireless network device drivers.
#

O_TARGET := wireless_net.o

obj-y		:=
obj-m		:=
obj-n		:=
obj-		:=

# Things that need to export symbols
export-objs	:= airo.o orinoco.o hermes.o atmel.o

obj-$(CONFIG_HERMES)		+= orinoco.o hermes.o
obj-$(CONFIG_PCMCIA_HERMES)	+= orinoco_cs.o
obj-$(CONFIG_APPLE_AIRPORT)	+= airport.o
obj-$(CONFIG_PLX_HERMES)	+= orinoco_plx.o
obj-$(CONFIG_TMD_HERMES)	+= orinoco_tmd.o
obj-$(CONFIG_PCI_HERMES)	+= orinoco_pci.o

obj-$(CONFIG_AIRO)		+= airo.o
obj-$(CONFIG_AIRO_CS)		+= airo_cs.o airo.o
obj-$(CONFIG_PCMCIA_ATMEL)	+= atmel_cs.o atmel.o

obj-$(CONFIG_HOSTAP)		+= hostap.o hostap_crypt_wep.o
obj-$(CONFIG_HOSTAP_CS)		+= hostap_cs.o
obj-$(CONFIG_HOSTAP_PLX)	+= hostap_plx.o
obj-$(CONFIG_HOSTAP_PCI)	+= hostap_pci.o


subdir-$(CONFIG_RTL8185) += rtl8185
ifeq ($(CONFIG_RTL8185),y)
  obj-y += rtl8185/rtl8185.o
endif

# added by Nick Wang
obj-m += gpio.o

# ################################### Ed LAI added for WLAN
# LSDK rc-5 version , USE this BLOCK
############################################
obj-m += madwifi_lsdk_rc5/src/modules/ath_hal.o
obj-m += madwifi_lsdk_rc5/src/modules/wlan.o
obj-m += madwifi_lsdk_rc5/src/modules/ath_pci.o
obj-m += madwifi_lsdk_rc5/src/modules/ath_rate_atheros.o
obj-m += madwifi_lsdk_rc5/src/modules/wlan_acl.o
obj-m += madwifi_lsdk_rc5/src/modules/wlan_ccmp.o
obj-m += madwifi_lsdk_rc5/src/modules/wlan_tkip.o
obj-m += madwifi_lsdk_rc5/src/modules/wlan_scan_ap.o
obj-m += madwifi_lsdk_rc5/src/modules/wlan_wep.o
obj-m += madwifi_lsdk_rc5/src/modules/wlan_xauth.o

# ################################### 
# LSDK rc version , USE this BLOCK
############################################
#obj-m += madwifi_lsdk_rc/src/modules/ath_hal.o
#obj-m += madwifi_lsdk_rc/src/modules/wlan.o
#obj-m += madwifi_lsdk_rc/src/modules/ath_pci.o
#obj-m += madwifi_lsdk_rc/src/modules/ath_rate_atheros.o
#obj-m += madwifi_lsdk_rc/src/modules/wlan_acl.o
#obj-m += madwifi_lsdk_rc/src/modules/wlan_ccmp.o
#obj-m += madwifi_lsdk_rc/src/modules/wlan_tkip.o
#obj-m += madwifi_lsdk_rc/src/modules/wlan_scan_ap.o
#obj-m += madwifi_lsdk_rc/src/modules/wlan_wep.o
#obj-m += madwifi_lsdk_rc/src/modules/wlan_xauth.o

# ################################### 
# LSDK Beta version , USE this BLOCK
##############################################
#obj-m += madwifi_lsdk_beta/src/modules/ath_hal.o
#obj-m += madwifi_lsdk_beta/src/modules/wlan.o
#obj-m += madwifi_lsdk_beta/src/modules/ath_pci.o
#obj-m += madwifi_lsdk_beta/src/modules/ath_rate_atheros.o
#obj-m += madwifi_lsdk_beta/src/modules/wlan_acl.o
#obj-m += madwifi_lsdk_beta/src/modules/wlan_ccmp.o
#obj-m += madwifi_lsdk_beta/src/modules/wlan_tkip.o
#obj-m += madwifi_lsdk_beta/src/modules/wlan_scan_ap.o
#obj-m += madwifi_lsdk_beta/src/modules/wlan_wep.o
#obj-m += madwifi_lsdk_beta/src/modules/wlan_xauth.o
# #################################### Ed LAI

# added start by Murphy
#obj-m += atheros/module/wlan.o
#obj-m += atheros/module/ath_hal.o
#obj-m += atheros/module/ath_pci.o
#obj-m += atheros/module/ath_rate_atheros.o
#obj-m += atheros/module/wlan_acl.o
#obj-m += atheros/module/wlan_ccmp.o
#obj-m += atheros/module/wlan_tkip.o
#obj-m += atheros/module/wlan_scan_ap.o
#obj-m += atheros/module/wlan_wep.o
#obj-m += atheros/module/wlan_xauth.o
# added end - Murphy


include $(TOPDIR)/Rules.make
