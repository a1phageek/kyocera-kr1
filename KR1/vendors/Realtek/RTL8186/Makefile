#
#	Makefile -- Build instructions for Conexant/P52
#

ROMFSIMG = $(IMAGEDIR)/romfs.img
IMAGE    = $(IMAGEDIR)/image.bin
KERNEL_FLASH_SIZE :=  $(shell awk  '/CONFIG_FLASH_SIZE_OF_KERNEL/ {print $$3}' $(ROOTDIR)/$(LINUXDIR)/include/linux/autoconf.h )
KSIZE_DEC := $(shell wc -c $(IMAGEDIR)/kernel.bin.gz | awk '{ print $$1}')
KSIZE_HEX := $(shell printf "0x%x" $(KSIZE_DEC))
KFS_DECIMAL := $(shell printf "%d" $(KERNEL_FLASH_SIZE))
PAD := $(shell expr $(KFS_DECIMAL) - $(KSIZE_DEC))

ROMFS_DIRS = bin dev etc home lib mnt proc usr var www etc/config home/samba/codepages /etc/linuxigd

DEVICES = \
	tty,c,5,0      console,c,5,1      cua0,c,5,64      cua1,c,5,65  \
	wlchr0,c,13,0\
	wlchr1,c,13,1\
	mem,c,1,1      kmem,c,1,2         null,c,1,3       ram0,b,1,0 \
	ram1,b,1,1 \
	\
	ppp,c,108,0 \
	ptyp0,c,2,0    ptyp1,c,2,1        ptyp2,c,2,2      ptyp3,c,2,3 \
	ptyp4,c,2,4    ptyp5,c,2,5        ptyp6,c,2,6      ptyp7,c,2,7 \
	ptyp8,c,2,8    ptyp9,c,2,9        ptypa,c,2,10     ptypb,c,2,11 \
	ptypc,c,2,12   ptypd,c,2,13       ptype,c,2,14     ptypf,c,2,15 \
	\
	rom0,b,31,0    rom1,b,31,1        rom2,b,31,2      rom3,b,31,3 \
	rom4,b,31,4    rom5,b,31,5        rom6,b,31,6      rom7,b,31,7 \
	rom8,b,31,8    rom9,b,31,9 \
	\
	tty0,c,4,0     tty1,c,4,1         tty2,c,4,2       tty3,c,4,3 \
	tty4,c,4,4    tty5,c,4,5\
	ttyS0,c,4,64   ttyS1,c,4,65 \
	\
	ttyp0,c,3,0    ttyp1,c,3,1        ttyp2,c,3,2      ttyp3,c,3,3 \
	ttyp4,c,3,4    ttyp5,c,3,5        ttyp6,c,3,6      ttyp7,c,3,7 \
	ttyp8,c,3,8    ttyp9,c,3,9        ttypa,c,3,10     ttypb,c,3,11 \
	ttypc,c,3,12   ttypd,c,3,13       ttype,c,3,14     ttypf,c,3,15 \
	\
	video0,c,81,0 \
	\
	zero,c,1,5     random,c,1,8       urandom,c,1,9 \
	sda,b,8,0 sda1,b,8,1 sda2,b,8,2 sda3,b,8,3 sda4,b,8,4 \
	sda5,b,8,5 sda6,b,8,6 sda7,b,8,7\
	sda8,b,8,8 sda9,b,8,9 sda10,b,8,10\
	sda11,b,8,11 sda12,b,8,12 \
	hda,b,3,0 hda1,b,3,1 hda2,b,3,2 \
	hda3,b,3,3 hda4,b,3,4 hda5,b,3,5 \
	hda6,b,3,6 hda7,b,3,7 hda8,b,3,8 hda9,b,3,9 \
	hdb,b,3,64 hdb1,b,3,65 hdb2,b,3,66 \
	hdb3,b,3,67 hdb1,b,4,68 hdb5,b,3,69 \
	hdb6,b,3,70 hdb7,b,3,71 hdb8,b,3,72 hdb8,b,3,73 \
	hdc,b,22,0 hdc1,b,22,1 hdc2,b,22,2 hdc3,b,22,3 \
	hdc4,b,22,4 hdc5,b,22,5 hdc6,b,22,6 hdc7,b,22,7 \
	hdc8,b,22,8 hdc9,b,22,9 \
	hdd,b,22,64 hdd1,b,22,65 hdd2,b,22,66 hdd3,b,22,67 \
	hdd4,b,22,68 hdd5,b,22,69 hdd6,b,22,70 hdd7,b,22,71 \
	hdd8,b,22,72 hdd9,b,22,73 \
	hde,b,33,0 hde1,b,33,1 hde2,b,33,2 hde3,b,33,3 \
	hde4,b,33,4 hde5,b,33,5 hde6,b,33,6 hde7,b,33,7 \
	hde8,b,33,8 hde9,b,33,9 \
	node,c,254,0	node1,c,254,1 \
	dsp,c,14,3 audio,c,14,4 audio1,c,14,20 dsp1,c,14,19 \
	mixer,c,14,0 mixer1,c,14,16 sequencer,c,14,1 sequencer2,c,14,8 \
	midi00,c,14,2 midi01,c,14,18 midi02,c,14,34 midi03,c,14,50 \
	sndstat,c,14,6 audioctl,c,14,7

