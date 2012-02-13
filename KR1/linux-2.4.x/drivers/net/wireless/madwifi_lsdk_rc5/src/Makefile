#
# (C) Copyright 2004 Atheros Communications, Inc.
# All rights reserved.
#
# $Id: //depot/sw/releases/linuxsrc/src/Makefile#3 $
# $File: //depot/sw/releases/linuxsrc/src/Makefile $
# $Author: zhifeng $
# $DateTime: 2005/08/09 13:42:05 $
# $Change: 146233 $
#
#

ifndef TOPDIR
export TOPDIR=$(PWD)/..
endif


ifndef KERNELVERSION 
LSDK_KERNELVERSION =2.4.25
endif

ifndef AP_TYPE 
AP_TYPE = ap30
endif

export OUTPUTDIR=$(shell pwd)

# Set the toolchain path
export TOOLPATH=/RSDK_LINUX/rcc/bin/
                                                                                
# Set the toolchain prefix
export TOOLPREFIX=mips-linux-
                                                                                
# Set the kernel path which is used by several build scripts
#export KERNELPATH=/work/evdo-router_b7/linux-2.4.x
export KERNELPATH=$(TOPDIR)/../../../../../linux-2.4.x
# to the tftpboot dir so redboot can find the files
ifndef TFTPPATH
export TFTPPATH=/tftpboot
endif

# to the tftpboot dir so redboot can find the files
ifndef INSTALLPATH
export INSTALLPATH=$(KERNELPATH)/install
endif
                                                                                
# For the SoC, set the bus type to AHB
export BUS=PCI

include Makefile.inc

#option to build redboot 
ifndef BUILD_REDBOOT
export BUILD_REDBOOT=1
endif

# Set the tftp directory so some scripts can automatically copy the imagesL build for SoC, we need 5312 and 5212 enabled, but all others
# disabled to save space

export PATH:=$(TOPDIR)/tools:$(TOOLPATH)/bin:$(TOPDIR)/src:${PATH}

KERNEL_INSTALL_ROOT=$(KERNELPATH)/arch/mips/ar531x/ROOTDISK/rootdir
                                                                                
../tools/gcc-3.3.3-2.4.25/toolchain_mips/bin: 
	cd ../tools/gcc-3.3.3-2.4.25 && $(MAKE); \
	sudo make PREFIX=$(KERNEL_INSTALL_ROOT) install

toolchain_mips: ../tools/gcc-3.3.3-2.4.25/toolchain_mips/bin

lzma:
	cd ../tools/lzma/sdk4.17/SRC/7zip/Compress/LZMA_Alone/ && $(MAKE)

ifeq ($(BUILD_REDBOOT),1)
redboot_build:
	echo aptype $(AP_TYPE)
	cd redboot_cobra && $(MAKE) $(AP_TYPE)
else

redboot_build:
	@echo "skip redboot build"
endif

kernel_dep: 
	cd $(KERNELPATH) &&  sudo $(MAKE) mrproper 
	cd $(KERNELPATH) &&  cp configs/defconfig-ar531x.$(AP_TYPE) .config
	cd $(KERNELPATH) && $(MAKE) oldconfig 
	cd $(KERNELPATH) &&  $(MAKE) dep 
	cd $(KERNEL_INSTALL_ROOT)/.. && sudo ./setup_rootdir

kernel_build: toolchain_mips
	(. configs/config.$(AP_TYPE); cd kernels/mips-linux-$(LSDK_KERNELVERSION) && $(MAKE)) 

enet_build: toolchain_mips
	cd drivers/ar531x-enet && $(MAKE); sudo $(MAKE) install 

