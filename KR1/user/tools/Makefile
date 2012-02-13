
EXEC1 = reset
OBJS1 = reset.o

EXEC2 = dw
OBJS2 =	dw.o

EXEC3 = ew
OBJS3 =	ew.o

ifdef CONFIG_UCLINUX
BINDIR = /bin
else
BINDIR = /sbin
endif

all: $(EXEC1) $(EXEC2) $(EXEC3)

$(EXEC1): $(OBJS1)
	$(CC) $(LDFLAGS) -o $@ $(OBJS1) $(LDLIBS$(LDLIBS-$(@)))

$(EXEC2): $(OBJS2)
	$(CC) $(LDFLAGS) -o $@ $(OBJS2) $(LDLIBS$(LDLIBS-$(@)))

$(EXEC3): $(OBJS3)
	$(CC) $(LDFLAGS) -o $@ $(OBJS3) $(LDLIBS$(LDLIBS-$(@)))

romfs:
	$(ROMFSINST)    $(BINDIR)/$(EXEC1)
	$(ROMFSINST)    $(BINDIR)/$(EXEC2)
	$(ROMFSINST)    $(BINDIR)/$(EXEC3)

clean:
	rm -f $(EXEC1) $(EXEC2) $(EXEC3) *.elf *.gdb *.o