clean:

romfs:
	[ -d $(ROMFSDIR)/$$i ] || mkdir -p $(ROMFSDIR)
	for i in $(ROMFS_DIRS); do \
		[ -d $(ROMFSDIR)/$$i ] || mkdir -p $(ROMFSDIR)/$$i; \
	done

	if egrep "^#define CONFIG_BLK_DEV_BLKMEM 1" $(ROOTDIR)/$(LINUXDIR)/include/linux/autoconf.h > /dev/null; then \
	for i in $(DEVICES); do \
		touch $(ROMFSDIR)/dev/@$$i; \
	done\
	fi

	if egrep "^#define CONFIG_MTD 1" $(ROOTDIR)/$(LINUXDIR)/include/linux/autoconf.h > /dev/null; then \
	for i in $(DEVICES); do \
		 mknod  $(ROMFSDIR)/dev/`echo $$i|sed 's/,/ /g'`;\
	done\
	fi

	$(ROMFSINST) -s var/tmp /tmp
	$(ROMFSINST) -s bin /sbin
	$(ROMFSINST) /etc/rc
	$(ROMFSINST) /etc/hosts
	$(ROMFSINST) -s /var/resolv.conf /etc/resolv.conf
	$(ROMFSINST) -s /var/zebra.conf /etc/config/zebra.conf
	$(ROMFSINST) -s /var/ripd.conf /etc/config/ripd.conf
	$(ROMFSINST) /etc/inittab
	$(ROMFSINST) /etc/services
	$(ROMFSINST) ../../Generic/romfs/etc/services /etc/services
	case "$(LINUXDIR)" in \
	*2.4.*) ;; \
	*) echo "ttyS0:linux:/bin/sh" >> $(ROMFSDIR)/etc/inittab ;; \
	esac
	$(ROMFSINST) /etc/udhcpd.conf
	$(ROMFSINST) /etc/ppp.options
	if egrep "^#define CONFIG_RTL865X_PPTPL2TP 1" $(ROOTDIR)/$(LINUXDIR)/include/linux/autoconf.h > /dev/null; then \
	$(ROMFSINST) /etc/pptp.options; \
	$(ROMFSINST) /etc/l2tp.options; \
	$(ROMFSINST) -s /var/l2tpd.conf /etc/config/l2tpd.conf; \
	$(ROMFSINST) -s /var/l2tpd-pppd.conf /etc/config/l2tpd-pppd.conf; \
	fi

	if egrep "^#define CONFIG_RTL865XB_PPTPL2TP 1" $(ROOTDIR)/$(LINUXDIR)/include/linux/autoconf.h > /dev/null; then \
	$(ROMFSINST) /etc/pptp.options; \
	$(ROMFSINST) /etc/l2tp.options; \
	$(ROMFSINST) -s /var/l2tpd.conf /etc/config/l2tpd.conf; \
	$(ROMFSINST) -s /var/l2tpd-pppd.conf /etc/config/l2tpd-pppd.conf; \
	fi
	
	$(ROMFSINST) /etc/linuxigd/gatedesc.xml.org
	$(ROMFSINST) -s /var/gatedesc.xml /etc/linuxigd/gatedesc.xml
	$(ROMFSINST) /etc/linuxigd/gateconnSCPD.xml
	$(ROMFSINST) /etc/linuxigd/gateicfgSCPD.xml
	$(ROMFSINST) /etc/linuxigd/dummy.xml
	$(ROMFSINST) /etc/upnpd.conf

