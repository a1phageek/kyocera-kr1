
EXEC1 = pptp
OBJS1 = pptp.o pptp_gre.o ppp_fcs.o pty.o pptp_ctrl.o dirutil.o \
	vector.o inststr.o version.o 

### DEPS
### pptp_callmgr.h pptp_gre.h ppp_fcs.h pty.h util.h

EXEC2 = pptp_callmgr
OBJS2 = pptp_callmgr.o pptp_ctrl.o dirutil.o vector.o \
	version.o

### DEPS
### pptp_callmgr.h pptp_ctrl.h dirutil.h pptp_msg.h vector.h
 

CFLAGS += '-DPPPD_BINARY="/bin/pppd"' '-DPPTP_LINUX_VERSION="1.0.2"'
FLTFLAGS += -s 8192

ifdef BUILD_NETtel
CFLAGS += -DCONFIG_NETtel
endif

all:		$(EXEC1) $(EXEC2)

$(EXEC1):	$(OBJS1)
		$(CC) $(LDFLAGS) -o $@ $(OBJS1) $(LDLIBS)

$(EXEC2):	$(OBJS2)
		$(CC) $(LDFLAGS) -o $@ $(OBJS2) $(LDLIBS)

romfs:
#	$(ROMFSINST) -e CONFIG_USER_PPTP_CLIENT_PPTP /bin/pptp
#	$(ROMFSINST) -e CONFIG_USER_PPTP_CLIENT_PPTP_CALLMGR /bin/pptp_callmgr
#	$(ROMFSINST) -e  CONFIG_RTL865X_PPTPL2TP /bin/pptp
#	$(ROMFSINST) -e  CONFIG_RTL865XB_PPTPL2TP /bin/pptp
#	$(ROMFSINST) -e  CONFIG_RTL865X_PPTPL2TP /bin/pptp_callmgr
#	$(ROMFSINST) -e  CONFIG_RTL865XB_PPTPL2TP /bin/pptp_callmgr
	@if egrep "^#define CONFIG_RTL865X_PPTPL2TP 1" ../../linux-2.4.x/include/linux/autoconf.h > /dev/null; then \
	$(ROMFSINST) $(EXEC1) /bin/pptp; \
	$(ROMFSINST) $(EXEC2) /bin/pptp_callmgr; \
	fi

	@if egrep "^#define CONFIG_RTL865XB_PPTPL2TP 1" ../../linux-2.4.x/include/linux/autoconf.h > /dev/null; then \
	$(ROMFSINST) $(EXEC1) /bin/pptp; \
	$(ROMFSINST) $(EXEC2) /bin/pptp_callmgr; \
	fi


clean:
		-rm -f $(EXEC1) $(EXEC2) *.elf *.gdb *.o