busybox_build: toolchain_mips
	cd ../apps/busybox-1.00-pre10; $(MAKE) PREFIX=$(KERNEL_INSTALL_ROOT) uninstall; make clean; \
	cp -f defconfig-AR531X$(BUILD_DEBUG) .config; \
	$(MAKE); $(MAKE) PREFIX=$(KERNEL_INSTALL_ROOT) install
	# copy AP configurations
	if ( test -e $(KERNEL_INSTALL_ROOT)/etc/wlan/default ) then rm -rf $(KERNEL_INSTALL_ROOT)/etc/wlan/default; fi
	mkdir -p $(KERNEL_INSTALL_ROOT)/etc/wlan/default
	cd ../apps/busybox-1.00-pre10/shell/cli_configs; \
	sudo cp -f default/$(AP_TYPE)/* $(KERNEL_INSTALL_ROOT)/etc/wlan/default/
	cd ../apps/busybox-1.00-pre10/shell/cli_configs; \
	sudo cp -f hostapd/hostapd.fixed$(BUILD_DEBUG) $(KERNEL_INSTALL_ROOT)/etc/wlan/default/hostapd.fixed
	# install libraries
	cd ../tools/gcc-3.3.3-2.4.25  && \
	sudo make PREFIX=$(KERNEL_INSTALL_ROOT) install

hal_build:
	cd $(HAL)/linux && make TARGET=mips1-be-elf clean \
	&&  make TARGET=mips1-be-elf \
	&& make TARGET=mips1-be-elf release

madwifi_build:
	rm -f $(TOPDIR)/src/802_11/madwifi/madwifi/ath/if_ath_phyerr.c
	ln -s $(TOPDIR)/src/802_11/madwifi/phyerr/if_ath_phyerr.c \
	      $(TOPDIR)/src/802_11/madwifi/madwifi/ath/if_ath_phyerr.c
	cd $(TOPDIR)/src/802_11/madwifi/madwifi && make clean \
	&&  make && sudo make install

hostapd:
	cd ../apps/wpa/hostapd-0.3.7; cp -f defconfig .config; make clean; make;
#	sudo make PREFIX=$(KERNEL_INSTALL_ROOT)/sbin install

wireless_tools:
	cd $(TOPDIR)/src/802_11/madwifi/madwifi/tools && make clean \
	&&  make &&  sudo cp -f wlanconfig $(OUTPUTDIR)/modules; 

release: lzma
	sudo $(TOPDIR)/tools/release_kernel.sh $(AP_TYPE)
	sudo $(TOPDIR)/tools/release_rootfs.sh $(AP_TYPE)

ap30 ap51 ap48 ap43:
	$(MAKE) redboot_build AP_TYPE=$@
	$(MAKE) kernel_dep AP_TYPE=$@
	$(MAKE) toolchain_mips
	$(MAKE) kernel_build AP_TYPE=$@
	$(MAKE) enet_build AP_TYPE=$@
	$(MAKE) busybox_build AP_TYPE=$@
	$(MAKE) madwifi_build AP_TYPE=$@
	$(MAKE) wireless_tools
	$(MAKE) hostapd
	$(MAKE) release AP_TYPE=$@
	@echo "LSDK Build Complete"

ap30-debug ap51-debug:
	$(MAKE) redboot_build AP_TYPE=$(subst -debug,,$@)
	$(MAKE) kernel_dep AP_TYPE=$@
	$(MAKE) toolchain_mips
	$(MAKE) kernel_build AP_TYPE=$(subst -debug,,$@)
	$(MAKE) enet_build AP_TYPE=$(subst -debug,,$@)
	$(MAKE) busybox_build AP_TYPE=$(subst -debug,,$@) BUILD_DEBUG=-debug
	$(MAKE) madwifi_build AP_TYPE=$(subst -debug,,$@)
	$(MAKE) wireless_tools
	$(MAKE) hostapd
	$(MAKE) release AP_TYPE=$@
	@echo "LSDK Build Complete"


release-ram:
	(. configs/config.$(AP_TYPE); \
	sudo $(TOPDIR)/tools/mkInstRamdisk.sh $(AP_TYPE))

ap30-ram ap51-ram ap48-ram:
	$(MAKE) kernel_dep AP_TYPE=$@
	$(MAKE) toolchain_mips
	$(MAKE) hal_build  AP_TYPE=$(subst -ram,,$@)
	$(MAKE) kernel_build AP_TYPE=$(subst -ram,,$@)
	$(MAKE) enet_build AP_TYPE=$(subst -ram,,$@)
	$(MAKE) busybox_build AP_TYPE=$(subst -ram,,$@) BUILD_DEBUG=-debug
	$(MAKE) wireless_tools
	$(MAKE) hostapd
	$(MAKE) release-ram AP_TYPE=$(subst -ram,,$@)
	@echo "LSDK Build Complete"