#	$(ROMFSINST) /bin/minimad
#	$(ROMFSINST) /etc/test.mp3


	$(ROMFSINST) -s /bin/webs /bin/ip-up
	$(ROMFSINST) -s /bin/webs /bin/ip-down
	$(ROMFSINST) -s /var/syslog /dev/log
	$(ROMFSINST) -s /var/TZ /etc/TZ
	$(ROMFSINST) -s /var/system.log /www/system.log	

	$(ROMFSINST) /etc/passwd
	$(ROMFSINST) -s /var/run/MACHINE.SID /etc/config/MACHINE.SID
	$(ROMFSINST) config/smb.conf /etc/config/smb.conf
	$(ROMFSINST) /etc/group
	$(ROMFSINST) /etc/services
	$(ROMFSINST) home/samba/codepages/codepage.437 /home/samba/codepages/codepage.437
	$(ROMFSINST) home/samba/codepages/codepage.850 /home/samba/codepages/codepage.850
	$(ROMFSINST) home/samba/codepages/codepage.950 /home/samba/codepages/codepage.950
	$(ROMFSINST) home/samba/codepages/unicode_map.437 /home/samba/codepages/unicode_map.437
	$(ROMFSINST) home/samba/codepages/unicode_map.850 /home/samba/codepages/unicode_map.850
	$(ROMFSINST) home/samba/codepages/unicode_map.950 /home/samba/codepages/unicode_map.950
#
	$(ROMFSINST) -s /var/ipsec.conf /etc/ipsec.conf
	$(ROMFSINST) -s /var/ipsec.secrets /etc/ipsec.secrets

	echo "$(VERSIONSTR) -- " `date` > $(ROMFSDIR)/etc/version

image:
	@if egrep "^#define CONFIG_BLK_DEV_BLKMEM 1" $(ROOTDIR)/$(LINUXDIR)/include/linux/autoconf.h > /dev/null; then \
	[ -d $(IMAGEDIR) ] || mkdir -p $(IMAGEDIR);\
	$(STRIP) -s $(ROOTDIR)/romfs/bin/*;\
	$(STRIP) -s $(ROOTDIR)/romfs/*;\
	$(ROOTDIR)/tools/genromfs -v -V "ROMdisk" -f $(ROMFSIMG) -d $(ROMFSDIR);\
	$(LD)	-EB -Tld.script.rd -b binary -o $(IMAGEDIR)/rom_image $(ROMFSIMG);\
	$(MAKEARCH_KERNEL) -C $(ROOTDIR)/$(LINUXDIR) image;\
	$(OBJCOPY) -S -O binary $(ROOTDIR)/$(LINUXDIR)/vmlinux $(IMAGEDIR)/run.bin  ;\
	gzip -9f $(IMAGEDIR)/run.bin;\
	$(ROOTDIR)/tools/packbin  $(IMAGEDIR)/run.bin.gz r 0 $(IMAGEDIR)/run.bix;\
	fi
	@if egrep "^#define CONFIG_MTD 1" $(ROOTDIR)/$(LINUXDIR)/include/linux/autoconf.h > /dev/null; then \
	[ -d $(IMAGEDIR) ] || mkdir -p $(IMAGEDIR);\
	$(STRIP) -s $(ROOTDIR)/romfs/bin/*;\
	$(STRIP) -s $(ROOTDIR)/romfs/*;\
	if egrep "^#define CONFIG_IMAGE_CRAMFS 1" $(ROOTDIR)/$(LINUXDIR)/include/linux/autoconf.h > /dev/null ; then\
		mkcramfs    $(ROMFSDIR) $(IMAGEDIR)/cramfs.img;\
	fi;\
	if egrep "^#define CONFIG_IMAGE_SQUASH 1" $(ROOTDIR)/$(LINUXDIR)/include/linux/autoconf.h > /dev/null;  then\
	$(ROOTDIR)/tools/mksquashfs $(ROMFSDIR) $(IMAGEDIR)/cramfs.img -be -noappend;\
	fi;\
	$(OBJCOPY) -S -O binary $(ROOTDIR)/$(LINUXDIR)/vmlinux $(IMAGEDIR)/kernel.bin;\
	gzip -9f $(IMAGEDIR)/kernel.bin;\
	if [ "$(PAD)"  -lt 50 ] ;then\
		echo -e  "\n\nError Kernel size is bigger than flash size of kernel";\
		echo -e "KERNEL size $(KSIZE_HEX) is Too Big Flash size only $(KERNEL_FLASH_SIZE)\n\n";\
		exit 1;\
	fi;\
	$(ROOTDIR)/tools/packbin  $(IMAGEDIR)/kernel.bin.gz r 0 $(IMAGEDIR)/kernel.bix;\
	$(ROOTDIR)/tools/kfspad  $(IMAGEDIR)/kernel.bix $(KERNEL_FLASH_SIZE)  $(IMAGEDIR)/cramfs.img $(IMAGEDIR)/run.bin;\
	$(ROOTDIR)/tools/packbin  $(IMAGEDIR)/run.bin k 0 $(IMAGEDIR)/run.bix;\
	fi
