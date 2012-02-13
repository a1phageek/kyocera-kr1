#
# Makefile for the GoAhead web server reference source base
#  for the Linux OS
#
# Copyright (c) GoAhead Software, Inc. 1995-2000
#
# $Id: Makefile,v 1.20 2004/06/14 13:48:17 rupert Exp $
#

all:	compile 


ARCH	= libwebs.a
NAME	= webs

# User Management switch
#UMSW	= -DUSER_MANAGEMENT_SUPPORT

# Digest Access switch
#DASW	= -DDIGEST_ACCESS_SUPPORT

# SSL switches (enable all to activate)
# SSLINC = /RSASSL/library/include
# SSLLIB = /RSASSL/library/lib/libsslc.a
# SSLSW	= -DWEBS_SSL_SUPPORT -I$(SSLINC)
# SSLPATCHFILE = ../websSSL.o

# If-Modified-Support switches (requires math library, libm.a)
# IFMODSW = -DWEBS_IF_MODIFIED_SUPPORT
# IFMODLIB = /usr/lib/libm.a

# dino 2004/10/29
# support GST web_server
OBJS = 	\
	./web_server/web_server.o\
	./web_server/http.o\
	./web_server/model/data/htmlcgi.o\
	./web_server/model/data/hpfiles.o\
	./web_server/model/data/hpvars.o\
	./web_server/model/cfg.o\
	./web_server/htmlproc.o\
	./web_server/strilib.o\
	./web_server/parseip.o\
	./web_server/model/fw_ver.o\
	./web_server/model/str.o\
	./web_server/utility.o \
	./web_server/hwid_def.o \
	./web_server/model/tmss_cfg.o
# dino
# Dependencies
DEPEND_FILES	=  ipupdown.o rome_asp.o flashdrv.o cfgmgr.o board.o tblDrv_mapping.o ../asp.o ../balloc.o ../base64.o ../cgi.o ../default.o  \
				  ../ejlex.o ../ejparse.o ../form.o \
				  ../h.o ../handler.o ../mime.o ../misc.o ../page.o  \
				  ../ringq.o ../rom.o \
				  ../sock.o ../sockGen.o $(SSLPATCHFILE) \
				  ../security.o ../sym.o ../uemf.o ../url.o ../value.o \
				  ../md5c.o ../um.o ../umui.o ../websda.o ../emfdb.o \
				  ../webrom.o ../webs.o ../websuemf.o main.o \
				  $(OBJS)

CFLAGS	+= -DWEBS -DUEMF -DOS="LINUX" -DLINUX $(UMSW) $(DASW) $(SSLSW) $(IFMODSW)  -DSINGLE_USER_PASSWORD
# dino 2004/10/29
# CFLAGS  += -D__NO_CGI_BIN   -I$(ROOTDIR)/include    -I$(ROOTDIR)/$(LINUXDIR)/include
#CFLAGS  += -I. -D__NO_CGI_BIN   -I$(ROOTDIR)/include    -I$(ROOTDIR)/$(LINUXDIR)/include  -I./web_server  -I./web_server/model/data   -I./web_server/model
# dino

# Dino Chang 2004/12/06
CFLAGS  += -I. -D__NO_CGI_BIN  -I$(ROOTDIR)/include    -I$(ROOTDIR)/$(LINUXDIR)/include  -I./web_server  -I./web_server/model/data   -I./web_server/model -I../../TMSS/src/include/ -I../../TMSS/src/common/include

TMLIB = -L../../TMSS/src/lib -ltmClientInfo -ltmDpcomposer -ltmRequest \
        -ltmIniParser -ltmGetAUInfo -ltmURLFilter -ltmExport -ltmcfg \
        -ltmMd5Check
# Dino
#-DWEBS_PAGE_ROM


