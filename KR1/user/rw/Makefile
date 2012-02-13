EXEC1 = x
OBJS1 = x.o 

EXEC2 = w
OBJS2 = w.o

EXEC3 = ctl
OBJS3 = ctl.o

ifdef CONFIG_UCLINUX
BINDIR = /bin
else
BINDIR = /sbin
endif
ifdef CONSOLE_BAUD_RATE
CFLAGS += -DCONSOLE_BAUD_RATE=$(CONSOLE_BAUD_RATE)
endif

ifdef CONFIG_LIB_UC_LIBC_TIMEZONE
CFLAGS += -DINCLUDE_TIMEZONE
endif

all: $(EXEC1) $(EXEC2) $(EXEC3)

$(EXEC1): $(OBJS1)
	$(CC) $(LDFLAGS) -o $@ $(OBJS1) $(LIBCRYPT) $(LDLIBS$(LDLIBS-$(@)))

$(EXEC2): $(OBJS2)
	$(CC) $(LDFLAGS) -o $@ $(OBJS2) $(LDLIBS$(LDLIBS-$(@)))

$(EXEC3): $(OBJS3)
	$(CC) $(LDFLAGS) -o $@ $(OBJS3) $(LDLIBS$(LDLIBS-$(@)))

romfs:
	$(ROMFSINST)  /bin/$(EXEC2)
	$(ROMFSINST)  /bin/$(EXEC1)
	$(ROMFSINST)  /bin/$(EXEC3)

clean:
	rm -f simpleinit $(EXEC1) $(EXEC2) *.elf *.gdb *.o

