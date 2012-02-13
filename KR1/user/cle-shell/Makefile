
CLESHELL = cleshell
CLESHELLOBJS = cleshell.o


all: $(CLESHELL)

$(CLESHELL): $(CLESHELLOBJS)
	$(CC) $(LDFLAGS) -o $@ $(CLESHELLOBJS) $(LDLIBS$(LDLIBS-$(@)))

romfs:
	$(ROMFSINST) -e CONFIG_RTL865X_CLE       /cleshell

clean:
	-rm -f $(CLESHELL) *.elf *.gdb *.o