OTHERS	= -DB_STATS -DB_FILL -DDEBUG
DEBUG	+= -g -Wall
IFLAGS 	= -I..  -I.
# Dino Chang 2004/12/06
# LDFLAGS       += $(SSLLIB) $(IFMODLIB)
LDFLAGS += $(SSLLIB) $(IFMODLIB) $(TMLIB)
# Dino
#LDFLAGS  += $(CFLAGS) -Wl,-elf2flt="-z"  

compile: $(NAME)

#
#	Build archive of objects
#
$(ARCH): $(DEPEND_FILES)
	$(AR) $(ARFLAGS) $(ARCH) $?

DEP_FILES = $(patsubst %.o,%.d,$(DEPEND_FILES) main.o)

dep: $(DEP_FILES)
	find .. -name "*.d" | xargs cat >.depend




#
#	Primary link
#
$(NAME): Makefile $(ARCH)
	$(CC) -o $(NAME) $(DEBUG) \
		$(CFLAGS) $(IFLAGS) \
		main.o $(ARCH) $(LDFLAGS) $(LDLIBS) 

clean:
# dino 2004/10/29
	rm -f $(NAME) $(ARCH) $(OBJS) .depend
	find ../ -name "*.o" | xargs rm -f
	find ../ -name "*.gdb" | xargs rm -f  
	find ../ -name "*.d" | xargs rm -f  


#
#	Dependencies
#

# dino 2004/10/29
$(OBJS) : \
	./web_server/model/ipport.h\
	./web_server/http.h\
	./web_server/model/data/htmlcgi.h\
	./web_server/model/data/hpfiles.h\
	./web_server/model/data/hpvars.h\
	./web_server/model/cfg.h\
	./web_server/model/fw_ver.h\
	./web_server/model/str.h\
# dino
../asp.o:  ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../balloc.o: ../balloc.c ../uemf.h

../base64.o:  ../base64.c ../webs.h ../wsIntrn.h  ../ej.h ../ejIntrn.h ../uemf.h

../cgi.o:  ../webs.h ../wsIntrn.h ../uemf.h

../default.o:  ../default.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../ejlex.o:  ../ejlex.c ../ej.h ../ejIntrn.h ../uemf.h

../ejparse.o:  ../ejparse.c ../ej.h ../ejIntrn.h ../uemf.h

../emfdb.o:  ../emfdb.h ../wsIntrn.h ../uemf.h

../form.o:  ../form.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../h.o:  ../h.c ../uemf.h

../handler.o:  ../handler.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../md5c.o:  ../md5.h ../wsIntrn.h ../uemf.h

../mime.o:  ../mime.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../misc.o:  ../misc.c ../uemf.h

../page.o:  ../page.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../ringq.o:  ../ringq.c ../uemf.h

../rom.o:  ../rom.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../security.o:  ../security.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../sock.o:  ../sock.c ../uemf.h

../sockGen.o:  ../sockGen.c ../uemf.h

../sym.o:  ../sym.c ../uemf.h

../uemf.o:  ../uemf.c ../uemf.h

../um.o:  ../webs.h ../wsIntrn.h ../um.h ../uemf.h

../umui.o:  ../webs.h ../wsIntrn.h ../um.h ../uemf.h

../url.o:  ../url.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../value.o:  ../value.c ../uemf.h

../webrom.o:  ../webrom.c ../webs.h ../wsIntrn.h ../uemf.h

../webs.o:  ../webs.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../websda.o:  ../webs.h ../wsIntrn.h ../websda.h ../uemf.h

../websuemf.o:  ../websuemf.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../websSSL.o:  ../websSSL.c ../websSSL.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

main.o:  main.c ../wsIntrn.h ../webs.h ../ej.h ../ejIntrn.h ../uemf.h

#
#	Transition rules (add -o to put object in right directory)
#
.c.o:
	$(CC) -c -o $@ $(DEBUG) $(CFLAGS) $(IFLAGS) $<
%.d: %.c
	$(CC) -E $(CFLAGS) $(EXTRACFLAGS) -Wp,-MD,$*.d $< >/dev/null

-include .depend

