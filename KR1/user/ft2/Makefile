KERNEL_HEADERS=-I$(ROOTDIR)/linux-2.4.x

EXEC1 = veri
OBJS1 =	veri.o

EXEC2 = tabparse
OBJS2 =	tabparse.o

EXEC3 = testiram
OBJS3 =	testiram.o

EXEC4 = testdram
OBJS4 =	testdram.o

ifdef CONFIG_UCLINUX
BINDIR = /bin
else
BINDIR = /sbin
endif

all: $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC4)

$(EXEC1): $(OBJS1)
	$(CC) $(LDFLAGS) -o $@ $(OBJS1) $(LDLIBS$(LDLIBS-$(@)))

$(EXEC2): $(OBJS2)
	$(CC) $(LDFLAGS) -o $@ $(OBJS2) $(LDLIBS$(LDLIBS-$(@)))

$(EXEC3): $(OBJS3)
	$(CC) $(LDFLAGS) -o $@ $(OBJS3) $(LDLIBS$(LDLIBS-$(@)))

$(EXEC4): $(OBJS4)
	$(CC) $(LDFLAGS) -o $@ $(OBJS4) $(LDLIBS$(LDLIBS-$(@)))

romfs:
	$(ROMFSINST)    /bin/$(EXEC1)
	$(ROMFSINST)    /bin/$(EXEC2)
	$(ROMFSINST)    /bin/$(EXEC3)
	$(ROMFSINST)    /bin/$(EXEC4)

clean:
	rm -f $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC4) *.elf *.gdb *.o

